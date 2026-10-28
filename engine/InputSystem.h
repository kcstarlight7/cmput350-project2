#pragma once

#include "Keyboard.h"

namespace ECSEngine {

const float kMaxRunVelocity = 15;
const float kDashVelocity = 50;
const float kJumpVelocity = 25;
const float kGroundRunAcceleration = 5;
const float kAirRunAcceleration = 4;
const float kDashInterval = 25; 
// currently just setting disabled timer to frame rate to handle dash interval
// pending movement system implementation to update?

template <typename... Components>
class InputSystem : public System<Components...> {
public:
    const std::string& GetName() const override {
        static std::string name("InputSystem");
        return name;
    }

    tSystemReturn Run(EngineView<Components...>* engine) override { 
        auto entityManager = engine->GetEntityManager();
        for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend(); ++v) {
            //loops through all entities
            auto currentID = iterator->GetID();
            if (entityManager.HasComponent<InputComponent>(currentID)) {
                //if entity has input component
                InputComponent component = entityManager.GetComponent<InputComponent>(currentID);
                bool currentDisabled =
                    (entityManager.GetComponent<GravityComponent>(currentID).disabledTimer == 0);
                if (component.keydown[Key::Left]) {
                    //move left
                    if (entityManager.HasComponent<MovementComponent>(currentID)) {
                        if (!currentDisabled && component.keydown[Key::C]) {
                            // dash while moving
                            Point2D currentVelocity =
                                entityManager.GetComponent<MovementComponent>(currentID).velocity;
                            entityManager.GetComponent<MovementComponent>(currentID).velocity.x =
                                currentVelocity.x + 0.3((-kMaxRunVelocity) - currentVelocity.x);
                            entityManager.GetComponent<GravityComponent>(currentID).disabledTimer =
                                30;
                            return kContinue;
                        }
                        Point2D currentVelocity = 
                            entityManager.GetComponent<MovementComponent>(currentID).velocity;
                        entityManager.GetComponent<MovementComponent>(currentID).velocity.x =
                            currentVelocity.x + 0.3((-kMaxRunVelocity)-currentVelocity.x);
                    }
                } else if (component.keydown[Key::Right]) {
                    //move right
                    if (entityManager.HasComponent<MovementComponent>(currentID)) {
                        if (!currentDisabled && component.keydown[Key::C]) {
                            // dash while moving
                            Point2D currentVelocity =
                                entityManager.GetComponent<MovementComponent>(currentID).velocity;
                            entityManager.GetComponent<MovementComponent>(currentID).velocity.x =
                                currentVelocity.x + 0.3(kDashVelocity - currentVelocity.x);
                            entityManager.GetComponent<GravityComponent>(currentID).disabledTimer =
                                30;
                            return kContinue;
                        }
                        Point2D currentVelocity =
                            entityManager.GetComponent<MovementComponent>(currentID).velocity;
                        entityManager.GetComponent<MovementComponent>(currentID).velocity.x =
                            currentVelocity.x + 0.3(kMaxRunVelocity - currentVelocity.x);
                        return kContinue;
                    }
                } else if (component.keydown[Key::Z]) {
                    //jump
                    entityManager.GetComponent<MovementComponent>(currentID).velocity.y =
                        kJumpVelocity;
                } else if (component.keydown[Key::C]) {
                    //dash
                    //default dash forward?
                    if (!currentDisabled && entityManager.HasComponent<MovementComponent>(currentID)) {
                        Point2D currentVelocity =
                            entityManager.GetComponent<MovementComponent>(currentID).velocity;
                        entityManager.GetComponent<MovementComponent>(currentID).velocity.x =
                            currentVelocity.x + 0.3(kDashVelocity - currentVelocity.x);
                        entityManager.GetComponent<GravityComponent>(currentID).disabledTimer = 30;
                        return kContinue;
                    }
                }
            }
        }
        return kContinue; 
    }
};

}  // namespace ECSEngine
