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
    // TO DO: add internal state of game to dialogue map
    // instead of a bunch of if statements and hard coding
    SoundSystem* soundSystem;
    std::unordered_map<Scene, std::vector<Dialogue>> storyDialogue;
    std::unordered_map<InteractibleDialogue, std::vector<Dialogue>> interactableDialogue;

    void summonDialogue();
    void playerInputDialogue();
    void loadStoryDialogue();
    void loadInteractableDialogue();
};