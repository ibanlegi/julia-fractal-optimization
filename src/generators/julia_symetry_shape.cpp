#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

    // Allocation de l'image (nécessaire pour la recopie par symétrie)
    vector<RGB> image(taille * taille);

    // OPTIMISATION 1 : On ne calcule que la MOITIÉ des lignes (taille / 2)
    for (int line = 0; line < taille / 2; ++line) {
        double y_coord = ymax - line * (ymax - ymin) / taille;
        
        for (int col = 0; col < taille; ++col) {
            double x = xmin + col * (xmax - xmin) / taille;
            double y = y_coord;
            
            // OPTIMISATION 2 : Early Exit (Shape checking simple)
            // Si le point est déjà hors du cercle de rayon 2
            if (x*x + y*y > 4.0) {
                RGB color = {4, 2, 6}; // Couleur itération 1
                image[line * taille + col] = color;
                // Symétrie centrale : point opposé
                image[(taille - 1 - line) * taille + (taille - 1 - col)] = color;
                continue;
            }

            int i = 1;
            while (i <= iterationmax && (x*x + y*y) <= 4.0) {
                double xtmp = x*x - y*y + a;
                y = 2*x*y + b;
                x = xtmp;
                i++;
            }

            RGB color;
            if (i > iterationmax) {
                color = {0, 0, 0};
            } else {
                color.r = (unsigned char)((4 * i) % 256);
                color.g = (unsigned char)((2 * i) % 256);
                color.b = (unsigned char)((6 * i) % 256);
            }

            // On remplit le pixel actuel
            image[line * taille + col] = color;
            
            // OPTIMISATION 1 (Suite) : On recopie le pixel symétrique
            // Le symétrique de (line, col) est (taille-1-line, taille-1-col)
            image[(taille - 1 - line) * taille + (taille - 1 - col)] = color;
        }
    }

    // Écriture binaire
    ofstream out(filePath, ios::binary);
    if (out) {
        out << "P6\n" << taille << " " << taille << "\n255\n";
        out.write(reinterpret_cast<char*>(image.data()), image.size() * 3);
        out.close();
    }

    return 0;
}