#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cstring>

#include "utils/csv_utils.h"
#include "utils/timing_utils.h"
#include "utils/mojitos_utils.h"

using namespace std;

#define DEST_FILE_DATA "./data"
const vector<string> DATA_CSV = {"totalram","freeram","totalswap","freeswap"};

int main(int argc, char *argv[]) {
    const char* userCmd = nullptr;
    string dataFile;
    string frequency = "10";
    bool printOutput = false;
    string sudoCmd = "sudo-g5k";

    // --- Parse command line arguments ---
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0 && i+1 < argc) userCmd = argv[++i];
        else if (strcmp(argv[i], "-d") == 0 && i+1 < argc) dataFile = argv[++i];
        else if (strcmp(argv[i], "-f") == 0 && i+1 < argc) frequency = argv[++i];
        else if (strcmp(argv[i], "-print") == 0) printOutput = true;
        else if (strcmp(argv[i], "-sudo") == 0 && i+1 < argc) sudoCmd = argv[++i];
    }

    if (dataFile.empty()) dataFile = generateFileName();

    if (!userCmd) { 
        cerr << "Usage: " << argv[0] << " -p \"command_to_run\" [-d \"output_file\"] [-f frequency] [-print] [-sudo sudo_command]" << endl; 
        cerr << "  -p : command to execute\n"; 
        cerr << "  -d : output CSV file (optional, default generated automatically)\n"; 
        cerr << "  -f : measurement frequency in seconds (optional, default 10)\n"; 
        cerr << "  -print : print CSV output to console (optional)\n";
        cerr << "  -sudo : sudo command (optional, default sudo-g5k)" << endl; 
        return 1; 
    }

    // --- Start Mojitos monitor and run command ---
    MojitosMonitor monitor(dataFile, frequency, sudoCmd, userCmd);

    Timer timer;
    timer.start();
    if (!monitor.start()) return 1;   // Mojitos executes the command
    timer.stop();

    double execTime = timer.elapsed();
    double energyCPU = 0.0, energyMem = 0.0;

    // --- Parse CSV to get energy measurements ---
    parseEnergyCSV(dataFile, energyCPU, energyMem);

    double totalEnergy = energyCPU + energyMem;
    double power = (execTime > 0) ? totalEnergy / execTime : 0.0;

    // --- Optional CSV output ---
    if (printOutput) {
        cout << dataFile << "," << execTime << "," << energyCPU << "," << energyMem << "," << totalEnergy << "," << power << endl;
    } else{
        cout << "Data file: " << dataFile << endl;
        cout << "Frequency: " << frequency << endl;
        cout << "Sudo command: " << sudoCmd << endl;
        cout << "Executing: " << userCmd << endl;
        
        cout << "\n===== Measurement Results =====\n";
        cout << "Execution time:     " << execTime << " s\n";
        cout << "CPU energy:         " << energyCPU << " J\n";
        cout << "Memory energy:      " << energyMem << " J\n";
        cout << "Total energy:       " << totalEnergy << " J\n";
        cout << "Average power:      " << power << " W\n";
        cout << "===============================\n";
    }

    return 0;
}
