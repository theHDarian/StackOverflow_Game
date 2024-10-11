#include "io_system.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

#include "tiny_ecs_registry.hpp"
#if IMGUI_ENABLED
#include "imgui_impl_glfw.h"
#endif


IOSystem::IOSystem() {
}
IOSystem::~IOSystem() {
}
bool IOSystem::init(GLFWwindow* window) {
    this->window = window;

    Entity ent = Entity();
    registry.ioStates.emplace(ent);
	registry.gameStates.emplace(ent);

    auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((IOSystem*)glfwGetWindowUserPointer(wnd))->onKey(_0, _1, _2, _3); };
	auto cursor_pos_redirect = [](GLFWwindow* wnd, double _0, double _1) { ((IOSystem*)glfwGetWindowUserPointer(wnd))->onMouseMove({ _0, _1 }); };
	auto mouseClick = [](GLFWwindow* wnd, int _0, int _1, int _2) { ((IOSystem*)glfwGetWindowUserPointer(wnd))->mouseClick(_0, _1, _2); };
	glfwSetKeyCallback(window, key_redirect);
	glfwSetCursorPosCallback(window, cursor_pos_redirect);
	glfwSetMouseButtonCallback(window, mouseClick);
    return true;
}


// On key callback
void IOSystem::onKey(int key, int _, int action, int mod) {
	#if IMGUI_ENABLED
	ImGui_ImplGlfw_KeyCallback(window, key,_,action,mod);
	#endif
    IOState& ioState = registry.ioStates.components[0];
	GameState& gameState = registry.gameStates.components[0];
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		gameState.gamePaused = !gameState.gamePaused;
	}
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		ioState.shouldEnd = true;
	}

	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R) {
		int w, h;
		glfwGetWindowSize(window, &w, &h);

        ioState.shouldRestart = true;
	}

	// show dialogue window and play dialogue sequence (temp function)
	if (action == GLFW_RELEASE && key == GLFW_KEY_E) {
		// pause game when dialogue plays for now
		if (!ioState.shouldShowDialogue)
			gameState.dialogueScene = true;
		ioState.shouldShowDialogue = true;
		ioState.nextDialogue = true;
	}

	//Player movement
	handleMovementInput(key,action,ioState);

}

void IOSystem::mouseClick(int button, int action, int mods) {
	#if IMGUI_ENABLED
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
	#endif

	IOState& state = registry.ioStates.components[0];
	if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS) {
		state.shouldDash = true;
	}
	if (button == GLFW_MOUSE_BUTTON_1) {
		state.shouldShoot = (action == GLFW_PRESS  || action == GLFW_REPEAT);
	}

}

void IOSystem::onMouseMove(vec2 mousePosition) {	
	#if IMGUI_ENABLED
	ImGui_ImplGlfw_CursorPosCallback(window, mousePosition.x, mousePosition.y);
	#endif
    IOState& state = registry.ioStates.components[0];
    state.mousePosition = mousePosition;
}

void IOSystem::handleMovementInput(int key, int action, IOState& state) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_A) {
			state.pressedHorizontal.push(-1.0f);
		} else if (key == GLFW_KEY_D) {
			state.pressedHorizontal.push(1.0f);
		} else if (key == GLFW_KEY_W) {
			state.pressedVertical.push(-1.0f);
		} else if (key == GLFW_KEY_S) {
			state.pressedVertical.push(1.0f);
		}
        if (key == GLFW_KEY_SPACE || key == GLFW_MOUSE_BUTTON_1) {

            state.shouldDash = true;
        }
	} else if (action == GLFW_RELEASE) {
		//on release, reset to last pressed key
		if (key == GLFW_KEY_A) {
			state.pressedHorizontal.remove(-1.0f);
		} else if (key == GLFW_KEY_D){
			state.pressedHorizontal.remove(1.0f);
		} else if (key == GLFW_KEY_W){
			state.pressedVertical.remove(-1.0f);
		} else if (key == GLFW_KEY_S){
			state.pressedVertical.remove(1.0f);
		}
	}
	float horizontalAxis = state.pressedHorizontal.empty() ? 0.0f : state.pressedHorizontal.top();
	float verticalAxis = state.pressedVertical.empty() ? 0.0f : state.pressedVertical.top();
	// std::cout << horizontalAxis << " " << verticalAxis << std::endl;
    state.inputAxis = {horizontalAxis,verticalAxis};
	if (state.inputAxis != vec2(0.0f)) {
		state.lastInputAxis = state.inputAxis;
	}

}

bool IOSystem::isPaused()const {
	return registry.gameStates.components[0].gamePaused;
};
bool IOSystem::isGameOver()const {
	return registry.gameStates.components[0].gameOver;
}

// snce dialogue triggers on keypress for now
// move it here for now
bool IOSystem::isDialogue()const {
	return registry.gameStates.components[0].dialogueScene;
}