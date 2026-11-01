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
        auto entityManager = engine->GetEntityManager();
        for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend(); ++iterator) {
            // loops through all entities
            auto currentID = iterator->GetID();
            if (entityManager.HasComponent<CollisionComponent>(currentID)) {
                auto& currentComponent = entityManager.GetComponent<CollisionComponent>(currentID);
                currentComponent.previousCollisionBox = currentComponent.collisionBox;
            }
        }
        return kContinue; 
    }
};

}  // namespace ECSEngine
