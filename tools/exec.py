# python3 tools/exec.py

import subprocess
import os

CSV_DIR = "./data"
PPM_DIR = "./export-pictures/ppm"

commands = [
    './main -p "generators/julia_bruteforce" -f 3.9 -print',
    './main -p "generators/julia_symmetric" -f 3.9 -print',
    './main -p "generators/julia_downsampling" -f 3.9 -print',
    './main -p "generators/julia_early_exit" -f 3.9 -print',
    './main -p "generators/julia_symmetric_omp" -f 3.9 -print',
    './main -p "generators/julia_low_energy_omp" -f 3.9 -print',
    './main -p "generators/julia_fully_optimized" -f 3.9 -print'
]

iterations = 20
final_file = "final_averages_test_3.csv"

def cleanup_files(filenames, keep_file):
    # Supprime les fichiers .csv et .ppm 
    # pour toutes les itérations sauf celle à garder
    for fname in filenames:
        if fname == keep_file:
            continue
        
        # Chemins des fichiers à supprimer
        print(fname)
        csv_path = os.path.join(CSV_DIR, f"{fname}.csv")
        ppm_path = os.path.join(PPM_DIR, f"{fname}.ppm")
        
        try:
            if os.path.exists(csv_path):
                os.remove(csv_path)
            if os.path.exists(ppm_path):
                os.remove(ppm_path)
        except Exception as e:
            print(f"Erreur lors de la suppression de {fname} : {e}")

def run_benchmark(cmd):
    print(f"\n>>> Traitement de : {cmd}")
    
    all_metrics = []
    all_filenames = []

    for i in range(iterations):
        try:
            raw_output = subprocess.check_output(cmd, shell=True).decode('utf-8').strip()
            parts = raw_output.split(',')
            
            filename = parts[0]
            metrics = [float(parts[1]), float(parts[2]), float(parts[3])]
            
            all_filenames.append(filename)
            all_metrics.append(metrics)

            print(f"   Itération {i+1}/{iterations} terminée", end='\r')
        except Exception as e:
            print(f"\n   Erreur à l'itération {i+1} : {e}")

    if all_metrics:
        # 1. Calcul des moyennes
        avg_time   = sum(m[0] for m in all_metrics) / len(all_metrics)
        avg_energy = sum(m[1] for m in all_metrics) / len(all_metrics)
        avg_power  = sum(m[2] for m in all_metrics) / len(all_metrics)
        averages   = [avg_time, avg_energy, avg_power]

        # 2. Trouver le fichier le plus proche
        best_idx = 0
        min_diff = float('inf')
        for idx, m in enumerate(all_metrics):
            diff = abs(m[0] - avg_time) + abs(m[1] - avg_energy)
            if diff < min_diff:
                min_diff = diff
                best_idx = idx
        
        closest_file = all_filenames[best_idx]

        # 3. NETTOYAGE : On garde seulement le meilleur
        print(f"\n>>> Nettoyage des fichiers inutiles...")
        cleanup_files(all_filenames, closest_file)

        return averages, closest_file
    
    return None, None

if __name__ == "__main__":
    with open(final_file, "w") as f:
        f.write("Command, File,Average Time (s),Average Energy (J),Average Maximum Power (W)\n")
        
        for cmd in commands:
            stats, closest_file = run_benchmark(cmd)
            if stats:
                res_line = f'"{cmd}",{closest_file},{stats[0]:.4f},{stats[1]:.2f},{stats[2]:.2f}'
                f.write(res_line + "\n")
                print(f">>> Fichier représentatif conservé : {closest_file}")

    print(f"\nTerminé. Résultats dans '{final_file}'.")
