#pragma once
#include <stdlib.h>
#include <assert.h>
#include "common.hpp"

class Random {
    public:
        static float Float(float min = 0.0f, float max = 1.0f) {
            assert(max > min);
            float range = max - min;
            return (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * range + min;
        }

        template<typename T>
        static T ListItem(const std::vector<T> &list) {
            assert(!list.empty());
            return list[(int)floor(Float()*list.size())];
        }
        static vec2 Vec2(vec2 v) {
            return vec2(Float()*v.x,Float()*v.y);
        }
};
