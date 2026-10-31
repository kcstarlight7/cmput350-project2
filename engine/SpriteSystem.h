#pragma once

#include <bitset>
#include <unordered_map>

#include "CameraComponent.h"
#include "EntityManager.h"
#include "MathUtil.h"
#include "SpriteManager.h"
#include "SystemManager.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace ECSEngine {

template <typename... Components>
class SpriteSystem : public System<Components...> {
public:
    const std::string& GetName() const override {
        static std::string name("SpriteSystem");
        return name;
    }

    tSystemReturn Run(EngineView<Components...>* engine) override { 
        auto entityManager = engine->GetEntityManager();
        auto windowManager = engine->GetWindowManager();
        auto spriteManager = engine->GetSpriteManager();
        for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend(); ++iterator) {
            // loops through all entities
            auto currentID = iterator->GetID();
            if (entityManager.HasComponent<SpriteComponent>(currentID) && entityManager.HasComponent<LocationComponent>(currentID)) {
                Point2D loc = entityManager.GetComponent<LocationComponent>(currentID).loc;
                SpriteComponent spriteData = entityManager.GetComponent<SpriteComponent>(currentID);
                
                sf::Sprite sprite(spriteManager.GetSprite(spriteData.textureID));

                if (spriteData.worldSpace) {
                    sprite.setPosition({ loc.x;  loc.y });
                } else {
                    Point2D screenLoc = windowManager.WorldToWindow(loc);
                    sprite.setPosition({
                        screenLoc.x;
                        screenLoc.y
                    });
                }
                
                windowManager.GetWindow()->draw(sprite);
            }
        }
    }
};

}  // namespace ECSEngine
