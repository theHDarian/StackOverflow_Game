#pragma once

#include "common.hpp"
#include "components.hpp"
#include "tiny_ecs.hpp"
#include "tiny_ecs_registry.hpp"

#include <unordered_map>

class SoundSystem;

// System responsible for handling dialogue and cutscenes
class SceneSystem {
public:

    SceneSystem(SoundSystem *soundSystem);

    ~SceneSystem();

    //bool init(); // load all dialogue
    void step(float elapsed_ms);
    void playCutscene();

    std::unordered_map<std::string, SoundType> soundEffectNames = {
        {"CombatBGM", SoundType::CombatBGM},
        {"BossBGM", SoundType::BossBGM},
        {"ClearedBGM", SoundType::ClearedBGM},
        {"PlayerHurt", SoundType::PlayerHurt},
        {"DashSound", SoundType::DashSound},
        {"PlayerShoot", SoundType::PlayerShoot},
        {"EnemyShoot", SoundType::EnemyShoot},
        {"IncomingDialogue", SoundType::IncomingDialogue},
        {"NormalDialogue", SoundType::NormalDialogue},
        {"DoorOpen", SoundType::DoorOpen},
        {"itemPickup", SoundType::itemPickup},
        {"rareItemPickup", SoundType::rareItemPickup},
        {"explosion", SoundType::explosion},
        { "alarm", SoundType::AlarmSound },
        { "fanfare", SoundType::FanFare },
        { "DoorClose", SoundType::DoorClose },
        { "PlayerZapped", SoundType::PlayerZapped },
        { "EnemyDeathSound", SoundType::EnemyDeathSound },
        { "PlayerDodgeSound", SoundType::PlayerDodgeSound },
        { "LaserSound", SoundType::LaserSound },
        { "DiggingSound", SoundType::DiggingSound }
    };

    std::unordered_map<InteractableItem, int> hasInteracted = {
        {InteractableItem::FightConsole, 0},
        {InteractableItem::PushConsole, 0},
        {InteractableItem::PopConsole, 0},
        {InteractableItem::Ram, 0},
        {InteractableItem::Ramlet, 0},
        {InteractableItem::Optimizer, 0},
        {InteractableItem::Inverter, 0},
            {InteractableItem::GlitchedPopConsole, 0}
    };

    std::unordered_map<std::string, RoomType> roomTypeNames = {
        {"EnemyRoom", RoomType::EnemyRoom},
        {"TreasureRoom", RoomType::TreasureRoom},
        {"RestRoom", RoomType::RestRoom},
        {"EventRoom", RoomType::EventRoom},
        {"Testing", RoomType::Testing},
        {"TutorialRoom", RoomType::TutorialRoom},
        {"TutorialRoom1", RoomType::TutorialRoom1},
        {"TutorialRoom2", RoomType::TutorialRoom2},
        {"EndRoom", RoomType::EndRoom},
        {"BossRoom", RoomType::BossRoom},
    };

private:
    SoundSystem* soundSystem;
    // fun fact: if you don't ask it to hash certain parts, it won't!
    // it'll then grab the nearest hash match based on supplied parameters! Could be useful
    // maybe can do check like "if this field is -1, then that means it doesn't matter to dialogue, so don't bother hashing"
    // (unless it was just a happy accident)
    std::unordered_map<Scene, std::vector<Dialogue>> storyDialogue;
    std::unordered_map<InteractibleDialogue, std::vector<Dialogue>> interactibleDialogue;
    bool isStoryDialogue;
    bool callScientist = false;

    void summonDialogue();
    void summonInteractibleDialogue(Entity object);
    void loadDialogue(std::string dialogueType);
    void handleStoryChoices();
};
