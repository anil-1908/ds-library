#include "../src/dynamic_array.hpp"
#include <cassert>
#include <iostream>

int main() {
    DynamicArray<int> a;
    a.push_back(10);
    a.push_back(20);
    a.push_back(30);

    assert(a.size() == 3);
    assert(a[0] == 10);
    assert(a[1] == 20);
    assert(a[2] == 30);

    a.pop_back();
    assert(a.size() == 2);
    assert(a[1] == 20);

    DynamicArray<int> A;
    for(size_t i = 0; i < 100; ++i) A.push_back(i);
    for(size_t i =0; i < 95; ++i)   A.pop_back();

    assert(A.size() == 5);
    for(size_t i = 0; i < 5; ++i) assert(A[i] == i);

    DynamicArray<int> b;
    for(size_t i = 0; i < 10; ++i) b.push_back(i);
    for(size_t i = 0; i < 10; ++i) b.pop_back();

    b.push_back(17);
    assert(b.size() == 1 && b[0] == 17);

    //Tests for Resize function
    //Shrinking correctness
    DynamicArray<int> x;
    for(int i = 0; i < 1000; i++) x.push_back(i);

    x.resize(200);
    for(int i = 0; i < 200; ++i) assert(x[i] == i);  

    //Grow Correctness

    x.resize(1200);
    for(int i = 0; i < 200; ++i) assert(x[i] == i);
    for (int i = 200; i <1200 ; ++i) assert(x[i] == 0);

    //Grow shrink cycles
    x.resize(500);
    x.resize(50);
    x.resize(300);

    assert(x.size() == 300);
    for(size_t i = 0; i < 50; ++i) assert(x[i] == i);

    // Tests for Copy ctor
    DynamicArray<int> y(x);
    assert(x.size() == y.size());
    for(size_t i = 0; i < x.size(); ++i) assert(x[i] == y[i]);
    
    //Tests for Move ctor
    DynamicArray<int> z(std::move(y));
    assert(y.size() == 0);
    assert(z.size() == x.size());
    for(size_t i = 0; i < x.size(); ++i) assert(x[i] == z[i]);

    //Tests for assign copy 
    DynamicArray<int> w;
    w = z;
    assert(w.size() == z.size());
    for(size_t i = 0; i < z.size(); ++i) assert(z[i] == w[i]);

    //Tests for assign move 
    DynamicArray<int> v;
    v = std::move(w);
    assert(w.size() == 0);
    assert(v.size() == z.size());
    for(size_t i = 0; i < z.size(); ++i) assert(z[i] == v[i]);

    std::cout << "Basic tests passed.\n";
}
