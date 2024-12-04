#include "ui_system.hpp"

#include "sound_system.hpp"
#include "text_system.hpp"
#include "premades.hpp"
#include "utils/colours.hpp"
#include <fstream>
#include <iomanip>
#include <sstream>

#include "world_init.hpp"

using Clock = std::chrono::high_resolution_clock;

UISystem::UISystem(SoundSystem* soundSystem) {
	this->soundSystem = soundSystem;
}

UISystem::~UISystem() {

}

void UISystem::step(float elapsed_ms) {
	GameState& gameState = registry.gameStates.components[0];
	IOState& ioState = registry.ioStates.components[0];

	registry.renderRequests.get(gameOverMenu).show = gameState.gameOver;
	registry.renderRequests.get(titleScreen).show = gameState.loading || gameState.titleScreen;

	//update FPS
	WindowState& ws = registry.windowStates.components[0];
	float elapsed = (float)(std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - ws.currUnixTime)).count() / 1000;
	registry.renderRequests.get(fpsCounter).show = ioState.showFPS;

	// check: should game be paused right now?
	// if already paused, then close latest menu
	// if latest menu is the pause menu, then unpause
	if (ioState.pressedEsc) {
		if (!gameState.gamePaused) {
			gameState.gamePaused = true;
			gameState.previousVolume = gameState.currentVolume;
			gameState.currentVolume *= 0.125f;
		}
		else {
			Entity currMenu = registry.activeMenus.entities[registry.activeMenus.entities.size() - 1];
			registry.renderRequests.get(currMenu).show = false;
			registry.activeMenus.remove(currMenu);
			ioState.activeMenu--;
			// clear current menu choices
			for (int i = registry.menuChoices.size() - 1; i >= 0; i--) {
				Entity e = registry.menuChoices.entities[i];
				registry.deleteEntityAndRelatedEntities(e);
			}

			if (ioState.activeMenu < 0) {
				gameState.gamePaused = false;
				gameState.currentVolume = gameState.previousVolume;
			}
		}
		ioState.pressedEsc = false;
	}

	// check: should menu be opened?
	if (gameState.titleScreen && !registry.activeMenus.has(registry.menus.entities[MenuType::TitleMenu])) {
		registry.activeMenus.emplace(registry.menus.entities[MenuType::TitleMenu]);
		soundSystem->playTitleMusic();
		ioState.activeMenu++;
	}
	if (gameState.gamePaused && !gameState.gameOver && !registry.activeMenus.has(registry.menus.entities[MenuType::PauseMenu])) {
		registry.activeMenus.emplace(registry.menus.entities[MenuType::PauseMenu]);
		ioState.activeMenu++;
	}

	if (ioState.activeMenu > -1) {
		if (registry.menuChoices.components.size() == 0) {
			Menu& menu = registry.menus.get(registry.activeMenus.entities[ioState.activeMenu]);
			//std::cout << "active: " << ioState.activeMenu << ", id: " << menu.type << std::endl;

			for (int i = 0; i < menu.options.size(); i++) {
				createMenuChoice(menu.options[i], menu.startPos + (float)i * menu.offset);
			}

			ioState.hoveringMenuChoice = 0;
			ioState.lastHoverMenuChoice = 0;
		}
		if (registry.menuChoices.entities.size() > 0) {
			// process hovered
			int lastChoice = registry.ioStates.components[0].lastHoverMenuChoice;
			int hoveringChoice = registry.ioStates.components[0].hoveringMenuChoice;
			
			int currHover = 0;
			// process mouse hovered
			for (Entity button : registry.buttons.entities) {
				UIButton& buttonComponent = registry.buttons.get(button);
				if (ioState.mousePosition.x > (buttonComponent.position.x - buttonComponent.buttonSize.x / 2) && ioState.mousePosition.x < (buttonComponent.position.x + buttonComponent.buttonSize.x / 2)
					&& ioState.mousePosition.y > (buttonComponent.position.y - buttonComponent.buttonSize.y / 2) && ioState.mousePosition.y < (buttonComponent.position.y + buttonComponent.buttonSize.y / 2)) {
					if (hoveringChoice != currHover) {
						registry.ioStates.components[0].lastHoverMenuChoice = hoveringChoice;
						hoveringChoice = currHover;
						registry.ioStates.components[0].hoveringMenuChoice = hoveringChoice;
						break;
					}
				}
				currHover++;
			}

			// unhighlight the last hovered choice
			registry.renderRequests.get(registry.menuChoices.entities[lastChoice]).show = false;
			registry.textRenderRequests.get(registry.menuChoices.entities[lastChoice]).color = vec3(1, 1, 1);
			// highlight current choice
			registry.renderRequests.get(registry.menuChoices.entities[hoveringChoice]).show = true;
			registry.textRenderRequests.get(registry.menuChoices.entities[hoveringChoice]).color = vec3(1, 1, 0);

			int clickedButtonIndex = -1;

			// process key press
			if (ioState.confirmedOption) {
				clickedButtonIndex = hoveringChoice;
			}

			// process clicked
			if (ioState.clickedButton) {
				int count = 0;
				for (Entity button : registry.buttons.entities) {
					UIButton& buttonComponent = registry.buttons.get(button);
					if (ioState.mousePosition.x > (buttonComponent.position.x - buttonComponent.buttonSize.x / 2) && ioState.mousePosition.x < (buttonComponent.position.x + buttonComponent.buttonSize.x / 2)
						&& ioState.mousePosition.y >(buttonComponent.position.y - buttonComponent.buttonSize.y / 2) && ioState.mousePosition.y < (buttonComponent.position.y + buttonComponent.buttonSize.y / 2)) {
						if (count == hoveringChoice) { 
							clickedButtonIndex = count;
							break;
						}
					}
					count++;
				}
			}

			// button was pressed
			if (clickedButtonIndex > -1) {
				//std::cout << "button pressed: " << clickedButtonIndex << std::endl;
				//std::cout << "total choices " << registry.menuChoices.entities.size() << std::endl;

				ioState.confirmedOption = false;
				ioState.clickedButton = false;
				registry.ioStates.components[0].lastHoverMenuChoice = -1;
				registry.ioStates.components[0].hoveringMenuChoice = -1;
				
				if (registry.menus.get(registry.activeMenus.entities[ioState.activeMenu]).type == MenuType::TitleMenu) {
					if (clickedButtonIndex == 0) {
						gameState.titleScreen = false;
						gameState.loading = true;
						ioState.shouldRestart = true;
					}
					else {
						ioState.shouldEnd = true;
					}
					registry.activeMenus.remove(registry.activeMenus.entities[registry.activeMenus.entities.size() - 1]);
					ioState.activeMenu--;
				}
				else if (registry.menus.get(registry.activeMenus.entities[ioState.activeMenu]).type == MenuType::PauseMenu)
				{
					if (clickedButtonIndex == 0) {
						//std::cout << "show controls!" << std::endl;
						registry.activeMenus.emplace(registry.menus.entities[MenuType::ControlsMenu]);
						ioState.activeMenu++;
						registry.renderRequests.get(controlsGuide).show = true;
					}
					else if (clickedButtonIndex == 1) {
						ioState.shouldRestart = true;
						gameState.titleScreen = true;
						//std::cout << "to title!" << std::endl;
					}
					else if (clickedButtonIndex == 2) {
						ioState.shouldEnd = true;
						//std::cout << "quit!" << std::endl;
					}
					else {
						gameState.gamePaused = false;
						registry.renderRequests.get(pauseMenu).show = false;
					}

					if (clickedButtonIndex != 0) {
						registry.activeMenus.remove(registry.activeMenus.entities[registry.activeMenus.entities.size() - 1]);
						ioState.activeMenu--;
						gameState.currentVolume = gameState.previousVolume;
					}
				}
				else if (registry.menus.get(registry.activeMenus.entities[ioState.activeMenu]).type == MenuType::ControlsMenu) {
					registry.renderRequests.get(controlsGuide).show = false;
					registry.activeMenus.remove(registry.activeMenus.entities[registry.activeMenus.entities.size() - 1]);
					ioState.activeMenu--;
				}
				else if (registry.menus.get(registry.activeMenus.entities[ioState.activeMenu]).type == MenuType::GameOverMenu) {
					if (clickedButtonIndex == 0) {
						ioState.shouldRestart = true;
						gameState.titleScreen = true;
					}
					else if (clickedButtonIndex == 1) {
						ioState.shouldEnd = true;
					}
					registry.activeMenus.remove(registry.activeMenus.entities[registry.activeMenus.entities.size() - 1]);
					ioState.activeMenu--;
				}

				// clear current menu choices
				for (int i = registry.menuChoices.size() - 1; i >= 0; i--) {
					Entity e = registry.menuChoices.entities[i];
					registry.deleteEntityAndRelatedEntities(e);
				}
			}
		}
	}
	StackCompile& stack = registry.stackCompile.get(registry.players.entities[0]);
	StackUI& stackui = registry.stackUI.get(stackUI);

	// update bullet ui positions, add stack notifs
	if (stack.currStack.size() > stackui.bulletPositions.size()) {

		// clean up stack add notifs
		for (int i = registry.stackAddNotifs.size() - 1; i >= 0; i--) {
			Entity e = registry.stackAddNotifs.entities[i];
			registry.deleteEntityAndRelatedEntities(e);
		}

		int diff = stack.currStack.size() - stackui.bulletPositions.size();
		// set up bubble first
		vec2 playerPos = registry.motions.get(registry.players.entities[0]).position;
		registry.renderRequests.get(stackAddBubble).show = true;
		registry.renderRequests.get(stackAddTail).show = true;
		updateStackAddBubble(playerPos, diff);
		
		if (!registry.showTimers.has(stackAddBubble)) {
			if (registry.fades.has(stackAddBubble)) {
				registry.fades.remove(stackAddBubble);
				registry.fades.remove(stackAddTail);
			}
			registry.showTimers.emplace(stackAddBubble);
			registry.showTimers.emplace(stackAddTail);
		}
		else {
			registry.showTimers.get(stackAddBubble).timer += registry.showTimers.get(stackAddBubble).base;
			registry.showTimers.get(stackAddTail).timer += registry.showTimers.get(stackAddBubble).base;
		}
		
		vec2 bulletStartPos = playerPos;
		for (int i = 0; i < diff; i++) {
			int index = i + stack.currStack.size() - diff;
			stackui.bulletPositions.push_back(vec2(stackui.bulletStartPos.x + index * stackui.bulletSize.x + index * stackui.bulletOffset,
				stackui.bulletStartPos.y));
			// can notify player when stack has changed here
			// NOTE: does not work for lightning bullets. Lightning bullets detected in handle player collision 
			// this spawn position is also v incorrect, corrected later in usual update
			createStackAddNotif(vec2(bulletStartPos.x + index * stackui.bulletSize.x + index * stackui.bulletOffset, bulletStartPos.y), 
				registry.stackUI.components[0].bulletSize* STACK_NOTIF_SCALE,
				bulletEffectShapes.at(stack.currStack[index].type),
				bulletEffectColors.at(stack.currStack[index].type));
		}
	}

	// clean up stack add notifs that have faded out
	for (int i = registry.stackAddNotifs.size() - 1; i >= 0; i--) {
		Entity e = registry.stackAddNotifs.entities[i];
		if (!registry.renderRequests.get(e).show)
			registry.deleteEntityAndRelatedEntities(e);
	}

	// handle ui requests
	for (UIRequest& uiRequest : registry.uiRequests.components) {
		if (uiRequest.type == UIRequestType::StackNotifReqShift 
			|| uiRequest.type == UIRequestType::StackNotifReqShuffle 
			|| uiRequest.type == UIRequestType::CallNotif) {
			// clean up stack add notifs
			for (int i = registry.stackAddNotifs.size() - 1; i >= 0; i--) {
				Entity e = registry.stackAddNotifs.entities[i];
				registry.deleteEntityAndRelatedEntities(e);
			}
			
			vec2 playerPos = registry.motions.get(registry.players.entities[0]).position;
			registry.renderRequests.get(stackAddBubble).show = true;
			registry.renderRequests.get(stackAddTail).show = true;
			updateStackAddBubble(playerPos,1);

			if (!registry.showTimers.has(stackAddBubble)) {
				if (registry.fades.has(stackAddBubble)) {
					registry.fades.remove(stackAddBubble);
					registry.fades.remove(stackAddTail);
				}
				registry.showTimers.emplace(stackAddBubble);
				registry.showTimers.emplace(stackAddTail);
			}
			else {
				registry.showTimers.get(stackAddBubble).timer += registry.showTimers.get(stackAddBubble).base;
				registry.showTimers.get(stackAddTail).timer += registry.showTimers.get(stackAddBubble).base;
			}
			vec2 bulletStartPos = playerPos;
			std::string sprite = "stackNotifShift.png";
			if (uiRequest.type == UIRequestType::StackNotifReqShuffle) {
				sprite = "stackNotifShuffle.png";
			}
			else if (uiRequest.type == UIRequestType::CallNotif) {
				sprite = "callNotif.png";
			}

			createStackAddNotif(vec2(bulletStartPos.x, bulletStartPos.y), vec2(192) / 2.5f, sprite, vec3(1));
		}

		if (uiRequest.type == UIRequestType::ResetUI) {
			DialogueLines& lines = registry.dialogueLines.components[0];
			lines = DialogueLines();

			// clear choices here for now
			for (int i = registry.dialogueChoices.size() - 1; i >= 0; i--) {
				Entity e = registry.dialogueChoices.entities[i];
				registry.deleteEntityAndRelatedEntities(e);
			}
			for (int i = registry.menuChoices.size() - 1; i >= 0; i--) {
				Entity e = registry.menuChoices.entities[i];
				registry.deleteEntityAndRelatedEntities(e);
			}

			registry.activeMenus.clear();
			ioState.activeMenu = -1;
			registry.renderRequests.get(stackAddBubble).show = false;
			registry.renderRequests.get(stackAddTail).show = false;
			registry.renderRequests.get(controlsGuide).show = false;
		}

		if (uiRequest.type == UIRequestType::GameOverReport) {
			std::string report = "\n" + reportStats();
			std::vector<std::string> reportTokenized = getTokenizedText(report);
			TextRenderRequest& text = registry.textRenderRequests.get(gameOverMenu);
			text.tokenizedText = uiTexts["GameOver"];
			text.tokenizedText.insert(text.tokenizedText.end(), reportTokenized.begin(), reportTokenized.end());
			registry.activeMenus.emplace(registry.menus.entities[MenuType::GameOverMenu]);
			ioState.activeMenu++;
		}

		if (uiRequest.type == UIRequestType::RoomClear) {
			registry.renderRequests.get(roomClearMessage).show = true;
			registry.showTimers.emplace(roomClearMessage);
			soundSystem->playRareItemPickupSound();
		}
	}

	registry.uiRequests.clear();

	// move position of bullet add notif
	if (registry.stackAddNotifs.entities.size() > 0) {
		vec2 playerPos = registry.motions.get(registry.players.entities[0]).position;
		vec2 bulletStartPos = playerPos + abs(registry.motions.get(registry.players.entities[0]).scale) * vec2(1, -1);
		updateStackAddBubble(bulletStartPos, registry.stackAddNotifs.entities.size());
		int index = 0;
		for (Entity entity : registry.stackAddNotifs.entities) {
			registry.motions.get(entity).position = vec2(bulletStartPos.x + index * stackui.bulletSize.x * STACK_NOTIF_SCALE + index * stackui.bulletOffset * STACK_NOTIF_SCALE, bulletStartPos.y);
			index++;
		}
	}

	else if (stack.currStack.size() < stackui.bulletPositions.size()) {
		// much harder to know what bullets got removed from stack though
		// need to rely on interact system for that (seems to be the only way bullets are popped?)
		stackui.bulletPositions.resize(stack.currStack.size());
	}

	if (gameState.gamePaused || gameState.dialogueScene || registry.maps.components[0].currRoom.cleared) {
		// is the player hovering over a stack ui bullet right now?
		// bad: copies code from render system; consider making each bullet an entity
		// may optimize using some other method like colour picking/just limiting search size
		// in the future (since search space is pretty deterministic)
		int bulletHoveredIndex = -1;
		int count = -1;
		vec2 bulletSize = stackui.bulletSize;
		// should check first: is it in stack ui at all?
		// this is point in aabb detection
		if (ioState.mousePosition.x > (stackui.stackPos.x - stackui.stackSize.x / 2) && ioState.mousePosition.x < (stackui.stackPos.x + stackui.stackSize.x / 2)
			&& ioState.mousePosition.y >(stackui.stackPos.y - stackui.stackSize.y / 2) && ioState.mousePosition.y < (stackui.stackPos.y + stackui.stackSize.y / 2)) {
			for (vec2 bulletPos : stackui.bulletPositions) {
				count++;
				if (ioState.mousePosition.x > (bulletPos.x - bulletSize.x / 2) && ioState.mousePosition.x < (bulletPos.x + bulletSize.x / 2)
					&& ioState.mousePosition.y >(bulletPos.y - bulletSize.y / 2) && ioState.mousePosition.y < (bulletPos.y + bulletSize.y / 2)) {
					bulletHoveredIndex = count;
					break;
				}
			}
			if (bulletHoveredIndex > -1 && lastHoveredBullet != bulletHoveredIndex) {
				updateBulletUI(vec2(stackui.bulletStartPos.x + bulletHoveredIndex * stackui.bulletSize.x + bulletHoveredIndex * stackui.bulletOffset,
					stackui.bulletStartPos.y), stack.currStack[bulletHoveredIndex]);
			}
			else if (bulletHoveredIndex == -1) {
				registry.renderRequests.get(bulletUI).show = false;
				registry.renderRequests.get(bulletUIArrow).show = false;
			}
			else {
			}
			lastHoveredBullet = bulletHoveredIndex;
		}
		else {
			registry.renderRequests.get(bulletUI).show = false;
			registry.renderRequests.get(bulletUIArrow).show = false;
		}
	}
	else {
		registry.renderRequests.get(bulletUI).show = false;
		registry.renderRequests.get(bulletUIArrow).show = false;
	}

	if (!gameState.gameOver) {
		// toggling basic menu uis on/off
		registry.renderRequests.get(pauseMenu).show = gameState.gamePaused && !registry.renderRequests.get(controlsGuide).show;
		registry.renderRequests.get(controlsGuide).show = registry.renderRequests.get(controlsGuide).show && gameState.gamePaused;
		registry.renderRequests.get(dialogueBox).show = gameState.dialogueScene;
		registry.renderRequests.get(dialogueReminder).show = gameState.dialogueScene;

		StackCompile& stack = registry.stackCompile.get(registry.players.entities[0]);
		StackUI& stackui = registry.stackUI.get(stackUI);

		// update stack ui
		registry.textRenderRequests.get(stackUI).text = "Stack: " + std::to_string(stack.currStack.size()) + " / " + std::to_string((int)getModifiedValue(PlayerStackSize, stack.baseStackSize));

		ws.numFramesThisSecond++;
		if (elapsed > 1000.0f) {
			ws.fps = ws.numFramesThisSecond;
			ws.numFramesThisSecond = 0;
			ws.currUnixTime = Clock::now();
			TextRenderRequest& fpsText = registry.textRenderRequests.get(fpsCounter);
			fpsText.text = "FPS: " + std::to_string(ws.fps);
		}
		
		Map& map = registry.maps.components[0];
		TextRenderRequest& roomCounterText = registry.textRenderRequests.get(roomCounter);
		std::string region;
		if (map.currRegion == Tutorial) region = "Tutorial";
		if (map.currRegion == Biology) region = "Biology";
		if (map.currRegion == Physics) region = "Physics";
		roomCounterText.text = region + " Room " + std::to_string(map.roomsTraversed);
		
		if (!gameState.dialogueScene && !gameState.cutScene && !gameState.gamePaused) { // normal game uis
			registry.renderRequests.get(dialogueAvatar).show = false;
			registry.renderRequests.get(screenCutIn).show = false;
			// clear prev frame's e indicators
			for (Entity entity : registry.interactIndicators.entities) {
				if (!registry.deleteds.has(entity)) {
					registry.deleteds.emplace(entity);
				}
			}

			// draw "press e to interact" over all items in nearby interactables list
			for (Entity entity : registry.nearbyInteractables.entities) {
				createInteractIndicator(registry.motions.get(entity).position);
			}
		
		} 
		if (gameState.dialogueScene) {
			// update which dialogue choice is highlighted. Consider updating only when necessary?
			if (registry.dialogueChoices.entities.size() > 0) {
				int lastChoice = registry.ioStates.components[0].lastHoverDialogueChoice;
				int hoveringChoice = registry.ioStates.components[0].hoveringDialogueChoice;
				// unhighlight the last hovered choice
				registry.renderRequests.get(registry.dialogueChoices.entities[lastChoice]).show = false;
				registry.textRenderRequests.get(registry.dialogueChoices.entities[lastChoice]).color = vec3(1, 1, 1);
				// highlight current choice
				registry.renderRequests.get(registry.dialogueChoices.entities[hoveringChoice]).show = true;
				registry.textRenderRequests.get(registry.dialogueChoices.entities[hoveringChoice]).color = vec3(1, 1, 0);
			}
		}
	}
}

bool UISystem::init(GLFWwindow* window) {
	// just use window state until can figure out grabbing from window directly
	WindowState& wS = registry.windowStates.components[0];

	loadText();
	loadBulletEffects();
	
	stackUI = createStackUI(wS, registry.stackCompile.components[0]);
	dialogueBox = createDialogueBox(vec2(wS.width / 2, wS.height - wS.height / 8), vec2(wS.width, wS.height / 4));
	dialogueAvatar = createDialogueAvatar(vec2(150, wS.height - wS.height / 8 - 25), vec2(wS.height / 4 - 100, wS.height / 4 - 100));
	screenCutIn = createScreenCutIn();
	bulletUI = createBulletUI();
	bulletUIArrow = createBulletUIArrow();
	fpsCounter = createFpsCounter();
	roomCounter = createRoomCounter();
	titleScreen = createTitleScreen();
	pauseMenu = createPauseMenu(vec2(wS.width / 2, wS.height / 2), vec2(wS.width / 4, wS.height - 200.f));
	controlsGuide = createControlsGuide(vec2(wS.width / 2, wS.height / 2), vec2(wS.width / 3, wS.height - 200.f));
	gameOverMenu = createGameOverMenu(vec2(wS.width / 2, wS.height / 2), vec2(wS.width / 2.5, wS.height - 200.f));
	stackAddBubble = createStackAddBubble();
	stackAddTail = createStackAddTail();
	dialogueReminder = createDialogueReminder();
	roomClearMessage = createRoomClearMessage();

	return true;
}

void UISystem::playDialogue() {
	IOState& input = registry.ioStates.components[0];
	GameState& gameState = registry.gameStates.components[0];
	WindowState& wS = registry.windowStates.components[0];

	if (gameState.dialogueScene && input.nextDialogue) {
		// keep track of current speaker stuff
		std::string currSpeakerName = registry.dialogueLines.get(dialogueBox).prev().speakerName;
		std::string currSpeakerAvatar = registry.dialogueLines.get(dialogueBox).prev().speakerAvatar;
		std::string currCutIn = registry.dialogueLines.get(dialogueBox).prev().cutInTexture;

		input.nextDialogue = false;
		Dialogue nextLine = registry.dialogueLines.get(dialogueBox).next();
		if (nextLine.text.compare("<end>") != 0) { // there is a next line
			registry.renderRequests.get(dialogueBox).show = true;
			registry.textRenderRequests.get(dialogueBox).tokenizedText = nextLine.tokenizedText;
			
			// play a sound if there is one
			if (nextLine.sfx == IncomingDialogue) {
				soundSystem->playIncomingDialogueSound();
			} else if (nextLine.sfx == DoorOpen) {
				soundSystem->playDoorOpenSound();
			} else if (nextLine.sfx == DoorClose) {
				soundSystem->playDoorCloseSound();
			} else if (nextLine.sfx == itemPickup) {
				soundSystem->playItemPickupSound();
			} else if (nextLine.sfx == rareItemPickup) {
				soundSystem->playRareItemPickupSound();
			} else if (nextLine.sfx == explosion) {
				soundSystem->playExplosionSound(2);
			}
			else {
				soundSystem->stopIncomingDialogueSound();
				soundSystem->playNextDialogueSound();
			}

			// change speaker avatar and name to current
			if (nextLine.speakerName.length() > 0 && nextLine.speakerName.compare(currSpeakerName) != 0 && nextLine.speakerName.compare("N") != 0) {
				registry.textRenderRequests.get(dialogueAvatar).text = nextLine.speakerName;
			}
			if (nextLine.speakerAvatar.length() > 0 && nextLine.speakerAvatar.compare(currSpeakerAvatar) != 0) {
				if (nextLine.speakerAvatar.compare("N") != 0) { // N is narrator for now
					registry.renderRequests.get(dialogueAvatar).show = true;
					registry.renderRequests.get(dialogueAvatar).texture_name = nextLine.speakerAvatar;
				}
				else {
					registry.renderRequests.get(dialogueAvatar).show = false;
				}
			}
			
			// change screen cut in image if there is one
			if (nextLine.cutInTexture.length() > 0 && nextLine.cutInTexture.compare(currCutIn) != 0) {
				if (nextLine.cutInTexture.compare("N") != 0) {
					registry.renderRequests.get(screenCutIn).texture_name = nextLine.cutInTexture;
					registry.renderRequests.get(screenCutIn).show = true;
				}
				else {
					registry.renderRequests.get(screenCutIn).show = true;
				}
			}

			vec2 startingPosition = vec2(400, wS.height - 35);
			// display options for player if there is one
			for (int i = nextLine.choices.size() - 1; i >= 0 ; i--) {
				//std::cout << nextLine.choices[i] << std::endl;
				vec2 nextPosition = vec2(startingPosition.x, startingPosition.y - (nextLine.choices.size() - 1 - i) * 50);
				//std::cout << nextPosition.y << std::endl;
				createDialogueChoice(nextLine.choices[i], nextPosition);
			}
			// set first choice to highlighted by default
			if (registry.dialogueChoices.components.size() > 0) {
				input.hoveringDialogueChoice = registry.dialogueChoices.components.size() - 1 - 0;
				input.lastHoverDialogueChoice = registry.dialogueChoices.components.size() - 1 - 0;
				// change dialogue reminder text
				TextRenderRequest& reminderText = registry.textRenderRequests.get(dialogueReminder);
				reminderText.text = "[W, S] Select, [E] Confirm";
				reminderText.x = registry.windowStates.components[0].width - reminderText.text.length() * reminderText.scale * 48 - 35;
			}
			else {
				// change dialogue reminder text
				TextRenderRequest& reminderText = registry.textRenderRequests.get(dialogueReminder);
				reminderText.text = "[E] Next";
				reminderText.x = registry.windowStates.components[0].width - reminderText.text.length() * reminderText.scale * 48 - 35;
			}
		}
		// no more lines of dialogue
		else {
			registry.renderRequests.get(dialogueBox).show = false;
			registry.renderRequests.get(dialogueAvatar).show = false;
			Map& map = registry.maps.components[0];
			map.currRoom.dialogueDone = true;
			gameState.dialogueScene = false;
			soundSystem->stopNextDialogueSound();
		}
	}
}

Entity UISystem::createButton(std::string label, vec2 position, vec2 scale) {
	Entity entity = Entity();
	WindowState& windowState = registry.windowStates.components[0];

	auto& rr = registry.renderRequests.insert(
		entity,
		{ "enemy_bullet_square.png", // temporary choice selection indicator
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.show = true;

	UIButton& button = registry.buttons.emplace(entity);

	registry.menuOverlayUIs.emplace(entity);
	registry.menuOverlayUITexts.emplace(entity);

	auto& text = registry.textRenderRequests.emplace(entity);
	text.color = vec3(1, 1, 1);
	text.scale = 0.40;
	text.text = label;
	text.topRightBound = { windowState.width, windowState.height };
	text.bottomLeftBound = { 0, 0 };
	text.y = windowState.height - position.y - button.padding;
	text.x = position.x ;
	text.alignment = TextAlignment::CenteredAlign;

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.scale = scale;
	motion.position = position;

	vec3& color = registry.colors.emplace(entity);
	color = COLOR_TEAL_MED;

	return entity;
}

Entity UISystem::createDialogueReminder() {
	WindowState& windowState = registry.windowStates.components[0];
	Entity entity = Entity();

	registry.dialogueUITexts.emplace(entity);
	auto& rr = registry.renderRequests.insert(
		entity, { "none",
				 EFFECT_ASSET_ID::EGG,
				 GEOMETRY_BUFFER_ID::DEBUG_LINE });
	rr.show = false;

	TextRenderRequest& trr = registry.textRenderRequests.emplace(entity);

	float padding = 35.f;
	trr.text = "[E] Next";
	trr.color = vec3(1.0f);
	trr.scale = 0.35f;
	trr.x = windowState.width - trr.text.length() * trr.scale * 48 - padding;
	trr.y = padding;
	trr.topRightBound = { windowState.width + 1000,windowState.height };
	trr.bottomLeftBound = { 0,0 };

	return entity;
}

Entity UISystem::createStackAddTail() {
	Entity entity = Entity();

	auto& rr = registry.renderRequests.insert(
		entity,
		{ "enemy_bullet_triangle.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.show = false;

	registry.gameUIs.emplace(entity);

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = M_PI - M_PI / 4.f;
	motion.velocity = { 0, 0 };
	motion.position = { 0,0 };
	motion.scale = { 12.5 , 12.5 };

	vec3& color = registry.colors.emplace(entity);
	color = vec3(1.f);

	return entity;
}

Entity UISystem::createStackAddBubble() {
	Entity entity = Entity();

	auto& rr = registry.renderRequests.insert(
		entity,
		{ "enemy_bullet_square.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.show = false;

	registry.gameUIs.emplace(entity);

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0;
	motion.velocity = { 0, 0 };
	motion.position = { 0,0 };
	motion.scale = { 50 * STACK_NOTIF_SCALE, 50 * STACK_NOTIF_SCALE } ;

	vec3& color = registry.colors.emplace(entity);
	color = vec3(0.f);

	UIBorder& border = registry.uiBorders.emplace(entity);
	border.borderColour = vec3(1.f);
	border.border = UIBorderType::Outlined;
	border.borderThickness = 5.f;

	return entity;
}

Entity UISystem::createStackAddNotif(vec2 position, vec2 scale, std::string sprite, vec3 c) {
	Entity entity = Entity();

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = scale;
	// but honestly this should be constant, not a field tied to stack ui??

	auto& rr = registry.renderRequests.insert(
		entity,
		{ sprite,
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.show = true;

	registry.gameUIs.emplace(entity);

	vec3& color = registry.colors.emplace(entity);
	color = c;
	
	registry.showTimers.emplace(entity);
	registry.stackAddNotifs.emplace(entity);

	return entity;
}

void UISystem::updateStackAddBubble(vec2 position, int bulletNum) {
	Motion& motion = registry.motions.get(stackAddBubble);
	vec2 bulletSize = registry.stackUI.components[0].bulletSize;
	float bulletOffset = registry.stackUI.components[0].bulletOffset;
	vec2 bulletStartPos = position;
	motion.scale = vec2(bulletNum * bulletSize.x * STACK_NOTIF_SCALE + bulletNum * bulletOffset * STACK_NOTIF_SCALE + 2 * bulletOffset * STACK_NOTIF_SCALE,
		bulletSize.y * STACK_NOTIF_SCALE + 2 * bulletOffset * STACK_NOTIF_SCALE);
	motion.position = vec2(bulletStartPos.x + motion.scale.x / 2 - bulletSize.x * STACK_NOTIF_SCALE - bulletOffset * STACK_NOTIF_SCALE / 2, bulletStartPos.y);
	motion.scale += vec2(10.f, 0);

	Motion& tailMotion = registry.motions.get(stackAddTail);
	tailMotion.position = motion.position - motion.scale * vec2(0.5, -0.5) - tailMotion.scale / 2.f * vec2(0.5, -0.5);
}

// update bullet ui and its arrow
// position = top middle position
void UISystem::updateBulletUI(vec2 position, BulletStackEffect bullet) {
	Motion& motion = registry.motions.get(bulletUI);
	WindowState& windowState = registry.windowStates.components[0];
	motion.position = vec2(position.x, position.y + motion.scale.y / 2 + 50 + 10);
	if ((motion.position.x - motion.scale.x / 2) < 0 + 25) {
		motion.position.x += (motion.position.x - motion.scale.x / 2) * -1 + 25;
	}
	registry.renderRequests.get(bulletUI).show = true;

	TextRenderRequest& textReq = registry.textRenderRequests.get(bulletUI);
	if (uiTexts.count("HoverBullet_" + bullet.name) > 0) {
		textReq.tokenizedText = uiTexts["HoverBullet_" + bullet.name];
	}
	else {
		// need to generate text and tokenize it
		std::string tooltip = makeBulletTooltip(bullet);
		uiTexts.insert({ "HoverBullet_" + bullet.name, getTokenizedText(tooltip) });
		textReq.tokenizedText = uiTexts["HoverBullet_" + bullet.name];
	}
	textReq.y = windowState.height - motion.position.y + motion.scale.y / 2 - 50;
	textReq.x = motion.position.x - motion.scale.x / 2 + 20;
	textReq.bottomLeftBound = {textReq.x, textReq.y - motion.scale.y + 25};
	textReq.topRightBound = { textReq.x + motion.scale.x - 25, textReq.y };

	Motion& arrowMotion = registry.motions.get(bulletUIArrow);
	arrowMotion.position = { position.x, position.y + 51 };
	registry.renderRequests.get(bulletUIArrow).show = true;
}

Entity UISystem::createBulletUIArrow() {
	Entity entity = Entity();

	auto& rr = registry.renderRequests.insert(
		entity,
		{ "enemy_bullet_triangle.png", // temporary choice selection indicator
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.show = false;

	registry.menuOverlayUIs.emplace(entity);

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = M_PI / 2;
	motion.velocity = { 0, 0 };
	motion.position = { 0,0 };
	motion.scale = {-30, 30};

	vec3& color = registry.colors.emplace(entity);
	color = COLOR_RED;

	return entity;
}

Entity UISystem::createBulletUI() {
	Entity entity = Entity();
	WindowState& windowState = registry.windowStates.components[0];

	auto& rr = registry.renderRequests.insert(
		entity,
		{ "enemy_bullet_square.png", // temporary choice selection indicator
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.show = false;

	registry.menuOverlayUIs.emplace(entity);

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.scale = { 350, 400 }; // hard code size for now; consider scaling to text in future (tho maybe not needed?)
	motion.position = { 0, 0 };

	vec3& color = registry.colors.emplace(entity);
	color = vec3(0.f);

	registry.menuOverlayUITexts.emplace(entity);
	auto& text = registry.textRenderRequests.emplace(entity);
	text.color = vec3(1, 1, 1);
	text.scale = 0.40;
	text.topRightBound = { windowState.width, windowState.height };
	text.bottomLeftBound = { 0, 0 };
	text.y = windowState.height - motion.position.y - 15;
	text.x = motion.position.x - 10;

	UIBorder& border = registry.uiBorders.emplace(entity);
	border.borderThickness = 5.f;
	border.borderColour = COLOR_WHITE;
	border.border = UIBorderType::Outlined;

	return entity;
}

// draw "E" to interact with object above object's position
// create for now instead of drawing above each interactable and showing/hiding (may regret later)
Entity UISystem::createInteractIndicator(vec2 position) {
	Entity entity = Entity();
	WindowState& windowState = registry.windowStates.components[0];

	auto& rr = registry.renderRequests.insert(
		entity,
		{ "enemy_bullet_square.png", // temporary choice selection indicator
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.show = true;

	registry.gameUIs.emplace(entity);

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.scale = { 50, 50 };
	motion.position = { position.x, position.y };
	
	// hard code these offsets to make the doors look nice
	Room room = registry.maps.components[0].currRoom;
	if (position.y <= registry.motions.get(registry.doors.entities[0]).position.y) { // bottom door
		motion.position.y -= 50;
	}
	else if (position.y >= registry.motions.get(registry.doors.entities[2]).position.y) { // top door
		motion.position.y += 50;
	}

	if (position.x <= registry.motions.get(registry.doors.entities[3]).position.x) { // left door
		motion.position.x -= 50;
	}
	else if (position.x >= registry.motions.get(registry.doors.entities[1]).position.x) { // right door
		motion.position.x += 50;
	}

	vec3& color = registry.colors.emplace(entity);
	color = { 0.3,0.3,0.3 };

	registry.gameOverlayUITexts.emplace(entity);
	auto& text = registry.textRenderRequests.emplace(entity);
	text.color = vec3(1, 1, 1);
	text.text = "E";
	text.scale = 0.40;
	text.topRightBound = { room.preset.roomSize.x * 10,  room.preset.roomSize.y * 10 };
	text.bottomLeftBound = { -room.preset.roomSize.x * 10, -room.preset.roomSize.y * 10 };
	text.y = windowState.height - motion.position.y - 15;
	text.x = motion.position.x - 10;

	registry.interactIndicators.emplace(entity);

	return entity;
}

// makes a dialogue choice to be choice
// consider separating text show with render request show
Entity UISystem::createDialogueChoice(std::string choice, vec2 position) {
	Entity entity = Entity();

	auto& rr = registry.renderRequests.insert(
		entity,
		{ "enemy_bullet_triangle.png", // temporary choice selection indicator
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.show = false;

	registry.dialogueUIs.emplace(entity);

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = {30, 30};

	vec3& color = registry.colors.emplace(entity);
	color = COLOR_RED;

	//registry.dialogueUITexts.emplace(entity); // comment out for now to avoid rendering twice (especially drawn in text render)
	auto& text = registry.textRenderRequests.emplace(entity);
	text.color = vec3(1, 1, 1);

	WindowState& windowState = registry.windowStates.components[0];
	text.x = position.x + motion.scale.x;
	text.y = windowState.height - position.y - motion.scale.y / 2;
	text.scale = 0.45;
	text.text = choice;
	text.topRightBound = { windowState.width - 75, windowState.height - 25 };
	text.bottomLeftBound = { text.x + 25, 0 + 25 };

	registry.dialogueChoices.emplace(entity);

	return entity;
}

// should consolidate w/ create dialogue, because is largely the same?
// Difference: menu choices are hoverable/clickable too (but performance cost)
Entity UISystem::createMenuChoice(std::string choice, vec2 position) {
	Entity entity = Entity();

	auto& rr = registry.renderRequests.insert(
		entity,
		{ "enemy_bullet_triangle.png", // temporary choice selection indicator
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.show = false;

	registry.menuUIs.emplace(entity);

	WindowState& windowState = registry.windowStates.components[0];

	auto& text = registry.textRenderRequests.emplace(entity);
	text.color = vec3(1, 1, 1);
	text.scale = 0.55;
	text.text = choice;
	text.topRightBound = { windowState.width - 75, windowState.height - 25 };
	text.bottomLeftBound = { text.x + 25, 0 + 25 };
	text.alignment = TextAlignment::CenteredAlign;

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.scale = { 40, 40 };
	motion.position = vec2(position.x - text.text.length() * text.scale * DEFAULT_FONT_SIZE / 2 - motion.scale.x, position.y);

	text.x = position.x;
	text.y = windowState.height - position.y - text.scale * DEFAULT_FONT_SIZE / 2.f;

	// also make it a button
	UIButton& button = registry.buttons.emplace(entity);
	button.padding = 15.f;
	button.buttonSize = vec2(text.text.length() * text.scale * DEFAULT_FONT_SIZE, text.scale * DEFAULT_FONT_SIZE);
	button.position = vec2(text.x, position.y);

	vec3& color = registry.colors.emplace(entity);
	color = COLOR_RED;

	registry.menuChoices.emplace(entity);

	return entity;
}

Entity UISystem::createDialogueAvatar(vec2 position, vec2 scale) {
	Entity entity = Entity();

	auto& rr = registry.renderRequests.insert(
		entity,
		{ "eel.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.show = false;

	registry.dialogueUIs.emplace(entity);

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = scale;

	// attach 1 text render request
	registry.dialogueUITexts.emplace(entity);
	auto& text = registry.textRenderRequests.emplace(entity);
	text.color = vec3(1, 1, 1);

	WindowState& windowState = registry.windowStates.components[0];
	text.x = position.x;				
	text.y = windowState.height - position.y - scale.y / 2 - 40;
	text.scale = 0.45;
	text.text = "speaker name";
	// too lazy to calculate fitting text box size, and it prob won't overflow
	// so just set it to some big number
	text.topRightBound = { 1000, 1000 };
	text.bottomLeftBound = { 0, 0 };
	text.alignment = TextAlignment::CenteredAlign;

	return entity;
}


// draws dialogue box
// may end up setting globals for box position later?
Entity UISystem::createDialogueBox(vec2 position, vec2 scale)
{
	Entity entity = Entity();

	// copies code from draw line as a box for now
	// consider doing a check of "should I render now"? Or hide entity?
	auto& rr = registry.renderRequests.insert(
		entity,
		{ "enemy_bullet_square.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.show = false;

	registry.dialogueUIs.emplace(entity);

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = scale;

	auto& color = registry.colors.emplace(entity);
	color.r = 0.0;
	color.b = 0.0;
	color.g = 0.0;

	// attach 1 text render request
	registry.dialogueUITexts.emplace(entity);
	auto& text = registry.textRenderRequests.emplace(entity);
	text.color = vec3(1, 1, 1);

	// want to place at top of dialogue box
	// with current text projection matrix being "flipped" coords
	// temp fix for getting window size for now
	WindowState& windowState = registry.windowStates.components[0];
	text.x = windowState.width - scale.x + 300;				// 25 is just some padding
	text.y = windowState.height - position.y + scale.y / 4; // place text slightly above middle of box
	text.scale = 0.5;										
	text.topRightBound = { scale.x - 75, scale.y - 25 };
	text.bottomLeftBound = { text.x + 25, 0 + 25 };

	// attach list of dialogue lines
	// probably shouldn't be attached to box, but to some dialogue state entity?
	auto& lines = registry.dialogueLines.emplace(entity);

	return entity;
}

Entity UISystem::createControlsGuide(vec2 position, vec2 scale) {
	Entity entity = Entity();

	// copies code from draw line as a box for now
	auto& rr = registry.renderRequests.insert(
		entity,
		{ "enemy_bullet_square.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.show = false;

	registry.menuUIs.emplace(entity);

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = scale;

	auto& color = registry.colors.emplace(entity);
	color.r = 0.0;
	color.b = 0.0;
	color.g = 0.0;

	// attach 1 text render request
	registry.menuUITexts.emplace(entity);
	auto& text = registry.textRenderRequests.emplace(entity);
	text.color = vec3(1, 1, 1);

	WindowState& windowState = registry.windowStates.components[0];
	text.scale = 0.5;
	text.x = position.x - scale.x/2.f + 25;
	text.y = position.y + scale.y / 2.f - 25.f - text.scale * DEFAULT_FONT_SIZE;
	text.topRightBound = { position.x + scale.x / 2.f - 25, position.y + scale.y / 2.f - 25 };
	text.bottomLeftBound = { text.x, 0 + 25 };
	text.tokenizedText = uiTexts["ControlsGuide"];

	UIBorder& border = registry.uiBorders.emplace(entity);
	border.borderColour = vec3(1.f);
	border.border = UIBorderType::Outlined;
	border.borderThickness = 10.f;

	Menu& menu = registry.menus.emplace(entity);
	menu.options = { "Back" };
	menu.startPos = { position.x, position.y + scale.y / 2.f - 50 };
	menu.offset = { 0, 50 + 30 };
	menu.type = MenuType::ControlsMenu;

	return entity;
}

// not a real menu right now; just to show the game is paused
Entity UISystem::createPauseMenu(vec2 position, vec2 scale)
{
	Entity entity = Entity();

	// copies code from draw line as a box for now
	auto& rr = registry.renderRequests.insert(
		entity,
		{ "enemy_bullet_square.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.show = false;

	registry.menuUIs.emplace(entity);

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = scale;

	// temp colour
	auto& color = registry.colors.emplace(entity);
	color.r = 0.0;
	color.b = 0.0;
	color.g = 0.0;

	// attach 1 text render request
	registry.menuUITexts.emplace(entity);
	auto& text = registry.textRenderRequests.emplace(entity);
	text.color = vec3(1, 1, 1);

	// note: position is not center, but start of text rendering
	// need a mechanism to figure out text line size
	WindowState& windowState = registry.windowStates.components[0];
	text.x = position.x;
	text.scale = 0.70;
	text.y = windowState.height - (position.y - motion.scale.y / 2.f + 25.f + text.scale * DEFAULT_FONT_SIZE);
	text.text = "Game Paused";
	text.topRightBound = { position.x + scale.x - 25, position.y - scale.y - 25 };
	text.bottomLeftBound = { text.x - 25, 0 + 25 };
	text.alignment = TextAlignment::CenteredAlign;

	UIBorder& border = registry.uiBorders.emplace(entity);
	border.borderColour = vec3(1.f);
	border.border = UIBorderType::Outlined;
	border.borderThickness = 10.f;

	Menu& menu = registry.menus.emplace(entity);
	menu.options = { "Controls", "Title", "Quit", "Resume"};
	menu.startPos = { text.x,  windowState.height - text.y + 100};
	menu.offset = { 0, 50 + 30 };
	menu.type = MenuType::PauseMenu;

	return entity;
}

// not a real menu right now; just to show the game is over
Entity UISystem::createGameOverMenu(vec2 position, vec2 scale)
{
	Entity entity = Entity();

	// copies code from draw line as a box for now
	auto& rr = registry.renderRequests.insert(
		entity,
		{ "enemy_bullet_square.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.show = false;

	registry.menuUIs.emplace(entity);

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = scale;

	// temp colour
	auto& color = registry.colors.emplace(entity);
	color.r = 0.0;
	color.b = 0.0;
	color.g = 0.0;

	// attach 1 text render request
	registry.menuUITexts.emplace(entity);
	auto& text = registry.textRenderRequests.emplace(entity);
	text.color = vec3(1, 1, 1);
	text.topRightBound = { scale.x, scale.y };
	text.bottomLeftBound = { 0, 0 };

	WindowState& windowState = registry.windowStates.components[0];
	text.scale = 0.7f;
	text.x = position.x - scale.x / 2.f + 25;
	text.y = position.y + scale.y / 2.f - 25.f - text.scale * DEFAULT_FONT_SIZE;
	text.topRightBound = { position.x + scale.x / 2.f - 25, position.y + scale.y / 2.f - 25 };
	text.bottomLeftBound = { 0, 0 + 25 };

	text.tokenizedText = uiTexts["GameOver"];

	UIBorder& border = registry.uiBorders.emplace(entity);
	border.borderColour = vec3(1.f);
	border.border = UIBorderType::Outlined;
	border.borderThickness = 10.f;

	Menu& menu = registry.menus.emplace(entity);
	menu.options = { "Title", "Quit"};
	menu.startPos = { position.x,  position.y + scale.y / 2.f - 200};
	menu.offset = { 0, 50 + 30 };
	menu.type = MenuType::GameOverMenu;

	return entity;
}

Entity UISystem::createStackUI(WindowState& windowState, StackCompile& stack)
{
	Entity entity = Entity();

	auto& rr = registry.renderRequests.insert(
		entity, { "none",
				 EFFECT_ASSET_ID::EGG,
				 GEOMETRY_BUFFER_ID::DEBUG_LINE });

	registry.gameUIs.emplace(entity);

	// placeholder component
	registry.motions.emplace(entity);

	StackUI& stackui = registry.stackUI.emplace(entity);

	vec2 outlinePosOffset = { 10, -10 }; // temp for now

	stackui.bulletStartPos = vec2(75, 100) + outlinePosOffset;
	stackui.bulletSize = { 20, 50 };
	stackui.bulletOffset = 10; // space between bullets

	stackui.updateStackUISize(stack.baseStackSize);

	registry.gameUITexts.emplace(entity);
	auto& text = registry.textRenderRequests.emplace(entity);
	text.color = vec3(1, 1, 1);
	text.x = stackui.bulletStartPos.x - stackui.bulletSize.x - stackui.bulletOffset / 2;
	text.scale = 0.35;
	text.y = (stackui.bulletStartPos.y - windowState.height) * -1 - 2 * DEFAULT_FONT_SIZE * text.scale - stackui.bulletSize.y;
	// too lazy to calculate fitting text box size, and it prob won't overflow
	// so just set it to some big number
	text.topRightBound = { 1000, 1000 };
	text.bottomLeftBound = { 0, 0 };

	return entity;
}

Entity UISystem::createScreenCutIn() {
	Entity entity = Entity();

	auto& rr = registry.renderRequests.insert(
		entity,
		{ "eel.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.show = false;

	registry.screenCutIns.emplace(entity);

	WindowState& windowState = registry.windowStates.components[0];

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = { windowState.width / 2, windowState.height / 2 };
	motion.scale = { windowState.width, windowState.height};

	return entity;
}

Entity UISystem::createRoomClearMessage() {
	WindowState& windowState = registry.windowStates.components[0];
	auto entity = Entity();

	registry.dialogueUIs.emplace(entity);
	registry.dialogueUITexts.emplace(entity);

	auto& rr = registry.renderRequests.insert(
		entity, { "enemy_bullet_square.png",
				 EFFECT_ASSET_ID::TEXTURED,
				 GEOMETRY_BUFFER_ID::SPRITE,
				false});

	registry.colors.insert(entity, { 0,0,0 });
	
	TextRenderRequest& trr = registry.textRenderRequests.emplace(entity);
	float padding = 35.f;
	trr.text = "Room Cleared";
	trr.color = vec3(1.0f);
	trr.scale = 0.7f;
	trr.x = windowState.width/2.f;
	trr.y = windowState.height/2.f + windowState.height / 4.f - DEFAULT_FONT_SIZE * trr.scale / 2.f;
	trr.topRightBound = { windowState.width,windowState.height };
	trr.bottomLeftBound = { 0,0 };
	trr.alignment = TextAlignment::CenteredAlign;

	Motion& motion = registry.motions.emplace(entity);
	motion.position = vec2(windowState.width / 2, windowState.height / 2 - windowState.height / 4);
	motion.scale = vec2(trr.text.length() * DEFAULT_FONT_SIZE * trr.scale + padding * 2, 100);

	UIBorder& border = registry.uiBorders.emplace(entity);
	border.borderColour = COLOR_WHITE;
	border.borderThickness = 10.f;
	border.border = UIBorderType::Outlined;

	return entity;
}

Entity UISystem::createFpsCounter() {
	WindowState& windowState = registry.windowStates.components[0];
	auto entity = Entity();

	registry.menuOverlayUITexts.emplace(entity);
	auto& rr = registry.renderRequests.insert(
		entity, { "none",
				 EFFECT_ASSET_ID::EGG,
				 GEOMETRY_BUFFER_ID::DEBUG_LINE });

	TextRenderRequest& trr = registry.textRenderRequests.emplace(entity);
	float padding = 50.f;
	trr.text = "FPS: 0";
	trr.color = vec3(1.0f);
	trr.scale = 0.35f;
	trr.x = windowState.width - padding;
	trr.y = windowState.height - (trr.scale * DEFAULT_FONT_SIZE + padding / 2) * 3.f; //appear below room count
	trr.topRightBound = { windowState.width + 1000,windowState.height };
	trr.bottomLeftBound = { 0,0 };
	trr.alignment = TextAlignment::RightAlign;

	return entity;
}

Entity UISystem::createRoomCounter() {
	WindowState& windowState = registry.windowStates.components[0];
	auto entity = Entity();

	registry.gameUITexts.emplace(entity);
	auto& rr = registry.renderRequests.insert(
		entity, { "none",
				 EFFECT_ASSET_ID::EGG,
				 GEOMETRY_BUFFER_ID::DEBUG_LINE });

	TextRenderRequest& trr = registry.textRenderRequests.emplace(entity);
	vec2 dimensions = { 240.f,25.f };
	float padding = 50.f;
	trr.text = "Room 0";
	trr.color = vec3(1.0f);
	trr.scale = 0.35f;
	trr.x = windowState.width - padding;
	trr.y = windowState.height - (dimensions.y + padding);
	trr.topRightBound = { windowState.width + 1000,windowState.height };
	trr.bottomLeftBound = { 0,0 };
	trr.alignment = TextAlignment::RightAlign;

	return entity;
}

Entity UISystem::createTitleScreen() {
	Entity entity = Entity();

	WindowState& windowState = registry.windowStates.components[0];

	// copies code from draw line as a box for now
	auto& rr = registry.renderRequests.insert(
		entity,
		{ "title_concept.png",
		 EFFECT_ASSET_ID::TEXTURED,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.show = true;

	registry.menuUIs.emplace(entity);

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = vec2(windowState.width / 2, windowState.height / 2);
	motion.scale = vec2(windowState.width, windowState.height);

	// attach 1 text render request
	registry.menuUITexts.emplace(entity);
	auto& text = registry.textRenderRequests.emplace(entity);
	text.color = COLOR_YELLOW;
	text.topRightBound = { motion.scale.x, motion.scale.y };
	text.bottomLeftBound = { 0, 0 };
	text.text = "Stack Overflow";
	text.x = motion.position.x;
	text.y = windowState.height - 135;
	text.scale = 1.0;
	text.topRightBound = { motion.scale.x - 25, motion.scale.y - 25 };
	text.bottomLeftBound = { text.x, 0 + 25 };
	text.alignment = TextAlignment::CenteredAlign;
	
	Menu& menu = registry.menus.emplace(entity);
	menu.options = { "New Game", "Quit" };
	menu.startPos = { windowState.width / 2.f, windowState.height - 150 };
	menu.offset = { 0, 50 + 30 };
	menu.type = MenuType::TitleMenu;

	return entity;
}

void UISystem::loadText() {
	std::string uiType = "uiText";
	std::string filename = dialogue_path(uiType + ".txt").c_str();
	std::ifstream entity_file(filename);
	std::string uiName;
	std::vector<std::string> tokenizedText;

	if (entity_file.is_open())
	{
		std::string line;

		while (!entity_file.eof())
		{
			std::getline(entity_file, line);

			if (line.length() > 0 && line[0] != '#')
			{
				// split line different based on what first word is
				std::string action = line.substr(0, line.find_first_of(" "));


				if (action.compare("UI") == 0) {
					// new ui text, so place all prev lines into map, unless this is the first one
					if (tokenizedText.size() > 0) {
						uiTexts.insert({ uiName, tokenizedText });
						tokenizedText.clear();
					}

					std::stringstream ss_line(line);
					ss_line >> action >> uiName;
				}
				else { // this is just a body of text
					// need to manually add \n back into strings... use this until can think of better way
					//ref: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c?page=1&tab=scoredesc#tab-top
					std::string delim = "\\n";
					std::string uiTextBody = "";
					std::string newLine = "\n";
					auto start = 0U;
					auto end = line.find(delim);
					while (end != std::string::npos)
					{
						uiTextBody += line.substr(start, end - start) + '\n';
						start = end + delim.length();
						end = line.find(delim, start);
					}
					uiTextBody += line.substr(start, end);
					std::vector<std::string> newTokenizedText = getTokenizedText(uiTextBody);
					// ref: https://www.geeksforgeeks.org/concatenate-two-vectors-in-cpp/
					tokenizedText.insert(tokenizedText.end(), newTokenizedText.begin(), newTokenizedText.end());
					tokenizedText.push_back(newLine);
				}
			}
		}
		entity_file.close();
		uiTexts.insert({ uiName, tokenizedText });
	}
	else
	{
		std::cout << "ERROR: failed to open file: " << filename << std::endl;
	}
}

std::string UISystem::makeBulletTooltip(BulletStackEffect bullet) {
	std::string tooltip = bullet.name + "\n\n";
	std::string modify = "";
	std::string effect = "";
	std::string amount = "";
	float intermediaryAmount = 0;

	// special cases
	if (bullet.type == BulletEffectType::Inert) {
		tooltip += "This bullet doesn't do anything.";
	}
	else if (bullet.type == BulletEffectType::Key) {
		tooltip += "A keycard used for unlocking doors. Pops all subsequent bullets on the stack after use.";
	}
	else if (bullet.type == BulletEffectType::Lightning) {
		tooltip += "Shifts the bullets in the stack over by 1.";
	}
	else {
		// ordinary bullets
		// format: [increases/decreases] [effect] by [amount]

		if (bullet.effectCalc == Additive) {
			if (bullet.value < 0) {
				modify = "Decreases ";
				if (abs(bullet.value) - abs((int)bullet.value) > 0) {
					std::stringstream amountString;
					amountString << std::fixed << std::setprecision(2) << bullet.value << "s";
					amount = amountString.str();
				}
				else {
					amount = std::to_string(abs((int)bullet.value));
				}
				if (bullet.type == BulletEffectType::PlayerDashCDR) {
					intermediaryAmount = abs(bullet.value / 1000.f);
					std::stringstream amountString;
					amountString << std::fixed << std::setprecision(2) << intermediaryAmount << "s";
					amount = amountString.str();
				}
				if (bullet.type == BulletEffectType::Homing) {
					intermediaryAmount = bullet.value * 100;
					std::stringstream amountString;
					amountString << (int)intermediaryAmount << "%";
					amount = amountString.str();
				}
			}
			else {
				modify = "Increases ";
				if (bullet.value - (int)bullet.value > 0) {
					std::stringstream amountString;
					amountString << std::fixed << std::setprecision(2) << bullet.value << "s";
					amount = amountString.str();
				}
				else {
					amount = std::to_string(abs((int)bullet.value));
				}
				if (bullet.type == BulletEffectType::PlayerDashCDR) {
					intermediaryAmount = abs(bullet.value / 1000.f);

					std::stringstream amountString;
					amountString << std::fixed << std::setprecision(2) << intermediaryAmount << "s";
					amount = amountString.str();
				}
				if (bullet.type == BulletEffectType::Homing) {
					intermediaryAmount = bullet.value * 100;
					std::stringstream amountString;
					amountString << (int)intermediaryAmount << "%";
					amount = amountString.str();
				}
			}
		}
		else if (bullet.effectCalc == Multiplicative) {
			if (bullet.value < 0) {
				modify = "Decreases ";
				intermediaryAmount = (-bullet.value) * 100;
			}
			else {
				modify = "Increases ";
				intermediaryAmount = (bullet.value) * 100;
			}
			std::stringstream amountString;
			amountString << (int)intermediaryAmount << "%";
			amount = amountString.str();
		}

		switch (bullet.type) {
		case BulletDamage:
			effect = "the damage of bullets ";
			break;
		case ProjectileSpeed:
			effect = "bullet speed ";
			break;
		case ProjectileSize:
			effect = "the size of bullets ";
			break;
		case FireRate:
			effect = "bullet fire rate ";
			break;
		case BulletRange:
			effect = "bullet range ";
			break;
		case BulletSpread:
			effect = "the spread of bullets ";
			break;
		case BulletNum:
			effect = "the number of bullets shot at once ";
			break;
		case BulletBurst:
			effect = "the number of bullets shot in a burst shot ";
			break;
		case Bounce:
			effect = "the number of times bullets bounce ";
			break;
		case Pierce:
			effect = "the pierce of bullets ";
			break;
		case Homing:
			effect = "the homing accuracy of bullets ";
			break;
		case PlayerSpeed:
			effect = "movement speed ";
			break;
		case PlayerNumDash:
			effect = "the number of dashes ";
			break;
		case PlayerStackSize:
			effect = "stack size ";
			break;
		case PlayerDashCDR:
			effect = "dash cooldown ";
			break;
		default:
			effect = "This bullet is not in the list?? Report immediately!";
		}
		tooltip += modify + effect + "by " + amount + ".";
	}
	return tooltip;
}

void UISystem::loadBulletEffects() {
	for (BulletStackEffect bullet : premadeBullets) {
		std::string tooltip = makeBulletTooltip(bullet);
		std::vector<std::string> tokenizedTooltip;
		if (tooltip.length() > 0) {
			tokenizedTooltip = getTokenizedText(tooltip);
			uiTexts.insert({ "HoverBullet_" + bullet.name, tokenizedTooltip});
			//std::cout << tooltip << std::endl;
		}
	}
}

std::string UISystem::reportStats() {
	GameReport& report = registry.gameReports.components[0];
	std::stringstream reportString;
	reportString << "System Diagnostics Report\nRooms Cleared: " << report.roomsCleared << "\nSystem Runtime: " << std::fixed << std::setprecision(2) 
		<< (float)(std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - report.gameStartTime)).count() / 1000 / 1000.f << "s";
	return reportString.str();
}

