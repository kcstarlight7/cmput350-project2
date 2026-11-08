#pragma once

#include <vector>
#include <iostream>

#include "CameraComponent.h"
#include "CollisionComponent.h"
#include "EntityManager.h"
#include "InputComponent.h"
#include "LocationComponent.h"
#include "MathUtil.h"
#include "ScoreComponent.h"
#include "SoundManager.h"
#include "SpawnComponent.h"
#include "SpriteComponent.h"
#include "SpriteManager.h"
#include "SystemManager.h"
#include "WindowManager.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace ECSEngine {

template <typename... Components>
class EngineView {
public:
    virtual SoundManager& GetSoundManager() = 0;
    virtual SpriteManager& GetSpriteManager() = 0;
    virtual EntityManager<Components...>& GetEntityManager() = 0;
    virtual WindowManager& GetWindowManager() = 0;
};

template <typename... Components>
class ECSEngine : public EngineView<Components...> {
public:
    ECSEngine(unsigned int width, unsigned int height, const std::string& name);
    ~ECSEngine();

    void Run();

    SoundManager& GetSoundManager() override { return mSoundManager; }

    SpriteManager& GetSpriteManager() override { return mSpriteManager; }

    EntityManager<Components...>& GetEntityManager() override { return mEntityManager; }

    WindowManager& GetWindowManager() override { return mWindowManager; }

    SystemManager<Components...>& GetSystemManager() { return mSystemManager; }

private:
    bool ProcessEvents();
    void DebugDraw();

    EntityManager<Components...> mEntityManager;
    SystemManager<Components...> mSystemManager;
    SpriteManager mSpriteManager;
    SoundManager mSoundManager;
    WindowManager mWindowManager;

    // For debugging - pause and step the engine
    bool running, step;
    int debug;
};

template <typename... Components>
ECSEngine<Components...>::ECSEngine(unsigned int width, unsigned int height,
                                    const std::string& name)
    : mWindowManager(width, height, name) {}

template <typename... Components>
ECSEngine<Components...>::~ECSEngine() {}

template <typename... Components>
void ECSEngine<Components...>::Run() {
    // repeats main game loop
    // (runs systems from system manager, then draws window)
    running = true;
    while (running) {
        mWindowManager.GetWindow()->clear();
        ProcessEvents();
        if (!mSystemManager.Run(this)) {
            running = false;
        }
        mWindowManager.GetWindow()->display();
    }
}

template <typename... Components>
bool ECSEngine<Components...>::ProcessEvents() {
    // process events from sfml and updating input component

    std::bitset<sf::Keyboard::ScancodeCount> keydowns{};
    std::bitset<sf::Keyboard::ScancodeCount> keyups{};

    while (const std::optional event = mWindowManager.GetWindow()->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            running = false;
            mWindowManager.GetWindow()->close();
        } 

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            keydowns.set((int)keyPressed->scancode);
        }

        if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
            keyups.set((int)keyReleased->scancode);
        }
    }

    //std::cout << keydowns << std::endl;

    
    auto& entityManager = GetEntityManager();
    for (auto iterator = entityManager.cbegin(); iterator != entityManager.cend(); ++iterator) {
        // loops through all entities
        auto currentID = iterator->GetID();
        if (entityManager.template HasComponent<InputComponent>(currentID)) {
            InputComponent& inpComp =
                entityManager.template GetComponent<InputComponent>(currentID);

            inpComp.lastKeydown = inpComp.keydown;
            inpComp.keydown |= keydowns;
            for (unsigned int i = 0; i < sf::Keyboard::ScancodeCount; i++) {
                if (inpComp.keydown.test(i) == keyups.test(i)) {
                    inpComp.keydown.reset(i);
                }
            }
        }
    }

    return true;
}

template <typename... Components>
void ECSEngine<Components...>::DebugDraw() {}

}  // namespace ECSEngine
