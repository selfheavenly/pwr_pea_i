//
// Created by Kamil Sobierajski on 12/10/2025.
//

#include "../include/result_writer.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <set>

using namespace std;

void ResultWriter::writeToCSV(const string& filename, const vector<Result>& results,
                              double avg_time) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot create results file: " << filename << endl;
        return;
    }

    // Zbierz wszystkie unikalne klucze parametrów
    set<string> all_param_keys;
    for (const auto& result : results) {
        for (const auto& [key, value] : result.method_parameters) {
            all_param_keys.insert(key);
        }
    }

    // Nagłówek
    file << "Instance,Method,Optimal_value,Optimal_tour,Found_value,"
         << "Found_tour,Execution_time[ms],Absolute_error,Relative_error,"
         << "Relative_error[%]";

    // Dodaj kolumny dla parametrów
    for (const auto& key : all_param_keys) {
        file << "," << key;
    }

    if (avg_time > 0) {
        file << ",Average_time[ms]";
    }
    file << "\n";

    // Dane
    for (const auto& result : results) {
        file << result.instance_name << ","
             << result.method_name << ","
             << fixed << setprecision(2) << result.optimal_value << ",";

        // Optymalna trasa (jeśli istnieje)
        file << "\"";
        if (!result.optimal_tour.empty()) {
            for (size_t i = 0; i < result.optimal_tour.size(); i++) {
                file << result.optimal_tour[i];
                if (i < result.optimal_tour.size() - 1) file << "-";
            }
        } else {
            file << "N/A";
        }
        file << "\",";

        file << fixed << setprecision(2) << result.found_value << ",";

        // Znaleziona trasa
        file << "\"";
        for (size_t i = 0; i < result.found_tour.size(); i++) {
            file << result.found_tour[i];
            if (i < result.found_tour.size() - 1) file << "-";
        }
        file << "\",";

        file << fixed << setprecision(6) << result.execution_time << ","
             << fixed << setprecision(2) << result.absolute_error << ","
             << fixed << setprecision(6) << result.relative_error << ","
             << fixed << setprecision(2) << result.relative_error_percent;

        // Dodaj wartości parametrów
        for (const auto& key : all_param_keys) {
            file << ",";
            auto it = result.method_parameters.find(key);
            if (it != result.method_parameters.end()) {
                file << it->second;
            } else {
                file << "N/A";
            }
        }

        if (avg_time > 0) {
            file << "," << fixed << setprecision(6) << avg_time;
        }
        file << "\n";
    }

    file.close();
    cout << "Results saved to file: " << filename << endl;
}