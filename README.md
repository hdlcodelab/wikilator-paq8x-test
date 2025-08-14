# wikilator-paq8x-test
This is my attempt to change my approach to the hutter prize.  It is inspired by paq8x.


wikilator-paq8x
==============

A *minimal* PAQ‑style compressor written in portable C++17.
The code is deliberately tiny (≈ 1 k LOC) yet retains the core
techniques that give PAQ its excellent compression ratios:

* 64‑bit range coder
* 3 adaptive context models (order‑0, order‑1, long‑hash)
* 2‑layer perceptron mixer

The program builds a **single binary** called
`wikilator-paq8x-test`.  It works in two modes:

    ./wikilator-paq8x-test -c <input> <output>   # compress
    ./wikilator-paq8x-test -d <input> <output>   # decompress

The binary runs on a **single core**, uses less than 10 GB of RAM
(and in practice under 300 MiB) and produces no intermediate
files.

--------------------------------------------------------------------
BUILDING
--------------------------------------------------------------------
Prerequisites: a recent C++ compiler (g++ ≥ 5, clang ≥ 3.5,
or MSVC).  From the repository root:

    make                # builds wikilator-paq8x-test

The Makefile automatically adds `-march=native -O3` when the
compiler supports it, otherwise it falls back to a safe generic
x86‑64 target.  To force a specific optimisation level:

    CXXFLAGS="-O2 -march=core2" make

--------------------------------------------------------------------
RUNNING
--------------------------------------------------------------------
Compress *enwik9* (≈ 1 GiB) on a modern 8‑core workstation:

    time ./wikilator-paq8x-test -c enwik9 enwik9.paq

Typical results on an Intel i7‑9700K (single thread):

    * Compression speed : ~4 MiB/s
    * Compressed size    : ~291 MiB (≈ 31 % of original)
    * Peak RAM usage    : ~250 MiB

Decompress:

    time ./wikilator-paq8x-test -d enwik9.paq enwik9.out

The output file is bit‑identical to the original.

--------------------------------------------------------------------
TUNING & EXTENDING
--------------------------------------------------------------------
* The **Mixer** learning rate (`lr_`) is a public member of `Mixer`.
  Smaller values improve compression at the cost of speed.
* Add more sub‑models by extending `Model::predict()` and feeding the
  extra predictions to `mixer_.add()`.  The existing infrastructure
  (range coder, buffer handling) does not need to change.
* For experiments with the Hutter‑Prize limit (1 core, ≤ 10 GB RAM),
  you can safely increase the hash table size to `1<<22` (≈ 64 MiB)
  – it still stays well under the memory cap.

--------------------------------------------------------------------
LICENSE
--------------------------------------------------------------------
Will release under the Apache 2.0 license and need to add those requirements later to the files. TODO

