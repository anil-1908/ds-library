#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <iostream>
#include <cassert>
#include <memory>
#include <cstddef>
#include <utility>
#include <iterator>

template <typename T>
class LinkedList{
public:
struct Node{
    T data;
    Node* next;
    template <typename... Args>
    Node(Args&&... args) : data(std::forward<Args>(args)...), next(nullptr) {}
};

class iterator {
    friend class const_iterator;
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = T;
    using difference_type   = std::ptrdiff_t;
    using pointer           = T*;
    using reference         = T&;

    iterator(Node* ptr) : node(ptr) {}
    iterator(const iterator& it) : node(it.node) {} 

    reference operator*()  const { return node->data; }
    pointer   operator->() const { return &node->data; }

    iterator& operator++() { node = node->next; return *this; }
    iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

    bool operator==(const iterator& other) const { return node == other.node; }
    bool operator!=(const iterator& other) const { return node != other.node; }


private:
    Node* node;
};


class const_iterator{
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = T;
    using difference_type   = std::ptrdiff_t;
    using pointer           = const T*;
    using reference         = const T&;

    const_iterator(const Node* ptr) : node(ptr) {}
    const_iterator(const iterator& it) : node(it.node) {} 

    reference operator*() const {return node->data;}
    pointer operator->() const {return &node->data;}

    const_iterator& operator++() { node = node->next; return *this; }
    const_iterator operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }

    bool operator==(const const_iterator& other) const { return node == other.node; }
    bool operator!=(const const_iterator& other) const { return node != other.node; }

    friend bool operator==(const iterator& a, const const_iterator& b) noexcept {
        return a.node == b.node;
    }
    friend bool operator==(const const_iterator& a, const iterator& b) noexcept {
        return a.node == b.node;
    }
    friend bool operator!=(const iterator& a, const const_iterator& b) noexcept {
        return !(a == b);
    }
    friend bool operator!=(const const_iterator& a, const iterator& b) noexcept {
        return !(a == b);
    }

private:
    const Node* node;
};
    
    using size_type = std::size_t;
    LinkedList(): head_{nullptr}, tail_{nullptr}, size_{0}{}
    ~LinkedList(){clear();}

    LinkedList(const LinkedList& other): head_{nullptr}, tail_{nullptr}, size_{0}
    {
        Node* cur = other.head_;
        while(cur){
            push_back(cur -> data);
            cur = cur -> next;
        }
    }

    LinkedList(LinkedList&& other)
         noexcept : head_{std::exchange(other.head_, nullptr)}, tail_{std::exchange(other.tail_, nullptr)}, size_{std::exchange(other.size_, 0)}{}



    LinkedList& operator=(const LinkedList& other){
        if(this == &other) return *this;

        LinkedList temp(other);
        std::swap(head_, temp.head_);
        std::swap(tail_, temp.tail_);
        std::swap(size_, temp.size_);   
        return *this; 
    }

    LinkedList& operator=(LinkedList&& other) noexcept{
        if(this == &other) return *this;
        clear();
        
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;

        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;

        return *this;
    }

    size_t size() const noexcept {return size_;}
    bool empty() const noexcept { return size_ == 0; }

    void push_front(const T& value){
        Node* temp = new Node(value);
        temp -> next = head_;
        head_ = temp;
        if(!tail_) tail_ = head_;
        ++size_;
    }

    void push_front(T&& value){
        Node* temp = new Node(std::move(value));
        temp -> next = head_;
        head_ = temp;
        if(!tail_) tail_ = head_;
        ++size_;
    }

    void push_back(const T& value){
        Node* temp = new Node(value);
        if(!tail_) {head_ = tail_ = temp;}
        else {tail_ -> next = temp; tail_ = temp;}
        ++size_;
    }

    void push_back(T&& value){
        Node* temp = new Node(std::move(value));
        if(!tail_) {head_ = tail_ = temp;}
        else {tail_ -> next = temp; tail_ = temp;}
        ++size_;
    }

    void clear(){
        Node* cur = head_;
        while(cur){
            Node* temp = cur -> next;
            delete cur;
            cur = temp;
        }
        head_ = tail_ = nullptr;
        size_ = 0;
    }

    void pop_front(){
        assert(size_ != 0 && "pop_front() on empty list");
        Node* temp = head_;
        head_ = head_ -> next;
        delete temp;
        --size_;
        if(size_ == 0) tail_ = nullptr;
    }

    void pop_back(){
        assert(size_ != 0 && "pop_back on empty list");
        if(size_ == 1){
            delete tail_;
            head_ = tail_ = nullptr;
            size_ = 0;
            return;
        }
        Node* temp = head_;
        while(temp -> next != tail_){
            temp = temp -> next;
        }
        temp -> next = nullptr;
        delete tail_;
        tail_ = temp;
        --size_;
    }

    void insert_at(size_type index, const T& value){
        assert(index <= size_ && index >= 0 && "Index out of bound");

        if(index == 0) {
            push_front(value);
             return;
        }

        if(index == size_) {
            push_back(value);
            return;
        }

        Node* cur = head_;
        size_type cur_index = 0;

        while(cur_index < index-1){
            cur = cur -> next;
            cur_index++;
        }

        Node* temp = new Node(value);
        temp -> next = cur -> next;
        cur -> next = temp;
        ++size_;
    }

    void insert_at(size_type index, T&& value){
        assert(index <= size_ && index >= 0 && "Index out of bound");

        if(index == 0) {
            push_front(std::move(value));
             return;
        }

        if(index == size_) {
            push_back(std::move(value));
            return;
        }

        Node* cur = head_;
        for (size_type i = 0; i < index - 1; ++i) {
            cur = cur->next;
        }

        Node* temp = new Node(std::move(value));
        temp -> next = cur -> next;
        cur -> next = temp;
        ++size_;
    }

  
    iterator begin() {return iterator(head_);}
    iterator end()  {return iterator(nullptr);}

    const_iterator begin() const noexcept {return const_iterator(head_);}
    const_iterator cbegin() const noexcept {return const_iterator(head_);}
    const_iterator cend() const noexcept {return const_iterator(nullptr);}  
    const_iterator end()   const noexcept {return const_iterator(nullptr);}   

    Node* head() noexcept {return head_;}
    const Node* head() const noexcept { return head_;}
    
    Node* tail() noexcept {return tail_;}
    const Node* tail() const noexcept { return tail_;}


private:
    Node* head_;
    Node* tail_;
    size_t size_;

};

#endif /* LINKED_LIST_HPP */