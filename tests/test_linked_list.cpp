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

// LinkedList pop tests
{
    LinkedList<int> L;
    L.push_back(1);
    L.push_back(2); // list: [1,2]
    assert(L.size() == 2);

    // pop_front
    L.pop_front();   // now [2]
    assert(L.size() == 1);
    assert(L.head() != nullptr && L.head()->data == 2);
    assert(L.head() == L.head()); // trivial check to ensure valid pointer

    // pop_back (now removes the last element)
    L.pop_back();    // now []
    assert(L.size() == 0);
    assert(L.head() == nullptr);

    // popping from empty should assert: (we check behavior manually/commented)
    // Uncommenting the following lines should trigger assertion (intended)
    // L.pop_front();
    // L.pop_back();
}

// chain of operations
{
    LinkedList<int> L;
    L.push_back(10);
    L.push_back(20);
    L.push_front(5); // [5,10,20]
    assert(L.size() == 3);

    L.pop_back();    // [5,10]
    assert(L.size() == 2);
    int expected_arr[] = {5, 10};
    int idx = 0;
    for (auto n = L.head(); n; n = n->next) {
        assert(n->data == expected_arr[idx++]);
    }
    assert(idx == 2);

    L.pop_front();   // [10]
    assert(L.size() == 1);
    assert(L.head()->data == 10);

    L.pop_back();    // []
    assert(L.size() == 0);
    assert(L.head() == nullptr);
}

// Non-trivial lifetime check with Counter
{
    Counter::constructions = Counter::destructions = Counter::copies = Counter::moves = 0;
    {
        LinkedList<Counter> L;
        L.push_back(Counter(1));
        L.push_back(Counter(2));
        L.push_back(Counter(3));
        assert(L.size() == 3);

        L.pop_front(); // removes 1
        L.pop_back();  // removes 3
        assert(L.size() == 1);
    }
    // after L is destroyed, at least constructions==destructions
    assert(Counter::destructions >= Counter::constructions);
}

// tests for insert 
{
    //insert into middle
    LinkedList<int> L;
    L.push_back(1);
    L.push_back(2);
    L.push_back(3); // 1 2 3

    L.insert_at(1, 10);
    assert(L.size() == 4);
    int cur = 0;
    LinkedList<int>::Node* temp = L.head();
    while(cur<1){
        temp = temp->next;
        cur++;
    }
    assert(temp->data == 10);

    L.insert_at(0, 99);
    assert(L.size() == 5);
    assert(L.head() -> data == 99);

    L.insert_at(5,77);
    assert(L.size() == 6);
    assert(L.tail()->data == 77);

}

std::cout << "LinkedList tests passed.\n";

}