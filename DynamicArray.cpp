#include <iostream>

class DynamicArray{
private:
    int* data;                          //data array
    size_t size;                        //Total size
    int capacity;                       //Max Capacity it can hold

public:
    DynamicArray();                     //Constructor    
    ~DynamicArray();                    //Destructor
    void push_back(int x);              //Dynamic addition of element
    void pop_back();                    //Dynamic delete of last element
    int operator[](int i) const;        //operator overloading for []
    int operator()(int i) const;        //Operator overloading for ()
};