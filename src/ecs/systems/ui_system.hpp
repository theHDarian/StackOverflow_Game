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
const float STACK_NOTIF_SCALE = 1.0f;

// System responsible for handling user input
class UISystem {
public:
    UISystem(SoundSystem *soundSystem);

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
    Entity controlsGuide;
    Entity screenCutIn;
    Entity bulletUI;
    Entity bulletUIArrow;
    Entity titleScreen;
    Entity stackAddBubble; // seems like bad practice; what's the best way for multi-part ui?
    Entity stackAddTail;
    Entity dialogueReminder;
    Entity roomClearMessage;

    int lastHoveredBullet = -1;

    std::unordered_map<std::string, std::vector<std::string>> uiTexts;
    Entity fpsCounter;
    Entity roomCounter;

    Entity createRoomClearMessage();

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

    Entity createStackAddNotif(vec2 position, vec2 scale, std::string sprite, vec3 color);

    Entity createDialogueReminder();

    void updateStackAddBubble(vec2 playerPosition, int bulletNum);

    void loadText();

    void loadBulletEffects();

    std::string makeBulletTooltip(BulletStackEffect bullet);

    void updateBulletUI(vec2 position, BulletStackEffect bullet);

    std::string bindScriptVariables(std::string text, std::vector<std::string>& variables);

    Entity createFpsCounter();
    Entity createRoomCounter();
    Entity createButton(std::string label, vec2 position, vec2 scale);

    std::string reportStats();

    SoundSystem* soundSystem;
};