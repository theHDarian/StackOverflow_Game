#pragma once

#include <iostream>
#include <vector>
#include <algorithm> // for std::find

template<typename T>
class ExtendedStack {
private:
    std::vector<T> stack;

public:
    void push(const T& item) {
        stack.push_back(item);
    }

    void pop() {
        if (!stack.empty()) {
            stack.pop_back();
        }
    }

    T top() const {
        if (!stack.empty()) {
            return stack.back();
        }
        throw std::out_of_range("Stack is empty");
    }

    bool empty() const {
        return stack.empty();
    }

    bool remove(const T& item) {
        auto it = std::find(stack.begin(), stack.end(), item);
        if (it != stack.end()) {
            stack.erase(it);
            return true; 
        }
    }
};