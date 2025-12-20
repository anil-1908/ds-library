#ifndef DOUBLY_LINKED_LIST_HPP
#define DOUBLY_LINKED_LIST_HPP

#include <cstddef>
#include <cassert>
#include <utility>

template<typename T>
class DoublyLinkedList{
    using size_type = std::size_t;
public:
    struct Node{
        T data;
        Node* next;
        Node* prev;
        template<typename... Args>
        Node(Args&&... args) :data(std::forward<Args>(args)...), next(nullptr), prev(nullptr) {}
    };

    DoublyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}
    ~DoublyLinkedList() {clear();}

    bool empty() const noexcept {return size_ == 0;}
    size_type size() const noexcept {return size_;}
    Node* head() noexcept {return head_;}
    Node* tail() noexcept {return tail_;}

    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList operator=(const DoublyLinkedList&) = delete;

    void push_back(const T& value){
        Node* temp = new Node(value);
        temp -> prev = tail_;
        if(tail_) tail_ -> next = temp;
        tail_ = temp;
        if(!head_) head_ = temp;
        ++size_;
    }

    void push_back(T&& value){
        Node* temp = new Node(std::move(value));
        temp -> prev = tail_;
        if(tail_) tail_ -> next = temp;
        tail_ = temp;
        if(!head_) head_ = temp;
        ++size_;
    }

    void push_front(const T& value){
        Node* temp = new Node(value);
        temp -> next = head_;
        if(head_) head_ -> prev = temp;
        head_ = temp;
        if(!tail_) tail_ = temp;
        ++size_;
    }

    void push_front(T&& value){
        Node* temp = new Node(std::move(value));
        temp -> next = head_;
        if(head_) head_ -> prev = temp;
        head_ = temp;
        if(!tail_) tail_ = temp;
        ++size_;
    }

    void pop_back(){
        assert(size_ > 0 && "Pop_back on empty list");
        Node* temp = tail_;
        tail_ = tail_ -> prev;
        if(tail_) tail_ -> next = nullptr;
        else head_ = nullptr;
        delete temp;
        --size_;
    }

    void pop_front(){
        assert(size_ > 0 && "Pop_front on empty list");
        Node* temp = head_;
        head_ = head_ -> next;
        if(head_) head_ -> prev = nullptr;
        else tail_ = nullptr;
        delete temp;
        --size_;
    }

    void clear() noexcept{
        Node * cur = head_;
        while(cur){
            Node* temp = cur;
            cur = cur -> next;
            delete temp;
        }
        head_ = tail_ = nullptr;
        size_ = 0;

    }

    void erase_at(size_type index){
        assert(index < size_ && "Index Out of bounds DoublyLinkedList::erase_at");
        if(index == 0) {pop_front(); return;}
        if(index == size_ - 1) {pop_back(); return;}
        Node* cur = head_;
        size_type idx = 0;
        while(idx<index){
            cur = cur -> next;
            idx++;
        }
        Node* previous_node   = cur -> prev;
        Node* next_node       = cur -> next;
        previous_node -> next = next_node;
        next_node -> prev     = previous_node;
        delete cur;
        --size_;
    }
private:

    Node* head_;
    Node* tail_;
    size_type size_;
};

#endif /*DOUBLY LINKED LIST HPP*/
