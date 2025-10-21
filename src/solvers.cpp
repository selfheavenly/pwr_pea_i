//
// Created by Kamil Sobierajski on 12/10/2025.
//

#include "../include/solvers.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>
#include <limits>
#include <cmath>

using namespace std;

TSPSolver::TSPSolver(const TSPInstance& inst, bool progress)
    : instance(inst), show_progress(progress) {}

TSPSolver::~TSPSolver() {}

void TSPSolver::calculateErrors(Result& result, double optimal) {
    result.absolute_error = abs(result.found_value - optimal);
    if (optimal != 0) {
        result.relative_error = result.absolute_error / optimal;
        result.relative_error_percent = result.relative_error * 100.0;
    } else {
        result.relative_error = 0;
        result.relative_error_percent = 0;
    }
}

// ============================================================================
// Brute Force Solver
// ============================================================================

BruteForceSolver::BruteForceSolver(const TSPInstance& inst, bool progress)
    : TSPSolver(inst, progress) {}

string BruteForceSolver::getName() const { return "Brute Force"; }

Result BruteForceSolver::solve() {
    Result result;
    result.instance_name = instance.name;
    result.optimal_value = instance.optimal_length.value_or(0);
    if (instance.optimal_tour.has_value()) {
        result.optimal_tour = instance.optimal_tour.value();
    }
    result.method_name = getName();

    // Parametry metody
    result.addParameter("algorithm", "brute-force");

    auto start = chrono::high_resolution_clock::now();

    if (instance.size > 10) {
        cerr << "Warning: Brute Force is not recommended for more than 10 cities!" << endl;
    }

    // Utworzenie permutacji miast (poza pierwszym, które jest ustalone)
    vector<int> cities;
    for (int i = 1; i < instance.size; i++) {
        cities.push_back(i);
    }

    double best_length = numeric_limits<double>::max();
    vector<int> best_tour;

    long long count = 0;
    do {
        vector<int> tour = {0}; // Rozpocznij od miasta 0
        tour.insert(tour.end(), cities.begin(), cities.end());

        double length = instance.calculateTourLength(tour);
        if (length < best_length) {
            best_length = length;
            best_tour = tour;
        }

        count++;
        if (show_progress && count % 100000 == 0) {
            cout << "Checked permutations: " << count << endl;
        }
    } while (next_permutation(cities.begin(), cities.end()));

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    result.found_tour = best_tour;
    result.found_value = best_length;
    result.execution_time = elapsed.count();
    result.addParameter("permutations_checked", count);

    calculateErrors(result, instance.optimal_length.value_or(best_length));

    return result;
}

// ============================================================================
// Nearest Neighbour Solver
// ============================================================================

NearestNeighbourSolver::NearestNeighbourSolver(const TSPInstance& inst, bool progress, int start)
    : TSPSolver(inst, progress), start_city(start) {}

string NearestNeighbourSolver::getName() const { return "Nearest Neighbour"; }

Result NearestNeighbourSolver::solve() {
    Result result;
    result.instance_name = instance.name;
    result.optimal_value = instance.optimal_length.value_or(0);
    if (instance.optimal_tour.has_value()) {
        result.optimal_tour = instance.optimal_tour.value();
    }
    result.method_name = getName();

    // Param
    result.addParameter("start_city", start_city);
    result.addParameter("algorithm", "nearest-neighbour");

    auto start = chrono::high_resolution_clock::now();

    vector<bool> visited(instance.size, false);
    vector<int> tour;

    int current = start_city;
    tour.push_back(current);
    visited[current] = true;

    for (int i = 1; i < instance.size; i++) {
        double min_dist = numeric_limits<double>::max();
        int next_city = -1;

        // Znajdź najbliższe nieodwiedzone miasto
        for (int j = 0; j < instance.size; j++) {
            if (!visited[j] && instance.distance_matrix[current][j] < min_dist) {
                min_dist = instance.distance_matrix[current][j];
                next_city = j;
            }
        }

        if (next_city != -1) {
            tour.push_back(next_city);
            visited[next_city] = true;
            current = next_city;
        }

        if (show_progress && i % 100 == 0) {
            cout << "Progress: " << i << "/" << instance.size << " cities" << endl;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    result.found_tour = tour;
    result.found_value = instance.calculateTourLength(tour);
    result.execution_time = elapsed.count();

    calculateErrors(result, instance.optimal_length.value_or(result.found_value));

    return result;
}

// ============================================================================
// Random Solver
// ============================================================================

RandomSolver::RandomSolver(const TSPInstance& inst, int iter, double time_limit, bool progress)
    : TSPSolver(inst, progress), iterations(iter), max_time(time_limit) {}

string RandomSolver::getName() const { return "Random"; }

Result RandomSolver::solve() {
    Result result;
    result.instance_name = instance.name;
    result.optimal_value = instance.optimal_length.value_or(0);
    if (instance.optimal_tour.has_value()) {
        result.optimal_tour = instance.optimal_tour.value();
    }
    result.method_name = getName();

    // Parametry
    result.addParameter("max_iterations", iterations);
    result.addParameter("max_time", max_time);
    result.addParameter("algorithm", "random");

    auto start = chrono::high_resolution_clock::now();

    random_device rd;
    mt19937 gen(rd());

    vector<int> cities;
    for (int i = 0; i < instance.size; i++) {
        cities.push_back(i);
    }

    double best_length = numeric_limits<double>::max();
    vector<int> best_tour;

    int iter = 0;
    bool time_exceeded = false;
    bool optimal_found = false;

    while (iter < iterations && !time_exceeded && !optimal_found) {
        shuffle(cities.begin(), cities.end(), gen);

        double length = instance.calculateTourLength(cities);
        if (length < best_length) {
            best_length = length;
            best_tour = cities;

            // Early stopping - jeśli znaleziono rozwiązanie optymalne (z tolerancją)
            if (instance.optimal_length.has_value()) {
                double tolerance = 0.01;
                if (abs(best_length - instance.optimal_length.value()) < tolerance) {
                    optimal_found = true;
                    if (show_progress) {
                        cout << "Optimal solution found at iteration " << iter << "!" << endl;
                    }
                }
            }
        }

        iter++;
        if (show_progress && (iter % 1000 == 0)) {
            cout << "Iteration: " << iter << "/" << iterations
                 << ", Best result: " << best_length << endl;
        }

        // Sprawdź limit czasu
        auto now = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = now - start;
        if (elapsed.count() > max_time) {
            time_exceeded = true;
            if (show_progress) {
                cout << "Time limit exceeded after " << iter << " iterations." << endl;
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    result.found_tour = best_tour;
    result.found_value = best_length;
    result.execution_time = elapsed.count();
    result.addParameter("actual_iterations", iter);
    result.addParameter("time_exceeded", time_exceeded ? "true" : "false");
    result.addParameter("optimal_found", optimal_found ? "true" : "false");

    calculateErrors(result, instance.optimal_length.value_or(best_length));

    return result;
}