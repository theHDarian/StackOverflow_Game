// internal
#include "render_system.hpp"
#include <SDL.h>
#include <glm/gtx/compatibility.hpp>

#include "ai_system.hpp"
#include "tiny_ecs_registry.hpp"
#include "world_system.hpp"
#include "utils/enum_string_mapping.hpp"

#if IMGUI_ENABLED
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl2.h"
#endif

void RenderSystem::step(float elapsed_ms)
{
	for (auto &entity : registry.spriteTimers.entities)
	{
		auto &spriteTimer = registry.spriteTimers.get(entity);
		spriteTimer.count_ms -= elapsed_ms;
		if (spriteTimer.count_ms <= 0)
		{
			// std::cout << " got hit, switch back to normal " << std::endl;
			registry.renderRequests.get(entity).texture_name = spriteTimer.nextSprite;
			registry.renderRequests.get(entity).used_effect = spriteTimer.nextEffect;
			registry.spriteTimers.remove(entity);
		}
	}

	// quick fix: make stack add notifs persist throughout game pause/dialogue
	// may need to change for future things that need to fade during those times

	GameState &gameState = registry.gameStates.components[0];

	if (registry.showTimers.entities.size() > 0 && !gameState.dialogueScene && !gameState.gamePaused)
	{
		for (int i = (int)registry.showTimers.entities.size() - 1; i >= 0; --i)
		{
			ShowTimer &timer = registry.showTimers.components[i];
			Entity entity = registry.showTimers.entities[i];
			if ((timer.timer -= elapsed_ms) <= 0)
			{
				registry.showTimers.remove(entity);
				// add a little fade
				registry.fades.emplace(entity);
			}
		}
	}

	if (registry.fades.entities.size() > 0)
	{
		for (int i = (int)registry.fades.entities.size() - 1; i >= 0; --i)
		{
			Entity fadeEntity = registry.fades.entities[i];
			auto &fade = registry.fades.get(fadeEntity);
			fade.time -= elapsed_ms;
			if (fade.time <= 0)
			{
				assert(registry.renderRequests.has(fadeEntity));
				registry.renderRequests.get(fadeEntity).show = false;
				registry.fades.remove(fadeEntity); // not sure if should remove here? Although seems to be fine
			}
		}
	}

	for (Entity entity : registry.animations.entities)
	{
		Animation &anim = registry.animations.get(entity);
		if (registry.renderRequests.get(entity).used_effect == EFFECT_ASSET_ID::ANIMATE && anim.animate != AnimationTypes::NONE)
		{
			if (anim.animate != AnimationTypes::ONCE || anim.frame != 0)
				anim.animation_countdown -= elapsed_ms;
			if (anim.animation_countdown <= 0)
			{
				anim.animation_countdown = anim.animation_countdown_base;
				if (anim.animate == AnimationTypes::ONCE)
				{
					anim.frame = (anim.frame + 1 >= anim.max_frames) ? 0 : anim.frame + 1;
				}
				else
				{
					anim.frame = (anim.frame + 1) % anim.max_frames;
				}
			}
		}
		if (registry.animationSequences.has(entity) && anim.frame >= anim.max_frames - 1)
		{
			AnimationSequence &as = registry.animationSequences.get(entity);
			RenderRequest &rr = registry.renderRequests.get(entity);
			rr.texture_name = as.nextSprite;
			rr.used_effect = as.nextEffect;
			registry.animationSequences.remove(entity);
			anim.frame = 0;
			//std::cout << "done" << std::endl;
			//std::cout << anim.frame << std::endl;
		}
	}
}

void RenderSystem::drawCursor()
{
	drawSetupFrame();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(vao);
	mat3 projection_2D = createProjectionMatrix();

	WindowState &windowState = registry.windowStates.components[0];

	mat4 view = mat4(1.0);

	mat4 projection = glm::ortho(0.0f, (float)windowState.width, (float)windowState.height, 0.0f, -3.0f, 3.0f);
	Entity Cursor = registry.cursors.entities[0];
	if (registry.renderRequests.has(Cursor) && registry.motions.has(Cursor))
	{
		drawTexturedMesh(Cursor, projection, view, true);
	}
	glBindVertexArray(0);
}

void RenderSystem::drawTexturedMesh(Entity entity,
									const mat4 &projection, const mat4 &view, bool isUI = false)
{
	assert(registry.renderRequests.has(entity));
	const RenderRequest &render_request = registry.renderRequests.get(entity);
	Motion &motion = registry.motions.get(entity);
	vec2 offset = render_request.offset;
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
	if (render_request.used_effect == EFFECT_ASSET_ID::BULLET || render_request.used_effect == EFFECT_ASSET_ID::TEXTURED || render_request.used_effect == EFFECT_ASSET_ID::ANIMATE)
	{
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");
		gl_has_errors();
		assert(in_texcoord_loc >= 0);

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_texcoord_loc);
		glVertexAttribPointer(in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *)sizeof(vec3));
		gl_has_errors();

		if (render_request.used_effect == EFFECT_ASSET_ID::TEXTURED)
		{
			GLint tile_uloc = glGetUniformLocation(program, "tile");
			glUniform1i(tile_uloc, render_request.idealScale.x > 0);
			gl_has_errors();

			vec2 tiling = registry.maps.components[0].currRoom.preset.roomSize / render_request.idealScale;
			GLint tiling_uloc = glGetUniformLocation(program, "tiling");
			glUniform2fv(tiling_uloc, 1, (float *)&tiling);
		}

		if (render_request.used_effect == EFFECT_ASSET_ID::ANIMATE)
		{
			GLint frame_uloc = glGetUniformLocation(program, "frame");
			glUniform1i(frame_uloc, registry.animations.get(entity).frame);
			GLuint time_uloc = glGetUniformLocation(program, "time");
			glUniform1f(time_uloc, (float)(glfwGetTime() * 10.0f));
			gl_has_errors();
		}

		if (render_request.used_effect == EFFECT_ASSET_ID::BULLET)
		{
			GLuint time_uloc = glGetUniformLocation(program, "time");
			glUniform1f(time_uloc, (float)(glfwGetTime() * 10.0f));

			GLint laser_uloc = glGetUniformLocation(program, "laser");
			glUniform1i(laser_uloc, registry.lasers.has(entity));

			GLint onDeath_uloc = glGetUniformLocation(program, "onDeath");
			glUniform1i(onDeath_uloc, registry.enemyBullets.get(entity).onDeath != EnemyBulletDeath::NONE);

			int size = registry.enemyBullets.get(entity).bulletEffects.size();
			std::vector<BulletStackEffect> bse = registry.enemyBullets.get(entity).bulletEffects;
			GLint effect_size_uloc = glGetUniformLocation(program, "effectSize");
			glUniform1i(effect_size_uloc, size);

			GLint shape_uloc = glGetUniformLocation(program, "shape");
			glUniform1i(shape_uloc, registry.enemyBullets.get(entity).shape);

			GLint scale_uloc = glGetUniformLocation(program, "scale");
			glUniform2fv(scale_uloc, 1, (float *)&registry.motions.get(entity).scale);

			vec3 c1, c2, c3, c4, c5;
			c1 = (size > 0) ? bulletEffectColors.at(bse[0].type) : vec3(-1.0);
			c2 = (size > 1) ? bulletEffectColors.at(bse[1].type) : vec3(-1.0);
			c3 = (size > 2) ? bulletEffectColors.at(bse[2].type) : vec3(-1.0);
			c4 = (size > 3) ? bulletEffectColors.at(bse[3].type) : vec3(-1.0);
			c5 = (size > 4) ? bulletEffectColors.at(bse[4].type) : vec3(-1.0);

			GLint bcolor1_uloc = glGetUniformLocation(program, "bcolor1");
			glUniform3fv(bcolor1_uloc, 1, (float *)&c1);
			GLint bcolor2_uloc = glGetUniformLocation(program, "bcolor2");
			glUniform3fv(bcolor2_uloc, 1, (float *)&c2);
			GLint bcolor3_uloc = glGetUniformLocation(program, "bcolor3");
			glUniform3fv(bcolor3_uloc, 1, (float *)&c3);
			GLint bcolor4_uloc = glGetUniformLocation(program, "bcolor4");
			glUniform3fv(bcolor4_uloc, 1, (float *)&c4);
			GLint bcolor5_uloc = glGetUniformLocation(program, "bcolor5");
			glUniform3fv(bcolor5_uloc, 1, (float *)&c5);

			gl_has_errors();
		}

		// Enable and bind the texture to slot 0
		glActiveTexture(GL_TEXTURE0);
		gl_has_errors();
		assert(registry.renderRequests.has(entity));
		GLuint texture_id = texture_gl_handles[(GLuint)name_to_texture[registry.renderRequests.get(entity).texture_name]];

		if (render_request.used_effect == EFFECT_ASSET_ID::ANIMATE)
		{
			GLuint texture_uloc = glGetUniformLocation(program, "sampler0");
			glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);
			glUniform1i(texture_uloc, 0);
			gl_has_errors();

			GLuint glitch_mask_id = texture_gl_handles[(GLuint)name_to_texture["glitch_mask"]];
			GLuint glitch_mask_uloc = glGetUniformLocation(program, "glitchMask");
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D_ARRAY, glitch_mask_id);
			glUniform1i(glitch_mask_uloc, 1);

			GLuint glitch_id = texture_gl_handles[(GLuint)name_to_texture["glitch"]];
			GLuint glitch_uloc = glGetUniformLocation(program, "glitch");
			glActiveTexture(GL_TEXTURE0 + 2);
			glBindTexture(GL_TEXTURE_2D_ARRAY, glitch_id);
			glUniform1i(glitch_uloc, 2);
			gl_has_errors();
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, texture_id);
		}
		gl_has_errors();
	}
	else if (render_request.used_effect == EFFECT_ASSET_ID::MESH || render_request.used_effect == EFFECT_ASSET_ID::EGG)
	{
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		gl_has_errors();

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
							  sizeof(ColoredVertex), (void *)0);
		gl_has_errors();

		GLuint time_uloc = glGetUniformLocation(program, "time");
		glUniform1f(time_uloc, (float)(glfwGetTime() * 10.0f));

		GLuint angle_uloc = glGetUniformLocation(program, "angle");
		glUniform1f(angle_uloc, (float)(registry.motions.get(entity).angle));

		if (render_request.used_effect == EFFECT_ASSET_ID::EGG)
		{
			GLint in_color_loc = glGetAttribLocation(program, "in_color");
			gl_has_errors();

			glEnableVertexAttribArray(in_color_loc);
			glVertexAttribPointer(in_color_loc, 3, GL_FLOAT, GL_FALSE,
								  sizeof(ColoredVertex), (void *)sizeof(vec3));

			gl_has_errors();
		}
	}
	else if (render_request.used_effect == EFFECT_ASSET_ID::ROOM_BOUND)
	{
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");
		gl_has_errors();
		assert(in_texcoord_loc >= 0);

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_texcoord_loc);
		glVertexAttribPointer(in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *)sizeof(vec3));
		gl_has_errors();

		// Enable and bind the texture to slot 0
		glActiveTexture(GL_TEXTURE0);
		gl_has_errors();
		assert(registry.renderRequests.has(entity));
		GLuint texture_id = texture_gl_handles[(GLuint)name_to_texture[registry.renderRequests.get(entity).texture_name]];

		WindowState &ws = registry.windowStates.components[0];
		float angle;
		vec3 axis;
		vec3 offset;
		int frame = 0;
		vec2 tiling = vec2(1.0);
		if (registry.bounds.has(entity))
		{
			Bound &b = registry.bounds.get(entity);
			angle = b.angle;
			axis = b.axis;
			offset = b.offset;
			tiling = vec2(registry.maps.components[0].currRoom.preset.roomSize.x / render_request.idealScale.x, 1);

			// For changing wall textures per region
			Map &map = registry.maps.components[0];
			frame = (map.currRegion <= MapRegion::Biology) ? 0 : 1;
		}
		else if (registry.doorSymbols.has(entity))
		{
			DoorSymbol &d = registry.doorSymbols.get(entity);
			angle = d.angle;
			axis = d.axis;
			offset = d.offset;
			if (d.door)
			{
				frame = 2;
				for (Entity &d1 : registry.doors.entities)
				{
					if (registry.doors.get(d1).side == d.side)
					{
						if (registry.interactables.has(d1))
						{
							if (registry.interactables.get(d1).name == "ClosedDoor")
							{
								frame = 0;
							}
							else if (registry.interactables.get(d1).name == "ClosedTutorialDoor")
							{
								frame = 0;
							}
							else if (registry.interactables.get(d1).name == "EmptyDoor")
							{
								frame = 0;
							}
							else if (registry.interactables.get(d1).name == "PrevDoor")
							{
								frame = 0;
							}
							else if (registry.interactables.get(d1).name == "LockedDoor")
							{
								frame = 1;
							}
							else if (registry.interactables.get(d1).name == "OpenDoor")
							{
								frame = 2;
							}
						}
						break;
					}
				}
			}
		}
		else
		{
			assert(false);
		}

		GLint frame_uloc = glGetUniformLocation(program, "frame");
		glUniform1i(frame_uloc, frame);
		gl_has_errors();

		glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);
		gl_has_errors();

		GLint tile_uloc = glGetUniformLocation(program, "tile");
		glUniform1i(tile_uloc, render_request.idealScale.x > 0);
		gl_has_errors();

		GLint tiling_uloc = glGetUniformLocation(program, "tiling");
		glUniform2fv(tiling_uloc, 1, (float *)&tiling);
		gl_has_errors();

		//Motion &targetMotion = registry.motions.get(registry.players.entities[0]);
		Camera &camera = registry.cameras.components[0];
		//Motion& targetMotion = registry.motions.get(camera.target);
		Room &room = registry.maps.components[0].currRoom;

		vec2 roomCenterOffset = vec2(0);
		if ((room.preset.roomSize.x / 2) < (ws.width / 2) || (room.preset.roomSize.y / 2) < (ws.height / 2))
			roomCenterOffset += -1.f * camera.zoom * vec2((ws.width / 2 - room.preset.roomSize.x / 2) * (1 - ((room.preset.roomSize.x * 0.33f) / 2.f / (ws.width / 2.f))), 0);
		if ((room.preset.roomSize.y / 2) < (ws.height / 2))
			roomCenterOffset += -1.f * camera.zoom * vec2(0, (ws.height / 2 - room.preset.roomSize.y / 2) * 1.8f * (1 - ((room.preset.roomSize.y * 0.33f) / 2.f / (ws.height / 2))));

		vec3 clampedTranslate = vec3(clamp(motion.position.x - camera.lookAtPos.x * 1.1f + ws.width * 0.1f / 2.f,
										   motion.position.x - room.preset.roomSize.x / clampAmount * 1.1f / 2 + ws.width / clampAmount * 0.1f / 2 - room.wallThickness * 1.5f * 1.1f / 2 - 25,
										   motion.position.x + room.preset.roomSize.x * 1.1f / 2 - ws.width * clampAmount * 1.1f / 2 - ws.width * clampAmount / 2 + room.wallThickness * 1.5f * 1.1f / 2 + 25),
									 clamp(motion.position.y + camera.lookAtPos.y * 2.f - ws.height * 2.f / 2.f - ws.height / 2.f,
										   motion.position.y + ws.height / clampAmount / 2.f - room.preset.roomSize.y / clampAmount - room.wallThickness * 1.5f,
										   motion.position.y - ws.height * clampAmount * 2.f + ws.height * clampAmount / 2.f + room.preset.roomSize.y + room.wallThickness * 1.5f),
									 -room.wallThickness * 1.5f + 50);
		vec3 unclampedTranslate = vec3(motion.position.x - camera.lookAtPos.x * 1.1f + ws.width * 0.1f / 2.f,
									   motion.position.y + camera.lookAtPos.y * 2.f - ws.height * 2.f / 2.f - ws.height / 2.f,
									   -room.wallThickness * 1.5f + 50);
		mat4 model =
			glm::translate(glm::mat4(1.0f), vec3(ws.width / 2, ws.height / 2, 0)) * glm::scale(glm::mat4(1.0f), vec3(camera.zoom, camera.zoom, 1.0f)) * glm::translate(glm::mat4(1.0f), clampedTranslate)
			// minor offset to close "gap" btween floor & wall
			* glm::rotate(glm::mat4(1.0f), motion.angle, vec3(0, 0, 1)) * glm::translate(glm::mat4(1.0f), vec3(0))
			/** glm::rotate(glm::mat4(1.0f),angle + radians(30.f) * (distance(targetMotion.position / 2.f, motion.position)), axis) //rotate to be vertical on z axis*/ // this generates wind turbine walls lmao
			* glm::rotate(glm::mat4(1.0f), angle, axis) * glm::scale(glm::mat4(1.0f), vec3(motion.scale, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, (float *)&model);

		glm::vec3 cameraPos = glm::vec3(ws.width / 2, ws.height / 2, ws.height / 6.575 + ws.width / 6.575); // Position above the XY plane
		cameraPos += vec3(roomCenterOffset, 0);
		glm::vec3 cameraTarget = glm::vec3(ws.width / 2, ws.height / 2, 0.0f);
		cameraTarget += vec3(roomCenterOffset, 0);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::mat4 view_roomBounds = glm::lookAt(cameraPos, cameraTarget, up);

		glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, (float *)&view_roomBounds);
		vec3 clampOffset = unclampedTranslate - clampedTranslate;
		glm::mat4 translateAfterClamp = glm::translate(glm::mat4(1.0f),
													   (clampOffset * vec3(1.8f, 1, 1)) * camera.zoom * vec3(room.preset.roomSize.x / ws.width, room.preset.roomSize.y / ws.height, 1) / vec3(room.preset.roomSize.x, room.preset.roomSize.y, 1));
		translateAfterClamp = glm::translate(translateAfterClamp, (vec3(roomCenterOffset, 0) * vec3(1.8f, 1, 1)) * vec3(room.preset.roomSize.x / ws.width, room.preset.roomSize.y / ws.height, 1) / vec3(room.preset.roomSize.x, room.preset.roomSize.y, 1));

		glUniformMatrix4fv(glGetUniformLocation(program, "translateAfterClamp"), 1, GL_FALSE, (float *)&translateAfterClamp);

		float fov = 125.0f; // makes walls appear larger the less there is
		float aspectRatio = (ws.width) / (ws.height);
		float near_var = 0.1f;
		float far_var = 10000.0f;
		mat4 proj4 = glm::perspective(glm::radians(fov), aspectRatio, near_var, far_var);
		glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, (float *)&proj4);
	}
	else
	{
		assert(false && "Type of render request not supported");
	}

	GLint currProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
	// Get number of indices from index buffer, which has elements uint16_t
	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	gl_has_errors();

	GLsizei num_indices = size / sizeof(uint16_t);

	// Getting uniform locations for glUniform* calls
	GLint color_uloc = glGetUniformLocation(program, "fcolor");

	float alpha = 1;

	GLint change_color_uloc = glGetUniformLocation(program, "changeColor");
	glUniform1i(change_color_uloc, 0);
	GLint effectAlpha = glGetUniformLocation(program, "effectAlpha");
	glUniform1f(effectAlpha, 1);

	// fade out entity if needed
	if (registry.fades.has(entity))
	{
		Fade &fade = registry.fades.get(entity);
		alpha = glm::lerp(1.f, 0.f, (fade.max - fade.time) / fade.max);
		if (registry.enemies.has(entity))
		{
			vec3 color = {1.2, 0.5, 0.5}; // red
			glUniform3fv(color_uloc, 1, (float *)&color);
			glUniform1f(effectAlpha, alpha);
			glUniform1i(change_color_uloc, 1);
		}
	}
	else if (registry.interactIndicators.has(entity))
	{ // hard code here for now
		alpha = 0.7;
	}
	else if (registry.shield.has(entity)) {
		alpha = 0.3;
	}

	GLint alpha_uloc = glGetUniformLocation(program, "alpha");
	glUniform1f(alpha_uloc, alpha);

	// Setting uniform values to the currently bound program
	if (render_request.used_effect != EFFECT_ASSET_ID::ROOM_BOUND)
	{
		WindowState &windowState = registry.windowStates.components[0];
		Motion &playerMotion = registry.motions.get(registry.players.entities[0]);
		float wallThickness = 100 + 50;
		float zoom = 1;
		// note: perspective seems to make no difference?
		GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
		glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (float *)&projection);

		// draw the border first
		if (registry.uiBorders.has(entity) && registry.uiBorders.get(entity).border == UIBorderType::Outlined)
		{
			UIBorder &uiBorder = registry.uiBorders.get(entity);
			mat4 border_transform = glm::mat4(1.0);
			Motion borderMotion = motion;
			borderMotion.scale += vec2(uiBorder.borderThickness);

			GLuint border_transform_loc = glGetUniformLocation(currProgram, "model");
			if (!isUI)
			{
				border_transform = createFollowCameraModel(borderMotion, offset);
			}
			else
			{
				border_transform = createNormalModel(borderMotion, offset);
			}
			glUniformMatrix4fv(border_transform_loc, 1, GL_FALSE, (float *)&border_transform);
			GLint border_color_uloc = glGetUniformLocation(program, "fcolor");
			glUniform3fv(border_color_uloc, 1, (float *)&uiBorder.borderColour);
			glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);
		}

		mat4 transform = glm::mat4(1.0);
		if (!isUI)
		{
			transform = createFollowCameraModel(motion, offset);
		}
		else
		{
			transform = createNormalModel(motion, offset);
		}
		GLuint transform_loc = glGetUniformLocation(currProgram, "model");
		glUniformMatrix4fv(transform_loc, 1, GL_FALSE, (float *)&transform);
		glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, (float *)&view);

		gl_has_errors();
	}

	const vec3 color = registry.colors.has(entity) ? registry.colors.get(entity) : vec3(1);
	glUniform3fv(color_uloc, 1, (float *)&color);

	gl_has_errors();

	// change opacity of entity if needed
	if (registry.invincibles.has(entity))
	{
		Invincible &invincible = registry.invincibles.get(entity);
		alpha = 1 - abs(sin(invincible.countdown / invincible.max * 10) * 0.3);
		glUniform1f(alpha_uloc, alpha);
	}

	if (registry.damageds.has(entity))
	{
		Damaged &damaged = registry.damageds.get(entity);
		vec3 color = {1.2, 0.5, 0.5}; // red
		glUniform3fv(color_uloc, 1, (float *)&color);
		glUniform1i(change_color_uloc, 1);
		alpha = glm::lerp(0.5f, 0.f, (damaged.max - damaged.countdown) / damaged.max);
		glUniform1f(effectAlpha, alpha);
	}

	if (registry.spawnings.has(entity))
	{
		Spawning &spawning = registry.spawnings.get(entity);
		//change color to grey
		vec3 Color = COLOR_GREY_LIGHT;
		glUniform3fv(color_uloc, 1, (float *)&Color);
		glUniform1i(change_color_uloc, 0);
		alpha = glm::lerp(0.f, 2.f, (spawning.max - spawning.countdown) / spawning.max);
		// GLuint effects_enum = static_cast<GLuint>(EFFECT_ASSET_ID::DASH);
		// GLuint Program = (GLuint)effects[effects_enum];
		// glUseProgram(Program);
		// gl_has_errors();
		// glBindBuffer(GL_ARRAY_BUFFER, vbo);
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		// gl_has_errors();
		// GLint in_position_loc = glGetAttribLocation(program, "in_position");
		// GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");
		// gl_has_errors();
		// assert(in_texcoord_loc >= 0);
		//
		// glEnableVertexAttribArray(in_position_loc);
		// glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
		// 					  sizeof(TexturedVertex), (void *)0);
		// gl_has_errors();
		//
		// glEnableVertexAttribArray(in_texcoord_loc);
		// glVertexAttribPointer(
		// 	in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
		// 	(void *)sizeof(
		// 		vec3)); // note the stride to skip the preceeding vertex position
		//
		// // Enabling and binding texture to slot 0
		// glActiveTexture(GL_TEXTURE0);
		// gl_has_errors();
		//
		// GLuint texture_id =
		// 	texture_gl_handles[(GLuint)name_to_texture["chevron.png"]];
		//
		// glBindTexture(GL_TEXTURE_2D, texture_id);
		// gl_has_errors();
		// GLuint Change_color_uloc = glGetUniformLocation(Program, "changeColor");
		// glUniform1i(Change_color_uloc, 1);
		//
		// GLint Color_uloc = glGetUniformLocation(Program, "fcolor");
		// glUniform3fv(Color_uloc, 1, (float *)&Color);
		// GLuint chargeBoundary_uloc = glGetUniformLocation(Program, "chargeBoundary");
		// glUniform1f(chargeBoundary_uloc, glm::lerp(1.f, 0.f, (spawning.max - spawning.countdown) / spawning.max));

	}
	// GLsizei num_triangles = num_indices / 3;

	// Drawing of num_indices/3 triangles specified in the index buffer
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);

	gl_has_errors();
}

void RenderSystem::drawMesh(Entity entity,
							const mat4 &projection, const mat4 &view)
{

	assert(registry.renderRequests.has(entity));
	const RenderRequest &render_request = registry.renderRequests.get(entity);
	Motion &motion = registry.motions.get(entity);
	// Transformation code, see Rendering and Transformation in the template
	// specification for more info Incrementally updates transformation matrix,
	// thus ORDER IS IMPORTANT

	vec2 offset = registry.renderRequests.get(entity).offset;

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

	GLint in_position_loc = glGetAttribLocation(program, "in_position");
	gl_has_errors();

	glEnableVertexAttribArray(in_position_loc);
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
						  sizeof(ColoredVertex), (void *)0);
	gl_has_errors();

	GLuint time_uloc = glGetUniformLocation(program, "time");
	glUniform1f(time_uloc, (float)(glfwGetTime() * 10.0f));

	GLuint angle_uloc = glGetUniformLocation(program, "angle");
	glUniform1f(angle_uloc, (float)(registry.motions.get(entity).angle));

	glActiveTexture(GL_TEXTURE0);
	gl_has_errors();

	GLuint texture_id = texture_gl_handles[(GLuint)name_to_texture["hexagon.png"]];
	glBindTexture(GL_TEXTURE_2D, texture_id);

	// Get number of indices from index buffer, which has elements uint16_t
	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	gl_has_errors();

	GLsizei num_indices = size / sizeof(uint16_t);
	// GLsizei num_triangles = num_indices / 3;

	GLint currProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
	// Setting uniform values to the currently bound program
	GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (float *)&projection);

	mat4 transform = createFollowCameraModel(motion, offset);

	GLuint transform_loc = glGetUniformLocation(currProgram, "model");
	glUniformMatrix4fv(transform_loc, 1, GL_FALSE, (float *)&transform);

	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, (float *)&view);
	gl_has_errors();

	// Drawing of num_indices/3 triangles specified in the index buffer
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);
	gl_has_errors();
}

// draw the intermediate texture to the screen, with some distortion to simulate
// water
void RenderSystem::drawToScreenExtra(EFFECT_ASSET_ID effect)
{
	Frame &frame = registry.frames.components[0];
	// Setting shaders
	// get the water texture, sprite mesh, and program
	glUseProgram(effects[(GLuint)effect]);
	glBindVertexArray(vao);
	gl_has_errors();
	// Clearing backbuffer
	int w, h;
	glfwGetFramebufferSize(window, &w, &h); // Note, this will be 2x the resolution given to glfwCreateWindow on retina displays
	glBindFramebuffer(GL_READ_FRAMEBUFFER, frame.prevFrameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame.prevFrameBuffer += 1);
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
	const GLuint postprocess_program = effects[(GLuint)effect];
	// Set clock
	GLuint time_uloc = glGetUniformLocation(postprocess_program, "time");
	glUniform1f(time_uloc, (float)(glfwGetTime() * 10.0f));
	StackCompile &stack = registry.stackCompile.get(registry.players.entities[0]);
	float intensity = (float)stack.currStack.size() / ((stack.baseStackSize + stack.additives[PlayerStackSize]) * stack.multiplicatives[PlayerStackSize]);
	GLuint chrom_abb_intensity_uloc = glGetUniformLocation(postprocess_program, "chromatic_abberation_intensity");
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
	glBindTexture(GL_TEXTURE_2D, frame.prevTexture);
	gl_has_errors();
	// Draw
	glDrawElements(
		GL_TRIANGLES, 3, GL_UNSIGNED_SHORT,
		nullptr); // one triangle = 3 vertices; nullptr indicates that there is
				  // no offset from the bound index buffer
	glBindVertexArray(0);
	gl_has_errors();

	drawSetupFrame();
}

void RenderSystem::drawToScreenFinal()
{

	// Setting shaders
	// get the water texture, sprite mesh, and program
	glUseProgram(effects[(GLuint)EFFECT_ASSET_ID::POSTPROCESS2]);
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
	const GLuint postprocess_program = effects[(GLuint)EFFECT_ASSET_ID::POSTPROCESS2];
	// Set clock
	GLuint time_uloc = glGetUniformLocation(postprocess_program, "time");
	glUniform1f(time_uloc, (float)(glfwGetTime() * 10.0f));
	gl_has_errors();
	// Set the vertex position and vertex texture coordinates (both stored in the
	// same VBO)
	GLint in_position_loc = glGetAttribLocation(postprocess_program, "in_position");
	glEnableVertexAttribArray(in_position_loc);
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void *)0);
	gl_has_errors();

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	Frame &frame = registry.frames.components[0];
	assert(frame.prevTexture != 0);
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

void RenderSystem::drawSetupFrame()
{
	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	Frame &frame = registry.frames.components[0];
	if (frame.prevFrameBuffer == frame_buffer)
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	glViewport(0, 0, w, h);
	glDepthRange(0.00001, 10);
	glClearColor(GLfloat(32 / 255), GLfloat(43 / 255), GLfloat(81 / 255), 1.0);
	glClearDepth(10.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	if (frame.prevFrameBuffer != 0)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, frame.prevFrameBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame_buffer);

		// copy contents of previous buffer to current
		glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		gl_has_errors();
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
	}
	frame.prevFrameBuffer = frame_buffer;
	frame.prevTexture = off_screen_render_buffer_color;
}

void RenderSystem::drawBackgroundElements()
{
	drawSetupFrame();
	/*mat3 projection_2D = createProjectionMatrix();*/
	WindowState &windowState = registry.windowStates.components[0];
	Camera &camera = registry.cameras.get(registry.players.entities[0]);

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// note: this camera doesn't really do anything rn
	mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	mat4 projection = glm::ortho(0.0f, (float)windowState.width, (float)windowState.height, 0.0f, -3.0f, 3.0f);
	IOState &ioState = registry.ioStates.components[0];
	glBindVertexArray(vao);
	for (Entity entity : registry.backgrounds.entities)
	{
		if (!registry.renderRequests.get(entity).show)
			continue;
		drawTexturedMesh(entity, projection, view);
	}

	for (Entity &entity : registry.objects.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || registry.invisibles.has(entity))
			continue;
		if (registry.motions.get(registry.players.entities[0]).position.y + registry.motions.get(registry.players.entities[0]).scale.y / 2.f >= registry.motions.get(entity).position.y + registry.objects.get(entity).baseOffset)
		{
			drawTexturedMesh(entity, projection, view);
			if (ioState.debugMode)
				drawAllColliders(entity, projection, view);
		}
	}

	glBindVertexArray(0);
}

// Render our game world
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
void RenderSystem::drawGameElements()
{
	drawSetupFrame();

	mat3 projection_2D = createProjectionMatrix();

	WindowState &windowState = registry.windowStates.components[0];
	Camera &camera = registry.cameras.get(registry.players.entities[0]);

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// note: this camera doesn't really do anything rn
	mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	mat4 projection = glm::ortho(0.0f, (float)windowState.width, (float)windowState.height, 0.0f, -3.0f, 3.0f);

	glBindVertexArray(vao);

	IOState &ioState = registry.ioStates.components[0];

	// this setup requires us know what types of things to render
	// and won't render all render requests if not given the proper component
	// Note, its not very efficient to access elements indirectly via the entity
	// albeit iterating through all Sprites in sequence. A good point to optimize

	for (Entity &entity : registry.enemyBullets.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || registry.invisibles.has(entity))
			continue;
		if (registry.lasers.has(entity) && registry.enemies.has(registry.lasers.get(entity).start))
			drawLaserIndicator(entity, projection, view);
		drawTexturedMesh(entity, projection, view);
		if (ioState.debugMode)
			drawAllColliders(entity, projection, view);
	}

	for (Entity &entity : registry.playerBullets.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || registry.invisibles.has(entity))
			continue;
		drawTexturedMesh(entity, projection, view);
		if (ioState.debugMode)
			drawAllColliders(entity, projection, view);
	}

	for (Entity &entity : registry.bosses.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || registry.invisibles.has(entity))
			continue;
		(!registry.meshColliders.has(entity)) ? drawTexturedMesh(entity, projection, view) : drawMesh(entity, projection, view);

		if (ioState.debugMode)
			drawAllColliders(entity, projection, view);
	}

	for (Entity &entity : registry.enemies.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || registry.invisibles.has(entity) || registry.bosses.has(entity))
			continue;
		(!registry.meshColliders.has(entity)) ? drawTexturedMesh(entity, projection, view) : drawMesh(entity, projection, view);

		if (ioState.debugMode)
			drawAllColliders(entity, projection, view);
	}

	for (Entity &entity : registry.players.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || registry.invisibles.has(entity))
			continue;
		drawTexturedMesh(entity, projection, view);
		if (ioState.debugMode)
			drawAllColliders(entity, projection, view);
	}

	for (Entity &entity : registry.objects.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || registry.invisibles.has(entity))
			continue;
		if (registry.motions.get(registry.players.entities[0]).position.y + registry.motions.get(registry.players.entities[0]).scale.y / 2.f < registry.motions.get(entity).position.y + registry.objects.get(entity).baseOffset)
		{
			drawTexturedMesh(entity, projection, view);
			if (ioState.debugMode)
				drawAllColliders(entity, projection, view);
		}
	}

	for (Entity &entity : registry.walls.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || registry.invisibles.has(entity) || registry.backgrounds.has(entity))
			continue;
		if (registry.renderRequests.get(entity).used_effect == EFFECT_ASSET_ID::ROOM_BOUND || ioState.debugMode)
			drawTexturedMesh(entity, projection, view);
	}

	for (Entity &entity : registry.doors.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || registry.invisibles.has(entity))
			continue;
		if (registry.renderRequests.get(entity).used_effect == EFFECT_ASSET_ID::ROOM_BOUND || ioState.debugMode)
			drawTexturedMesh(entity, projection, view);
	}

	for (Entity &entity : registry.critters.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || registry.invisibles.has(entity))
			continue;
		drawTexturedMesh(entity, projection, view);
	}

	glBindVertexArray(0);
	gl_has_errors();
}

void RenderSystem::drawGameUI()
{
	drawSetupFrame();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(vao);
	// mat3 projection_2D = createProjectionMatrix();

	mat3 projection_2D = createProjectionMatrix();

	WindowState &windowState = registry.windowStates.components[0];
	Camera &camera = registry.cameras.get(registry.players.entities[0]);

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// note: this camera doesn't really do anything rn
	mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	mat4 projection = glm::ortho(0.0f, (float)windowState.width, (float)windowState.height, 0.0f, -3.0f, 3.0f);

	for (Entity &entity : registry.enemies.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || registry.invisibles.has(entity))
			continue;
		if (!registry.boids.has(entity) && !registry.bossParts.has(entity) && !registry.invisibleEnemy.has(entity)) {
			drawHPbar(entity, projection, view);
		}
	}

	for (Entity &entity : registry.bosses.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || registry.invisibles.has(entity))
			continue;
		if (!registry.boids.has(entity) && !registry.bossParts.has(entity))
		{
			drawHPbar(entity, projection, view);
			BossEnemy &boss = registry.bosses.get(entity);
			if (!registry.textRenderRequests.has(entity))
			{
				GameUIText &text = registry.gameUITexts.emplace(entity);
				TextRenderRequest &textRequest = registry.textRenderRequests.emplace(entity);
				textRequest.text = boss.name;
				textRequest.x = windowState.width / 2,
				textRequest.y = windowState.height * 0.03f;
				textRequest.alignment = TextAlignment::CenteredAlign;
				textRequest.scale = 0.4f;
				textRequest.color = vec3(1, 1, 1);
				textRequest.bottomLeftBound = vec2(0);
				textRequest.topRightBound = vec2(windowState.width, windowState.height);
			}
		}
	}

	for (Entity &entity : registry.gameUIs.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || !registry.renderRequests.get(entity).show)
			continue;

		// figure out how to modularize drawing this later; maybe each bullet is an entity
		if (registry.stackUI.has(entity))
		{
			drawBulletStack(projection, view);
		}
		else
		{
			drawTexturedMesh(entity, projection, view, false);
		}
	}

	drawDashes(projection, view);

	glBindVertexArray(0);
	gl_has_errors();
}

void RenderSystem::drawDialogueUI()
{
	drawSetupFrame();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(vao);
	mat3 projection_2D = createProjectionMatrix();

	WindowState &windowState = registry.windowStates.components[0];
	Camera &camera = registry.cameras.get(registry.players.entities[0]);

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// note: this camera doesn't really do anything rn
	mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	mat4 projection = glm::ortho(0.0f, (float)windowState.width, (float)windowState.height, 0.0f, -3.0f, 3.0f);

	for (Entity entity : registry.screenCutIns.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || !registry.renderRequests.get(entity).show)
			continue;
		drawTexturedMesh(entity, projection, view, true);
	}

	for (Entity &entity : registry.dialogueUIs.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || !registry.renderRequests.get(entity).show)
			continue;
		drawTexturedMesh(entity, projection, view, true);
	}

	glBindVertexArray(0);
	gl_has_errors();
}

void RenderSystem::drawMenuUI()
{
	drawSetupFrame();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(vao);
	mat3 projection_2D = createProjectionMatrix();

	WindowState &windowState = registry.windowStates.components[0];
	Camera &camera = registry.cameras.get(registry.players.entities[0]);

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// note: this camera doesn't really do anything rn
	mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	mat4 projection = glm::ortho(0.0f, (float)windowState.width, (float)windowState.height, 0.0f, -3.0f, 3.0f);

	for (Entity &entity : registry.menuUIs.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || !registry.renderRequests.get(entity).show)
			continue;
		drawTexturedMesh(entity, projection, view, true);
	}

	glBindVertexArray(0);
	gl_has_errors();

#if IMGUI_ENABLED
	// draw Imgui
	drawImGui();
#endif
}

void RenderSystem::drawMenuOverlayUI()
{
	drawSetupFrame();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(vao);
	mat3 projection_2D = createProjectionMatrix();

	WindowState &windowState = registry.windowStates.components[0];
	Camera &camera = registry.cameras.get(registry.players.entities[0]);

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// note: this camera doesn't really do anything rn
	mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	mat4 projection = glm::ortho(0.0f, (float)windowState.width, (float)windowState.height, 0.0f, -3.0f, 3.0f);

	for (Entity &entity : registry.menuOverlayUIs.entities)
	{
		if (!registry.renderRequests.has(entity) || !registry.motions.has(entity) || !registry.renderRequests.get(entity).show)
			continue;
		drawTexturedMesh(entity, projection, view, true);
	}

	glBindVertexArray(0);
	gl_has_errors();
}

mat3 RenderSystem::createProjectionMatrix()
{
	// Fake projection matrix, scales with respect to window coordinates
	float left = 0.f;
	float top = 0.f;

	gl_has_errors();
	WindowState &windowState = registry.windowStates.components[0];
	Camera &camera = registry.cameras.components[0];
	float right = (float)(windowState.width);
	float bottom = (float)(windowState.height);

	float sx = 2.f / (right - left);
	float sy = 2.f / (top - bottom);
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);

	mat3 projection = {{sx, 0.f, 0.f}, {0.f, sy, 0.f}, {tx, ty, 1.f}};
	// mat4 projection = glm::ortho(0.0f, (float)windowState.width, (float)windowState.height, 0.0f, 0.0f, 1.0f);
	mat4 trans = glm::mat3(1.0);
	// trans = glm::translate(trans, glm::vec3(0.0, -0.5, 0.0));

	return projection;
}

#if IMGUI_ENABLED
void RenderSystem::drawImGui()
{
	int menuWidth = 200;
	IOState &ioState = registry.ioStates.components[0];
	WindowState &windowState = registry.windowStates.components[0];
	Map &map = registry.maps.components[0];
	// std::cout << windowState.width << " " << windowState.height << std::endl;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	const ImVec2 &size = ImVec2(menuWidth, windowState.height);
	ImGui::SetNextWindowSize(size);
	ImGui::SetNextWindowPos(ImVec2(windowState.width - menuWidth, 0));
	ImGui::Begin("Debug window");

	// BASIC INFORMATION
	ImGui::Text("Enemy Bullet Count: %lu", registry.enemyBullets.size());
	ImGui::Text("Viewport Size: (%d, %d)", windowState.width, windowState.height);
	ImGui::Text("Mouse Pos: (%.2f, %.2f)", ioState.mousePosition.x, ioState.mousePosition.y);

	// STACK INFORMATION
	StackCompile &sc = registry.stackCompile.components[0];
	// ImGui::Text("Stack Size: %lu", sc.currStack.size());
	ImGui::Text("S. Bullets Left: %d", map.currRoom.preset.numSpecialBulletsToSpawn);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Additives");
	ImGui::BeginChild("AdditiveContent", ImVec2(180, 80), true);
	std::map<BulletEffectType, float>::iterator it;
	for (it = sc.additives.begin(); it != sc.additives.end(); it++)
	{
		if (it->second == 0)
			continue;
		ImGui::Text("%s: %.1f", bulletEffectTypeNames[it->first].c_str(), it->second);
	}
	ImGui::EndChild();

	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Multiplicatives");
	ImGui::BeginChild("MultiplicativeContent", ImVec2(180, 80), true);
	for (it = sc.multiplicatives.begin(); it != sc.multiplicatives.end(); it++)
	{
		if (it->second == 1)
			continue;
		ImGui::Text("%s: %.1f", bulletEffectTypeNames[it->first].c_str(), it->second);
	}
	ImGui::EndChild();

	for (int i = 0; i < registry.doors.components.size(); i++)
	{
		Door &d = registry.doors.components[i];
		int type = d.isPrev ? -1 : d.room;
		ImGui::Text("Door %d: type %d", i, type);
	}

	if (ImGui::Button("Restart Game"))
	{
		registry.ioStates.components[0].shouldRestart = true;
	}
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGui::UpdatePlatformWindows();
}
#endif

// bandaid fix to draw all colliders an entity has right now
void RenderSystem::drawAllColliders(Entity entity, const mat4 &projection, const mat4 &view)
{
	if (registry.circleColliders.has(entity))
		drawCollider(entity, "circle.png", projection, view);
	if (registry.aabbs.has(entity))
		drawCollider(entity, "rectangle.png", projection, view);
}

// should really consider making a draw textured mesh function without relying on an entity/for UI
// currently just draws a box as a container
void RenderSystem::drawBulletStack(const mat4 &projection, const mat4 &view)
{
	StackCompile &stack = registry.stackCompile.get(registry.players.entities[0]);

	Entity stackEntity = registry.stackUI.entities[0];
	StackUI &stackui = registry.stackUI.get(stackEntity);

	Motion motion = Motion();
	motion.scale = stackui.stackSize;
	motion.position = stackui.stackPos;

	Motion motionOutline = Motion();
	motionOutline.scale = motion.scale + vec2(10);
	motionOutline.position = motion.position;
	vec3 outlineColor = vec3(1.0);

	vec3 color = vec3(0);
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
						  sizeof(TexturedVertex), (void *)0);
	gl_has_errors();

	glEnableVertexAttribArray(in_texcoord_loc);
	glVertexAttribPointer(
		in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
		(void *)sizeof(
			vec3)); // note the stride to skip the preceeding vertex position

	glActiveTexture(GL_TEXTURE0);
	gl_has_errors();

	GLuint texture_id =
		texture_gl_handles[(GLuint)name_to_texture["enemy_bullet_square.png"]];

	glBindTexture(GL_TEXTURE_2D, texture_id);
	gl_has_errors();

	// Getting uniform locations for glUniform* calls
	// want to overwrite the colour with given; could also use a separate shader program
	GLint change_color_uloc = glGetUniformLocation(program, "changeColor");
	glUniform1i(change_color_uloc, 1);
	float alpha = 1;
	float effect_alpha = 1;

	GameState& gameState = registry.gameStates.components[0];
	if (registry.invincibles.has(registry.players.entities[0]) && !registry.dashes.has(registry.players.entities[0])
		&& !gameState.gameOver && !gameState.gamePaused && !gameState.dialogueScene)
	{

		Invincible& invincible = registry.invincibles.get(registry.players.entities[0]);
		//float effect_alpha = abs(sin(invincible.countdown / invincible.max * 10) * 0.3);
		//glUniform1f(effect_alpha_uloc, effect_alpha);
		color = COLOR_RED * 0.7f + vec3(0);
		alpha = 1 - abs(sin(invincible.countdown / invincible.max * 10) * 0.5);
		// effect_alpha = lerp( 0.5f, 0.f, (invincible.max-invincible.countdown) / invincible.max);
	}

	GLint alpha_uloc = glGetUniformLocation(program, "alpha");
	glUniform1f(alpha_uloc, alpha);
	GLint effect_alpha_uloc = glGetUniformLocation(program, "effectAlpha");
	glUniform1f(effect_alpha_uloc, effect_alpha);

	gl_has_errors();

	// Get number of indices from index buffer, which has elements uint16_t
	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	gl_has_errors();

	GLsizei num_indices = size / sizeof(uint16_t);
	// GLsizei num_triangles = num_indices / 3;

	GLint currProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);

	GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (float *)&projection);

	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, (float *)&view);

	gl_has_errors();

	mat4 transform = createNormalModel(motion, vec2(0));
	mat4 transformOutline = createNormalModel(motionOutline, vec2(0));

	// draw outline here
	GLuint transform_loc = glGetUniformLocation(currProgram, "model");
	glUniformMatrix4fv(transform_loc, 1, GL_FALSE, (float *)&transformOutline);
	GLint color2_uloc = glGetUniformLocation(program, "fcolor");
	glUniform3fv(color2_uloc, 1, (float *)&outlineColor);
	// Drawing of num_indices/3 triangles specified in the index buffer
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);

	// draw actual stack here
	GLuint transform2_loc = glGetUniformLocation(currProgram, "model");
	glUniformMatrix4fv(transform2_loc, 1, GL_FALSE, (float *)&transform);
	GLint color_uloc = glGetUniformLocation(program, "fcolor");
	glUniform3fv(color_uloc, 1, (float *)&color);
	// Drawing of num_indices/3 triangles specified in the index buffer
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);

	gl_has_errors();

	// draw bullet stack here for now, based on bullet effects
	for (int i = 0; i < stack.currStack.size(); i++)
	{
		// defaults
		std::string bulletShape = bulletEffectShapes.at(BulletEffectType::Inert);
		vec3 bulletColor = COLOR_GREY_MED;
		// no variance on shape for now
		if (bulletEffectShapes.count(stack.currStack[i].type) > 0)
		{
			bulletShape = bulletEffectShapes.at(stack.currStack[i].type);
		}
		if (bulletEffectColors.count(stack.currStack[i].type) > 0)
		{
			bulletColor = bulletEffectColors.at(stack.currStack[i].type);
		}

		// start from bottom to top
		drawUIBullet(vec2(stackui.bulletStartPos.x + i * stackui.bulletSize.x + i * stackui.bulletOffset, stackui.bulletStartPos.y), stackui.bulletSize,
					 bulletColor, bulletShape, projection, view);
	}
}

void RenderSystem::drawUIBullet(vec2 position, vec2 bullet_size, vec3 color, std::string shape, const mat4 &projection, const mat4 &view)
{
	Motion motion = Motion();
	motion.scale = bullet_size;
	motion.position = position;

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
						  sizeof(TexturedVertex), (void *)0);
	gl_has_errors();

	glEnableVertexAttribArray(in_texcoord_loc);
	glVertexAttribPointer(
		in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
		(void *)sizeof(
			vec3)); // note the stride to skip the preceeding vertex position

	// Enabling and binding texture to slot 0
	glActiveTexture(GL_TEXTURE0);
	gl_has_errors();

	GLuint texture_id =
		texture_gl_handles[(GLuint)name_to_texture[shape]];

	glBindTexture(GL_TEXTURE_2D, texture_id);
	gl_has_errors();

	// Getting uniform locations for glUniform* calls
	GLint color_uloc = glGetUniformLocation(program, "fcolor");
	glUniform3fv(color_uloc, 1, (float *)&color);
	// want to overwrite the colour with given; could also use a separate shader program
	GLint change_color_uloc = glGetUniformLocation(program, "changeColor");
	glUniform1i(change_color_uloc, 0);
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
	GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (float *)&projection);

	mat4 transform = createNormalModel(motion, vec2(0));

	GLuint transform_loc = glGetUniformLocation(currProgram, "model");
	glUniformMatrix4fv(transform_loc, 1, GL_FALSE, (float *)&transform);

	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, (float *)&view);
	gl_has_errors();
	// Drawing of num_indices/3 triangles specified in the index buffer
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);
	gl_has_errors();
}

void RenderSystem::drawLaserIndicator(Entity entity, const mat4 &projection, const mat4 &view)
{

	assert(registry.renderRequests.has(entity));
	const RenderRequest &render_request = registry.renderRequests.get(entity);
	Motion &motion1 = registry.motions.get(entity);
	Laser &laser = registry.lasers.get(entity);
	Motion motion = Motion();
	motion.angle = motion1.angle;
	motion.position = registry.motions.get(registry.lasers.get(entity).start).position + vec2(cos(motion.angle), sin(motion.angle)) * (laser.maxLength / 2.0f);
	motion.scale = vec2(laser.maxLength, motion.scale.y * 2.f);
	vec2 offset = render_request.offset;

	const GLuint program = (GLuint)effects[EFFECT_ASSET_ID::TEXTURED];

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
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *)0);
	gl_has_errors();

	glEnableVertexAttribArray(in_texcoord_loc);
	glVertexAttribPointer(in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *)sizeof(vec3));
	gl_has_errors();

	GLint tile_uloc = glGetUniformLocation(program, "tile");
	glUniform1i(tile_uloc, true);
	gl_has_errors();

	vec2 tiling = vec2(laser.maxLength / 48.f, 1);
	GLint tiling_uloc = glGetUniformLocation(program, "tiling");
	glUniform2fv(tiling_uloc, 1, (float *)&tiling);

	// Enable and bind the texture to slot 0
	glActiveTexture(GL_TEXTURE0);
	gl_has_errors();
	GLuint texture_id = texture_gl_handles[(GLuint)name_to_texture["LaserIndicator.png"]];

	glBindTexture(GL_TEXTURE_2D, texture_id);
	gl_has_errors();

	GLint currProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
	// Get number of indices from index buffer, which has elements uint16_t
	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	gl_has_errors();

	GLsizei num_indices = size / sizeof(uint16_t);

	// Setting uniform values to the currently bound program
	WindowState &windowState = registry.windowStates.components[0];
	Motion &playerMotion = registry.motions.get(registry.players.entities[0]);
	float wallThickness = 100 + 50;
	float zoom = 1;
	// note: perspective seems to make no difference?
	GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (float *)&projection);

	mat4 transform = glm::mat4(1.0);
	transform = createFollowCameraModel(motion, offset);
	GLuint transform_loc = glGetUniformLocation(currProgram, "model");
	glUniformMatrix4fv(transform_loc, 1, GL_FALSE, (float *)&transform);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, (float *)&view);

	gl_has_errors();

	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);

	gl_has_errors();
}

// draw collider shape based on shape texture passed
// only draws circles and boxes for now
// (more work required to include polygons)
void RenderSystem::drawCollider(Entity entity, std::string shape, const mat4 &projection, const mat4 &view)
{
	Motion &motion = registry.motions.get(entity);
	Motion colliderMotion = Motion();
	colliderMotion.position = motion.position;
	colliderMotion.angle = motion.angle;

	// note: because of repeat, circles display wierdly rn
	if (shape == "circle.png")
	{
		auto &circle = registry.circleColliders.get(entity);
		colliderMotion.scale = { circle.radius * 2, circle.radius * 2};
		//std::cout << "pos: " << colliderMotion.position.x << ", " << colliderMotion.position.y << std::endl;
		//std::cout << colliderMotion.scale.x << ", " << colliderMotion.scale.y << std::endl;
	}
	// note: this isn't working for offset aabbs
	else if (shape == "rectangle.png")
	{
		auto &aabb = registry.aabbs.get(entity);
		colliderMotion.position = vec2(motion.position.x + (abs(aabb.bottomRight.x) - abs(aabb.topLeft.x)) / 2.f, motion.position.y + (abs(aabb.bottomRight.y) - abs(aabb.topLeft.y)) / 2.f);
		colliderMotion.scale = {aabb.bottomRight.x - aabb.topLeft.x, aabb.bottomRight.y - aabb.topLeft.y};
	}
	else {
		assert(false);
	}

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

	GLint tile_uloc = glGetUniformLocation(program, "tile");
	glUniform1i(tile_uloc, 0);
	gl_has_errors();

	glEnableVertexAttribArray(in_position_loc);
	glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
						  sizeof(TexturedVertex), (void *)0);
	gl_has_errors();

	glEnableVertexAttribArray(in_texcoord_loc);
	glVertexAttribPointer(
		in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
		(void *)sizeof(
			vec3)); // note the stride to skip the preceeding vertex position

	// Enabling and binding texture to slot 0
	glActiveTexture(GL_TEXTURE0);
	gl_has_errors();

	assert(registry.renderRequests.has(entity));
	GLuint texture_id =
		texture_gl_handles[(GLuint)name_to_texture[shape]];

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

	GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (float *)&projection);

	mat4 transform = createFollowCameraModel(colliderMotion, vec2(0));

	GLuint transform_loc = glGetUniformLocation(currProgram, "model");
	glUniformMatrix4fv(transform_loc, 1, GL_FALSE, (float *)&transform);

	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, (float *)&view);
	gl_has_errors();

	// Drawing of num_indices/3 triangles specified in the index buffer
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);
	gl_has_errors();
}

void RenderSystem::drawDashes(const mat4 &projection, const mat4 &view)
{
	vec2 pos = {77, 220};
	vec2 scale = {50, 50};
	float offset = 10;

	Player &player = registry.players.get(registry.players.entities[0]);

	// draw all filled dash charges first
	for (int i = 0; i < player.currDashCharges; ++i)
	{
		drawDashCharges(vec2(pos.x + i * (scale.x + offset), pos.y), scale, false, 0, 0, projection, view);
	}

	// draw currently charging dash charge, if any
	if (player.currDashCharges < WorldSystem::getModifiedValue(PlayerNumDash, player.maxDashCharges))
	{
		drawDashCharges(vec2(pos.x + player.currDashCharges * (scale.x + offset), pos.y), scale, true, player.currDashCooldown, WorldSystem::getModifiedValue(PlayerDashCDR, player.baseDashCDR), projection, view);
	}

	// draw empty dash charges last, if any
	for (int i = player.currDashCharges + 1; i < WorldSystem::getModifiedValue(PlayerNumDash, player.maxDashCharges); ++i)
	{
		drawDashCharges(vec2(pos.x + i * (scale.x + offset), pos.y), scale, -1, 0, 0, projection, view);
	}

	gl_has_errors();
}

void RenderSystem::drawDashCharges(vec2 position, vec2 scale, int isCharging, float cooldown, float max, const mat4 &projection, const mat4 &view)
{
	Motion motion = Motion();
	motion.scale = scale;
	motion.position = position;

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
						  sizeof(TexturedVertex), (void *)0);
	gl_has_errors();

	glEnableVertexAttribArray(in_texcoord_loc);
	glVertexAttribPointer(
		in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
		(void *)sizeof(
			vec3)); // note the stride to skip the preceeding vertex position

	// Enabling and binding texture to slot 0
	glActiveTexture(GL_TEXTURE0);
	gl_has_errors();

	GLuint texture_id =
		texture_gl_handles[(GLuint)name_to_texture["chevron.png"]];

	glBindTexture(GL_TEXTURE_2D, texture_id);
	gl_has_errors();

	// Getting uniform locations for glUniform* calls
	if (!isCharging)
	{
		vec3 color = COLOR_YELLOW;
		GLint color_uloc = glGetUniformLocation(program, "fcolor");
		glUniform3fv(color_uloc, 1, (float *)&color);
		GLint change_color_uloc = glGetUniformLocation(program, "changeColor");
		glUniform1i(change_color_uloc, 0);
		GLint charge_boundary_uloc = glGetUniformLocation(program, "chargeBoundary");
		glUniform1f(charge_boundary_uloc, 1.0);
	}
	// consider changing color later
	else if (isCharging == -1)
	{
		vec3 color = COLOR_YELLOW * vec3(0.2, 0.2, 0.2); // grey
		GLint color_uloc = glGetUniformLocation(program, "fcolor");
		glUniform3fv(color_uloc, 1, (float *)&color);
		GLint change_color_uloc = glGetUniformLocation(program, "changeColor");
		glUniform1i(change_color_uloc, 1);
		GLint charge_boundary_uloc = glGetUniformLocation(program, "chargeBoundary");
		glUniform1f(charge_boundary_uloc, 1.0);
	}
	else
	{
		vec3 color = COLOR_YELLOW * vec3(0.60, 0.60, 0.60); // grey
		GLint color_uloc = glGetUniformLocation(program, "fcolor");
		glUniform3fv(color_uloc, 1, (float *)&color);
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
	GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (float *)&projection);

	mat4 transform = createNormalModel(motion, vec2(0));

	GLuint transform_loc = glGetUniformLocation(currProgram, "model");
	glUniformMatrix4fv(transform_loc, 1, GL_FALSE, (float *)&transform);

	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, (float *)&view);
	gl_has_errors();
	// Drawing of num_indices/3 triangles specified in the index buffer
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);
	gl_has_errors();
}

void RenderSystem::drawHPbar(Entity &entity, const mat4 &projection, const mat4 &view)
{
	WindowState &windowState = registry.windowStates.components[0];
	Motion &motion = registry.motions.get(entity);
	Motion HPBarMotion = Motion();
	HPBarMotion.scale = {600, 30};
	HPBarMotion.position = {windowState.width / 2, windowState.height * 0.92};

	if (!registry.bosses.has(entity))
	{
		HPBarMotion.position = motion.position + vec2(0, motion.scale.y / 2 + 10);
		HPBarMotion.scale = {100, 10};
	}

	if (registry.damageds.has(entity) && !registry.gameStates.components[0].gamePaused && !registry.gameStates.components[0].gameOver)
	{
		HPBarMotion.position.x += (rand() % 10) - 5;
		HPBarMotion.position.y += (rand() % 10) - 5;
	}

	float max = registry.enemies.get(entity).maxHealth;
	float current = registry.enemies.get(entity).currHealth;

	// for now, draw bullets using textures
	const GLuint used_effect_enum = (GLuint)EFFECT_ASSET_ID::HP_BAR;
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
						  sizeof(TexturedVertex), (void *)0);
	gl_has_errors();

	glEnableVertexAttribArray(in_texcoord_loc);
	glVertexAttribPointer(
		in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
		(void *)sizeof(
			vec3)); // note the stride to skip the preceeding vertex position

	// Enabling and binding texture to slot 0
	glActiveTexture(GL_TEXTURE0);
	gl_has_errors();

	GLuint texture_id =
		texture_gl_handles[(GLuint)name_to_texture["parallelogram.png"]];

	glBindTexture(GL_TEXTURE_2D, texture_id);
	gl_has_errors();

	vec3 color = {11.50, 0.0, 0.0}; // red
	GLint color_uloc = glGetUniformLocation(program, "fcolor");
	glUniform3fv(color_uloc, 1, (float *)&color);
	GLint change_color_uloc = glGetUniformLocation(program, "changeColor");
	glUniform1i(change_color_uloc, 1);

	// charge boundary is the percentage of health left, finetuned to account for the shape of the hp bar
	float chargeBoundary = glm::lerp(0.f, 0.94f, (max - current) / max);

	GLint charge_boundary_uloc = glGetUniformLocation(program, "chargeBoundary");
	glUniform1f(charge_boundary_uloc, chargeBoundary);

	GLint alpha_uloc = glGetUniformLocation(program, "alpha");
	glUniform1f(alpha_uloc, 1);
	gl_has_errors();

	GLint tile_uloc = glGetUniformLocation(program, "tile");
	glUniform1i(tile_uloc, 0);
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

	GLuint projection_loc = glGetUniformLocation(currProgram, "projection");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (float *)&projection);

	mat4 transform = createFollowCameraModel(HPBarMotion, vec2(0));
	if (registry.bosses.has(entity))
	{
		transform = createNormalModel(HPBarMotion, vec2(0));
	}

	GLuint transform_loc = glGetUniformLocation(currProgram, "model");
	glUniformMatrix4fv(transform_loc, 1, GL_FALSE, (float *)&transform);

	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, (float *)&view);
	gl_has_errors();

	// Drawing of num_indices/3 triangles specified in the index buffer
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);
	gl_has_errors();
}

mat4 createNormalModel(Motion &motion, vec2 offset = vec2(0))
{
	mat4 transform = glm::mat4(1.0);
	transform = glm::translate(transform, vec3(motion.position, 0.0f));
	transform = glm::rotate(transform, motion.angle, vec3(0.0, 0.0, 1.0));
	transform = glm::translate(transform, vec3(offset * glm::normalize(motion.scale), 0.0f));
	transform = glm::scale(transform, vec3(motion.scale.x, motion.scale.y, 1.0));

	return transform;
}

mat4 createFollowCameraModel(Motion &motion, vec2 offset = vec2(0))
{
	WindowState &windowState = registry.windowStates.components[0];
	Camera &camera = registry.cameras.components[0];
	//Motion& targetMotion = registry.motions.get(camera.target);
	mat4 transform = glm::mat4(1.0);
	transform = glm::translate(transform, vec3(windowState.width / 2, windowState.height / 2, 0));
	transform = glm::scale(transform, vec3(camera.zoom));
	transform = glm::translate(transform,
							   vec3(motion.position.x - camera.lookAtPos.x,
									motion.position.y - camera.lookAtPos.y,
									0.0));
	transform = glm::rotate(transform, motion.angle, vec3(0.0, 0.0, 1.0));
	transform = glm::translate(transform, vec3(offset * glm::normalize(motion.scale), 0.0f));
	transform = glm::scale(transform, vec3(motion.scale.x, motion.scale.y, 1.0));

	return transform;
}

mat4 createFollowCameraModelText(Motion &motion, vec2 offset)
{
	WindowState &windowState = registry.windowStates.components[0];
	Camera &camera = registry.cameras.components[0];
	//Motion& targetMotion = registry.motions.get(camera.target);
	Room &room = registry.maps.components[0].currRoom;
	mat4 transform = glm::mat4(1.0);
	transform = glm::translate(transform, vec3(windowState.width / 2, windowState.height / 2, 0));
	transform = glm::scale(transform, vec3(camera.zoom));
	transform = glm::translate(transform,
							   vec3(motion.position.x - camera.lookAtPos.x,
									motion.position.y - (windowState.height - camera.lookAtPos.y),
									0.0));
	transform = glm::rotate(transform, motion.angle, vec3(0.0, 0.0, 1.0));
	transform = glm::translate(transform, vec3(offset * glm::normalize(motion.scale), 0.0f));
	transform = glm::scale(transform, vec3(motion.scale.x, motion.scale.y, 1.0));

	return transform;
}
