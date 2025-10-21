//
// Created by Kamil Sobierajski on 12/10/2025.
//

#ifndef PEA_TSP_1_SOLVER_H
#define PEA_TSP_1_SOLVER_H

#include "tsp_instance.h"
#include "result.h"
#include <vector>
#include <chrono>
#include <random>
#include <limits>

class TSPSolver {
protected:
    const TSPInstance& instance;
    bool show_progress;

public:
    TSPSolver(const TSPInstance& inst, bool progress = false);
    virtual ~TSPSolver();

    virtual Result solve() = 0;
    virtual std::string getName() const = 0;

    // Oblicz błędy
    void calculateErrors(Result& result, double optimal);
};

class BruteForceSolver : public TSPSolver {
public:
    BruteForceSolver(const TSPInstance& inst, bool progress = false);

    std::string getName() const override;
    Result solve() override;
};

class NearestNeighbourSolver : public TSPSolver {
private:
    int start_city;

public:
    NearestNeighbourSolver(const TSPInstance& inst, bool progress = false, int start = 0);

    std::string getName() const override;
    Result solve() override;
};

class RandomSolver : public TSPSolver {
private:
    int iterations;
    double max_time;

public:
    RandomSolver(const TSPInstance& inst, int iter, double time_limit, bool progress = false);

    std::string getName() const override;
    Result solve() override;
};
#endif //PEA_TSP_1_SOLVER_H