# Plan d'optimisation du générateur de l'ensemble de Julia
L'optimisation du système repose sur trois axes métriques : le Temps d'exécution ($s$), la Consommation d'énergie ($J$) et la Puissance maximale ($W$).

### Version de Référence : Brute-force
- __Fichier__ : `julia_bruteforce.cpp`
- __Description__ : Point de comparaison initial calculant chaque pixel unitairement sans optimisation matérielle ou algorithmique.
- __Usage__ : Sert de base pour quantifier les gains de performance et la dégradation de la qualité (SSIM/PSNR) des versions suivantes.

## Série 1 : Optimisations algorithmiques séquentielles

- __Objectif__ : Réduire drastiquement la consommation d'énergie et le temps d'exécution en réduisant le nombre total d'opérations effectuées. 
- __Conséquence__ : Accepeter une légère dégradation de la qualité (bruit, crénelage ou perte de détail).

### Version 1.1 : Symétrique centrale séquentielle (Réduction algorithmique)
- __Fichier__ : `julia_symmetric.cpp`
- __Levier__ : Réduction du domaine de calcul

Cette version divise par deux le nombre de calculs mathématiques en exploitant la symétrie centrale de 
l'ensemble de Julia $(f(z) = f(-z))$.

- __Méthodologie__ : L'algorithme ne calcule que la moitié supérieure des pixels (5000 lignes sur 10000) et duplique les résultats par rotation de 180° pour compléter l'image.
- __Impact__ : Réduction théorique de 50% du temps CPU et de l'énergie dynamique.
- __Qualité__ : Théoriquement sans perte, bien que des erreurs d'arrondi ou d'alignement puissent légèrement impacter le score SSIM.

### Version 1.2 : Sous-échantillonnage spatial (Downsampling)
- __Fichier__ : `julia_downsampling.cpp`
- __Levier__ : Réduction de la résolution

Au lieu de calculer chaque pixel d'une image $10000 \times 10000$, l'algorithme calcule une image plus petite et l'agrandie.
On va calculer seulement 1 pixel sur 2 (ou sur 4) dans chaque direction. La résolution par 2 réduit le nombre de pixels à calculer par 4.

- __Méthodologie__ : Calcul d'un seul pixel sur un bloc de $2 \times 2$ (ou $4 \times 4$), puis extension de la valeur aux pixels voisins.- __Impact__ : Un saut de 2 pixels dans chaque direction divise par 4 le nombre total d'itérations, offrant le gain énergétique le plus radical de cette série.
- __Qualité__ : Apparition d'un effet de pixellisation, entraînant une chute significative du SSIM.

### Version 1.3 : Réduction du seuil d'itération (Early exit)
- __Fichier__ : `julia_early_exit.cpp`
- __Levier__ : Réduction de la profondeur de calcul

Au lieu de faire 1000 itérations, on réduit à 250 ou 500. Ca nous permet d'avoir Réduction proportionnelle du temps passé dans la boucle while pour les points appartenant à l'ensemble de Julia. Au niveau de la qualité, on risque d'avoir des détails fins à la frontière de la fractale qui vont disparaitre, rendant les bords plus "grossiers".

- __Méthodologie__ : Le seuil `iterationmax` est abaissé de 1000 à 250 ou 500.
- __Impact__ : Réduction du temps de calcul principalement dans les zones denses de la fractale (points n'ayant pas encore échappé à la condition $|z| \le 2$).
- __Qualité__ : Perte de précision sur les structures fines et les filaments complexes situés à la frontière de l'ensemble, rendant les contours plus abrupts ou "grossiers".

## Série 2 : Optimisations algorithmiques parallèles
En se basant sur les résultats obtenus dans la première série, on va paralléliser les algorithmes les plus performants pour gagner du temps d'optimisation.

### Version 2.1 : Symétrie parallèle de l'ensemble Julia
- __Fichier__ : `julia_symmetric_omp.cpp `
- __Levier__ : Algorithme (Symétrie centrale) + Matériel (OpenMP).

Cette version combine la réduction de 50% du domaine de calcul avec la répartition de la charge sur tous les cœurs.
- __Méthodologie__ : On utilise `#pragma omp parallel for` sur la boucle des 5000 premières lignes.

### Version 2.2 : Basse consommation d'énergie
- __Fichier__ : `julia_low_energy_omp.cpp` 
- __Levier__ : Résolution dégradée (Downsampling) + Parallélisme.

C'est la version destinée à établir le record de la plus basse consommation d'énergie possible pour le projet.
- __Méthodologie__ : On parallélise l'algorithme de la V1.2 (Saut de pixels).

### Version 2.3 : Optimisation totale
- __Fichier__ : `julia_fully_optimized.cpp `
- __Levier__ : Symétrie + Early Exit + OpenMP.

Il s'agit de la version finale qui combine les gains sans trop sacrifier la résolution spatiale.

- __Méthodologie__ : Utilisation de la symétrie, d'un plafond d'itérations raisonnable (ex: 500) et du parallélisme dynamique.
- __Qualité__ : C'est ici qu'on devrait avoir le meilleur compromis SSIM/Énergie.