#include "SpriteManager.h"

namespace ECSEngine {

SpriteID SpriteManager::RegisterTexture(const std::string& texturePath, const Rect& sourceRect) {
    for (auto& value : mTextures) {
        if (value.first == texturePath) {
            SpriteID id = mSprites.size();
            mSprites.push_back(sf::Sprite(value.second));
            return id;
        }
    }
    
    sf::Texture createdTexture(texturePath, false);
    mTextures.insert({texturePath, createdTexture});
    SpriteID id = mSprites.size();
    sf::Sprite createdSprite(createdTexture, sf::IntRect(sf::Vector2i(static_cast<int>(sourceRect.topLeft.x), static_cast<int>(sourceRect.topLeft.y)), sf::Vector2i(static_cast<int>(sourceRect.width), static_cast<int>(sourceRect.height))));
    mSprites.push_back(sf::Sprite(createdTexture));
    return id;
}

sf::Sprite& SpriteManager::GetSprite(SpriteID id) { return mSprites[id]; }

}  // namespace ECSEngine
