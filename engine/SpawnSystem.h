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
                    }
                }
            }
        }

        for (std::string spawn : spawns) {
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
            
            GravityComponent gravComp(Point2D(0, 0));

            CollisionComponent collideComp;
            collideComp.collisionBox = Rect(Point2D(0, 0), 40, 40);
            collideComp.doesDamage = 0;
            collideComp.staticEntity = 0;
            
            entityManager.template AddComponent<SpriteComponent>(spawnID, spriteComp);
            entityManager.template AddComponent<LocationComponent>(spawnID, locComp);
            entityManager.template AddComponent<GravityComponent>(spawnID, gravComp);
            entityManager.template AddComponent<CollisionComponent>(spawnID, collideComp);
        }
        return kContinue; 
    }
};

}  // namespace ECSEngine
