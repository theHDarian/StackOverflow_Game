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

//TODO add something to keep track of the sounds - soundType (background, sfx), volume, loop boolean
enum SoundType {
	normalBGM,
	bossBGM,
	specialBGM,
	PlayerHurt,
	DashSound,
	PlayerShoot,
	EnemyShoot,
	IncomingDialogue,
	NormalDialogue,
	DoorOpen,
};
struct SoundRequest {
	SoundType type;
	std::string path;
	float volume; //0 to 1
	int loops;
	float ticks;
	int songIndex;
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
	COLOURED = 0,
	EGG = COLOURED + 1,
	MESH = EGG + 1,
	TEXTURED = MESH + 1,
	POSTPROCESS = TEXTURED + 1,
	DASH = POSTPROCESS + 1,
	HP_BAR = DASH + 1,
	ROOM_BOUND = HP_BAR + 1,
	ANIMATE = ROOM_BOUND + 1,
	BULLET = ANIMATE + 1,
	EFFECT_COUNT = BULLET + 1
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
};

enum AnimationTypes {
	NONE = 0,
	REGULAR = 1,
	ONCE = 2
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
	glm::vec3 color;

	// size of text box
	vec2 topRightBound;
	vec2 bottomLeftBound;

	// temp: add for now if it is preloaded
	std::vector<std::string> tokenizedText = std::vector<std::string>();
};

struct Dialogue {
	std::string text;
	std::vector<std::string> tokenizedText;
	std::string speakerName;
	std::string speakerAvatar;
	std::vector<std::string> choices;
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

struct DialogueChoice {

};

enum InteractableType {
	DialogueInteractable,
	ActionInteractable
};

enum InteractableItem {
	Ram,
	PopConsole,
	PushConsole,
	OpenDoor,
	LockedDoor,
	BibleTree,
	Gardener,
	NA,
};

struct InteractableObject {
	std::string name;
	int dialogueCount = 0;
	InteractableType interactType = DialogueInteractable;
	InteractableItem item = InteractableItem::NA;
};

enum DialogueRequestType {
	StoryDialogue,
	InteractableDialogue
};

struct DialogueRequest { // consider adding req types, so that dialogue system knows what type (story/interactible)
	DialogueRequestType type = InteractableDialogue;
	int choice = -1;
};

struct InteractableReaction {
	// something changed, let the object know what
	Entity object;
	int choice = -1;
	InteractableReaction(Entity& object, int choice) { this->object = object; this->choice = choice; };
};

struct NearbyInteractables {
	// placeholder component that has list of nearby interactibles the io system can respond to
};