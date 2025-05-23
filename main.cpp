#include <print>
#include <vector>
#include <array>
#include <chrono>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>
#include <list>

#define STREAM_ENABLED 1

#if STREAM_ENABLED
#define INIT(name) std::filesystem::remove(name); std::ofstream _MyStream(name)
#define OUT(x) _MyStream << x << '\n'
#define CLEANUP do { } while (false)
#define GET_STEAM _MyStream
#else
#define INIT(name) do { } while (false)
#define OUT(x) std::cout << x << '\n'
#define CLEANUP do { } while (false)
#define GET_STEAM std::cout
#endif

class timer {
    typedef std::chrono::steady_clock type;

    std::ostream& _MyStream;
    type::time_point start_;

public:

    timer(std::ostream& Str) : _MyStream(Str) { start_ = type::now(); }
    ~timer() { OUT(std::chrono::duration_cast<std::chrono::microseconds>(get() - start_)); }

private:

    type::time_point
    get() {
        return type::now();
    }
};

void
cache_misses_benchmark();

void
iteration_contiguous_memory();

void
iteration_non_contiguous_memory();

int32_t
main(int32_t argc, char* argv[]) {
    cache_misses_benchmark();
    iteration_contiguous_memory();
    iteration_non_contiguous_memory();

    return 0;
}

void
cache_misses_benchmark() {
    INIT("out_misses.txt");

    constexpr size_t unit = 1024u;
    constexpr size_t maxCache = static_cast<size_t>(8u * 1024u) * 1024u;
    constexpr size_t testCache = maxCache + unit * 10u;
    constexpr size_t accesses = 1'000'000u;

    std::mt19937 rng(42);
    std::uniform_int_distribution<size_t> dist;

    for (size_t i{ unit }; i < testCache; i += unit) {
        std::vector<int32_t> buffer(i, 1);
        dist = std::uniform_int_distribution<size_t>(0u, i - 1u);

        std::vector<size_t> indexes(accesses);

        for (size_t& idx : indexes) {
            idx = dist(rng);
        }

        volatile size_t sum = 0u;

        {

        timer t(GET_STEAM);

        for (size_t j = 0; j < accesses; ++j) {
            sum += buffer[indexes[j]];
        }

        OUT((i * sizeof(int32_t)) / 1024.0 / 1024.0);
        OUT((i * sizeof(int32_t)) / 1024.0);
        OUT(i * sizeof(int32_t));

        }
    }

    CLEANUP;
}

void
iteration_contiguous_memory() {
    INIT("out_cont_mem.txt");

    constexpr size_t start = 10u;
    constexpr size_t unit = 10u;
    constexpr size_t maxSteps = 50000u;

    std::mt19937 rng(42);
    std::uniform_int_distribution<size_t> dist;

    for (size_t i{ start }; i < maxSteps; i += unit) {
        std::vector<int32_t> vec;
        vec.reserve(i);

        for (size_t j{}; j < i; ++j) {
            vec.push_back(dist(rng));
        }

        volatile size_t sum = 0u;

        {

        timer t(GET_STEAM);

        for (size_t j{}; j < i; ++j) {
            sum += vec[j];
        }

        OUT(i);

        }
    }

    CLEANUP;
}

void
iteration_non_contiguous_memory() {
    INIT("out_non_cont_mem.txt");

    constexpr size_t start = 10u;
    constexpr size_t unit = 10u;
    constexpr size_t maxSteps = 50000u;

    std::mt19937 rng(42);
    std::uniform_int_distribution<size_t> dist;

    for (size_t i{ start }; i < maxSteps; i += unit) {
        std::list<int32_t> list;

        for (size_t j{}; j < i; ++j) {
            list.push_back(dist(rng));
        }

        volatile size_t sum = 0u;

        {

        timer t(GET_STEAM);

        for (int32_t i : list) {
            sum += i;
        }

        OUT(i);

        }
    }

    CLEANUP;
}
