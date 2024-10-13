#pragma once
#include "common.hpp"
#include <vector>
#include <stack>
#include <unordered_map>
#include "../ext/stb_image/stb_image.h"

#include "map_components.hpp"
#include "actor_components.hpp"
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
struct EmitParticle {
	vec2 position;
	
};

//TODO add something to keep track of the sounds - soundType (background, sfx), volume, loop boolean
enum SoundType { Background, SFX };
struct Sound {
	SoundType type;
	float volume; //0 to 1
	bool isLooped;
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

// maybe a universal map would be easier to load + manage files with...
enum class TEXTURE_ASSET_ID {
	FISH = 0,
	PUFFERFISH = FISH + 1,
	CIRCLE = PUFFERFISH + 1,
	MC_BASE = CIRCLE + 1,
	MC_HIT = MC_BASE + 1,
	AIM_INDICATOR = MC_HIT + 1,
	FLOOR = AIM_INDICATOR + 1,
	MC_BULLET = FLOOR + 1,
	ENEMY_BULLET_SQUARE = MC_BULLET + 1,
	ENEMY_BULLET_CIRCLE = ENEMY_BULLET_SQUARE + 1,
	ENEMY_BULLET_TRIANGLE = ENEMY_BULLET_CIRCLE + 1,
	TEXTURE_COUNT = ENEMY_BULLET_TRIANGLE + 1
};
const int texture_count = (int)TEXTURE_ASSET_ID::TEXTURE_COUNT;

enum class EFFECT_ASSET_ID {
	COLOURED = 0,
	EGG = COLOURED + 1,
	SALMON = EGG + 1,
	TEXTURED = SALMON + 1,
	POSTPROCESS = TEXTURED + 1,
	EFFECT_COUNT = POSTPROCESS + 1
};
const int effect_count = (int)EFFECT_ASSET_ID::EFFECT_COUNT;

enum class GEOMETRY_BUFFER_ID {
	SALMON = 0,
	SPRITE = SALMON + 1,
	EGG = SPRITE + 1,
	DEBUG_LINE = EGG + 1,
	SCREEN_TRIANGLE = DEBUG_LINE + 1,
	GEOMETRY_COUNT = SCREEN_TRIANGLE + 1
};
const int geometry_count = (int)GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;

struct RenderRequest {
	TEXTURE_ASSET_ID used_texture = TEXTURE_ASSET_ID::TEXTURE_COUNT;
	EFFECT_ASSET_ID used_effect = EFFECT_ASSET_ID::EFFECT_COUNT;
	GEOMETRY_BUFFER_ID used_geometry = GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;
	bool show = true;
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
struct Sprites {
	// map of sprite type (enum) to sprite texture
	// eg: when bullet collides w/ enemy in physics system,
	// physics system will change the sprite to "DAMAGED_SPRITE"
	std::unordered_map<SPRITE_STATE, TEXTURE_ASSET_ID> sprites;
};

struct CollisionShape {
	// not sure if we need to draw that many
	std::vector<Entity> shapes;
};

// if a sprite should switch after a certain amount of time
struct SpriteTimer {
	float count_ms = 1000;
	TEXTURE_ASSET_ID nextSprite;
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

	// from experience, this is often a small number < 10, not sure why
	float scale; 
	glm::vec3 color;
};

struct DialogueLines {
	std::vector<std::string> lines;
	int current = 0;

	std::string next() {
		if (current < lines.size()) {
			return lines[current++];
		}
		else {
			return "<end>"; // maybe end of str constant
		}
	}
};

struct BG {

};

struct Fade {
	float max = 500;
	float time = max;
};