#pragma once

#include <array>
#include <utility>
#include "common.hpp"

#if IMGUI_ENABLED
#include "imgui.h"
#endif

#include "components.hpp"
#include "actor_components.hpp"
#include "tiny_ecs.hpp"

//#include <experimental/filesystem>

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
	std::array<GLuint, 100> texture_gl_handles;
	std::array<ivec2, 100> texture_dimensions;
	GLuint vao;

	std::map<std::string, int> name_to_texture;

	// Make sure these paths remain in sync with the associated enumerators.
	// Associated id with .obj path
	const std::vector < std::pair<GEOMETRY_BUFFER_ID, std::string>> mesh_paths =
	{
		  std::pair<GEOMETRY_BUFFER_ID, std::string>(GEOMETRY_BUFFER_ID::MESH_GB, mesh_path("BigC.obj"))
		  // specify meshes of other assets here
	};

	std::array<GLuint, effect_count> effects;
	// Make sure these paths remain in sync with the associated enumerators.
	const std::array<std::string, effect_count> effect_paths = {
		shader_path("coloured"),
		shader_path("egg"),
		shader_path("mesh"),
		shader_path("textured"),
		shader_path("postprocess"),
		shader_path("dash"),
		shader_path("healthbar"),
		shader_path("roombound"),
		shader_path("animate"),
		shader_path("bullet")
	};

	std::array<GLuint, geometry_count> vertex_buffers;
	std::array<GLuint, geometry_count> index_buffers;
	std::array<Mesh, geometry_count> meshes;

	std::unordered_map<BulletEffectType, vec3> bulletEffectColors = {
			{BulletDamage,      {1.f, 0.f, 1.f}},
			{ProjectileSpeed,   {1.f, 0.f, 0.f}},
			{ProjectileSize,    {173.f / 255.f, 49.f / 255.f, 75.f / 255.f}},
			{FireRate,          {1.f, 1.f, 1.f}},
			{BulletRange,       {0.f, 0.f, 1.f}},
			{BulletSpread,      {1.f, 0.f, 0.f}},
			{BulletNum,         {0.f, 0.f, 1.f}},
			{BulletBurst,       {0.f, 0.f, 0.f}},
			{Bounce,            {0.f, 1.f, 0.f}},
			{Pierce,            {1.f, 0.f, 0.f}},
			{Homing,            {1.f, 0.f, 0.f}},
			{PlayerSpeed,       {0.f, 0.f, 1.f}},
			{PlayerNumDash,     {0.f, 1.f, 1.f}},
			{PlayerStackSize,   {0.f, 1.f, 0.f}},
			{PlayerDashCDR,     {1.f, 0.f, 0.f}},
			{Inert,             {91.f / 255.f, 99.f / 255.f, 128.f / 255.f}},
			{Key,				{1.0, 1.0, 1.0}}
	};

	std::unordered_map<BulletEffectType, std::string> bulletEffectShapes = {
			{BulletDamage,      "enemy_bullet_square.png"},
			{ProjectileSpeed,   "enemy_bullet_square.png"},
			{ProjectileSize,    "enemy_bullet_square.png"},
			{FireRate,          "enemy_bullet_square.png"},
			{BulletRange,       "enemy_bullet_square.png"},
			{BulletSpread,      "enemy_bullet_square.png"},
			{BulletNum,         "enemy_bullet_square.png"},
			{BulletBurst,       "enemy_bullet_square.png"},
			{Bounce,            "enemy_bullet_square.png"},
			{Pierce,            "enemy_bullet_square.png"},
			{Homing,            "enemy_bullet_square.png"},
			{PlayerSpeed,       "enemy_bullet_square.png"},
			{PlayerNumDash,     "enemy_bullet_square.png"},
			{PlayerStackSize,   "enemy_bullet_square.png"},
			{PlayerDashCDR,     "enemy_bullet_square.png"},
			{Inert,             "enemy_bullet_square.png"},
			{Key,				"enemy_bullet_key (2).png"} 
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
	void drawMenuOverlayUI();
	void drawDialogueUI();
	void drawBackgroundElements();
	void drawToScreen();
	void step(float elapsed_ms);

	void drawCursor();


	mat3 createProjectionMatrix();

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);


private:
	// Internal drawing functions for each entity type
	void drawTexturedMesh(Entity entity, const mat3& projection);
	void drawMesh(Entity entity, const mat3& projection);
	void drawDashes(const mat3& projection);
	void drawHPbar(::Entity &entity, const mat3 & projection);
	//glm::mat4 createTransform(float x, float y, float scaleX, float scaleY);

	//void drawDashCharges(GLuint &VAO, GLuint &VBO, GLuint &EBO);
	void drawDashCharges(vec2 position, vec2 scale, int isCharging, float cooldown, float max, const mat3 & projection);
	void drawAllColliders(Entity entity, const mat3& projection_2D);
	void drawCollider(Entity entity, std::string shape,  const mat3& projection);
	void drawUIBullet(vec2 position, vec2 bullet_size, vec3 color, std::string shape, const mat3& projection);
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

		void initImGui();
		void drawImGui();
	#endif
};

bool loadEffectFromFile(
	const std::string& vs_path, const std::string& fs_path, GLuint& out_program);