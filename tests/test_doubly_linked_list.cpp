#include "../src/doubly_linked_list.hpp"
#include <cassert>
#include <iostream>


int main(){
    DoublyLinkedList<int> L;
    assert(L.empty());

    L.push_back(1);
    L.push_back(2);
    L.push_back(3);
    assert(L.size() == 3);
    assert(L.head()->data == 1);
    assert(L.tail()->data == 3);

    L.pop_front(); // now [2,3]
    assert(L.size() == 2);
    assert(L.head()->data == 2);

    L.pop_back(); // now [2]
    assert(L.size() == 1);
    assert(L.head() == L.tail());
    assert(L.head()->data == 2);

    L.push_front(0); // now [0,2]
    assert(L.size() == 2);
    assert(L.head()->data == 0);
    assert(L.tail()->data == 2);

    // L.clear();
    // assert(L.size() == 0);
    // assert(L.head() == nullptr && L.tail() == nullptr);

    //erase_at function tests
    {
        DoublyLinkedList<int> L;
        for(int i = 0; i < 10; ++i){
            L.push_back(i);
        }
        L.erase_at(0);
        assert(L.size() == 9);
        assert(L.head() -> data == 1);
        
        L.erase_at(8);
        assert(L.size() == 8);
        assert(L.tail() -> data == 8);

        L.erase_at(2);
        assert(L.size() == 7);
        DoublyLinkedList<int>::Node* cur = L.head();
        int idx = 0;
        while(idx < 2){
            cur = cur -> next;
            idx++;
        }
        assert(cur -> data = 4);
        // L.erase_at(7);

    }

    std::cout << "DoublyLinkedList tests passed.\n";
    return 0;
}