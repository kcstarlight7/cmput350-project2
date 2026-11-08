#pragma once

#include "Keyboard.h"

namespace ECSEngine {

const float kMaxRunVelocity = 15;
const float kDashVelocity = 50;
const float kJumpVelocity = 5;
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
        auto& entityManager = engine->GetEntityManager();
        for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend(); ++iterator) {
            //loops through all entities
            auto currentID = iterator->GetID();
            if (entityManager.template HasComponent<InputComponent>(currentID)) {
                //if entity has input component
                //std::cout << "has input" << "\n";
                InputComponent& inputComp = entityManager.template GetComponent<InputComponent>(currentID);
                bool currentDisabled =
                    (entityManager.template GetComponent<GravityComponent>(currentID).disabledTimer == 0);
                //std::cout << inputComp.keydown.any() << "\n";
                if (inputComp.keydown[Key::Left]) {
                    //move left
                    std::cout << "left" << "\n";
                    if (entityManager.template HasComponent<MovementComponent>(currentID)) {
                        if (!currentDisabled && inputComp.keydown[Key::C]) {
                            // dash while moving
                            Point2D currentVelocity =
                                entityManager.template GetComponent<MovementComponent>(currentID)
                                    .velocity;
                            entityManager.template GetComponent<MovementComponent>(currentID)
                                .velocity.x =
                                currentVelocity.x + 0.8 * ((-kMaxRunVelocity) - currentVelocity.x);
                            entityManager.template GetComponent<GravityComponent>(currentID)
                                .disabledTimer =
                                30;
                            return kContinue;
                        }
                        Point2D currentVelocity = 
                            entityManager.template GetComponent<MovementComponent>(currentID)
                                .velocity;
                        entityManager.template GetComponent<MovementComponent>(currentID)
                            .velocity.x =
                            currentVelocity.x + 0.8 * ((-kMaxRunVelocity) - currentVelocity.x);
                    }
                } else if (inputComp.keydown[Key::Right]) {
                    //move right
                    std::cout << "right" << "\n";
                    if (entityManager.template HasComponent<MovementComponent>(currentID)) {
                        if (!currentDisabled && inputComp.keydown[Key::C]) {
                            // dash while moving
                            Point2D currentVelocity =
                                entityManager.template GetComponent<MovementComponent>(currentID)
                                    .velocity;
                            entityManager.template GetComponent<MovementComponent>(currentID)
                                .velocity.x =
                                currentVelocity.x + 0.8 * (kDashVelocity - currentVelocity.x);
                            entityManager.template GetComponent<GravityComponent>(currentID)
                                .disabledTimer =
                                30;
                            return kContinue;
                        }
                        Point2D currentVelocity =
                            entityManager.template GetComponent<MovementComponent>(currentID)
                                .velocity;
                        entityManager.template GetComponent<MovementComponent>(currentID)
                            .velocity.x =
                            currentVelocity.x + 0.8 * (kMaxRunVelocity - currentVelocity.x);
                        return kContinue;
                    }
                } else {
                    if (entityManager.template HasComponent<MovementComponent>(currentID)) {
                        Point2D currentVelocity =
                            entityManager.template GetComponent<MovementComponent>(currentID)
                                .velocity;
                        entityManager.template GetComponent<MovementComponent>(currentID)
                            .velocity.x = 0;
                    }
                }
                if (inputComp.keydown[Key::Z]) {
                    // jump
                    std::cout << "jump" << "\n";
                    inputComp.jumpDuration++;
                    std::cout << inputComp.jumpDuration << "\n";
                    
                    entityManager.template GetComponent<CollisionComponent>(currentID).onGround = 0;
                    entityManager.template GetComponent<GravityComponent>(currentID)
                        .acceleration.y = -(kJumpVelocity / inputComp.jumpDuration);
                } else {
                    if (inputComp.lastKeydown[Key::Z]) {
                        entityManager.template GetComponent<GravityComponent>(currentID)
                            .acceleration.y = 0;
                        if (inputComp.jumpDuration >= 5) {
                            engine->GetSoundManager().GetSound("high_jump").play();
                        } else {
                            engine->GetSoundManager().GetSound("jump").play();
                        }
                    }
                    inputComp.jumpDuration = 0;
                }
                if (inputComp.keydown[Key::C]) {
                    // dash
                    // default dash forward?
                    std::cout << "dash" << "\n";
                    if (!currentDisabled &&
                        entityManager.template HasComponent<MovementComponent>(currentID)) {
                        Point2D currentVelocity =
                            entityManager.template GetComponent<MovementComponent>(currentID)
                                .velocity;
                        entityManager.template GetComponent<MovementComponent>(currentID)
                            .velocity.x =
                            currentVelocity.x + 0.8 * (kDashVelocity - currentVelocity.x);
                        entityManager.template GetComponent<GravityComponent>(currentID)
                            .disabledTimer = 30;
                        return kContinue;
                    }
                }
                
            }
        }
        return kContinue; 
    }
};

}  // namespace ECSEngine
