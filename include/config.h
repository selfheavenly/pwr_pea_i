//
// Created by Kamil Sobierajski on 12/10/2025.
//

#ifndef PEA_TSP_1_CONFIG_H
#define PEA_TSP_1_CONFIG_H

#include <string>
#include <optional>

class Config {
public:
    std::string data_file;
    std::optional<double> optimal_value;  // Opcjonalne, może być w pliku TSPLIB
    int iterations;
    int repetitions;
    double max_time;
    int max_iterations;
    bool show_progress;
    std::string output_file;
    std::string method; // brute-force, nearest-neighbour, random

    Config();
};

class ConfigReader {
public:
    static Config readConfig(const std::string& filename);
};

#endif //PEA_TSP_1_CONFIG_H