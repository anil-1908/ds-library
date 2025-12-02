#include "../src/dynamic_array.hpp"
#include<chrono>
#include<iostream>

int main(){
    DynamicArray<int> a;
    const int N = 1000000; //1e6
    auto t0 = std::chrono::high_resolution_clock::now();
    for(size_t i = 0; i < N; ++i) a.push_back(i);
    auto t1 = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    std::cout<<"push_back " << N << " ints took " << ms << "ms\n";

    volatile long sum = 0;
    auto t2 = std::chrono::high_resolution_clock::now();
    for(size_t i = 0; i < N; ++i) sum += a[i];
    auto t3 = std::chrono::high_resolution_clock::now();
    auto ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();
    std::cout<<"read " << N << " ints took " << ms2 << "ms\n";

    (void)sum;
    return 0;

}