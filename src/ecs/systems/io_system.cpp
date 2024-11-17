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
	if (key == GLFW_KEY_P && action == GLFW_PRESS && !gameState.titleScreen) {
		gameState.gamePaused = !gameState.gamePaused;
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		ioState.shouldEnd = true;
	}
	if  (key == GLFW_KEY_MINUS && action != GLFW_RELEASE) {
		gameState.currentVolume = std::max(0.0f, gameState.currentVolume - 0.0125f);
	}
	if  (key == GLFW_KEY_EQUAL && action != GLFW_RELEASE) {
		gameState.currentVolume = std::min(1.0f, gameState.currentVolume + 0.0125f);
	}

	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R) {
		int w, h;
		glfwGetWindowSize(window, &w, &h);

        //ioState.shouldRestart = true;
		gameState.titleScreen = true;
	}

	// Debug toggle for colliders
	if (key == GLFW_KEY_C && action == GLFW_RELEASE) {
		ioState.debugMode = !ioState.debugMode;
	}

	// turn FPS counter on/off
	if (key == GLFW_KEY_F && action == GLFW_RELEASE) {
		ioState.showFPS = !ioState.showFPS;
	}

	// interacted with object/play story dialogue
	if (action == GLFW_RELEASE && key == GLFW_KEY_E && !gameState.gamePaused && !gameState.cutScene) {
		ioState.nextDialogue = true;

		if (gameState.titleScreen) {
			if (ioState.hoveringMenuChoice == 0) {
				gameState.titleScreen = false;
				ioState.shouldRestart = true;
			}
			else {
				ioState.shouldEnd = true;
			}
		}

		// take latest object
		if (registry.nearbyInteractables.entities.size() > 0) {
			size_t index = registry.nearbyInteractables.entities.size() - 1;
			InteractableObject& object = registry.interactables.get(registry.nearbyInteractables.entities[index]);
			if (object.interactType == InteractableType::DialogueInteractable) {
				registry.dialogueRequests.emplace(registry.nearbyInteractables.entities[index]);
			}
			else {
				// no idea what choice to map, just assume all Es will map to 0
				registry.interactableReactions.emplace_with_duplicates(registry.nearbyInteractables.entities[index], registry.nearbyInteractables.entities[index], 0);
			}
		}
		else if (!gameState.dialogueScene && registry.maps.components[0].currRoom.dialogueDone) {
			if (!registry.dialogueRequests.has(registry.players.entities[0])) {
				DialogueRequest& req = registry.dialogueRequests.emplace(registry.players.entities[0]);
				req.type = DialogueRequestType::StoryDialogue;
			}
		}
	}
	
	if (gameState.dialogueScene && !gameState.gamePaused) {
		handleDialogueChoice(key, action, ioState, gameState);
	}
	if (gameState.titleScreen) {
		// only for title for now, think of where else menus might appear
		// menu state? or just pause?
		handleMenuChoice(key, action, ioState, gameState);
	}
	handleMovementInput(key, action, ioState, gameState); // seems like always need to handle this, or else weird movement bugs

}

void IOSystem::handleMenuChoice(int key, int action, IOState& state, GameState& gameState) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_W) { // highlight choice above
			state.lastHoverMenuChoice = state.hoveringMenuChoice;
			state.hoveringMenuChoice = max(0, state.hoveringMenuChoice - 1);
		}
		else if (key == GLFW_KEY_S) { // highlight choice below
			state.lastHoverMenuChoice = state.hoveringMenuChoice;
			
			state.hoveringMenuChoice = min(state.hoveringMenuChoice + 1, (int)registry.menuChoices.components.size() - 1);
		}
	}
}

void IOSystem::handleDialogueChoice(int key, int action, IOState& state, GameState& gameState) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_W) { // highlight choice above
			state.lastHoverDialogueChoice = state.hoveringDialogueChoice;
			state.hoveringDialogueChoice = min(state.hoveringDialogueChoice + 1, (int)registry.dialogueChoices.components.size() - 1);
		}
		else if (key == GLFW_KEY_S) { // highlight choice below
			state.lastHoverDialogueChoice = state.hoveringDialogueChoice;
			state.hoveringDialogueChoice = max(0, state.hoveringDialogueChoice - 1);
			
		}
		//else if (key == GLFW_KEY_SPACE) { // progress through dialogue
		//	state.nextDialogue = true;
		//}
	}
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

void IOSystem::handleMovementInput(int key, int action, IOState& state, GameState& gameState) {
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
        if ((key == GLFW_KEY_SPACE || key == GLFW_MOUSE_BUTTON_1) && !gameState.gamePaused && !gameState.dialogueScene && !gameState.cutScene) {

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

bool IOSystem::isTitle()const {
	return registry.gameStates.components[0].titleScreen;
}

// snce dialogue triggers on keypress for now
// move it here for now
bool IOSystem::isDialogue()const {
	return registry.gameStates.components[0].dialogueScene;
}

// temporary!
bool IOSystem::isCutscene()const {
	return registry.gameStates.components[0].cutScene;
}