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
        auto entityManager = engine->GetEntityManager();
        std::vector<std::string> spawns;
        for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend(); ++iterator) {
            // loops through all entities
            auto currentID = iterator->GetID();
            if (entityManager.HasComponent<SpawnComponent>(currentID)) {
                auto& currentComponent = entityManager.GetComponent<SpawnComponent>(currentID);
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
            entityManager.CreateEntity(spawn);
        }
        return kContinue; 
    }
};

}  // namespace ECSEngine
