import matplotlib.pyplot as plt
import numpy as np
from datetime import datetime
import os

DEST_FILE_DATA = "export-pictures"  # dossier local

# Vérifie et crée le dossier si nécessaire
os.makedirs(DEST_FILE_DATA, exist_ok=True)

def generateNameFile():
    now = datetime.now()
    filename = now.strftime("%d-%m_%H-%M-%S.jpeg")
    return os.path.join(DEST_FILE_DATA, filename)


taille = 200
xmin, xmax = -1, 1
ymin, ymax = -1, 1
iterationmax = 100

a = -0.8
b = 0.156

pixels = np.zeros((taille, taille, 3), dtype='uint8')

for line in range(taille):
    for col in range(taille):
        i = 1
        x = xmin + col*(xmax-xmin)/taille
        y = ymax - line*(ymax-ymin)/taille
        while i <= iterationmax and (x*x + y*y) <= 4:
            x, y = x*x - y*y + a, 2*x*y + b
            i += 1

        if i > iterationmax:
            pixels[line, col] = (0, 0, 0)
        else:
            pixels[line, col] = ((4*i) % 256, (2*i) % 256, (6*i) % 256)

# Sauvegarde de l'image fractale en JPEG
plt.imsave(generateNameFile(), pixels)
