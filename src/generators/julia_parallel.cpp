/* Version 1: Parallelization (Multi-threading)
Compile : g++ -Wall -O2 -std=c++17 -fopenmp -o generators/julia_parallel src/generators/julia_parallel.cpp
Execute : ./main -p "./generators/julia_parallel" -f 10 -d "parallel"
-> v1_parallel


## 1er test
Résultat (reference VS v1_parallel) :
Score SSIM : 100 %
Score PSNR : 361.2020 dB


*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <omp.h>

using namespace std;

#define DEST_FILE_DATA "./export-pictures/ppm"

string generateFileName() {
    time_t now = time(nullptr);
    tm* t = localtime(&now);

    char buffer[64];
    strftime(buffer, sizeof(buffer), "no_name-%d-%m_%H-%M-%S.ppm", t);

    return buffer;
}

struct RGB {
    unsigned char r, g, b;
};

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

    // 2. Préparation de la mémoire (L'image entière en RAM)
    // Taille : 10000x10000x3 octets = 300 Mo
    vector<RGB> image(taille * taille);

    //cout << "Parallel computing across " << omp_get_max_threads() << " cores..." << endl;

    // 3. Parallélisation de la boucle externe
    #pragma omp parallel for schedule(dynamic)
    for (int line = 0; line < taille; ++line) {
        for (int col = 0; col < taille; ++col) {
            double x = xmin + col * (xmax - xmin) / taille;
            double y = ymax - line * (ymax - ymin) / taille;
            int i = 1;

            while (i <= iterationmax && (x*x + y*y) <= 4.0) {
                double xtmp = x*x - y*y + a;
                y = 2*x*y + b;
                x = xtmp;
                i++;
            }

            RGB pixel;
            if (i > iterationmax) {
                pixel = {0, 0, 0};
            } else {
                pixel.r = (unsigned char)((4 * i) % 256);
                pixel.g = (unsigned char)((2 * i) % 256);
                pixel.b = (unsigned char)((6 * i) % 256);
            }

            // On stocke dans le tableau au lieu d'écrire dans le fichier
            image[line * taille + col] = pixel;
        }
    }

    // 4. Écriture finale sur le disque
    ofstream out(filePath, ios::binary);
    if (!out) {
        cerr << "Cannot open file " << filePath << endl;
        return 1;
    }
    out << "P6\n" << taille << " " << taille << "\n255\n";
    out.write(reinterpret_cast<char*>(image.data()), image.size() * 3);
    out.close();

    //cout << "Image generated: " << filePath << endl;

    return 0;
}