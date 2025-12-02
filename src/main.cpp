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

// Timer 
struct Timer {
    timespec startTime{};
    timespec endTime{};

    void start() { 
        clock_gettime(CLOCK_MONOTONIC, &startTime); 
    }
    
    void stop()  { 
        clock_gettime(CLOCK_MONOTONIC, &endTime);
    }

    double elapsed() const {
        return (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_nsec - startTime.tv_nsec) / 1e9;
    }
};

// Generate filename from timestamp
string generateFileName() {
    time_t now = time(nullptr);
    tm* t = localtime(&now);
    char buffer[64];
    strftime(buffer, 64, "%d-%m_%H-%M-%S", t);
    return buffer;
}

// Class: MojitosMonitor (Responsible for building and executing Mojitos command)
class MojitosMonitor {
private:
    string filePath;
    string frequency;
    string sudoCmd;
    string userCmd;

    string buildCommand() const {
        return sudoCmd + " ../mojitos/bin/mojitos -r -m -e"
             + " -f " + frequency
             + " -o " + filePath
             + " -- " + userCmd;
    }

public:
    MojitosMonitor(
        const string& fileName,
        const string& freq,
        const string& sudo,
        const string& cmd
    ) :
        filePath(string(PATH_DATA) + fileName + ".csv"),
        frequency(freq),
        sudoCmd(sudo),
        userCmd(cmd) {}

    bool run() {
        cout << "[MOJITOS] Generating CSV → " << filePath << endl;

        int ret = system(buildCommand().c_str());
        if (ret != 0) {
            cerr << "ERROR: Mojitos or command failed" << endl;
            return false;
        }
        return true;
    }

    string getDataFile() const { return filePath; }
};

// Class: EnergyReport (Reads energy CSV and extracts metrics)
class EnergyReport {
private:
    double cpuEnergy = 0.0;
    double memEnergy = 0.0;
    double totalEnergy = 0.0;

public:
    double get_cpuEnergy() {
        return cpuEnergy;
    } 

    double get_memEnergy() {
        return memEnergy;
    }

    double get_totalEnergy() {
        return totalEnergy;
    }

    bool parse(const string& fileName) {
        string filePath = string(PATH_DATA) + fileName + ".csv";
        ifstream file(filePath);

        if (!file) {
            cerr << "ERROR: Cannot open file " << filePath << endl;
            return false;
        }

        string header;
        if (!getline(file, header)) {
            cerr << "ERROR: File empty: " << filePath << endl;
            return false;
        }

        stringstream ss(header);
        unordered_map<string,int> colIndex;
        string col;
        int idx = 0;
        while (ss >> col) colIndex[col] = idx++;

        // Check required columns
        vector<string> required = {"totalram","freeram","totalswap","freeswap"};
        for (auto& r : required) {
            if (!colIndex.count(r)) {
                cerr << "ERROR: Missing column " << r << endl;
                return false;
            }
        }

        string line;
        while (getline(file, line)) {
            stringstream row(line);
            vector<double> vals(idx, 0.0);

            for (int i = 0; i < idx; i++) {
                if (!(row >> vals[i])) break;
            }

            cpuEnergy += vals[colIndex["totalram"]] - vals[colIndex["freeram"]];
            memEnergy += vals[colIndex["totalswap"]] - vals[colIndex["freeswap"]];
            totalEnergy = cpuEnergy + memEnergy;
        }

        return true;
    }
};

// MAIN
int main(int argc, char* argv[]) {
    const char* userCmd = nullptr;
    string fileName;
    string frequency = "10";
    string sudoCmd = "sudo-g5k";
    bool printOutput = false;

    // Parse args
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if      (arg == "-p"    && i + 1 < argc) userCmd     = argv[++i];
        else if (arg == "-d"    && i + 1 < argc) fileName    = argv[++i];
        else if (arg == "-f"    && i + 1 < argc) frequency   = argv[++i];
        else if (arg == "-sudo" && i + 1 < argc) sudoCmd     = argv[++i];
        else if (arg == "-print")                printOutput = true;
    }

    if (!userCmd) {
        cerr << "Usage: ./main -p \"command\" [-d name] [-f freq] [-print] [-sudo cmd]\n";
        return 1;
    }

    if (fileName.empty()) fileName = generateFileName();

    MojitosMonitor monitor(fileName, frequency, sudoCmd, userCmd);

    Timer timer;
    timer.start();
    if (!monitor.run()) return 1;
    timer.stop();

    double execTime = timer.elapsed();

    EnergyReport report;
    report.parse(fileName);

    //double totalEnergy = report.get_cpuEnergy() + report.get_memEnergy();
    double power = (execTime > 0) ? report.get_totalEnergy() / execTime : 0.0;

    if (printOutput) { 
        cout << fileName << "," << execTime << "," << report.get_cpuEnergy() << "," << report.get_memEnergy() << "," << report.get_totalEnergy() << "," << power << endl; 
    }else{ 
        cout << "Data file: " << fileName << endl; 
        cout << "Frequency: " << frequency << endl; 
        cout << "Sudo command: " << sudoCmd << endl; 
        cout << "Executing: " << userCmd << endl; 
        
        cout << "\n===== Measurement Results =====\n"; 
        cout << "Execution time: " << execTime << " s\n"; 
        cout << "CPU energy: " << report.get_cpuEnergy() << " J\n"; 
        cout << "Memory energy: " << report.get_memEnergy() << " J\n"; 
        cout << "===============================\n"; 
    } 
    

    string cmd = string("make convert FILE=") + PATH_PPM + fileName + ".ppm";
    int ret = system(cmd.c_str()); 
    if (ret != 0) cerr << "Command failed with code " << ret << endl; 
        
    return 0;

}
