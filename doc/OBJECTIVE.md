# Optimization of Julia Set Image Generation

## Project Objective

The goal is to optimize the generation of Julia set images while minimizing quality loss. The optimization focuses on three primary axes:

* **Execution Time**: Reducing the total time required to generate the image while maintaining image integrity.
* **Energy Consumption**: Minimizing the total energy consumed by the CPU and memory during generation to improve energy efficiency.
* **Peak Power**: Lowering the peak power (energy divided by time) to ensure the system operates more stably and efficiently.

## Approach

* Implement multiple generation strategies or algorithms for the Julia set.
* Measure the performance of each approach using specialized tools (e.g., MojitO/S).
* Compare results to identify the best compromise between speed, energy consumption, and image quality.

## Success Criteria

* Preserved image quality (pixel fidelity, color mapping, resolution).
* Precise measurement of execution time, energy, and peak power.
* Identification of configurations offering the best balance across the three optimization axes.

## Image Quality Measurement Tools

### Objective Methods

*These compare a test image against a reference image to quantify quality loss.*

* **PSNR (Peak Signal-to-Noise Ratio)**: Measures the difference between two images in decibels. A higher PSNR indicates better quality. (Implemented via OpenCV or Scikit-image).
* **SSIM (Structural Similarity Index)**: Evaluates structural similarity, contrast, and luminance. A value closer to 1 indicates higher quality; it is particularly effective for fractal structures.
* **MSE (Mean Squared Error)**: Measures the average squared difference between pixels. A value closer to 0 indicates higher quality.

### Perceptual Methods

*These attempt to mimic human perception of quality.*

* **FSIM (Feature Similarity Index)**: Compares significant local image features.
* **VIF (Visual Information Fidelity)**: Compares visual information content between two images.
* **LPIPS (Learned Perceptual Image Patch Similarity)**: Uses neural networks to measure the human perception of differences between images.

### No-Reference (Blind) Methods

*Metrics used when a reference image is unavailable.*

* **BRISQUE**: Evaluates image quality without a reference by analyzing natural scene statistics.
* **NIQE**: Measures statistical similarity to natural images.
* **PIQE**: Measures "blurriness" and noise levels.

> **Proposal:** Use a combination of **SSIM** (Objective/Reference-based) to verify fidelity to the base image, alongside **BRISQUE/NIQE** to detect blur or artifacts introduced by the optimization process.
