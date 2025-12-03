#include "../src/dynamic_array.hpp"
#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>

// Non-trivial type that counts operations
struct Heavy {
    static long constructions, destructions, copies, moves;
    int payload;
    Heavy(int v=0): payload(v) { ++constructions; }
    Heavy(const Heavy& o): payload(o.payload) { ++constructions; ++copies; }
    Heavy(Heavy&& o) noexcept: payload(o.payload) { ++constructions; ++moves; o.payload = -1; }
    ~Heavy() { ++destructions; }
};
long Heavy::constructions = 0;
long Heavy::destructions = 0;
long Heavy::copies = 0;
long Heavy::moves = 0;

template <typename F>
long run_once(F fn, int warm = 1) {
    for (int i = 0; i < warm; ++i) fn();
    auto t0 = std::chrono::steady_clock::now();
    fn();
    auto t1 = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
}

int main() {
    // Increase N if times are tiny on your machine
    const long N = 5000000;        // 5e6
    const int TRIES = 7;           // take median

    std::cout << "Benchmark: N=" << N << " TRIES=" << TRIES << "\n\n";

    // Helper: run a bunch and return median microseconds
    auto median_time = [&](auto fn){
        std::vector<long> times;
        for (int t=0; t<TRIES; ++t) {
            times.push_back(run_once(fn, 0));
        }
        std::sort(times.begin(), times.end());
        return times[TRIES/2];
    };

    // 1) int emplace (with reserve)
    {
        auto us = median_time([&](){
            DynamicArray<int> a;
            a.reserve(N);
            for (long i=0;i<N;++i) a.emplace_back((int)i);
        });
        std::cout << "int emplace_back (with reserve) median us: " << us << "\n";
    }

    // 2) int push (with reserve)
    {
        auto us = median_time([&](){
            DynamicArray<int> a;
            a.reserve(N);
            for (long i=0;i<N;++i) a.push_back((int)i);
        });
        std::cout << "int push_back (with reserve) median us:   " << us << "\n\n";
    }

    // 3) int emplace (no reserve)
    {
        auto us = median_time([&](){
            DynamicArray<int> a;
            for (long i=0;i<N;++i) a.emplace_back((int)i);
        });
        std::cout << "int emplace_back (no reserve) median us:  " << us << "\n";
    }

    // 4) int push (no reserve)
    {
        auto us = median_time([&](){
            DynamicArray<int> a;
            for (long i=0;i<N;++i) a.push_back((int)i);
        });
        std::cout << "int push_back (no reserve) median us:    " << us << "\n\n";
    }

    // 5) Heavy emplace (with reserve) — container inside lambda
    {
        std::vector<long> times;
        for (int t=0; t<TRIES; ++t) {
            Heavy::constructions = Heavy::destructions = Heavy::copies = Heavy::moves = 0;
            auto us = run_once([&](){
                DynamicArray<Heavy> a;
                a.reserve(N);
                for (long i=0;i<N;++i) a.emplace_back((int)i);
                // 'a' destroyed here, before run_once returns
            }, 0);
            times.push_back(us);
            std::cout << "  emplace: constructions=" << Heavy::constructions
                      << " copies=" << Heavy::copies << " moves=" << Heavy::moves
                      << " destructions=" << Heavy::destructions << "\n";
        }
        std::sort(times.begin(), times.end());
        std::cout << "Heavy emplace_back (with reserve) median us: " << times[TRIES/2] << "\n";
    }

    // 6) Heavy push (with reserve)
    {
        std::vector<long> times;
        for (int t=0; t<TRIES; ++t) {
            Heavy::constructions = Heavy::destructions = Heavy::copies = Heavy::moves = 0;
            auto us = run_once([&](){
                DynamicArray<Heavy> a;
                a.reserve(N);
                for (long i=0;i<N;++i) a.push_back(Heavy((int)i));
                // 'a' destroyed here
            }, 0);
            times.push_back(us);
            std::cout << "  push: constructions=" << Heavy::constructions
                      << " copies=" << Heavy::copies << " moves=" << Heavy::moves
                      << " destructions=" << Heavy::destructions << "\n";
        }
        std::sort(times.begin(), times.end());
        std::cout << "Heavy push_back (with reserve) median us:    " << times[TRIES/2] << "\n";
    }

    return 0;
}
