/* Version 2.1: Symétrique parallèle (réduction algorithmique)
Compile : g++ -O3 -fopenmp julia_symmetric_omp.cpp -o julia_symmetric_omp
Execute : ./main -p "./generators/julia_symmetric_omp" -f 10 -d "v2_1_symmetric_omp"

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

    // V2.1
    #pragma omp parallel for schedule(dynamic)
    for (int line = 0; line < taille / 2; line++) {
        for (int col = 0; col < taille; col++) {
            
            // Calcul du pixel (Moitié supérieure)
            int i = 1;
            double x = xmin + col * (xmax - xmin) / taille;
            double y = ymax - line * (ymax - ymin) / taille;

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

            // Remplissage du pixel opposé (Rotation 180°)
            int sym_idx = ((taille - 1 - line) * taille + (taille - 1 - col)) * 3;
            image_buffer[sym_idx] = r;
            image_buffer[sym_idx+1] = g;
            image_buffer[sym_idx+2] = bv;
        }
    }

    // end V2.1

    // Gestion du fichier de sortie
    string fileName = (argc > 1) ? argv[1] : "julia_symmetric_omp";
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