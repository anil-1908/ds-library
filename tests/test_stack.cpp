#include "../src/stack.hpp"
#include <cassert>
#include <iostream>

int main(){
    Stack<int> st;
    assert(st.isEmpty());

    st.push(2);
    st.push(4);

    assert(st.top() == 4);
    assert(st.size() == 2);

    st.pop();
    assert(st.top() == 2);

    std::cout<<"All stack tests passed";
}