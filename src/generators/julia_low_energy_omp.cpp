/* Version 2.2: Sous-échantillonnage spatial (Downsampling)
Compile : g++ -O3 -fopenmp julia_low_energy_omp.cpp -o julia_low_energy_omp
Execute : ./main -p "./generators/julia_low_energy_omp" -f 10 -d "v2_2_low_energy_omp"

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

    const int STEP = 2;

    vector<unsigned char> image_buffer(taille * taille * 3);

    // V2.2
    #pragma omp parallel for schedule(dynamic)
    for (int line = 0; line < taille; line += STEP) {
        for (int col = 0; col < taille; col += STEP) {
            
            // Calcul d'un seul pixel pour le bloc STEP x STEP
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

            // Remplissage du bloc dans le buffer
            for (int d_line = 0; d_line < STEP && (line + d_line) < taille; d_line++) {
                for (int d_col = 0; d_col < STEP && (col + d_col) < taille; d_col++) {
                    int idx = ((line + d_line) * taille + (col + d_col)) * 3;
                    image_buffer[idx] = r;
                    image_buffer[idx+1] = g;
                    image_buffer[idx+2] = bv;
                }
            }
        }
    }
    // end V2.2

    // Gestion du fichier de sortie
    string fileName = (argc > 1) ? argv[1] : "julia_low_energy_omp";
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