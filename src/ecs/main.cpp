
#define GL3W_IMPLEMENTATION
#include <gl3w.h>

// stlib
#include <chrono>
#include <thread>

// internal
#include "physics_system.hpp"
#include "render_system.hpp"
#include "world_system.hpp"
#include "io_system.hpp"
#include "particle_system.hpp"
#include "enemy_system.hpp"
#include "ai_system.hpp"
#include "map_system.hpp"
#include "text_system.hpp"
#include "ui_system.hpp"
#include "scene_system.hpp"
#include "sound_system.hpp"

using Clock = std::chrono::high_resolution_clock;

#if IMGUI_ENABLED
	#include "imgui.h"
	#include "backends/imgui_impl_glfw.h"
	#include "backends/imgui_impl_opengl3.h"
	#include "imguiThemes.h"
#endif

#include <stdlib.h>

#define TARGET_FPS 120

// Entry point
int main() {
    // Global systems
    WorldSystem world;
    RenderSystem renderer;
    PhysicsSystem physics;
    IOSystem ioSystem;
    ParticleSystem particleSystem;
    AISystem aiSystem;
    SoundSystem soundSystem;
    EnemySystem enemySystem(&renderer, &soundSystem);
    MapSystem mapSystem;
    TextSystem textSystem;
	UISystem uiSystem(&soundSystem);
	SceneSystem sceneSystem(&soundSystem);

    // Initialize window
    GLFWwindow* window = world.createWindow();
    if (!window) {
        // Time to read the error message
        printf("Press any key to exit");
        getchar();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    // Initialize the main systems
    renderer.init(window);
    particleSystem.init(window);
    ioSystem.init(window);
    world.init(&renderer, &soundSystem);
    uiSystem.init(window);
    textSystem.initFreetypeLib();
    mapSystem.init(&renderer, &soundSystem);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// variable timestep loop
	const float frameDuration = (1000.f / (float) TARGET_FPS);
	auto t = Clock::now();
	while (!world.isOver()) {
		// Processes system messages, if this wasn't present the window would become unresponsive
		glfwPollEvents();

		// Calculating elapsed times in milliseconds from the previous iteration
		auto now = Clock::now();
		float elapsed_ms = (float)(std::chrono::duration_cast<std::chrono::microseconds>(now - t)).count() / 1000;
		
		if (elapsed_ms < frameDuration) { //limit FPS
			std::chrono::microseconds duration = std::chrono::microseconds((int)((frameDuration - elapsed_ms) * 1000) );
			std::this_thread::sleep_for(duration);
			continue;
		}

		t = now;
		uiSystem.step(elapsed_ms);
		sceneSystem.step(elapsed_ms); // not sure if this should always be here
		world.handleInput(); // to allow for pausing while cutscene is happening, can be taken out later

		if (ioSystem.isPaused() || ioSystem.isGameOver()) {
			// do nothing
		}
		else if (ioSystem.isCutscene()) { // should be in separate system, but lazy
			renderer.step(elapsed_ms);
			world.playCutscene();
			world.step(elapsed_ms);
		}
		else if (ioSystem.isDialogue()) {
			mapSystem.step(elapsed_ms); // just so the tutorial room can spawn an enemy right away
			uiSystem.playDialogue();
			world.step(elapsed_ms); // this is just where interactable objects are currently reacting in, consider separating later
			renderer.step(elapsed_ms);
		}
		else {
			mapSystem.step(elapsed_ms);
			world.step(elapsed_ms);
			physics.step(elapsed_ms);
			aiSystem.step(elapsed_ms);
			enemySystem.step(elapsed_ms);
			particleSystem.step(elapsed_ms);
			// soundSystem.step(elapsed_ms);
			renderer.step(elapsed_ms);
			world.handleCollisions();
		}
		world.clearDeleteQueue();
		// note: the more complex our drawing is, the more complex the order,
		// and the more appealing z-buffering...
		// strong assumption: each of these entities has a renderRequest
		// OR: make multiple renderRequest type components
		// (probably easier to avoid entities that could span multiple components)
		// (or maybe just check each item manually for now...)
		// Q: what about finer grain order? like where does hover over bullet stack ui go?
		registry.frames.components[0].prevFrameBuffer = 0;
		renderer.drawBackgroundElements();
		particleSystem.render();
		renderer.drawGameElements();
		renderer.drawGameUI();
		textSystem.renderGameUIText();
		renderer.drawDialogueUI();
		textSystem.renderDialogueUIText();
		renderer.drawMenuUI();
		textSystem.renderMenuUIText();
		renderer.drawMenuOverlayUI();
		textSystem.renderMenuOverlayUIText();
		renderer.drawCursor();
		renderer.drawToScreen(); //postprocessing

		glfwSwapBuffers(window);
	}

    // Cleanup
    // glfwDestroyCursor(customCursor);
    return EXIT_SUCCESS;
}

