#pragma once

// not sure how I feel about struct just for this
// but is easiest way for text and render system to talk
struct StackUI {
	vec2 bulletStartPos;
	vec2 bulletSize;
	float bulletOffset; // space between bullets
	vec2 stackSize;
	vec2 stackPos;

	std::vector<vec2> bulletPositions;

	void updateStackUISize(int bulletCapacity) {
		stackSize = vec2(bulletCapacity * bulletSize.x + bulletCapacity * bulletOffset + 2 * bulletOffset, bulletSize.y + 2 * bulletOffset);
		stackPos = vec2(bulletStartPos.x + stackSize.x / 2 - bulletSize.x - bulletOffset / 2, bulletStartPos.y);
	}
};

struct GameUI {
	// this is ui used for displaying game stats
};

struct DialogueUI {
	// this is ui for displaying dialogue
};

struct MenuUI {
	// this is used for menus like pause, game over, etc
	// and appear at the very top
};

struct GameText {
	// this is text shown in game; may not neccessarily use this?
};

struct GameUIText {

};

struct DialogueUIText {

};

struct MenuUIText {

};

struct ScreenCutIn {

};

struct Cursor {

};

struct InteractIndicator {

};

struct MenuOverlayUI {

};

struct MenuOverlayUIText {

};

struct GameOverlayUI {

};