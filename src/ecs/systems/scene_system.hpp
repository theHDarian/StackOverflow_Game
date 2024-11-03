#pragma once

#include "common.hpp"
#include "components.hpp"

#include "tiny_ecs.hpp"
#include "tiny_ecs_registry.hpp"

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
    void summonDialogue();
    SoundSystem* soundSystem;
};