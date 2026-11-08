#pragma once

#include <random>

namespace ECSEngine {

template <typename... Components>
class SpawnSystem : public System<Components...> {
public:
    const std::string& GetName() const override {
        static std::string name("SpawnSystem");
        return name;
    }

    tSystemReturn Run(EngineView<Components...>* engine) override { 
        auto& entityManager = engine->GetEntityManager();
        std::vector<std::string> spawns;
        Point2D spawnerLoc;
        Rect spawnerSize;
        for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend(); ++iterator) {
            // loops through all entities
            auto currentID = iterator->GetID();
            if (entityManager.template HasComponent<SpawnComponent>(currentID)) {
                auto& currentComponent =
                    entityManager.template GetComponent<SpawnComponent>(currentID);
                spawnerLoc = entityManager.template GetComponent<LocationComponent>(currentID).loc;
                spawnerSize = entityManager.template GetComponent<SpriteComponent>(currentID).bounds;
                if (currentComponent.timeToNextSpawn > 0) {
                    currentComponent.timeToNextSpawn--;
                } else {
                    if (currentComponent.totalSpawns > 0) {
                        spawns.push_back(currentComponent.spawnType);
                        currentComponent.timeToNextSpawn = currentComponent.timeBetweenSpawns;
                        currentComponent.totalSpawns--;
                    }
                }
            }
        }

        for (std::string spawn : spawns) {
            std::random_device seed;
            std::mt19937 random(seed());
            std::uniform_real_distribution<float> range(-2.f, 2.f);
            std::cout << "spawning" << std::endl;
            EntityID spawnID = entityManager.CreateEntity(spawn);
            SpriteComponent spriteComp;
            spriteComp.textureID = engine->GetSpriteManager().RegisterTexture(
                "..\\assets\\spritesheet-tiles-default.png", 
                Rect(Point2D(652, 332), 40, 40));
            spriteComp.bounds = Rect(Point2D(0, 0), 40, 40);

            Point2D spawnerCenter(spawnerLoc.x + (spawnerSize.width / 2), spawnerLoc.y + (spawnerSize.height / 2));
            LocationComponent locComp;
            locComp.loc = Point2D(spawnerCenter.x - spriteComp.bounds.width / 2, spawnerCenter.y - spriteComp.bounds.height / 2);
            
            GravityComponent gravComp(Point2D(range(random), range(random)));
            MovementComponent moveComp;
            moveComp.velocity = Point2D(0, 0);

            CollisionComponent collideComp;
            collideComp.collisionBox = Rect(Point2D(0, 0), 40, 40);
            collideComp.doesDamage = 0;
            collideComp.staticEntity = 0;
            collideComp.onGround = 0;
            collideComp.onCeiling = 0;
            collideComp.onLeftWall = 0;
            collideComp.onRightWall = 0;
            
            entityManager.template AddComponent<SpriteComponent>(spawnID, spriteComp);
            entityManager.template AddComponent<LocationComponent>(spawnID, locComp);
            entityManager.template AddComponent<GravityComponent>(spawnID, gravComp);
            entityManager.template AddComponent<CollisionComponent>(spawnID, collideComp);
            entityManager.template AddComponent<MovementComponent>(spawnID, moveComp);
        }
        return kContinue; 
    }
};

}  // namespace ECSEngine
