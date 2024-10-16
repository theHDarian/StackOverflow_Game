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

float COOLDOWN_SHOOT_MS = 5000;
float BASE_BULLET_SPEED = 1;
float PLACEHOLDER_FOR_ANGLE = 0.f;

EnemySystem::EnemySystem(RenderSystem *renderer)
{
    render = renderer;
};

EnemySystem::~EnemySystem() {
};

void EnemySystem::step(float elapsed_ms) {

    Entity player = registry.players.entities[0];
    Motion& playerMotion = registry.motions.get(player);

    // HANDLING DAMGE FROM COLLISION
    for (auto& entity : registry.collisions.entities)
    {
        if (!registry.collisions.has(entity)) // if this isn't included, it will cause a get assertion error
            continue;
        const Collision& collision = registry.collisions.get(entity);
        Entity other_entity = collision.other;

        if (registry.enemies.has(entity) && registry.playerBullets.has(other_entity))
        {
            Enemy& enemyStat = registry.enemies.get(entity);
            PlayerBullet& bulletStat = registry.playerBullets.get(other_entity);

            enemyStat.currHealth -= bulletStat.damage;
            //std::cout << "current enemy health" << enemyStat.currHealth << std::endl;
            if (enemyStat.currHealth <= 0)
            {
                if (!registry.fades.has(entity)) {
                    for (int i = 0; i < (rand() % 50 + 10); i++) {
                        EmitParticle& p = registry.emitParticles.emplace(Entity());
                        Motion& motion = registry.motions.get(entity);
                        p.requestType = RequestType::Explosion;
                        p.requestOrigin = motion.position + vec2{ 0, rand() % (int)(motion.scale.y * 0.8) - 0 };
                        p.position = motion.position + vec2{ rand() % (int)(motion.scale.x * 0.8) - 0, rand() % (int)(motion.scale.y * 0.8) - 0 };
                    }
                    registry.fades.emplace(entity);
                }
                if ((registry.fades.get(entity).time <= 0) && (!registry.deleteds.has(entity))) {
                    registry.deleteds.emplace(entity);
                }
                if (!registry.deleteds.has(entity)) {
                    registry.deleteds.emplace(entity);
                }

                //std::cout << "enemy " << entity << "has died" << std::endl;
            }

            if (!registry.deleteds.has(other_entity))
                registry.deleteds.emplace(other_entity);
        }
    }

    //for (Entity& entity : registry.renderRequests.entities)
    //{
    //    if (registry.renderRequests.get(entity).used_effect > EFFECT_COUNT)
    //        std::cout << "way to big of an effect b4 step!" << registry.renderRequests.get(entity).used_effect << std::endl;
    //    if (registry.renderRequests.get(entity).used_geometry > GEOMETRY_COUNT)
    //        std::cout << "way to big of a geometryin b4 step" << registry.renderRequests.get(entity).used_geometry << std::endl;
    //    if (registry.renderRequests.get(entity).used_texture > TEXTURE_COUNT)
    //        std::cout << "way to big of a texture in b4 step!" << registry.renderRequests.get(entity).used_texture << std::endl;
    //}
    
    //int i = 0;
    //for (int i = registry.enemies.size() - 1; i >= 0; i--) {
    for (Entity entity : registry.enemies.entities) {
        //i++;
        //for (Entity& entity : registry.renderRequests.entities)
        //{
        //    if (registry.renderRequests.get(entity).used_effect > EFFECT_COUNT)
        //        std::cout << "way to big of an effect in loop! i = " << i << " " << registry.renderRequests.get(entity).used_effect << std::endl;
        //    if (registry.renderRequests.get(entity).used_geometry > GEOMETRY_COUNT)
        //        std::cout << "way to big of a geometryin in loop i = " << i << " " << registry.renderRequests.get(entity).used_geometry << std::endl;
        //    if (registry.renderRequests.get(entity).used_texture > TEXTURE_COUNT)
        //        std::cout << "way to big of a texture in in loop! i = " << i << " " << registry.renderRequests.get(entity).used_texture << std::endl;
        //}
        //std::cout << " size of array " << registry.enemies.size() << std::endl;
        //std::cout << " size of i " << i << std::endl;
        //int i = registry.deleteds.size() - 1; i >= 0; i--
        Enemy& enemy = registry.enemies.get(entity);
        //Entity entity = e;

        Motion& motion = registry.motions.get(entity);
        AttackData& atkData = registry.attackDatas.get(entity);
        vec2 pos = motion.position;
        float angle = motion.angle;

        /*
      * Sky's AI logic: firing cooldown is handled by the shoot function, so no need to worry about that here
      * The number of bursts and the interval between bursts is stored in the PlayerAttackData component
      * change them per enemy type in createEnemy function in world_init.cpp
      * pass render and elapsed_ms directly
      * instead of an angle, pass a normalized vector. in the example, it's a vector pointing towards the player
      * after that, pass the number of shots in a spread (think shotgun) and the spread angle (angle from the leftmost to the rightmost bullet)
      * !! careful with the enemy death logic, this may crash if the enemy is removed before the function runs.
      * the check for shoots component is there to prevent that, still best to put it before the damage handling
      */

        //if (registry.shoots.has(entity)) {
        //    vec2 playerPos = registry.motions.get(registry.players.entities[0]).position;
        //    vec2 playerDir = playerPos - pos;
        //    playerDir = glm::normalize(playerDir);
        //    shoot(entity, pos, playerDir, elapsed_ms, 3, 30.f);
        //}

        for (Entity& entity : registry.renderRequests.entities)
        {
            if (registry.renderRequests.get(entity).used_effect > EFFECT_COUNT)
                std::cout << "way to big of an effect after collusuib!" << registry.renderRequests.get(entity).used_effect << std::endl;
            if (registry.renderRequests.get(entity).used_geometry > GEOMETRY_COUNT)
                std::cout << "way to big of a geometryin after collusuib" << registry.renderRequests.get(entity).used_geometry << std::endl;
            if (registry.renderRequests.get(entity).used_texture > TEXTURE_COUNT)
                std::cout << "way to big of a texture in after collusuib!" << registry.renderRequests.get(entity).used_texture << std::endl;
        }

        // move enemy using lerp
        if (registry.enemyMovement.has(entity)) {
            EnemyMovement& movement = registry.enemyMovement.get(entity);
            vec2 direction = movement.posB - movement.posA;
            if (direction != vec2(0, 0)) {
                float targetAngle = atan2(direction.y, direction.x);
                float deltaAngle = targetAngle - motion.angle;
                float angularSpeedRad = movement.angularSpeed * 2 * M_PI / 360.0f;
                float maxChange = angularSpeedRad * elapsed_ms / 1000.0f;
                if (deltaAngle > M_PI) deltaAngle -= 2 * M_PI;
                if (deltaAngle < -M_PI) deltaAngle += 2 * M_PI;
                if (deltaAngle > maxChange) deltaAngle = maxChange;
                if (deltaAngle < -maxChange) deltaAngle = -maxChange;

                motion.angle += deltaAngle;

                float totalDistance = glm::distance(movement.posA, movement.posB);
                movement.distanceTraveled = glm::min(movement.distanceTraveled + movement.speed * elapsed_ms / 1000.f, glm::distance(movement.posA, movement.posB));
                motion.position = glm::lerp(movement.posA, movement.posB, movement.distanceTraveled / totalDistance);
            }

        }

        enemy.attackCooldown -= elapsed_ms;
        // std::cout << "enemy attack in:" << enemy.attackCooldown << std::endl;
        if (enemy.attackCooldown < 0.f)
        {
            if (atkData.attackType == EnemyAttackPattern::SHOTGUN)
            {
                shootShotgun(playerMotion.position - pos, pos, atkData);
                enemy.attackCooldown = COOLDOWN_SHOOT_MS;
            }
            else if (atkData.attackType == EnemyAttackPattern::ALL_DIRECTION)
            {
                shootAllDirection(pos, atkData);
                enemy.attackCooldown = COOLDOWN_SHOOT_MS;
            }
            else if (atkData.attackType == EnemyAttackPattern::BURST || atkData.attackType == EnemyAttackPattern::SPRAY)
            {
                Burst& burst = registry.bursts.get(entity);
                vec2 velocity = playerMotion.position - pos;
                shootBurst(playerMotion.position - pos, pos, atkData, elapsed_ms, burst, enemy);
                burst.burstDirection = atan2(velocity.y, velocity.x);
                if (burst.curBurst <= 0)
                {
                    enemy.attackCooldown = COOLDOWN_SHOOT_MS;
                    burst.curBurst = atkData.numBullets;
                    burst.burstCooldown = 0;
                }
            }
            nextAtkData(enemy, entity);
        }    
        
    }
}

//Make sure the enemy and the entity are the same entity
void EnemySystem::nextAtkData(Enemy& enemy, Entity& entity) {
    AttackData& atkData = registry.attackDatas.get(entity);
    std::vector<AttackData> atkDatas = enemy.attackData;
    if (atkDatas.size() == 0 || atkDatas.size() == 1) {
        return;
    }
    if (atkData.attackType == EnemyAttackPattern::BURST || atkData.attackType == EnemyAttackPattern::SPRAY) {
        if (enemy.attackCooldown != COOLDOWN_SHOOT_MS) {
            return;
        }
    }
    atkData = atkDatas[rand()% atkDatas.size()];
}


void EnemySystem::shootShotgun(vec2 velocity, vec2 pos, AttackData atkData) {
    float angle = atan2(velocity.y, velocity.x);
    if (atkData.numBullets % 2 == 0) {
        for (uint i = 0; i < atkData.numBullets / 2; i++) {
            float a1 = angle + (i + 0.5) * atkData.angleOffset;
            float a2 = angle - (i + 0.5) * atkData.angleOffset;
            createEnemyBullet(render, pos, {cos(a1), sin(a1)}, atkData.veer.x * vec2(cos(a1 + atkData.veer.y), sin(a1 + atkData.veer.y)), atkData);
            createEnemyBullet(render, pos, {cos(a2), sin(a2)}, atkData.veer.x * vec2(cos(a2 - atkData.veer.y), sin(a2 - atkData.veer.y)), atkData);
        }
    }
    else {
        createEnemyBullet(render, pos, { cos(angle), sin(angle) }, atkData.veer.x * vec2(cos(angle), sin(angle)), atkData);
        for (uint i = 0; i < (atkData.numBullets - 1) / 2; i++) {
            float a1 = angle + (i + 1) * atkData.angleOffset;
            float a2 = angle - (i + 1) * atkData.angleOffset;
            createEnemyBullet(render, pos, { cos(a1), sin(a1) }, atkData.veer.x * vec2(cos(a1 + atkData.veer.y), sin(a1 + atkData.veer.y)), atkData);
            createEnemyBullet(render, pos, { cos(a2), sin(a2) }, atkData.veer.x * vec2(cos(a2 - atkData.veer.y), sin(a2 - atkData.veer.y)), atkData);
        }
    }
}

void EnemySystem::shootAllDirection(vec2 pos, AttackData atkData) {
    for (uint i = 0; i < atkData.numBullets; i++) {
        float a = atkData.angleOffset + i * (2 * M_PI / atkData.numBullets);
        createEnemyBullet(render, pos, { cos(a), sin(a) }, atkData.veer.x * vec2(cos(a + atkData.veer.y), sin(a + atkData.veer.y)), atkData);
    }
}

void EnemySystem::shootBurst(vec2 velocity, vec2 pos, AttackData atkData, float elapsed_ms, Burst& burst, Enemy& enemy) {
    if ((burst.curBurst <= 0) || (burst.burstCooldown -= elapsed_ms) > 0) {
        return;
    }

    if (atkData.attackType == EnemyAttackPattern::SPRAY)
    {
        double range = atkData.angleOffset;

        // Generate a random offset within the range
        double offset = (2 * (static_cast<double>(rand()) / RAND_MAX) - 1) * range;
        offset = burst.burstDirection + offset;
        createEnemyBullet(render, pos, {cos(offset), sin(offset)}, atkData.veer.x * vec2(cos(offset + atkData.veer.y)),atkData);
    } else if (burst.curBurst != atkData.numBullets) {
        float currentAngle = atan2(velocity.y, velocity.x);
        float angleDifference = currentAngle - burst.burstDirection;

        float maxDifference = M_PI / 32;
        if (abs(angleDifference) > maxDifference) {
            if (angleDifference > 0) {
                currentAngle = burst.burstDirection + maxDifference;
            } else {
                currentAngle = burst.burstDirection - maxDifference;
            }
        }

        float angle = currentAngle;
        //std::cout << angle << std::endl;
        createEnemyBullet(render, pos, {cos(angle), sin(angle)}, atkData.veer.x * vec2(cos(angle + atkData.veer.y)), atkData);
    }
    burst.curBurst--;
    burst.burstCooldown = 150;


}





