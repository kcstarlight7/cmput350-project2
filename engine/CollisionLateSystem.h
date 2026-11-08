#pragma once

namespace ECSEngine {

template <typename... Components>
class CollisionLateSystem : public System<Components...> {
public:
    const std::string& GetName() const override {
        static std::string name("CollisionLateSystem");
        return name;
    }

    tSystemReturn Run(EngineView<Components...>* engine) override { 
        auto& entityManager = engine->GetEntityManager();
        for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend(); ++iterator) {
            // loops through all entities
            auto currentID = iterator->GetID();
            if (entityManager.template HasComponent<CollisionComponent>(currentID) && entityManager.template HasComponent<LocationComponent>(currentID)) {
                CollisionComponent& collisionComp =
                    entityManager.template GetComponent<CollisionComponent>(currentID);
                LocationComponent& locationComp =
                    entityManager.template GetComponent<LocationComponent>(currentID);
                collisionComp.previousCollisionBox = collisionComp.collisionBox;
                collisionComp.previousCollisionBox.topLeft = locationComp.loc;
            }
        }
        return kContinue; 
    }
};

}  // namespace ECSEngine
