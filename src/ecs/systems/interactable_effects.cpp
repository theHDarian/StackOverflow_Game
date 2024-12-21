//
// Created by sky on 2024-11-12.
//

#include "interactable_effects.h"

#include "map_system.hpp"
#include "sound_system.hpp"
#include "tiny_ecs.hpp"
#include "tiny_ecs_registry.hpp"
#include "world_init.hpp"
#include "premades.hpp"

std::vector<std::vector<std::tuple<EnemyType,vec2>>> fightConsolePresetsPhysics =
	{
		{
			{EnemyType::EnemySniper, {0.6f,0.5f}},
			   {EnemyType::EnemySniper, {0.2f,0.5f}},
			   {EnemyType::EnemySniper, {0.8f,0.5f}},
			   {EnemyType::EnemySniper, {0.8f,0.6f}},
			   {EnemyType::EnemySniper, {0.2f,0.4f}},
		},
	{
			{EnemyType::EnemyHifiCannon, {0.9f,0.2f}},
			{EnemyType::EnemyHealer, {0.9f,0.5f}},
			{EnemyType::EnemyHifiCannonHard, {0.1f,0.5f}},
			{EnemyType::EnemyHealer, {0.1f,0.8f}},
	},
	{
		{EnemyHifiChargerHard, {0.8f,0.2f}},
		{EnemyHifiChargerHard, {0.8f,0.8f}},
		{EnemyHifiChargerHard, {0.2f,0.2f}},
		{EnemyHifiChargerHard, {0.2f,0.8f}},
			{EnemyType::EnemyHifiCannon, {0.9f,0.2f}},

	},
	{
		{ EnemyType::EnemyLaserSniper, {0.7f,0.3f}},
		{ EnemyType::EnemyHifiTrail, {0.7f,0.7f}},
		{ EnemyType::EnemyLaserSniper, {0.3f,0.3f}},
		{ EnemyType::EnemyHifiTrail, {0.3f,0.7f}},

	},

		{
			{ EnemyType::EnemyHifiTwinLaserHorizontal1, {0.7f,0.1f}},
			{ EnemyType::EnemyTwinLaserVertical1, {0.1f,0.7f}},
			{ EnemyType::EnemyHifiCannon, {0.6f,0.6f}},
			{ EnemyType::EnemyHifiCannon, {0.4f,0.4f}}
		},
	{
			{ EnemyType::EnemyHifiCannonHard, {0.6f,0.6f}},
				{ EnemyType::EnemyHifiCannonHard, {0.4f,0.4f}},
		{EnemyType::EnemyHealer, {0.9f,0.5f}},
		{EnemyType::EnemyHifiTrail, {0.1f,0.5f}},
	},


	};

std::vector<std::vector<std::tuple<EnemyType,vec2>>> fightConsolePresetsBio =
	{
		{
			{EnemyType::EnemyEvilSnail, {0.5f, 0.5f}},
		{EnemySnail, {0.2f, 0.8f}},
		{EnemyType::EnemyLaserCrab, {0.8f, 0.2f}},
		{EnemyMagnet, {0.2f, 0.4f}},
		},
	{
	{EnemyType::EnemyCrab, {0.8f, 0.8f}},
		 {EnemyType::EnemyLaserCrab, {0.8f, 0.2f}},
		 {EnemyType::EnemyCrab, {0.2f, 0.2f}},
		 {EnemyType::EnemyEvilCrab, {0.3f, 0.7f}},
	},
{
	{EnemyType::EnemyTwoBee, {0.8f, 0.2f}},
 {EnemyType::EnemyTwoBee, {0.2f, 0.2f}},
 {EnemyType::EnemyBeeHive, {0.2f, 0.4f}},
 {EnemyType::EnemyBeeHive, {0.8f, 0.4f}}
},

{{EnemyType::EnemyTwoBee, {0.2f, 0.8f}},
 {EnemyType::EnemyThreeBee, {0.8f, 0.8f}},
 {EnemyType::EnemyTwoBee, {0.8f, 0.2f}},
 {EnemyType::EnemyOneBee, {0.2f, 0.2f}},

},

	};

void resetStack(Entity player, RenderSystem* renderer) {

    //Invincible& inv =registry.invincibles.emplace(player);
    //inv.countdown = 1000.0f;

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
			registry.interactables.get(registry.doors.entities[i]).item = ClosedDoor;
			registry.interactables.get(registry.doors.entities[i]).interactType = InteractableType::DialogueInteractable;
		}
	}
	map.currRoom.type = RoomType::EnemyRoom;
	soundPlayer->playNextMusic();
}

void addEffect(Entity player, std::vector<BulletStackEffect> effects, SoundSystem* soundPlayer) {

    if (registry.stackCompile.has(player)) {
        StackCompile& reg = registry.stackCompile.get(player);
        for (BulletStackEffect b : effects) {
            printf("Adding: %s\n",b.name.c_str());
            bool success = reg.add(b);
        	if (!success) {
        		std::cout << "Stack Overflowed!" << std::endl;
        		GameState& gameState = registry.gameStates.components[0];
        		gameState.gameOver = true;
        		gameState.currentVolume *= 0.15f;
        		soundPlayer->playGameOverSound();
        		soundPlayer->setMusicVolume(gameState.currentVolume);
        		if (!registry.uiRequests.has(player)) {
        			registry.uiRequests.insert(player, { UIRequestType::GameOverReport });
        		}
        	}
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
	map.currRoom.preset.enemies.push_back( enemies);
}

void spawnEnemies (SoundSystem* soundPlayer,std::vector<std::vector<std::tuple<EnemyType,vec2>>>enemies) {
	Map& map = registry.maps.components[0];
	closeDoors( soundPlayer);
	map.currRoom.preset.enemies.insert(map.currRoom.preset.enemies.end(), enemies.begin(), enemies.end());
}

void grantWish (Entity player, RenderSystem* renderer, int choice, SoundSystem* soundPlayer) {
	switch (choice) {
		case 0: {
			addEffect(player, {fireRateUpA, dmgUpM, numBulletsUpA }, soundPlayer);
			spawnEnemies( soundPlayer, {
			{EnemyType::EnemyHifiCharger, {0.25f,0.5f}},
			{EnemyType::EnemyHifiCharger, {0.75f,0.5f}},
			{EnemyType::EnemyHifiCannon, {0.9f,0.2f}},
			{EnemyType::EnemyHealer, {0.9f,0.5f}},
				{EnemyLaserSniper, {0.7f,0.3f}},
				{EnemyHifiTrail, {0.7f,0.7f}},
				{EnemyLaserSniper, {0.3f,0.3f}},
			});
			registry.invincibles.emplace(player);
			break;
		}
		case 1: {
			//plays a cutscene
			break;
		}
		case 2: {
			extendStack(player, 4);
			addEffect(player, {playerSpeedDownA}, soundPlayer);
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
		MapRegion region = Tutorial;
		RoomType room = TutorialRoom;
		if (object.name.compare("SkipTutorial") == 0) {
			IOState& iostate = registry.ioStates.components[0];
			if (reaction.choice == 0) { // do tutorial
				iostate.tutorialOn = true;
			}
			else{
				iostate.tutorialOn = false;
				if (reaction.choice == 1) { // bio region
					region = Biology;
				}
				else if (reaction.choice == 2) {
					region = Physics;
				}
				else if (reaction.choice == 3) {
					region = Biology;
					room = BossRoom;
				}
				else if (reaction.choice == 4) {
					region = Physics;
					room = BossRoom;
				}
			}
			MapRequest& req = registry.mapRequests.emplace(player, NewGame);
			req.type = room;
			req.region = region;
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
			// doesn't actually do anything, but leave here for now

			//if (reaction.choice == 0) {
			//	registry.mapRequests.emplace(reaction.object, MapRequestType::ChangeRoom, registry.doors.get(reaction.object).room, registry.doors.get(reaction.object).doorIndex);
			//}
		}

		if (object.item == PopConsole) { // the choices are known implicitly by person who wrote object script for now
			if (reaction.choice == 0) { // yes
				object.dialogueCount++;
				resetStack(player, renderer);
			}
			else if (reaction.choice == 1) { // no
				// not incrementing allows player to keep asking to pop until pop, but potentially finicky
			}
		}

		if (object.item == BibleTree) {
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
				addEffect(player, stack.stack, soundPlayer);
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
				object.dialogueCount++;

				Map& map = registry.maps.components[0];
				closeDoors(soundPlayer);
				map.currRoom.preset.enemies.push_back( {{EnemyType::EnemyTwoBee, {0.2f, 0.8f}},
				{EnemyType::EnemyThreeBee, {0.8f, 0.8f}},
					{EnemyType::EnemyTwoBee, {0.8f, 0.2f}},
				{EnemyType::EnemyThreeBee, {0.2f, 0.2f}},
			});
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
							addEffect(player, {WeaponOfWar}, soundPlayer);
						}
						object.dialogueCount = 4;
					}
					break;
				}
			}
		}
		if (object.item == OracleCrab) {
			switch (object.dialogueCount) {
				case 0 : {
					if (reaction.choice == 0) {
						object.dialogueCount++;
					}
					break;
				}
				case 1 : {
					if (reaction.choice == 0) {
						// auto atkdata = AttackData();
						// atkdata.defaultEffect = lightning2;
						// createEnemyBullet(renderer, registry.motions.get(player).position, {0,0}, vec2(0), atkdata);
						auto& spriteMap = registry.sprites.get(player).sprites;
						registry.renderRequests.get(player).texture_name = spriteMap[SPRITE_STATE::DAMAGED];
						registry.renderRequests.get(player).used_effect = EFFECT_ASSET_ID::TEXTURED;
						if (!registry.spriteTimers.has(player)) {
							auto& spriteTimer = registry.spriteTimers.emplace(player);
							spriteTimer.count_ms = 250;
							spriteTimer.nextSprite = spriteMap[SPRITE_STATE::BASE];
							spriteTimer.nextEffect = EFFECT_ASSET_ID::TEXTURED;
						}
						// can consider putting this, but doesn't tick down in dialogue so looks a bit goofy (?)
						//if (!registry.invincibles.has(player)) {
						//	registry.invincibles.emplace(player);
						//}
						addEffect( player, {lightning2}, soundPlayer);
						soundPlayer->playPlayerZappedSound();
						registry.uiRequests.insert(player, {UIRequestType::StackNotifReqShuffle});
						object.dialogueCount++;
					}
					break;
				}
				case 2 : {
					if (reaction.choice == 0) {
						// auto atkdata = AttackData();
						// atkdata.defaultEffect = lightning2;
						// createEnemyBullet(renderer, registry.motions.get(player).position, {0,0}, vec2(0), atkdata);
						addEffect( player, {lightning2}, soundPlayer);
						soundPlayer->playPlayerZappedSound();
						auto& spriteMap = registry.sprites.get(player).sprites;
						registry.renderRequests.get(player).texture_name = spriteMap[SPRITE_STATE::DAMAGED];
						registry.renderRequests.get(player).used_effect = EFFECT_ASSET_ID::TEXTURED;
						if (!registry.spriteTimers.has(player)) {
							auto& spriteTimer = registry.spriteTimers.emplace(player);
							spriteTimer.count_ms = 250;
							spriteTimer.nextSprite = spriteMap[SPRITE_STATE::BASE];
							spriteTimer.nextEffect = EFFECT_ASSET_ID::TEXTURED;
						}
						registry.uiRequests.insert(player, {UIRequestType::StackNotifReqShuffle});
					} else if (reaction.choice == 1) {
						// auto atkdata = AttackData();
						// atkdata.defaultEffect = lightning1;
						// createEnemyBullet(renderer, registry.motions.get(player).position, {0,0}, vec2(0), atkdata);
						addEffect( player, {lightning1}, soundPlayer);
						soundPlayer->playPlayerZappedSound();
						auto& spriteMap = registry.sprites.get(player).sprites;
						registry.renderRequests.get(player).texture_name = spriteMap[SPRITE_STATE::DAMAGED];
						registry.renderRequests.get(player).used_effect = EFFECT_ASSET_ID::TEXTURED;
						if (!registry.spriteTimers.has(player)) {
							auto& spriteTimer = registry.spriteTimers.emplace(player);
							spriteTimer.count_ms = 250;
							spriteTimer.nextSprite = spriteMap[SPRITE_STATE::BASE];
							spriteTimer.nextEffect = EFFECT_ASSET_ID::TEXTURED;
						}
						registry.uiRequests.insert(player, {UIRequestType::StackNotifReqShift});
					}
					break;
				}

			}
		}
		if (object.item == InteractableItem::FightConsole) {
			if (reaction.choice == 0) {
				EffectStack& stack = registry.effectStacks.get(reaction.object);
				addEffect(player, stack.stack, soundPlayer);
				registry.invincibles.emplace(player);
				object.dialogueCount++;
				MapSystem::clearRoomInteractables();
				Map &map = registry.maps.components[0];
				switch (map.currRegion) {
					case Physics: {
						spawnEnemies( soundPlayer, Random::ListItem( fightConsolePresetsPhysics));
						break;
					}
					case Biology: {
						spawnEnemies( soundPlayer, Random::ListItem( fightConsolePresetsBio));
						break;
					}
				}
			}
		}
		if (object.item == InteractableItem::Swarm) {
			switch (object.dialogueCount) {
				case 0 : {
					object.dialogueCount = 1;
					break;
				}
				case 1 : {
					if (reaction.choice == 0) {
						addEffect(player, {theCurse, Freedom}, soundPlayer);
						object.dialogueCount++;
					}
					break;
				}

			}
		}
		if (object.item == InteractableItem::Gardener) {
			switch (object.dialogueCount) {
				case 0 : {
					object.dialogueCount++;
					break;
				}
				case 1 : {
					object.dialogueCount++;
					break;
				}
				case 2 : {
					if (reaction.choice == 0) {
						if (!registry.keyItems.has(player)) {
							registry.keyItems.emplace(player);
						}
						KeyItems& keyItems = registry.keyItems.get(player);
						keyItems.hasSeenPieRecipe = true;
						object.dialogueCount++;
					}
					break;
				}

			}
		}

		if (object.item == InteractableItem::Phone) {
			if (reaction.choice == 0) {
				registry.renderRequests.get(reaction.object).show = false;
				registry.deleteds.emplace(reaction.object);
				DialogueRequest& req = registry.dialogueRequests.emplace(registry.players.entities[0]); // note: MUST be player because chip gets deleted after being picked up
				req.type = DialogueRequestType::StoryDialogue;
			}
		}

		if ( object.item == InteractableItem::Mouse) {
			switch (object.dialogueCount) {
				case  0 : {
					if (reaction.choice == 0) {

						DialogueRequest& req = registry.dialogueRequests.emplace(reaction.object);
						// registry.renderRequests.get(reaction.object).show = false;
						// registry.deleteds.emplace(reaction.object);
						StackCompile& stack = registry.stackCompile.get(player);
						if (stack.useKey()) {
							req.choice = 7;
							object.dialogueCount = 1;
							if (!registry.keyItems.has(player)) {
								registry.keyItems.emplace(player);
							}
							KeyItems& keyItems = registry.keyItems.get(player);
							keyItems.cheese++;
						} else {
							req.choice = 3;
						}
					}
					break;
				}
				case 1 : {
					if (reaction.choice == 0) {
						if (!registry.keyItems.has(player)) {
							registry.keyItems.emplace(player);
						}
						KeyItems& keyItems = registry.keyItems.get(player);
						if (keyItems.cheese > 0) {
							keyItems.cheese--;
						}
						object.dialogueCount = 2;
					}
					break;
				}
			}
		}
		if (object.item == InteractableItem::Oven) {
			switch ( object.dialogueCount) {
				case 0 : {
					if (reaction.choice == 0) {
						DialogueRequest& req = registry.dialogueRequests.emplace(reaction.object);
						if (!registry.keyItems.has(player)) {
							registry.keyItems.emplace(player);
						}
						KeyItems& keyItems = registry.keyItems.get(player);
						if (keyItems.hasSeenPieRecipe) {
							if (reaction.choice == 0) {
								if (keyItems.fruits > 0 && keyItems.honey > 0) {
									req.choice = 6;
								} else if (keyItems.fruits > 0) {
									req.choice = 4;
								} else if (keyItems.honey > 0) {
									req.choice = 3;
								} else {
									req.choice = 5;
								}
								object.dialogueCount = 1;
							}
						} else {
							req.choice = 2;
						}

					}
					break;
				}

			}
		}
		if (object.item == InteractableItem::OracleTurret) {
			switch ( object.dialogueCount) {
				case 0 : {
					if (reaction.choice == 0) {
						object.dialogueCount = 1;
					}
					break;
				}
				case 1 : {
					object.dialogueCount = 2;
					break;
				}
				case 2 : {
					object.dialogueCount = 3;
					break;
				}
				case 3 : {
					object.dialogueCount = 4;
					break;
				}
				case 4 : {
					object.dialogueCount = 5;
					break;
				}
				case 5 : {
					object.dialogueCount = 2;
					break;
				}


			}
		}

		registry.interactableReactions.clear();
	}
}



