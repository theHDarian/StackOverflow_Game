#pragma once
#include <SDL_mixer.h>

#include "common.hpp"
#include <vector>
#include <stack>
#include <unordered_map>

#include "../ext/stb_image/stb_image.h"

#include "io_components.hpp"
#include "ui_components.hpp"

// Stucture to store collision information
struct Collision
{
	// Note, the first object is stored in the ECS container.entities
	Entity other; // the second object involved in the collision
	Collision(Entity& other) { this->other = other; };
};

struct CircleCollider {
	float radius;
};

struct PolyCollider {
	// Points are relative to the origin {0,0}
	// Points are in order around the polygon
	std::vector<vec2> offsetVertices;
	float maxLength;
	float minLength;

	// Call this function after creating offsetVertices please
	// Bit pricey though, so if you know the max and min length just hardcode them in!
	void setPolyLengths() {
		maxLength = glm::distance(offsetVertices[0], { 0,0 });
		minLength = glm::distance(offsetVertices[0], { 0,0 });
		for (uint i = 1; i < offsetVertices.size(); i++) {
			if (glm::distance(offsetVertices[i], { 0,0 }) > maxLength) maxLength = glm::distance(offsetVertices[i], { 0,0 });
			vec2 a = -offsetVertices[i];
			vec2 b = offsetVertices[i-1] - offsetVertices[i];
			vec2 c = (glm::dot(a, glm::normalize(b)) * glm::normalize(b));
			if (glm::distance(a - c + offsetVertices[i], { 0,0 }) < minLength) minLength = glm::distance(a - c + offsetVertices[i], { 0,0 });
			if (glm::distance(offsetVertices[i], { 0,0 }) < minLength) minLength = glm::distance(offsetVertices[i], { 0,0 });
		}
	}
};

struct WallCollider {
	// Start an end of the line segment
	vec2 startPosition;
	vec2 endPosition;
};

//For ignoring collisions
struct Ignore {
	std::vector<Entity> ignores = {};

	bool has(Entity entity) {
		for (Entity e : ignores) {
			if ((unsigned int)e == (unsigned int)entity) return true;
		}
		return false;
	}

	void clear() {
		ignores = {};
	}
};

struct Bound {
	float angle; //degrees
	vec3 axis;
	vec3 offset;
	char side = 'L';
}; //room bounds

struct AABBCollider {
	// As offsets of motion.position:
	vec2 topLeft;		//max x, max y
	vec2 bottomRight;	//min x, min y
};

struct MeshCollider {
	// basically just exists for checking
};

// Data structure for toggling debug mode
struct Debug {
	bool in_debug_mode = false;
	bool in_freeze_mode = false;
};
extern Debug debugging;

// Struct for dash

// A struct to refer to debugging graphics in the ECS
struct DebugComponent
{
	// Note, an empty struct has size 1
};

struct Frame {
	GLuint prevTexture;
	GLuint prevFrameBuffer;
};

enum class SoundType {
	CombatBGM,
	BossBGM,
	ClearedBGM,
	TitleBGM,
	PlayerHurt,
	PlayerZapped,
	DashSound,
	PlayerShoot,
	EnemyShoot,
	IncomingDialogue,
	NormalDialogue,
	DoorOpen,
	DoorClose,
	itemPickup,
	rareItemPickup,
	explosion,
	AlarmSound,
	FanFare,
	EnemyDeathSound,
	PlayerDodgeSound,
	LaserSound,
	DiggingSound,
	PauseSounds,
	ResumeSounds,
	PausePersistentSounds,
	ResumePersistentSounds,
};
struct SoundRequest {
	SoundType type;
	std::string path;
	float volume; //0 to 1
	int loops;
	float ticks;
	int songIndex = -1;
	Mix_Music* music= nullptr;
	float delay = 0;
	Entity sourceEntity;
};

struct PersistentSounds {
	std::unordered_map<SoundType, vec2 > channels = {
	{SoundType::LaserSound, {-1, -1}},
		{SoundType::DiggingSound, {-1, -1}},
	};
	// x = channel id, y = duration

	void stop() {
		for (auto& channel : channels) {
			if (channel.second.x != -1) {
				Mix_HaltChannel(channel.second.x);
				channel.second.x = -1;
			}
		}
	}
	void stopChannel(SoundType type) {
		if (channels[type].x != -1) {
			Mix_HaltChannel(channels[type].x);
			channels[type].x = -1;
		}
	}
};


// Single Vertex Buffer element for non-textured meshes (coloured.vs.glsl & salmon.vs.glsl)
struct ColoredVertex
{
	vec3 position;
	vec3 color;
};

// Single Vertex Buffer element for textured sprites (textured.vs.glsl)
struct TexturedVertex
{
	vec3 position;
	vec2 texcoord;
};

// Mesh datastructure for storing vertex and index buffers
struct Mesh
{
	static bool loadFromOBJFile(std::string obj_path, std::vector<ColoredVertex>& out_vertices, std::vector<uint16_t>& out_vertex_indices, vec2& out_size);
	vec2 original_size = {1,1};
	std::vector<ColoredVertex> vertices;
	std::vector<uint16_t> vertex_indices;
};

/**
 * The following enumerators represent global identifiers refering to graphic
 * assets. For example TEXTURE_ASSET_ID are the identifiers of each texture
 * currently supported by the system.
 *
 * So, instead of referring to a game asset directly, the game logic just
 * uses these enumerators and the RenderRequest struct to inform the renderer
 * how to structure the next draw command.
 *
 * There are 2 reasons for this:
 *
 * First, game assets such as textures and meshes are large and should not be
 * copied around as this wastes memory and runtime. Thus separating the data
 * from its representation makes the system faster.
 *
 * Second, it is good practice to decouple the game logic from the render logic.
 * Imagine, for example, changing from OpenGL to Vulkan, if the game logic
 * depends on OpenGL semantics it will be much harder to do the switch than if
 * the renderer encapsulates all asset data and the game logic is agnostic to it.
 *
 * The final value in each enumeration is both a way to keep track of how many
 * enums there are, and as a default value to represent uninitialized fields.
 */


// NOTE: these were originall enum CLASSES in the template
// shouldn't matter much, but apparently enum CLASSES don't inherently cast to ints
// so I've (Amanda) explicitly set them to be ints
// should ask why that's the case/why it works fine even w/o inherent casting

enum  EFFECT_ASSET_ID : unsigned int {
	COLOURED = 0,						// Unused
	EGG = COLOURED + 1,					// Has own call
	MESH = EGG + 1,						// Has own call
	TEXTURED = MESH + 1,
	POSTPROCESS1 = TEXTURED + 1,		// Extra
	POSTPROCESS2 = POSTPROCESS1 + 1,	// Extra
	POSTPROCESS3 = POSTPROCESS2 + 1,	// Extra
	POSTPROCESS4 = POSTPROCESS3 + 1,	// Extra
	DASH = POSTPROCESS4 + 1,			// Has own call
	HP_BAR = DASH + 1,					// Has own call
	ROOM_BOUND = HP_BAR + 1,			// Has own call
	ANIMATE = ROOM_BOUND + 1,
	BULLET = ANIMATE + 1,				// Has own call
	PBULLET = BULLET + 1,
	EFFECT_COUNT = PBULLET + 1
};
const int effect_count = (int)EFFECT_ASSET_ID::EFFECT_COUNT;



enum  GEOMETRY_BUFFER_ID : unsigned int {
	MESH_GB = 0,
	SPRITE = MESH_GB + 1,
	EGG_GB = SPRITE + 1,
	DEBUG_LINE = EGG_GB + 1,
	SCREEN_TRIANGLE = DEBUG_LINE + 1,
	GEOMETRY_COUNT = SCREEN_TRIANGLE + 1
};



const int geometry_count = (int)GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;

struct RenderRequest {
	std::string texture_name;
	EFFECT_ASSET_ID used_effect = EFFECT_ASSET_ID::EFFECT_COUNT;
	GEOMETRY_BUFFER_ID used_geometry = GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;
	bool show = true;
	vec2 offset = { 0, 0 }; // how much the position should be shifted so that center of texture = center of object
	vec2 idealScale = vec2(-1); //Used for tiling textures. Tiling is based on difference between idealScale and motion.scale
};

enum AnimationTypes {
	NONE = 0,
	REGULAR = 1,
	ONCE = 2,
	RANDOM = 3
};

struct Animation {
	int frame = 0; // stick in animation info to here for now
	int max_frames = 5; // this type of info should be known by render/sprite system?
	float animation_countdown = 85;
	float animation_countdown_base = animation_countdown;
	int animate = true;
};

// For 3D rendering
struct Object {
	float baseOffset = 0; //Where, from the center of the obj, the player should transition from front to back
};

// Expected sprite states other systems can use
// eg: physics system sets object's sprite to DAMAGED upon collision
// not all entities may have all these sprites, so should check
// state exists in Sprites' component's sprite map first
enum class SPRITE_STATE {
	BASE = 0,
	ATTACKING = BASE + 1,
	DAMAGED = ATTACKING + 1,
	DEAD = DAMAGED + 1,
	MOVING = DEAD + 1
};

// all the sprites this entity will use
// for performance, consider 1 map per entity type
// as opposed to 1 map per entity
struct Sprites {
	// map of sprite type (enum) to sprite texture
	// eg: when bullet collides w/ enemy in physics system,
	// physics system will change the sprite to "DAMAGED_SPRITE"
	std::unordered_map<SPRITE_STATE, std::string> sprites;
};

struct CollisionShape {
	// not sure if we need to draw that many
	std::vector<Entity> shapes;
};

// if a sprite should switch after a certain amount of time
struct SpriteTimer {
	float count_ms = 1000;
	std::string nextSprite;
	EFFECT_ASSET_ID nextEffect;
};

// play an animation sequence for the entity
struct AnimationSequence {
	std::string nextSprite;
	EFFECT_ASSET_ID nextEffect;
};

enum TextAnimationType {
	WavyText, // params: the period of the wave (bigger = longer period), the amplitude of the wave (pixels), the character width of the wave (bigger = more chars)
	WobblyText, // params: the max random value for offset 1, the max random value for offset 2 
	NoTextAnimation
};

// the base text animation class, adds no animation
struct TextAnimation {
	std::vector<float> parameters; // parameters used by animations, defined per animation type
	int requiredParameters = 0;
	TextAnimation() {
		this->parameters = std::vector<float>();
	}
	TextAnimation(std::vector<float> parameters) {
		this->parameters = parameters;
	}
	virtual vec2 getPositionOffset(float time, float baseTime, int charIndex) { return vec2(0); }
};

// uses cos/sin functions to give wavy animation to text
// parameters: {period of wave modifier (bigger = longer period), the amplitude of the wave (in pixels), 
// the character width of the wave} (in the y direction) (and then same thing in the x direction)
// make sure period and character width are nonzero!
struct WavyTextAnimation : public TextAnimation {
	WavyTextAnimation(std::vector<float> parameters = {1000, 10, 5, 1, 0, 1}) : TextAnimation(parameters) {
		this->requiredParameters = 6;
		for (int i = this->parameters.size(); i < this->requiredParameters; i++) {
			if (i == 1 || i == 4) {
				this->parameters.push_back(0);
			}
			else {
				this->parameters.push_back(1);
			}
		}
	}
	vec2 getPositionOffset(float time, float baseTime, int charIndex) override {
		return vec2(cos(time / parameters.at(3) + charIndex / parameters.at(5)) * parameters.at(4),
			sin(time / parameters.at(0) + charIndex / parameters.at(2)) * parameters.at(1));
	}
};

// every X ms (when timer goes off), offset text by a random amount
// parameters: {max random value for offset 1, max random value for offset 2}
struct WobblyTextAnimation : public TextAnimation {
	WobblyTextAnimation(std::vector<float> parameters = { 5, 4 }) : TextAnimation(parameters) {
		this->requiredParameters = 2;
	}

	// ref: hp bar wobble
	vec2 getPositionOffset(float time, float baseTime, int charIndex) override {
		if (time == baseTime) {
			return vec2((rand() % (int)parameters.at(0)) - parameters.at(1), (rand() % (int)parameters.at(0)) - parameters.at(1));
		}
		return vec2(0);
	}
};

// change colour, etc of text span in text
struct TextDecorationSpan {
	size_t startIndex = -1; // starting position (character) of span
	size_t endIndex = -1; // ending position
	vec3 color = vec3(-1); // no colour

	// consider removing types entirely and having an update timer function?
	// but also needed right now to reconstruct types...
	TextAnimationType animationType = TextAnimationType::NoTextAnimation; 

	// the function used for animation. default = no animation
	// this function returns the {x, y} value that will offset the text position
	std::shared_ptr<TextAnimation> animation = std::make_shared<TextAnimation>(); 
	float timer = 60; // used for animations
	float baseTimer = timer; // used by animations that trigger every X amount of time (in ms)

	bool operator<(const TextDecorationSpan& other) const {
		return startIndex < other.startIndex;
	}
};

/*
struct TextAnimationSpan : TextDecorationSpan {
	TextAnimationType animation = TextAnimationType::WavyText;
	float timer = 60; // used for animations
	float baseTimer = timer; // used by animations that trigger every X amount of time (in ms)

	TextAnimationSpan(size_t startIndex, size_t endIndex = 0, TextAnimationType animation = TextAnimationType::WavyText,
		float baseTimer = 60) {
		this->startIndex = startIndex;
		this->endIndex = endIndex;
		this->animation = animation;
		this->baseTimer = baseTimer;
		this->timer = baseTimer;
	}
};

struct TextColorSpan : TextDecorationSpan {
	vec3 color = vec3(0);

	TextColorSpan(size_t startIndex, size_t endIndex = 0, vec3 color = vec3(0)) {
		this->startIndex = startIndex;
		this->endIndex = endIndex;
		this->color = color;
	}
};

struct TextAnimationSpanList {
	std::vector<TextAnimationSpan> spans;
};

struct TextColorSpanList {
	std::vector<TextColorSpan> spans;
};
*/

// used to store info of what text needs to be rendered
// currently, 1 request per entity (like how render requests work)
// but may consider changing (eg: emplace with duplicates)
// for things like menus that may have many text sections
struct TextRenderRequest {
	std::string text;

	// maybe position should be vec2 to match with rest of game?
	float x;
	float y;

	// text size = defualt loaded in font size * scale, default is 48 pixels
	float scale; 
	glm::vec3 color; // default text colour

	// absolute positions of text box
	vec2 topRightBound;
	vec2 bottomLeftBound;

	// this will be formatted text used for breaking up multilines
	std::vector<std::string> formattedText = std::vector<std::string>();

	// left, right or center alignment
	TextAlignment alignment = TextAlignment::LeftAlign;

	// define special spans of word colours here
	std::vector<TextDecorationSpan> decorations;
};

struct UIText {
	std::string text;
	std::vector<std::string> tokenizedText;
	std::vector<TextDecorationSpan> decorations;
};

struct Dialogue {
	std::string text;
	std::vector<TextDecorationSpan> decorations;
	std::string speakerName;
	std::string speakerAvatar;
	std::vector <std::string> choices;
	std::string cutInTexture;
	SoundType sfx;
};

struct DialogueLines {
	std::vector<Dialogue> lines;
	int current = 0;

	Dialogue prev() {
		if (current > 0) {
			return lines[current - 1];
		}
		else {
			return Dialogue{ "<end>" }; // maybe end of str constant
		}
	}

	Dialogue next() {
		if (current < lines.size()) {
			return lines[current++];
		}
		else {
			return Dialogue{"<end>"}; // maybe end of str constant
		}
	}
};

struct BG {
	// is BG
};

struct Fade {
	float max = 500;
	float time = max;
};

struct Deleted {
	// this entity is marked for deletion
};

// If parent is deleted, delete children
struct Parent {
	std::vector<Entity> children;
};

struct DialogueChoice {

};

enum InteractableType {
	DialogueInteractable,
	ActionInteractable
};

enum InteractableItem {
	Ram,
	Ramlet,
	PopConsole,
	PushConsole,
	FightConsole,
	OpenDoor,
	LockedDoor,
	ClosedDoor,
	BibleTree,
	Gardener,
	HoneyCanister,
	Baru,
	OracleCrab,
	WishGranter,
	Swarm,
	WhiteBoard,
	Desk,
	Phone,
	Mouse,
	OracleTurret,
	Oven,
	Optimizer,
	Inverter,
	NA
};

struct InteractableObject {
	std::string name;
	int dialogueCount = 0;
	InteractableType interactType = DialogueInteractable;
	InteractableItem item = InteractableItem::NA;
	float timer = 1000;
	float base = timer;
	std::vector<std::string> scriptVariables = std::vector<std::string>();
	std::vector<std::vector<TextDecorationSpan>> decorations; // can define multiple spans per variable
};

enum DialogueRequestType {
	StoryDialogue,
	InteractableDialogue,
	CallDialogue,
	ResetDialogue
};

struct DialogueRequest { 
	DialogueRequestType type = InteractableDialogue;
	int choice = -1;
};

// something changed, let the object know what
struct InteractableReaction {
	Entity object;
	int choice = -1;
	InteractableReaction(Entity& object, int choice) { this->object = object; this->choice = choice; };
};

struct NearbyInteractables {
	// placeholder component that has list of nearby interactibles the io system can respond to
};


struct InteractableInDialogue {
	// container to store the current interactable active in dialogue
};
