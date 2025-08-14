cpp
#ifndef COMMON_HPP
#define COMMON_HPP

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

/* ------------------------------------------------------------------
 * Utility helpers used by both compressor and decompressor
 * ------------------------------------------------------------------ */
inline void die(const std::string &msg) {
    std::cerr << "Fatal error: " << msg << std::endl;
    std::exit(EXIT_FAILURE);
}

/* Buffer type used by the PAQ8X encoder/decoder */
using Byte = uint8_t;
using Buffer = std::vector<Byte>;

/* ------------------------------------------------------------------
 * Forward declarations – the implementation lives in the single .cpp file
 * ------------------------------------------------------------------ */
int  compressFile(const std::string &inFile, const std::string &outFile);
int  decompressFile(const std::string &inFile, const std::string &outFile);

#endif // COMMON_HPP
