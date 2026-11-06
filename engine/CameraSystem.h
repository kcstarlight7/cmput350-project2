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

        for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend();
             ++iterator) {
            auto currentID = iterator->GetID();
            if (entityManager.HasComponent<CameraFollower>(currentID)) {
                // set camera to currentID2
                camPos = &entityManager.GetComponent<LocationComponent>(currentID).loc;
            }
        }

        for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend(); ++iterator) {
            // loops through all entities
            auto currentID = iterator->GetID();
            if (entityManager.HasComponent<CameraComponent>(currentID)) {
                CameraComponent camComp = entityManager.GetComponent<CameraComponent>(currentID);

                if (camPos != nullptr) {
                    camComp.position = *camPos;
                }

                // set camera
                sf::Vector2f screenPos = (sf::Vector2f)windowManager.GetWindow()->getSize() * 0.5f;
                
                windowManager.SetWorldScale(camComp.worldUnitsPerPixel);
                windowManager.SetCamera(Point2D(camComp.position.x, 0.f), Point2D(screenPos.x, screenPos.y));
                //windowManager.SetCamera()
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
