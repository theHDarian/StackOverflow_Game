#include "ui_system.hpp"

#include "sound_system.hpp"


UISystem::UISystem(SoundSystem* soundSystem) {
	this->soundSystem = soundSystem;
}

UISystem::~UISystem() {

}

void UISystem::step(float elapsed_ms) {
	GameState& gameState = registry.gameStates.components[0];
	registry.renderRequests.get(gameOverMenu).show = gameState.gameOver;
	if (!gameState.gameOver) {
		registry.renderRequests.get(pauseMenu).show = gameState.gamePaused;
		registry.renderRequests.get(controlsGuide).show = gameState.gamePaused;
		registry.renderRequests.get(dialogueBox).show = gameState.dialogueScene;
		if (!gameState.dialogueScene) {
			registry.renderRequests.get(dialogueAvatar).show = false;
			registry.renderRequests.get(screenCutIn).show = false;
		}
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

bool UISystem::init(GLFWwindow* window) {
	// just use window state until can figure out grabbing from window directly
	WindowState& wS = registry.windowStates.components[0];
	
	pauseMenu = createPauseMenu(vec2(wS.width / 2, wS.height / 2), vec2(wS.width, wS.height / 4));
	controlsGuide = createControlsGuide(vec2(wS.width / 2, wS.height / 2 + wS.height / 8), vec2(wS.width, wS.height / 4));
	gameOverMenu = createGameOverMenu(vec2(wS.width / 2, wS.height / 2), vec2(wS.width, wS.height / 4));
	stackUI = createStackUI(wS, registry.stackCompile.components[0]);
	dialogueBox = createDialogueBox(vec2(wS.width / 2, wS.height - wS.height / 8), vec2(wS.width, wS.height / 4));
	dialogueAvatar = createDialogueAvatar(vec2(150, wS.height - wS.height / 8 - 25), vec2(wS.height / 4 - 100, wS.height / 4 - 100));
	screenCutIn = createScreenCutIn();

	return true;
}

bool UISystem::resetStackUI() {
	WindowState& wS = registry.windowStates.components[0];
	stackUI = createStackUI(wS, registry.stackCompile.components[0]);
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
			registry.textRenderRequests.get(dialogueBox).text = nextLine.text;
			
			// play a sound if there is one
			if (nextLine.sfx == IncomingDialogue) {
				soundSystem->playIncomingDialogueSound();
			} else if (nextLine.sfx == DoorOpen) {
				soundSystem->playDoorOpenSound();
			} else {
				soundSystem->stopIncomingDialogueSound();
				soundSystem->playNextDialogueSound();
			}

			// change speaker avatar and name to current
			if (nextLine.speakerName.length() > 0 && nextLine.speakerName.compare(currSpeakerName) != 0) {
				registry.textRenderRequests.get(dialogueAvatar).text = nextLine.speakerName;
			}
			if (nextLine.speakerAvatar.length() > 0 && nextLine.speakerAvatar.compare(currSpeakerAvatar) != 0) {
				if (nextLine.speakerName.compare("N") != 0) { // N is narrator for now
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

			vec2 startingPosition = vec2(400, wS.height - wS.height / 8 - 25);
			// display options for player if there is one
			for (int i = 0; i < nextLine.choices.size(); i++) {
				vec2 nextPosition = vec2(startingPosition.x, startingPosition.y + i * 50);
				createDialogueChoice(nextLine.choices[i], nextPosition);
			}
			// set first choice to highlighted by default
			if (registry.dialogueChoices.components.size() > 0) {
				input.hoveringDialogueChoice = 0;
				input.lastHoverDialogueChoice = 0;
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
	text.x = position.x - scale.x / 2;				
	text.y = windowState.height - position.y - scale.y / 2 - 40;
	text.scale = 0.45;
	text.text = "speaker name";
	// too lazy to calculate fitting text box size, and it prob won't overflow
	// so just set it to some big number
	text.topRightBound = { 1000, 1000 };
	text.bottomLeftBound = { 0, 0 };

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
		{ "none",
		 EFFECT_ASSET_ID::EGG,
		 GEOMETRY_BUFFER_ID::DEBUG_LINE });
	rr.show = false;

	registry.dialogueUIs.emplace(entity);

	Motion& motion = registry.motions.emplace(entity);
	motion.angle = 0.f;
	motion.velocity = { 0, 0 };
	motion.position = position;
	motion.scale = scale;

	// temp colour
	auto& color = registry.colors.emplace(entity);
	color.r = 0.0;
	color.b = 1.0;
	color.g = 1.0;

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
	text.text = "hello this is test dialogue!";
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
		{ "none",
		 EFFECT_ASSET_ID::EGG,
		 GEOMETRY_BUFFER_ID::DEBUG_LINE });
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
	color.g = 0.9;

	// attach 1 text render request
	registry.menuUITexts.emplace(entity);
	auto& text = registry.textRenderRequests.emplace(entity);
	text.color = vec3(1, 1, 1);

	WindowState& windowState = registry.windowStates.components[0];
	text.x = windowState.width - scale.x + 25;
	text.y = windowState.height - position.y + scale.y / 4;
	text.scale = 0.5;
	text.text = "Controls:\n[WASD] to move, [SPACE]/[RMB] to dash, [LMB] to shoot\n[P] to pause, [R] to restart, [ESC] to quit, [E] to progress dialogue\n[C] to toggle collider visuals, [T] to turn tutorial on/off";
	text.topRightBound = { scale.x - 25, scale.y - 25 };
	text.bottomLeftBound = { text.x, 0 + 25 };

	return entity;
}

// not a real menu right now; just to show the game is paused
Entity UISystem::createPauseMenu(vec2 position, vec2 scale)
{
	Entity entity = Entity();

	// copies code from draw line as a box for now
	auto& rr = registry.renderRequests.insert(
		entity,
		{ "none",
		 EFFECT_ASSET_ID::EGG,
		 GEOMETRY_BUFFER_ID::DEBUG_LINE });
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
	color.g = 0.9;

	// attach 1 text render request
	registry.menuUITexts.emplace(entity);
	auto& text = registry.textRenderRequests.emplace(entity);
	text.color = vec3(1, 1, 1);

	// note: position is not center, but start of text rendering
	// need a mechanism to figure out text line size
	WindowState& windowState = registry.windowStates.components[0];
	text.x = windowState.width - scale.x + 25;
	text.y = windowState.height - position.y;
	text.scale = 1.5;
	text.text = "Game Paused";
	text.topRightBound = { scale.x - 25, scale.y - 25 };
	text.bottomLeftBound = { text.x, 0 + 25 };

	return entity;
}

// not a real menu right now; just to show the game is over
Entity UISystem::createGameOverMenu(vec2 position, vec2 scale)
{
	Entity entity = Entity();

	// copies code from draw line as a box for now
	auto& rr = registry.renderRequests.insert(
		entity,
		{ "none",
		 EFFECT_ASSET_ID::EGG,
		 GEOMETRY_BUFFER_ID::DEBUG_LINE });
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
	text.x = windowState.width - scale.x + 25;
	text.y = windowState.height - position.y + scale.y / 4;
	text.scale = 1.2;
	text.text = "Game Over \npress R to restart";
	text.topRightBound = { scale.x - 25, scale.y - 25 };
	text.bottomLeftBound = { text.x, 0 + 25 };

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

	stackui.bulletStartPos = { 75, 100 };
	stackui.bulletSize = { 20, 50 };
	stackui.bulletOffset = 10; // space between bullets

	stackui.stackSize = vec2(stack.baseStackSize * stackui.bulletSize.x + stack.baseStackSize * stackui.bulletOffset + 2 * stackui.bulletOffset, stackui.bulletSize.y + 2 * stackui.bulletOffset);
	stackui.stackPos = vec2(stackui.bulletStartPos.x + stackui.stackSize.x / 2 - stackui.bulletSize.x - stackui.bulletOffset / 2, stackui.bulletStartPos.y);

	registry.gameUITexts.emplace(entity);
	auto& text = registry.textRenderRequests.emplace(entity);
	text.color = vec3(1, 1, 1);
	text.x = stackui.bulletStartPos.x - stackui.bulletSize.x - stackui.bulletOffset / 2;
	text.y = (stackui.bulletStartPos.y - windowState.height) * -1 - 2 * stackui.bulletOffset - stackui.bulletSize.y;
	text.scale = 0.25;
	// too lazy to calculate fitting text box size, and it prob won't overflow
	// so just set it to some big number
	text.topRightBound = { 1000, 1000 };
	text.bottomLeftBound = { 0, 0 };

	return entity;
}

Entity UISystem::createScreenCutIn() {
	Entity entity = Entity();

	// copies code from draw line as a box for now
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