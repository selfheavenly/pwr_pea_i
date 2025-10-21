//
// Created by Kamil Sobierajski on 12/10/2025.
//

#ifndef PEA_TSP_1_RESULT_H
#define PEA_TSP_1_RESULT_H

#include <string>
#include <vector>
#include <map>
#include <type_traits>

struct Result {
    std::string instance_name;
    double optimal_value;
    std::vector<int> optimal_tour;
    double found_value;
    std::vector<int> found_tour;
    double execution_time;
    double absolute_error;
    double relative_error;
    double relative_error_percent;
    std::string method_name;

    // Parametry specyficzne dla metody (elastyczne przechowywanie)
    std::map<std::string, std::string> method_parameters;

    // Pomocnicza metoda do dodawania parametru - wersja dla stringów
    void addParameter(const std::string& key, const std::string& value) {
        method_parameters[key] = value;
    }

    // Wersja dla const char*
    void addParameter(const std::string& key, const char* value) {
        method_parameters[key] = std::string(value);
    }

    // Template dla typów num
    template<typename T>
    typename std::enable_if<std::is_arithmetic<T>::value, void>::type
    addParameter(const std::string& key, const T& value) {
        method_parameters[key] = std::to_string(value);
    }
};

#endif //PEA_TSP_1_RESULT_H