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
    while (const std::optional event = mWindowManager.GetWindow()->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            running = false;
            mWindowManager.GetWindow()->close();
        } 

        int keydown = 0;
        uint64_t pressed = 0;
        
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            pressed = 1;
            keydown = (int)keyPressed->scancode;
        }

        ComponentStorage<InputComponent>& inputComponent =
            mEntityManager.template GetComponentStorage<InputComponent>();
        for (InputComponent& i : inputComponent.mStorage) {
            std::bitset<sf::Keyboard::ScancodeCount>& lastKey = i.lastKeydown;
            std::bitset<sf::Keyboard::ScancodeCount>& keyD = i.keydown;
            lastKey = keyD;
            keyD.reset();
            if (pressed) {
                keyD.set(keydown);
            }
            std::cout << "code of key pressed: " << keydown << "\n";
        }
        
        /* else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
            ComponentStorage<InputComponent> inputComponent =
                mEntityManager.GetComponentStorage<InputComponent>();
            for (InputComponent i : inputComponent.mStorage) {
                i.lastKeydown = i.keydown;
                i.keydown.reset();
                i.keydown.set((int)keyReleased->scancode);
                std::cout << "code of key released: " << (int)keyReleased->scancode << "\n";
            }
        }*/
    }
    return true;
}

template <typename... Components>
void ECSEngine<Components...>::DebugDraw() {}

}  // namespace ECSEngine
