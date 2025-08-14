# -------------------- Makefile -----------------------------------------
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O3 -ffast-math -funroll-loops
LDFLAGS  := -static   # optional: produces a single binary

# Detect CPU – if gcc supports -march=native we use it,
# otherwise fall back to a safe generic set.
ifeq ($(shell $(CXX) -march=native -Q --help=target | grep -c "enabled"),1)
	CXXFLAGS += -march=native
else
	CXXFLAGS += -march=x86-64   # generic x86‑64
endif

# Enable Link Time Optimization if the compiler knows it
ifneq (,$(findstring -flto,$(CXXFLAGS)))
	CXXFLAGS += -flto
endif

OBJS := compress.o decompress.o

# The single binary – the name required by the description
wikilator-paq8x-test: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

compress.o: compress.cpp common.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

decompress.o: decompress.cpp common.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o wikilator-paq8x-test

.PHONY: clean
