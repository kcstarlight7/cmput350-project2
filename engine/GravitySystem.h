#pragma once

namespace ECSEngine {

const float kGravity = -1.0f; // TODO: fine tune in testing
const float kGravityWall = -0.5f; // TODO: ^

template <typename... Components>
class GravitySystem : public System<Components...> {
public:
    const std::string& GetName() const override {
        static std::string name("GravitySystem");
        return name;
    }

    tSystemReturn Run(EngineView<Components...>* engine) override {
        auto& entityManager = engine->GetEntityManager();
        for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend(); ++iterator) {
            // loops through all entities
            auto currentID = iterator->GetID();
            if (entityManager.template HasComponent<GravityComponent>(currentID) &&
                entityManager.template HasComponent<CollisionComponent>(currentID) &&
                entityManager.template HasComponent<MovementComponent>(currentID)) {
                GravityComponent& gravityComp =
                    entityManager.template GetComponent<GravityComponent>(currentID);
                if (gravityComp.disabledTimer) {
                    gravityComp.disabledTimer -= 1;
                    return kContinue;
                }

                CollisionComponent& collisionComp =
                    entityManager.template GetComponent<CollisionComponent>(currentID);
                MovementComponent& movementComp =
                    entityManager.template GetComponent<MovementComponent>(currentID);

                if (collisionComp.onGround) {  // does not fall if on ground
                    gravityComp.acceleration = {0.0f, 0.0f};
                    movementComp.velocity.y = 0;
                    if (entityManager.GetEntityType(currentID) == "Star") {
                        movementComp.velocity.x = 0;
                    }
                } else {
                    gravityComp.acceleration.y -= kGravity;
                }
                if (collisionComp.onLeftWall || collisionComp.onRightWall) {  // case wall jumping
                    gravityComp.acceleration.y -= kGravityWall;
                }

                movementComp.velocity += gravityComp.acceleration;
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine