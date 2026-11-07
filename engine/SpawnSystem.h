#pragma once

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
        for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend(); ++iterator) {
            // loops through all entities
            auto currentID = iterator->GetID();
            if (entityManager.template HasComponent<SpawnComponent>(currentID)) {
                auto& currentComponent =
                    entityManager.template GetComponent<SpawnComponent>(currentID);
                spawnerLoc = entityManager.template GetComponent<LocationComponent>(currentID).loc;
                if (currentComponent.timeToNextSpawn > 0) {
                    currentComponent.timeToNextSpawn--;
                } else {
                    if (currentComponent.totalSpawns > 0) {
                        spawns.push_back(currentComponent.spawnType);
                        currentComponent.timeToNextSpawn = currentComponent.timeBetweenSpawns;
                    }
                }
            }
        }

        for (std::string spawn : spawns) {
            std::cout << "spawning" << std::endl;
            EntityID spawnID = entityManager.CreateEntity(spawn);
            SpriteComponent spriteComp;
            spriteComp.textureID = engine->GetSpriteManager().RegisterTexture("..\\assets\\spritesheet-tiles-default.png", Rect(Point2D(640, 320), 64, 64));

            LocationComponent locComp;
            locComp.loc = spawnerLoc;
            
            GravityComponent gravComp(Point2D(0, 0));

            CollisionComponent collideComp;
            collideComp.collisionBox = Rect(Point2D(0, 0), 64, 64);
            collideComp.doesDamage = 0;
            collideComp.staticEntity = 0;
            
            entityManager.template AddComponent<SpriteComponent>(spawnID, spriteComp);
            entityManager.template AddComponent<LocationComponent>(spawnID, locComp);
            entityManager.template AddComponent<GravityComponent>(spawnID, gravComp);
            //entityManager.template AddComponent<CollisionComponent>(spawnID, collideComp);
        }
        return kContinue; 
    }
};

}  // namespace ECSEngine
