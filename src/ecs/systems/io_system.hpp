#pragma once

#include "common.hpp"
#include <array>
#include <utility>

#include "components.hpp"
#include "tiny_ecs.hpp"

// System responsible for setting up OpenGL and for rendering all the
// visual entities in the game
class IOSystem {
public:
    IOSystem();
    ~IOSystem();
	// Initialize the window
	bool init(GLFWwindow* window);

    bool isPaused()const;

private:
	GLFWwindow* window;
    void onKey(int key, int, int action, int mod);

    void mouseClick(int button, int action, int mods);

    void onMouseMove(vec2 mousePosition);

    void handleMovementInput(int key, int action, IOState& state);
};