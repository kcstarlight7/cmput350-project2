#pragma once

#include <unordered_map>

#include "LocationComponent.h"
#include "MathUtil.h"
#include "SpriteComponent.h"
#include "SystemManager.h"
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
        auto entityManager = engine->GetEntityManager();
        auto windowManager = engine->GetWindowManager();
        for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend(); ++iterator) {
            // loops through all entities
            auto currentID = iterator->GetID();
            if (entityManager.HasComponent<CameraFollower>(currentID) && entityManager.HasComponent<LocationComponent>(currentID)) {
                Point2D loc = entityManager.GetComponent<LocationComponent>(currentID).loc;
                sf::Vector2f screenPos = windowManager.GetWindow()->getSize() * 0.5f;

                windowManager.SetCamera(loc, Point2D(screenPos.x, screenPos.y)); // implemented as center tracking camera for now
            }
        }
    }
};

}  // namespace ECSEngine
