#pragma once

#include "common.hpp"
#include "components.hpp"
#include "tiny_ecs.hpp"
#include "tiny_ecs_registry.hpp"

// System responsible for handling camera
// ref: https://learnopengl.com/Getting-started/Camera
class CameraSystem {
public:
    CameraSystem();
    ~CameraSystem();
    // Initialize the window
    //bool init(/*GLFWwindow* window*/);
    void step(float elapsed_ms);

private:
    
};