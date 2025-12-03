
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

    explicit DynamicArray(size_type n, const T& value) 
        :data_(nullptr), size_(0), capacity_(0)
        {
            if(n) reserve(n);
            for(size_type i=0; i < n; i++){
                alloc_.construct(data_+i, value);
            }
            size_ = n;
        }

    //copy ctor
    DynamicArray(const DynamicArray<T>& other)
        :data_(nullptr), size_(0), capacity_(0)
    {   
        if(other.size_){
           T* new_data = alloc_.allocate(other.size_);
           size_type i = 0;
           try{
                for(;i < other.size_; i++){
                    alloc_.construct(new_data + i, other[i]);
                }
           }
           catch(...){
                for(size_type j = 0; j < i; j++){
                    alloc_.destroy(new_data + j);
                }
                alloc_.deallocate(new_data, other.size_);
           }
           data_ = new_data;
           size_ = other.size_;
           capacity_ = other.size_;
        }
    }

    //move ctor
    DynamicArray(DynamicArray<T>&& other) noexcept
        :alloc_(std::move(other.alloc_)), data_(other.data_), size_(other.size_), capacity_(other.capacity_)
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
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

    //Assignment (copy)
     DynamicArray& operator=(const DynamicArray<T>& other){
        if(this == &other) return *this;
        DynamicArray<T> temp(other);
        swap(temp);
        return *this;
     }

     //Assignment Move
    DynamicArray& operator=(DynamicArray<T>&& other) noexcept{
        if(this == &other) return *this;
        if(data_){
            for(size_type i = 0; i < size_; i++){
                alloc_.destroy(data_+i);
            }
            alloc_.deallocate(data_, capacity_);
        }
        alloc_ = std::move(other.alloc_);
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
        return *this;
    }     

    void swap(DynamicArray<T>& other) noexcept{
        using std::swap;
        swap(alloc_, other.alloc_);
        swap(data_, other.data_);
        swap(size_, other.size_);
        swap(capacity_, other.capacity_);
    }
    void reserve(size_type new_cap) {
        if (new_cap <= capacity_) return;
         T* new_data = alloc_.allocate(new_cap);
        // move/construct existing elements
        size_type i = 0;
        try{
            for (; i < size_; ++i) {
            alloc_.construct(new_data + i, std::move_if_noexcept(data_[i]));
            }
        }
        catch(...){
            for(size_type j = 0; j < i; ++j) alloc_.destroy(new_data + j);
            alloc_.deallocate(new_data, new_cap);
            throw;
        }

        for(size_type j=0; j < size_; j++) alloc_.destroy(data_+j);  
        if (data_) alloc_.deallocate(data_, capacity_);
        data_ = new_data;
        capacity_ = new_cap;
    }

    void resize(size_type new_size){
        if(size_ > new_size){
            for(size_type i = new_size; i < size_; i++) {
                alloc_.destroy(data_ + i);
            }
            size_ = new_size;
            maybe_shrink();
        }

        if(new_size > capacity_){
            reserve(std::max(new_size, capacity_*2));
        }

        for(size_type i = size_; i < new_size; i++){
            alloc_.construct(data_+i, T());
        }
        size_ = new_size;
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

    template <typename... Args>
    void emplace_back(Args&&... args){
        ensure_capacity_for_push();
        alloc_.construct(data_+size_, std::forward<Args>(args)...);
        ++size_;
    }

    void pop_back(){
        assert(size_ > 0 && "Pop back on empty DynamicArray");
        --size_;
        alloc_.destroy(data_ + size_);
        maybe_shrink();
    }

    //lvalue
    void insert(size_t index, const T& value){
        assert(index <= size_  && index >= 0 && "Index out of bound");
        ensure_capacity_for_push();
        for(int i = size_ ;i > index; --i){
            alloc_.construct(data_ + i, std::move_if_noexcept(data_[i-1]));
            alloc_.destroy(data_+ (i - 1));
        }
        alloc_.construct(data_+index, value);
        ++size_;
    }

    //rvalue
    void insert(size_t index,  T&& value){
        assert(index <= size_  && index >= 0 && "Index out of bound");
        ensure_capacity_for_push();
        for(int i = size_ ;i > index; --i){
            alloc_.construct(data_ + i, std::move_if_noexcept(data_[i-1]));
            alloc_.destroy(data_+ (i - 1));
        }
        alloc_.construct(data_+index, std::move(value));
        ++size_;
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

    void erase(int index){
        assert(index <= size_  && index >= 0 && "Index out of bound");
        for(size_t i = index; i < size_ - 1; ++i){
            alloc_.destroy(data_ + i);
            alloc_.construct(data_ + i, std::move_if_noexcept(data_[i+1]));
        }
        --size_;
        maybe_shrink();
    }
    T* begin() noexcept { return data_; }
    T* end() noexcept{ return data_ + size_; }

    const T* begin() const noexcept { return data_; }
    const T* end() const noexcept{ return data_ + size_; }
    T* data() noexcept { return data_; }
    const T* data() const noexcept { return data_; }
    
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