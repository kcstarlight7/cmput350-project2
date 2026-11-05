#pragma once

namespace ECSEngine {

template <typename... Components>
class ScoreSystem : public System<Components...> {
public:
    const std::string& GetName() const override {
        static std::string name("ScoreSystem");
        return name;
    }

    tSystemReturn Run(EngineView<Components...>* engine) override { 
        auto entityManager = engine->GetEntityManager();
        for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend(); ++iterator) {
            // loops through all entities
            auto currentID = iterator->GetID();
            if (entityManager.HasComponent<ScoreComponent>(currentID)) {
                int score;
                auto& scoreComponent = entityManager.GetComponent<ScoreComponent>(currentID);
                
                if (scoreComponent.score > 999) {
                    score = 999;
                } else {
                    score = scoreComponent.score;
                }
                int digitCount = 0;
                while (score > 0) {
                    // from right to left (rev order)
                    int digit = score % 10;
                    auto& currentEntity = scoreComponent.digitID[2 - digitCount];
                    entityManager.GetComponent<SpriteComponent>(currentEntity).textureID =
                        scoreComponent.digitTextureID[digit];
                    entityManager.GetComponent<SpriteComponent>(currentEntity).worldSpace = false;
                    score /= 10;
                    digitCount++;
                }
                while (digitCount != 3) {
                    // fill rest with 0 if score is less than 3 digits long
                    // assumes digitTextureID is filled from 0 to 9 instead of 1 to 9 then 0
                    auto& currentEntity = scoreComponent.digitID[2 - digitCount];
                    if (entityManager.HasComponent<SpriteComponent>(currentEntity)) {
                        auto spriteComp =
                            entityManager.GetComponent<SpriteComponent>(currentEntity);
                        spriteComp.textureID =
                            scoreComponent.digitTextureID[0];
                        spriteComp.worldSpace =
                            false;
                    }
                    digitCount++;
                }

            }
        }
        return kContinue; 
    }
};

}  // namespace ECSEngine
