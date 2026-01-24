#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <cstdlib>
#include <cstring>

using namespace std;

#define PATH_DATA "./data/"
#define PATH_PPM  "./export-pictures/ppm/"
#define PATH_JPEG "./export-pictures/jpeg/"

string generateFileName() {
    time_t now = time(nullptr);
    tm* t = localtime(&now);
    char buffer[64];
    strftime(buffer, 64, "%d-%m_%H-%M-%S", t);
    return buffer;
}

class MojitosMonitor {
private:

    string filePath;
    string fileName;
    string frequency;
    string sudoCmd;
    string userCmd;

    string buildCommand() const {
        return sudoCmd + " ../mojitos/bin/mojitos -r"
             + " -f " + frequency
             + " -o " + filePath
             + " -- " + userCmd + " " + fileName;
    }

public:
    MojitosMonitor(
        const string& file_name,
        const string& freq,
        const string& sudo,
        const string& cmd
    ) :
        filePath(string(PATH_DATA) + file_name + ".csv"),
        fileName(file_name),
        frequency(freq),
        sudoCmd(sudo),
        userCmd(cmd) {}

    bool run() {
        //cout << "[MOJITOS] Generating CSV --> " << filePath << endl;

        int ret = system(buildCommand().c_str());
        if (ret != 0) {
            cerr << "ERROR: Mojitos or command failed" << endl;
            return false;
        }
        return true;
    }

    string getDataFile() const { return filePath; }
};

class EnergyReport {
private:
    double cpuEnergy = 0.0;      // J
    double memEnergy = 0.0;      // J
    double totalEnergy = 0.0;    // J
    double executionTime = 0.0;  // s
    double maxPower = 0.0;       // W

public:
    double get_cpuEnergy()     { return cpuEnergy; } 
    double get_memEnergy()     { return memEnergy; }
    double get_totalEnergy()   { return totalEnergy; }
    double get_executionTime() { return executionTime; }
    double get_maxPower()      { return maxPower; }

    bool parse(const string& fileName) {
        string filePath = "./data/" + fileName + ".csv";
        ifstream file(filePath);
        if (!file) return false;

        string header;
        if (!getline(file, header)) return false;

        stringstream ss(header);
        unordered_map<string, int> col;
        string colName;
        int idx = 0;
        while (ss >> colName) col[colName] = idx++;

        string cpuKey = col.count("package-00") ? "package-00" : "package-0";
        string memKey = "dram0";

        string line;
        bool first = true;
        double firstTS = 0.0, lastTS = 0.0;
        double firstCPU = 0.0, lastCPU = 0.0;
        double firstMem = 0.0, lastMem = 0.0;
        double prevTS = 0.0, prevCPU = 0.0, prevMem = 0.0;

        while (getline(file, line)) {
            stringstream row(line);
            vector<double> vals(idx, 0.0);
            for (int i = 0; i < idx; i++) row >> vals[i];

            double ts = vals[col["#timestamp"]];
            double cpu = vals[col[cpuKey]];
            double mem = col.count(memKey) ? vals[col[memKey]] : 0.0;

            if (first) {
                firstTS = ts; firstCPU = cpu; firstMem = mem;
                first = false;
            } else {
                // Calcul de la puissance instantanée (delta E / delta t)
                double dt = ts - prevTS;
                if (dt > 0) {
                    double dE = ((cpu - prevCPU) + (mem - prevMem)) / 1e6; 
                    double instantPower = dE / dt;
                    if (instantPower > maxPower) maxPower = instantPower;
                }
            }
            lastTS = ts; lastCPU = cpu; lastMem = mem;
            prevTS = ts; prevCPU = cpu; prevMem = mem;
        }

        // Calcul final du temps d'exécution
        executionTime = lastTS - firstTS; 
        
        // Calcul final de l'énergie en J
        cpuEnergy = (lastCPU - firstCPU) / 1e6;
        memEnergy = (lastMem - firstMem) / 1e6;
        totalEnergy = cpuEnergy + memEnergy;

        return true;
    }
};

int main(int argc, char* argv[]) {
    const char* userCmd = nullptr;
    string fileName;
    string frequency = "10";
    string sudoCmd = "sudo-g5k";
    bool printOutput = false;
    bool exportJpeg = false;

    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if      (arg == "-p"    && i + 1 < argc) userCmd     = argv[++i];
        else if (arg == "-d"    && i + 1 < argc) {fileName    = argv[++i]; exportJpeg = true;}
        else if (arg == "-f"    && i + 1 < argc) frequency   = argv[++i];
        else if (arg == "-sudo" && i + 1 < argc) sudoCmd     = argv[++i];
        else if (arg == "-print")                printOutput = true; // Si true, adapté pour la lecture du script exec.py
    }

    if (!userCmd) {
        cerr << "Usage: ./main -p \"command\" [-d <name>] [-f freq] [-print] [-sudo <cmd>]\n";
        return 1;
    }

    if (fileName.empty()) fileName = generateFileName();

    MojitosMonitor monitor(fileName, frequency, sudoCmd, userCmd);

    if (!monitor.run()) return 1;

    EnergyReport report;
    report.parse(fileName);

    if (printOutput) { 
        cout << fileName << "," << report.get_executionTime() << "," << report.get_totalEnergy() << "," << report.get_maxPower() << endl; 
    }else{ 
        cout << "Data file: " << fileName << endl; 
        cout << "Frequency: " << frequency << endl; 
        cout << "Sudo command: " << sudoCmd << endl; 
        cout << "Executing: " << userCmd << endl; 
        
        cout << "\n===== Measurement Results =====\n"; 
        cout << "Execution time: " << report.get_executionTime() << " s\n"; 
        cout << "CPU energy: " << report.get_cpuEnergy() << " J\n"; 
        cout << "Memory energy: " << report.get_memEnergy() << " J\n"; 
        cout << "(CPU + Memory): " << report.get_totalEnergy() << " J\n"; 
        cout << "Max power: " << report.get_maxPower() << " W\n";
        cout << "===============================\n"; 
    } 
    
    if(exportJpeg && !(printOutput)){
        string cmd = string("make convert FILE=") + PATH_PPM + fileName + ".ppm";
        int ret = system(cmd.c_str()); 
        if (ret != 0) cerr << "Command failed with code " << ret << endl;
    }
       
    return 0;

}
