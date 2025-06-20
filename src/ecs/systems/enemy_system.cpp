#include "enemy_system.hpp"
#include <iostream>
#include <glm/trigonometric.hpp>

#include "tiny_ecs_registry.hpp"
#include "common.hpp"
#include "world_init.hpp"
#include "actor_components.hpp"
#include <thread>
#include <vector>
#include <glm/gtx/compatibility.hpp>
#include "sound_system.hpp"
#include <chrono>

#include "ai_system.hpp"
#include <mutex>

#include "actor_components.hpp"
#include "actor_components.hpp"
#include "components/presets/particle_presets.hpp"
#include "utils/random.hpp"
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

std::mutex beeMutex;

float COOLDOWN_SHOOT_MS = 2000;
float BASE_BULLET_SPEED = 1;
float PLACEHOLDER_FOR_ANGLE = 0.f;

EnemySystem::EnemySystem(RenderSystem *renderer, SoundSystem *sound)
{
    render = renderer;
    this->sound = sound;
};

EnemySystem::~EnemySystem() {
};

void EnemySystem::step(float Elapsed_ms)
{
    Entity player = registry.players.entities[0];
    Motion &playerMotion = registry.motions.get(player);
    // std::cout << "current enemy :" << registry.enemies.entities.size() << std::endl;
    // std::cout << "current bee enemy: " << registry.bees.entities.size() << std::endl;
    std::vector<Entity> pendingDeletion;
    std::vector<vec3> createBees;

    // handle enemy moving & shooting
    for (Entity entity : registry.enemies.entities)
    {
        if (registry.fades.has(entity) || registry.spawnings.has(entity))
            continue;
        if (!registry.enemies.has(entity))
        {
            continue;
        }
        float elapsed_ms = getAdjustedTime(Elapsed_ms, entity);
        Enemy &enemy = registry.enemies.get(entity);
        Motion &motion = registry.motions.get(entity);
        vec2 pos = motion.position;
        float angle = motion.angle;

        // Lightning bullet creation
        if (((float)enemy.currHealth / (float)enemy.maxHealth < 0.3) && (rand() % 1000 > 990))
        {
            createLightningBullet(render, motion.position);
        }

        // Key bullet creation
        if (((float)enemy.currHealth / (float)enemy.maxHealth > 0.7) && (rand() % 1000 < 1))
        {
            bool shouldKey = (registry.enemyBullets.components.size() > 0);
            for (EnemyBullet &eb : registry.enemyBullets.components)
            {
                if (eb.bulletEffects[0].type == Key)
                {
                    shouldKey = false;
                    break;
                }
            }
            if (shouldKey)
                createKeyBullet(render, motion.position);
        }

        if (registry.enemyParts.has(entity)) {
            EnemyPart &part = registry.enemyParts.get(entity);
            if (!registry.enemies.has(part.parent) || registry.fades.has(part.parent) || registry.deleteds.has(part.parent)){
                destruct(enemy);
            }
        }

        if (registry.roomWideBuffers.has(entity)) {
            Map& map = registry.maps.components[0];
            if (registry.enemies.entities.size() <= registry.roomWideBuffers.entities.size() && map.currRoom.enemiesToSpawn.empty() && map.currRoom.preset.enemies.empty())
                destruct(enemy);
        }

        if (registry.bossParts.has(entity) && registry.bosses.entities.empty())
        {
            if (registry.bossParts.get(entity).diesWithBoss)
                destruct(enemy);
        }

        // merge bee logic
        EnemyPattern &pattern = enemy.currEnemyPattern();
        if (registry.bees.has(entity) && pattern.type == EnemyBehavior::MERGE_BEE && registry.bees.get(entity).nearbyBees.size() > 0)
        {
            // std::cout << "MERGING WITH BEE SIZE:" << registry.bees.get(entity).nearbyBees.size() << std::endl;
            merge(entity, pattern, pendingDeletion);
        }

        if (registry.healers.has(entity) && pattern.type == EnemyBehavior::HEALING)
        {
            Healer& healer = registry.healers.get(entity);
            healer.coolDown -= elapsed_ms;
            heal(entity, pattern);
        }

        if (registry.buffers.has(entity) && (pattern.type == EnemyBehavior::GRANTINGBUFFS || pattern.type == EnemyBehavior::GRANTINGBUFFSAOE))
        {
            Buffer& buffer = registry.buffers.get(entity);
            buffer.cooldown -= elapsed_ms;
            if (buffer.cooldown < 0) {
                grantBuff(entity, pattern);
                buffer.cooldown = buffer.maxCoolDown;
            }
        }

        if (pattern.type == EnemyBehavior::DEATHSTATE)
        {
            // std::cout << "got here" << std::endl;
            if (!registry.deleteds.has(entity))
            {
                Fade &f = registry.fades.emplace(entity);
                registry.deleteds.emplace(entity);
                if (registry.persistentSounds.has(entity))
                    registry.persistentSounds.get(entity).stop();
            }
        }

        // for (Entity deletedBee : pendingDeletion)
        //{
        //     //std::cout << pendingDeletion.size() << "to be delted" << std::endl;
        //     if (!registry.deleteds.has(deletedBee))
        //         registry.deleteds.emplace(deletedBee);
        //     registry.bees.remove(deletedBee);
        //     registry.enemies.remove(deletedBee);
        // }
        if (registry.boids.has(entity))
        {
            Boid &boid = registry.boids.get(entity);
            boid.position += boid.velocity * (elapsed_ms / 1000.f);
            motion.position += boid.velocity * (elapsed_ms / 1000.f);

            if (glm::length2(boid.velocity) > 0.0f)
            {
                motion.angle = atan2(boid.velocity.y, boid.velocity.x);
            }

            continue;
        }

        // move enemy using lerp
        if (registry.enemyMovement.has(entity))
        {

            EnemyMovement &movement = registry.enemyMovement.get(entity);
            vec2 direction = movement.posB - movement.posA;
            if (pattern.type == EnemyBehavior::ROTATE_IN_PLACE)
            {
                float angularSpeed = movement.angularSpeed * 2 * M_PI / 360.0f;
                float rotationChange = angularSpeed * elapsed_ms / 1000.f;
                if (enemy.rotationBehaviour != EnemyRotationBehavior::NONE)
                    rotationChange *= enemy.rotatePower;
                motion.angle += rotationChange;
            } else if (registry.wormBodies.has(entity) || registry.wormHeads.has(entity))
            {
                // Do nothing!
            } else if (direction != vec2(0, 0))
            {
                float targetAngle = atan2(direction.y, direction.x);
                float deltaAngle = targetAngle - motion.angle;
                float angularSpeedRad = movement.angularSpeed * enemy.rotatePower * 2 * M_PI / 360.0f;
                float maxChange = angularSpeedRad * elapsed_ms / 1000.0f;
                if (deltaAngle > M_PI)
                    deltaAngle -= 2 * M_PI;
                if (deltaAngle < -M_PI)
                    deltaAngle += 2 * M_PI;
                if (deltaAngle > maxChange)
                    deltaAngle = maxChange;
                if (deltaAngle < -maxChange)
                    deltaAngle = -maxChange;

                motion.angle += deltaAngle;
                motion.angle = motion.angle - 2.f * M_PI * floor(motion.angle / (2.f * M_PI));

                // Keeps assets facing upwards regardless of rotation
                if (enemy.rotationBehaviour == EnemyRotationBehavior::FACE_UP)
                {
                    if (motion.angle > M_PI / 2.f && motion.angle < 3.f * M_PI / 2.f)
                    {
                        motion.scale = (motion.scale.y > 0) ? motion.scale * vec2(1, -1) : motion.scale;
                    }
                    else
                    {
                        motion.scale = (motion.scale.y < 0) ? motion.scale * vec2(1, -1) : motion.scale;
                    }
                }
                else if (enemy.rotationBehaviour == EnemyRotationBehavior::SPIN)
                {
                    float angularSpeed = movement.angularSpeed * M_PI / 360.0f;
                    float rotationChange = angularSpeed * elapsed_ms / 1000.f;
                    motion.angle += rotationChange;
                }
                else if (enemy.rotationBehaviour == EnemyRotationBehavior::FACE_CENTER)
                {
                    WindowState &wS = registry.windowStates.components[0];
                    vec2 mid = vec2(wS.width, wS.height) * 0.5f;
                    mid = mid - motion.position;
                    motion.angle = atan2(mid.y, mid.x);
                }
                else if (enemy.rotationBehaviour == EnemyRotationBehavior::FACE_PLAYER)
                {
                    if ((!registry.bosses.has(entity) && !registry.bossParts.has(entity)) && !registry.specialRotators.has(entity) && !registry.timeModifiers.has(entity) && !registry.timeModifiers.has(registry.players.entities[0]))
                    {
                        Motion &playerMotion = registry.motions.get(registry.players.entities[0]);
                        vec2 mid = playerMotion.position - motion.position;
                        motion.angle = atan2(mid.y, mid.x);
                    } else {
                        // Assuming you have a deltaTime variable that represents the time elapsed since the last frame
                        float deltaTime = getAdjustedTime(elapsed_ms, entity) / 1000.f;

                        // Define a rotation speed (radians per second)
                        float rotationSpeed = enemy.rotatePower; // Adjust this value to control the turning speed

                        // Define a minimum turning speed (radians per second)
                        float minTurningSpeed = enemy.rotatePower/2.f; // Adjust this value to control the minimum turning speed

                        // Get the current angle of the object
                        float currentAngle = motion.angle;

                        // Calculate the target angle towards the player
                        vec2 mid = playerMotion.position - motion.position;
                        float targetAngle = atan2(mid.y, mid.x);

                        // Calculate the shortest angle difference
                        float angleDifference = targetAngle - currentAngle;
                        if (angleDifference > M_PI) {
                            angleDifference -= 2 * M_PI;
                        } else if (angleDifference < -M_PI) {
                            angleDifference += 2 * M_PI;
                        }

                        // Calculate the desired angle change for this frame
                        float desiredAngleChange = rotationSpeed * deltaTime;

                        // Ensure the angle change is at least the minimum turning speed
                        if (std::abs(angleDifference) < minTurningSpeed * deltaTime) {
                            angleDifference = (angleDifference > 0 ? 1 : -1) * minTurningSpeed * deltaTime;
                        } else if (std::abs(angleDifference) > desiredAngleChange) {
                            angleDifference = (angleDifference > 0 ? 1 : -1) * desiredAngleChange;
                        }

                        // Update the object's angle
                        motion.angle = currentAngle + angleDifference;
                    }

                }
                else if (enemy.rotationBehaviour == EnemyRotationBehavior::FACE_TWIN)
                {
                    Motion &twinMotion = registry.motions.get(registry.enemyGroups.get(entity).others[0]);
                    vec2 mid = twinMotion.position - motion.position;
                    motion.angle = atan2(mid.y, mid.x);
                }
                else if (enemy.rotationBehaviour == EnemyRotationBehavior::NONE)
                {
                    motion.angle = 0.f;
                }

                if (pattern.type == EnemyBehavior::TELEPORT)
                {
                    //std::cout << "teleporting in" << movement.posB[0] << ": " << movement.posB[1] << std::endl;
                    motion.position = movement.posB;
                }
                else if (pattern.type == EnemyBehavior::ROLLING)
                {
                    motion.velocity = glm::normalize(movement.posB - movement.posA) * movement.speed;
                    float angularSpeed = movement.angularSpeed * 20.f * M_PI / 360.0f;
                    float rotationChange = angularSpeed * elapsed_ms / 1000.f;
                    motion.angle += rotationChange * sign(motion.velocity.x) * enemy.rotatePower;
                }
                else
                {
                    float totalDistance = glm::distance(movement.posA, movement.posB);
                    movement.distanceTraveled = glm::min(movement.distanceTraveled + movement.speed * elapsed_ms / 1000.f, glm::distance(movement.posA, movement.posB));
                    motion.position = glm::lerp(movement.posA, movement.posB, movement.distanceTraveled / totalDistance);
                }
            }
            else
            {
                if (enemy.rotationBehaviour == EnemyRotationBehavior::FACE_PLAYER)
                {
                    if ((!registry.bosses.has(entity) && !registry.bossParts.has(entity))  && !registry.specialRotators.has(entity) && !registry.timeModifiers.has(entity) && !registry.timeModifiers.has(registry.players.entities[0]))
                    {
                         Motion &playerMotion = registry.motions.get(registry.players.entities[0]);
                         vec2 mid = playerMotion.position - motion.position;
                         motion.angle = atan2(mid.y, mid.x);
                    } else {
                        // Assuming you have a deltaTime variable that represents the time elapsed since the last frame
                        float deltaTime = getAdjustedTime(elapsed_ms, entity) / 1000.f; // Set this to the appropriate value

                        // Define a rotation speed (radians per second)
                        float rotationSpeed = enemy.rotatePower; // Adjust this value to control the turning speed

                        // Define a minimum turning speed (radians per second)
                        float minTurningSpeed = enemy.rotatePower/2.f; // Adjust this value to control the minimum turning speed

                        // Get the current angle of the object
                        float currentAngle = motion.angle;

                        // Calculate the target angle towards the player
                        vec2 mid = playerMotion.position - motion.position;
                        float targetAngle = atan2(mid.y, mid.x);

                        // Calculate the shortest angle difference
                        float angleDifference = targetAngle - currentAngle;
                        if (angleDifference > M_PI) {
                            angleDifference -= 2 * M_PI;
                        } else if (angleDifference < -M_PI) {
                            angleDifference += 2 * M_PI;
                        }

                        // Calculate the desired angle change for this frame
                        float desiredAngleChange = rotationSpeed * deltaTime;

                        // Ensure the angle change is at least the minimum turning speed
                        if (std::abs(angleDifference) < minTurningSpeed * deltaTime) {
                            angleDifference = (angleDifference > 0 ? 1 : -1) * minTurningSpeed * deltaTime;
                        } else if (std::abs(angleDifference) > desiredAngleChange) {
                            angleDifference = (angleDifference > 0 ? 1 : -1) * desiredAngleChange;
                        }

                        // Update the object's angle
                        motion.angle = currentAngle + angleDifference;
                    }
                }
            }
        }

        // NOTE: enemy must attack AFTER being moved
        // or else causes corrupted memory in effect/geometry/texture id and makes it a huge number
        // no idea why
        if (pattern.canAttack == true)
        {
            pattern.currAtkCD -= elapsed_ms;
            if (pattern.currAtkCD < 0)
            {
                if (!pattern.atkData.gottenRoomEffects) fetchRoomEffects(entity, pattern.atkData);
                AttackData atkData = pattern.atkData;
                if (atkData.attackType != EnemyAttackPattern::SPAWNING)
                    attack(entity, pattern, playerMotion, pos, atkData, elapsed_ms);
                else
                    spawn(entity, pattern, pos, atkData);
            }
        }
    }

    // HANDLING DAMGE FROM COLLISION
    for (auto &entity : registry.collisions.entities)
    {
        if (registry.invisibleEnemy.has(entity))
        {
            continue;
        }
        const Collision &collision = registry.collisions.get(entity);
        Entity other_entity = collision.other;
        if (registry.enemies.has(entity) && registry.playerBullets.has(other_entity) && !registry.deleteds.has(other_entity) && !registry.spawnings.has(entity) && !registry.moles.has(entity) && !registry.roomWideBuffers.has(entity))
        {
            Enemy &enemyStat = registry.enemies.get(entity);
            PlayerBullet &bulletStat = registry.playerBullets.get(other_entity);
            EnemyPattern &pattern = enemyStat.currEnemyPattern();

            if (registry.invincibles.has(entity))
            {
                //if enemy is invincible, block damage and destroy bullet
                if (!registry.deleteds.has(other_entity) && (!registry.boids.has(entity) || (registry.boids.has(entity) && registry.enemies.components.size() > registry.boids.components.size())))
                    registry.deleteds.emplace(other_entity);
                if (!registry.damageds.has(entity) && enemyStat.currHealth > 0)
                {
                    registry.damageds.emplace(entity);
                }
                else if (registry.damageds.has(entity))
                {
                    registry.damageds.get(entity).countdown = registry.damageds.get(entity).max;
                }
                continue;
            }

            // Deal damage
            float damage = registry.elites.has(entity) ? max((float)((1.f - registry.elites.get(entity).eliteLevel * 0.03) * bulletStat.damage), 1.f) : bulletStat.damage;
            if (registry.vulnerabilities.has(entity))
            {
                damage = max((float)(damage * registry.vulnerabilities.get(entity).modifier), damage);
            }
            if (checkTierThreshold(BulletRange)) {
                float dist = glm::length(registry.motions.get(entity).position - registry.motions.get(player).position);
                float snipeMultiplier = clamp(1.f, dist / 500.f, 2.f);
                damage *= snipeMultiplier;
            }
            if (registry.wormBodies.has(entity)) {
               Enemy& head = registry.enemies.get(registry.wormBodies.get(entity).head);
               if (!registry.invincibles.has(registry.wormBodies.get(entity).head)) head.currHealth -= damage;
            }
            else if (registry.instanceDamages.has(entity)) {
                InstanceDamage& instance = registry.instanceDamages.get(entity);
                instance.instance--;
                registry.enemies.get(entity).currHealth = instance.instance;
            }
            else {
                enemyStat.currHealth -= damage;
            }

            bulletStat.bulletPierce -= enemyStat.armour;
            registry.ignores.get(other_entity).ignores.push_back(entity);

            if (!registry.deleteds.has(other_entity) && bulletStat.bulletPierce < 0 && (!registry.boids.has(entity) || (registry.boids.has(entity) && registry.enemies.components.size() > registry.boids.components.size())))
                registry.deleteds.emplace(other_entity);
            if (!registry.damageds.has(entity) && enemyStat.currHealth > 0)
            {
                registry.damageds.emplace(entity);
            }
            else if (registry.damageds.has(entity))
            {
                registry.damageds.get(entity).countdown = registry.damageds.get(entity).max;
            }

            // Inflict Vulnerable on enemy if spread tier
            if (checkTierThreshold(BulletAccuracy)) {
                auto& vul = registry.vulnerabilities.has(entity) ? registry.vulnerabilities.get(entity) : registry.vulnerabilities.emplace(entity);
                vul.modifier = checkTierThreshold(Pierce) ? 1.75 : 1.5;
                vul.countdown = 4000;
            }

            // Inflict burning or build burning stack if damage tier
            if (checkTierThreshold(BulletDamage)) {
                int chance = (getEffectValueTierThresholdDifference(BulletDamage) + 1) * 15;
                int stacks = ((chance - (chance % 100)) / 100) + ((rand() % 100) < (chance % 100));
                if (!registry.onFires.has(entity)) registry.onFires.emplace(entity);
                if (stacks > 0) registry.onFires.get(entity).stack += stacks;
            }
        }
    }

    // Handle death by no health left
    for (Entity entity : registry.enemies.entities) {
        Enemy& enemyStat = registry.enemies.get(entity);
        if (enemyStat.currHealth <= 0)
        {
            if (registry.scientist.has(entity))
            {
                Scientist& scien = registry.scientist.get(entity);
                if (!registry.deleteds.has(scien.hand))
                {
                    registry.deleteds.emplace(scien.hand);
                }
            }
            if (!registry.deleteds.has(entity))
            {
                Fade& f = registry.fades.emplace(entity);
                registry.deleteds.emplace(entity);

                if (registry.persistentSounds.has(entity))
                    registry.persistentSounds.get(entity).stop();

                ParticleProps props = enemyDeath;
                registry.emitParticles.replace(entity, PExplode, props, f.max, Random::Int(20) + 20);
                if (registry.enemyGroups.has(entity))
                {
                    for (Entity other : registry.enemyGroups.get(entity).others)
                    {
                        if (!registry.deleteds.has(other))
                            registry.deleteds.emplace(other);
                        registry.emitParticles.replace(other, PExplode, props, f.max, Random::Int(20) + 20);
                    }
                }
            }
            if (!registry.boids.has(entity) && !registry.enemyParts.has(entity) && !registry.roomWideBuffers.has(entity)) {
                sound-> playEnemyDeathSound(Random::Int(4));
            }
        }
    }

    // remove bees, create new ones
    for (Entity bee : registry.bees.entities)
    {
        if (!registry.deleteds.has(bee) && registry.bees.get(bee).merge)
        {
            createBees.push_back(vec3(registry.motions.get(bee).position.x, registry.motions.get(bee).position.y, registry.bees.get(bee).mergeCount));
            registry.deleteds.emplace(bee);
        }
    }

    for (vec3 newBee : createBees)
    {
        creatingMergeBee(newBee.z, vec2(newBee.x, newBee.y));
    }

    // Clear straggler boids
    // 0 < #boids <= 5, #non-boid == 0
    if (registry.boids.components.size() > 0 && registry.boids.components.size() <= 5 && registry.enemies.components.size() - registry.boids.components.size() == 0)
    {
        for (auto &boid : registry.boids.entities)
        {
            if (!registry.deleteds.has(boid))
                registry.deleteds.emplace(boid);
        }
    }
}

void EnemySystem::shootShotgun(vec2 velocity, vec2 pos, const AttackData &atkData)
{
    float angle = atan2(velocity.y, velocity.x);
    if (atkData.numBullets % 2 == 0)
    {
        for (uint i = 0; i < atkData.numBullets / 2; i++)
        {
            float a1 = angle + (i + 0.5) * atkData.angleOffset;
            float a2 = angle - (i + 0.5) * atkData.angleOffset;
            createEnemyBullet(render, pos, {cos(a1), sin(a1)}, atkData.veer.x * vec2(cos(a1 + atkData.veer.y), sin(a1 + atkData.veer.y)), atkData);
            createEnemyBullet(render, pos, {cos(a2), sin(a2)}, atkData.veer.x * vec2(cos(a2 - atkData.veer.y), sin(a2 - atkData.veer.y)), atkData);
        }
    }
    else
    {
        createEnemyBullet(render, pos, {cos(angle), sin(angle)}, atkData.veer.x * vec2(cos(angle), sin(angle)), atkData);
        for (uint i = 0; i < (atkData.numBullets - 1) / 2; i++)
        {
            float a1 = angle + (i + 1) * atkData.angleOffset;
            float a2 = angle - (i + 1) * atkData.angleOffset;
            createEnemyBullet(render, pos, {cos(a1), sin(a1)}, atkData.veer.x * vec2(cos(a1 + atkData.veer.y), sin(a1 + atkData.veer.y)), atkData);
            createEnemyBullet(render, pos, {cos(a2), sin(a2)}, atkData.veer.x * vec2(cos(a2 - atkData.veer.y), sin(a2 - atkData.veer.y)), atkData);
        }
    }
}

void EnemySystem::shootAllDirection(vec2 pos, float offset, const AttackData &atkData)
{
    for (uint i = 0; i < atkData.numBullets; i++)
    {
        float a = offset + atkData.angleOffset + i * (2 * M_PI / atkData.numBullets);
        createEnemyBullet(render, pos, {cos(a), sin(a)}, atkData.veer.x * vec2(cos(a + atkData.veer.y), sin(a + atkData.veer.y)), atkData);
    }
}

void EnemySystem::shootRadialPolygon(vec2 pos, const AttackData &atkData)
{
    AttackData atkData2 = atkData;
    atkData2.speed = atkData.speed * sin(M_PI / atkData.numBullets + M_PI / 2.0f);
    float offset = M_PI / atkData.numBullets;
    for (uint i = 0; i < atkData.numBullets; i++)
    {
        float a = atkData.angleOffset + i * (2.0f * M_PI / atkData.numBullets);
        createEnemyBullet(render, pos, {cos(a), sin(a)}, atkData.veer.x * vec2(cos(a + atkData.veer.y), sin(a + atkData.veer.y)), atkData);
        createEnemyBullet(render, pos, {cos(a + offset), sin(a + offset)}, atkData.veer.x * vec2(cos(a + atkData.veer.y), sin(a + atkData.veer.y)), atkData2);
    }
}

void EnemySystem::shootBurst(vec2 velocity, vec2 pos, const AttackData &atkData, float elapsed_ms, Burst &burst)
{
    int sfxNum = atkData.shape == EnemyBulletShape::CIRCLE ? 0 : atkData.shape == EnemyBulletShape::RECTANGLE ? 1
                                                                                                              : 2;
    if ((burst.curBurst <= 0) || (burst.burstCooldown -= elapsed_ms) > 0)
    {
        return;
    }

    if (atkData.attackType == EnemyAttackPattern::SPRAY)
    {
        double range = atkData.angleOffset;

        // Generate a random offset within the range
        double offset = (2 * (static_cast<double>(rand()) / RAND_MAX) - 1) * range;
        offset = burst.burstDirection + offset;
        createEnemyBullet(render, pos, {cos(offset), sin(offset)}, atkData.veer.x * vec2(cos(offset + atkData.veer.y), sin(offset + atkData.veer.y)), atkData);
    }
    else if (burst.curBurst != atkData.numBullets)
    {
        float currentAngle = atan2(velocity.y, velocity.x);
        float angleDifference = currentAngle - burst.burstDirection;

        float maxDifference = M_PI / 32;
        if (abs(angleDifference) > maxDifference)
        {
            if (angleDifference > 0)
            {
                currentAngle = burst.burstDirection + maxDifference;
            }
            else
            {
                currentAngle = burst.burstDirection - maxDifference;
            }
        }

        float angle = currentAngle;
        // std::cout << angle << std::endl;
        createEnemyBullet(render, pos, {cos(angle), sin(angle)}, atkData.veer.x * vec2(cos(angle + atkData.veer.y), sin(angle + atkData.veer.y)), atkData);
    }
    sound->playEnemyShootSound(sfxNum, 0);
    burst.curBurst--;
    burst.burstCooldown = 150;
}

void EnemySystem::createAOEIndicator(vec2 pos, float radius, SpecialStates buff, float timer)
{
    Entity entity = Entity();
    auto& aoe = registry.aoeIndicators.emplace(entity);
    aoe.type = buff;
    auto& motion = registry.motions.emplace(entity);
    motion.position = pos;
    motion.scale = vec2(radius * 1.1 , radius * 1.1);
    RenderRequest& request = registry.renderRequests.emplace(entity);
    request.texture_name = "enemy_bullet_circle.png";
    request.used_effect = TEXTURED;
    request.used_geometry = GEOMETRY_BUFFER_ID::SPRITE;
    request.show = true;
    request.offset = { 0, 0 }; // how much the position should be shifted so that center of texture = center of object
    request.idealScale = vec2(-1); //Used for tiling textures. Tiling is based on difference between idealScale and motion.scale
    Fade& fade = registry.fades.emplace(entity);
    fade.max = timer;
}

void EnemySystem::grantBuff (Entity entity, const EnemyPattern &pattern)
{
    Buffer &buffer = registry.buffers.get(entity);
    EnemyBehavior behavior = pattern.type;
    SpecialStates buff = pattern.buffEffect;
    switch ( buff )
    {
    case SpecialStates::INVINCIBLE: {
            for (Entity& e : registry.enemies.entities) {
                if (e == entity) {
                    continue;
                }
                Motion& m = registry.motions.get(e);
                if (glm::distance(m.position, registry.motions.get(entity).position) < buffer.range) {
                    if (!registry.invincibles.has(e)) {
                        Invincible &inv = registry.invincibles.emplace(e);
                        inv.countdown = buffer.duration;
                        if (registry.buffers.has(e)) {
                            inv.countdown = inv.countdown / 2;
                        } else {
                            buffer.targetEntity = e;
                        }
                    } else {
                        auto& inv = registry.invincibles.get(e);
                        float countDown = registry.buffers.has(e) ? inv.countdown / 2 : buffer.duration;
                        if (countDown > inv.countdown) {
                            inv.countdown = countDown;
                        }
                    }
                }
                if (behavior == EnemyBehavior::GRANTINGBUFFS) {
                    break;
                }
            }
            if (behavior == EnemyBehavior::GRANTINGBUFFSAOE) {
                createAOEIndicator( registry.motions.get(entity).position, buffer.range, SpecialStates::INVINCIBLE, buffer.duration);
            }
            break;
    }
    case SpecialStates::INVISIBLE: {
            for (Entity& e : registry.enemies.entities) {
                if (e == entity) {
                    continue;
                }
                Motion& m = registry.motions.get(e);
                if (glm::distance(m.position, registry.motions.get(entity).position) < buffer.range) {
                    if (!registry.invisibles.has(e)) {
                        Invisible &inv = registry.invisibles.emplace(e);
                        inv.countdown = buffer.duration;
                        if (registry.buffers.has(e)) {
                            inv.countdown = inv.countdown / 2;
                        } else {
                            buffer.targetEntity = e;
                        }
                    } else {
                        auto& inv = registry.invisibles.get(e);
                        float countDown = registry.buffers.has(e) ? inv.countdown / 2 : buffer.duration;
                        if (countDown > inv.countdown) {
                            inv.countdown = countDown;
                        }
                    }
                }
                if (behavior == EnemyBehavior::GRANTINGBUFFS) {
                    break;
                }
            }
            if (behavior == EnemyBehavior::GRANTINGBUFFSAOE) {
                createAOEIndicator( registry.motions.get(entity).position, buffer.range, SpecialStates::INVISIBLE, buffer.duration);
            }

            break;
    }
        case SpecialStates::PROTECTED: {
        for (Entity& e : registry.enemies.entities) {
            if (e == entity) {
                continue;
            }
            Motion& m = registry.motions.get(e);
            if (glm::distance(m.position, registry.motions.get(entity).position) < buffer.range) {
                if (!registry.vulnerabilities.has(e)) {
                    auto& vul = registry.vulnerabilities.emplace(e);
                    vul.modifier = checkTierThreshold(Pierce) ? 0.75 : 0.5;
                    vul.countdown = buffer.duration;
                    if (registry.buffers.has(e)) {
                        vul.countdown = vul.countdown / 2;
                    } else {
                        buffer.targetEntity = e;
                    }
                } else {
                    auto& inv = registry.vulnerabilities.get(e);
                    float countDown = registry.buffers.has(e) ? inv.countdown / 2 : buffer.duration;
                    if (countDown > inv.countdown) {
                        inv.countdown = countDown;
                    }
                    inv.modifier = checkTierThreshold(Pierce) ? 0.75 : 0.5;
                }
            }
            if (behavior == EnemyBehavior::GRANTINGBUFFS) {
                break;
            }
        }
        if (behavior == EnemyBehavior::GRANTINGBUFFSAOE) {
            createAOEIndicator( registry.motions.get(entity).position, buffer.range, SpecialStates::PROTECTED, buffer.duration);
        }

        break;
        }

    case SpecialStates::VULNERABLE: {
            for (Entity& e : registry.enemies.entities) {
                if (e == entity) {
                    continue;
                }
                Motion& m = registry.motions.get(e);
                if (glm::distance(m.position, registry.motions.get(entity).position) < buffer.range) {
                    if (!registry.vulnerabilities.has(e)) {
                        auto& vul = registry.vulnerabilities.emplace(e);
                        vul.modifier = checkTierThreshold(Pierce) ? 1.75 : 1.5;
                        vul.countdown = buffer.duration;
                        if (registry.buffers.has(e)) {
                            vul.countdown = vul.countdown / 2;
                        } else {
                            buffer.targetEntity = e;
                        }
                    } else {
                        auto& inv = registry.vulnerabilities.get(e);
                        float countDown = registry.buffers.has(e) ? inv.countdown / 2 : buffer.duration;
                        if (countDown > inv.countdown) {
                            inv.countdown = countDown;
                        }
                        inv.modifier = checkTierThreshold(Pierce) ? 1.75 : 1.5;
                    }
                }
                if (behavior == EnemyBehavior::GRANTINGBUFFS) {
                    break;
                }
            }

            if (behavior == EnemyBehavior::GRANTINGBUFFSAOE) {
                createAOEIndicator( registry.motions.get(entity).position, buffer.range, SpecialStates::VULNERABLE, buffer.duration);
            }
            break;
    }
    case SpecialStates::REGENERATING: {
        for (Entity& e : registry.enemies.entities) {
            if (e == entity) {
                continue;
            }
            Motion& m = registry.motions.get(e);
            if (glm::distance(m.position, registry.motions.get(entity).position) < buffer.range) {
                if (!registry.regenerates.has(e)) {
                    auto& regen = registry.regenerates.emplace(e);
                    if (registry.healers.has(entity)) {
                        regen.healAmount = registry.healers.get(entity).healPower;
                    } else {
                        regen.healAmount = registry.enemies.get(e).maxHealth * 0.015;
                    }
                    regen.countdown = buffer.duration;
                    if (registry.buffers.has(e)) {
                        regen.countdown = regen.countdown / 2;
                    } else {
                        buffer.targetEntity = e;
                    }
                } else {
                    auto& regen = registry.regenerates.get(e);
                    float countDown = registry.buffers.has(e) ? regen.countdown / 2 : buffer.duration;
                    if (countDown > regen.countdown) {
                        regen.countdown = countDown;
                    }
                }
            }
            if (behavior == EnemyBehavior::GRANTINGBUFFS) {
                break;
            }
        }
        if (behavior == EnemyBehavior::GRANTINGBUFFSAOE) {
            createAOEIndicator( registry.motions.get(entity).position, buffer.range, SpecialStates::REGENERATING, buffer.duration);
        }
        break;
    }
    case SpecialStates::CLOAKED: {
        for (Entity& e : registry.enemies.entities) {
            if (e == entity) {
                continue;
            }
            Motion& m = registry.motions.get(e);
            if (glm::distance(m.position, registry.motions.get(entity).position) < buffer.range) {
                if (!registry.cloaks.has(e)) {
                    auto& cloak = registry.cloaks.emplace(e);
                    cloak.countdown = buffer.duration;
                    if (registry.buffers.has(e)) {
                        cloak.countdown = cloak.countdown / 2;
                    } else {
                        buffer.targetEntity = e;
                    }
                } else {
                    auto& cloak = registry.cloaks.get(e);
                    float countDown = registry.buffers.has(e) ? cloak.countdown / 2 : buffer.duration;
                    if (countDown > cloak.countdown) {
                        cloak.countdown = countDown;
                    }
                }
            }
            if (behavior == EnemyBehavior::GRANTINGBUFFS) {
                break;
            }
        }
        if (behavior == EnemyBehavior::GRANTINGBUFFSAOE) {
            createAOEIndicator( registry.motions.get(entity).position, buffer.range, SpecialStates::CLOAKED, buffer.duration);
        }
        break;

    }

        
        default:
            break;
    }

}

void EnemySystem::shootWave(vec2 pos, const AttackData &atkData, float elapsed_ms, Burst &burst)
{
    int sfxNum = atkData.shape == EnemyBulletShape::CIRCLE ? 0 : atkData.shape == EnemyBulletShape::RECTANGLE ? 1
                                                                                                              : 2;
    // std::cout << burst.curBurst << std::endl;
    if ((burst.curBurst <= 0) || (burst.burstCooldown -= elapsed_ms) > 0)
    {
        return;
    }
    vec2 velocity = vec2(cos(burst.burstDirection), sin(burst.burstDirection));
    if (atkData.numBullets == burst.curBurst)
    {
        sound->playEnemyShootSound(sfxNum, 0);
        createEnemyBullet(render, burst.start, velocity, atkData.veer.x * vec2(cos(atkData.veer.y), sin(atkData.veer.y)), atkData);
    }
    else
    {
        vec2 perp = vec2(-velocity.y, velocity.x) * 30.f * (float)(atkData.numBullets - burst.curBurst);
        createEnemyBullet(render, burst.start + perp, velocity, atkData.veer.x * vec2(cos(atkData.veer.y), sin(atkData.veer.y)), atkData);
        createEnemyBullet(render, burst.start - perp, velocity, atkData.veer.x * vec2(cos(atkData.veer.y), sin(atkData.veer.y)), atkData);
    }
    burst.curBurst--;
    burst.burstCooldown = 200;
}

void EnemySystem::shootOneWall(const AttackData &atkData, float angle, float elapsed_ms)
{
    Map& map = registry.maps.components[0];
    vec2 roomStartPos = map.currRoom.roomStart;
    vec2 roomEndPos = map.currRoom.roomEnd;
    float roomDiagonal = glm::distance(roomStartPos, roomEndPos);

    vec2 velocity = vec2(cos(angle), sin(angle));
    vec2 perp = vec2(-velocity.y, velocity.x);
    vec2 startPos = (roomEndPos + ((roomStartPos - roomEndPos) / 2.f)) - velocity * roomDiagonal / 1.9f;
    int numBullets = floor(roomDiagonal / (atkData.size.x * 3.f));
    float segment = roomDiagonal / (2.f * numBullets);
    vec2 veer = atkData.veer.x * vec2(cos(atkData.veer.y), sin(atkData.veer.y));

    bool hole = (atkData.numBullets % 2 != 0);
    int holeSize = 2 * numBullets / (2 * atkData.numBullets + 1);
    int holeTracker = holeSize / 2;

    for (int i = 0; i < numBullets; i++) {
        if (hole) {
            holeTracker++;
            hole = (holeTracker != holeSize);
        }
        else {
            createEnemyBullet(render, startPos + perp * (segment * (i + 0.5f)), velocity, veer, atkData);
            createEnemyBullet(render, startPos - perp * (segment * (i + 0.5f)), velocity, veer, atkData);
            holeTracker -= (atkData.numBullets > 0) ? 1 : 0;
            hole = (holeTracker == 0);
        }
    }
}

void EnemySystem::shootTwoWall(const AttackData &atkData, float angle, float elapsed_ms)
{
    Map& map = registry.maps.components[0];
    vec2 roomStartPos = map.currRoom.roomStart;
    vec2 roomEndPos = map.currRoom.roomEnd;
    float roomDiagonal = glm::distance(roomStartPos, roomEndPos);

    vec2 velocity = vec2(cos(angle), sin(angle));
    vec2 perp = vec2(-velocity.y, velocity.x);
    vec2 startPosA = (roomEndPos + ((roomStartPos - roomEndPos) / 2.f)) - velocity * roomDiagonal / 1.9f;
    vec2 startPosB = (roomEndPos + ((roomStartPos - roomEndPos) / 2.f)) - perp * roomDiagonal / 1.9f;
    int numBullets = floor(roomDiagonal / (atkData.size.x * 3.f));
    float segment = roomDiagonal / (2.f * numBullets);\
    vec2 veer = atkData.veer.x * vec2(cos(atkData.veer.y), sin(atkData.veer.y));

    bool hole = (atkData.numBullets % 2 != 0);
    int holeSize = 2 * numBullets / (2 * atkData.numBullets + 1);
    int holeTracker = holeSize / 2;

    for (int i = 0; i < numBullets; i++) {
        if (hole) {
            holeTracker++;
            hole = (holeTracker != holeSize);
        }
        else 
        {
            createEnemyBullet(render, startPosA + perp * (segment * (i + 0.5f)), velocity, veer, atkData);
            createEnemyBullet(render, startPosA - perp * (segment * (i + 0.5f)), velocity, veer, atkData);

            createEnemyBullet(render, startPosB + velocity * (segment * (i + 0.5f)), perp, veer, atkData);
            createEnemyBullet(render, startPosB - velocity * (segment * (i + 0.5f)), perp, veer, atkData);
            holeTracker -= (atkData.numBullets > 0) ? 1 : 0;
            hole = (holeTracker == 0);
        }
    }
}

void EnemySystem::shootRadialBurst(vec2 pos, const AttackData &atkData, float elapsed_ms, Burst &burst)
{
    // std::cout << burst.curBurst << std::endl;
    if ((burst.curBurst <= 0) || (burst.burstCooldown -= elapsed_ms) > 0)
    {
        return;
    }
    for (uint i = 0; i < atkData.veer.x; i++)
    {
        float a = atkData.angleOffset * (float)(atkData.numBullets - burst.curBurst) + i * (2 * M_PI / atkData.veer.x);
        createEnemyBullet(render, pos, {cos(a), sin(a)}, {0, 0}, atkData);
    }
    burst.curBurst--;
    burst.burstCooldown = atkData.veer.y;
}

void EnemySystem::shootLaser(vec2 pos, Entity enemy, const AttackData &atkData, bool shouldPlayFiringSound)
{
    for (uint i = 0; i < atkData.numBullets; i++)
    {
        float a = atkData.angleOffset + i * (2 * M_PI / atkData.numBullets);
        if (registry.enemies.get(enemy).rotationBehaviour == EnemyRotationBehavior::FACE_PLAYER) {
            a = registry.motions.get(enemy).angle + i * (2 * M_PI / atkData.numBullets);
        }
        createEnemyLaser(render, pos, a, enemy, atkData);
    }

    if (atkData.veer.x >= 400) {
        SoundRequest& laserSound = registry.soundRequests.emplace(Entity());
        laserSound.type = SoundType::LaserSound;
        laserSound.delay = 1000.f;
        laserSound.ticks = atkData.bulletRange;
        laserSound.songIndex = shouldPlayFiringSound;
        laserSound.sourceEntity = enemy;
    } else {
        sound->playLaserSound(atkData.bulletRange, shouldPlayFiringSound, enemy);
    }
}
void EnemySystem::shootTwinLaser(vec2 pos, Entity enemy, const AttackData &atkData)
{
    // shoot towards the twin
    Entity other = registry.enemyGroups.get(enemy).others[0];
    Motion &otherMotion = registry.motions.get(other);
    Motion &motion = registry.motions.get(enemy);
    vec2 diff = otherMotion.position - motion.position;
    float angle = atan2(diff.y, diff.x);
    Entity e = createEnemyLaser(render, pos, angle, enemy, atkData);
}

void EnemySystem::launchBombard(const AttackData& atkData) {
    Map& map = registry.maps.components[0];
    vec2 min = map.currRoom.roomStart;
    vec2 max = map.currRoom.roomEnd;
    float roomLength = glm::length(min - max);
    vec2 center = (max + min) / 2.f;
    vec2 line = (glm::normalize(atkData.veer) * roomLength / 2.2f);
    vec2 perp = glm::normalize(vec2(atkData.veer.y, -atkData.veer.x));
    for (uint i = 1; i < atkData.numBullets + 1; i++) {
        float ratio = (float)i / (float)atkData.numBullets;
        vec2 pos = (ratio * line) + ((1.f - ratio) * -line);
        pos += center + perp * (float)(200 - rand() % 400);
        if (pos.x < min.x || pos.y < min.y || pos.x > max.x || pos.y > max.y) continue;
        createBombard(render, i * 300, pos, atkData.attackType);
    }
}

void EnemySystem::attack(Entity entity, EnemyPattern &currPattern, Motion playerMotion, vec2 pos, const AttackData &atkData, float elapsed_ms)
{
    int sfxNum = atkData.shape == EnemyBulletShape::CIRCLE ? 0 : atkData.shape == EnemyBulletShape::RECTANGLE ? 1 : 2;

    Enemy &enemy = registry.enemies.get(entity);
    Motion &em = registry.motions.get(entity);
    vec2 velocity = (playerMotion.position + playerMotion.velocity / 2.0f) - pos;

    if (atkData.attackType == EnemyAttackPattern::SHOTGUN)
    {
        sound->playEnemyShootSound(sfxNum, 0);
        shootShotgun(velocity, pos, atkData);
        currPattern.currAtkCD = currPattern.maxAtkCD;
    }
    else if (atkData.attackType == EnemyAttackPattern::RADIAL)
    {
        // std::cout << em.angle << std::endl;
        sound->playEnemyShootSound(sfxNum, 0);
        shootAllDirection(pos, em.angle, atkData);
        currPattern.currAtkCD = currPattern.maxAtkCD;
    }
    else if (atkData.attackType == EnemyAttackPattern::RADIAL_POLYGON)
    {
        // std::cout << em.angle << std::endl;
        sound->playEnemyShootSound(sfxNum, 0);
        shootRadialPolygon(pos, atkData);
        currPattern.currAtkCD = currPattern.maxAtkCD;
    }
    else if (atkData.attackType == EnemyAttackPattern::LASER)
    {
        if (registry.persistentSounds.has(entity)) {
            bool shouldPlayFiringSound = registry.persistentSounds.get(entity).channels.at(SoundType::LaserSound).x != -1;
            shootLaser( pos, entity, atkData, shouldPlayFiringSound);
        } else if (currPattern.maxAtkCD <= 750.f || atkData.veer.x < 5) {
            shootLaser(pos, entity, atkData, false);
        }
        else {
            shootLaser(pos, entity, atkData, true);
        }
        currPattern.currAtkCD = currPattern.maxAtkCD;
    }
    else if (atkData.attackType == EnemyAttackPattern::TWIN_LASER)
    {
        shootTwinLaser(pos, entity, atkData);
        currPattern.currAtkCD = currPattern.maxAtkCD;
    }
    else if (atkData.attackType == EnemyAttackPattern::TRAIL)
    {
        // Sound is kinda annoying yeah, and its not really "shooting? I guess?
        // sound->playEnemyShootSound(sfxNum, atkData.numBullets);
        float a = (float)(rand()) / (float)(RAND_MAX);
        shootShotgun(vec2(cos(a), sin(a)), pos, atkData);
        currPattern.currAtkCD = 600;
    }
    else if (atkData.attackType == EnemyAttackPattern::BURST || atkData.attackType == EnemyAttackPattern::SPRAY)
    {
        Burst &burst = registry.bursts.get(entity);
        shootBurst(velocity, pos, atkData, elapsed_ms, burst);
        burst.burstDirection = atan2(velocity.y, velocity.x);
        if (burst.curBurst <= 0)
        {
            currPattern.currAtkCD = currPattern.maxAtkCD;
            burst.curBurst = atkData.numBullets;
            burst.burstCooldown = 0;
        }
    }
    else if (atkData.attackType == EnemyAttackPattern::WAVE)
    {
        Burst &burst = registry.bursts.get(entity);
        if (burst.curBurst == atkData.numBullets)
        {
            burst.burstDirection = atan2(velocity.y, velocity.x);
            burst.start = pos;
        }
        shootWave(pos, atkData, elapsed_ms, burst);
        if (burst.curBurst <= 0)
        {
            currPattern.currAtkCD = currPattern.maxAtkCD;
            burst.curBurst = atkData.numBullets;
            burst.burstCooldown = 0;
        }
    }
    else if (atkData.attackType == EnemyAttackPattern::BURST_RADIAL)
    {
        Burst &burst = registry.bursts.get(entity);
        shootRadialBurst(pos, atkData, elapsed_ms, burst);
        burst.burstDirection = atan2(velocity.y, velocity.x);
        if (burst.curBurst <= 0)
        {
            currPattern.currAtkCD = currPattern.maxAtkCD;
            burst.curBurst = atkData.numBullets;
            burst.burstCooldown = 0;
        }
    } else if (atkData.attackType == EnemyAttackPattern::ONE_WALL)
    {
        shootOneWall(atkData, atkData.angleOffset, elapsed_ms);
        currPattern.currAtkCD = currPattern.maxAtkCD;
    } else if (atkData.attackType == EnemyAttackPattern::TWO_WALL)
    {
        shootTwoWall(atkData, atkData.angleOffset, elapsed_ms);
        currPattern.currAtkCD = currPattern.maxAtkCD;
    }
    else if (atkData.attackType == EnemyAttackPattern::BOMBARD || atkData.attackType == EnemyAttackPattern::BOMBARDBOMBING)
    {
        launchBombard(atkData);
        currPattern.currAtkCD = currPattern.maxAtkCD;
    }
}

void EnemySystem::spawn(Entity entity, EnemyPattern &currPattern, vec2 pos, AttackData atkData)
{
    if (registry.enemies.components.size() > MAX_ENEMY_SPAWN)
        return;

    if (registry.animations.has(entity) && registry.animations.get(entity).animation_countdown_base != -5) registry.animations.get(entity).frame = 1;
    Map &map = registry.maps.components[0];
    // vec2 roomStartPos = map.currRoom.roomStart;
    // vec2 roomEndPos = map.currRoom.roomEnd;

    for (uint i = 0; i < atkData.numBullets; i++)
    {
        if (atkData.spawn == EnemyType::ScientistShield && registry.scientist.has(entity))
        {
            if (registry.shield.entities.size() > 0) {
                break;
            }
            Entity shield = createEnemy(render, pos, atkData.spawn);
            Scientist &scientist = registry.scientist.get(entity);
            scientist.shield = shield;
            continue;
        }
        else if (atkData.spawn == EnemyType::ScientistHand && registry.scientist.has(entity))
        {
            Entity hand = createEnemy(render, pos, atkData.spawn);
            Scientist &scientist = registry.scientist.get(entity);
            scientist.hand = hand;
            continue;
        } 

        if (atkData.spawnPosition.size() == atkData.numBullets)
        {
            std::tuple<EnemyType, vec2> spawn = std::make_tuple(  atkData.spawn,atkData.spawnPosition[i]);
            map.currRoom.enemiesToSpawn.push_back(spawn);
        }
        else
        {
            createEnemy(render, pos, atkData.spawn);
        }
    }

    currPattern.currAtkCD = currPattern.maxAtkCD;
}

void EnemySystem::creatingMergeBee(int count, vec2 pos)
{
    switch (count)
    {
    case 2:
        // std::cout << "CREATING" << std::endl;
        createEnemy(render, pos, EnemyType::EnemyTwoBee);
        break;
    case 3:
        createEnemy(render, pos, EnemyType::EnemyThreeBee);
        break;
    default:
        // std::cout << "CREATING 1" << std::endl;
        createEnemy(render, pos, EnemyType::EnemyOneBee);
    }
}

void EnemySystem::merge(Entity entity, EnemyPattern &currPattern, std::vector<Entity> &pendingDeletion)
{
    // std::lock_guard<std::mutex> lock(beeMutex);
    if (registry.bees.has(entity) && currPattern.type == EnemyBehavior::MERGE_BEE)
    {
        BeeEnemy &bee = registry.bees.get(entity);
        // getting the first bee
        // std::cout << "WANT MERGE!" << std::endl;
        // std::vector<Entity> deletedBees;
        for (Entity otherBeeEntity : registry.bees.get(entity).nearbyBees)
        {
            if (bee.mergeCount >= bee.maxMerge)
            {
                break;
            }
            if (!registry.deleteds.has(otherBeeEntity) && !registry.bees.get(otherBeeEntity).merge /* && otherBeeEntity != NULL*/)
            {
                BeeEnemy &otherBee = registry.bees.get(otherBeeEntity);
                // int mergeTotal = otherBee.mergeCount + bee.mergeCount;
                // Motion &motion = registry.motions.get(entity);
                // if(otherBee.merge == false && bee.merge == false)
                //{
                // creatingMergeBee(mergeTotal, motion.position);
                //}
                bee.mergeCount += otherBee.mergeCount;
                registry.deleteds.emplace(otherBeeEntity);
                // registry.bees.remove(otherBeeEntity);
                //  std::cout << "MERGED AND CREATED COMBINED BEES" << std::endl;
                // otherBee.merge = true;
                bee.merge = true;
                // bee.nearbyBees.clear();
                // otherBee.nearbyBees.clear();
                /*deletedBees.push_back(entity);
                deletedBees.push_back(otherBeeEntity);*/
                // std::cout << "NOW DELETE EXISTING BEE" << std::endl;
                // break;
            }
        }
        bee.nearbyBees.clear();
    }
}

void EnemySystem::heal(Entity entity, EnemyPattern &currPattern)
{
    Healer& healer = registry.healers.get(entity);
    if (healer.targetEntity && currPattern.type == EnemyBehavior::HEALING)
    {
        Entity otherEntity = healer.targetEntity;
        if (healer.coolDown < 0.f)
        {
            Enemy &otherEnemy = registry.enemies.get(otherEntity);
            Enemy &healerEnemy = registry.enemies.get(entity);

            otherEnemy.currHealth = glm::min(otherEnemy.maxHealth, otherEnemy.currHealth + healer.healPower);

            healer.coolDown = healer.maxCoolDown;
        }
    }
}

void EnemySystem::destruct(Enemy &enemy)
{
    enemy.currHealth = -1;
}

void EnemySystem::fetchRoomEffects(Entity entity, AttackData& atkData)
{
    // TEMP FIX: return immediately if game is resetting
    // TODO: the following problem then seems to arise: bullets getting empty effects b/c returned nothing!
    if (registry.ioStates.components[0].shouldRestart || registry.gameStates.components[0].resetRoom)
        return;
    
    atkData.gottenRoomEffects = true;
    Map& map = registry.maps.components[0];

    // Add checks here to exclude certain enemies from adopting room effects
    if (registry.bosses.has(entity) || registry.bossParts.has(entity) 
        || registry.elites.has(entity) || map.currRoom.type == Testing || map.currRoom.type == TutorialRoom2) {
        atkData.positiveBulletEffects = atkData.rareBulletEffects;
        atkData.negativeBulletEffects.push_back(atkData.defaultEffect);
        return;
    }

    assert(!map.currRoom.preset.positiveEffects.empty());
    assert(!map.currRoom.preset.negativeEffects.empty());

    atkData.positiveBulletEffects = map.currRoom.preset.positiveEffects[rand() % map.currRoom.preset.positiveEffects.size()];
    atkData.negativeBulletEffects = map.currRoom.preset.negativeEffects[rand() % map.currRoom.preset.negativeEffects.size()];
}
