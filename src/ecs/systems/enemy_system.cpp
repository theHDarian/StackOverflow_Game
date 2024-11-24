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
#include "components/presets/particle_presets.hpp"
#include "utils/random.hpp"

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

void EnemySystem::step(float elapsed_ms)
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

        // Lightning bullet creation
        if (((float)enemy.currHealth / (float)enemy.maxHealth < 0.3) && (rand() % 1000 > 990)) {
            createLightningBullet(render, motion.position);
        }

        // Key bullet creation
        if (((float)enemy.currHealth / (float)enemy.maxHealth > 0.7) && (rand() % 1000 < 1)) {
            bool shouldKey = (registry.enemyBullets.components.size() > 0);
            for (EnemyBullet& eb : registry.enemyBullets.components) {
                if (eb.bulletEffects[0].type == Key) {
                    shouldKey = false;
                    break;
                }
            }
            if (shouldKey) createKeyBullet(render, motion.position);
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
            Healer &healer = registry.healers.get(entity);
            healer.coolDown -= elapsed_ms;
            heal(entity, pattern);
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

            if (glm::length(boid.velocity) > 0.0f)
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
                motion.angle += rotationChange;
            }
            else if (direction != vec2(0, 0) && enemy.rotationBehaviour != EnemyRotationBehavior::NONE)
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
                else if (enemy.rotationBehaviour == EnemyRotationBehavior::FACE_CENTER)
                {
                    WindowState &wS = registry.windowStates.components[0];
                    vec2 mid = vec2(wS.width, wS.height) * 0.5f;
                    mid = mid - motion.position;
                    motion.angle = atan2(mid.y, mid.x);
                }
                else if (enemy.rotationBehaviour == EnemyRotationBehavior::FACE_PLAYER)
                {
                    Motion &playerMotion = registry.motions.components[registry.players.entities[0]];
                    vec2 mid = playerMotion.position - motion.position;
                    motion.angle = atan2(mid.y, mid.x);
                }
                else if (enemy.rotationBehaviour == EnemyRotationBehavior::FACE_TWIN)
                {
                    Motion &twinMotion = registry.motions.get(registry.enemyGroups.get(entity).others[0]);
                    vec2 mid = twinMotion.position - motion.position;
                    motion.angle = atan2(mid.y, mid.x);
                }

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
        const Collision &collision = registry.collisions.get(entity);
        Entity other_entity = collision.other;
        if (registry.enemies.has(entity) && registry.playerBullets.has(other_entity) && !registry.deleteds.has(other_entity))
        {
            Enemy &enemyStat = registry.enemies.get(entity);
            PlayerBullet &bulletStat = registry.playerBullets.get(other_entity);
            EnemyPattern &pattern = enemyStat.currEnemyPattern();

            enemyStat.currHealth -= bulletStat.damage;
            if (enemyStat.currHealth <= 0)
            {
                if (!registry.deleteds.has(entity))
                {
                    Fade &f = registry.fades.emplace(entity);
                    registry.deleteds.emplace(entity);



                    registry.emitParticles.replace(entity,PExplode, ParticleProps(),f.max, Random::Int(20) + 20);
                }

                // std::cout << "enemy " << entity << "has died" << std::endl;
            }

            bulletStat.bulletPierce -= 1;
            registry.ignores.get(other_entity).ignores.push_back(entity);

            if (!registry.deleteds.has(other_entity) && bulletStat.bulletPierce < 0)
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
    for (auto &entity : registry.invisibleEnemy.entities)
    {
        Enemy &enemyStat = registry.enemies.get(entity);
        EnemyPattern &pattern = enemyStat.currEnemyPattern();
        if (pattern.type == EnemyBehavior::DEATHSTATE)
        {
            std::cout << "got here" << std::endl;
            if (!registry.deleteds.has(entity))
            {
                Fade &f = registry.fades.emplace(entity);
                registry.deleteds.emplace(entity);
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

void EnemySystem::shootAllDirection(vec2 pos, float offset, AttackData atkData)
{
    for (uint i = 0; i < atkData.numBullets; i++)
    {
        float a = offset + atkData.angleOffset + i * (2 * M_PI / atkData.numBullets);
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
        sound->playEnemyShootSound(sfxNum, 0);
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
void EnemySystem::shootTwinLaser(vec2 pos, Entity enemy, AttackData atkData)
{
        //shoot towards the twin
        Entity other = registry.enemyGroups.get(enemy).others[0];
        Motion& otherMotion = registry.motions.get(other);
        Motion& motion = registry.motions.get(enemy);
        vec2 diff = otherMotion.position - motion.position;
        float angle = atan2(diff.y, diff.x);
        Entity e = createEnemyLaser(render, pos, angle, enemy, atkData);
}

void EnemySystem::attack(Entity entity, EnemyPattern &currPattern, Motion playerMotion, vec2 pos, AttackData atkData, float elapsed_ms)
{
    int sfxNum = atkData.shape == EnemyBulletShape::CIRCLE ? 0 : atkData.shape == EnemyBulletShape::RECTANGLE ? 1
                                                                                                              : 2;

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
        shootLaser(pos, entity, atkData);
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
        shootShotgun(velocity, pos, atkData);
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
    }
}

void EnemySystem::spawn(Entity entity, EnemyPattern &currPattern, vec2 pos, AttackData atkData)
{
    if (registry.animations.has(entity))
        registry.animations.get(entity).frame = 1;
    for (uint i = 0; i < atkData.numBullets; i++)
    {
        createEnemy(render, pos, atkData.spawn);
    }

    currPattern.currAtkCD = currPattern.maxAtkCD;
}

void EnemySystem::creatingMergeBee(int count, vec2 pos)
{
    switch (count)
    {
    case 2:
        // std::cout << "CREATING" << std::endl;
        createEnemy(render, pos, EnemyType::TwoBee);
        break;
    case 3:
        createEnemy(render, pos, EnemyType::ThreeBee);
        break;
    default:
        // std::cout << "CREATING 1" << std::endl;
        createEnemy(render, pos, EnemyType::OneBee);
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
    Healer &healer = registry.healers.get(entity);
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
