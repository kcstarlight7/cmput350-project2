#include "SpriteManager.h"

namespace ECSEngine {

SpriteID SpriteManager::RegisterTexture(const std::string& texturePath, const Rect& sourceRect) {
    sf::Texture* tex = nullptr;
    for (auto& value : mTextures) {
        if (value.first == texturePath) {
            tex = &value.second;
        }
    }

    if (tex == nullptr) {
        tex = new sf::Texture(texturePath, false);
        mTextures.insert({texturePath, *tex});
    }

    SpriteID id = mSprites.size();
    sf::Sprite sprite(*tex);
    sprite.setTextureRect(sf::IntRect(sf::Vector2i(static_cast<int>(sourceRect.topLeft.x), static_cast<int>(sourceRect.topLeft.y)), sf::Vector2i(static_cast<int>(sourceRect.width), static_cast<int>(sourceRect.height))));
    mSprites.push_back(sprite);
    return id;
}

sf::Sprite& SpriteManager::GetSprite(SpriteID id) { return mSprites[id]; }

}  // namespace ECSEngine
