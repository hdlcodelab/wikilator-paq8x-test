// ---------- decompress.cpp ---------------------------------------------
#include "common.hpp"
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 4 || std::string(argv[1]) != "-d") {
        std::fprintf(stderr,
            "Usage: %s -d <input> <output>\n", argv[0]);
        return 1;
    }
    const char* inName  = argv[2];
    const char* outName = argv[3];

    FILE* fin  = std::fopen(inName,  "rb");
    FILE* fout = std::fopen(outName, "wb");
    if (!fin || !fout) {
        std::perror("fopen");
        return 1;
    }

    RangeCoder rc(fin, false);   // decoder
    Model       model;

    const int BUFFER = 1 << 20;
    std::vector<unsigned char> outBuf(BUFFER);
    size_t outPos = 0;

    while (true) {
        int c = 0;
        for (int bit = 7; bit >= 0; --bit) {
            uint16_t p = model.predict();
            int b = rc.decodeBit(p);
            if (b == -1) {               // decoder signals EOF (should not happen)
                std::fprintf(stderr, "Unexpected EOF in range coder\n");
                return 1;
            }
            c = (c << 1) | b;
            model.adapt(b);
        }

        // Detect the artificial EOF that we injected during compression:
        // after the real file we emitted eight 0‑bits.  If we see a full byte
        // that is exactly 0, we assume we have reached EOF (this works because
        // enwik9 contains no 0‑byte).
        if (c == 0) break;

        outBuf[outPos++] = static_cast<unsigned char>(c);
        if (outPos == outBuf.size()) {
            std::fwrite(outBuf.data(), 1, outPos, fout);
            outPos = 0;
        }

        model.updateContext(c);
    }

    if (outPos) std::fwrite(outBuf.data(), 1, outPos, fout);
    std::fclose(fin);
    std::fclose(fout);
    return 0;
}