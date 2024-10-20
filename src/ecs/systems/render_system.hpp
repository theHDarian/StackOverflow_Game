#pragma once

#include <array>
#include <utility>
#include "common.hpp"

#if IMGUI_ENABLED
#include "imgui.h"
#endif

#include "components.hpp"
#include "tiny_ecs.hpp"

// System responsible for setting up OpenGL and for rendering all the
// visual entities in the game
class RenderSystem {
	/**
	 * The following arrays store the assets the game will use. They are loaded
	 * at initialization and are assumed to not be modified by the render loop.
	 *
	 * Whenever possible, add to these lists instead of creating dynamic state
	 * it is easier to debug and faster to execute for the computer.
	 */
	std::array<GLuint, texture_count> texture_gl_handles;
	std::array<ivec2, texture_count> texture_dimensions;
	GLuint vao;

	// Make sure these paths remain in sync with the associated enumerators.
	// Associated id with .obj path
	const std::vector < std::pair<GEOMETRY_BUFFER_ID, std::string>> mesh_paths =
	{
		  std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::SALMON_GB, mesh_path("salmon.obj"))
		  // specify meshes of other assets here
	};

	// Make sure these paths remain in sync with the associated enumerators.
	const std::array<std::string, texture_count> texture_paths = {
			textures_path("green_fish.png"),
			textures_path("enemy_Pufferfish.png"),
			textures_path("circle.png"),
			textures_path("mcv1_base.png"), 
			textures_path("mcv1_hit.png"),
			textures_path("aim_indicator.png"),
			textures_path("blankFloor.png"),
			textures_path("player_bullet.png"),
			textures_path("enemy_bullet_square.png"),
			textures_path("enemy_bullet_circle.png"),
			textures_path("enemy_bullet_triangle.png"),
			textures_path("chevron.png"),
	};

	std::array<GLuint, effect_count> effects;
	// Make sure these paths remain in sync with the associated enumerators.
	const std::array<std::string, effect_count> effect_paths = {
		shader_path("coloured"),
		shader_path("egg"),
		shader_path("salmon"),
		shader_path("textured"),
		shader_path("postprocess"),
		shader_path("dash"),
	};

	std::array<GLuint, geometry_count> vertex_buffers;
	std::array<GLuint, geometry_count> index_buffers;
	std::array<Mesh, geometry_count> meshes;

	std::unordered_map<BulletEffectType, vec3> bulletEffectColors = {
			{BulletDamage,      {1.f, 0.f, 1.f}},
			{ProjectileSpeed,   {1.f, 0.f, 0.f}},
			{ProjectileSize,    {173 / 255.f, 49 / 255.f, 75 / 255.f}},
			{FireRate,          {1.f, 1.f, 1.f}},
			{BulletRange,       {0.f, 0.f, 1.f}},
			{BulletSpread,      {1.f, 0.f, 0.f}},
			{BulletNum,         {0.f, 0.f, 1.f}},
			{BulletBurst,       {0.f, 0.f, 0.f}},
			{Bounce,            {0.f, 1.f, 0.f}},
			{Pierce,            {1.f, 0.f, 0.f}},
			{Homing,            {1.f, 0.f, 0.f}},
			{PlayerSpeed,       {0.f, 0.f, 1.f}},
			{PlayerNumDash,     {0.f, 0.f, 1.f}},
			{PlayerStackSize,   {0.f, 1.f, 0.f}},
			{PlayerDashCDR,     {1.f, 0.f, 0.f}},
			{Inert,             {91 / 255.f, 99 / 255.f, 128 / 255.f}}
	};

public:
	// Initialize the window
	bool init(GLFWwindow* window);

	template <class T>
	void bindVBOandIBO(GEOMETRY_BUFFER_ID gid, std::vector<T> vertices, std::vector<uint16_t> indices);

	void initializeGlTextures();

	void initializeGlEffects();

	void initializeGlMeshes();
	Mesh& getMesh(GEOMETRY_BUFFER_ID id) { return meshes[(int)id]; };

	void initializeGlGeometryBuffers();
	// Initialize the screen texture used as intermediate render target
	// The draw loop first renders to this texture, then it is used for the wind
	// shader
	bool initScreenTexture();

	// Destroy resources associated to one or all entities created by the system
	~RenderSystem();

	// Draw all entities
	void drawSetupFrame();
	void drawGameElements();
	void drawGameUI();
	void drawMenuUI();
	void drawDialogueUI();
	void drawBackgroundElements();
	void drawToScreen();
	void step(float elapsed_ms);


	mat3 createProjectionMatrix();

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);


private:
	// Internal drawing functions for each entity type
	void drawTexturedMesh(Entity entity, const mat3& projection);
	void drawCircleCollider(Entity entity, const mat3& projection);
	void drawDashes(const mat3& projection);
	//glm::mat4 createTransform(float x, float y, float scaleX, float scaleY);


	//void drawDashCharges(GLuint &VAO, GLuint &VBO, GLuint &EBO);
	void drawDashCharges(vec2 position, vec2 scale, int isCharging, float cooldown, float max, const mat3 & projection);

	void drawUIBullet(vec2 position, vec2 bullet_size, vec3 color, TEXTURE_ASSET_ID shape, const mat3& projection);
	void drawBulletStack(const mat3& projection);

	// Window handle
	GLFWwindow* window;

	// Screen texture handles
	GLuint frame_buffer;
	GLuint off_screen_render_buffer_color;
	GLuint off_screen_render_buffer_depth;

	Entity screen_state_entity;

	#if IMGUI_ENABLED
	public:
		ImGuiContext* imgui_context;
	private:
		void initImGui();
		void drawImGui();
	#endif
};

bool loadEffectFromFile(
	const std::string& vs_path, const std::string& fs_path, GLuint& out_program);