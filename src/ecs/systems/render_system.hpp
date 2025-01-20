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
#include "utils/ui_constants.hpp"
#include "components/presets/enemy_bullet_properties.hpp"

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
	std::array<GLuint, 256> texture_gl_handles;
	std::array<ivec2, 256> texture_dimensions;
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
		shader_path("postprocess1"),
		shader_path("postprocess2"),
		shader_path("postprocess3"),
		shader_path("dash"),
		shader_path("healthbar"),
		shader_path("roombound"),
		shader_path("animate"),
		shader_path("bullet")
	};

	std::array<GLuint, geometry_count> vertex_buffers;
	std::array<GLuint, geometry_count> index_buffers;
	std::array<Mesh, geometry_count> meshes;
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
	void drawGameOverlayUI();
	void drawGameUI();
	void drawMenuUI();
	void drawMenuOverlayUI();
	void drawDialogueUI();
	void drawBackgroundElements();
	void drawRoomBound(Entity entity, const mat4& projection, const mat4& view);
	void drawDash(Entity entity, const mat4& projection, const mat4& view);
	void drawToScreenExtra(EFFECT_ASSET_ID effect);
	void drawToScreenFinal();
	void step(float elapsed_ms);

	void drawCursor();

	void effectToDrawCall(Entity entity, const mat4& projection, const mat4& view, bool isUI);

	mat3 createProjectionMatrix();

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);


private:
	// Internal drawing functions for each entity type
	void drawTextured(Entity entity, const mat4& projection, const mat4& view, bool isUI);
	void drawAnimate(Entity entity, const mat4& projection, const mat4& view, bool isUI);
	void drawMesh(Entity entity, const mat4& projection, const mat4& view);
	void drawBullet(Entity entity, const mat4& projection, const mat4& view);
	void drawDashes(const mat4& projection, const mat4& view);
	void drawHPbar(Entity& entity, const mat4& projection, const mat4& view);
	void drawEnemyIndicator(Entity& enemy, const mat4& projection, const mat4& view);

	//void drawDashCharges(GLuint &VAO, GLuint &VBO, GLuint &EBO);
	void drawDashCharges(vec2 position, vec2 scale, int isCharging, float cooldown, float max, const mat4& projection, const mat4& view);
	void drawAllColliders(Entity entity, const mat4& projection, const mat4& view);
	void drawCollider(Entity entity, std::string shape, const mat4& projection, const mat4& view);
	void drawUIBullet(vec2 position, vec2 bullet_size, vec3 color, std::string shape, const mat4& projection, const mat4& view);
	void drawLaserIndicator(Entity entity, const mat4& projection, const mat4& view);
	void drawBulletStack(const mat4& projection, const mat4& view);

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
mat4 createFollowCameraModel(Motion& motion, vec2 offset);
mat4 createFollowCameraModelText(Motion& motion, vec2 offset);
mat4 createNormalModel(Motion& motion, vec2 offset);
// currently only seems to work for 0.5 and 1.0
const float clampAmount = 1.f;