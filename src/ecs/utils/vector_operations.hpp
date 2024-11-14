#pragma once
#include "common.hpp"

class VecOp {
    public:
        static vec2 rotate(vec2 v, float angle) {
            vec2 rotated;
            rotated.x = cos(angle)*v.x - sin(angle)*v.y;
            rotated.y = sin(angle) * v.x + cos(angle) * v.y;
            return rotated;
        }
};
