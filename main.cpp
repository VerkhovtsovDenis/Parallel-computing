#include <iostream>
#include <omp.h>
#include <memory>
#include <cstdio>
#include "prototype.h"
#include "num_threads.h"
#include "run_experements.hpp"

int main(int argc, char** argv){
    
    for (auto &funk : functions){
        auto filename = "output/" + std::string(funk.first) + ".csv"; 
        auto file = fopen(filename.c_str(), "w");
        fprintf(file, "Thread,match,time,speedup,efficiency\n");

        auto result = run_experiment(funk.second, N);
        
        for (auto &item : result){
            fprintf(file, p, item.T, item.match, item.time, item.speedup, item.efficiency);
        }
	fclose(file);
        std::cout << funk.first << " is end \n";

    }
    return 0;

}
