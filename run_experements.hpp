#include <chrono>

#if !defined(__cplusplus) || __cplusplus < 20200000 
    typedef unsigned (*sum_ptr) (const unsigned* V, size_t n);
#else 
    template <class F> 
    concept sum_callable = std::is_invocable_r<unsigned, F, const unsigned*, size_t>;
#endif

struct table_row {
    unsigned T; bool match; double time, speedup, efficiency;
};

std::vector<table_row> run_experiment(sum_ptr sum, const size_t n) {

    unsigned P = get_num_threads();
    std::vector<table_row> table(P);
    auto V = std::make_unique<unsigned[]>(n);

    for (unsigned T = 1; T <= P; ++T) {
        set_num_threads(T);
        for (size_t i = 0; i < n; ++i) V[i] = i + T;

        table[T-1].T = T;
        auto t1 = std::chrono::steady_clock::now();
        table[T-1].match = (sum(V.get(), n) == 0xFC000000 + T - 1);
        auto t2 = std::chrono::steady_clock::now();
        table[T - 1].time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        table[T - 1].speedup = table[0].time / table[T - 1].time;
        table[T - 1].efficiency = table[T - 1].speedup / T;
    }

    return table;
};

const char *p = "%u,%d,%f,%f,%f\n";
std::vector<std::pair<const char *, sum_ptr>> functions {
        {"1. simple_sum", simple_sum},
        {"2. simple_sum_with_aligned", simple_sum_with_aligned},
        {"3. sum_with_omp_reduce", sum_with_omp_reduce},
        {"4. sum_with_omp_round_robin", sum_with_omp_round_robin},
        {"5. sum_with_aligned_omp_round_robin", sum_with_aligned_omp_round_robin},
        {"6. sum_with_omp_and_thread", sum_with_omp_and_thread},
        {"7. sum_with_cpp_methons", sum_with_cpp_methons},
        {"8. sum_with_omp_mutex", sum_with_omp_mutex},
        {"9. sum_with_cpp_barrier", sum_with_cpp_barrier}
    };
