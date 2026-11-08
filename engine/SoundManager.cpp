#include "SoundManager.h"

namespace ECSEngine {

void SoundManager::RegisterSound(const std::string& soundPath, const std::string& soundName) {
    sf::SoundBuffer buffer(soundPath);
    mSoundBuffers.insert( {soundName, buffer} );
    mSounds.insert({soundName, mSoundInstances.size()});
    mSoundInstances.push_back(sf::Sound(mSoundBuffers[soundName]));
}

sf::Sound& SoundManager::GetSound(const std::string& soundName) {
    auto soundLocation = mSounds.find(soundName);
    return mSoundInstances[soundLocation->second];
}

}  // namespace ECSEngine
