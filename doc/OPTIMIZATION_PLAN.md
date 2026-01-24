# Optimization Plan for the Julia Set Generator

The system optimization is based on three metric axes: **Execution Time** (), **Energy Consumption** (), and **Peak Power** ().

### Reference Version: Brute-force

* **File**: `julia_bruteforce.cpp`
* **Description**: Initial baseline that calculates each pixel individually without any hardware or algorithmic optimization.
* **Usage**: Serves as the foundation to quantify performance gains and quality degradation (SSIM/PSNR) for subsequent versions.

---

## Series 1: Sequential Algorithmic Optimizations

* **Goal**: Drastically reduce energy consumption and execution time by decreasing the total number of operations.
* **Consequence**: Accept a slight degradation in quality (noise, aliasing, or loss of detail).

### Version 1.1: Sequential Central Symmetry (Algorithmic Reduction)

* **File**: `julia_symmetric.cpp`
* **Lever**: Domain reduction.
This version cuts the number of mathematical calculations in half by exploiting the central symmetry of the Julia set: .
* **Methodology**: The algorithm only calculates the upper half of the pixels (e.g., 5,000 lines out of 10,000) and duplicates the results via a 180° rotation to complete the image.
* **Impact**: Theoretical reduction of 50% in CPU time and dynamic energy.
* **Quality**: Theoretically lossless, though rounding or alignment errors may slightly affect the SSIM score.

### Version 1.2: Spatial Downsampling

* **File**: `julia_downsampling.cpp`
* **Lever**: Resolution reduction.
Instead of calculating every pixel in a  image, the algorithm calculates a smaller image and scales it up. By calculating only 1 pixel out of every 2 (or 4) in each direction, a resolution reduction by factor of 2 reduces the total pixel count by 4.
* **Methodology**: Calculate a single pixel for a  (or ) block, then extend that value to neighboring pixels.
* **Impact**: Skipping 2 pixels in each direction divides the total iterations by 4, offering the most radical energy gain in this series.
* **Quality**: Visible pixelation effect, leading to a significant drop in SSIM.

### Version 1.3: Early Exit (Iteration Threshold Reduction)

* **File**: `julia_early_exit.cpp`
* **Lever**: Calculation depth reduction.
Instead of 1,000 iterations, the limit is reduced to 250 or 500. This provides a proportional reduction in time spent within the `while` loop for points belonging to the Julia set.
* **Methodology**: The `iterationmax` threshold is lowered from 1,000 to 250 or 500.
* **Impact**: Reduced calculation time primarily in dense fractal zones (points that have not yet escaped the condition ).
* **Quality**: Loss of precision in fine structures and complex filaments at the boundaries, making edges appear "coarser" or more abrupt.

---

## Series 2: Parallel Algorithmic Optimizations

Based on the results from the first series, the most efficient algorithms are parallelized to maximize optimization gains.

### Version 2.1: Parallel Symmetry

* **File**: `julia_symmetric_omp.cpp`
* **Lever**: Algorithm (Central Symmetry) + Hardware (OpenMP).
Combines the 50% domain reduction with workload distribution across all CPU cores.
* **Methodology**: Implementation of `#pragma omp parallel for` on the loop processing the first 5,000 lines.

### Version 2.2: Low Energy Profile

* **File**: `julia_low_energy_omp.cpp`
* **Lever**: Degraded resolution (Downsampling) + Parallelism.
This version is designed to set the record for the lowest possible energy consumption for the project.
* **Methodology**: Parallelization of the V1.2 algorithm (pixel skipping).

### Version 2.3: Full Optimization

* **File**: `julia_fully_optimized.cpp`
* **Lever**: Symmetry + Early Exit + OpenMP.
The final version that combines multiple gains without sacrificing too much spatial resolution.
* **Methodology**: Concurrent use of symmetry, a reasonable iteration cap (e.g., 500), and dynamic parallelism.
* **Quality**: Expected to provide the best balance (compromise) between SSIM and Energy.
