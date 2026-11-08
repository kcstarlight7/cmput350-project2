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

        for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend();
             ++iterator) {
            auto currentID = iterator->GetID();
            if (entityManager.template HasComponent<CameraFollower>(currentID) &&
                entityManager.template HasComponent<SpriteComponent>(currentID)) {
                // set camera to currentID2
                camPos = &entityManager.template GetComponent<LocationComponent>(currentID).loc;

                spriteComp = entityManager.template GetComponent<SpriteComponent>(currentID);
            }

        }

        for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend(); ++iterator) {
            // loops through all entities
            auto currentID = iterator->GetID();
            if (entityManager.template HasComponent<CameraComponent>(currentID)) {
                CameraComponent camComp =
                    entityManager.template GetComponent<CameraComponent>(currentID);

                if (camPos != nullptr) {
                    camComp.position = *camPos;
                    camComp.position += spriteComp.bounds.width / 2;
                }

                // set camera
                sf::Vector2f screenPos = (sf::Vector2f)windowManager.GetWindow()->getSize() * 0.5f;
                
                windowManager.SetWorldScale(camComp.worldUnitsPerPixel);
                windowManager.SetCamera(
                    Point2D(camComp.position.x - screenPos.x * windowManager.GetWorldScale(), 0.f),
                    Point2D(screenPos.x, screenPos.y));
                //windowManager.SetCamera()
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
