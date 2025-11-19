#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <ctime>
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

#define DEST_FILE_DATA "./data"
const vector<string> DATA_CSV = {"#timestamp", "user", "system"};

void printCSV(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Cannot open file: " << filename << endl;
        return;
    }

    string line;
    if (!getline(file, line)) {
        cerr << "File is empty: " << filename << endl;
        return;
    }

    stringstream ss(line);
    string headerCell;
    vector<string> headers;
    unordered_map<string, int> headerIndex;
    int idx = 0;
    while (ss >> headerCell) {
        headers.push_back(headerCell);
        headerIndex[headerCell] = idx++;
    }

    vector<int> colIndices;
    for (const string& colName : DATA_CSV) {
        if (headerIndex.count(colName))
            colIndices.push_back(headerIndex[colName]);
        else
            cerr << "Warning: column not found: " << colName << endl;
    }

    for (size_t i = 0; i < colIndices.size(); ++i) {
        cout << headers[colIndices[i]];
        if (i != colIndices.size() - 1) cout << " ";
    }
    cout << endl;

    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;
        while (ss >> cell) row.push_back(cell);

        for (size_t i = 0; i < colIndices.size(); ++i) {
            int cidx = colIndices[i];
            if (cidx < (int)row.size()) {
                cout << row[cidx];
                if (i != colIndices.size() - 1) cout << " ";
            }
        }
        cout << endl;
    }
}

string generateNameFile() {
    time_t now = time(nullptr);
    tm* t = localtime(&now);

    string path = string(DEST_FILE_DATA) + "/%d-%m_%H-%M-%S.csv";
    char buffer[64];
    strftime(buffer, 64, path.c_str(), t);

    return string(buffer);
}


int main(int argc, char *argv[]) {
    const char* cmd_param = nullptr;
    string dataFile;
    string frequency = "10";
    bool print_output = false;

    for (int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-p") == 0 && i+1 < argc){
            cmd_param = argv[i+1];
            i++;
        }
        else if(strcmp(argv[i], "-d") == 0 && i+1 < argc){
            dataFile = argv[i+1];
            i++;
        }
        else if(strcmp(argv[i], "-f") == 0 && i+1 < argc){
            frequency = argv[i+1];
            i++;
        }
        else if(strcmp(argv[i], "-v") == 0){
            print_output = true;
        }
    }

    if(dataFile.empty())
        dataFile = generateNameFile();

    if(!cmd_param){
        cerr << "Usage : " << argv[0] << " -p \"command\" -d \"datafile\"" << endl;
        return 1;
    }

    cout << "Commande execute :\t   " << cmd_param << endl;
    cout << "Output file destination :  " << dataFile << endl;
    cout << "Frequency : \t\t   " << frequency << endl;

    string cmd_mojitos = "sudo-g5k ../mojitos/bin/mojitos  -r -u -f "
                        + frequency
                        + " -o " 
                        + dataFile 
                        + " & echo $!";

    FILE* pipe = popen(cmd_mojitos.c_str(), "r");
    if (!pipe) {
        cerr << "Failed to launch mojitos" << endl;
        return 1;
    }

    int pid = 0;
    if (fscanf(pipe, "%d", &pid) != 1) {
        cerr << "Failed to read PID from mojitos" << endl;
        pclose(pipe);
        return 1;
    }
    pclose(pipe);

    int ret = system(cmd_param);
    if (ret != 0) {
        cerr << "User command failed: " << cmd_param << endl;
    }
    cout << "User command completed" << endl;

    string kill_cmd = "sudo kill " + to_string(pid);
    ret = system(kill_cmd.c_str());
    if (ret != 0) {
        cerr << "Failed to kill mojitos process (" << pid << ")" << endl;
    }


    if (print_output) {
        cout << "Output " << dataFile << endl;
        printCSV(dataFile);
    }

    return 0;
}

