#pragma once
#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

class MojitosMonitor {
private:
    string dataFile;
    string frequency;
    string sudoCmd;
    string userCmd;

    string buildCommand() const {
        // Mojitos exécute la commande utilisateur et écrit le CSV
        return sudoCmd + " ../mojitos/bin/mojitos -r -m -e"
             + " -f " + frequency
             + " -o " + dataFile
             + " -- " + userCmd;
    }

public:
    MojitosMonitor(const string& file, const string& freq, const string& sudo, const string& cmd)
        : dataFile(file), frequency(freq), sudoCmd(sudo), userCmd(cmd) {}

    // Lancement de Mojitos + exécution de la commande utilisateur
    bool start() {
        string cmd = buildCommand();
        int ret = system(cmd.c_str());
        if (ret != 0) {
            cerr << "Failed to execute Mojitos or user command" << endl;
            return false;
        }
        return true;
    }

    string getDataFile() const { 
        return dataFile; 
    }
};
