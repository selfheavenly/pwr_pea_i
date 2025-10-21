//
// Created by Kamil Sobierajski on 12/10/2025.
//

#include "../include/tsp_instance.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

TSPInstance::TSPInstance()
    : size(0),
      weight_type(EdgeWeightType::EXPLICIT),
      weight_format(EdgeWeightFormat::FULL_MATRIX) {}

string TSPInstance::trim(const string& str) {
    auto start = str.begin();
    while (start != str.end() && isspace(*start)) {
        start++;
    }
    auto end = str.end();
    do {
        end--;
    } while (distance(start, end) > 0 && isspace(*end));
    return string(start, end + 1);
}

bool TSPInstance::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open data file: " << filename << endl;
        return false;
    }

    string line;
    bool reading_matrix = false;
    bool reading_tour = false;

    while (getline(file, line)) {
        line = trim(line);

        if (line.empty()) continue;

        // Sprawdź marker EOF
        if (line == "EOF" || line == "-1") {
            break;
        }

        // Sprawdź markery sekcji
        if (line == "EDGE_WEIGHT_SECTION") {
            reading_matrix = true;
            reading_tour = false;
            readDistanceMatrix(file);
            continue;
        }

        if (line == "TOUR_SECTION") {
            reading_tour = true;
            reading_matrix = false;
            // Wczytaj optymalną trasę jeśli obecna
            vector<int> tour;
            while (getline(file, line)) {
                line = trim(line);
                if (line == "-1" || line == "EOF") break;
                if (!line.empty()) {
                    tour.push_back(stoi(line) - 1); // Konwersja na indeksowanie od 0
                }
            }
            if (!tour.empty()) {
                optimal_tour = tour;
            }
            continue;
        }

        // Parsuj pola nagłówka
        if (!reading_matrix && !reading_tour) {
            parseHeader(line);
        }
    }

    file.close();

    // Oblicz długość optymalną jeśli trasa jest podana
    if (optimal_tour.has_value() && !distance_matrix.empty()) {
        optimal_length = calculateTourLength(optimal_tour.value());
    }

    return !distance_matrix.empty();
}

void TSPInstance::parseHeader(const string& line) {
    size_t colon_pos = line.find(':');
    if (colon_pos == string::npos) return;

    string key = trim(line.substr(0, colon_pos));
    string value = trim(line.substr(colon_pos + 1));

    if (key == "NAME") {
        name = value;
    } else if (key == "TYPE") {
        type = value;
    } else if (key == "COMMENT") {
        comment = value;
    } else if (key == "DIMENSION") {
        size = stoi(value);
        distance_matrix.resize(size, vector<double>(size, 0));
    } else if (key == "EDGE_WEIGHT_TYPE") {
        if (value == "EXPLICIT") weight_type = EdgeWeightType::EXPLICIT;
        else if (value == "EUC_2D") weight_type = EdgeWeightType::EUC_2D;
        else if (value == "GEO") weight_type = EdgeWeightType::GEO;
        else if (value == "ATT") weight_type = EdgeWeightType::ATT;
    } else if (key == "EDGE_WEIGHT_FORMAT") {
        if (value == "FULL_MATRIX") weight_format = EdgeWeightFormat::FULL_MATRIX;
        else if (value == "UPPER_ROW") weight_format = EdgeWeightFormat::UPPER_ROW;
        else if (value == "LOWER_ROW") weight_format = EdgeWeightFormat::LOWER_ROW;
        else if (value == "UPPER_DIAG_ROW") weight_format = EdgeWeightFormat::UPPER_DIAG_ROW;
        else if (value == "LOWER_DIAG_ROW") weight_format = EdgeWeightFormat::LOWER_DIAG_ROW;
    }
}

void TSPInstance::readDistanceMatrix(ifstream& file) {
    switch (weight_format) {
        case EdgeWeightFormat::FULL_MATRIX:
            readFullMatrix(file);
            break;
        case EdgeWeightFormat::UPPER_ROW:
            readUpperRow(file);
            break;
        case EdgeWeightFormat::LOWER_DIAG_ROW:
            readLowerDiagRow(file);
            break;
        default:
            cerr << "Warning: Unsupported weight format, trying FULL_MATRIX" << endl;
            readFullMatrix(file);
    }
}

void TSPInstance::readFullMatrix(ifstream& file) {
    string line;
    vector<double> all_values;

    // Wczytaj wszystkie wartości numeryczne
    while (getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;
        if (line == "EOF" || line == "-1" || line.find("SECTION") != string::npos) {
            // Cofnij wskaźnik pliku - POPRAWIONE
            auto pos = file.tellg();
            file.seekg(pos - static_cast<streamoff>(line.length()) - static_cast<streamoff>(1));
            break;
        }

        stringstream ss(line);
        double value;
        while (ss >> value) {
            all_values.push_back(value);
        }
    }

    // Wypełnij macierz
    size_t idx = 0;
    for (int i = 0; i < size && idx < all_values.size(); i++) {
        for (int j = 0; j < size && idx < all_values.size(); j++) {
            distance_matrix[i][j] = all_values[idx++];
        }
    }
}

void TSPInstance::readUpperRow(ifstream& file) {
    string line;
    vector<double> all_values;

    // Wczytaj wszystkie wartości numeryczne
    while (getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;
        if (line == "EOF" || line == "-1" || line.find("SECTION") != string::npos) {
            // POPRAWIONE
            auto pos = file.tellg();
            file.seekg(pos - static_cast<streamoff>(line.length()) - static_cast<streamoff>(1));
            break;
        }

        stringstream ss(line);
        double value;
        while (ss >> value) {
            all_values.push_back(value);
        }
    }

    // Wypełnij górny trójkąt (bez przekątnej)
    size_t idx = 0;
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size && idx < all_values.size(); j++) {
            distance_matrix[i][j] = all_values[idx];
            distance_matrix[j][i] = all_values[idx]; // Symetryczna
            idx++;
        }
    }
}

void TSPInstance::readLowerDiagRow(ifstream& file) {
    string line;
    vector<double> all_values;

    while (getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;
        if (line == "EOF" || line == "-1" || line.find("SECTION") != string::npos) {
            // POPRAWIONE
            auto pos = file.tellg();
            file.seekg(pos - static_cast<streamoff>(line.length()) - static_cast<streamoff>(1));
            break;
        }

        stringstream ss(line);
        double value;
        while (ss >> value) {
            all_values.push_back(value);
        }
    }

    // Wypełnij dolny trójkąt (z przekątną)
    size_t idx = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j <= i && idx < all_values.size(); j++) {
            distance_matrix[i][j] = all_values[idx];
            if (i != j) {
                distance_matrix[j][i] = all_values[idx]; // Symetryczna
            }
            idx++;
        }
    }
}

double TSPInstance::calculateTourLength(const vector<int>& tour) const {
    double length = 0;
    for (size_t i = 0; i < tour.size() - 1; i++) {
        length += distance_matrix[tour[i]][tour[i + 1]];
    }
    // Powrót do miasta startowego
    length += distance_matrix[tour[tour.size() - 1]][tour[0]];
    return length;
}