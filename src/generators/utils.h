#include <string>
#include <ctime>

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