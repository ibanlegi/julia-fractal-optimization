# Optimisation de la génération d’images de l’ensemble de Julia

## Objectif du projet
Optimiser la génération d’images de l’ensemble de Julia tout en minimisant la perte de qualité. L’optimisation se fait selon trois axes principaux :
- **Temps d’exécution** : Réduire le temps total nécessaire à la génération de l’image, tout en conservant la qualité de l’image.
- **Consommation d’énergie** : Minimiser l’énergie totale consommée par le CPU et la mémoire pendant la génération de l’image, afin d’optimiser l’efficacité énergétique.
- **Puissance maximale** : Réduire la puissance de pointe (énergie divisée par le temps), pour que le système fonctionne de manière plus stable et efficace.

## Approche
- Implémenter plusieurs stratégies ou algorithmes de génération pour l'ensemble de Julia.
- Mesurer les performances de chaque approche à l'aide d'outils automatisés (ex.: Mojitos).
- Comparer les résultats pour identifier le meilleur compromis entre rapidité, consommation d'énergie et qualité d'image.

## Critères de réussite
- Qualité des images préservée autant que possible (fidélité des pixels, mapping des couleurs, résolution).
- Mesure du temps d’exécution, de l’énergie et de la puissance maximale.
- Identifier les configurations offrant le meilleur équilibre entre ces trois axes.

## Outils de mesure de la qualité de l'image

### Méthodes objectives
*Comparent une image test à une image de référence pour quantifier la perte de qualité.*

- **PSNR** (Peak Signal-to-Noise Ratio) : Mesure la différence entre deux images en décibels. Plus le PSNR est élevé, meilleure est la qualité. OpenCV (*cv2.PSNR*), skimage (*compare_psnr*).

- **SSIM** (Structural Similarity Index) : Évalue la similarité structurelle, contraste et luminance entre images. Plus proche de 1, meilleure la qualité (tient compte de la structure fractale de l’image). OpenCV (*cv2.quality.QualitySSIM_create*), scikit-image (*compare_ssim*).

- **MSE** (Mean Squared Error) : Mesure la moyenne des carrés des différences de pixels. Plus proche de 0, meilleure qualité.

### Méthodes perceptuelles
*Essaient d'approcher la perception humaine de la qualité.*

- **FSIM** (Feature Similarity Index) : Compare les caractéristiques locales importantes de l'image. MATLAB, python implementations.

- **VIF** (Visual Information Fidelity) : Compare les informations visuelles entre deux images. MATLAB, python.

- **LPIPS** (Learned Perceptual Image Patch Similarity) : Basé sur un réseau neuronal pour mesurer la perception humaine de la différence entre images. PyTorch, TensorFlow.

### Méthodes sans référence (No-Reference/Blind)
*Utilisation des métriques sans référence.*

- **BRISQUE** : Evalue la qualité d'une image sans référence. Python (*imquality* ou *pybrisque*).

- **NIQE** : Similarité statistique avec des images naturelles. Python (*imquality*).

- **PIQE** : Mesure de "blurriness" et bruit. Python (*imquality*)

> **Proposition :** Utilisation combinée de la méthode objective SSIM (avec référence) pour vérifier la fidélité à l'image de base, et BRISQUE/NIQE pour détecter flou ou artefacts introduits par l’optimisation.