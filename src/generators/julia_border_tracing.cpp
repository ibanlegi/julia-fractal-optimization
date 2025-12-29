/* Version 3: Border Tracing
Compile : g++ -Wall -O2 -std=c++17 -o generators/julia_border_tracing src/generators/julia_border_tracing.cpp
Execute : ./main -p "./generators/julia_border_tracing" -f 10 -d "v3_border_tracing"
*/

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

// Paramètres globaux pour simplifier les appels récursifs
const int taille = 10000;
const int iterationmax = 1000;
const double xmin = -1.0, xmax = 1.0;
const double ymin = -1.0, ymax = 1.0;
const double a = -0.8, b = 0.156;

// Fonction de calcul pour un pixel unique
int computeJulia(double x, double y) {
    int i = 1;
    while (i <= iterationmax && (x*x + y*y) <= 4.0) {
        double xtmp = x*x - y*y + a;
        y = 2*x*y + b;
        x = xtmp;
        i++;
    }
    return i;
}

RGB getRGB(int i) {
    if (i > iterationmax) return {0, 0, 0};
    return { (unsigned char)((4 * i) % 256), 
             (unsigned char)((2 * i) % 256), 
             (unsigned char)((6 * i) % 256) };
}

// Fonction récursive de remplissage par blocs (Mariani-Silver algorithm)
void renderBlock(int x1, int y1, int x2, int y2, vector<RGB>& image) {
    if (x1 > x2 || y1 > y2) return;

    // Calculer les bords du bloc
    int first_val = -1;
    bool uniform = true;

    // Tester le périmètre du rectangle
    for (int i = x1; i <= x2; i++) {
        for (int j : {y1, y2}) { // Lignes haut et bas
            double x_coord = xmin + i * (xmax - xmin) / taille;
            double y_coord = ymax - j * (ymax - ymin) / taille;
            int val = computeJulia(x_coord, y_coord);
            image[j * taille + i] = getRGB(val);
            if (first_val == -1) first_val = val;
            else if (val != first_val) uniform = false;
        }
    }
    for (int j = y1 + 1; j < y2; j++) {
        for (int i : {x1, x2}) { // Colonnes gauche et droite
            double x_coord = xmin + i * (xmax - xmin) / taille;
            double y_coord = ymax - j * (ymax - ymin) / taille;
            int val = computeJulia(x_coord, y_coord);
            image[j * taille + i] = getRGB(val);
            if (val != first_val) uniform = false;
        }
    }

    // Si tout le bord est uniforme et le bloc est assez grand, on remplit l'intérieur
    if (uniform && (x2 - x1) > 1 && (y2 - y1) > 1) {
        RGB color = getRGB(first_val);
        for (int j = y1 + 1; j < y2; j++) {
            for (int i = x1 + 1; i < x2; i++) {
                image[j * taille + i] = color;
            }
        }
    } 
    // Sinon, on divise le bloc en 4 (Quadtree) si le bloc est encore divisible
    else if ((x2 - x1) > 1 || (y2 - y1) > 1) {
        int midX = (x1 + x2) / 2;
        int midY = (y1 + y2) / 2;
        renderBlock(x1, y1, midX, midY, image);         // Top-Left
        renderBlock(midX + 1, y1, x2, midY, image);     // Top-Right
        renderBlock(x1, midY + 1, midX, y2, image);     // Bottom-Left
        renderBlock(midX + 1, midY + 1, x2, y2, image); // Bottom-Right
    }
}

int main(int argc, char* argv[]) {
    string fileName;
    if (argc > 1) fileName = argv[1];
    else fileName = generateFileName();

    string filePath = string(DEST_FILE_DATA) + "/" + argv[1] + ".ppm";
   
    vector<RGB> image(taille * taille);

    // Lancement de l'algorithme sur l'image entière
    renderBlock(0, 0, taille - 1, taille - 1, image);

    ofstream out(filePath, ios::binary);
    if (out) {
        out << "P6\n" << taille << " " << taille << "\n255\n";
        out.write(reinterpret_cast<char*>(image.data()), image.size() * 3);
        out.close();
    }

    return 0;
}