#pragma once
#include <vector>

#include "tiny_ecs.hpp"
#include "components.hpp"
#include "actor_components.hpp"
#include "map_components.hpp"
#include "particle_components.hpp"

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
	ComponentContainer<IOState> ioStates;
	ComponentContainer<GameState> gameStates;
	ComponentContainer<WindowState> windowStates;
	ComponentContainer<Enemy> enemies;
	ComponentContainer<DebugComponent> debugComponents;
	ComponentContainer<vec3> colors;
	ComponentContainer<CollisionShape> collisionShapes;
	ComponentContainer<TextRenderRequest> textRenderRequests;
	ComponentContainer<DialogueLines> dialogueLines;
	ComponentContainer<Sprites> sprites;
	ComponentContainer<SpriteTimer> spriteTimers;
	ComponentContainer<BG> backgrounds;
	ComponentContainer<EnemyMovement> enemyMovement;
	ComponentContainer<StackCompile> stackCompile;
	ComponentContainer<Invincible> invincibles;
	ComponentContainer<CircleCollider> circleColliders;
	ComponentContainer<PlayerBullet> playerBullets;
	ComponentContainer<EnemyBullet> enemyBullets;
	ComponentContainer<PolyCollider> polyColliders;
	ComponentContainer<WallCollider> walls;
	ComponentContainer<AABBCollider> aabbs;
	ComponentContainer<MeshCollider> meshColliders;
	ComponentContainer<PlayerAttackData> shoots;
    ComponentContainer<Invisible> invisibles;
	ComponentContainer<Dash> dashes;
	ComponentContainer <Frame> frames;
	ComponentContainer<EmitParticle> emitParticles;
	ComponentContainer<Burst> bursts;
	ComponentContainer<HomingBullet> homes;
	ComponentContainer<Laser> lasers;
	ComponentContainer<Fade> fades;
	ComponentContainer<Map> maps;
	ComponentContainer<Deleted> deleteds;
	ComponentContainer<StackUI> stackUI;
	ComponentContainer<GameUI> gameUIs;
	ComponentContainer<DialogueUI> dialogueUIs;
	ComponentContainer<MenuUI> menuUIs;
	ComponentContainer<GameUIText> gameUITexts;
	ComponentContainer<DialogueUIText> dialogueUITexts;
	ComponentContainer<MenuUIText> menuUITexts;
	ComponentContainer<Damaged> damageds;
	ComponentContainer<Door> doors;
	ComponentContainer<DoorSymbol> doorSymbols;
	ComponentContainer<Bound> bounds; //room boundaries
	ComponentContainer<MapRequest> mapRequests;
	ComponentContainer<BossEnemy> bosses;
	ComponentContainer<BossParts> bossParts;
	ComponentContainer<Animation> animations;
	ComponentContainer<BeeEnemy> bees;
	ComponentContainer<AnimationSequence> animationSequences;
	ComponentContainer<SoundRequest> soundRequests;
	ComponentContainer<Boid> boids;
	ComponentContainer<Critter> critters;
	ComponentContainer<Object> objects;
	ComponentContainer<DialogueChoice> dialogueChoices;
	ComponentContainer<InteractableObject> interactables;
	ComponentContainer<DialogueRequest> dialogueRequests;
	ComponentContainer<InteractableReaction> interactableReactions;
	ComponentContainer<NearbyInteractables> nearbyInteractables;
	ComponentContainer<ScreenCutIn> screenCutIns;
	ComponentContainer<Cursor> cursors;
	ComponentContainer<InteractIndicator> interactIndicators;
	ComponentContainer<MenuOverlayUI> menuOverlayUIs;
	ComponentContainer<MenuOverlayUIText> menuOverlayUITexts;
	ComponentContainer<Ignore> ignores;
	ComponentContainer<Healer> healers;
	ComponentContainer<EffectStack> effectStacks;
	ComponentContainer<MenuChoice> menuChoices;
	ComponentContainer<InvisibleEnemy> invisibleEnemy;
	ComponentContainer<Parent> parents;
	ComponentContainer<Camera> cameras;
	ComponentContainer<GameOverlayUIText> gameOverlayUITexts;
	ComponentContainer<EnemyGroup> enemyGroups;

	// constructor that adds all containers for looping over them
	// IMPORTANT: Don't forget to add any newly added containers!
	ECSRegistry()
	{
		registry_list.push_back(&motions);
		registry_list.push_back(&collisions);
		registry_list.push_back(&players);
		registry_list.push_back(&meshPtrs);
		registry_list.push_back(&renderRequests);
		registry_list.push_back(&ioStates);
		registry_list.push_back(&gameStates);
		registry_list.push_back(&windowStates);
		registry_list.push_back(&enemies);
		registry_list.push_back(&debugComponents);
		registry_list.push_back(&colors);
		registry_list.push_back(&textRenderRequests);
		registry_list.push_back(&frames);
		registry_list.push_back(&emitParticles);
		registry_list.push_back(&dialogueLines);
		registry_list.push_back(&enemyMovement);
		registry_list.push_back(&backgrounds);
		registry_list.push_back(&stackCompile);
		registry_list.push_back(&invincibles);
		registry_list.push_back(&circleColliders);
		registry_list.push_back(&playerBullets);
		registry_list.push_back(&enemyBullets);
		registry_list.push_back(&polyColliders);
		registry_list.push_back(&walls);
		registry_list.push_back(&aabbs);
		registry_list.push_back(&meshColliders);
		registry_list.push_back(&collisionShapes);
		registry_list.push_back(&shoots);
        registry_list.push_back(&invisibles);
		registry_list.push_back(&dashes);
		registry_list.push_back(&homes);
		registry_list.push_back(&lasers);
		registry_list.push_back(&bursts);
		registry_list.push_back(&fades);
		registry_list.push_back(&maps);
		registry_list.push_back(&deleteds);
		registry_list.push_back(&doors);
		registry_list.push_back(&bounds);
		registry_list.push_back(&mapRequests);
		registry_list.push_back(&stackUI);
		registry_list.push_back(&gameUIs);
		registry_list.push_back(&dialogueUIs);
		registry_list.push_back(&menuUIs);
		registry_list.push_back(&gameUITexts);
		registry_list.push_back(&dialogueUITexts);
		registry_list.push_back(&menuUITexts);
		registry_list.push_back(&damageds);
		registry_list.push_back(&bosses);
		registry_list.push_back(&bossParts);
		registry_list.push_back(&animations);
		registry_list.push_back(&sprites);
		registry_list.push_back(&spriteTimers);
		registry_list.push_back(&doorSymbols);
		registry_list.push_back(&bees);
		registry_list.push_back(&animationSequences);
		registry_list.push_back(&soundRequests);
		registry_list.push_back(&boids);
		registry_list.push_back(&critters);
		registry_list.push_back(&objects);
		registry_list.push_back(&dialogueChoices);
		registry_list.push_back(&interactables);
		registry_list.push_back(&dialogueRequests);
		registry_list.push_back(&interactableReactions);
		registry_list.push_back(&nearbyInteractables);
		registry_list.push_back(&screenCutIns);
		registry_list.push_back(&cursors);
		registry_list.push_back(&interactIndicators);
		registry_list.push_back(&menuOverlayUIs);
		registry_list.push_back(&menuOverlayUITexts);
		registry_list.push_back(&ignores);
		registry_list.push_back(&healers);
		registry_list.push_back(&effectStacks);
		registry_list.push_back(&menuChoices);
		registry_list.push_back(&invisibleEnemy);
		registry_list.push_back(&parents);
		registry_list.push_back(&cameras);
		registry_list.push_back(&gameOverlayUITexts);
		registry_list.push_back(&enemyGroups);
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
		if (parents.has(entity)) {
			for (Entity child : parents.get(entity).children) {
				if (!deleteds.has(child)) remove_all_components_of(child);
			}
		}
		if (enemyGroups.has(entity)) {
			for (Entity other : enemyGroups.get(entity).others) {
				if (!deleteds.has(other)) remove_all_components_of(other);
			}
		}
		remove_all_components_of(entity);
	}
};

extern ECSRegistry registry;