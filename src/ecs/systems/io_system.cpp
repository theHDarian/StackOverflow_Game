#include "io_system.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

#include "tiny_ecs_registry.hpp"


IOSystem::IOSystem() {
    std::cout << "create sys" << std::endl;    
}
IOSystem::~IOSystem() {
    std::cout << "destroy sys" << std::endl;
}
bool IOSystem::init(GLFWwindow* window) {
    this->window = window;

    Entity ioEntity = Entity();
    registry.ioStates.emplace(ioEntity);

    auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((IOSystem*)glfwGetWindowUserPointer(wnd))->onKey(_0, _1, _2, _3); };
	auto cursor_pos_redirect = [](GLFWwindow* wnd, double _0, double _1) { ((IOSystem*)glfwGetWindowUserPointer(wnd))->onMouseMove({ _0, _1 }); };
	glfwSetKeyCallback(window, key_redirect);
	glfwSetCursorPosCallback(window, cursor_pos_redirect);
    return true;
}

// On key callback
void IOSystem::onKey(int key, int, int action, int mod) {
    IOState& state = registry.ioStates.components[0];
	if (key == GLFW_KEY_ESCAPE) {
		state.shouldEnd = true;
	}

	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R) {
		int w, h;
		glfwGetWindowSize(window, &w, &h);

        state.shouldRestart = true;
	}

	//Player movement
	state.inputAxis = handleMovementInput(key,action,state.inputAxis);
}

void IOSystem::onMouseMove(vec2 mousePosition) {	
    IOState& state = registry.ioStates.components[0];
    state.mousePosition = mousePosition;
}

vec2 IOSystem::handleMovementInput(int key, int action, vec2 lastInput) {
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (key == GLFW_KEY_A)
			lastInput = {-1.0f,lastInput[1]};
		else if (key == GLFW_KEY_D)
			lastInput = {1.0f,lastInput[1]};
		else if (key == GLFW_KEY_W)
			lastInput = {lastInput[0],-1.0f}; //up is negative
		else if (key == GLFW_KEY_S)
			lastInput = {lastInput[0],1.0f};
	} else if (action == GLFW_RELEASE) { //on release, reset to zero or based on other held key
		if (key == GLFW_KEY_A)
			lastInput = {glm::max(lastInput[0],0.0f),lastInput[1]};
		else if (key == GLFW_KEY_D)
			lastInput = {glm::min(lastInput[0],0.0f),lastInput[1]};
		else if (key == GLFW_KEY_W)
			lastInput = {lastInput[0],glm::max(0.0f,lastInput[1])}; //up is negative
		else if (key == GLFW_KEY_S)
			lastInput = {lastInput[0],glm::min(0.0f,lastInput[1])};
	}
    return lastInput;
}