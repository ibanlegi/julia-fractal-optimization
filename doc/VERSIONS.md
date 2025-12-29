
# 1. Version 0 : Brute-force (La Référence)
> `julia_bruteforce.cpp`
```bash
===== Measurement Results =====
Execution time: 21.7056 s
CPU energy: 4.18938 J
Memory energy: 0.780943 J
(CPU + Memory): 4.97032 J
Max power: 50.8568 W
===============================
```
C'est le point de comparaison initial. Elle calcule chaque pixel un par un sans aucune optimisation mathématique ou matérielle. Elle sert de base pour mesurer le gain des versions suivantes.

# 2. Version 1 : Parallélisation (Multi-threading)
> `julia_parallel.cpp`
```bash
Parallel computing across 36 cores...
===== Measurement Results =====
Execution time: 1.5288 s
CPU energy: 2.20306 J
Memory energy: 0.378415 J
(CPU + Memory): 2.58147 J
Max power: 48.6091 W
===============================
```
Cette version utilise **OpenMP** ou les threads natifs pour répartir le calcul des lignes de l'image sur tous les cœurs de votre processeur. Les sources indiquent que cette étape est cruciale car le calcul des fractales est "parfaitement parallèle".

# 3. Version 2 : Optimisations Mathématiques Simples
> `julia_symmetry_shape.cpp`
Les optimisations implémentées :
* **Symmetry Pasting (Symétrie Centrale)** :\\
L'ensemble de Julia pour un point $c$ possède une symétrie centrale par rapport à l'origine $(0,0)$. Cela signifie que $f(z) = f(-z)$. On ne calcule donc que les 50 premières lignes, et on recopie chaque pixel à sa position symétrique ($180^\circ$). On divise le temps de calcul par 2.

* **Shape Checking (Détection de zone)** :\\ A
vant de lancer la boucle de 1000 itérations, on vérifie si le point est déjà très loin de l'origine. Pour Julia, si $|z| > 2$ dès le départ, on sait qu'il s'échappera.Buffer Local : Pour éviter les accès disque lents ou les problèmes de symétrie avec out.put(), on utilise un petit buffer pour reconstruire l'image avant l'écriture.

# 4. Version 3 : Border Tracing (Suivi de Frontière)
> `julia_border_tracing.cpp`
Cette version est l'une des plus performantes citées dans les sources. Au lieu de tester chaque pixel, l'algorithme trace le contour des zones de couleur et remplit l'intérieur, ce qui peut accélérer le calcul de **30 fois** sur certains domaines.

# 5. Version 4 : Version Combinée (Optimisation Totale)
> `julia_fully_optimized.cpp`
Il s'agit de la version la plus aboutie qui fusionne toutes les techniques précédentes : **multi-threading + border tracing + symmetry + shape checking**. C'est celle qui devrait normalement offrir la consommation d'énergie la plus basse car elle réduit radicalement le temps d'exécution.

# 6. Version 5 : Stratégie Basse Fréquence (DVFS)
> `julia_dvfs_strategy.cpp`
Ce n'est pas une nouvelle version de code, mais une **stratégie d'exécution** de votre version la plus rapide (V4) à une fréquence processeur réduite. C'est le levier indispensable pour minimiser la **puissance maximale (en Watts)** demandée dans votre tableau de résultats.

**Résumé pour votre rendu :**
Votre tableau final devra comparer ces approches sur les axes **Temps (s), Énergie (J) et Puissance (W)** afin d'identifier la configuration offrant le meilleur équilibre entre rapidité et efficacité énergétique.

**Analogie :** 
C'est comme préparer une course. La **V0** est un marcheur seul. La **V1** est un groupe de marcheurs (parallélisme). La **V2/V3** est un marcheur qui connaît des raccourcis (algorithmes). La **V4** est un groupe de coureurs qui prennent tous les raccourcis. Enfin, la **basse fréquence** consiste à demander à ces coureurs de ralentir leur rythme cardiaque pour ne pas s'épuiser (puissance max).