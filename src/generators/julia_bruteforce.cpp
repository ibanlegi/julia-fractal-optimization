/* Version 0: Brute-force (The Reference)
Compile : g++ -Wall -O2 -std=c++17 -o generators/julia_bruteforce src/generators/julia_bruteforce.cpp
Execute : ./main -p "./generators/julia_bruteforce" -f 10 -d "reference"
-> reference
*/

#include <iostream>
#include <fstream>
#include <string>
#include <string>
#include <ctime>

using namespace std;

#define DEST_FILE_DATA "./export-pictures/ppm"

string generateFileName() {
    time_t now = time(nullptr);
    tm* t = localtime(&now);

    char buffer[64];
    strftime(buffer, sizeof(buffer), "no_name-%d-%m_%H-%M-%S.ppm", t);

    return buffer;
}

int main(int argc, char* argv[]) {
    const int taille = 10000;
    const double xmin = -1.0, xmax = 1.0;
    const double ymin = -1.0, ymax = 1.0;
    const int iterationmax = 1000;

    // Paramètres de Julia
    const double a = -0.8;
    const double b =  0.156;

    // --- Gestion du nom de fichier ---
    string fileName;
    if (argc > 1) fileName = argv[1];
    else fileName = generateFileName();


    string filePath = string(DEST_FILE_DATA) + "/" + argv[1] + ".ppm";
    // ---------------------------------

    ofstream out(filePath, ios::binary);
    if (!out) {
        cerr << "Cannot open file " << filePath << endl;
        return 1;
    }

    // En-tête PPM
    out << "P6\n" << taille << " " << taille << "\n255\n";

    for (int line = 0; line < taille; ++line) {
        for (int col = 0; col < taille; ++col) {
            int i = 1;
            double x = xmin + col * (xmax - xmin) / taille;
            double y = ymax - line * (ymax - ymin) / taille;

            while (i <= iterationmax && (x*x + y*y) <= 4.0) {
                double xtmp = x*x - y*y + a;
                y = 2*x*y + b;
                x = xtmp;
                i++;
            }

            unsigned char rgb[3];
            if (i > iterationmax) {
                rgb[0] = rgb[1] = rgb[2] = 0;
            } else {
                rgb[0] = (4 * i) % 256;
                rgb[1] = (2 * i) % 256;
                rgb[2] = (6 * i) % 256;
            }

            out.write(reinterpret_cast<char*>(rgb), 3);
        }
    }

    out.close();
    cout << "Image generated: " << filePath << endl;

    return 0;
}