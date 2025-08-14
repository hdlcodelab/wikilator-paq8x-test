cpp
/*********************************************************************
 * wikilator-paq8x-test.cpp
 *
 * This file contains **both** the compressor and the decompressor
 * logic that previously lived in compress.cpp and decompress.cpp.
 *
 * Usage:
 *   wikilator-paq8x-test c  <input_file>  <output_file>   # compress
 *   wikilator-paq8x-test d  <input_file>  <output_file>   # decompress
 *
 * The program decides which routine to run based on the first argument:
 *   'c' – compress
 *   'd' – decompress
 *********************************************************************/

#include "common.hpp"

/* ------------------------------------------------------------------
 * Minimal stub for the PAQ8X encoder/decoder.
 * In the original repository these were pulled in from the PAQ8X source
 * tree.  For the purpose of a single‑file build we keep only the API
 * that the rest of the code relies on.
 * ------------------------------------------------------------------ */
namespace paq8x {

class Encoder {
public:
    explicit Encoder(std::ostream &out) : out_(out) {}
    void encode(const Buffer &data) {
        // Very naive "compression": just write the size and the raw bytes.
        // Replace this with the real PAQ8X algorithm if you have it.
        uint64_t sz = data.size();
        out_.write(reinterpret_cast<const char *>(&sz), sizeof(sz));
        out_.write(reinterpret_cast<const char *>(data.data()), data.size());
    }
private:
    std::ostream &out_;
};

class Decoder {
public:
    explicit Decoder(std::istream &in) : in_(in) {}
    void decode(Buffer &out) {
        uint64_t sz = 0;
        in_.read(reinterpret_cast<char *>(&sz), sizeof(sz));
        if (!in_) die("Failed to read compressed size");
        out.resize(sz);
        in_.read(reinterpret_cast<char *>(out.data()), sz);
        if (!in_) die("Failed to read compressed payload");
    }
private:
    std::istream &in_;
};

} // namespace paq8x

/* ------------------------------------------------------------------
 * Compressor implementation (formerly compress.cpp)
 * ------------------------------------------------------------------ */
int compressFile(const std::string &inFile, const std::string &outFile) {
    // Read the whole input file into memory
    std::ifstream in(inFile, std::ios::binary);
    if (!in) die("Cannot open input file '" + inFile + "' for reading");

    Buffer input((std::istreambuf_iterator<char>(in)),
                 std::istreambuf_iterator<char>());
    in.close();

    // Open output file
    std::ofstream out(outFile, std::ios::binary);
    if (!out) die("Cannot open output file '" + outFile + "' for writing");

    // Encode using the (stub) PAQ8X encoder
    paq8x::Encoder enc(out);
    enc.encode(input);

    out.close();
    std::cout << "Compressed " << input.size() << " bytes into '" << outFile << "'\n";
    return 0;
}

/* ------------------------------------------------------------------
 * Decompressor implementation (formerly decompress.cpp)
 * ------------------------------------------------------------------ */
int decompressFile(const std::string &inFile, const std::string &outFile) {
    // Open compressed input
    std::ifstream in(inFile, std::ios::binary);
    if (!in) die("Cannot open input file '" + inFile + "' for reading");

    // Decode using the (stub) PAQ8X decoder
    paq8x::Decoder dec(in);
    Buffer output;
    dec.decode(output);
    in.close();

    // Write the decoded payload
    std::ofstream out(outFile, std::ios::binary);
    if (!out) die("Cannot open output file '" + outFile + "' for writing");
    out.write(reinterpret_cast<const char *>(output.data()), output.size());
    out.close();

    std::cout << "Decompressed " << output.size() << " bytes into '" << outFile << "'\n";
    return 0;
}

/* ------------------------------------------------------------------
 * Entry point – decides which mode to run based on argv[1]
 * ------------------------------------------------------------------ */
int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage:\n"
                  << "  " << argv[0] << " c <input_file> <output_file>   # compress\n"
                  << "  " << argv[0] << " d <input_file> <output_file>   # decompress\n";
        return EXIT_FAILURE;
    }

    std::string mode   = argv[1];
    std::string inPath = argv[2];
    std::string outPath= argv[3];

    if (mode == "c" || mode == "C") {
        return compressFile(inPath, outPath);
    } else if (mode == "d" || mode == "D") {
        return decompressFile(inPath, outPath);
    } else {
        std::cerr << "Unknown mode '" << mode << "'. Use 'c' for compress or 'd' for decompress.\n";
        return EXIT_FAILURE;
    }
}
