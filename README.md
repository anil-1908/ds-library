# ds-library
Library of datastructures

A lightweight collection of hand-implemented data structures built over basic memory primitives.  
The goal of this project is to understand how fundamental containers work internally by building them from scratch, testing them thoroughly, and benchmarking their performance.

This library will grow over time as more data structures are added.

---

## 1. Dynamic Array

The first data structure implemented in this library is a simple `DynamicArray<T>`.

### Features

- Amortized **O(1)** `push_back`
- **O(1)** random access using `operator[]`
- **O(1)** `pop_back`
- Automatic resizing using capacity‐doubling strategy
- Minimal, header-only implementation
- Tests + micro-benchmarks included

---

## **Build / Test / Benchmark Instructions**
*(Exact commands used in development.)*

### **Compile tests (debug mode)**
```bash
g++ -std=c++17 -O0 -g tests/test_dynamic_array.cpp  -o tests/test_dynamic_array
```
### **run tests (asserts enabled)**
./tests/test_dynamic_array

### **compile benchmark (optimized)**
```bash
g++ -std=c++17 -O2 bench/bench_dynamic_array.cpp  -o bench/bench_dynamic_array
```
### **run benchmark**
```bash
./bench/bench_dynamic_array
```

---

## Benchmark results 

1. push_back 1000000 ints took 1ms
2. read 1000000 ints took 2ms

