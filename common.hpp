// ---------- common.hpp -------------------------------------------------
#pragma once
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <array>
#include <vector>
#include <algorithm>
#include <type_traits>

// ---------------------------------------------------------------------
// 1.  Range coder (same class works for both encoder & decoder)
// ---------------------------------------------------------------------
class RangeCoder {
public:
    explicit RangeCoder(FILE* f, bool encode);
    ~RangeCoder();

    // encode a bit with probability p (p = 0..4095, i.e. 12‑bit fixed point)
    inline void encodeBit(int bit, uint16_t p);
    // decode a bit, returning 0 or 1; p must be identical to encoder's p
    inline int decodeBit(uint16_t p);

    // Flush pending bytes (only needed for encoder)
    void flush();

private:
    FILE* file_;
    bool  encode_;                // true → encoder, false → decoder
    uint64_t low_, range_;
    uint32_t cache_;              // for output buffering (encoder)
    uint64_t code_;                // only used by decoder
    // ... (implementation details are ~80 lines)
};

// ---------------------------------------------------------------------
// 2.  Simple 2‑layer mixer
// ---------------------------------------------------------------------
class Mixer {
public:
    explicit Mixer(size_t nContexts);
    // add predictor output (0..4095) for the i‑th context
    void add(uint16_t predictor);
    // produce final probability, also updates internal weights
    uint16_t get();               // returns 0..4095
    // called after each bit to adapt the weights
    void update(int bit);

private:
    size_t n_;                     // number of active contexts
    std::vector<float> w_;         // first‑layer weights (N)
    float                bias_;    // bias term
    float                lr_;      // learning rate (e.g. 0.001f)
};

// ---------------------------------------------------------------------
// 3.  Context models ----------------------------------------------------
class Model {
public:
    Model();
    // update all sub‑models with the latest byte (or -1 for EOF)
    void updateContext(int c);
    // produce a combined probability (0..4095) for the next bit
    uint16_t predict();
    // called after a bit is encoded/decoded
    void adapt(int bit);

private:
    // --- sub‑models ----------------------------------------------------
    uint32_t order0_[256];               // byte frequencies (0..65535)
    uint32_t order1_[256][256];          // byte‑pair frequencies
    // hash‑based long context
    struct Entry { uint64_t key; uint16_t cnt[2]; };
    static constexpr size_t HSIZE = 1 << 20;  // 1 MiB table (≈16 MiB)
    std::array<Entry, HSIZE> hashTable_;
    uint64_t rollingHash_;

    // mixer that combines the three sub‑models
    Mixer mixer_;
};
