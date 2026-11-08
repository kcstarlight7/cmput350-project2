#pragma once

#include <bitset>
#include <unordered_map>

#include "CameraComponent.h"
#include "EntityManager.h"
#include "MathUtil.h"
#include "SpriteManager.h"
#include "SystemManager.h"
#include "EntityManager.h"
#include "WindowManager.h"
#include "SpriteManager.h"
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
        EntityManager<Components...>& entityManager = engine->GetEntityManager();
        WindowManager& windowManager = engine->GetWindowManager();
        SpriteManager& spriteManager = engine->GetSpriteManager();
        for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend(); ++iterator) {
            // loops through all entities
            auto currentID = iterator->GetID();
            if (entityManager.template HasComponent<SpriteComponent>(currentID) &&
                entityManager.template HasComponent<LocationComponent>(currentID)) {
                Point2D loc = entityManager.template GetComponent<LocationComponent>(currentID).loc;
                Point2D drawingLoc;
                SpriteComponent spriteData =
                    entityManager.template GetComponent<SpriteComponent>(currentID);
                
                sf::Sprite sprite = spriteManager.GetSprite(spriteData.textureID);

                if (spriteData.worldSpace) {
                    sprite.setPosition({loc.x - windowManager.GetWorldCenter().x,
                                        loc.y - windowManager.GetWorldCenter().y});
                    drawingLoc = Point2D(loc.x - windowManager.GetWorldCenter().x,
                                         loc.y - windowManager.GetWorldCenter().y);
                } else {
                    sprite.setPosition({loc.x, loc.y});
                    drawingLoc = Point2D(loc.x, loc.y);
                }
                windowManager.GetWindow()->draw(sprite);
                windowManager.DebugFrameRect(Rect(drawingLoc, spriteData.bounds.width, spriteData.bounds.height), 0, 0, 255, 150);
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
