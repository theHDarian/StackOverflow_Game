//
// Created by sky on 2024-11-12.
//

#include "interactable_effects.h"

#include "sound_system.hpp"
#include "tiny_ecs.hpp"
#include "tiny_ecs_registry.hpp"
#include "world_init.hpp"
#include "premades.hpp"

std::vector<std::vector<std::tuple<EnemyType,vec2>>> fightConsolePresets =
	{
		{
			{EnemyType::HifiEnemySniper, {0.6f,0.5f}},
			   {EnemyType::HifiEnemySniper, {0.4f,0.5f}},
			   {EnemyType::HifiEnemySniper, {0.5f,0.5f}},
			   {EnemyType::HifiEnemySniper, {0.5f,0.6f}},
			   {EnemyType::HifiEnemySniper, {0.5f,0.4f}},
		},
	{
			{EnemyType::HifiEnemyCannon, {0.9f,0.2f}},
			{EnemyType::HifiEnemyCannon, {0.9f,0.5f}},
			{EnemyType::HifiEnemyCannon, {0.9f,0.8f}},
			{EnemyType::HifiEnemyCannon, {0.1f,0.2f}},
			{EnemyType::HifiEnemyCannon, {0.1f,0.5f}},
			{EnemyType::HifiEnemyCannon, {0.1f,0.8f}},

	},


	};

void resetStack(Entity player, RenderSystem* renderer) {

    Invincible& inv =registry.invincibles.emplace(player);
    inv.countdown = 1000.0f;

    if (registry.stackCompile.has(player)) {
        StackCompile& reg = registry.stackCompile.get(player);
        if (reg.currStack.size() == 0) {
            return;
        }
        int size = reg.baseStackSize;
        int i = 0;
        for (BulletStackEffect b : reg.currStack) {
            AttackData atkData = AttackData();
			atkData.shape = EnemyBulletShape::RECTANGLE;
            atkData.defaultEffect = b;
            atkData.rareBulletEffects = {b};
            atkData.speed = 200;
            atkData.size = vec2(60,30);
            atkData.bulletRange = 9000;
            atkData.bulletBounce = 3;
            float angle = (2 * M_PI / size) * i;
            createEnemyBullet( renderer, registry.motions.get(player).position + 150.f * vec2(cos(angle), sin(angle)), {cos(angle), sin(angle)}, vec2(0), atkData);
            i++;
        }
        // reg.currStack.clear();
        registry.stackCompile.remove(player);
        StackCompile& newreg = registry.stackCompile.emplace(player);
        newreg.baseStackSize = size;
        Player& pl = registry.players.get(player);
        pl.currDashCharges = pl.baseDashNum;
        pl.currDashCooldown = pl.baseDashCDR;
        StackUI& ui = registry.stackUI.components[0];
        ui.updateStackUISize(reg.baseStackSize);

    }
}

void clearStack(Entity player) {
	StackCompile& reg = registry.stackCompile.get(player);
	if (reg.currStack.size() == 0) {
		return;
	}
	int size = reg.baseStackSize;
	registry.stackCompile.remove(player);
	StackCompile& newreg = registry.stackCompile.emplace(player);
	newreg.baseStackSize = size;
	Player& pl = registry.players.get(player);
	pl.currDashCharges = pl.baseDashNum;
	pl.currDashCooldown = pl.baseDashCDR;
	StackUI& ui = registry.stackUI.components[0];
	ui.updateStackUISize(reg.baseStackSize);
}

void extendStack (Entity player, int extension) {
    if (registry.stackCompile.has(player)) {
        StackCompile& reg = registry.stackCompile.get(player);
        reg.baseStackSize += extension;
        StackUI& ui = registry.stackUI.components[0];
    	ui.updateStackUISize(getModifiedValue( PlayerStackSize, reg.baseStackSize));
    }
}

void closeDoors (SoundSystem* soundPlayer) {
	Map& map = registry.maps.components[0];
	map.currRoom.cleared = false;
	// make all doors locked doors
	for (int i = 0; i < 4; i++) {
		if (registry.interactables.get(registry.doors.entities[i]).name == "LockedDoor" || registry.interactables.get(registry.doors.entities[i]).name == "ClosedDoor") {
			continue;
		}
		if(registry.doors.components[i].room != RoomType::None && !registry.doors.components[i].isPrev && registry.interactables.get(registry.doors.entities[i]).name != "LockedDoor") {
			if (map.currRoom.type != RoomType::TutorialRoom1) {
				soundPlayer->playDoorCloseSound();
			}
			registry.interactables.get(registry.doors.entities[i]).name = "ClosedDoor";
			registry.interactables.get(registry.doors.entities[i]).interactType = InteractableType::ActionInteractable;
		}
	}
	map.currRoom.type = RoomType::EnemyRoom;
	soundPlayer->playNextMusic();
}

void addEffect(Entity player, std::vector<BulletStackEffect> effects) {

    if (registry.stackCompile.has(player)) {
        StackCompile& reg = registry.stackCompile.get(player);
        for (BulletStackEffect b : effects) {
            printf("Adding: %s\n",b.name.c_str());
            reg.add(b);
        	if (b.type == BulletEffectType::PlayerStackSize) {
				StackUI& ui = registry.stackUI.components[0];
				ui.updateStackUISize(getModifiedValue( PlayerStackSize, reg.baseStackSize));
			}
        }
    }
}

void spawnEnemies (SoundSystem* soundPlayer, std::vector<std::tuple<EnemyType,vec2>> enemies) {
	Map& map = registry.maps.components[0];
	closeDoors( soundPlayer);
	map.currRoom.preset.enemies = enemies;
}

void grantWish (Entity player, RenderSystem* renderer, int choice, SoundSystem* soundPlayer) {
	switch (choice) {
		case 0: {
			addEffect(player, {fireRateUpA, dmgUpM, numBulletsUpA });
			Map& map = registry.maps.components[0];
			map.currRoom.preset.enemies ={{EnemyType::EasyEnemySkull, {0.2f, 0.8f}},
	 {EnemyType::EasyEnemySkull, {0.8f, 0.8f}},
	 {EnemyType::EasyEnemySkull, {0.8f, 0.2f}},
	 {EnemyType::EasyEnemySkull, {0.2f, 0.2f}},
{EnemyType::HifiEnemyCharger, {0.25f,0.5f}},
{EnemyType::HifiEnemyCharger, {0.75f,0.5f}},

			};
			registry.invincibles.emplace(player);
			closeDoors( soundPlayer);
			break;
		}
		case 1: {
			//plays a cutscene
			break;
		}
		case 2: {
			extendStack(player, 4);
			addEffect(player, {playerSpeedDownA});
			break;
		}
		case 3: {
			clearStack(player);
			break;
		}
		case 4: {
			break;
		}
	}

}


void interact(float elapsed_ms, Entity player, RenderSystem* renderer, SoundSystem* soundPlayer) {
    // interactible object management placed here and hard coded for now
	// can consider: each behaviour type is component, when choice X is selected then enact that behaviour
	GameState& gameState = registry.gameStates.components[0];
	for (InteractableReaction& reaction : registry.interactableReactions.components) {
		InteractableObject& object = registry.interactables.get(reaction.object);
		if (object.name.compare("PopStack") == 0) { // the choices are known implicitly by person who wrote object script for now
			if (reaction.choice == 0) { // yes
				object.dialogueCount++;
				resetStack(player, renderer);
			}
			else if (reaction.choice == 1) { // no
				// not incrementing allows player to keep asking to pop until pop, but potentially finicky
			}
		}

		if (object.name.compare("BibleTree") == 0) {
			if (reaction.choice == 0) { // yes
				object.dialogueCount++;
				if (!registry.keyItems.has(player)) {
					registry.keyItems.emplace(player);
				}
				KeyItems& keyItems = registry.keyItems.get(player);
				keyItems.fruits++;
			}
			else if (reaction.choice == 1) { // no
				// not incrementing allows player to keep asking to pop until pop, but potentially finicky
			}
		}

		if (object.name.compare("SkipTutorial") == 0) {
			IOState& iostate = registry.ioStates.components[0];
			if (reaction.choice == 0) { // yes
				iostate.tutorialOn = false;
			}
			else if (reaction.choice == 1) { // no
				iostate.tutorialOn = true;
			}
			registry.mapRequests.emplace(player, MapRequestType::NewGame);
		}

		if (object.name.compare("LockedDoor") == 0) {
			assert(registry.doors.has(reaction.object));

			StackCompile& stack = registry.stackCompile.get(player);
			if (reaction.choice == 0) {
				if (stack.useKey()) {
					soundPlayer->playDoorOpenSound();
					object.name = "OpenDoor";
					object.interactType = InteractableType::ActionInteractable;
					reaction.choice = -1;
				} else { // does not have key, but attempted opening
					DialogueRequest& req = registry.dialogueRequests.emplace(reaction.object);
					req.choice = 2; // use this as temporary way to get back to dialogue system
					if (!gameState.seenLockedDoor) {
						req.choice = 3;
						gameState.seenLockedDoor = true;
					}
				}
			}
		}

		if (object.name.compare("OpenDoor") == 0) {
			assert(registry.doors.has(reaction.object));

			if (reaction.choice == 0) {
				registry.mapRequests.emplace(reaction.object, MapRequestType::ChangeRoom, registry.doors.get(reaction.object).room, registry.doors.get(reaction.object).doorIndex);
			}
		}

		if (object.item == InteractableItem::Ram) {
			if (reaction.choice == 0) {
				DialogueRequest& req = registry.dialogueRequests.emplace(reaction.object);
				extendStack( player, 8);
				object.dialogueCount++;
				registry.deleteds.emplace(reaction.object);
			}
		}
		if (object.item == PushConsole) {
			if (reaction.choice == 0) {
				EffectStack& stack = registry.effectStacks.get(reaction.object);
				addEffect(player, stack.stack);
				object.dialogueCount++;
				RenderRequest& req = registry.renderRequests.get(reaction.object);
				req.texture_name = "push_console_pushed.png";
				req.used_effect = EFFECT_ASSET_ID::TEXTURED;
			}
		}

		if (object.item == HoneyCanister) {
			if (reaction.choice == 0) {
				if (!registry.keyItems.has(player)) {
					registry.keyItems.emplace(player);
				}
				KeyItems& keyItems = registry.keyItems.get(player);
				keyItems.honey++;
				RenderRequest& req = registry.renderRequests.get(reaction.object);
				req.texture_name = "HoneyCanisterEmpty.png";

				Map& map = registry.maps.components[0];
				closeDoors(soundPlayer);
				map.currRoom.preset.enemies = {{EnemyType::TwoBee, {0.2f, 0.8f}},
				{EnemyType::ThreeBee, {0.8f, 0.8f}},
					{EnemyType::TwoBee, {0.8f, 0.2f}},
				{EnemyType::ThreeBee, {0.2f, 0.2f}},
			};
			}
		}
		if (object.item == WishGranter) {
			grantWish( player, renderer, reaction.choice, soundPlayer);
			object.dialogueCount++;
			RenderRequest& req = registry.renderRequests.get(reaction.object);
			req.texture_name = "wishGranter_granted.png";
			req.used_effect = EFFECT_ASSET_ID::TEXTURED;
		}
		if (object.item == Baru) {
			int currentDialogue = object.dialogueCount;
			switch (currentDialogue) {
				case 0 : {
					if (reaction.choice == 0) {
						// DialogueRequest& req = registry.dialogueRequests.emplace(reaction.object);
						// req.choice = 1;
						object.dialogueCount++;
					}
					break;
				}
				case 1 : {
					if (reaction.choice == 0) {
						clearStack(player);
						extendStack(player, -8);
						object.dialogueCount++;
					}
					break;
				}
				case 2 : {
					if (reaction.choice == 0) {
						if (reaction.choice == 0) {
							addEffect(player, {WarMachine});
						}
						object.dialogueCount = 4;
					}
					break;
				}
			}
		}
		if (object.item == OracleCrab) {
			if (reaction.choice == 0) {
				// DialogueRequest& req = registry.dialogueRequests.emplace(reaction.object);
				// req.choice = 1;
				object.dialogueCount = Random::Int(5) + 1;
			}
		}
		if (object.item == InteractableItem::FightConsole) {
			if (reaction.choice == 0) {
				EffectStack& stack = registry.effectStacks.get(reaction.object);
				addEffect(player, stack.stack);
				object.dialogueCount++;
				spawnEnemies( soundPlayer, fightConsolePresets[Random::Int(fightConsolePresets.size())]);
			}
		}
	}

	registry.interactableReactions.clear();
}



