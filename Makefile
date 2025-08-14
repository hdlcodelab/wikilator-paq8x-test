make
# --------------------------------------------------------------
# Makefile for wikilator-paq8x-test
# --------------------------------------------------------------

CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic
LDFLAGS  :=
SRC      := wikilator-paq8x-test.cpp
OBJ      := $(SRC:.cpp=.o)
TARGET   := wikilator-paq8x-test

# Default target
all: $(TARGET)

# Link the final binary
$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

# Compile .cpp → .o
%.o: %.cpp common.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
.PHONY: clean
clean:
	$(RM) $(OBJ) $(TARGET)

# Convenience target to run a quick test (optional)
.PHONY: test
test: $(TARGET)
	@echo "Running a quick compress‑decompress sanity check..."
	@echo "Hello, PAQ8X!" > sample.txt
	@./$(TARGET) c sample.txt sample.paq
	@./$(TARGET) d sample.paq sample.out
	@cmp -s sample.txt sample.out && echo "PASS: round‑trip identical" || echo "FAIL: files differ"
	@$(RM) sample.txt sample.paq sample.out
