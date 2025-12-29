#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <omp.h>

using namespace std;

#define DEST_FILE_DATA "./export-pictures/ppm"

struct RGB {
    unsigned char r, g, b;
};

// Paramètres
const int taille = 10000;
const int iterationmax = 1000;
const double xmin = -1.0, xmax = 1.0;
const double ymin = -1.0, ymax = 1.0;
const double a = -0.8, b = 0.156;

// Cache et compteurs globaux (atomiques pour le multi-threading)
vector<int> iter_cache(taille * taille, -1);
unsigned long long count_computed = 0;

// Fonction de calcul avec protection contre le recalcul
int getJulia(int col, int line) {
    int idx = line * taille + col;
    
    // Si déjà dans le cache, on renvoie
    if (iter_cache[idx] != -1) return iter_cache[idx];

    // Calcul mathématique
    double x = xmin + col * (xmax - xmin) / taille;
    double y = ymax - line * (ymax - ymin) / taille;
    
    // Shape checking rapide (Cercle de convergence)
    if (x*x + y*y > 4.0) {
        #pragma omp atomic
        count_computed++;
        return iter_cache[idx] = 1;
    }

    int i = 1;
    while (i <= iterationmax && (x*x + y*y) <= 4.0) {
        double xtmp = x*x - y*y + a;
        y = 2*x*y + b;
        x = xtmp;
        i++;
    }

    #pragma omp atomic
    count_computed++;
    return iter_cache[idx] = i;
}

RGB toRGB(int i) {
    if (i > iterationmax) return {0, 0, 0};
    return {(unsigned char)((4 * i) % 256), (unsigned char)((2 * i) % 256), (unsigned char)((6 * i) % 256)};
}

// Algorithme de remplissage de bloc
void renderBlock(int x1, int y1, int x2, int y2) {
    if (x1 > x2 || y1 > y2) return;

    int first_val = getJulia(x1, y1);
    bool uniform = true;

    // Vérification des bords
    for (int i = x1; i <= x2 && uniform; ++i) {
        if (getJulia(i, y1) != first_val) uniform = false;
        if (getJulia(i, y2) != first_val) uniform = false;
    }
    for (int j = y1 + 1; j < y2 && uniform; ++j) {
        if (getJulia(x1, j) != first_val) uniform = false;
        if (getJulia(x2, j) != first_val) uniform = false;
    }

    if (uniform) {
        // Remplissage du cache pour tout le bloc
        for (int j = y1; j <= y2; ++j) {
            for (int i = x1; i <= x2; ++i) {
                iter_cache[j * taille + i] = first_val;
            }
        }
    } else {
        // Division
        int mx = (x1 + x2) / 2;
        int my = (y1 + y2) / 2;
        if (x1 != x2 || y1 != y2) {
            renderBlock(x1, y1, mx, my);
            renderBlock(mx + 1, y1, x2, my);
            renderBlock(x1, my + 1, mx, y2);
            renderBlock(mx + 1, my + 1, x2, y2);
        }
    }
}

int main(int argc, char* argv[]) {
    string name = (argc > 1) ? argv[1] : "fully_optimized";
    string filePath = string(DEST_FILE_DATA) + "/" + name + ".ppm";

    // 1. Parallélisation de haut niveau (On divise l'image en bandes)
    // On ne traite que la moitié (Symmetry)
    #pragma omp parallel for schedule(dynamic)
    for (int b = 0; b < 10; ++b) {
        int y_start = b * (taille / 20);
        int y_end = (b + 1) * (taille / 20) - 1;
        renderBlock(0, y_start, taille - 1, y_end);
    }

    // 2. Application de la symétrie centrale sur le cache
    for (int j = 0; j < taille / 2; ++j) {
        for (int i = 0; i < taille; ++i) {
            iter_cache[(taille - 1 - j) * taille + (taille - 1 - i)] = iter_cache[j * taille + i];
        }
    }

    // 3. Écriture finale
    ofstream out(filePath, ios::binary);
    out << "P6\n" << taille << " " << taille << "\n255\n";
    
    vector<RGB> buffer(taille);
    for (int j = 0; j < taille; ++j) {
        for (int i = 0; i < taille; ++i) {
            buffer[i] = toRGB(iter_cache[j * taille + i]);
        }
        out.write(reinterpret_cast<char*>(buffer.data()), taille * 3);
    }

    /*cout << "--- Stats Finales ---" << endl;
    cout << "Pixels calculés : " << count_computed << endl;
    cout << "Efficacité : " << 100.0 - ((double)count_computed / (taille * taille) * 100.0) << "% économisés" << endl;
    */

    return 0;
}