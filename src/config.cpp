//
// Created by Kamil Sobierajski on 12/10/2025.
//

#include "../include/config.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

Config::Config() : iterations(1000), repetitions(10),
                   max_time(60.0), max_iterations(10000), show_progress(false),
                   output_file("results.csv"), method("nearest-neighbour") {}

Config ConfigReader::readConfig(const string& filename) {
    Config config;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Cannot open config file: " << filename << endl;
        return config;
    }

    string line;
    int line_number = 0;
    while (getline(file, line)) {
        line_number++;

        // Usuwanie bialych znakow
        size_t start = line.find_first_not_of(" \t\r\n");
        size_t end = line.find_last_not_of(" \t\r\n");

        if (start == string::npos) continue; // Pusta linia

        line = line.substr(start, end - start + 1);

        // Pomijanie komentarzy
        if (line.empty() || line[0] == '#') continue;

        size_t pos = line.find('=');
        if (pos == string::npos) {
            cerr << "Warning: Invalid config line " << line_number << ": " << line << endl;
            continue;
        }

        // Ekstrakcja klucza i waertosci
        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);

        // Usuwania b. znakow dla klucza i wartosci
        key.erase(remove_if(key.begin(), key.end(), ::isspace), key.end());
        value.erase(remove_if(value.begin(), value.end(), ::isspace), value.end());

        if (key == "data_file") {
            config.data_file = value;
        } else if (key == "optimal_value") {
            config.optimal_value = stod(value);
        } else if (key == "iterations") {
            config.iterations = stoi(value);
        } else if (key == "repetitions") {
            config.repetitions = stoi(value);
        } else if (key == "max_time") {
            config.max_time = stod(value);
        } else if (key == "max_iterations") {
            config.max_iterations = stoi(value);
        } else if (key == "show_progress") {
            config.show_progress = (value == "true");
        } else if (key == "output_file") {
            config.output_file = value;
        } else if (key == "method") {
            config.method = value;
        } else {
            cerr << "Warning: Unknown config key: " << key << endl;
        }
    }

    file.close();
    return config;
}