
# 1. Version 0 : Brute-force (La Référence)
> `julia_bruteforce.cpp`
C'est votre point de comparaison initial, correspondant au code que vous avez déjà fourni. Elle calcule chaque pixel un par un sans aucune optimisation mathématique ou matérielle. Elle sert de base pour mesurer le gain des versions suivantes.

# 2. Version 1 : Parallélisation (Multi-threading)
> `julia_parallel.cpp`
Cette version utilise **OpenMP** ou les threads natifs pour répartir le calcul des lignes de l'image sur tous les cœurs de votre processeur. Les sources indiquent que cette étape est cruciale car le calcul des fractales est "parfaitement parallèle".

# 3. Version 2 : Optimisations Mathématiques Simples
> `julia_symmetry_shape.cpp`
Vous pouvez ici implémenter les méthodes qui évitent des calculs inutiles :
*   **Symmetry Pasting :** Calculer uniquement la moitié de l'image en exploitant la symétrie de l'ensemble de Julia.
*   **Shape Checking :** Utiliser des formules pour colorier instantanément les points situés dans les formes connues (comme la cardioïde), évitant ainsi les 1000 itérations par pixel.

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