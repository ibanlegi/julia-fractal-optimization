#include <stdio.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <sstream>

#include "utils/csv_utils.h"

using namespace std;

// Fonction utilitaire pour split une chaîne par un délimiteur
vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream ss(str);
    while (getline(ss, token, delimiter)) {
        if(!token.empty())
            tokens.push_back(token);
    }
    return tokens;
}

int main(int argc, char *argv[]) { 
    string dataFile;
    vector<string> data_csv = {"totalram","freeram","totalswap","freeswap"};

    for (int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-d") == 0 && i+1 < argc) {
            dataFile = argv[++i];
        } else if(strcmp(argv[i], "-p") == 0 && i+1 < argc) {
            // Split les paramètres passés en virgule
            data_csv = split(argv[++i], ',');
        }
    }

    if(dataFile.empty()){
        cerr << "Usage: " << argv[0] << " -d \"data-file\" [-p \"param1,param2,...\"]" << endl;
        return 1;
    }

    cout << "Data file: " << dataFile << endl;
    cout << "Parameters: ";
    for(const auto& param : data_csv) cout << param << " ";
    cout << endl;

    printCSV(dataFile, data_csv);

    return 0;
}
