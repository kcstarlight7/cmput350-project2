#include "EntityManager.h"
#include "ECSEngine.h"

int main(int argc, char* argv[]) { 

    auto engine = ECSEngine::ECSEngine<>(1400, 800, "Testing");
    auto& mSystemManager = engine.GetSystemManager();
    //mSystemManager.AddSystem(std::make_unique<CollisionLateSystem<Components...>>());
    //mSystemManager.AddSystem(std::make_unique<InputSystem<Components...>>());
    //mSystemManager.AddSystem(std::make_unique<GravitySystem<Components...>>());
    //mSystemManager.AddSystem(std::make_unique<MovementSystem<Components...>>());
    //mSystemManager.AddSystem(std::make_unique<CollisionSystem<Components...>>());
    //mSystemManager.AddSystem(std::make_unique<ScoreSystem<Components...>>());
    //mSystemManager.AddSystem(std::make_unique<CameraSystem<Components...>>());
    //mSystemManager.AddSystem(std::make_unique<SpriteSystem<Components...>>());
    //mSystemManager.AddSystem(std::make_unique<SpawnSystem<Components...>>());
    engine.Run();
	return 0; 
}
