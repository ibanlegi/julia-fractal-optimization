# embedded-sys-energy

Ce dépôt contient les travaux liés à l'optimisation énergétique de la génération de fractales.

* **Objectif du projet** : [OBJECTIVE](./doc/OBJECTIVE.md)
* **Plan d'optimisation** : [OPTIMIZATION_PLAN](./doc/OPTIMIZATION_PLAN.md)

## Installation et Préparation

Avant de lancer les tests, il est nécessaire de configurer l'environnement de travail :

1. **Initialisation des dossiers** : Créez l'arborescence nécessaire au projet (dossiers de données et d'export).
```bash
make setup
```


2. **Compilation du programme principal** : Génère l'exécutable `main` qui sert d'interface de test.
```bash
make
```


3. **Compilation des générateurs** : Compile l'ensemble des approches algorithmiques situées dans le dossier `generators`.
```bash
make gen-julia
```



## Exécution

Le projet s'appuie sur l'exécutable `main` pour instancier les différents générateurs.

### Lancer une génération

Pour exécuter une approche spécifique (ex: brute-force) :

```bash
./main -p "./generators/julia_bruteforce" -f 10 -d "reference"
```

**Fichiers générés :**

* `data/reference.csv` : Données brutes de sortie de MojitO/S.
* `export-pictures/ppm/reference.ppm` : Image de la fractale au format PPM.
* `export-pictures/jpeg/reference.jpeg` : Conversion JPEG automatique.

### Outils et Mesures

* **Conversion manuelle** : `make convert FILE=export-pictures/ppm/reference.ppm`
* **Mesure PSNR** : `python3 -m tools.psnr -r export-pictures/jpeg/reference.jpeg -t export-pictures/jpeg/julia_symmetric.jpeg`
* **Mesure SSIM** : `python3 -m tools.ssim -r export-pictures/jpeg/reference.jpeg -t export-pictures/jpeg/julia_symmetric.jpeg`

### Automatisation (Benchmarking)

Pour lancer une campagne de tests sur plusieurs itérations :

```bash
python3 tools/exec.py
```

Le script calcule les moyennes et génère le fichier `final_averages.csv`.
>Note : Les images JPEG et les mesures de qualité (PSNR/SSIM) doivent être traitées séparément après l'exécution du script.