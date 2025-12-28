# ==========================================================
# Main Makefile
# ==========================================================

# Compiler and flags
CXX      = g++
CXXFLAGS = -Wall -O2 -std=c++17

# Directories
SRC_DIR       = src
GEN_DIR       = $(SRC_DIR)/generators
GEN_BUILD_DIR = generators
DATA_DIR      = data
DATA_SAVE_DIR = data-save
PIC_DIR       = export-pictures
PIC_JPEG_DIR  = $(PIC_DIR)/jpeg
PIC_PPM_DIR   = $(PIC_DIR)/ppm

# Source files
# All .cpp files inside src/
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
SRC_TARGETS = $(patsubst $(SRC_DIR)/%.cpp,%,$(SRC_FILES))

GEN_SOURCES  = $(wildcard $(GEN_DIR)/*.cpp)
GEN_TARGETS  = $(patsubst $(GEN_DIR)/%.cpp,$(GEN_BUILD_DIR)/%,$(GEN_SOURCES))

# ==========================================================
# Compile ALL sources in src/ (excluding generators)
# ==========================================================

%: $(SRC_DIR)/%.cpp
	@echo "Compilation de $< -> $@"
	$(CXX) $(CXXFLAGS) -o $@ $<

src: $(SRC_TARGETS)
	@echo "Tous les fichiers sources dans $(SRC_DIR)/ ont été compilés."

# ==========================================================
# Compile all generators
# ==========================================================
gen-julia: $(GEN_TARGETS)
	@echo "All generators compiled in $(GEN_BUILD_DIR)/"

$(GEN_BUILD_DIR)/%: $(GEN_DIR)/%.cpp
	@mkdir -p $(GEN_BUILD_DIR)
	@echo "Compiling generator $< -> $@"
	$(CXX) $(CXXFLAGS) -o $@ $<

# ==========================================================
# Compile a single file
# Usage: make FILE=path/foo.cpp
# ==========================================================
single:
ifndef FILE
	$(error "Please provide FILE=src/yourfile.cpp")
endif
	@echo "Compiling single file $(FILE)..."
	$(CXX) $(CXXFLAGS) -o $(basename $(notdir $(FILE))) $(FILE)

# ==========================================================
# Setup directories
# ==========================================================
setup:
	@mkdir -p $(DATA_DIR) $(DATA_SAVE_DIR) $(PIC_JPEG_DIR) $(PIC_PPM_DIR)
	@echo "Created directories."

# ==========================================================
# Install dependencies
# ==========================================================
deps:
	@echo "Installing dependencies..."
	sudo-g5k apt install imagemagick
	pip install scikit-image
	@echo "Dependencies installed."

# ==========================================================
# Init project
# ==========================================================
init: setup deps all

# ==========================================================
# Cleaning
# ==========================================================
clean:
	@echo "Cleaning project..."
	rm -f main
	rm -rf $(GEN_BUILD_DIR)/*

clean-data:
	@read -p "Delete ALL files in $(DATA_DIR)/ ? [y/N] " ans; \
	if [ "$$ans" = "y" ] || [ "$$ans" = "Y" ]; then \
		rm -f $(DATA_DIR)/*; \
		echo "Deleted."; \
	else echo "Abort."; fi

clean-pictures:
	@read -p "Delete ALL pictures in $(PIC_DIR)/ ? [y/N] " ans; \
	if [ "$$ans" = "y" ] || [ "$$ans" = "Y" ]; then \
		rm -f $(PIC_JPEG_DIR)/*; \
		rm -f $(PIC_PPM_DIR)/*; \
		echo "Deleted."; \
	else echo "Abort."; fi

clean-all: clean-data clean-pictures

# ==========================================================
# Convert PPM to JPEG
# Usage: make convert FILE=path/filename.ppm
# ==========================================================
convert:
ifndef FILE
	$(error "Please provide FILE=path/name_of_file.ppm")
endif
	@mkdir -p export-pictures/jpeg
	@echo "Converting $(FILE) to export-pictures/jpeg/... in JPEG"
	@convert $(FILE) export-pictures/jpeg/$(basename $(notdir $(FILE))).jpeg
	@echo "Saved as export-pictures/jpeg/$(basename $(notdir $(FILE))).jpeg"


# ==========================================================
# Help
# ==========================================================
help:
	@echo "Available make commands:"
	@echo ""
	@echo "  make src                        Compile all sources in src/ into executables (same name as .cpp)"
	@echo "  make gen-julia                  Compile all generators in src/generators/ into generators/"
	@echo "  make single FILE=path/foo.cpp   Compile a single C++ file into an executable named after the file"
	@echo "  make setup                      Create required directories"
	@echo "  make deps                       Install dependencies"
	@echo "  make init                       setup + deps + compile everything"
	@echo "  make clean                      Remove main binaries and generator binaries"
	@echo "  make clean-data                 Delete all files in data/ (confirmation required)"
	@echo "  make clean-pictures             Delete all exported pictures (confirmation required)"
	@echo "  make clean-all                  clean-data + clean-pictures"
	@echo "  make convert FILE=path/xxx.ppm  Convert PPM file to JPEG in export-pictures/jpeg/ folder"


.PHONY: all gen-julia single clean clean-data clean-pictures clean-all save help
