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

enum UIBorderType {
	Outlined,
	Fancy // does not currently exist
};

struct UIBorder {
	UIBorderType border = UIBorderType::Outlined;
	vec3 borderColour = vec3(1.f);
	float borderThickness = 0;
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

enum TextAlignment : int {
	LeftAlign,
	RightAlign,
	CenteredAlign // not yet implemented
};

struct MenuChoice {

};

struct GameOverlayUIText {

};

struct GameOverlayUI {

};

struct ShowTimer {
	float timer = 1000.f;
	float base = timer;
};

struct StackAddNotif {

};

enum UIRequestType {
	ResetUI,
	GameOverReport,
	CallNotif,
	DisplayFlashMessage,
	StackNotifBullet
};

struct UIButton {
	float padding = 15.f;
	vec2 buttonSize = vec2(0);
	vec2 position = vec2(0);
};

enum MenuType {
	TitleMenu,
	PauseMenu,
	ControlsMenu,
	GameOverMenu
};

struct Menu {
	std::vector<std::string> options;
	vec2 startPos;
	vec2 offset;
	MenuType type;
};

struct ActiveMenu {

};

struct GaugeVisual {
	float chargeBoundary = 1.0;
	vec4 unchargedColor = vec4(0.65, 0.65, 0.65, 1.0);
	bool isVertical = false;
};

struct NotifMessage {
	// this is a notif message
};

// used for drawing text char by char
struct DrawingText {
	float interval = 16; // how fast draw next char (ms)
	float current = 0;
	int toDraw = 0; // buffer for how many char to draw this frame based on interval
	bool doneDrawing = false; // set by text system when done drawing (no outside char counter implemented yet)
	bool blink = false; // blink the cursor
};

struct UIGauge {

};

// simplest way for text system to know where hp bar is drawn in render system
// so that text can "wobble" along with hp bar
struct HPBarUI {
	vec2 position;
	vec2 scale;
	float alpha = 1.0;
	bool followCamera = true;
};