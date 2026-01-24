/* Version 2.3: Optimisiation totale
Compile : g++ -O3 -fopenmp julia_fully_optimized.cpp -o julia_fully_optimized
Execute : ./main -p "./generators/julia_fully_optimized" -f 10 -d "v2_3_fully_optimized"

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
    const int iterationmax = 500;

    const double a = -0.8;
    const double b = 0.156;

    vector<unsigned char> image_buffer(taille * taille * 3);

    // V2.3
    #pragma omp parallel for schedule(dynamic)
    for (int line = 0; line < taille / 2; line++) { // Symétrie centrale (50% de lignes)
        for (int col = 0; col < taille; col++) {
            
            int i = 1;
            double x = xmin + col * (xmax - xmin) / taille;
            double y = ymax - line * (ymax - ymin) / taille;

            // Boucle optimisée par iterationmax réduit
            while (i <= iterationmax && (x*x + y*y) <= 4.0) {
                double xtmp = x*x - y*y + a;
                y = 2*x*y + b;
                x = xtmp;
                i++;
            }

            unsigned char r, g, bv;
            if (i > iterationmax) {
                r = g = bv = 0;
            } else {
                r = (4 * i) % 256;
                g = (2 * i) % 256;
                bv = (6 * i) % 256;
            }

            // Remplissage du pixel actuel
            int idx = (line * taille + col) * 3;
            image_buffer[idx] = r;
            image_buffer[idx+1] = g;
            image_buffer[idx+2] = bv;

            // Recopie par symétrie (Rotation 180°)
            int sym_idx = ((taille - 1 - line) * taille + (taille - 1 - col)) * 3;
            image_buffer[sym_idx] = r;
            image_buffer[sym_idx+1] = g;
            image_buffer[sym_idx+2] = bv;
        }
    }
    // end V2.3

    // Gestion du fichier de sortie
    string fileName = (argc > 1) ? argv[1] : "julia_fully_optimized";
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