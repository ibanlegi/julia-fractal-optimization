# Julia fractal optimization

__Energy-efficient Julia set generation and algorithmic benchmarking.__

* [Project Description – Da Costa](https://www.irit.fr/~Georges.Da-Costa/syst%C3%A8mes-embarqu%C3%A9s-%C3%A9nergie/)

This repository contains research and implementation work focused on the **energy optimization of fractal generation**.

* **Project Objective**: [OBJECTIVE](./doc/OBJECTIVE.md)
* **Optimization Plan**: [OPTIMIZATION_PLAN](./doc/OPTIMIZATION_PLAN.md)

## Installation and Setup

Before running any tests, the working environment must be configured:

1. **Directory Initialization**: Create the necessary project structure (data and export folders).

```bash
make setup

```

2. **Main Program Compilation**: Generates the `main` executable, which serves as the testing interface.

```bash
make

```

3. **Generator Compilation**: Compiles all algorithmic approaches located in the `generators` folder.

```bash
make gen-julia

```

## Execution

The project utilizes the `main` executable to instantiate the various fractal generators.

### Running a Generation

To execute a specific approach (e.g., brute-force):

```bash
./main -p "./generators/julia_bruteforce" -f 10 -d "reference"

```

**Generated Files:**

* `data/reference.csv`: Raw output data from MojitO/S.
* `export-pictures/ppm/reference.ppm`: Fractal image in PPM format.
* `export-pictures/jpeg/reference.jpeg`: Automatic JPEG conversion.

### Tools and Metrics

* **Manual Conversion**: `make convert FILE=export-pictures/ppm/reference.ppm`
* **PSNR Measurement**: `python3 -m tools.psnr -r export-pictures/jpeg/reference.jpeg -t export-pictures/jpeg/julia_symmetric.jpeg`
* **SSIM Measurement**: `python3 -m tools.ssim -r export-pictures/jpeg/reference.jpeg -t export-pictures/jpeg/julia_symmetric.jpeg`

### Automation (Benchmarking)

To run a test campaign over multiple iterations:

```bash
python3 tools/exec.py

```

The script calculates averages and generates the `final_averages.csv` file.

> **Note**: JPEG images and quality metrics (PSNR/SSIM) must be processed separately after the script execution.
