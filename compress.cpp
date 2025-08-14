// ---------- compress.cpp -----------------------------------------------
#include "common.hpp"
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 4 || std::string(argv[1]) != "-c") {
        std::fprintf(stderr,
            "Usage: %s -c <input> <output>\n", argv[0]);
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

    RangeCoder rc(fout, true);   // encoder
    Model       model;

    const int BUFFER = 1 << 20;           // 1 MiB I/O buffer
    std::vector<unsigned char> buf(BUFFER);
    while (true) {
        size_t got = std::fread(buf.data(), 1, BUFFER, fin);
        if (got == 0) break;
        for (size_t i = 0; i < got; ++i) {
            int c = buf[i];
            model.updateContext(c);
            for (int bit = 7; bit >= 0; --bit) {
                uint16_t p = model.predict();          // 0..4095
                int b = (c >> bit) & 1;
                rc.encodeBit(b, p);
                model.adapt(b);
            }
        }
    }

    // Encode EOF marker (a single 0‑bit with p=0.5 is sufficient)
    model.updateContext(-1);
    for (int i = 0; i < 8; ++i) {
        uint16_t p = model.predict();
        rc.encodeBit(0, p);
        model.adapt(0);
    }

    rc.flush();
    std::fclose(fin);
    std::fclose(fout);
    return 0;
}
