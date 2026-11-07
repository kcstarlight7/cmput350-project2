#pragma once

namespace ECSEngine {

template <typename... Components>
class MovementSystem : public System<Components...> {
public:
    const std::string& GetName() const override {
        static std::string name("MovementSystem");
        return name;
    }

    tSystemReturn Run(EngineView<Components...>* engine) override { 
        auto& entityManager = engine->GetEntityManager();
        for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend(); ++iterator) {
            // loops through all entities
            auto currentID = iterator->GetID();
            if (entityManager.template HasComponent<MovementComponent>(currentID)) {
                Point2D& velocity =
                    entityManager.template GetComponent<MovementComponent>(currentID).velocity;
                Point2D& currentLoc =
                    entityManager.template GetComponent<LocationComponent>(currentID).loc;
                int frames = engine->GetWindowManager().framesPerSecond;
                currentLoc.x += (velocity.x / frames);
                currentLoc.y += (velocity.y / frames);
            }
        }
        return kContinue; 
    }
};

}  // namespace ECSEngine
