#pragma once
#include <vector>

#include "tiny_ecs.hpp"
#include "components.hpp"

class ECSRegistry
{
	// Callbacks to remove a particular or all entities in the system
	std::vector<ContainerInterface*> registry_list;

public:
	// Manually created list of all components this game has
	ComponentContainer<Motion> motions;
	ComponentContainer<Collision> collisions;
	ComponentContainer<Player> players;
	ComponentContainer<Mesh*> meshPtrs;
	ComponentContainer<RenderRequest> renderRequests;
	ComponentContainer<ScreenState> screenStates;
	ComponentContainer<IOState> ioStates;
	ComponentContainer<GameState> gameStates;
	ComponentContainer<WindowState> windowStates;
	ComponentContainer<Enemy> enemies;
	ComponentContainer<DebugComponent> debugComponents;
	ComponentContainer<vec3> colors;
	ComponentContainer<Sprites> sprites;
	ComponentContainer<CollisionShape> collisionShapes;
	ComponentContainer<SpriteTimer> spriteTimers;
	ComponentContainer<TextRenderRequest> textRenderRequests;
	ComponentContainer<UI> uis;

	ComponentContainer<StackCompile> stackCompile;
	ComponentContainer<Invincible> invincibles;
	ComponentContainer<CircleCollider> circleColliders;
	ComponentContainer<PlayerBullet> playerBullets;
	ComponentContainer<EnemyBullet> enemyBullets;
	ComponentContainer<PolyCollider> polyColliders;
	ComponentContainer<WallCollider> walls;
	ComponentContainer<Shoots> shoots;
    ComponentContainer<Invisible> invisibles;
	ComponentContainer<Dash> dashes;


	// constructor that adds all containers for looping over them
	// IMPORTANT: Don't forget to add any newly added containers!
	ECSRegistry()
	{
		registry_list.push_back(&motions);
		registry_list.push_back(&collisions);
		registry_list.push_back(&players);
		registry_list.push_back(&meshPtrs);
		registry_list.push_back(&renderRequests);
		registry_list.push_back(&screenStates);
		registry_list.push_back(&ioStates);
		registry_list.push_back(&gameStates);
		registry_list.push_back(&windowStates);
		registry_list.push_back(&enemies);
		registry_list.push_back(&debugComponents);
		registry_list.push_back(&colors);
		registry_list.push_back(&sprites);
		registry_list.push_back(&spriteTimers);
		registry_list.push_back(&textRenderRequests);
		registry_list.push_back(&uis);

		registry_list.push_back(&stackCompile);
		registry_list.push_back(&invincibles);
		registry_list.push_back(&circleColliders);
		registry_list.push_back(&playerBullets);
		registry_list.push_back(&enemyBullets);
		registry_list.push_back(&polyColliders);
		registry_list.push_back(&walls);
		registry_list.push_back(&collisionShapes);
		registry_list.push_back(&shoots);
        registry_list.push_back(&invisibles);
		registry_list.push_back(&dashes);
	}

	void clear_all_components() {
		for (ContainerInterface* reg : registry_list)
			reg->clear();
	}

	void list_all_components() {
		printf("Debug info on all registry entries:\n");
		for (ContainerInterface* reg : registry_list)
			if (reg->size() > 0)
				printf("%4d components of type %s\n", (int)reg->size(), typeid(*reg).name());
	}

	void list_all_components_of(Entity e) {
		printf("Debug info on components of entity %u:\n", (unsigned int)e);
		for (ContainerInterface* reg : registry_list)
			if (reg->has(e))
				printf("type %s\n", typeid(*reg).name());
	}

	void remove_all_components_of(Entity e) {
		for (ContainerInterface* reg : registry_list)
			reg->remove(e);
	}

	// deletes all entities associated with entities
	void deleteEntityAndRelatedEntities(Entity& entity) {
		// remove all other entities associated with this entity
		// notably, check collision outlines
		if (collisionShapes.has(entity)) {
			for (Entity shape : collisionShapes.get(entity).shapes) {
				remove_all_components_of(shape);
			}
		}
		remove_all_components_of(entity);
	}
};

extern ECSRegistry registry;