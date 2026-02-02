# Results summary

This `README` focuses **exclusively on the results obtained** during the experimental campaign (performance, energy, power, and image quality).

The file [OPTIMIZATION_PLAN](./OPTIMIZATION_PLAN.md) describes the **test scenario and methodology** used in this project:

* the different optimization stages,
* the rationale behind each variant,
* the order in which experiments should be conducted,
* and the parameters used for fair comparison.

## Experimental Context

All experiments were conducted under controlled and reproducible conditions:

* **Platform**: Grid’5000 cluster – node `gros-55.nancy.grid5000.fr`
* **CPU**: Intel® Xeon® Gold 5220 @ 2.20 GHz
* **CPU Governor**: `performance` (max frequency 3.9 GHz)
* **Compiler**: `gcc 10.2.1` (Debian)
* **Image Resolution**: 10,000 × 10,000 pixels
* **Maximum Iterations**: 1,000
* **Measurements**: Averages over 20 runs per version
* **Energy Tool**: MojitOS (CPU + memory)


## Optimization Strategy

The reference implementation is a **brute-force Julia set generator**, used as a baseline.

Two main optimization phases were explored:

1. **Sequential optimizations**
   Reduce computational workload at the cost of limited visual degradation:

   * Central symmetry
   * Downsampling
   * Early exit (iteration reduction)

2. **Parallel optimizations (OpenMP)**
   Apply multithreading to the most efficient sequential versions in order to drastically reduce execution time.


## Results Overview

### Performance, Energy, and Quality Metrics

| ID   | Method                  | Time (s) | Energy (J) | Pmax (W) | PSNR (dB) | SSIM (%) |
| ---- | ----------------------- | -------: | ---------: | -------: | --------: | -------: |
| Ref  | Bruteforce              |    20.37 |      17.35 |    52.44 |         – |        – |
| V1.1 | Symmetry                |    10.49 |      17.33 |    53.49 |     18.49 |    89.34 |
| V1.2 | Downsampling            |     5.57 |      17.66 |    53.17 |     16.88 |    84.31 |
| V1.3 | Early Exit              |    16.53 |      17.35 |    52.67 |     16.84 |    88.70 |
| V2.1 | Symmetry + OpenMP       |     0.81 |      22.26 |    90.33 |     18.49 |    89.34 |
| V2.2 | Low-Energy (Down + OMP) |     0.63 |      20.31 |    81.80 |     16.88 |    84.31 |
| V2.3 | Fully Optimized         |     0.79 |      22.56 |    91.96 |     17.60 |    87.45 |


## Key Observations

### Execution Time

* Sequential symmetry divides execution time by **2×** with no energy penalty.
* Parallel versions reduce execution time by **more than 95%**, reaching sub-second runtimes.

### Energy Consumption

* Sequential optimizations consume the least absolute energy (~17 J).
* Parallel versions increase peak power but keep total energy within a reasonable range (20–23 J).
* **V2.2 (Low-Energy)** achieves the lowest energy consumption among parallel approaches.

### Visual Quality

* Downsampling produces the largest quality degradation (SSIM ≈ 84%).
* Symmetry-based approaches preserve structural fidelity best.
* **V2.3 (Fully Optimized)** offers the best overall compromise between speed, energy, and image quality.


## Conclusion

This study shows that **algorithmic simplifications combined with parallel execution** can dramatically improve performance while keeping energy consumption under control.

* **Best performance**: Parallel implementations (< 1 s)
* **Best energy efficiency (parallel)**: V2.2 – Low-Energy
* **Best global trade-off**: **V2.3 – Fully Optimized**

The results highlight that energy efficiency is not solely about reducing power, but about **minimizing energy per useful computation**, even if that means briefly spending watts aggressively.
