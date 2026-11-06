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

                for (auto iterator2 = entityManager.cbegin(); iterator2 != entityManager.cend();
                     ++iterator2) {
                    auto currentID2 = iterator2->GetID();
                    if (entityManager.HasComponent<CameraFollower>(currentID2)) {
                        // set camera to currentID2
                        camComp.position =
                            entityManager.GetComponent<LocationComponent>(currentID2).loc;
                    }
                }

                // set camera
                sf::Vector2f screenPos = (sf::Vector2f)windowManager.GetWindow()->getSize() * 0.5f;
                
                //windowManager.SetWorldScale(camComp.worldUnitsPerPixel);
                //windowManager.SetWorldCenter(camComp.position);
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
