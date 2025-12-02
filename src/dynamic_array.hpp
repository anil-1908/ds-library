
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

    ~DynamicArray(){
        //destruct constructed elements
        for(size_type i=0; i < size_; i++){
            alloc_.destroy(data_ + i);
        }
        //deallocate any extra reserved data
        if(data_) alloc_.deallocate(data_, capacity_);
        //assigning default values back
        data_ = nullptr;
        size_ = 0;
        capacity_ = 0;
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

    size_type size() const noexcept{
        return size_;
    }

    size_type capacity() const noexcept{
        return capacity_;
    }

    bool empty() const noexcept{
        return size_ == 0;
    }

    //push back lvalue
    void push_back(const T& value){
        ensure_capacity_for_push();
        alloc_.construct(data_ + size_, value);
        ++size_;
    }

    //push back rvalue
    void push_back(T&& value){
        ensure_capacity_for_push();
        alloc_.construct(data_ + size_, std::move(value));
        ++size_;
    }

    void pop_back(){
        assert(size_ > 0 && "Pop back on empty DynamicArray");
        --size_;
        alloc_.destroy(data_ + size_);
        maybe_shrink();
    }

    // shrink-to-policy: when size_ <= capacity_/4, shrink to max(1, size_*2)
    void maybe_shrink(){
        if (capacity_ == 0) return;              // nothing to do
        if (size_ > capacity_ / 4) return;       // not enough slack to shrink

        size_type new_cap = size_== 0 ? 1: size_ * 2; 
        
        T* new_data = alloc_.allocate(new_cap);
        size_type i = 0;
        try{
            for(; i < size_ ; ++i){
            alloc_.construct(new_data + i, std::move_if_noexcept(data_[i]));
            }
        }
        catch(...){
            for(size_type j = 0; j < i; ++j){
                alloc_.destroy(new_data + j);
            }
            alloc_.deallocate(new_data, new_cap);
            throw;
        }

        for(size_type j = 0; j < size_; j++) {alloc_.destroy(data_ + j);}
        if(data_) alloc_.deallocate(data_, capacity_);

        data_ = new_data;
        capacity_ = new_cap;
    }                    
    T& operator[](size_type index){
        assert(index < size_ && "index out of bound");
        return data_[index];
    }

    const T& operator[](size_type index) const {
        assert(index < size_ && "index out of bound");
        return data_[index];
    }       

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