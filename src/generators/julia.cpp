#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <cmath>
#include <sys/stat.h>

using namespace std;

#define DEST_FILE_DATA "./export-pictures/ppm"

// Génère un nom de fichier horodaté
string generateFileName() {
    time_t now = time(nullptr);
    tm* t = localtime(&now);

    char buffer[64];
    strftime(buffer, sizeof(buffer), "%d-%m_%H-%M-%S.ppm", t);

    return string(DEST_FILE_DATA) + "/" + buffer;
}

int main() {
    const int taille = 10000;
    const double xmin = -1.0, xmax = 1.0;
    const double ymin = -1.0, ymax = 1.0;
    const int iterationmax = 1000;

    // Paramètres de Julia
    const double a = -0.8;
    const double b =  0.156;

    string filename = generateFileName();
    ofstream out(filename, ios::binary);
    if (!out) {
        cerr << "Cannot open file " << filename << endl;
        return 1;
    }

    // En-tête PPM (P6 = binaire)
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
    cout << "Image generated: " << filename << endl;

    return 0;
}
