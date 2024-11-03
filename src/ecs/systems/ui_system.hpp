#pragma once

#include "common.hpp"
#include "components.hpp"
#include "tiny_ecs.hpp"
#include "tiny_ecs_registry.hpp"

// System responsible for handling user input
class UISystem {
public:
    UISystem();
    ~UISystem();
    // Initialize the window
    bool init(GLFWwindow* window);
    void step(float elapsed_ms);
    void playDialogue();

private:
    //GLFWwindow* window;
  
    Entity pauseMenu;
    Entity gameOverMenu;
    Entity stackUI;
    Entity dialogueBox;
    Entity dialogueAvatar;

    Entity createPauseMenu(vec2 position, vec2 scale);

    Entity createGameOverMenu(vec2 position, vec2 scale);

    Entity createStackUI(WindowState& windowState, StackCompile& stack);

    Entity createDialogueBox(vec2 position, vec2 scale);

    Entity createDialogueAvatar(vec2 position, vec2 scale);
};