#include "../src/linked_list.hpp"
#include <cassert>
#include <iostream>

struct Counter {
    static int constructions;
    static int destructions;
    static int copies;
    static int moves;
    int val;
    Counter(int v = 0) : val(v) {++constructions;}
    Counter(const Counter& o): val(o.val) {++constructions; ++copies;}
    Counter(Counter&& o) noexcept : val(o.val) {++moves; ++constructions; o.val = -1;}
    ~Counter() {++destructions;}
};

    int Counter::constructions = 0;
    int Counter::destructions = 0;
    int Counter::copies = 0;
    int Counter::moves = 0;
int main(){
    // simple sanity
{
    LinkedList<int> L;
    assert(L.empty());
    L.push_back(1);
    L.push_back(2);
    L.push_back(3);
    assert(L.size() == 3);

    int expected = 1;
    for (auto node = L.head(); node; node = node->next) {
        assert(node->data == expected++);
    }

    L.clear();
    assert(L.size() == 0);
    assert(L.head() == nullptr);
}

//push_front / order check
{
    LinkedList<int> L;
    L.push_front(3); // 3
    L.push_front(2); // 2,3
    L.push_front(1); // 1,2,3
    assert(L.size() == 3);
    int seq = 1;
    for (auto n = L.head(); n; n = n->next) {
        assert(n->data == seq++);
    }
    L.clear();
}

// Non-trivial element lifetime test (Counter must be at file scope in tests)
// Reuse Counter from existing test suite; if not present, define a similar struct at file scope.
{
    Counter::constructions = Counter::destructions = Counter::copies = Counter::moves = 0;
    {
        LinkedList<Counter> L;
        L.push_back(Counter(10));
        L.push_back(Counter(20));
        L.push_front(Counter(5));
        assert(L.size() == 3);
    }
    // after L is out of scope, destructors should have run
    assert(Counter::destructions >= Counter::constructions);
}
std::cout << "LinkedList tests passed.\n";

}