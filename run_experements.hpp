#include <chrono>

#if !defined(__cplusplus) || __cplusplus < 20200000 
    typedef void (*randomize) (unsigned* V, size_t n, unsigned x0, unsigned xmin, unsigned xmax);
#else 
    template <class F> 
    concept randomize = std::is_invocable_r<unsigned, F, unsigned*, size_t, unsigned, unsigned, unsigned>;
#endif

struct table_row {
    unsigned T; double time, speedup;
};

std::vector<table_row> run_experiment(randomize random, const size_t n) {
    using std::chrono::steady_clock, std::chrono::duration_cast, std::chrono::milliseconds;


    unsigned P = get_num_threads();
    std::vector<table_row> results;

    auto buf = std::make_unique<unsigned[]>(n);

    for (unsigned T = 1; T <= P; ++T) {
        set_num_threads(T);

        results.emplace_back();
        auto &rr = results.back();

        auto t1 = steady_clock::now();
        random(buf.get(), n, 125, 0, 100);
        auto t2 = steady_clock::now();
        
        rr.T = T;
        rr.time = duration_cast<milliseconds>(t2 - t1).count();
        rr.speedup = results.front().time / rr.time;

    }

    return results;
};

const char *p = "%u,%f,%f\n";
std::vector<std::pair<const char *, randomize>> functions {
        {"1. cogerent_random", cogerent_random},
    };
