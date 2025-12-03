#include "../src/dynamic_array.hpp"
#include <cassert>
#include <iostream>
#include <numeric>

struct Counter {
    static int constructions;
    static int destructions;
    static int copies;
    static int moves;
    int val;
    Counter(int v = 0) : val(v) {++constructions;}
    Counter(const Counter& o): val(o.val) {++constructions; ++copies;}
    Counter(Counter&& o) noexcept : val(o.val) {++constructions; ++moves; ++constructions; o.val = -1;}
    ~Counter() {++destructions;}
    };

    int Counter::constructions = 0;
    int Counter::destructions = 0;
    int Counter::copies = 0;
    int Counter::moves = 0;

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

    //non-trivial tests
    {
        Counter::constructions = Counter::destructions = Counter::copies = Counter::moves = 0;
        DynamicArray<Counter> arr;
        arr.reserve(10);
        arr.emplace_back(1);
        arr.emplace_back(2);
        arr.push_back(Counter(3));
        assert(arr.size() == 3);
        //triggers moves by resizing to grow and shrink
        arr.resize(34);
        arr.resize(2);
        assert(arr[0].val == 1);
        assert(arr[1].val == 2);
        //sanity
        assert(Counter::constructions > 0);
        assert(Counter::destructions > 0);
    }

    //iterator tests
    {
        DynamicArray<int> arr;
        for(int i=0; i<10; ++i) arr.push_back(i);

        int sum = 0;
        for(auto it: arr) sum += it;
        assert(sum == 45);

        //STL Compatability test
        assert(std::accumulate(arr.begin(), arr.end(), 0) == 45);

        //Mutating through range
        for(auto& it : arr) it*=2;  
        for(int i=0; i < 10; i++) assert(arr[i] == i*2);

    }

    //insert tests
    {
        DynamicArray<int> a;
        for (int i = 0; i < 5; i++) a.push_back(i);   // [0,1,2,3,4]

        // middle
        a.insert(2, 99);     // [0,1,99,2,3,4]
        assert(a.size() == 6);
        assert(a[2] == 99);
        assert(a[3] == 2);

        // front
        a.insert(0, -1);     // [-1,0,1,99,2,3,4]
        assert(a[0] == -1);
        assert(a.size() == 7);

        // back
        a.insert(a.size(), 500); 
        assert(a[a.size()-1] == 500);

        // multiple inserts
        a.insert(3, 111);
        a.insert(3, 222);
        assert(a[3] == 222);
        assert(a[4] == 111);
    }
    std::cout << "Basic tests passed.\n";
}
