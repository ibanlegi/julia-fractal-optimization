#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>

using namespace std;

#define DEST_FILE_DATA "./data"

inline string generateFileName() {
    time_t now = time(nullptr);
    tm* t = localtime(&now);

    string path = string(DEST_FILE_DATA) + "/%d-%m_%H-%M-%S.csv";
    char buffer[64];
    strftime(buffer, 64, path.c_str(), t);

    return string(buffer);
}



inline void printCSV(const string& filename, const vector<string>& columns) {
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
    for (const auto& col : columns) {
        if (headerIndex.count(col)) colIndices.push_back(headerIndex[col]);
        else cerr << "Warning: column not found: " << col << endl;
    }

    vector<vector<string>> rows;
    rows.push_back(headers);
    while (getline(file, line)) {
        stringstream ss(line);
        vector<string> row;
        string cell;
        while (ss >> cell) row.push_back(cell);
        rows.push_back(row);
    }

    vector<size_t> colWidths(colIndices.size(), 0);
    for (size_t i = 0; i < colIndices.size(); ++i) {
        int cidx = colIndices[i];
        size_t maxLen = 0;
        for (const auto& row : rows)
            if (cidx < (int)row.size()) maxLen = max(maxLen, row[cidx].size());
        colWidths[i] = maxLen;
    }

    auto pad = [](const string& s, size_t width) -> string {
        return s + string(width > s.size() ? width - s.size() : 0, ' ');
    };

    for (size_t i = 0; i < colIndices.size(); ++i) {
        int cidx = colIndices[i];
        cout << pad(headers[cidx], colWidths[i]);
        if (i != colIndices.size() - 1) cout << " | ";
    }
    cout << endl;

    for (size_t i = 0; i < colIndices.size(); ++i) {
        cout << string(colWidths[i], '-');
        if (i != colIndices.size() - 1) cout << "-+-";
    }
    cout << endl;

    for (size_t r = 1; r < rows.size(); ++r) {
        const auto& row = rows[r];
        for (size_t i = 0; i < colIndices.size(); ++i) {
            int cidx = colIndices[i];
            string cellText = (cidx < (int)row.size()) ? row[cidx] : "";
            cout << pad(cellText, colWidths[i]);
            if (i != colIndices.size() - 1) cout << " | ";
        }
        cout << endl;
    }
}

inline void parseEnergyCSV(const string& filename, double& cpuEnergy, double& memEnergy) {
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
    string cell;
    unordered_map<string,int> colIndex;
    int idx = 0;
    while (ss >> cell) {
        colIndex[cell] = idx++;
    }

    // Required columns
    vector<string> required = {"totalram","freeram","totalswap","freeswap"};
    for (auto& col : required) {
        if (!colIndex.count(col)) {
            cerr << "CSV missing required column: " << col << endl;
            return;
        }
    }

    cpuEnergy = 0.0;
    memEnergy = 0.0;

    while (getline(file, line)) {
        stringstream ss(line);
        vector<double> values(idx, 0.0);
        double val;
        int col = 0;
        while (ss >> val) {
            if (col < idx) values[col++] = val;
        }
        // Approximation: CPU energy ~ totalram - freeram
        cpuEnergy += (values[colIndex["totalram"]] - values[colIndex["freeram"]]);
        // Memory energy ~ totalswap - freeswap
        memEnergy += (values[colIndex["totalswap"]] - values[colIndex["freeswap"]]);
    }
    file.close();
}

