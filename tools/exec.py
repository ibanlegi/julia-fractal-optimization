import os

# Liste des commandes complètes à exécuter
commands = [
    './main -p "python3 files-test/ensemble_julia.py" -f 20 -sudo sudo -print',
    './main -p "python3 files-test/ensemble_julia.py" -f 10 -sudo sudo -print',
    './main -p "python3 files-test/ensemble_julia.py" -f 10 -sudo sudo -print',
    './main -p "python3 files-test/ensemble_julia.py" -f 10 -sudo sudo -print',
    './main -p "python3 files-test/ensemble_julia.py" -f 10 -sudo sudo -print'
]

output_file = "all_commands_output.txt"

with open(output_file, "w") as f:
    for cmd in commands:
        print(f"\n> Executing: {cmd}n")
        # os.system avec redirection standard et erreur vers le fichier
        f.flush()  # assure que l'en-tête est écrit avant l'exécution
        os.system(f"{cmd} >> {output_file} 2>&1")
