
#define GL3W_IMPLEMENTATION
#include <gl3w.h>

// stlib
#include <chrono>

// internal
#include "physics_system.hpp"
#include "render_system.hpp"
#include "world_system.hpp"
#include "io_system.hpp"
#include "particle_system.hpp"
#include "enemy_system.hpp"
#include "ai_system.hpp"
#include "text_system.hpp"

using Clock = std::chrono::high_resolution_clock;

#if IMGUI_ENABLED
	#include "imgui.h"
	#include "backends/imgui_impl_glfw.h"
	#include "backends/imgui_impl_opengl3.h"
	#include "imguiThemes.h"
#endif

#include <stdlib.h>

// Entry point
int main()
{

	// Global systems
	WorldSystem world;
	RenderSystem renderer;
	PhysicsSystem physics;
	IOSystem ioSystem;
	ParticleSystem particleSystem;
	AISystem aiSystem;
	EnemySystem enemySystem(&renderer);
	TextSystem textSystem;


	// Initializing window
	GLFWwindow* window = world.createWindow();
	if (!window) {
		// Time to read the error message
		printf("Press any key to exit");
		getchar();
		return EXIT_FAILURE;
	}

	// initialize the main systems
	renderer.init(window);
	particleSystem.init(window);
	ioSystem.init(window);
	world.init(&renderer);
	textSystem.initFreetypeLib();

	// variable timestep loop
	auto t = Clock::now();
	while (!world.isOver()) {
		// Processes system messages, if this wasn't present the window would become unresponsive
		glfwPollEvents();

		// Calculating elapsed times in milliseconds from the previous iteration
		auto now = Clock::now();
		float elapsed_ms =
			(float)(std::chrono::duration_cast<std::chrono::microseconds>(now - t)).count() / 1000;
		t = now;
		if (ioSystem.isPaused() || ioSystem.isGameOver() || ioSystem.isDialogue()) {
			world.handleInput();
		} else {
			world.step(elapsed_ms);
			physics.step(elapsed_ms);
			aiSystem.step(elapsed_ms);
			enemySystem.step(elapsed_ms);
			particleSystem.step(elapsed_ms);
			renderer.step(elapsed_ms);
			world.handleCollisions();
			world.clearDeleteQueue();
		}
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
		renderer.drawToScreen(); //postprocessing
		renderer.drawMenuUI();
		textSystem.renderMenuUIText();

		glfwSwapBuffers(window);
	}

	return EXIT_SUCCESS;
}
