#pragma once

#include "common.hpp"
#include "components.hpp"
#include "tiny_ecs.hpp"
#include "tiny_ecs_registry.hpp"
#include "components/presets/enemy_bullet_properties.hpp"

#include <unordered_map>
#include <string>
#include <sstream>

class SoundSystem;

// System responsible for handling user input
class UISystem {
public:
    UISystem(SoundSystem *soundSystem);

    ~UISystem();
    // Initialize the window
    bool init(GLFWwindow* window);

    bool resetStackUI();

    void step(float elapsed_ms);
    void playDialogue();

private:
    //GLFWwindow* window;
  
    Entity pauseMenu;
    Entity gameOverMenu;
    Entity stackUI;
    Entity dialogueBox;
    Entity dialogueAvatar;
    Entity controlsGuide;
    Entity screenCutIn;
    Entity bulletUI;
    Entity bulletUIArrow;
    Entity titleScreen;
    Entity stackAddBubble; // seems like bad practice; what's the best way for multi-part ui?
    Entity stackAddTail;

    int lastHoveredBullet = -1;

    std::unordered_map<std::string, std::vector<std::string>> uiTexts;
    Entity fpsCounter;
    Entity roomCounter;

    Entity createPauseMenu(vec2 position, vec2 scale);

    Entity createGameOverMenu(vec2 position, vec2 scale);

    Entity createStackUI(WindowState& windowState, StackCompile& stack);

    Entity createDialogueBox(vec2 position, vec2 scale);

    Entity createDialogueAvatar(vec2 position, vec2 scale);

    Entity createControlsGuide(vec2 position, vec2 scale);

    Entity createDialogueChoice(std::string choice, vec2 position);
    
    Entity createScreenCutIn();

    Entity createInteractIndicator(vec2 position);

    Entity createTitleScreen();

    Entity createMenuChoice(std::string choice, vec2 position);

    // assume will only ever have one
    Entity createBulletUI();

    Entity createBulletUIArrow();

    Entity createStackAddBubble();

    Entity createStackAddTail();

    Entity UISystem::createStackAddNotif(vec2 position, BulletStackEffect bullet);

    void updateStackAddBubble(vec2 playerPosition, int bulletNum);

    void loadText();

    void loadBulletEffects();

    std::string makeBulletTooltip(BulletStackEffect bullet);

    void updateBulletUI(vec2 position, BulletStackEffect bullet);

    Entity createFpsCounter();
    Entity createRoomCounter();

    SoundSystem* soundSystem;
};