#pragma once
#include <stdlib.h>
#include <assert.h>
#include "common.hpp"

class Random {
    public:
        static float Float(float val = 1.0f) {
            return (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * val;
        }

        template<typename T>
        static T ListItem(const std::vector<T> &list) {
            assert(!list.empty());
            return list[(int)floor(Float()*list.size())];
        }
        static vec2 Vec2(vec2 v) {
            return vec2(Float()*v.x,Float()*v.y);
        }

        static vec2 Direction() {
            float angle = Float() * 2.0f * M_PI;
            return glm::vec2(cos(angle), sin(angle));
        }
};
