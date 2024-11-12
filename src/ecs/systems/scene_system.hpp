#pragma once

#include "common.hpp"
#include "components.hpp"
#include "tiny_ecs.hpp"
#include "tiny_ecs_registry.hpp"

#include <unordered_map>

class SoundSystem;

// System responsible for handling user input
class SceneSystem {
public:

    SceneSystem(SoundSystem *soundSystem);

    ~SceneSystem();

    //bool init(); // load all dialogue
    void step(float elapsed_ms);

private:
    SoundSystem* soundSystem;
    std::unordered_map<Scene, std::vector<Dialogue>> storyDialogue;
    std::unordered_map<InteractibleDialogue, std::vector<Dialogue>> interactibleDialogue;
    Entity currentObject;
    bool isStoryDialogue;

    void summonDialogue();
    void summonInteractibleDialogue(Entity object);
    void loadDialogue(std::string dialogueType);
    void handleStoryChoices();
};