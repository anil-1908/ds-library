#ifndef STACK_HPP
#define STACK_HPP

#include "dynamic_array.hpp"
#include <cassert>
#include <cstddef>
#include <utility>

template<typename T>
class Stack{
public:
    using size_type = std::size_t;
    Stack()
        : size_(0) {}
   

bool isEmpty() const noexcept{
    return size_ == 0;
}

void push(const T& value ){
    buffer_.push_back(value);
    ++size_;
}

void push(T&& value){
    buffer_.push_back(std::move(value));
    ++size_;
}

void pop(){
    assert(size_ > 0 && "Pop on empty stack");
    buffer_.pop_back();
    --size_;
}

T& top(){
    assert(size_ > 0 && "Top on empty stack");
    return buffer_[size_ - 1];
}

const T& top() const {
    assert(size_ > 0 && "Top on empty stack");
    return buffer_[size_ - 1];
}

void clear(){
    buffer_.clear();
    size_ = 0 ;
}

size_type size() const noexcept{return size_;}

private:
    DynamicArray<T> buffer_;
    size_type size_;

};


#endif /*STACK HPP*/