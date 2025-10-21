//
// Created by Kamil Sobierajski on 12/10/2025.
//

#include "include/config.h"
#include "include/tsp_instance.h"
#include "include/solvers.h"
#include "include/result_writer.h"
#include "include/test_suite.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    cout << "==================================================" << endl;
    cout << "   Travelling Salesman Problem Solver v2.0" << endl;
    cout << "   TSPLIB Format Support | C++23" << endl;
    cout << "==================================================" << endl;

    // Sprawdź tryb testowy
    if (argc > 1 && string(argv[1]) == "--test") {
        cout << "\nRunning test suite..." << endl;
        TestSuite suite(true);
        if (!suite.loadTestInstances("test_instances.txt")) {
            cerr << "Error loading test instances!" << endl;
            return 1;
        }
        auto results = suite.runAllTests();
        ResultWriter::writeToCSV("test_results.csv", results);
        cout << "Test suite completed. Results saved to test_results.csv" << endl;
        return 0;
    }

    // Określ nazwę pliku konfiguracyjnego
    string config_file = "config.txt";
    if (argc > 1) {
        config_file = argv[1];
    }

    // Wczytaj konfigurację
    cout << "\nLoading configuration from: " << config_file << endl;
    Config config = ConfigReader::readConfig(config_file);

    // Wczytaj instancję TSP
    cout << "Loading data from: " << config.data_file << endl;
    TSPInstance instance;
    if (!instance.loadFromFile(config.data_file)) {
        cerr << "Error loading data!" << endl;
        return 1;
    }

    if (config.optimal_value.has_value() && !instance.optimal_length.has_value()) {
        instance.optimal_length = config.optimal_value.value();
        cout << "Using optimal value from config: " << config.optimal_value.value() << endl;
    }

    if (instance.optimal_tour.has_value() && instance.size < 16) {
        cout << "Optimal tour: ";
        const auto& tour = instance.optimal_tour.value();
        for (size_t i = 0; i < tour.size(); i++) {
            cout << tour[i];
            if (i < tour.size() - 1) cout << "-";
        }
        cout << endl;
    }

    cout << "Instance: " << instance.name << endl;
    cout << "Type: " << instance.type << endl;
    if (!instance.comment.empty()) {
        cout << "Comment: " << instance.comment << endl;
    }
    cout << "Size: " << instance.size << " cities" << endl;
    if (instance.optimal_length.has_value()) {
        cout << "Optimal length: " << instance.optimal_length.value() << endl;
    } else {
        cout << "Optimal length: Unknown" << endl;
    }
    cout << "Method: " << config.method << endl;
    cout << "Repetitions: " << config.repetitions << endl;

    // Wybierz solver
    TSPSolver* solver = nullptr;

    if (config.method == "brute-force") {
        solver = new BruteForceSolver(instance, config.show_progress);
    } else if (config.method == "nearest-neighbour") {
        solver = new NearestNeighbourSolver(instance, config.show_progress);
    } else if (config.method == "random") {
        solver = new RandomSolver(instance, config.iterations, config.max_time, config.show_progress);
    } else {
        cerr << "Unknown method: " << config.method << endl;
        return 1;
    }

    // Rozwiąż problem
    cout << "\n--------------------------------------------------" << endl;
    cout << "Starting solving..." << endl;
    cout << "--------------------------------------------------" << endl;

    vector<Result> results;
    double total_time = 0;

    for (int rep = 0; rep < config.repetitions; rep++) {
        cout << "\nRepetition " << (rep + 1) << "/" << config.repetitions << endl;

        Result result = solver->solve();
        results.push_back(result);
        total_time += result.execution_time;

        cout << "Found value: " << result.found_value << endl;
        if (instance.size < 16) {
            cout << "Found tour: ";
            for (size_t i = 0; i < result.found_tour.size(); i++) {
                cout << result.found_tour[i];
                if (i < result.found_tour.size() - 1) cout << "-";
            }
            cout << endl;
        }
        cout << "Execution time: " << fixed << setprecision(3) << (result.execution_time * 1000.0) << " ms" << endl;
        if (instance.optimal_length.has_value()) {
            cout << "Relative error: " << result.relative_error_percent << "%" << endl;
        }
    }

    double avg_time = total_time / config.repetitions;

    cout << "\n==================================================" << endl;
    cout << "Summary:" << endl;
    cout << "Average execution time: " << fixed << setprecision(3) << (avg_time * 1000.0) << " ms" << endl;
    cout << "==================================================" << endl;

    // Zapisz wyniki
    ResultWriter::writeToCSV(config.output_file, results, avg_time);

    delete solver;

    cout << "\nProgram finished." << endl;
    return 0;
}