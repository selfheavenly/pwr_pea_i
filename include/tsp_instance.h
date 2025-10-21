//
// Created by Kamil Sobierajski on 12/10/2025.
//

#ifndef PEA_TSP_1_TSP_INSTANCE_H
#define PEA_TSP_1_TSP_INSTANCE_H

#include <string>
#include <vector>
#include <optional>

enum class EdgeWeightFormat {
    FULL_MATRIX,
    UPPER_ROW,
    LOWER_ROW,
    UPPER_DIAG_ROW,
    LOWER_DIAG_ROW,
    UPPER_COL,
    LOWER_COL
};

enum class EdgeWeightType {
    EXPLICIT,
    EUC_2D,
    GEO,
    ATT
};

class TSPInstance {
public:
    std::string name;
    std::string type;  // TSP lub ATSP
    std::string comment;
    int size;
    EdgeWeightType weight_type;
    EdgeWeightFormat weight_format;
    std::vector<std::vector<double>> distance_matrix;
    std::optional<std::vector<int>> optimal_tour;
    std::optional<double> optimal_length;

    TSPInstance();

    // Wczytanie instancji z pliku w formacie TSPLIB
    bool loadFromFile(const std::string& filename);

    // Obliczenie długości trasy
    double calculateTourLength(const std::vector<int>& tour) const;

private:
    void parseHeader(const std::string& line);
    void readDistanceMatrix(std::ifstream& file);
    void readFullMatrix(std::ifstream& file);
    void readUpperRow(std::ifstream& file);
    void readLowerDiagRow(std::ifstream& file);

    static std::string trim(const std::string& str);
};

#endif //PEA_TSP_1_TSP_INSTANCE_H