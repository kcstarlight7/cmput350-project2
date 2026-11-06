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
        for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend(); ++iterator) {
            // loops through all entities
            auto currentID = iterator->GetID();
            if (entityManager.HasComponent<CameraComponent>(currentID)) {
                CameraComponent camComp = entityManager.GetComponent<CameraComponent>(currentID);


                // handles camera follower by tracking entity and updating camera component location
                if (entityManager.HasComponent<CameraFollower>(currentID) &&
                    entityManager.HasComponent<LocationComponent>(currentID)) {
                    Point2D loc = entityManager.GetComponent<LocationComponent>(currentID).loc;

                    camComp.position = loc; // center tracking for now
                }

                // set camera
                sf::Vector2f screenPos = (sf::Vector2f)windowManager.GetWindow()->getSize() * 0.5f;
                
                windowManager.SetWorldScale(camComp.worldUnitsPerPixel);
                windowManager.SetCamera(camComp.position, camComp.position);
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
