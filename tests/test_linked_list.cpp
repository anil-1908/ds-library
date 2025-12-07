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

// copy ctor, move ctor, copy and move assignments
{
    LinkedList<int> a;
    a.push_back(1);
    a.push_back(2);
    a.push_back(3);

    LinkedList<int> b(a);
    assert(b.size() == 3);
    assert(b.head()->data == 1);
    assert(b.head()->next->data == 2);
    assert(a.head() != b.head());  // deep copy

    LinkedList<int> c(std::move(b));//move ctor
    assert(c.size() == 3);
    assert(b.size() == 0);
    assert(b.head() == nullptr);

    LinkedList<int> d; //copy assignment 
    d.push_back(10);
    d = c;
    assert(d.size() == c.size());

    LinkedList<int> e;
    e = std::move(c);
    assert(e.size() == 3);
    assert(c.size() == 0);

}


//Deep-copy test with non-trivial type (Counter)
{
    Counter::constructions = Counter::destructions = Counter::copies = Counter::moves = 0;
    {
        LinkedList<Counter> a;
        a.push_back(Counter(5));
        a.push_back(Counter(6));
        LinkedList<Counter> b(a);
        assert(b.size() == 2);
        // ensure deep copy: heads not same pointer
        assert(a.head() != b.head());
    }
    // after scope all destructors should have run
    assert(Counter::destructions >= Counter::constructions);
}

// Many operations loop test (stress, sanity)
{
    LinkedList<int> a;
    for (int i=0;i<1000;i++) a.push_back(i);
    LinkedList<int> b(a);
    LinkedList<int>::Node* cur = b.head();
    for (int i=0;i<1000;i++) {assert(cur -> data == i && "iterate and compare"); cur = cur->next;}
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
//iterators check
{
    LinkedList<int> L;
    for (int i = 1; i <= 5; i++) L.push_back(i);

    int x = 1;
    for (auto& v : L) {
        assert(v == x++);
    }
}

{
    const LinkedList<int> L_const = []{
        LinkedList<int> tmp;
        tmp.push_back(10);
        tmp.push_back(20);
        return tmp;
    }();

    int x = 10;
    for (const auto& v : L_const) {
        assert(v == x);
        x += 10;
    }
}

std::cout << "LinkedList tests passed.\n";

}