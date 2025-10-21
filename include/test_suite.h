//
// Created by Kamil Sobierajski on 12/10/2025.
//

#ifndef PEA_TSP_1_TEST_SUITE_H
#define PEA_TSP_1_TEST_SUITE_H

#include "../include/config.h"
#include "../include/tsp_instance.h"
#include "solvers.h"
#include "result_writer.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class TestSuite {
private:
    std::vector<std::string> instance_files;
    bool show_progress;

public:
    TestSuite(bool progress = false) : show_progress(progress) {}

    // Wczytanie instancji testowych z pliku
    bool loadTestInstances(const std::string& test_list_file) {
        std::ifstream file(test_list_file);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open test list file: " << test_list_file << std::endl;
            return false;
        }
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty() && line[0] != '#') {
                instance_files.push_back(line);
            }
        }
        file.close();
        return true;
    }

    // Uruchom wszystkie solwery na wszystkich instancjach
    std::vector<Result> runAllTests() {
        std::vector<Result> all_results;
        double total_time_all = 0;

        for (const auto& data_file : instance_files) {
            std::cout << "\nTesting instance: " << data_file << std::endl;
            TSPInstance instance;
            if (!instance.loadFromFile(data_file)) {
                std::cerr << "Skipping invalid instance: " << data_file << std::endl;
                continue;
            }

            // Lista solwerów do testowania
            std::vector<TSPSolver*> solvers;
            solvers.push_back(new BruteForceSolver(instance, show_progress));
            solvers.push_back(new NearestNeighbourSolver(instance, show_progress));
            solvers.push_back(new RandomSolver(instance, 10000, 60.0, show_progress));  // Stałe parametry dla spójności

            double total_time_per_instance = 0;
            std::vector<Result> instance_results;

            for (auto solver : solvers) {
                // 1 powtórzenie dla Brute/NN, 5 dla Random (deterministyczne vs stochastyczne)
                int reps = (solver->getName() == "Random") ? 5 : 1;

                for (int rep = 0; rep < reps; ++rep) {
                    Result result = solver->solve();
                    instance_results.push_back(result);
                    total_time_per_instance += result.execution_time;

                    // Weryfikacja: Dla Brute Force sprawdź czy zgadza się z optimum
                    if (solver->getName() == "Brute Force" && instance.optimal_length.has_value()) {
                        double tolerance = 0.01;
                        if (std::abs(result.found_value - instance.optimal_length.value()) > tolerance) {
                            std::cerr << "WARNING: Brute Force mismatch for " << instance.name
                                      << "! Found: " << result.found_value << ", Expected: "
                                      << instance.optimal_length.value() << std::endl;
                        } else {
                            std::cout << "Brute Force verified OK for " << instance.name << std::endl;
                        }
                    }

                    // Raportuj błąd dla pozostałych
                    std::cout << solver->getName() << " error: " << result.relative_error_percent << "%" << std::endl;
                }
                delete solver;
            }

            // Dodaj średni czas
            double avg_time = total_time_per_instance / instance_results.size();
            for (auto& res : instance_results) {
                all_results.push_back(res);
            }
            total_time_all += total_time_per_instance;
        }

        double overall_avg = total_time_all / (all_results.empty() ? 1 : all_results.size());
        std::cout << "Overall average time: " << overall_avg << "s" << std::endl;
        return all_results;
    }
};

#endif //PEA_TSP_1_TEST_SUITE_H