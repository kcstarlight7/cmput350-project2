#include "EntityManager.h"
#include "ECSEngine.h"
#include "InputComponent.h"
#include "InputSystem.h"
#include <CollisionLateSystem.h>
#include <GravitySystem.h>
#include <MovementSystem.h>
#include <CollisionSystem.h>
#include <ScoreSystem.h>
#include <CameraSystem.h>
#include <SpriteSystem.h>
#include <SpawnSystem.h>

#define COMPONENTS                                                                             \
    ECSEngine::InputComponent, ECSEngine::MovementComponent, ECSEngine::ScoreComponent, \
        ECSEngine::SpawnComponent, ECSEngine::SpriteComponent, ECSEngine::CameraComponent,     \
        ECSEngine::CameraFollower, ECSEngine::LocationComponent, ECSEngine::GravityComponent,     \
        ECSEngine::CameraShake, ECSEngine::CollisionComponent, ECSEngine::RespawnComponent
using Engine = ECSEngine::ECSEngine<COMPONENTS>;

int main(int argc, char* argv[]) { 

    auto engine = Engine(1400, 800, "Testing");
    auto& sm = engine.GetSystemManager();
    sm.AddSystem(std::make_unique<ECSEngine::CollisionLateSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::InputSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::GravitySystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::MovementSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::CollisionSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::ScoreSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::CameraSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::SpriteSystem<COMPONENTS>>());
    sm.AddSystem(std::make_unique<ECSEngine::SpawnSystem<COMPONENTS>>());
    engine.Run();
	return 0; 
}
