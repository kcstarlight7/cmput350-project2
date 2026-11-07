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
            if (entityManager.template HasComponent<GravityComponent>(currentID)) {
                if (!entityManager.template GetComponent<GravityComponent>(currentID)
                         .disabledTimer) {  // apply gravity
                    if (entityManager.template HasComponent<CollisionComponent>(
                            currentID)) {  // gravity respects collisions
                        if (entityManager.template GetComponent<CollisionComponent>(currentID)
                                .onGround) {  // does not fall if on ground
                            entityManager.template GetComponent<GravityComponent>(currentID)
                                .acceleration = {0.0f, 0.0f};
                        } else if (!entityManager
                                        .template GetComponent<CollisionComponent>(
                                            currentID)  // case not on ground
                                       .onGround) {
                            entityManager.template GetComponent<GravityComponent>(currentID)
                                .acceleration.y += kGravity;
                        } else if (entityManager
                                       .template GetComponent<CollisionComponent>(currentID)
                                       .onLeftWall ||
                                   entityManager
                                       .template GetComponent<CollisionComponent>(currentID)
                                       .onRightWall) {  // case wall jumping
                            entityManager.template GetComponent<GravityComponent>(currentID)
                                .acceleration.y += kGravityWall;
                        }
                    }
                } else {  // tick down timer
                    entityManager.template GetComponent<GravityComponent>(currentID)
                        .disabledTimer -= 1;
                }
			}
        }

        return kContinue;
    }
};

}  // namespace ECSEngine