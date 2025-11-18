# src/Makefile

# Compiler
CXX := g++
CXXFLAGS := -std=c++17 -Wall -O2

.PHONY: all single clean help

# Default: compile all .cpp files in src/ and output executables in parent directory
all:
	@for f in *.cpp; do \
		name=$$(basename $$f .cpp); \
		echo "Compiling $$f -> ../$$name"; \
		$(CXX) $(CXXFLAGS) $$f -o ../$$name; \
	done

# Compile a single file
# Usage: make single FILE=foo.cpp
single:
ifdef FILE
	@name=$(basename $(FILE) .cpp); \
	echo "Compiling $(FILE) -> ../$$name"; \
	$(CXX) $(CXXFLAGS) $(FILE) -o ../$$name
else
	$(error Please provide FILE=filename.cpp)
endif

# Optional: clean executables in parent directory
clean:
	@for f in *.cpp; do \
		name=$$(basename $$f .cpp); \
		if [ -f ../$$name ]; then rm ../$$name; fi; \
	done

# Help target
help:
	@echo "Usage:"
	@echo "  make all                  Compile all .cpp files in src/, output to parent dir"
	@echo "  make single FILE=foo.cpp  Compile single file, executable goes to parent dir"
	@echo "  make clean                Remove executables in parent directory"
