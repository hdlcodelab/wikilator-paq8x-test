# ------------------------------------------------------------
# Makefile for the wikilator‑paq8x‑test project
# ------------------------------------------------------------

# ------------------------------------------------------------------
# Configuration
# ------------------------------------------------------------------
CXX      := g++                     # compiler
CXXSTD   := -std=c++17              # language standard
CXXWARN  := -Wall -Wextra -Werror   # turn warnings into errors
CXXOPT   := -O2                     # optimisation level
CXXFLAGS := $(CXXSTD) $(CXXWARN) $(CXXOPT)

# If you need extra include directories, add them here, e.g.
# CXXFLAGS += -I/usr/local/include

LDFLAGS  :=                       # linker flags (e.g. -pthread, -lm …)

# ------------------------------------------------------------------
# Files
# ------------------------------------------------------------------
TARGET   := wikilator-paq8x-test
SRC      := wikilator-paq8x-test.cpp
OBJ      := $(SRC:.cpp=.o)

# ------------------------------------------------------------------
# Phony targets
# ------------------------------------------------------------------
.PHONY: all clean

# ------------------------------------------------------------------
# Build everything
# ------------------------------------------------------------------
all: $(TARGET)

$(TARGET): $(OBJ)
	@echo "Linking $@ …"
	$(CXX) $(LDFLAGS) -o $@ $^

# ------------------------------------------------------------------
# Compile each .cpp → .o
# ------------------------------------------------------------------
$(OBJ): %.o : %.cpp
	@echo "Compiling $< → $@ …"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ------------------------------------------------------------------
# Clean up
# ------------------------------------------------------------------
clean:
	@echo "Removing objects and binary…"
	$(RM) $(OBJ) $(TARGET)

# ------------------------------------------------------------------
# Show the variables (helpful when you’re stuck)
# ------------------------------------------------------------------
print-%:
	@echo '$* = $($*)'
