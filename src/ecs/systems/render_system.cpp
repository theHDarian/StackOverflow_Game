// internal
#include "render_system.hpp"
#include <SDL.h>
#include <glm/gtx/compatibility.hpp>

#include "ai_system.hpp"
#include "tiny_ecs_registry.hpp"
#include "world_system.hpp"
#include "../utils/enum_string_mapping.hpp"


#if IMGUI_ENABLED
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"
#endif

void RenderSystem::step(float elapsed_ms) {
	if (registry.fades.entities.size() > 0) {
		for (auto& fadeEntity : registry.fades.entities) {
			auto& fade = registry.fades.get(fadeEntity);
			fade.time -= elapsed_ms;
		}
	}
}

void RenderSystem::drawTexturedMesh(Entity entity,
									const mat3 &projection)
{
	Motion &motion = registry.motions.get(entity);
	// Transformation code, see Rendering and Transformation in the template
	// specification for more info Incrementally updates transformation matrix,
	// thus ORDER IS IMPORTANT
	Transform transform;
	vec2 offset = registry.renderRequests.get(entity).offset;;

	transform.translate(motion.position);
	transform.rotate(motion.angle);
	transform.translate(offset);
	transform.scale(motion.scale);

	assert(registry.renderRequests.has(entity));
	const RenderRequest &render_request = registry.renderRequests.get(entity);

	const GLuint used_effect_enum = static_cast<GLuint>(render_request.used_effect);
	assert(used_effect_enum < static_cast<GLuint>(EFFECT_ASSET_ID::EFFECT_COUNT));
	const GLuint program = (GLuint)effects[used_effect_enum];

	// Setting shaders
	glUseProgram(program);
	gl_has_errors();

	assert(render_request.used_geometry < GEOMETRY_BUFFER_ID::GEOMETRY_COUNT);
	const GLuint vbo = vertex_buffers[(GLuint)render_request.used_geometry];
	const GLuint ibo = index_buffers[(GLuint)render_request.used_geometry];

	// Setting vertex and index buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	gl_has_errors();

	// Input data location as in the vertex buffer
	if (render_request.used_effect == EFFECT_ASSET_ID::TEXTURED)
	{
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");
		gl_has_errors();
		assert(in_texcoord_loc >= 0);

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_texcoord_loc);
		glVertexAttribPointer(in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)sizeof(vec3));
		gl_has_errors();


		// Enable and bind the texture to slot 0
		glActiveTexture(GL_TEXTURE0);
		gl_has_errors();
		assert(registry.renderRequests.has(entity));
		GLuint texture_id = texture_gl_handles[(GLuint)registry.renderRequests.get(entity).used_texture];
		glBindTexture(GL_TEXTURE_2D, texture_id);
		gl_has_errors();
	}
	else if (render_request.used_effect == EFFECT_ASSET_ID::SALMON || render_request.used_effect == EFFECT_ASSET_ID::EGG)
	{
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_color_loc = glGetAttribLocation(program, "in_color");
		gl_has_errors();

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
							  sizeof(ColoredVertex), (void *)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_color_loc);
		glVertexAttribPointer(in_color_loc, 3, GL_FLOAT, GL_FALSE,
							  sizeof(ColoredVertex), (void *)sizeof(vec3));
		gl_has_errors();
	}
	else
	{
		assert(false && "Type of render request not supported");
	}

	// Getting uniform locations for glUniform* calls
	GLint color_uloc = glGetUniformLocation(program, "fcolor");
	const vec3 color = registry.colors.has(entity) ? registry.colors.get(entity) : vec3(1);
	glUniform3fv(color_uloc, 1, (float *)&color);
	GLint change_color_uloc = glGetUniformLocation(program, "changeColor");
	glUniform1i(change_color_uloc, 0);

	float alpha = 1;

	// fade out entity if needed
	if (registry.fades.has(entity)) {
		Fade& fade = registry.fades.get(entity);
		alpha = glm::lerp(1.f, 0.f, (fade.max - fade.time) / fade.max);
		vec3 color = { 1.2, 0.5, 0.5 }; // red
		glUniform3fv(color_uloc, 1, (float*)&color);
		glUniform1i(change_color_uloc, 1);
		GLint effectAlpha = glGetUniformLocation(program, "effectAlpha");
		glUniform1f(effectAlpha, alpha);
	}

	GLint alpha_uloc = glGetUniformLocation(program, "alpha");
	glUniform1f(alpha_uloc, alpha);
	gl_has_errors();

	// Change color if entity is invincible
	if (registry.invincibles.has(entity)) {
		Invincible& invincible = registry.invincibles.get(entity);
		vec3 color = { 1.2, 1.2, 1.2 }; // grey
		glUniform3fv(color_uloc, 1, (float*)&color);
		glUniform1i(change_color_uloc, 1);
		GLint effectAlpha = glGetUniformLocation(program, "effectAlpha");
		alpha = glm::lerp(0.7f, 0.f, ( invincible.max - invincible.countdown) / invincible.max);
		glUniform1f(effectAlpha, alpha);
	}

	if (registry.damageds.has(entity)) {
		Damaged& damaged = registry.damageds.get(entity);
		vec3 color = { 1.2, 0.5, 0.5 }; // red
		glUniform3fv(color_uloc, 1, (float*)&color);
		glUniform1i(change_color_uloc, 1);
		GLint effectAlpha = glGetUniformLocation(program, "effectAlpha");
		alpha = glm::lerp(1.f, 0.f, ( damaged.max - damaged.countdown) / damaged.max);
		glUniform1f(effectAlpha, alpha);
	}

	// Get number of indices from index buffer, which has elements uint16_t
	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	gl_has_errors();

	GLsizei num_indices = size / sizeof(uint16_t);
	// GLsizei num_triangles = num_indices / 3;

	GLint currProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
	// Setting uniform values to the currently bound program
	GLuint transform_loc = glGetUniformLocation(currProgram, "transform");
	glUniformMatrix3fv(transform_loc, 1, GL_FALSE, (float *)&transform.mat);
	GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
	glUniformMatrix3fv(projection_loc, 1, GL_FALSE, (float *)&projection);
	gl_has_errors();
	// Drawing of num_indices/3 triangles specified in the index buffer
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);
	gl_has_errors();
}

// draw the intermediate texture to the screen, with some distortion to simulate
// water
void RenderSystem::drawToScreen()
{
	// Setting shaders
	// get the water texture, sprite mesh, and program
	glUseProgram(effects[(GLuint)EFFECT_ASSET_ID::POSTPROCESS]);
	glBindVertexArray(vao);
	gl_has_errors();
	// Clearing backbuffer
	int w, h;
	glfwGetFramebufferSize(window, &w, &h); // Note, this will be 2x the resolution given to glfwCreateWindow on retina displays
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, w, h);
	glDepthRange(0, 10);
	glClearColor(1.f, 0, 0, 1.0);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gl_has_errors();
	// Enabling alpha channel for textures
	glDisable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	// Draw the screen texture on the quad geometry
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[(GLuint)GEOMETRY_BUFFER_ID::SCREEN_TRIANGLE]);
	glBindBuffer(
		GL_ELEMENT_ARRAY_BUFFER,
		index_buffers[(GLuint)GEOMETRY_BUFFER_ID::SCREEN_TRIANGLE]); // Note, GL_ELEMENT_ARRAY_BUFFER associates
																	 // indices to the bound GL_ARRAY_BUFFER
	gl_has_errors();
	const GLuint postprocess_program = effects[(GLuint)EFFECT_ASSET_ID::POSTPROCESS];
	// Set clock
	GLuint time_uloc = glGetUniformLocation(postprocess_program, "time");
	GLuint chrom_abb_intensity_uloc = glGetUniformLocation(postprocess_program, "chromatic_abberation_intensity");
	glUniform1f(time_uloc, (float)(glfwGetTime() * 10.0f));
	StackCompile &stack = registry.stackCompile.get(registry.players.entities[0]);
	float intensity = (float)stack.currStack.size() / ((stack.baseStackSize + stack.additives[PlayerStackSize]) * stack.multiplicatives[PlayerStackSize]);
	glUniform1f(chrom_abb_intensity_uloc, intensity);
	gl_has_errors();
	// Set the vertex position and vertex texture coordinates (both stored in the
	// same VBO)
	GLint in_position_loc = glGetAttribLocation(postprocess_program, "in_position");
	glEnableVertexAttribArray(in_position_loc);
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void *)0);
	gl_has_errors();

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	Frame& frame = registry.frames.components[0];
	glBindTexture(GL_TEXTURE_2D, frame.prevTexture);
	gl_has_errors();
	// Draw
	glDrawElements(
		GL_TRIANGLES, 3, GL_UNSIGNED_SHORT,
		nullptr); // one triangle = 3 vertices; nullptr indicates that there is
				  // no offset from the bound index buffer
	glBindVertexArray(0);
	gl_has_errors();
}

void RenderSystem::drawSetupFrame(){
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	Frame& frame = registry.frames.components[0];
	if (frame.prevFrameBuffer == frame_buffer) return;

	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	glViewport(0, 0, w, h);
	glDepthRange(0.00001, 10);
	glClearColor(GLfloat(32/ 255), GLfloat(43 / 255), GLfloat(81 / 255), 1.0);
	glClearDepth(10.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	if(frame.prevFrameBuffer != 0) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, frame.prevFrameBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame_buffer);

		//copy contents of previous buffer to current
		glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		gl_has_errors();
		glBindFramebuffer(GL_FRAMEBUFFER,frame_buffer);
	}
	frame.prevFrameBuffer = frame_buffer;
	frame.prevTexture = off_screen_render_buffer_color;
}

void RenderSystem::drawBackgroundElements() {
	drawSetupFrame();
	mat3 projection_2D = createProjectionMatrix();
	glBindVertexArray(vao);
	for (Entity entity : registry.backgrounds.entities) {
		if (!registry.renderRequests.get(entity).show)
			continue;
		
		drawTexturedMesh(entity, projection_2D);
	}
	glBindVertexArray(0);
}

// Render our game world
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
void RenderSystem::drawGameElements()
{
	drawSetupFrame();

	mat3 projection_2D = createProjectionMatrix();
	glBindVertexArray(vao);

	// this setup requires us know what types of things to render
	// and won't render all render requests if not given the proper component
	// Note, its not very efficient to access elements indirectly via the entity
	// albeit iterating through all Sprites in sequence. A good point to optimize
	for (Entity& entity : registry.enemyBullets.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || registry.invisibles.has(entity))
			continue;
		drawTexturedMesh(entity, projection_2D);
		if (registry.circleColliders.has(entity))
			drawCircleCollider(entity, projection_2D);
	}

	for (Entity& entity : registry.playerBullets.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || registry.invisibles.has(entity))
			continue;
		drawTexturedMesh(entity, projection_2D);
		if (registry.circleColliders.has(entity))
			drawCircleCollider(entity, projection_2D);
	}

	for (Entity& entity : registry.enemies.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || registry.invisibles.has(entity))
			continue;
		drawTexturedMesh(entity, projection_2D);
		if (registry.circleColliders.has(entity))
			drawCircleCollider(entity, projection_2D);
	}

	for (Entity& entity : registry.players.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || registry.invisibles.has(entity))
			continue;
		drawTexturedMesh(entity, projection_2D);
		if (registry.circleColliders.has(entity))
			drawCircleCollider(entity, projection_2D);
	}

	glBindVertexArray(0);
	gl_has_errors();

}

void RenderSystem::drawGameUI() {
	drawSetupFrame();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(vao);
	mat3 projection_2D = createProjectionMatrix();

	for (Entity& entity : registry.gameUIs.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || !registry.renderRequests.get(entity).show)
			continue;

		// figure out how to modularize drawing this later; maybe each bullet is an entity
		if (registry.stackUI.has(entity)) {
			drawBulletStack(projection_2D);
		}
		else {
			drawTexturedMesh(entity, projection_2D);
		}
	}

	drawDashes(projection_2D);

	glBindVertexArray(0);
	gl_has_errors();
}

void RenderSystem::drawDialogueUI() {
	drawSetupFrame();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(vao);
	mat3 projection_2D = createProjectionMatrix();

	for (Entity& entity : registry.dialogueUIs.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || !registry.renderRequests.get(entity).show)
			continue;
		drawTexturedMesh(entity, projection_2D);
	}

	glBindVertexArray(0);
	gl_has_errors();
}

void RenderSystem::drawMenuUI() {
	drawSetupFrame();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(vao);
	mat3 projection_2D = createProjectionMatrix();

	for (Entity& entity : registry.menuUIs.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || !registry.renderRequests.get(entity).show)
			continue;
		drawTexturedMesh(entity, projection_2D);
	}

	glBindVertexArray(0);
	gl_has_errors();

	#if IMGUI_ENABLED
		//draw Imgui
		drawImGui();
	#endif
}

mat3 RenderSystem::createProjectionMatrix()
{
	// Fake projection matrix, scales with respect to window coordinates
	float left = 0.f;
	float top = 0.f;

	gl_has_errors();
	WindowState& windowState = registry.windowStates.components[0];
	float right = (float) windowState.width;
	float bottom = (float) windowState.height;

	float sx = 2.f / (right - left);
	float sy = 2.f / (top - bottom);
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	return {{sx, 0.f, 0.f}, {0.f, sy, 0.f}, {tx, ty, 1.f}};
}

#if IMGUI_ENABLED
void RenderSystem::drawImGui() {
	int menuWidth = 200;
	IOState& ioState = registry.ioStates.components[0];
	WindowState& windowState = registry.windowStates.components[0];
	// std::cout << windowState.width << " " << windowState.height << std::endl;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	const ImVec2& size = ImVec2(menuWidth,windowState.height);
	ImGui::SetNextWindowSize(size);
	ImGui::SetNextWindowPos(ImVec2(windowState.width - menuWidth, 0));
	ImGui::Begin("Debug window");

	// BASIC INFORMATION
    ImGui::Text("Enemy Bullet Count: %lu",registry.enemyBullets.size());
	ImGui::Text("Viewport Size: (%d, %d)",windowState.width,windowState.height);
	ImGui::Text("Mouse Pos: (%.2f, %.2f)",ioState.mousePosition.x,ioState.mousePosition.y);
	
	// STACK INFORMATION
	StackCompile& sc = registry.stackCompile.components[0];
	ImGui::Text("Stack Size: %lu", sc.currStack.size());
	ImGui::TextColored(ImVec4(1,1,0,1), "Additives");
	ImGui::BeginChild("AdditiveContent",ImVec2(180,250),true);
		std::map<BulletEffectType, float>::iterator it;
		for (it = sc.additives.begin(); it != sc.additives.end(); it++) {
			// if (it->second == 0) continue;
			ImGui::Text("%s: %.1f", bulletEffectTypeNames[it->first].c_str(), it->second);
		}
	ImGui::EndChild();

	ImGui::TextColored(ImVec4(1,1,0,1), "Multiplicatives");
	ImGui::BeginChild("MultiplicativeContent",ImVec2(180,250),true);
		for (it = sc.multiplicatives.begin(); it != sc.multiplicatives.end(); it++) {
			// if (it->second == 1) continue;
			ImGui::Text("%s: %.1f", bulletEffectTypeNames[it->first].c_str(), it->second);
		}
	ImGui::EndChild();

	if (ImGui::Button("Restart Game")) {
		registry.ioStates.components[0].shouldRestart = true;
	}
    ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGui::UpdatePlatformWindows();
}
#endif

// should really consider making a draw textured mesh function without relying on an entity/for UI
// currently just draws a box as a container
void RenderSystem::drawBulletStack(const mat3& projection) {
	StackCompile& stack = registry.stackCompile.get(registry.players.entities[0]);
	
	Entity stackEntity = registry.stackUI.entities[0];
	StackUI& stackui = registry.stackUI.get(stackEntity);
	
	Transform transform;
	transform.translate(stackui.stackPos);
	transform.scale(stackui.stackSize);

	vec3 color = { 11/255.f, 84/255.f, 87/255.f };
	const GLuint used_effect_enum = (GLuint)EFFECT_ASSET_ID::TEXTURED;
	const GLuint program = (GLuint)effects[used_effect_enum];

	// Setting shaders
	glUseProgram(program);
	gl_has_errors();

	const GLuint vbo = vertex_buffers[(GLuint)GEOMETRY_BUFFER_ID::SPRITE];
	const GLuint ibo = index_buffers[(GLuint)GEOMETRY_BUFFER_ID::SPRITE];

	// Setting vertex and index buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	gl_has_errors();

	GLint in_position_loc = glGetAttribLocation(program, "in_position");
	GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");
	gl_has_errors();
	assert(in_texcoord_loc >= 0);

	glEnableVertexAttribArray(in_position_loc);
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
		sizeof(TexturedVertex), (void*)0);
	gl_has_errors();

	glEnableVertexAttribArray(in_texcoord_loc);
	glVertexAttribPointer(
		in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
		(void*)sizeof(
			vec3)); // note the stride to skip the preceeding vertex position

	// Enabling and binding texture to slot 0
	glActiveTexture(GL_TEXTURE0);
	gl_has_errors();

	GLuint texture_id =
		texture_gl_handles[(GLuint)TEXTURE_ASSET_ID::ENEMY_BULLET_SQUARE];

	glBindTexture(GL_TEXTURE_2D, texture_id);
	gl_has_errors();

	// Getting uniform locations for glUniform* calls
	GLint color_uloc = glGetUniformLocation(program, "fcolor");
	glUniform3fv(color_uloc, 1, (float*)&color);
	// want to overwrite the colour with given; could also use a separate shader program
	GLint change_color_uloc = glGetUniformLocation(program, "changeColor");
	glUniform1i(change_color_uloc, 1);
	GLint alpha_uloc = glGetUniformLocation(program, "alpha");
	glUniform1f(alpha_uloc, 1);
	GLint effect_alpha_uloc = glGetUniformLocation(program, "effectAlpha");
	glUniform1f(effect_alpha_uloc, 1);
	gl_has_errors();

	// Get number of indices from index buffer, which has elements uint16_t
	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	gl_has_errors();

	GLsizei num_indices = size / sizeof(uint16_t);
	// GLsizei num_triangles = num_indices / 3;

	GLint currProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
	// Setting uniform values to the currently bound program
	GLuint transform_loc = glGetUniformLocation(currProgram, "transform");
	glUniformMatrix3fv(transform_loc, 1, GL_FALSE, (float*)&transform.mat);
	GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
	glUniformMatrix3fv(projection_loc, 1, GL_FALSE, (float*)&projection);
	gl_has_errors();
	// Drawing of num_indices/3 triangles specified in the index buffer
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);
	gl_has_errors();

	// draw bullet stack here for now, based on bullet effects
	for (int i = 0; i < stack.currStack.size(); i++) {
		// no variance on shape for now
		TEXTURE_ASSET_ID bulletShape = TEXTURE_ASSET_ID::ENEMY_BULLET_CIRCLE;
		// start from bottom to top
		drawUIBullet(vec2(stackui.bulletStartPos.x, stackui.bulletStartPos.y - i * stackui.bulletSize.y - i * stackui.bulletOffset), stackui.bulletSize,
			bulletEffectColors[stack.currStack[i].type], bulletShape, projection);
	}
}

void RenderSystem::drawUIBullet(vec2 position, vec2 bullet_size, vec3 color, TEXTURE_ASSET_ID shape, const mat3& projection) {
	Transform transform;
	transform.translate(position);
	transform.scale(bullet_size);

	// for now, draw bullets using textures
	const GLuint used_effect_enum = (GLuint)EFFECT_ASSET_ID::TEXTURED;
	const GLuint program = (GLuint)effects[used_effect_enum];

	// Setting shaders
	glUseProgram(program);
	gl_has_errors();

	const GLuint vbo = vertex_buffers[(GLuint)GEOMETRY_BUFFER_ID::SPRITE];
	const GLuint ibo = index_buffers[(GLuint)GEOMETRY_BUFFER_ID::SPRITE];

	// Setting vertex and index buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	gl_has_errors();

	GLint in_position_loc = glGetAttribLocation(program, "in_position");
	GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");
	gl_has_errors();
	assert(in_texcoord_loc >= 0);

	glEnableVertexAttribArray(in_position_loc);
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
		sizeof(TexturedVertex), (void*)0);
	gl_has_errors();

	glEnableVertexAttribArray(in_texcoord_loc);
	glVertexAttribPointer(
		in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
		(void*)sizeof(
			vec3)); // note the stride to skip the preceeding vertex position

	// Enabling and binding texture to slot 0
	glActiveTexture(GL_TEXTURE0);
	gl_has_errors();

	GLuint texture_id =
		texture_gl_handles[(GLuint)shape];

	glBindTexture(GL_TEXTURE_2D, texture_id);
	gl_has_errors();

	// Getting uniform locations for glUniform* calls
	GLint color_uloc = glGetUniformLocation(program, "fcolor");
	glUniform3fv(color_uloc, 1, (float*)&color);
	// want to overwrite the colour with given; could also use a separate shader program
	GLint change_color_uloc = glGetUniformLocation(program, "changeColor");
	glUniform1i(change_color_uloc, 1);
	GLint alpha_uloc = glGetUniformLocation(program, "alpha");
	glUniform1f(alpha_uloc, 1);
	gl_has_errors();

	// Get number of indices from index buffer, which has elements uint16_t
	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	gl_has_errors();

	GLsizei num_indices = size / sizeof(uint16_t);
	// GLsizei num_triangles = num_indices / 3;

	GLint currProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
	// Setting uniform values to the currently bound program
	GLuint transform_loc = glGetUniformLocation(currProgram, "transform");
	glUniformMatrix3fv(transform_loc, 1, GL_FALSE, (float*)&transform.mat);
	GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
	glUniformMatrix3fv(projection_loc, 1, GL_FALSE, (float*)&projection);
	gl_has_errors();
	// Drawing of num_indices/3 triangles specified in the index buffer
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);
	gl_has_errors();
}

// draws outlines for collision circles; considering just passing in circle data
void RenderSystem::drawCircleCollider(Entity entity, const mat3& projection) {
	Motion& motion = registry.motions.get(entity);
	auto& circle = registry.circleColliders.get(entity);

	Transform transform;
	transform.translate(motion.position);
	transform.rotate(motion.angle);
	transform.scale({ circle.radius * 2, circle.radius * 2});

	const GLuint used_effect_enum = (GLuint)EFFECT_ASSET_ID::TEXTURED;
	assert(used_effect_enum != (GLuint)EFFECT_ASSET_ID::EFFECT_COUNT);
	const GLuint program = (GLuint)effects[used_effect_enum];

	// Setting shaders
	glUseProgram(program);
	gl_has_errors();

	const GLuint vbo = vertex_buffers[(GLuint)GEOMETRY_BUFFER_ID::SPRITE];
	const GLuint ibo = index_buffers[(GLuint)GEOMETRY_BUFFER_ID::SPRITE];

	// Setting vertex and index buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	gl_has_errors();

	GLint in_position_loc = glGetAttribLocation(program, "in_position");
	GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");
	gl_has_errors();
	assert(in_texcoord_loc >= 0);

	glEnableVertexAttribArray(in_position_loc);
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
		sizeof(TexturedVertex), (void*)0);
	gl_has_errors();

	glEnableVertexAttribArray(in_texcoord_loc);
	glVertexAttribPointer(
		in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
		(void*)sizeof(
			vec3)); // note the stride to skip the preceeding vertex position

	// Enabling and binding texture to slot 0
	glActiveTexture(GL_TEXTURE0);
	gl_has_errors();

	assert(registry.renderRequests.has(entity));
	GLuint texture_id =
		texture_gl_handles[(GLuint)TEXTURE_ASSET_ID::CIRCLE_SPRITE];

	glBindTexture(GL_TEXTURE_2D, texture_id);
	gl_has_errors();

	// Get number of indices from index buffer, which has elements uint16_t
	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	gl_has_errors();

	GLsizei num_indices = size / sizeof(uint16_t);
	// GLsizei num_triangles = num_indices / 3;

	GLint currProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
	// Setting uniform values to the currently bound program
	GLuint transform_loc = glGetUniformLocation(currProgram, "transform");
	glUniformMatrix3fv(transform_loc, 1, GL_FALSE, (float*)&transform.mat);
	GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
	glUniformMatrix3fv(projection_loc, 1, GL_FALSE, (float*)&projection);
	gl_has_errors();
	// Drawing of num_indices/3 triangles specified in the index buffer
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);
	gl_has_errors();
}

void RenderSystem::drawDashes(const mat3& projection) {
	drawSetupFrame();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	vec2 pos = { 100,100 };
	vec2 scale = { 50, 50 };
	float offset = 10;

    Player& player = registry.players.get(registry.players.entities[0]);

	// draw all filled dash charges first
    for (int i = 0; i < player.currDashCharges; ++i) {
        drawDashCharges(vec2(pos.x + i * (scale.x + offset), pos.y), scale, false, 0, 0, projection);
    }

	// draw currently charging dash charge, if any
	if (player.currDashCharges < WorldSystem::getModifiedValue(PlayerDashCDR,player.maxDashCharges)) {
		drawDashCharges(vec2(pos.x + player.currDashCharges * (scale.x + offset), pos.y), scale, true, player.currDashCooldown, WorldSystem::getModifiedValue(PlayerDashCDR, player.baseDashCDR), projection);
	}

	// draw empty dash charges last, if any
	for (int i = player.currDashCharges + 1; i < WorldSystem::getModifiedValue(PlayerDashCDR,player.maxDashCharges); ++i) {
		drawDashCharges(vec2(pos.x + i * (scale.x + offset), pos.y), scale, -1, 0, 0, projection);
	}

	gl_has_errors();
}

void RenderSystem::drawDashCharges(vec2 position, vec2 scale, int isCharging, float cooldown, float max, const mat3& projection) {
	Transform transform;
	transform.translate(position);
	transform.scale(scale);

	// for now, draw bullets using textures
	const GLuint used_effect_enum = (GLuint)EFFECT_ASSET_ID::DASH;
	const GLuint program = (GLuint)effects[used_effect_enum];

	// Setting shaders
	glUseProgram(program);
	gl_has_errors();

	const GLuint vbo = vertex_buffers[(GLuint)GEOMETRY_BUFFER_ID::SPRITE];
	const GLuint ibo = index_buffers[(GLuint)GEOMETRY_BUFFER_ID::SPRITE];

	// Setting vertex and index buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	gl_has_errors();

	GLint in_position_loc = glGetAttribLocation(program, "in_position");
	GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");
	gl_has_errors();
	assert(in_texcoord_loc >= 0);

	glEnableVertexAttribArray(in_position_loc);
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
		sizeof(TexturedVertex), (void*)0);
	gl_has_errors();

	glEnableVertexAttribArray(in_texcoord_loc);
	glVertexAttribPointer(
		in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
		(void*)sizeof(
			vec3)); // note the stride to skip the preceeding vertex position

	// Enabling and binding texture to slot 0
	glActiveTexture(GL_TEXTURE0);
	gl_has_errors();

	GLuint texture_id =
		texture_gl_handles[(GLuint)TEXTURE_ASSET_ID::CHEVRON];

	glBindTexture(GL_TEXTURE_2D, texture_id);
	gl_has_errors();

	// Getting uniform locations for glUniform* calls
	if (!isCharging) {
		vec3 color = { 1, 1, 1 }; 
		GLint color_uloc = glGetUniformLocation(program, "fcolor");
		glUniform3fv(color_uloc, 1, (float*)&color);
		GLint change_color_uloc = glGetUniformLocation(program, "changeColor");
		glUniform1i(change_color_uloc, 0);
		GLint charge_boundary_uloc = glGetUniformLocation(program, "chargeBoundary");
		glUniform1f(charge_boundary_uloc, 1.0);
	}
	else if (isCharging == -1) {
		vec3 color = { 0.5, 0.5, 0.5 }; // grey
		GLint color_uloc = glGetUniformLocation(program, "fcolor");
		glUniform3fv(color_uloc, 1, (float*)&color);
		GLint change_color_uloc = glGetUniformLocation(program, "changeColor");
		glUniform1i(change_color_uloc, 1);
		GLint charge_boundary_uloc = glGetUniformLocation(program, "chargeBoundary");
		glUniform1f(charge_boundary_uloc, 0.0);
	}
	else {
		vec3 color = { 0.5, 0.5, 0.5 }; // grey
		GLint color_uloc = glGetUniformLocation(program, "fcolor");
		glUniform3fv(color_uloc, 1, (float*)&color);
		GLint change_color_uloc = glGetUniformLocation(program, "changeColor");
		glUniform1i(change_color_uloc, 1);

		float chargeBoundary = glm::lerp(0.f, 1.f, (max - cooldown) / max);

		GLint charge_boundary_uloc = glGetUniformLocation(program, "chargeBoundary");
		glUniform1f(charge_boundary_uloc, chargeBoundary);
	}
	
	GLint alpha_uloc = glGetUniformLocation(program, "alpha");
	glUniform1f(alpha_uloc, 1);
	gl_has_errors();

	// Get number of indices from index buffer, which has elements uint16_t
	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	gl_has_errors();

	GLsizei num_indices = size / sizeof(uint16_t);
	// GLsizei num_triangles = num_indices / 3;

	GLint currProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
	// Setting uniform values to the currently bound program
	GLuint transform_loc = glGetUniformLocation(currProgram, "transform");
	glUniformMatrix3fv(transform_loc, 1, GL_FALSE, (float*)&transform.mat);
	GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
	glUniformMatrix3fv(projection_loc, 1, GL_FALSE, (float*)&projection);
	gl_has_errors();
	// Drawing of num_indices/3 triangles specified in the index buffer
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);
	gl_has_errors();
}

