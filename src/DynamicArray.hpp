
#include <iostream>
#include <memory>
#include <cassert>
#include <algorithm>
#include <cstddef>
#include <utility>

template <typename T> 
class DynamicArray{
public:
    using value_type = T;
    using size_type  = std::size_t;
    DynamicArray()
        :data_(nullptr), size_(0), capacity_(0) {}                     //Default Constructor    
    explicit DynamicArray(size_type n)
        :data_(nullptr), size_(0), capacity_(0)
    {
        if(n) reserve(n);
        for(size_type i=0; i < n; i++){
            alloc_.construct(data_+i, T());
        }
        size_ = n;
    }
     void reserve(size_type new_cap) {
        if (new_cap <= capacity_) return;
         T* new_data = alloc_.allocate(new_cap);
        // move/construct existing elements
        for (size_type i = 0; i < size_; ++i) {
            alloc_.construct(new_data + i, std::move_if_noexcept(data_[i]));
            alloc_.destroy(data_ + i);
        }
        if (data_) alloc_.deallocate(data_, capacity_);
        data_ = new_data;
        capacity_ = new_cap;
    }

    ~DynamicArray();                    //Destructor
    void push_back(int x);              //Dynamic addition of element
    void pop_back();                    //Dynamic delete of last element
    int operator[](int i) const;        //operator overloading for []
    int operator()(int i) const;        //Operator overloading for ()

private:
    std::allocator<T> alloc_;
    T* data_;
    size_type size_;
    size_type capacity_;
    
    void ensure_capacity_for_push(){
        if(capacity_ == 0){
            reserve(1);
            return;
        }
        if(size_ >= capacity_){
            //grow policy: capacity *= 2;
            reserve(capacity_ * 2);
        }
    }
};