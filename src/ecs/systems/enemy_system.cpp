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
#include <chrono>

#include "ai_system.hpp"
#include <mutex>

std::mutex beeMutex;

float COOLDOWN_SHOOT_MS = 2000;
float BASE_BULLET_SPEED = 1;
float PLACEHOLDER_FOR_ANGLE = 0.f;

EnemySystem::EnemySystem(RenderSystem *renderer)
{
    render = renderer;
};

EnemySystem::~EnemySystem() {
};

void EnemySystem::step(float elapsed_ms)
{

    Entity player = registry.players.entities[0];
    Motion &playerMotion = registry.motions.get(player);
    // std::cout << "current enemy :" << registry.enemies.entities.size() << std::endl;
    // std::cout << "current bee enemy: " << registry.bees.entities.size() << std::endl;
    std::vector<Entity> pendingDeletion;
    // handle enemy moving & shooting
    for (Entity entity : registry.enemies.entities)
    {
        if (registry.fades.has(entity))
            continue;
        if (!registry.enemies.has(entity))
        {
            continue;
        }
        Enemy &enemy = registry.enemies.get(entity);
        Motion &motion = registry.motions.get(entity);
        vec2 pos = motion.position;
        float angle = motion.angle;

        EnemyPattern &pattern = enemy.currEnemyPattern();
        if (registry.bees.has(entity) && pattern.type == EnemyBehavior::MERGE_BEE && registry.bees.get(entity).nearbyBees.size() > 0)
        {
            std::cout << "MERGING WITH BEE SIZE:" << registry.bees.get(entity).nearbyBees.size() << std::endl;
             merge(entity, pattern, pendingDeletion);
        }

        for (Entity deletedBee : pendingDeletion)
        {
            std::cout << pendingDeletion.size() << "to be delted" << std::endl;
            if (!registry.deleteds.has(deletedBee))
                registry.deleteds.emplace(deletedBee);
            registry.bees.remove(deletedBee);
            registry.enemies.remove(deletedBee);
        }

        // move enemy using lerp
        if (registry.enemyMovement.has(entity))
        {
            EnemyMovement &movement = registry.enemyMovement.get(entity);
            if (pattern.type == EnemyBehavior::ROTATE_IN_PLACE)
            {
                float angularSpeed = movement.angularSpeed * 2 * M_PI / 360.0f;
                float rotationChange = angularSpeed * elapsed_ms / 1000.f;
                motion.angle += rotationChange;
            }
            vec2 direction = movement.posB - movement.posA;
            if (direction != vec2(0, 0))
            {
                float targetAngle = atan2(direction.y, direction.x);
                float deltaAngle = targetAngle - motion.angle;
                float angularSpeedRad = movement.angularSpeed * 2 * M_PI / 360.0f;
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

                float totalDistance = glm::distance(movement.posA, movement.posB);
                movement.distanceTraveled = glm::min(movement.distanceTraveled + movement.speed * elapsed_ms / 1000.f, glm::distance(movement.posA, movement.posB));
                motion.position = glm::lerp(movement.posA, movement.posB, movement.distanceTraveled / totalDistance);
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
                AttackData atkData = pattern.atkData;
                attack(entity, pattern, playerMotion, pos, atkData, elapsed_ms);
            }
        }
    }

    // HANDLING DAMGE FROM COLLISION
    for (auto &entity : registry.collisions.entities)
    {
        const Collision &collision = registry.collisions.get(entity);
        Entity other_entity = collision.other;

        if (registry.enemies.has(entity) && registry.playerBullets.has(other_entity))
        {
            Enemy &enemyStat = registry.enemies.get(entity);
            PlayerBullet &bulletStat = registry.playerBullets.get(other_entity);

            enemyStat.currHealth -= bulletStat.damage;
            if (enemyStat.currHealth <= 0)
            {
                if (!registry.deleteds.has(entity))
                {
                    Fade &f = registry.fades.emplace(entity);
                    registry.deleteds.emplace(entity);
                    if (!registry.emitParticles.has(entity))
                        registry.emitParticles.emplace(entity, ParticleRequestType::EnemyDeath, f.max, rand() % 10 + 10);
                }

                // std::cout << "enemy " << entity << "has died" << std::endl;
            }

            if (!registry.deleteds.has(other_entity))
                registry.deleteds.emplace(other_entity);
            if (!registry.damageds.has(entity) && enemyStat.currHealth > 0)
            {
                registry.damageds.emplace(entity);
            }
            else if (registry.damageds.has(entity))
            {
                registry.damageds.get(entity).countdown = registry.damageds.get(entity).max;
            }
        }
    }
}

void EnemySystem::shootShotgun(vec2 velocity, vec2 pos, AttackData atkData)
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

void EnemySystem::shootAllDirection(vec2 pos, AttackData atkData)
{
    for (uint i = 0; i < atkData.numBullets; i++)
    {
        float a = atkData.angleOffset + i * (2 * M_PI / atkData.numBullets);
        createEnemyBullet(render, pos, {cos(a), sin(a)}, atkData.veer.x * vec2(cos(a + atkData.veer.y), sin(a + atkData.veer.y)), atkData);
    }
}

void EnemySystem::shootRadialPolygon(vec2 pos, AttackData atkData)
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

void EnemySystem::shootBurst(vec2 velocity, vec2 pos, AttackData atkData, float elapsed_ms, Burst &burst)
{
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
    burst.curBurst--;
    burst.burstCooldown = 150;
}

void EnemySystem::shootWave(vec2 pos, AttackData atkData, float elapsed_ms, Burst &burst)
{
    // std::cout << burst.curBurst << std::endl;
    if ((burst.curBurst <= 0) || (burst.burstCooldown -= elapsed_ms) > 0)
    {
        return;
    }
    vec2 velocity = vec2(cos(burst.burstDirection), sin(burst.burstDirection));
    if (atkData.numBullets == burst.curBurst)
    {
        createEnemyBullet(render, pos, velocity, atkData.veer.x * vec2(cos(atkData.veer.y), sin(atkData.veer.y)), atkData);
    }
    else
    {
        vec2 perp = vec2(-velocity.y, velocity.x) * 30.f * (float)(atkData.numBullets - burst.curBurst);
        createEnemyBullet(render, pos + perp, velocity, atkData.veer.x * vec2(cos(atkData.veer.y), sin(atkData.veer.y)), atkData);
        createEnemyBullet(render, pos - perp, velocity, atkData.veer.x * vec2(cos(atkData.veer.y), sin(atkData.veer.y)), atkData);
    }
    burst.curBurst--;
    burst.burstCooldown = 200;
}

void EnemySystem::shootRadialBurst(vec2 pos, AttackData atkData, float elapsed_ms, Burst &burst)
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

void EnemySystem::shootLaser(vec2 pos, Entity enemy, AttackData atkData)
{
    for (uint i = 0; i < atkData.numBullets; i++)
    {
        float a = atkData.angleOffset + i * (2 * M_PI / atkData.numBullets);
        createEnemyLaser(render, pos, a, enemy, atkData);
    }
}

void EnemySystem::attack(Entity entity, EnemyPattern &currPattern, Motion playerMotion, vec2 pos, AttackData atkData, float elapsed_ms)
{
    Enemy &enemy = registry.enemies.get(entity);
    vec2 velocity = (playerMotion.position + playerMotion.velocity / 2.0f) - pos;
    if (atkData.attackType == EnemyAttackPattern::SHOTGUN)
    {
        shootShotgun(velocity, pos, atkData);
        currPattern.currAtkCD = currPattern.maxAtkCD;
    }
    else if (atkData.attackType == EnemyAttackPattern::RADIAL)
    {
        shootAllDirection(pos, atkData);
        currPattern.currAtkCD = currPattern.maxAtkCD;
    }
    else if (atkData.attackType == EnemyAttackPattern::LASER)
    {
        shootLaser(pos, entity, atkData);
        currPattern.currAtkCD = currPattern.maxAtkCD;
    }
    else if (atkData.attackType == EnemyAttackPattern::TRAIL)
    {
        shootShotgun(velocity, pos, atkData);
        currPattern.currAtkCD = 400;
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
    }
}

void EnemySystem::creatingMergeBee(int count, vec2 pos)
{
    switch (count)
    {
    case 2:
        std::cout << "CREATING" << std::endl;
        createEnemy(render, pos, EnemyType::TwoBee);
        break;
    case 3:
        createEnemy(render, pos, EnemyType::ThreeBee);
        break;
    default:
        std::cout << "CREATING 1" << std::endl;
        createEnemy(render, pos, EnemyType::TwoBee);
    }
}

void EnemySystem::merge(Entity entity, EnemyPattern &currPattern, std::vector<Entity> &pendingDeletion)
{
    std::lock_guard<std::mutex> lock(beeMutex);
    if (registry.bees.has(entity) && currPattern.type == EnemyBehavior::MERGE_BEE)
    {
        BeeEnemy &bee = registry.bees.get(entity);
        // getting the first bee
        // std::cout << "WANT MERGE!" << std::endl;
        std::vector<Entity> deletedBees;
        for (Entity otherBeeEntity : registry.bees.get(entity).nearbyBees)
        {
            if (!registry.deleteds.has(otherBeeEntity) && otherBeeEntity != NULL)
            {

                BeeEnemy &otherBee = registry.bees.get(otherBeeEntity);
                int mergeTotal = otherBee.mergeCount + bee.mergeCount;
                Motion &motion = registry.motions.get(entity);
                if(otherBee.merge == false && bee.merge == false)
                {
                    creatingMergeBee(mergeTotal, motion.position);
                }

                // std::cout << "MERGED AND CREATED COMBINED BEES" << std::endl;
                otherBee.merge = true;
                bee.merge = true;
                deletedBees.push_back(entity);
                deletedBees.push_back(otherBeeEntity);
                // std::cout << "NOW DELETE EXISTING BEE" << std::endl;
                break;
            }
        }

        for (Entity deletedBee : deletedBees)
        {
            for (Entity nearby : registry.bees.get(deletedBee).nearbyBees)
            {
                if (registry.bees.has(nearby))
                {
                    auto &nearbyBee = registry.bees.get(nearby);
                    nearbyBee.nearbyBees.erase(deletedBee);
                }
            }
            pendingDeletion.push_back(deletedBee);
        }
        // 		std::cout << " time to merge bees with other bees: " << registry.bees.get(bee).nearbyBees.size() << std::endl;
        // 		registry.bees.get(bee).mergeCount += registry.bees.get(bee).nearbyBees.size();
        // 		registry.bees.get(bee).nearbyBees.clear();
        // 		if (registry.bees.get(bee).mergeCount >= 1) {
        // 			registry.renderRequests.get(bee).texture_name = "bee_fly_2";
        // 		}
    }
}
