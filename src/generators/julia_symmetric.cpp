/* Version 1.1: Symétrique séquentielle (réduction algorithmique)
Compile : g++ -Wall -O2 -std=c++17 -o generators/julia_symmetric src/generators/julia_symmetric.cpp
Execute : ./main -p "./generators/julia_symmetric" -f 10 -d "v1_1_symmetric"

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

    // V1.1
    for (int line = 0; line < taille / 2; line++) {
        for (int col = 0; col < taille; col++) {
            int i = 1;
            double x = xmin + col * (xmax - xmin) / taille;
            double y = ymax - line * (ymax - ymin) / taille;

            while (i <= iterationmax && (x*x + y*y) <= 4.0) {
                double xtmp = x*x - y*y + a;
                y = 2*x*y + b;
                x = xtmp;
                i++;
            }

            // Définition de la couleur pour le pixel courant
            int idx = (line * taille + col) * 3;
            image_buffer[idx] = (i > iterationmax) ? 0 : (4 * i) % 256;
            image_buffer[idx+1] = (i > iterationmax) ? 0 : (2 * i) % 256;
            image_buffer[idx+2] = (i > iterationmax) ? 0 : (6 * i) % 256;

            // On remplit le point opposé (Rotation 180°)
            int sym_idx = ((taille - 1 - line) * taille + (taille - 1 - col)) * 3;
            image_buffer[sym_idx] = image_buffer[idx];
            image_buffer[sym_idx+1] = image_buffer[idx+1];
            image_buffer[sym_idx+2] = image_buffer[idx+2];
        }
    }

    // end V1.1

    // Gestion du fichier de sortie
    string fileName = (argc > 1) ? argv[1] : "julia_symmetric";
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