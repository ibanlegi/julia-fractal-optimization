/* Version 1.3: Réduction du seuil d'itération (Early exit)
Compile : g++ -Wall -O2 -std=c++17 -o generators/julia_early_exit src/generators/julia_early_exit.cpp
Execute : ./main -p "./generators/julia_early_exit" -f 10 -d "v1_2_early_exit"

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
    const int iterationmax = 250;

    const double a = -0.8;
    const double b = 0.156;

    vector<unsigned char> image_buffer(taille * taille * 3);

    // V1.3
    for (int line = 0; line < taille; line++) {
        for (int col = 0; col < taille; col++) {
            int i = 1;
            double x = xmin + col * (xmax - xmin) / taille;
            double y = ymax - line * (ymax - ymin) / taille;

            // La boucle s'arrêtera beaucoup plus vite pour les points "longs"
            while (i <= iterationmax && (x*x + y*y) <= 4.0) {
                double xtmp = x*x - y*y + a;
                y = 2*x*y + b;
                x = xtmp;
                i++;
            }

            int idx = (line * taille + col) * 3;

            if (i > iterationmax) {
                // Les points n'ayant pas échappé après 250 itérations sont mis en noir
                image_buffer[idx] = image_buffer[idx+1] = image_buffer[idx+2] = 0;
            } else {
                // Coloration basée sur l'indice d'échappement réduit
                image_buffer[idx]   = (4 * i) % 256;
                image_buffer[idx+1] = (2 * i) % 256;
                image_buffer[idx+2] = (6 * i) % 256;
            }
        }
    }

    // end V1.3

    // Gestion du fichier de sortie
    string fileName = (argc > 1) ? argv[1] : "julia_early_exit";
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