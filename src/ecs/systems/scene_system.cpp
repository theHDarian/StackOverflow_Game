#include "scene_system.hpp"

#include "sound_system.hpp"

SceneSystem::SceneSystem( SoundSystem* soundSystem) {
	this->soundSystem = soundSystem;

}

SceneSystem::~SceneSystem() {

}

void SceneSystem::summonDialogue() {
	IOState& iostate = registry.ioStates.components[0];
	GameState& gameState = registry.gameStates.components[0];
	Map& map = registry.maps.components[0];
	iostate.nextDialogue = true;
	gameState.dialogueScene = true;
	map.currRoom.dialogueCount++;
	map.currRoom.dialogueDone = false;
}

void SceneSystem::step(float elapsed_ms) {
	Map& map = registry.maps.components[0];
	
	if (map.currRoom.cutSceneDone) {
		if (map.currRoom.type == RoomType::TutorialRoom1 && map.currRoom.cutsceneCount == 0) {
			// beginning animation sequence - mc wakes up
			Entity player = registry.players.entities[0];
			AnimationSequence& as = registry.animationSequences.emplace(player);
			as.nextEffect = EFFECT_ASSET_ID::TEXTURED;
			as.nextSprite = "mcv1_base.png";
			RenderRequest& rr = registry.renderRequests.get(player);
			rr.used_effect = EFFECT_ASSET_ID::ANIMATE;
			rr.texture_name = "mc_startup";
			registry.animations.get(player).max_frames = 40; // hard code for now

			// play cutscene
			GameState& gameState = registry.gameStates.components[0];
			gameState.cutScene = true;
			map.currRoom.cutSceneDone = false;
		}
	}

	if (map.currRoom.dialogueDone) {
		if (map.currRoom.type == RoomType::TutorialRoom1) {
			if (!map.currRoom.cleared) {
				if (map.currRoom.cutsceneCount == 1 && map.currRoom.dialogueCount == 0) {
					DialogueLines& lines = registry.dialogueLines.components[0];
					lines = DialogueLines();
					lines.lines.push_back(Dialogue{ "[Incoming Message...]\n[Press [E] to accept.]", "N", "N" });
					lines.lines.back().sfx = SoundType::IncomingDialogue;
					lines.lines.push_back(Dialogue{ "Hello? Can you hear me?", "Scientist", "scientist_avatar.png" });
					lines.lines.push_back(Dialogue{ "Oh, great! I was worried I'd be stuck in this lab all alone!", "Scientist", "scientist_avatar.png" });
					lines.lines.push_back(Dialogue{ "It's been ages since I've found someone who wouldn't outright attack me here, and there's so much I want to say, but before that--", "Scientist", "scientist_avatar.png" });
					lines.lines.push_back(Dialogue{ "--you must've just come online not too long ago, so I'll let you get your bearings and look around first.", "Scientist", "scientist_avatar.png" });
					lines.lines.push_back(Dialogue{ "Let me know once you're done.", "Scientist", "scientist_avatar.png" });
					lines.lines.push_back(Dialogue{ "[Try moving around the room using the [WASD] keys, or dash around using [SPACE]/[Right Mouse Button].\nCall the scientist again by pressing [E] when you're done looking around.]", "N", "N" });
					summonDialogue();
				}
				else if (map.currRoom.dialogueCount == 1) {
					DialogueLines& lines = registry.dialogueLines.components[0];
					lines = DialogueLines();
					lines.lines.push_back(Dialogue{ "Done looking around?", "Scientist", "scientist_avatar.png" });
					lines.lines.back().sfx = SoundType::IncomingDialogue;
					lines.lines.push_back(Dialogue{ "Oh, was the door locked?", "Scientist", "scientist_avatar.png" });
					lines.lines.push_back(Dialogue{ "Well, doesn't look like anyone's guarding it, so I think I can just...", "Scientist", "scientist_avatar.png" });
					lines.lines.push_back(Dialogue{ "Aha, there! It should be unlocked now.", "Scientist", "scientist_avatar.png" });
					lines.lines.back().sfx = SoundType::DoorOpen;
					lines.lines.push_back(Dialogue{ "Ah, about all the things I wanted to say--I think I got too excited got a bit ahead of myself there, sorry about that.", "Scientist", "scientist_avatar.png" });
					lines.lines.push_back(Dialogue{ "But, there is one thing I do want to tell you.", "Scientist", "scientist_avatar.png" });
					lines.lines.push_back(Dialogue{ "This place isn't worth staying in. It's overrun by dangerous robots, and everyone who was once here has either long since left, or...\nyeah, they've all left.", "Scientist", "scientist_avatar.png" });
					lines.lines.push_back(Dialogue{ "So, you should leave too. I'll even help you and make sure of that.", "Scientist", "scientist_avatar.png" });
					lines.lines.push_back(Dialogue{ "And what about me? \nHeh, don't worry about me. I've already been stuck here long enough, so this is the least I could do.", "Scientist", "scientist_avatar.png" });
					lines.lines.push_back(Dialogue{ "Anyway, the only way out is forwards! Head on down to the door on the bottom once you're ready.", "Scientist", "scientist_avatar.png" });
					// this relies on the player to press E, so don't use summonDialogue() here
					map.currRoom.dialogueCount++;
					map.currRoom.dialogueDone = false;
				}
				else if (map.currRoom.dialogueCount == 2) {
					map.currRoom.cleared = true;
				}
			}
		}

		if (map.currRoom.type == RoomType::TutorialRoom2) {
			if (map.currRoom.dialogueCount == 0) {
				DialogueLines& lines = registry.dialogueLines.components[0];
				lines = DialogueLines();
				lines.lines.push_back(Dialogue{ "Ah, there it is, one of those dangerous robots...", "Scientist", "scientist_avatar.png" });
				lines.lines.push_back(Dialogue{ "With that thing there, I don't think I'll be able to open the door so easily for you this time.\nLooks like you've got no choice but to fight it.", "Scientist", "scientist_avatar.png" });
				lines.lines.push_back(Dialogue{ "No need to worry though, I'm sure you're equipped with the means to deal with it, right?", "Scientist", "scientist_avatar.png" });
				lines.lines.push_back(Dialogue{ "[Aim and shoot with the [Left Mouse Button]!]", "N", "N" });
				summonDialogue();
			}

			if (map.currRoom.cleared && map.currRoom.dialogueCount == 1) {
				DialogueLines& lines = registry.dialogueLines.components[0];
				lines = DialogueLines(); // reset instead of properly making new dialogue req to be processed
				lines.lines.push_back(Dialogue{ "Whew, I was a bit worried there, but you did it!" , "Scientist", "scientist_avatar.png" });
				lines.lines.push_back(Dialogue{ "Now that no one's guarding the door anymore, you should be able to walk through it." , "Scientist", "scientist_avatar.png" });
				lines.lines.push_back(Dialogue{ "Well, since you handled all that just fine, looks like you don't need to much of my help after all." , "Scientist", "scientist_avatar.png" });
				lines.lines.push_back(Dialogue{ "Ah, before I go though, there's one last thing I should tell you." , "Scientist", "scientist_avatar.png" });
				lines.lines.push_back(Dialogue{ "You seem to have a special stack container that can encode and store anything you come in contact with into units of pure data." , "Scientist", "scientist_avatar.png" }); // maybe better to demonstrate with inert bullet in earlier room
				lines.lines.push_back(Dialogue{ "But it's a stack after all, so I'm sure it'll overflow if you have too many--and that can't be a good thing." , "Scientist", "scientist_avatar.png" });
				lines.lines.push_back(Dialogue{ "I sure wonder how you'll make use of this stack... Perhaps you'll use it to modify your abilities?" , "Scientist", "scientist_avatar.png" });
				lines.lines.push_back(Dialogue{ "Anyway, to get to the lab exit, I suggest you make your way to the Biology Wing. There should be less rogue robot activity there." , "Scientist", "scientist_avatar.png" });
				lines.lines.push_back(Dialogue{ "I'll contact you again when you get there. I wish you luck, little robot." , "Scientist", "scientist_avatar.png" });
				lines.lines.push_back(Dialogue{ "[Press [P] to pause and hover over your stach to take a closer look at it!]" , "N", "N" });
				summonDialogue();
			}
		}
	}


}