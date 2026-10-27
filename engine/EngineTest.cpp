#include "EntityManager.h"

int main(int argc, char* argv[]) { 
	struct Position {
        float x;
        float y;
    };

    struct Health {
        int value;
    };

    ECSEngine::EntityManager<Position, Health> manager;
    auto id = manager.CreateEntity("Test");
    manager.AddComponent(id, Position{1.0f, 2.0f});
    manager.AddComponent(id, Health{100});
    manager.RemoveEntity(id);
	return 0; 
}
