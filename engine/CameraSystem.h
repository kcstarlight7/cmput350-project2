#pragma once

#include <unordered_map>

#include "LocationComponent.h"
#include "CameraComponent.h"
#include "MathUtil.h"
#include "SpriteComponent.h"
#include "SystemManager.h"
#include "WindowManager.h"
#include "EntityManager.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace ECSEngine {

template <typename... Components>
class CameraSystem : public System<Components...> {
public:
    const std::string& GetName() const override {
        static std::string name = "CameraSystem";
        return name;
    }

    tSystemReturn Run(EngineView<Components...>* engine) override {
        EntityManager<Components...>& entityManager = engine->GetEntityManager();
        WindowManager& windowManager = engine->GetWindowManager();

        Point2D *camPos = nullptr;
        SpriteComponent spriteComp;
        CameraShake *camShakeComp;

        for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend();
             ++iterator) {
            auto currentID = iterator->GetID();
            if (entityManager.template HasComponent<CameraFollower>(currentID) &&
                entityManager.template HasComponent<SpriteComponent>(currentID)) {
                // set camera to currentID2
                camPos = &entityManager.template GetComponent<LocationComponent>(currentID).loc;

                spriteComp = entityManager.template GetComponent<SpriteComponent>(currentID);
                camShakeComp = &entityManager.template GetComponent<CameraShake>(currentID);
            }

        }

        for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend(); ++iterator) {
            // loops through all entities
            auto currentID = iterator->GetID();
            if (entityManager.template HasComponent<CameraComponent>(currentID)) {
                CameraComponent& camComp =
                    entityManager.template GetComponent<CameraComponent>(currentID);

                Point2D shakeOffset(0.f, 0.f);

                if (camPos != nullptr) {
                    if (camShakeComp->frameLimit) {
                        camShakeComp->frameLimit -= 1;
                        std::cout << camShakeComp->frameLimit << std::endl;
                        if (camShakeComp->frameLimit % 2 == 1) {
                            shakeOffset = camShakeComp->offset;
                        } else {
                            shakeOffset = camShakeComp->offset * -1;
                        }
                    }

                    Point2D entityPtWorld = *camPos + spriteComp.bounds.width / 2;  // world
                    Point2D entityPtWindow = windowManager.WorldToWindow(entityPtWorld);

                    std::cout << "player: " << windowManager.WorldToWindow(entityPtWorld) << "\n";

                    if (!(entityPtWindow.x > camComp.hardExclusion.topLeft.x && entityPtWindow.x < camComp.hardExclusion.topLeft.x + camComp.hardExclusion.width)) {
                        if (entityPtWindow.x > windowManager.GetWindowBounds().width / 2) {
                            camComp.position.x +=
                                1 * (entityPtWindow.x - (camComp.hardExclusion.topLeft.x +
                                                           camComp.hardExclusion.width));
                        } else {
                            camComp.position.x +=
                                1 * (entityPtWindow.x - (camComp.hardExclusion.topLeft.x));
                        }
                    }
                    if (!(entityPtWindow.x > camComp.softExclusion.topLeft.x && entityPtWindow.x < camComp.softExclusion.topLeft.x + camComp.softExclusion.width)) {
                        if (entityPtWindow.x > windowManager.GetWindowBounds().width / 2) {
                            camComp.position.x +=
                                0.3 * (entityPtWindow.x - (camComp.softExclusion.topLeft.x +
                                                           camComp.softExclusion.width));
                        } else {
                            camComp.position.x +=
                                0.3 * (entityPtWindow.x - (camComp.softExclusion.topLeft.x));
                        }
                    }

                    
                }

                // set camera
                sf::Vector2f screenPos = (sf::Vector2f)windowManager.GetWindow()->getSize() * 0.5f;
                
                windowManager.SetWorldScale(camComp.worldUnitsPerPixel);
                windowManager.SetCamera(Point2D(camComp.position.x, 0.f),
                                        Point2D(screenPos.x + shakeOffset.x, 0.f + shakeOffset.y));
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
