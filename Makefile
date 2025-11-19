# Makefile

# Directories
SRC_DIR := src
DATA_DIR := data
SAVE_DIR := data-save
PICTURE_DIR := export-pictures

# Compiler
CXX := g++
CXXFLAGS := -std=c++17 -Wall -O2

.PHONY: all clean clean-data save single

# -----------------------------
# Build all sources
# -----------------------------
all:
	@for f in $(SRC_DIR)/*.cpp; do \
		name=$$(basename $$f .cpp); \
		echo "Compiling $$f -> $$name"; \
		$(CXX) $(CXXFLAGS) $$f -o $$name; \
	done

# -----------------------------
# Compile single source
# Usage: make single FILE=src/foo.cpp
# Output: foo
# -----------------------------
s:
ifdef FILE
	@name=$(basename $(notdir $(FILE))); \
	echo "Compiling $(FILE) -> $$name"; \
	$(CXX) $(CXXFLAGS) $(FILE) -o $$name
else
	$(error Please provide FILE=source.cpp)
endif

# -----------------------------
# Clean all data files with confirmation
# -----------------------------
clean-data:
	@echo "Warning: This will delete all files in $(DATA_DIR)/"
	@read -p "Are you sure? [y/N] " answer && \
	if [ "$$answer" = "y" ] || [ "$$answer" = "Y" ]; then \
		rm -f $(DATA_DIR)/*; \
		echo "All files in $(DATA_DIR) deleted."; \
	else \
		echo "Aborted."; \
	fi

# -----------------------------
# Clean all pictures files with confirmation
# -----------------------------
clean-pictures:
	@echo "Warning: This will delete all files in $(PICTURE_DIR)/"
	@read -p "Are you sure? [y/N] " answer && \
	if [ "$$answer" = "y" ] || [ "$$answer" = "Y" ]; then \
		rm -f $(PICTURE_DIR)/*; \
		echo "All files in $(PICTURE_DIR) deleted."; \
	else \
		echo "Aborted."; \
	fi


# -----------------------------
# Clean BOTH DATA & PICTURES with confirmation
# -----------------------------
clean-all:
	@echo "Warning: This will delete ALL files in:"
	@echo "  - $(DATA_DIR)/"
	@echo "  - $(PICTURE_DIR)/"
	@read -p "Are you sure? [y/N] " answer && \
	if [ "$$answer" = "y" ] || [ "$$answer" = "Y" ]; then \
		rm -f $(DATA_DIR)/* 2>/dev/null; \
		rm -f $(PICTURE_DIR)/* 2>/dev/null; \
		echo "All files deleted in both folders."; \
	else \
		echo "Aborted."; \
	fi

# -----------------------------
# Save a specific data file
# Usage: make save FILE=xxx.csv
# -----------------------------
save:
ifdef FILE
	@mkdir -p $(SAVE_DIR)
	@if [ -f "$(FILE)" ]; then \
		mv $(FILE) $(SAVE_DIR)/; \
		echo "Saved $(FILE) to $(SAVE_DIR)/"; \
	else \
		echo "File $(FILE) does not exist!"; \
	fi
else
	$(error Please provide FILE=filename.csv)
endif


# -----------------------------
# Help
# -----------------------------
help:
	@echo "Available make commands:"
	@echo "  make all                Compile all sources in $(SRC_DIR)/, executables named after source files"
	@echo "  make s FILE=src/foo.cpp Compile single source file; executable named after the file"
	@echo "  make clean-data         Delete all files in $(DATA_DIR)/ (asks for confirmation)"
	@echo "  make clean-pictures     Delete all files in $(PICTURE_DIR)/ (asks for confirmation)"
	@echo "  make clean-all          Delete all files in $(DATA_DIR) and $(PICTURE_DIR) / (asks for confirmation)"
	@echo "  make save FILE=xxx.csv  Move FILE to $(SAVE_DIR)/"
	@echo "  make help               Show this help message"

