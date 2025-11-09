#include "ui_system.hpp"

#include "sound_system.hpp"
#include "text_system.hpp"
#include "ai_system.hpp"
#include "premades.hpp"
#include <glm/gtx/compatibility.hpp>
#include "utils/enum_string_mapping.hpp"
#include "utils/ui_constants.hpp"
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
	ws.numFramesThisSecond++;
	if (elapsed > 1000.0f) {
		ws.fps = ws.numFramesThisSecond;
		ws.numFramesThisSecond = 0;
		ws.currUnixTime = Clock::now();
		TextRenderRequest& fpsText = registry.textRenderRequests.get(fpsCounter);
		fpsText.text = "FPS: " + std::to_string(ws.fps);
	}

	// check: should game be paused right now?
	// if already paused, then close latest menu
	// if latest menu is the pause menu, then unpause
	if (ioState.pressedEsc) {
		if (!gameState.gamePaused) {
			gameState.gamePaused = true;
			gameState.previousVolume = gameState.currentVolume;
			gameState.currentVolume *= 0.125f;
			soundSystem->pausePersistentSounds();
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
				soundSystem->resumePersistentSounds();
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
					if (clickedButtonIndex == 1) {
						//std::cout << "show controls!" << std::endl;
						registry.activeMenus.emplace(registry.menus.entities[MenuType::ControlsMenu]);
						ioState.activeMenu++;
						registry.renderRequests.get(controlsGuide).show = true;
					}
					else if (clickedButtonIndex == 2) {
						ioState.shouldRestart = true;
						gameState.titleScreen = true;
						//std::cout << "to title!" << std::endl;
					}
					else if (clickedButtonIndex == 3) {
						ioState.shouldEnd = true;
						//std::cout << "quit!" << std::endl;
					}
					else {
						gameState.gamePaused = false;
						registry.renderRequests.get(pauseMenu).show = false;
					}

					if (clickedButtonIndex != 1) {
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

	// update bullet ui positions
	if (stack.currStack.size() > stackui.bulletPositions.size()) {
		int diff = stack.currStack.size() - stackui.bulletPositions.size();
		vec2 playerPos = registry.motions.get(registry.players.entities[0]).position;
		vec2 bulletStartPos = playerPos;
		for (int i = 0; i < diff; i++) {
			int index = i + stack.currStack.size() - diff;
			stackui.bulletPositions.push_back(vec2(stackui.bulletStartPos.x + index * stackui.bulletSize.x + index * stackui.bulletOffset,
				stackui.bulletStartPos.y));
		}
	}
	else if (stack.currStack.size() < stackui.bulletPositions.size()) {
		stackui.bulletPositions.resize(stack.currStack.size());
	}

	// update tier icons of player
	stackui.activeTiers.clear();
	vec2 offset = TIER_ICON_SCALE + TIER_ICON_OFFSET;
	vec2 startingPosition = vec2(50, 290) + vec2(TIER_ICON_SCALE.x / 2.f, 0);
	vec2 pos = startingPosition;
	for (auto& tier : stack.tierThresholds) {
		if (getEffectValue(tier.first) > 1) { // only display icon if have +1 or more towards tier
			// start icons on new row if overflow into pause menu
			if ((pos.x + TIER_ICON_SCALE.x) > (registry.motions.get(pauseMenu).position.x - registry.motions.get(pauseMenu).scale.x / 2)) {
				pos = startingPosition;
				pos.y += offset.y;
			}
			stackui.activeTiers[tier.first] = pos;
			pos.x += offset.x;
		}
	}

	// clean up stack add notifs that have faded out
	for (int i = registry.stackAddNotifs.size() - 1; i >= 0; i--) {
		Entity e = registry.stackAddNotifs.entities[i];
		if (!registry.renderRequests.get(e).show)
			registry.deleteEntityAndRelatedEntities(e);
	}

	// decide which statuses to be drawn on enemies
	// would be nice to use something like observer pattern, but do this for now
	for (Entity entity : registry.hpBarHavers.entities) {
		HPBarUI& hp = registry.hpBarHavers.get(entity);
		Motion& motion = registry.motions.get(entity);

		// configure what hpBar will look like (in terms of alpha)
		if (registry.invisibles.has(entity) || registry.deleteds.has(entity)) {
			hp.alpha = 0;
		}
		else if (registry.cloaks.has(entity))
		{
			//enemy gradually becomes invisible the further from the player, becomes fully invisible outside of cloak distance
			Cloaked& cloak = registry.cloaks.get(entity);
			Motion& playerMotion = registry.motions.get(registry.players.entities[0]);
			hp.alpha = glm::lerp(1.f, 0.f, (glm::distance(playerMotion.position, registry.motions.get(entity).position) - cloak.cloakingDistance) / cloak.cloakingDistance);
		}
		else {
			hp.alpha = 1;
		}
		
		// determine which statuses are active
		hp.activeStatuses[static_cast<int>(SpecialStates::INVINCIBLE)] = registry.invincibles.has(entity) - 1;
		hp.activeStatuses[static_cast<int>(SpecialStates::UNDERGROUND)] = registry.moles.has(entity) - 1;
		hp.activeStatuses[static_cast<int>(SpecialStates::VULNERABLE)] = (registry.vulnerabilities.has(entity) && registry.vulnerabilities.get(entity).modifier > 1.01) - 1;
		hp.activeStatuses[static_cast<int>(SpecialStates::PROTECTED)] = (registry.vulnerabilities.has(entity) && registry.vulnerabilities.get(entity).modifier < 0.99) - 1;
		hp.activeStatuses[static_cast<int>(SpecialStates::REGENERATING)] = registry.regenerates.has(entity) - 1;
		hp.activeStatuses[static_cast<int>(SpecialStates::ONFIRE)] = (registry.onFires.has(entity) && registry.onFires.get(entity).stack > 0) ? registry.onFires.get(entity).stack : -1;
		hp.activeStatuses[static_cast<int>(SpecialStates::HASTY)] = (registry.timeModifiers.has(entity) && registry.timeModifiers.get(entity).modifier == HASTY_TIME_MODIFIER) - 1;
		hp.activeStatuses[static_cast<int>(SpecialStates::SLUGGISH)] = (registry.timeModifiers.has(entity) && registry.timeModifiers.get(entity).modifier == SLUGGISH_TIME_MODIFIER) - 1;

		// configure position of hpbar
		WindowState& windowState = registry.windowStates.components[0];
		hp.scale = { 600, 30 };
		hp.position = { windowState.width / 2, windowState.height * 0.92 };

		if (!registry.bosses.has(entity))
		{
			hp.position = motion.position + vec2(0, motion.scale.y / 2 + 10);
			hp.scale = { 100, 10 };
		}
		else if (registry.bosses.entities[0] != entity) {
			hp.position = motion.position + vec2(0, motion.scale.y / 2 + 10 * 2.5);
			hp.scale = { 100 * 2.5, 10 * 2.5 };
		}

		// make hp bar wobble
		if (registry.damageds.has(entity) && !registry.invincibles.has(entity) && !registry.gameStates.components[0].gamePaused && !registry.gameStates.components[0].gameOver)
		{
			hp.position.x += (rand() % 10) - 5;
			hp.position.y += (rand() % 10) - 5;
		}

		// configure position of status icons
		hp.iconSize = STATUS_ICON_SCALE;
		hp.textSize = STATUS_TEXT_SCALE;
		vec2 offset = STATUS_ICON_OFFSET;
		float followCameraMultiplier = -1;

		// Boss hp bars on fixed on screen and are bigger
		if (!hp.followCamera) {
			followCameraMultiplier = 1;
			hp.iconSize *= STATUS_ICON_BOSS_MULTIPLIER;
			offset *= STATUS_ICON_BOSS_MULTIPLIER;
			hp.textSize *= STATUS_TEXT_BOSS_MULTIPLIER;
		}

		vec2 iconPos = hp.position - vec2(hp.scale.x / 2.f, followCameraMultiplier * (hp.scale.y + hp.iconSize.y / 2.f));
		// account for icon size
		iconPos.x += hp.iconSize.x / 2.f;
		iconPos.y -= hp.iconSize.y / 2.f * followCameraMultiplier;

		offset.x += hp.iconSize.x;
		vec2 startingPos = iconPos;

		for (int i = 0; i < hp.activeStatuses.size(); i++) {
			if (hp.activeStatuses[i] >= 0) {
				if ((iconPos.x - hp.iconSize.x) > hp.position.x + hp.scale.x / 2) {
					iconPos.x = startingPos.x;
					iconPos.y -= (hp.iconSize.y + offset.y) * followCameraMultiplier;
				}
				hp.statusPositions[i] = iconPos;
				iconPos.x += offset.x;
			}
		}
	}

	// handle ui requests
	for (UIRequest& uiRequest : registry.uiRequests.components) {
		if (uiRequest.type == UIRequestType::CallNotif
			|| uiRequest.type == UIRequestType::StackNotifBullet) {

			// clean up previous stack add notifs
			for (int i = registry.stackAddNotifs.size() - 1; i >= 0; i--) {
				Entity e = registry.stackAddNotifs.entities[i];
				registry.deleteEntityAndRelatedEntities(e);
			}

			if (!registry.motions.has(uiRequest.targetEntity)) {
				// if target entity is not present, then assume the request is for the player
				uiRequest.targetEntity = registry.players.entities[0];
			}

			vec2 position = registry.motions.get(uiRequest.targetEntity).position;
			registry.renderRequests.get(stackAddBubble).show = true;
			registry.renderRequests.get(stackAddTail).show = true;

			if (uiRequest.effects.size() > 0) {
				updateStackAddBubble(position, { -1, 1 }, uiRequest.effects.size(), stackAddBubble, stackAddTail);
			}
			else {
				updateStackAddBubble(position, { -1, 1 }, 1, stackAddBubble, stackAddTail);
			}

			if (!registry.showTimers.has(stackAddBubble)) {
				if (registry.fades.has(stackAddBubble)) {
					registry.fades.remove(stackAddBubble);
					registry.fades.remove(stackAddTail);
				}
				registry.showTimers.emplace(stackAddBubble);
				registry.showTimers.emplace(stackAddTail);
			}

			vec2 bulletStartPos = position;

			// first merge effects
			uiRequest.effects = mergeEffects(uiRequest.effects);

			// note: shuffles will end up here too, so need to catch them beforehand
			if (uiRequest.effects.size() > 0 && uiRequest.type == UIRequestType::StackNotifBullet) {
				for (int index = 0; index < uiRequest.effects.size(); index++) {
					std::string bulletSprite = "";
					vec3 bulletColor = vec3(1);
					std::string bulletMsg = "";
					std::string effectStr = "";

					if (uiRequest.effects[index].type == BulletEffectType::Lightning) {
						if (uiRequest.effects[index].value == lightningShuffle.value) { // shuffle
							bulletSprite = "stackNotifShuffle.png";
							bulletMsg = "Shuffled stack";
							bulletColor = COLOR_YELLOW;
							effectStr = "shuffled";
						}
						else {
							bulletSprite = "stackNotifShift.png";
							bulletMsg = "Shifted stack by 1";
							bulletColor = COLOR_TURQUOISE;
							effectStr = "shifted";
						}

						createStackAddNotif(vec2(bulletStartPos.x, bulletStartPos.y), vec2(192) / 2.5f, bulletSprite, vec3(1), BulletStackEffect());
					}
					else if (uiRequest.effects[index].type == BulletEffectType::Eat) {
						//TODO MAKE A NOTIFICATION FOR EAT
					}
					else if (uiRequest.effects[index].type == BulletEffectType::Knock) {
						//TODO MAKE A NOTIFICATION FOR EAT
					}

					else {
						bulletSprite = bulletEffectShapes.at(uiRequest.effects[index].type);
						bulletColor = bulletEffectColors.at(uiRequest.effects[index].type);
						effectStr = getFormattedBulletEffectString(uiRequest.effects[index]);
						bulletMsg = effectStr + " added onto the stack";

						createStackAddNotif(vec2(bulletStartPos.x + index * stackui.bulletSize.x + index * stackui.bulletOffset, bulletStartPos.y),
							registry.stackUI.components[0].bulletSize* STACK_NOTIF_SCALE,
							bulletSprite,
							bulletColor,
							uiRequest.effects[index]);
					}

					Entity notif = createNotifMessage(bulletMsg);

					// highlight bullet effect name
					// do a special check for keys, but can alternatively make all non-bullet items share a colour (teal?)
					if (uiRequest.effects[index].type == Key) {
						registry.textRenderRequests.get(notif).decorations.push_back(
							TextDecorationSpan{ 0, uiRequest.effects[index].name.length(), COLOR_YELLOW });
					}
					else {
						registry.textRenderRequests.get(notif).decorations.push_back(
							TextDecorationSpan{ 0, effectStr.length(), bulletColor });
					}
				}
			}
			else {
				std::string sprite = "stackNotifShift.png";
				if (uiRequest.type == UIRequestType::CallNotif) {
					sprite = "callNotif.png";
				}

				createStackAddNotif(vec2(bulletStartPos.x, bulletStartPos.y), vec2(192) / 2.5f, sprite, vec3(1), BulletStackEffect());
			}
		}

			if (uiRequest.type == UIRequestType::ResetUI) {
				DialogueLines& lines = registry.dialogueLines.components[0];
				lines = DialogueLines();
				DrawingText& drawingText = registry.drawingTexts.get(dialogueBox);
				drawingText = DrawingText();

				for (int i = registry.dialogueChoices.size() - 1; i >= 0; i--) {
					Entity e = registry.dialogueChoices.entities[i];
					registry.deleteEntityAndRelatedEntities(e);
				}
				for (int i = registry.menuChoices.size() - 1; i >= 0; i--) {
					Entity e = registry.menuChoices.entities[i];
					registry.deleteEntityAndRelatedEntities(e);
				}
				for (int i = registry.notifMessages.size() - 1; i >= 0; i--) {
					Entity e = registry.notifMessages.entities[i];
					registry.deleteEntityAndRelatedEntities(e);
				}

				registry.activeMenus.clear();
				ioState.activeMenu = -1;
				registry.renderRequests.get(stackAddBubble).show = false;
				registry.renderRequests.get(stackAddTail).show = false;
				registry.renderRequests.get(controlsGuide).show = false;
				registry.renderRequests.get(flashMessageDisplay).show = false;
				registry.textRenderRequests.get(roomName).decorations.clear();
				registry.textRenderRequests.get(roomCounter).decorations.clear();
			}

			if (uiRequest.type == UIRequestType::GameOverReport) {
				//std::cout << "Game over report!" << std::endl;
				TextRenderRequest& text = registry.textRenderRequests.get(gameOverMenu);
				std::string report = "\n" + reportStats();
				std::vector<std::string> reportTokenized = getTokenizedText(report);
				text.formattedText = uiTexts["GameOver"].tokenizedText;
				text.formattedText.insert(text.formattedText.end(), reportTokenized.begin(), reportTokenized.end());
				text.formattedText = getFormattedText(text.formattedText, text.scale, text.alignment, { text.x, text.y }, text.topRightBound, text.bottomLeftBound);
				registry.activeMenus.emplace_with_duplicates(registry.menus.entities[MenuType::GameOverMenu]);
				ioState.activeMenu++;
			}

			if (uiRequest.type == UIRequestType::DisplayFlashMessage) {
				updateFlashMessageDisplay(uiRequest.text);
				// lazy -- only play for room cleared. Should either use a separate req or at least a string constant
				if (uiRequest.text.compare("Room Cleared") == 0) {
					soundSystem->playRareItemPickupSound();
				}
				else if (uiRequest.text == "Boss Defeated") {
					soundSystem->playFanFareSound();
				}
			}
	}

		registry.uiRequests.clear();

		// move position of bullet add notif
		if (registry.stackAddNotifs.entities.size() > 0) {
			vec2 playerPos = registry.motions.get(registry.players.entities[0]).position;
			vec2 bulletStartPos = playerPos + abs(registry.motions.get(registry.players.entities[0]).scale) * vec2(1, -1);
			updateStackAddBubble(bulletStartPos, { -1, 1 }, registry.stackAddNotifs.entities.size(), stackAddBubble, stackAddTail);
			int index = 0;
			for (Entity entity : registry.stackAddNotifs.entities) {
				registry.motions.get(entity).position = vec2(bulletStartPos.x + index * stackui.bulletSize.x * STACK_NOTIF_SCALE + index * stackui.bulletOffset * STACK_NOTIF_SCALE, bulletStartPos.y);
				index++;
			}
			if (registry.showTimers.has(registry.stackAddNotifs.entities[0])) {
				registry.showTimers.get(stackAddBubble).timer = registry.showTimers.get(registry.stackAddNotifs.entities[0]).timer;
				registry.showTimers.get(stackAddTail).timer = registry.showTimers.get(registry.stackAddNotifs.entities[0]).timer;
			}
		}

		// move positions of notif messages
		if (registry.notifMessages.size() > 0) {
			// clean up notifs that have faded out
			for (int i = 0; i < registry.notifMessages.size(); i++) {
				Entity e = registry.notifMessages.entities[i];
				if (!registry.renderRequests.get(e).show) {
					registry.deleteEntityAndRelatedEntities(e);
				}
			}

			vec2 startingPosition = vec2(50, ws.height);
			for (int i = registry.notifMessages.size() - 1; i >= 0; i--) {
				Entity messageEntity = registry.notifMessages.entities[i];
				TextRenderRequest& text = registry.textRenderRequests.get(messageEntity);
				startingPosition.y -= (text.formattedText.size()) * 50;
				text.x = startingPosition.x;
				text.y = ws.height - startingPosition.y - text.scale * DEFAULT_FONT_SIZE / 2.f;
			}
		}

		if (gameState.gamePaused || gameState.dialogueScene || registry.maps.components[0].currRoom.cleared || gameState.gameOver) {
			// is the player hovering over a stack ui bullet right now?
			// may optimize using some other method like colour picking/just limiting search size
			// in the future (since search space is pretty deterministic)
			if (!hoverBulletStack(ioState, stackui, stack) && !hoverBossStatus(ioState) && !hoverTierStatus(ioState, stackui)) {
				registry.renderRequests.get(bulletUI).show = false;
				registry.renderRequests.get(bulletUIArrow).show = false;
			}

			// Temp fix: because door gauges are deleted everyframe
			for (Entity gaugeEntity : registry.uiGauges.entities) {
				registry.deleteds.emplace(gaugeEntity);
			}

			// update guages of all interactables whose timers have gone off
			for (Entity interactEntity : registry.interactables.entities) {
				InteractableObject& interactable = registry.interactables.get(interactEntity);
				if (interactable.timer < interactable.base) {
					// if don't already have gauge drawn, then make gauge ui
					// else, just update timer
					// dumb method: do like interact indicators, and just clear all prev frames
					createInteractGauge(registry.motions.get(interactEntity).position, interactable.timer, interactable.base);
				}
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
			registry.textRenderRequests.get(stackUI).text = "STACK: " + std::to_string(stack.currStack.size()) + " / " + std::to_string((int)getModifiedValue(PlayerStackSize, stack.baseStackSize));

			Map& map = registry.maps.components[0];
			TextRenderRequest& roomCounterText = registry.textRenderRequests.get(roomCounter);
			std::string region;
			if (map.currRegion == Tutorial) region = "Tutorial";
			if (map.currRegion == Biology) region = "Biology";
			if (map.currRegion == Physics) region = "Physics";
			roomCounterText.text = region + " Room " + std::to_string(map.roomsTraversed);
			auto& roomNameTextComponent = registry.textRenderRequests.get(roomName);
			auto& roomNameText = roomNameTextComponent.text;
			roomNameText = map.currRoom.preset.ID;
			if (map.currRoom.type == EnemyRoom || map.currRoom.type == BossRoom) {
				if (map.currRoom.cleared) {
					roomNameText += ", Cleared";
					if (!roomNameTextComponent.decorations.empty())
						roomNameTextComponent.decorations.clear();
					if (!roomCounterText.decorations.empty())
						roomCounterText.decorations.clear();
				}
				else if (map.currRoom.spawnedElite) {
					roomNameText += ", Wave ???";
					if (roomNameTextComponent.decorations.empty()) {
						roomNameTextComponent.decorations.push_back(TextDecorationSpan{ 0, roomNameText.length() - 1, COLOR_RED, WobblyText, std::make_shared<WobblyTextAnimation>(std::vector<float> { 12, 12 }) });
						roomCounterText.decorations.push_back(TextDecorationSpan{ 0, roomCounterText.text.length() - 1, COLOR_RED, WobblyText, std::make_shared<WobblyTextAnimation>(std::vector<float> { 12, 12 }) });
					}
				}
				else {
					roomNameText += ", Wave " + std::to_string(map.currRoom.currentWave);
					if (!roomNameTextComponent.decorations.empty())
						roomNameTextComponent.decorations.clear();
				}
			}

			// clear prev frame's e indicators
			for (Entity entity : registry.interactIndicators.entities) {
				if (!registry.deleteds.has(entity)) {
					registry.deleteds.emplace(entity);
				}
			}

			// TEMP: hide previous frame's door preview
			registry.renderRequests.get(roomPreviewBubble).show = false;
			registry.renderRequests.get(roomPreviewTail).show = false;

			for (Entity entity : registry.roomPreviewBullets.entities) {
				if (!registry.deleteds.has(entity)) {
					registry.deleteds.emplace(entity);
				}
			}

			if (!gameState.dialogueScene && !gameState.cutScene && !gameState.gamePaused) { // normal game uis
				registry.renderRequests.get(dialogueAvatar).show = false;
				registry.renderRequests.get(screenCutIn).show = false;
				// draw "press e to interact" over all items in nearby interactables list
				for (Entity entity : registry.nearbyInteractables.entities) {
					if (registry.doors.has(entity)) { // doors should show room bullet previews instead
						Door& door = registry.doors.get(entity);

						// TODO: only show for enemy rooms, or other rooms too?
						if (door.isPrev || (door.room != RoomType::EnemyRoom && door.room != RoomType::BossRoom)) {
							continue;
						}

						registry.renderRequests.get(roomPreviewBubble).show = true;
						registry.renderRequests.get(roomPreviewTail).show = true;

						Motion& doorMotion = registry.motions.get(entity);

						std::vector<BulletStackEffect> positiveEffects = extractBulletEffects(door.preset.positiveEffects);
						std::vector<BulletStackEffect> negativeEffects = extractBulletEffects(door.preset.negativeEffects);

						// TEMP: figure out collapsing later
						std::vector<BulletStackEffect> allEffects = positiveEffects;
						allEffects.insert(allEffects.end(), negativeEffects.begin(), negativeEffects.end());

						vec2 doorDirection;
						vec2 doorOffset;
						switch (door.side) {
						case 'B':
							doorDirection = { 0, -1 };
							doorOffset = { -doorMotion.scale.x / 4, 50 };
							break;
						case 'T':
							doorDirection = { 0, 1 };
							doorOffset = { -doorMotion.scale.x / 4, -50 };
							break;
						case 'L':
							doorDirection = { -1, 1 };
							doorOffset = { 50, 0 };
							break;
						case 'R':
							doorDirection = { 1, 1 };
							doorOffset = { -100, 0 };
							break;
						}

						int numBullets = allEffects.size();
						vec2 bulletStartPos = doorMotion.position + doorOffset;

						// draw bullets here
						for (int index = 0; index < allEffects.size(); index++) {
							BulletStackEffect effect = allEffects[index];
							std::string bulletSprite = bulletEffectShapes.at(effect.type);
							vec3 bulletColor = bulletEffectColors.at(effect.type);

							Entity bullet = createUIBullet(vec2(bulletStartPos.x + index * stackui.bulletSize.x + index * stackui.bulletOffset, bulletStartPos.y),
								registry.stackUI.components[0].bulletSize* STACK_NOTIF_SCALE,
								bulletSprite,
								bulletColor,
								effect);

							registry.roomPreviewBullets.emplace(bullet);
						}

						updateStackAddBubble(bulletStartPos, doorDirection, numBullets, roomPreviewBubble, roomPreviewTail);

					} else {
						createInteractIndicator(registry.motions.get(entity).position);
					}
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
	
	// REMOVE LATER
	// put placeholder text for any tiers w/o descriptions
	for (auto& tier : bulletEffectColors) {
		if ((bulletEffectTypeNames.count(tier.first) > 0) && uiTexts.count("Tier_" + bulletEffectTypeNames.at(tier.first)) == 0) {
			std::cout<<bulletEffectTypeNames.at(tier.first)<<std::endl;
			uiTexts["Tier_" + bulletEffectTypeNames.at(tier.first)] = {"Does nothing for now!", getTokenizedText("Does nothing for now!") , {}};
		}
	}

	stackUI = createStackUI(wS, registry.stackCompile.components[0]);
	dialogueBox = createDialogueBox(vec2(wS.width / 2, wS.height - wS.height / 8), vec2(wS.width, wS.height / 3.5));
	dialogueAvatar = createDialogueAvatar(vec2(150, wS.height - wS.height / 8 - 25), vec2(wS.height / 4 - 100, wS.height / 4 - 100));
	screenCutIn = createScreenCutIn();
	bulletUI = createBulletUI();
	bulletUIArrow = createBulletUIArrow();
	fpsCounter = createFpsCounter();
	roomCounter = createRoomCounter();
	roomName = createRoomName();
	titleScreen = createTitleScreen();
	pauseMenu = createPauseMenu(vec2(wS.width / 2, wS.height / 2), vec2(wS.width / 4, wS.height - 400.f));
	controlsGuide = createControlsGuide(vec2(wS.width / 2, wS.height / 2), vec2(wS.width / 3, wS.height - 200.f));
	gameOverMenu = createGameOverMenu(vec2(wS.width / 2, wS.height / 2), vec2(wS.width / 2.5, wS.height - 200.f));
	stackAddBubble = createStackAddBubble();
	stackAddTail = createStackAddTail();
	roomPreviewBubble = createStackAddBubble();
	roomPreviewTail = createStackAddTail();
	dialogueReminder = createDialogueReminder();
	flashMessageDisplay = createFlashMessageDisplay();

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
			TextRenderRequest& text = registry.textRenderRequests.get(dialogueBox);
			text.text = nextLine.text;
			text.decorations = nextLine.decorations;

			// TODO: remove later
			text.y = wS.height - registry.motions.get(dialogueBox).position.y + registry.motions.get(dialogueBox).scale.y / 2 - DEFAULT_FONT_SIZE * 1.5;

			// check if any script variables need to be bound
			if (registry.interactableInDialogue.entities.size() > 0) {
				InteractableObject& currItem = registry.interactables.get(registry.interactableInDialogue.entities[0]);
				if (currItem.scriptVariables.size() > 0) {
					bindScriptVariables(text, currItem.scriptVariables, currItem.decorations);
				}

				// TODO: remove later
				if (currItem.name.compare("SkipTutorial") == 0) {
					text.y = wS.height - 100;
				}
			}

			text.formattedText = getFormattedText(getTokenizedText(text.text), text.scale, text.alignment, { text.x, text.y }, text.topRightBound, text.bottomLeftBound);

			// play a sound if there is one
			switch (nextLine.sfx) {
				case SoundType::IncomingDialogue:
					soundSystem->playIncomingDialogueSound();
					break;
				case SoundType::DoorOpen:
					soundSystem->playDoorOpenSound();
					break;
				case SoundType::DoorClose:
					soundSystem->playDoorCloseSound();
					break;
				case SoundType::itemPickup:
					soundSystem->playItemPickupSound();
					break;
				case SoundType::rareItemPickup:
					soundSystem->playRareItemPickupSound();
					break;
				case SoundType::explosion:
					soundSystem->playExplosionSound(2);
					break;
				case SoundType::AlarmSound:
					soundSystem->playAlarmSound();
					break;
				case SoundType::FanFare:
					soundSystem->playFanFareSound();
					break;
				case SoundType::EnemyShoot:
					soundSystem->playEnemyShootSound(0, 0);
					break;
				case SoundType::EnemyDeathSound:
					soundSystem->playEnemyDeathSound(Random::Int(4));
					break;
				case SoundType::PlayerShoot:
					soundSystem->playPlayerShootSound(200);
					break;
				case SoundType::PlayerDodgeSound:
					soundSystem->playPlayerDodgeSound(Random::Int(2));
					break;
				case SoundType::PlayerHurt:
					soundSystem->playPlayerHurtSound();
					break;
				case SoundType::PlayerZapped:
					soundSystem->playPlayerZappedSound();
					break;
				case SoundType::LaserSound:
					soundSystem->playLaserSound(1000);
					break;
				case SoundType::DiggingSound:
					soundSystem-> playDiggingSound(2000);
					break;
				case SoundType::DashSound:
					soundSystem->playPlayerDashSound();
					break;
				default:
					soundSystem->playNextDialogueSound();
					break;
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

			// can draw choices right under dialogue
			float startingY = wS.height - text.y + text.scale * DEFAULT_FONT_SIZE * text.formattedText.size() * 2 + DEFAULT_FONT_SIZE * text.scale * 0.5;
			if (text.formattedText.size() > 0 && text.formattedText[0].compare(" ") == 0) {
				startingY = wS.height - text.y - text.scale * DEFAULT_FONT_SIZE;
			}

			vec2 nextPosition = vec2(text.x, startingY);
			// first create all choices
			for (int i = 0; i < nextLine.choices.size(); i++) {
				Entity choice = createDialogueChoice(nextLine.choices[i], nextPosition);
			}
			
			// then readjust positioning here to allow for multiline choice formatting
			for (Entity dialogueChoice : registry.dialogueChoices.entities) {
				TextRenderRequest& choiceText = registry.textRenderRequests.get(dialogueChoice);
				choiceText.y = wS.height - nextPosition.y;
				registry.motions.get(dialogueChoice).position = nextPosition - choiceText.scale * DEFAULT_FONT_SIZE * 0.5f;
				nextPosition.y += (choiceText.formattedText.size()) * 50;
			}

			// set first choice to highlighted by default
			if (registry.dialogueChoices.components.size() > 0) {
				input.hoveringDialogueChoice = 0;
				input.lastHoverDialogueChoice = 0;

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
			// reset timer for animated text
			DrawingText& drawingText = registry.drawingTexts.get(dialogueBox);
			drawingText = DrawingText();
		}
		// no more lines of dialogue
		else {
			registry.renderRequests.get(dialogueBox).show = false;
			registry.renderRequests.get(dialogueAvatar).show = false;
			Map& map = registry.maps.components[0];
			map.currRoom.dialogueDone = true;
			gameState.dialogueScene = false;
			soundSystem->stopNextDialogueSound();
			input.pressedHorizontal = ExtendedStack<int>();
			input.pressedVertical = ExtendedStack<int>();
			input.lastInputAxis = vec2(0);
			input.inputAxis = vec2(0);
			registry.motions.get(registry.players.entities[0]).velocity = vec2(0);
		}
	}
}

Entity UISystem::createNotifMessage(std::string message) {
	Entity entity = Entity();
	WindowState& windowState = registry.windowStates.components[0];

	auto& rr = registry.renderRequests.insert(
		entity, { "none",
				 EFFECT_ASSET_ID::EGG,
				 GEOMETRY_BUFFER_ID::DEBUG_LINE });

	registry.gameUIs.emplace(entity);
	registry.gameUITexts.emplace(entity);

	//Motion& motion = registry.motions.emplace(entity);
	//motion.angle = 0.f;
	//motion.velocity = { 0, 0 };
	//motion.position = vec2(0);

	auto& text = registry.textRenderRequests.emplace(entity);
	text.color = vec3(1, 1, 1);
	text.scale = 0.40;
	text.text = message;
	text.topRightBound = { 700, windowState.height }; // need to update this
	text.bottomLeftBound = { 0, 0 };
	text.x = 50;
	text.y = 50;
	text.formattedText = getFormattedText(getTokenizedText(message), text.scale, text.alignment, {text.x, text.y}, text.topRightBound, text.bottomLeftBound);

	vec3& color = registry.colors.emplace(entity);
	color = vec3(0.f);

	UIBorder& border = registry.uiBorders.emplace(entity);
	border.borderThickness = 5.f;
	border.borderColour = COLOR_WHITE;
	border.border = UIBorderType::Outlined;

	ShowTimer& showtimer = registry.showTimers.emplace(entity);
	showtimer.base = 3000;
	showtimer.timer = showtimer.base;

	registry.notifMessages.emplace(entity);

	return entity;
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

Entity UISystem::createStackAddNotif(vec2 position, vec2 scale, std::string sprite, vec3 c, BulletStackEffect effect) {
	Entity entity = createUIBullet(position, scale, sprite, c, effect);
	
	registry.showTimers.emplace(entity);
	registry.stackAddNotifs.emplace(entity);

	return entity;
}

Entity UISystem::createUIBullet(vec2 position, vec2 scale, std::string sprite, vec3 c, BulletStackEffect effect) {
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

	if (effect.value != 0) {
		rr.used_effect = EFFECT_ASSET_ID::BULLET;
		EnemyBullet& bullet = registry.enemyBullets.emplace(entity);
		bullet.bulletEffects = { effect };
	}

	registry.gameUIs.emplace(entity);

	vec3& color = registry.colors.emplace(entity);
	color = c;

	return entity;
}

void UISystem::updateStackAddBubble(vec2 position, vec2 tailDirection, int bulletNum, Entity bubbleEntity, Entity tailEntity) {
	Motion& motion = registry.motions.get(bubbleEntity);
	vec2 bulletSize = registry.stackUI.components[0].bulletSize;
	float bulletOffset = registry.stackUI.components[0].bulletOffset;
	vec2 bulletStartPos = position;
	motion.scale = vec2(bulletNum * bulletSize.x * STACK_NOTIF_SCALE + bulletNum * bulletOffset * STACK_NOTIF_SCALE + 2 * bulletOffset * STACK_NOTIF_SCALE,
		bulletSize.y * STACK_NOTIF_SCALE + 2 * bulletOffset * STACK_NOTIF_SCALE);
	motion.position = vec2(bulletStartPos.x + motion.scale.x / 2 - bulletSize.x * STACK_NOTIF_SCALE - bulletOffset * STACK_NOTIF_SCALE / 2, bulletStartPos.y);
	motion.scale += vec2(10.f, 0);

	Motion& tailMotion = registry.motions.get(tailEntity);

	tailMotion.position = motion.position + motion.scale * vec2(0.5 * tailDirection.x, 0.5 * tailDirection.y) 
		+ tailMotion.scale / 2.f * vec2(0.5 * tailDirection.x, 0.5 * tailDirection.y);

	if (tailDirection.x < 0 && tailDirection.y > 0) { // bottom left
		tailMotion.angle = M_PI - M_PI / 4.f;
	}
	else if (tailDirection.x == 0 && tailDirection.y > 0) { // direct top
		tailMotion.angle = M_PI / 2;
		tailMotion.position.y += tailMotion.scale.y / 2;
	}
	else if (tailDirection.x == 0 && tailDirection.y < 0) { // direct bottom
		tailMotion.angle = -M_PI / 2;
		tailMotion.position.y -= tailMotion.scale.y / 2;
	}
	else { // used to catch bottom right case
		tailMotion.angle = M_PI / 4.f;
	}
}

void UISystem::updateStatusUI(vec2 position, SpecialStates status, Entity& enemy) {
	Motion& motion = registry.motions.get(bulletUI);
	WindowState& windowState = registry.windowStates.components[0];
	motion.position = position;
	if ((motion.position.x - motion.scale.x / 2) < 0 + 25) {
		motion.position.x += (motion.position.x - motion.scale.x / 2) * -1 + 25;
	}
	registry.renderRequests.get(bulletUI).show = true;

	TextRenderRequest& textReq = registry.textRenderRequests.get(bulletUI);

	textReq.x = motion.position.x - motion.scale.x / 2 + 20;
	textReq.bottomLeftBound = { textReq.x, 0 };
	textReq.topRightBound = { textReq.x + motion.scale.x - 25, 10000 };
	textReq.decorations.clear();
	vec3 color = specialStateUIColors.at(status);

	std::string name = specialStateNames.at(status);
	name += "\n\n";
	textReq.decorations = uiTexts.at("Status_" + specialStateNames.at(status)).decorations;

	// need to correct any decorations from the original text...
	for (TextDecorationSpan& deco : textReq.decorations) {
		deco.startIndex += name.length();
		deco.endIndex += name.length();
	}

	textReq.decorations.push_back(TextDecorationSpan{ 0, name.length() - 3, color });
	textReq.text = name + uiTexts.at("Status_" + specialStateNames.at(status)).text;

	std::vector<std::string> variables;
	std::vector<std::vector<TextDecorationSpan>> variableDecos;
	textReq.text += "\n\n";
	std::string val = "";

	// better way to do this with a map?
	switch (status) {
	case SpecialStates::INVINCIBLE: {
		textReq.text += "Time remaining: ";
		val = getTruncatedDecimal(registry.invincibles.get(enemy).countdown / 1000) + "s";
		break;
	}
	case SpecialStates::VULNERABLE: {
		textReq.text += "Time remaining: ";
		val = getTruncatedDecimal(registry.vulnerabilities.get(enemy).countdown / 1000) + "s";
		break;
	}
	case SpecialStates::PROTECTED: {
		textReq.text += "Time remaining: ";
		val = getTruncatedDecimal(registry.vulnerabilities.get(enemy).countdown / 1000) + "s";
		break;
	}
	case SpecialStates::UNDERGROUND: {
		textReq.text += "Time remaining: ";
		val = getTruncatedDecimal(registry.moles.get(enemy).countdown / 1000) + "s";
		break;
	}
	case SpecialStates::ONFIRE: {
		std::string var1 = getTruncatedDecimal(registry.onFires.get(enemy).maxCountdown / 1000) + "s";
		variables.push_back(var1);
		textReq.text += "Stacks: ";
		val = std::to_string(registry.onFires.get(enemy).stack);
		break;
	}
	case SpecialStates::REGENERATING: {
		std::string var1 = std::to_string(registry.regenerates.get(enemy).healAmount);
		variables.push_back(var1);
		std::string var2 = getTruncatedDecimal(registry.regenerates.get(enemy).healInterval / 1000) + "s";
		variables.push_back(var2);
		textReq.text += "Time remaining: ";
		val =getTruncatedDecimal(registry.regenerates.get(enemy).countdown / 1000) + "s";
		break;
	}

	default:
		printf("\nUnknown status encountered: %d", (int)status);
	}

	textReq.decorations.push_back({ textReq.text.length(), textReq.text.length() + val.length(), color });
	textReq.text += val;

	bindScriptVariables(textReq, variables, variableDecos);

	// get formatted text based on bounds
	std::vector<std::string> formatted =
		getFormattedText(getTokenizedText(textReq.text), textReq.scale, textReq.alignment, { textReq.x, textReq.y }, textReq.topRightBound, textReq.bottomLeftBound);
	textReq.formattedText = formatted;

	// scale box vertically to number of lines
	// need to flip this for statuses
	motion.scale.y = textReq.formattedText.size() * 50;
	motion.position.y = position.y - ( motion.scale.y / 2 + 50 + 10);
	textReq.y = windowState.height - motion.position.y + motion.scale.y / 2 - 50;

	// need to flip this for statuses too
	Motion& arrowMotion = registry.motions.get(bulletUIArrow);
	arrowMotion.position = { position.x, motion.position.y + motion.scale.y / 2 + 10};
	arrowMotion.scale.x = abs(arrowMotion.scale.x);
	registry.renderRequests.get(bulletUIArrow).show = true;
}

// really just borrows the bulletUI tooltip box
void UISystem::updateTierUI(vec2 position, BulletEffectType tier) {
	Motion& motion = registry.motions.get(bulletUI);
	WindowState& windowState = registry.windowStates.components[0];
	motion.position = position;
	if ((motion.position.x - motion.scale.x / 2) < 0 + 25) {
		motion.position.x += (motion.position.x - motion.scale.x / 2) * -1 + 25;
	}
	registry.renderRequests.get(bulletUI).show = true;

	TextRenderRequest& textReq = registry.textRenderRequests.get(bulletUI);

	textReq.x = motion.position.x - motion.scale.x / 2 + 20;
	textReq.bottomLeftBound = { textReq.x, 0 };
	textReq.topRightBound = { textReq.x + motion.scale.x - 25, 10000 };
	textReq.decorations.clear();
	vec3 color = bulletEffectColors.at(tier);

	Motion& arrowMotion = registry.motions.get(bulletUIArrow);
	arrowMotion.position = { position.x, position.y + 51 };
	registry.renderRequests.get(bulletUIArrow).show = true;
	arrowMotion.scale.x = abs(arrowMotion.scale.x) * -1;

	std::string name = tierNames.at(tier);
	name += " (" + std::to_string(getEffectValue(tier)) + "/" + std::to_string(getEffectTierThreshold(tier)) + ")";
	name += "\n\n";

	textReq.decorations = uiTexts.at("Tier_" + bulletEffectTypeNames.at(tier)).decorations;

	// need to correct any decorations from the original text...
	for (TextDecorationSpan& deco : textReq.decorations) {
		deco.startIndex += name.length();
		deco.endIndex += name.length();
	}

	// note: -3 because -1 to end on last character, then another -2 for the two new lines we added (note a new empty row doesn't count as a character)
	textReq.decorations.push_back(TextDecorationSpan{ 0, name.length() - 3, color });
	textReq.text = name + uiTexts.at("Tier_" + bulletEffectTypeNames.at(tier)).text;

	std::vector<std::string> variables;
	std::vector<std::vector<TextDecorationSpan>> variableDecos;
	std::string val;
	if (tier == BulletEffectType::BulletNum) {
		val = std::to_string(4 * (1 + getEffectValue(tier) - getEffectTierThreshold(tier)));
		variables.push_back(val);
		variableDecos.push_back({ { 0, val.length(), bulletEffectColors.at(tier)} });
	}
	if (tier == BulletEffectType::BulletDamage) {
		val = std::to_string(15 * (1 + getEffectValue(tier) - getEffectTierThreshold(tier))) + "%";
		variables.push_back(val);
		variableDecos.push_back({ { 0, val.length(), bulletEffectColors.at(tier)} });
	}
	if (tier == BulletEffectType::PlayerNumDash) {
		val = std::to_string(50 * (1 + getEffectValueTierThresholdDifference(PlayerNumDash))) + "px";
		variables.push_back(val);
		variableDecos.push_back({ { 0, val.length(), bulletEffectColors.at(tier)} });
	}
	if (tier == BulletEffectType::FireRate) {
		val = std::to_string((2 + getEffectValueTierThresholdDifference(FireRate)));
		variables.push_back(val);
		variableDecos.push_back({ { 0, val.length(), bulletEffectColors.at(tier)} });
	}
	if (tier == BulletEffectType::PlayerSpeed) {
		TimeModifier tm = TimeModifier();
		val = getTruncatedDecimal(tm.BASECOUNTDOWN / 1000) + "s"; // TODO: comma is also coloured green for some reason
		variables.push_back(val);
		variableDecos.push_back({ { 0, val.length(), bulletEffectColors.at(tier)} });
		std::string val2 = getTruncatedDecimal((tm.BASECOUNTDOWN + getEffectValueTierThresholdDifference(PlayerSpeed) * tm.COUNTDOWNPERSPEED) / 1000) + "s";
		variables.push_back(val2);
		variableDecos.push_back({ { 0, val2.length(), bulletEffectColors.at(tier)} });
	}
	bindScriptVariables(textReq, variables, variableDecos);

	// get formatted text based on bounds
	std::vector<std::string> formatted =
		getFormattedText(getTokenizedText(textReq.text), textReq.scale, textReq.alignment, {textReq.x, textReq.y}, textReq.topRightBound, textReq.bottomLeftBound);
	textReq.formattedText = formatted;

	// scale box vertically to number of lines
	motion.scale.y = textReq.formattedText.size() * 50;
	motion.position.y = position.y + motion.scale.y / 2 + 50 + 10;
	textReq.y = windowState.height - motion.position.y + motion.scale.y / 2 - 50;

}

// update bullet ui and its arrow
// position = top middle position
void UISystem::updateBulletUI(vec2 position, BulletStackEffect bullet) {
	Motion& motion = registry.motions.get(bulletUI);
	WindowState& windowState = registry.windowStates.components[0];
	StackCompile& stack = registry.stackCompile.components[0];
	motion.position = position;
	if ((motion.position.x - motion.scale.x / 2) < 0 + 25) {
		motion.position.x += (motion.position.x - motion.scale.x / 2) * -1 + 25;
	}
	registry.renderRequests.get(bulletUI).show = true;

	TextRenderRequest& textReq = registry.textRenderRequests.get(bulletUI);
	if (uiTexts.count("HoverBullet_" + bullet.name) == 0) { // new effect, need to generate text and tokenize it
		std::string tooltip = makeBulletTooltip(bullet);
		uiTexts.insert({ "HoverBullet_" + bullet.name, {tooltip, getTokenizedText(tooltip),{}} });
	}

	textReq.x = motion.position.x - motion.scale.x / 2 + 20;
	textReq.bottomLeftBound = {textReq.x, 0};
	textReq.topRightBound = { textReq.x + motion.scale.x - 25, 10000 };
	textReq.decorations.clear();
	vec3 color = bulletEffectColors.at(bullet.type);
	if (bullet.type == Key) {
		color = COLOR_YELLOW;
	}
	
	Motion& arrowMotion = registry.motions.get(bulletUIArrow);
	arrowMotion.position = { position.x, position.y + 51 };
	registry.renderRequests.get(bulletUIArrow).show = true;
	arrowMotion.scale.x = abs(arrowMotion.scale.x) * -1;

	std::vector<std::string> tokenizedBody = uiTexts["HoverBullet_" + bullet.name].tokenizedText;
	std::string name = bullet.name;

	if (bullet.value != 0) {
		name += " (";
		if (bullet.value > 0)
			name += "+";
		name += std::to_string(bullet.value) + ")";
	}
	else {
		name += " (unstackable)";
	}
	name += "\n\n";

	if (bullet.value != 0) {
		textReq.text = name + uiTexts["HoverBullet_" + bullet.name].text + "\n\nTotal in stack: ";

		textReq.decorations.push_back(TextDecorationSpan{ textReq.text.length(), textReq.text.length() + std::to_string(getEffectValue(bullet.type)).length(), color });
		textReq.text += std::to_string(getEffectValue(bullet.type)) + "\nTotal " + bulletEffectDescriptions.at(bullet.type) + ": ";
		
		// add any units here, using map
		float mod = stack.Call(bullet.type);
		float futureMod = stack.Call(bullet.type, 1);

		// convert ms to s if needed
		if (bulletEffectUnits.at(bullet.type).compare("s") == 0) {
			mod /= 1000;
			futureMod /= 1000;
		}

		std::string modStr = getTruncatedDecimal(mod) + " " + bulletEffectUnits.at(bullet.type);
		// special case: bullet range is range and bullet speed
		if (bullet.type == BulletEffectType::BulletRange) {
			modStr += ", " + getTruncatedDecimal(getModifiedBulletSpeed(stack.Call(bullet.type))) + " px/s";
		}

		textReq.decorations.push_back(TextDecorationSpan{ textReq.text.length(), textReq.text.length() + modStr.length(), color });
		textReq.text += modStr + "\nAt +1 value: "; // TODO: figure out a good name

		std::string futureModStr = getTruncatedDecimal(futureMod) + " " + bulletEffectUnits.at(bullet.type);
		// special case: bullet range is range and bullet speed
		if (bullet.type == BulletEffectType::BulletRange) {
			futureModStr += ", " + getTruncatedDecimal(getModifiedBulletSpeed(stack.Call(bullet.type, 1))) + " px/s";
		}
		textReq.decorations.push_back(TextDecorationSpan{ textReq.text.length(), textReq.text.length() + futureModStr.length(), color });
		textReq.text += futureModStr;
		
		std::vector<std::string> formatted =
			getFormattedText(getTokenizedText(textReq.text), textReq.scale, textReq.alignment, {textReq.x, textReq.y}, textReq.topRightBound, textReq.bottomLeftBound);
		textReq.formattedText = formatted;
	}
	else {
		std::vector<std::string> tokenizedName = getTokenizedText(name);
		tokenizedName.insert(tokenizedName.end(), tokenizedBody.begin(), tokenizedBody.end());
		// get formatted text based on bounds
		std::vector<std::string> formatted =
			getFormattedText(tokenizedName, textReq.scale, textReq.alignment, { textReq.x, textReq.y }, textReq.topRightBound, textReq.bottomLeftBound);
		textReq.formattedText = formatted;
	}

	textReq.decorations.push_back(TextDecorationSpan{ 0, name.length() - 3, color });

	// scale box vertically to number of lines
	motion.scale.y = textReq.formattedText.size() * 50;
	motion.position.y = position.y + motion.scale.y / 2 + 50 + 10;
	textReq.y = windowState.height - motion.position.y + motion.scale.y / 2 - 50;
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
	motion.scale = { 500, 400 }; // hard code scale.x, scale.y scaled to # of lines
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

	registry.gameOverlayUIs.emplace(entity);

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

Entity UISystem::createInteractGauge(vec2 position, float timer, float baseTimer) {
	Entity entity = Entity();

	auto& rr = registry.renderRequests.insert(
		entity,
		{ "enemy_bullet_square.png", // temporary choice selection indicator
		 EFFECT_ASSET_ID::DASH,
		 GEOMETRY_BUFFER_ID::SPRITE });
	rr.show = true;

	registry.gameUIs.emplace(entity);

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.scale = { 10, 50 };
	motion.position = { position.x, position.y };

	vec3& color = registry.colors.emplace(entity);
	color = COLOR_TEAL_LIGHT;

	GaugeVisual& gauge = registry.gaugeVisuals.emplace(entity);
	gauge.chargeBoundary = glm::lerp(0.f, 1.f, (baseTimer - timer) / baseTimer);
	gauge.isVertical = true;
	gauge.unchargedColor = vec4(0.65, 0.65, 0.65, 1.0);

	// hard code these offsets to make the doors look nice
	Room room = registry.maps.components[0].currRoom;
	if (position.y <= registry.motions.get(registry.doors.entities[0]).position.y) { // bottom door
		motion.position.y -= 50;
		motion.scale = { 50, 10 };
		gauge.isVertical = false;
	}
	else if (position.y >= registry.motions.get(registry.doors.entities[2]).position.y) { // top door
		motion.position.y += 50;
		motion.scale = { 50, 10 };
		gauge.isVertical = false;
	}

	if (position.x <= registry.motions.get(registry.doors.entities[3]).position.x) { // left door
		motion.position.x -= 50;
	}
	else if (position.x >= registry.motions.get(registry.doors.entities[1]).position.x) { // right door
		motion.position.x += 50;
	}

	registry.uiGauges.emplace(entity);

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
	text.formattedText = getFormattedText(getTokenizedText(text.text), text.scale, text.alignment, { text.x, text.y }, text.topRightBound, text.bottomLeftBound);

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
	button.buttonSize = vec2(text.text.length() * text.scale * DEFAULT_FONT_SIZE, text.scale * DEFAULT_FONT_SIZE + button.padding * 2.f);
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
	text.y = windowState.height - position.y + scale.y / 2 - DEFAULT_FONT_SIZE * 1.5; // place text slightly above middle of box
	text.scale = 0.5;										
	text.topRightBound = { scale.x - 75, scale.y - 25 };
	text.bottomLeftBound = { text.x + 25, 0 + 25 };

	// attach list of dialogue lines
	// probably shouldn't be attached to box, but to some dialogue state entity?
	auto& lines = registry.dialogueLines.emplace(entity);

	registry.drawingTexts.emplace(entity);

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
	text.x = position.x/* - scale.x/2.f + 25*/;
	text.y = position.y + scale.y / 2.f - 25.f - text.scale * DEFAULT_FONT_SIZE;
	text.topRightBound = { position.x + scale.x / 2.f - 25, position.y + scale.y / 2.f - 25 };
	text.bottomLeftBound = { text.x, 0 + 25 };
	text.formattedText = uiTexts["ControlsGuide"].tokenizedText;
	text.alignment = TextAlignment::CenteredAlign;

	UIBorder& border = registry.uiBorders.emplace(entity);
	border.borderColour = vec3(1.f);
	border.border = UIBorderType::Outlined;
	border.borderThickness = 10.f;

	Menu& menu = registry.menus.emplace(entity);
	menu.options = { "Back" };
	menu.startPos = { position.x, position.y + scale.y / 2.f - 50 };
	menu.offset = { 0, 50 + 30 };
	menu.type = MenuType::ControlsMenu;

	std::vector<std::string> formatted = getFormattedText(text.formattedText, text.scale, text.alignment, { text.x, text.y }, text.topRightBound, text.bottomLeftBound);
	text.formattedText = formatted;
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
	menu.options = { "Resume", "Controls", "Title", "Quit"};
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
	text.x = position.x /*- scale.x / 2.f + 25*/;
	text.y = position.y + scale.y / 2.f - 25.f - text.scale * DEFAULT_FONT_SIZE;
	text.topRightBound = { position.x + scale.x / 2.f - 25, position.y + scale.y / 2.f - 25 };
	text.bottomLeftBound = { 0, 0 + 25 };
	text.alignment = TextAlignment::CenteredAlign;
	text.formattedText = getFormattedText(uiTexts["GameOver"].tokenizedText, text.scale, text.alignment, { text.x, text.y }, text.topRightBound, text.bottomLeftBound);

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

void UISystem::updateFlashMessageDisplay(std::string text) {
	registry.renderRequests.get(flashMessageDisplay).show = true;
	if (registry.showTimers.has(flashMessageDisplay)) {
		registry.showTimers.get(flashMessageDisplay).timer = registry.showTimers.get(flashMessageDisplay).base;
	}
	else {
		if (registry.fades.has(flashMessageDisplay))
			registry.fades.remove(flashMessageDisplay);
		registry.showTimers.emplace(flashMessageDisplay);
	}

	TextRenderRequest& trr = registry.textRenderRequests.get(flashMessageDisplay);
	trr.text = text;

	Motion& motion = registry.motions.get(flashMessageDisplay);
	motion.scale.x = text.length() * DEFAULT_FONT_SIZE * trr.scale + 35.f * 2;
}

Entity UISystem::createFlashMessageDisplay() {
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
	trr.y = windowState.height - (trr.scale * DEFAULT_FONT_SIZE + padding / 2) * 3.0f - trr.scale * DEFAULT_FONT_SIZE / 2 - padding; //appear below room name
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

Entity UISystem::createRoomName() {
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
	trr.text = "";
	trr.color = vec3(1.0f);
	trr.scale = 0.35f;
	trr.x = windowState.width - padding;
	trr.y = windowState.height - (trr.scale * DEFAULT_FONT_SIZE + padding / 2) * 3.f; // appear below room counter
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
	std::vector<TextDecorationSpan> decorationSpans;
	std::string text;

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
					if (text != "") {
						std::string parsedBody;
						// get rid of the extra new line at the end
						parseBodyDecorations(text.substr(0, text.length()-1), parsedBody, decorationSpans);
						uiTexts.insert({ uiName, {parsedBody, getTokenizedText(parsedBody), decorationSpans}});
						tokenizedText.clear();
						decorationSpans.clear();
						text = "";
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
					uiTextBody += line.substr(start, end) + "\n";
					text += uiTextBody;
					
					//// ref: https://www.geeksforgeeks.org/concatenate-two-vectors-in-cpp/
					//tokenizedText.insert(tokenizedText.end(), newTokenizedText.begin(), newTokenizedText.end());
					//tokenizedText.push_back(newLine);
				}
			}
		}
		entity_file.close();
		std::string parsedBody;
		parseBodyDecorations(text.substr(0, text.length() - 1), parsedBody, decorationSpans);
		uiTexts.insert({ uiName, {parsedBody, getTokenizedText(parsedBody), decorationSpans} });
	}
	else
	{
		std::cout << "ERROR: failed to open file: " << filename << std::endl;
	}
}

// returns a string that contains:
// "<bullet name>" if the bullet doesn't have a value (key, inert, etc)
// "<bullet name> (+/-<value>)" otherwise
std::string getFormattedBulletEffectString(BulletStackEffect bullet) {
	std::string str = bullet.name;

	if (bullet.value != 0) {
		str += " (";
		if (bullet.value > 0) {
			str += "+";
		}
		str += std::to_string(bullet.value) + ")";
	}
	
	return str;
}

std::string UISystem::makeBulletTooltip(BulletStackEffect bullet) {
	std::string tooltip = /*"bullet.name + "\n\n"*/"";
	std::string modify = "";
	std::string effect = "";
	float intermediaryAmount = 0;

	// special cases
	if (bullet.type == BulletEffectType::Inert) {
		tooltip += "This bullet doesn't do anything.";
	}
	else if (bullet.type == BulletEffectType::Key) {
		tooltip += "A keycard used for unlocking doors. Pops all subsequent bullets on the stack after use.";
	}
	else if (bullet.type == BulletEffectType::Lightning) { // not actually used
		tooltip += "Shifts the bullets in the stack over by 1.";
	}
	else {// ordinary bullets
		// Not sure if player initialized at this point
		StackCompile stack = StackCompile();
		if (bullet.value > 0 && stack.Call(bullet.type, 1) < 0) {
			modify = "Decreases ";
		}
		else {
			modify = "Increases ";
		}

		switch (bullet.type) {
		case BulletDamage:
			effect = "the damage of bullets.";
			break;
		case ProjectileSize:
			effect = "the size of bullets.";
			break;
		case FireRate:
			effect = "interval of bullets fired.";
			break;
		case BulletRange:
			effect = "bullet range and speed.";
			break;
		case BulletAccuracy:
			effect = "the spread of bullets fired.";
			break;
		case BulletNum:
			effect = "the number of bullets shot at once.";
			break;
		case Bounce:
			effect = "the number of times bullets bounce.";
			break;
		case Pierce:
			effect = "the pierce of bullets.";
			break;
		case Homing:
			effect = "the homing effect of bullets.";
			break;
		case PlayerSpeed:
			effect = "movement speed.";
			break;
		case PlayerNumDash:
			effect = "the number of dashes.";
			break;
		case PlayerStackSize:
			effect = "stack size.";
			break;
		case PlayerDashRecharge:
			effect = "dash cooldown.";
			break;
		default:
			effect = "This bullet is not in the list?? Report immediately!";
		}
		tooltip += modify + effect;
	}
	return tooltip;
}

void UISystem::loadBulletEffects() {
	for (BulletStackEffect bullet : premadeBullets) {
		std::string tooltip = makeBulletTooltip(bullet);
		std::vector<std::string> tokenizedTooltip;
		if (tooltip.length() > 0) {
			tokenizedTooltip = getTokenizedText(tooltip);
			uiTexts.insert({ "HoverBullet_" + bullet.name, {tooltip, tokenizedTooltip, {}}});
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

void UISystem::bindScriptVariables(TextRenderRequest& request, std::vector<std::string>& variables, std::vector<std::vector<TextDecorationSpan>> variableDecorations) {
	std::string text = request.text;
	// assumes script format: {x}, where x corresponds to the index of the variable
	for (int i = 0; i < text.length(); i++) {
		char c = text.at(i);
		if (c == '{') {
			// very basic format checking -- make sure there's a closing brace
			assert(text.find_first_of('}', i) != -1);
			int closingBraceIndex = text.find_first_of('}', i);
			int varNum = std::stoi(text.substr(i + 1, closingBraceIndex)); // assume this will work for now
			assert(varNum < variables.size() || variables.size() == 0);
			text = text.substr(0, i) + variables[varNum] + text.substr(closingBraceIndex + 1);

			int varLength = (variables[varNum].length() > 3? variables[varNum].length() : variables[varNum].length() - 3);

			// make sure to readjust decoration spans too
			for (TextDecorationSpan& deco : request.decorations) {
				if (deco.startIndex > i) {
					deco.startIndex += varLength;
				}
				if (deco.endIndex >= i) {
					deco.endIndex += varLength;
				}
			}
			
			if (variableDecorations.size() > varNum) {
				for (TextDecorationSpan& span : variableDecorations.at(varNum)) {
					// need to make different animations based on type... or else will be treated as no text animation
					// is there a way to just move instead of reconstructing?
					if (span.animationType == TextAnimationType::WavyText) {
						request.decorations.push_back(TextDecorationSpan{ span.startIndex + i, span.endIndex + i, span.color,
							span.animationType, std::make_shared<WavyTextAnimation>(span.animation->parameters), span.timer, span.baseTimer });
					}
					else if (span.animationType == TextAnimationType::WobblyText) {
						request.decorations.push_back(TextDecorationSpan{ span.startIndex + i, span.endIndex + i, span.color,
							span.animationType, std::make_shared<WobblyTextAnimation>(span.animation->parameters), span.timer, span.baseTimer });
					}
					else {
						request.decorations.push_back(TextDecorationSpan{ span.startIndex + i, span.endIndex + i, span.color,
							span.animationType, std::make_shared<TextAnimation>(std::move(*span.animation)), span.timer, span.baseTimer });
					}
				}
			}

			i += variables[varNum].length();
		}
	}

	request.text = text;
}

// merges effects as if they were on the stack
std::vector<BulletStackEffect> mergeEffects(std::vector<BulletStackEffect> effects) {
	std::vector<BulletStackEffect> mergeEffects;
	BulletStackEffect prev = blunt;
	const int maxPosVal = 3;
	const int maxNegVal = -3;

	// first check size is more than 1 effect, i.e there's something to actually merge
	if (effects.size() <= 1) {
		return effects;
	}

	for (int i = 0; i < effects.size(); i++) {
		if (effects[i].type == Key || effects[i].type == Inert || effects[i].type == Lightning) {
			mergeEffects.push_back(effects[i]);
		}
		else { // need to count normal bullets
			if (effects[i].type == prev.type) {
				prev.value += effects[i].value;

				// make sure doesn't overflow
				if (prev.value > maxPosVal) {
					BulletStackEffect prevCopy = prev;
					prevCopy.value = maxPosVal;
					prev.value -= maxPosVal;
					mergeEffects.push_back(prevCopy);
				}

				if (prev.value < maxNegVal) {
					BulletStackEffect prevCopy = prev;
					prevCopy.value = maxNegVal;
					prev.value -= maxNegVal;
					mergeEffects.push_back(prevCopy);
				}
			}
			else {
				if (prev.type != BulletEffectType::Inert && prev.value != 0) {
					mergeEffects.push_back(prev);
				}
				prev = effects[i];
			}
		}
	}
	if (prev.type != BulletEffectType::Inert && prev.value != 0) {
		mergeEffects.push_back(prev);
	}

	return mergeEffects;
}

std::string getTruncatedDecimal(float num) {
	if (abs(num - (int)num) > 0) {
		std::stringstream amountString;
		amountString << std::fixed << std::setprecision(2) << num;
		return amountString.str();
	}
	else {
		return std::to_string((int)num);
	}
}

bool UISystem::hoverBulletStack(IOState& ioState, StackUI& stackui, StackCompile& stack) {
	int bulletHoveredIndex = -1;
	int count = 0;
	vec2 bulletSize = stackui.bulletSize;

	// should check first: is it in stack ui at all?
	// this is point in aabb detection
	if (ioState.mousePosition.x > (stackui.stackPos.x - stackui.stackSize.x / 2) && ioState.mousePosition.x < (stackui.stackPos.x + stackui.stackSize.x / 2)
		&& ioState.mousePosition.y >(stackui.stackPos.y - stackui.stackSize.y / 2) && ioState.mousePosition.y < (stackui.stackPos.y + stackui.stackSize.y / 2)) {
		for (vec2 bulletPos : stackui.bulletPositions) {
			if (ioState.mousePosition.x > (bulletPos.x - bulletSize.x / 2) && ioState.mousePosition.x < (bulletPos.x + bulletSize.x / 2)
				&& ioState.mousePosition.y >(bulletPos.y - bulletSize.y / 2) && ioState.mousePosition.y < (bulletPos.y + bulletSize.y / 2)) {
				bulletHoveredIndex = count;
				break;
			}
			count++;
		}
		if (bulletHoveredIndex > -1) {
			updateBulletUI(vec2(stackui.bulletStartPos.x + bulletHoveredIndex * stackui.bulletSize.x + bulletHoveredIndex * stackui.bulletOffset,
				stackui.bulletStartPos.y), stack.currStack[bulletHoveredIndex]);
			return true;
		}
		else if (bulletHoveredIndex == -1) {
			return false;
		}
	}
	return false;
}

bool UISystem::hoverTierStatus(IOState& ioState, StackUI& stackui) {
	// check tier icon hover
	int bulletHoveredIndex = -1;
	int count = 0;
	for (auto& tier : stackui.activeTiers) {
		if (ioState.mousePosition.x > (tier.second.x - TIER_ICON_SCALE.x / 2) && ioState.mousePosition.x < (tier.second.x + TIER_ICON_SCALE.x / 2)
			&& ioState.mousePosition.y >(tier.second.y - TIER_ICON_SCALE.y / 2) && ioState.mousePosition.y < (tier.second.y + TIER_ICON_SCALE.y / 2)) {
			bulletHoveredIndex = count;
			// first is const so can't actually capture value, have to do call inside here instead
			updateTierUI(tier.second, tier.first);
			break;
		}
		count++;
	}
	lastHoveredBullet = bulletHoveredIndex;
	if (bulletHoveredIndex == -1) {
		return false;
	}
	return true;
}

bool UISystem::hoverBossStatus(IOState& ioState) {
	int bulletHoveredIndex = -1;
	int count = 0;
	if (registry.bosses.entities.size() > 0) {
		// check status icon hover
		// feels a bit finicky
		HPBarUI& bossHP = registry.hpBarHavers.get(registry.bosses.entities[0]);
		for (int i = 0; i < bossHP.activeStatuses.size(); i++) {
			if (bossHP.activeStatuses[i] >= 0) {
				if (ioState.mousePosition.x > (bossHP.statusPositions[i].x - bossHP.iconSize.x / 2)
					&& ioState.mousePosition.x < (bossHP.statusPositions[i].x + bossHP.iconSize.x / 2)
					&& ioState.mousePosition.y >(bossHP.statusPositions[i].y - bossHP.iconSize.y / 2)
					&& ioState.mousePosition.y < (bossHP.statusPositions[i].y + bossHP.iconSize.y / 2)) {
					bulletHoveredIndex = count;
					break;
				}
			}
			count++;
		}
		lastHoveredBullet = bulletHoveredIndex;
		if (bulletHoveredIndex > -1) {
			updateStatusUI(bossHP.statusPositions[bulletHoveredIndex], static_cast<SpecialStates>(bulletHoveredIndex), registry.bosses.entities[0]);
			lastHoveredBullet = bulletHoveredIndex;
			return true;
		}
		else if (bulletHoveredIndex == -1) {
			return false;
		}
	}
	lastHoveredBullet = bulletHoveredIndex;
	return false;
}

std::vector<BulletStackEffect> UISystem::extractBulletEffects(std::vector<std::vector<BulletStackEffect>> bulletChances) {
	std::vector<BulletStackEffect> effects;
	
	for (auto chance : bulletChances) {
		for (auto effect : chance) {
			effects.push_back(effect);
		}
	}

	return effects;
}