#pragma once
#include <stdlib.h>
#include <assert.h>

class Random {
    public:
        static float Float(float min = 0.0f, float max = 1.0f) {
            assert(max > min);
            float range = max - min;
            return (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * range + min;
        }
        
        template<typename T>
        static T* ListItem(const std::vector<T> &list) {
            if (list.empty()) return nullptr;
            return const_cast<T*>(&list[(int)floor(Float()*list.size())]);
        }
};
