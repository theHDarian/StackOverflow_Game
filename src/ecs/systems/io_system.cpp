#include "io_system.hpp"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <iostream>

#include "tiny_ecs_registry.hpp"
#include "world_init.hpp"
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
		if (gameState.titleScreen || gameState.gameOver) {
			ioState.shouldEnd = true;
		}
		else {
			ioState.pressedEsc = true;
		}
	}
	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		ioState.shouldEnd = true;
	}
	if  (key == GLFW_KEY_MINUS && action != GLFW_RELEASE && !gameState.gamePaused) {
		gameState.currentVolume = std::max(0.0f, gameState.currentVolume - 0.0125f);
		gameState.previousVolume = gameState.currentVolume;
	}
	if  (key == GLFW_KEY_EQUAL && action != GLFW_RELEASE && !gameState.gamePaused) {
		gameState.currentVolume = std::min(1.0f, gameState.currentVolume + 0.0125f);
		gameState.previousVolume = gameState.currentVolume;
	}
	if (key == GLFW_KEY_COMMA && action != GLFW_RELEASE && !gameState.gamePaused) {
		gameState.currentSfxVolume = std::max(0.0f, gameState.currentSfxVolume - 0.0125f);
		gameState.previousSfxVolume = gameState.currentSfxVolume;
	}
	if (key == GLFW_KEY_PERIOD && action != GLFW_RELEASE && !gameState.gamePaused) {
		gameState.currentSfxVolume = std::min(1.0f, gameState.currentSfxVolume + 0.0125f);
		gameState.previousSfxVolume = gameState.currentSfxVolume;
	}

	//toggle fullscreen
	if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
		ToggleWindowMode();
	}

	// Resetting game
	if (action == GLFW_RELEASE && key == GLFW_KEY_R && !gameState.titleScreen) {
		int w, h;
		glfwGetWindowSize(window, &w, &h);

        ioState.shouldRestart = true;
		gameState.titleScreen = true;
	}

	// Debug toggle for colliders
	if (key == GLFW_KEY_V && action == GLFW_RELEASE) {
		ioState.debugMode = !ioState.debugMode;
	}

	// turn FPS counter on/off
	if (key == GLFW_KEY_F && action == GLFW_RELEASE) {
		ioState.showFPS = !ioState.showFPS;
	}

	// other options for progressing dialogue: Enter, space
	if (action == GLFW_RELEASE && (key == GLFW_KEY_ENTER || key == GLFW_KEY_SPACE) && (gameState.dialogueScene || ioState.activeMenu > -1)) {
		if (gameState.dialogueScene && !registry.drawingTexts.get(registry.dialogueUIs.entities[0]).doneDrawing) {
			registry.drawingTexts.get(registry.dialogueUIs.entities[0]).doneDrawing = true;
		}
		else {
			ioState.nextDialogue = true;
		}
		if (ioState.activeMenu > -1)
			ioState.confirmedOption = true;
	}

	if (action == GLFW_RELEASE && key == GLFW_KEY_E && ioState.activeMenu > -1) {
		ioState.confirmedOption = true;
	}

	// call the scientist
	if (action == GLFW_RELEASE && key == GLFW_KEY_C && !gameState.gamePaused && !gameState.cutScene && !gameState.gameOver && !gameState.dialogueScene && !gameState.titleScreen) {
		if (!registry.dialogueRequests.has(registry.players.entities[0])) {
			DialogueRequest& req = registry.dialogueRequests.emplace(registry.players.entities[0]);
			req.type = DialogueRequestType::CallDialogue;
		}
	}

	// interacted with object/play story dialogue
	if (action == GLFW_RELEASE && key == GLFW_KEY_E && !gameState.gamePaused && !gameState.cutScene) {
		if (gameState.dialogueScene && !registry.drawingTexts.get(registry.dialogueUIs.entities[0]).doneDrawing) {
			registry.drawingTexts.get(registry.dialogueUIs.entities[0]).doneDrawing = true;
		}
		else {
			ioState.nextDialogue = true;
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
			// remove for now

			//if (!registry.dialogueRequests.has(registry.players.entities[0])) {
			//	DialogueRequest& req = registry.dialogueRequests.emplace(registry.players.entities[0]);
			//	req.type = DialogueRequestType::StoryDialogue;
			//}
		}
	}
	
	if (ioState.activeMenu > -1) {
		// only for title for now, think of where else menus might appear
		// menu state? or just pause?
		handleMenuChoice(key, action, ioState, gameState);
	}
	else if (gameState.dialogueScene && !gameState.gamePaused) {
		handleDialogueChoice(key, action, ioState, gameState);
	}
	if (!ioState.lockControls && !gameState.dialogueScene)
		handleMovementInput(key, action, ioState, gameState); // seems like always need to handle this, or else weird movement bugs

}

void IOSystem::handleMenuChoice(int key, int action, IOState& state, GameState& gameState) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_W || key == GLFW_KEY_UP) { // highlight choice above
			state.lastHoverMenuChoice = state.hoveringMenuChoice;
			state.hoveringMenuChoice = max(0, state.hoveringMenuChoice - 1);
		}
		else if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) { // highlight choice below
			state.lastHoverMenuChoice = state.hoveringMenuChoice;
			
			state.hoveringMenuChoice = min(state.hoveringMenuChoice + 1, (int)registry.menuChoices.components.size() - 1);
		}
	}
}

void IOSystem::handleDialogueChoice(int key, int action, IOState& state, GameState& gameState) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_W || key == GLFW_KEY_UP) { // highlight choice above
			state.lastHoverDialogueChoice = state.hoveringDialogueChoice;
			state.hoveringDialogueChoice = max(0, state.hoveringDialogueChoice - 1);
		}
		else if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) { // highlight choice below
			state.lastHoverDialogueChoice = state.hoveringDialogueChoice;
			state.hoveringDialogueChoice = min(state.hoveringDialogueChoice + 1, (int)registry.dialogueChoices.components.size() - 1);
		}
	}

	// this is for when dialogue choices are drawn bottom up
	//if (action == GLFW_PRESS) {
	//	if (key == GLFW_KEY_W || key == GLFW_KEY_UP) { // highlight choice above
	//		state.lastHoverDialogueChoice = state.hoveringDialogueChoice;
	//		state.hoveringDialogueChoice = min(state.hoveringDialogueChoice + 1, (int)registry.dialogueChoices.components.size() - 1);
	//	}
	//	else if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) { // highlight choice below
	//		state.lastHoverDialogueChoice = state.hoveringDialogueChoice;
	//		state.hoveringDialogueChoice = max(0, state.hoveringDialogueChoice - 1);
	//		
	//	}
	//}
}

void IOSystem::mouseClick(int button, int action, int mods) {
	#if IMGUI_ENABLED
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
	#endif

	IOState& state = registry.ioStates.components[0];
	GameState& gameState = registry.gameStates.components[0];
	if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS && !state.lockControls) {
		state.shouldDash = true;
	}

	// have it check when in menu in the future
	if (button == GLFW_MOUSE_BUTTON_1) {
		if (state.activeMenu > -1 && action == GLFW_PRESS) {
			state.clickedButton = true;
		}
		else if (!state.lockControls) {
			state.shouldShoot = (action == GLFW_PRESS || action == GLFW_REPEAT);
		}
	}

}

void IOSystem::onMouseMove(vec2 mousePosition) {	
	#if IMGUI_ENABLED
	ImGui_ImplGlfw_CursorPosCallback(window, mousePosition.x, mousePosition.y);
	#endif
    IOState& state = registry.ioStates.components[0];
	state.mousePosition = mousePosition;
	/*glfwSetCursorPos(window, 0, 0);*/
	//std::cout << mousePosition.x << ", " << mousePosition.y << std::endl;
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
        if ((key == GLFW_KEY_SPACE || key == GLFW_MOUSE_BUTTON_1) && !gameState.gamePaused && !gameState.cutScene) {
			if (state.pressedHorizontal.empty() && state.pressedVertical.empty()) {
				state.lastInputAxis = { 1, 1 };
			}
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

void AdjustViewport(GLFWwindow* window, int winWidth, int winHeight) {

	float aspectRatio = 16.0f / 9.0f;
	int viewWidth, viewHeight;

	if (winWidth / (float)winHeight > aspectRatio) {
		viewHeight = winHeight;
		viewWidth = (int)(winHeight * aspectRatio);
	} else {
		viewWidth = winWidth;
		viewHeight = (int)(winWidth / aspectRatio);
	}

	int xOffset = (winWidth - viewWidth) / 2;
	int yOffset = (winHeight - viewHeight) / 2;

	glViewport(xOffset, yOffset, viewWidth, viewHeight);
}


void IOSystem::ToggleWindowMode() {
	auto monitor = glfwGetPrimaryMonitor();
	auto vidMode = glfwGetVideoMode(monitor);
	WindowState& windowState = registry.windowStates.components[0];
	IOState& ioState = registry.ioStates.components[0];

	if (!ioState.isFullscreen) {
		// Switch to fullscreen mode
		std:: cout << "Fullscreen: " << vidMode->width << ", " << vidMode->height << std::endl;
		glfwSetWindowMonitor(window, monitor, 0,0 , windowState.width  , windowState.height, vidMode->refreshRate);
		ioState.isFullscreen = true;
		ioState.isBorderless = false;
	} else {
		// Switch to windowed mode
		glfwSetWindowMonitor(window, nullptr, 100, 100,  windowState.width, windowState.height, 0);
		ioState.isFullscreen = false;
		ioState.isBorderless = false;
	}
	AdjustViewport(window, windowState.width, windowState.height);
	glfwSetWindowAspectRatio(window,windowState.width,windowState.height);
}


