# ==========================================================
# Main Makefile
# ==========================================================

# Compiler and flags
CXX      = g++
CXXFLAGS = -Wall -O2 -std=c++17

# Directories
SRC_DIR       = src
GEN_DIR       = $(SRC_DIR)/generators
BUILD_DIR     = generators
DATA_DIR      = data
DATA_SAVE_DIR = data-save
PIC_DIR       = export-pictures
PIC_JPEG_DIR  = $(PIC_DIR)/jpeg
PIC_PPM_DIR   = $(PIC_DIR)/ppm

# Source files
MAIN_SRC     = $(SRC_DIR)/main.cpp
GEN_SOURCES  = $(wildcard $(GEN_DIR)/*.cpp)
GEN_TARGETS  = $(patsubst $(GEN_DIR)/%.cpp,$(BUILD_DIR)/%,$(GEN_SOURCES))

# ==========================================================
# Default rule
# ==========================================================
all: main gen-julia

# ==========================================================
# Compile main
# ==========================================================
main: $(MAIN_SRC)
	@echo "Compiling main..."
	$(CXX) $(CXXFLAGS) -o main $(MAIN_SRC)

# ==========================================================
# Compile al generator files
# ==========================================================
$(BUILD_DIR)/%: $(GEN_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	@echo "Compiling generator $< -> $@"
	$(CXX) $(CXXFLAGS) -o $@ $<

gen-julia: $(GEN_TARGETS)
	@echo "All generators compiled in $(BUILD_DIR)/"

# ==========================================================
# Compile single source file
# Usage: make FILE=src/foo.cpp
# ==========================================================
:
ifndef FILE
	$(error "Please provide FILE=src/yourfile.cpp")
endif
	@echo "Compiling single file $(FILE)..."
	$(CXX) $(CXXFLAGS) -o $(notdir $(FILE:.cpp=)) $(FILE)

# ==========================================================
# Setup directories
# ==========================================================
setup:
	@mkdir -p $(DATA_DIR) $(DATA_SAVE_DIR) $(PIC_JPEG_DIR) $(PIC_PPM_DIR)
	@echo "Created directories: $(DATA_DIR), $(DATA_SAVE_DIR), $(PIC_JPEG_DIR), $(PIC_PPM_DIR)"

# ==========================================================
# Install dependencies
# ==========================================================
deps:
	@echo "Installing dependencies..."
	# ImageMagick
	sudo-g5k apt install imagemagick
	@echo "Dependencies installed"

# ==========================================================
# Initialize project: setup + deps + build all
# ==========================================================
init: setup deps all
	@echo "Project initialized successfully!"

# ==========================================================
# Cleaning
# ==========================================================
clean:
	@echo "Cleaning project..."
	rm -f main
	rm -rf $(BUILD_DIR)/*

clean-data:
	@read -p "Are you sure to delete all files in $(DATA_DIR)/ ? [y/N] " ans; \
	if [ "$$ans" = "y" ] || [ "$$ans" = "Y" ]; then \
		rm -f $(DATA_DIR)/*; \
		echo "Deleted all files in $(DATA_DIR)/"; \
	else \
		echo "Abort"; \
	fi

clean-pictures:
	@read -p "Are you sure to delete all files in $(PIC_DIR)/ ? [y/N] " ans; \
	if [ "$$ans" = "y" ] || [ "$$ans" = "Y" ]; then \
		rm -f $(PIC_JPEG_DIR)/*; \
		rm -f $(PIC_PPM_DIR)/*; \
		echo "Deleted all files in $(PIC_DIR)/"; \
	else \
		echo "Abort"; \
	fi

clean-all: clean-data clean-pictures

# ==========================================================
# Save file
# Usage: make save FILE=xxx.csv
# ==========================================================
save:
ifndef FILE
	$(error "Please provide FILE=filename.csv")
endif
	@mkdir -p $(DATA_SAVE_DIR)
	@mv $(FILE) $(DATA_SAVE_DIR)/
	@echo "Moved $(FILE) -> $(DATA_SAVE_DIR)/"

# ==========================================================
# Help
# ==========================================================
help:
	@echo "Available make commands:"
	@echo "  make all                Compile all sources in src/, executables named after source files"
	@echo "  make init               Setup, install dependencies, and build all"
	@echo "  make setup              Create necessary directories (data, data-save, export-pictures/jpeg, export-pictures/ppm)"
	@echo "  make deps               Install dependencies"
	@echo "  make gen-julia          Compile all sources in src/generators/ in generators/, executable named after source files"
	@echo "  make FILE=src/foo.cpp Compile single source file; executable named after the file"
	@echo "  make clean              Remove main and generator binaries"
	@echo "  make clean-data         Delete all files in data/ (asks for confirmation)"
	@echo "  make clean-pictures     Delete all files in export-pictures/ (asks for confirmation)"
	@echo "  make clean-all          Delete all files in data and export-pictures/ (asks for confirmation)"
	@echo "  make save FILE=xxx.csv  Move FILE to data-save/"
	@echo "  make help               Show this help message"

# ==========================================================
# Declarations
# ==========================================================
.PHONY: all main gen-julia s clean clean-data clean-pictures clean-all save help
