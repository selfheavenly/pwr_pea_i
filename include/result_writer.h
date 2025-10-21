//
// Created by Kamil Sobierajski on 12/10/2025.
//

#ifndef PEA_TSP_1_RESULT_WRITER_H
#define PEA_TSP_1_RESULT_WRITER_H

#include "result.h"
#include <string>
#include <vector>

class ResultWriter {
public:
    static void writeToCSV(const std::string& filename, const std::vector<Result>& results,
                           double avg_time = 0);
};


#endif //PEA_TSP_1_RESULT_WRITER_H