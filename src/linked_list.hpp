#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <iostream>
#include <cassert>
#include <memory>
#include <cstddef>
#include <utility>

template <typename T>
class LinkedList{
public:
struct Node{
    T data;
    Node* next;
    template <typename... Args>
    Node(Args&&... args) noexcept : data(std::forward<Args>(args)...), next(nullptr) {}
};

    LinkedList(): head_{nullptr}, tail_{nullptr}, size_{0}{}
    ~LinkedList(){clear();}

    //no copy/move for now will be implemented later 
    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;

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
            Node* temp = cur;
            temp = cur -> next;
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

    Node* head() noexcept {return head_;}
    const Node* head() const noexcept { return head_;}


private:
    Node* head_;
    Node* tail_;
    size_t size_;

};

#endif /* LINKED_LIST_HPP */