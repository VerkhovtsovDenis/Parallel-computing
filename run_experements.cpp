#include <chrono>

struct table_row {
    bool match; double time, speedup, efficiency;
};

std::vector<table_row> run_experiment(sum_ptr sum, const size_t n) {

    unsigned P = get_num_threads();
    std::vector<table_row> table(P);

    auto V = std::make_unique<unsigned[]>(n);

    for (unsigned T = 1; T <= P; ++T) {
        for (size_t i = 0; i < V.size(); ++i) V[i] = i + T;
        auto t1 = std::chrono::steady_clock::now();
        table[T-1].match = (sum(V.get(), n) == 0xFC000000 + T - 1);
        auto t2 = std::chrono::steady_clock::now();
        table[T - 1].time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        table[T - 1].speedup = table[0].time / table[T - 1].time;
        table[T - 1].efficiency = table[T - 1].speedup / T;
    }

    return table;
}
