/* Version 1.2: Sous-échantillonnage spatial (Downsampling)
Compile : g++ -Wall -O2 -std=c++17 -o generators/julia_downsampling src/generators/julia_downsampling.cpp
Execute : ./main -p "./generators/julia_downsampling" -f 10 -d "v1_2_downsampling"

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <omp.h>

using namespace std;

#define DEST_FILE_DATA "./export-pictures/ppm"

int main(int argc, char* argv[]) {
    const int taille = 10000;
    const double xmin = -1.0, xmax = 1.0;
    const double ymin = -1.0, ymax = 1.0;
    const int iterationmax = 1000;

    const double a = -0.8;
    const double b = 0.156;

    vector<unsigned char> image_buffer(taille * taille * 3);

    // V1.2
    // STEP = 2 divise le nombre de calculs par 4 (2*2)
    // STEP = 4 diviserait le nombre de calculs par 16 (4*4)
    const int STEP = 2; 

    // Parcours de l'image avec un pas de STEP
    for (int line = 0; line < taille; line += STEP) {
        for (int col = 0; col < taille; col += STEP) {
            
            // 1. Calcul d'un seul pixel pour le bloc STEP x STEP
            int i = 1;
            double x = xmin + col * (xmax - xmin) / taille;
            double y = ymax - line * (ymax - ymin) / taille;

            while (i <= iterationmax && (x*x + y*y) <= 4.0) {
                double xtmp = x*x - y*y + a;
                y = 2*x*y + b;
                x = xtmp;
                i++;
            }

            // Détermination de la couleur du bloc
            unsigned char r, g, b_val;
            if (i > iterationmax) {
                r = g = b_val = 0;
            } else {
                r = (4 * i) % 256;
                g = (2 * i) % 256;
                b_val = (6 * i) % 256;
            }

            // 2. Extension de la valeur aux pixels voisins (le "bloc")
            // Cela crée l'effet de pixellisation (downsampling)
            for (int d_line = 0; d_line < STEP && (line + d_line) < taille; d_line++) {
                for (int d_col = 0; d_col < STEP && (col + d_col) < taille; d_col++) {
                    int idx = ((line + d_line) * taille + (col + d_col)) * 3;
                    image_buffer[idx] = r;
                    image_buffer[idx+1] = g;
                    image_buffer[idx+2] = b_val;
                }
            }
        }
    }

    // end V1.2

    // Gestion du fichier de sortie
    string fileName = (argc > 1) ? argv[1] : "julia_downsampling";
    string filePath = string(DEST_FILE_DATA) + "/" + fileName + ".ppm";

    ofstream out(filePath, ios::binary);
    if (!out) {
        cerr << "Erreur d'ouverture du fichier " << filePath << endl;
        return 1;
    }

    out << "P6\n" << taille << " " << taille << "\n255\n";
    out.write(reinterpret_cast<char*>(image_buffer.data()), image_buffer.size());
    out.close();

    return 0;
}