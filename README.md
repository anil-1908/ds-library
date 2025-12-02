# **ds-library**
Library of datastructures

A lightweight collection of hand-implemented data structures built over basic memory primitives.  
The goal of this project is to understand how fundamental containers work internally by building them from scratch, testing them thoroughly, and benchmarking their performance.

**Status:** v0.1 stable — passes tests and benchmarks; new features under development.

This library will grow over time as more data structures are added.

---

## **v0.1 - DynamicArray**
`DynamicArray<T>` is a minimal vector-like container built from scratch to
understand memory allocation, growth policies, move semantics, and exception
safety in contiguous storage containers.

### Features
- Amortized **O(1)** `push_back`
- **O(1)** random access via `operator[]`
- Automatic growth using capacity-doubling
- Shrink policy: capacity halves when `size <= capacity/4`
- Header-only, template-based design
- Strong exception safety during reallocation
- Move-aware (`std::move_if_noexcept`)
- Invariants: `capacity == 0 <=> data == nullptr`

---

### **Build & run tests:**
```bash
g++ -std=c++17 -O0 -g tests/test_dynamic_array.cpp -I src -o tests/test_dynamic_array
./tests/test_dynamic_array  # prints: Basic tests passed.
```

### **Benchmark (optimized)**:
```bash
g++ -std=c++17 -O2 bench/bench_dynamic_array.cpp -I src -o bench/bench_dynamic_array
./bench/bench_dynamic_array  # sample output: push_back 1000000 ints took 1ms / read 1000000 ints took 2ms
```

### **Notes:**
- Public API: push_back, pop_back, operator[], size(), capacity(), reserve(), resize(), clear().
- Implementation detail: strong exception-safety on shrink; template definitions in header.
- Template-based container: all method definitions are header-only (no .cpp needed for templates).
- Invariants: `capacity == 0 <=> data == nullptr`.
- Memory model: all allocations performed via `std::allocator<T>`.

---

## Benchmark results 

1. push_back 1000000 ints took 1ms
2. read 1000000 ints took 2ms

