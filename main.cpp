#include <iostream>
#include <omp.h>
#include <memory>

#include "prototype.h"
#include "num_threads.h"
#include "run_experements.hpp"

int main(int argc, char** argv){
    // set_num_threads(4);

    using std::cin, std::cout, std::endl;
    
    // cout << "N = " << N << endl;

    // auto V = std::make_unique<unsigned[]>(N);
    // for(auto i = 0; i < N; ++i)
    //     V[i] = i;
    
    for (auto &funk : functions){
        auto filename = ("output//" + std::string(funk.first) + std::string(".csv")).c_str(); 
        auto file = fopen(filename, "w");
        fprintf(file, "Thread,match,time,speedup,efficiency\n");

        auto result = run_experiment(funk.second, N);
        
        for (auto &item : result){
            fprintf(file, p, item.T, item.match, item.time, item.speedup, item.efficiency);
        }
        cout << funk.first << " is end \n";

    }
    return 0;

}