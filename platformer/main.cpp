#include <fstream>

#include "CameraSystem.h"
#include "CollisionComponent.h"
#include "CollisionLateSystem.h"
#include "CollisionSystem.h"
#include "ECSEngine.h"
#include "EntityManager.h"
#include "GravitySystem.h"
#include "InputComponent.h"
#include "InputSystem.h"
#include "LocationComponent.h"
#include "MathUtil.h"
#include "MovementSystem.h"
#include "ScoreComponent.h"
#include "ScoreSystem.h"
#include "SpawnComponent.h"
#include "SpawnSystem.h"
#include "SpriteComponent.h"
#include "SpriteManager.h"
#include "SpriteSystem.h"

std::string gResourcePath = "../assets/";
#define COMPONENTS                                                                             \
    ECSEngine::LocationComponent, ECSEngine::MovementComponent, ECSEngine::CollisionComponent, \
        ECSEngine::SpriteComponent, ECSEngine::SpawnComponent, ECSEngine::CameraComponent,     \
        ECSEngine::CameraFollower, ECSEngine::InputComponent, ECSEngine::GravityComponent,     \
        ECSEngine::CameraShake, ECSEngine::ScoreComponent, ECSEngine::RespawnComponent
using Engine = ECSEngine::ECSEngine<COMPONENTS>;

static void LoadMap(Engine& engine, const std::string& path, const std::string& file) {
    std::string folder = "..\\assets\\";
    std::filesystem::path fullPath = std::filesystem::path(folder) / file;
    //std::string fullPath = path + file;
    //while (fullPath[0] == '.') {
        //fullPath.erase(0, 1);
    //}
    //fullPath.erase(0, 1);
    std::ifstream mapFile(fullPath);
    if (!mapFile.is_open()) {
        return;
    }
    std::string line;
    int dictionaryType;
    bool spriteRead = false;
    int currentLine = 0;
    std::unordered_map<char, std::vector<std::string>> spriteDict;
    float tileWidth = 0;
    float tileHeight = 0;
    int originX = 0;
    int originY = 0;
    while (std::getline(mapFile, line)) {
        if (line.substr(0, 10) == "dictionary") {
            if (line.substr(11, 1) == "1") {
                dictionaryType = 1;
            } else {
                dictionaryType = 2;
            }
        } else if (line.substr(0, 3) == "map") {
            spriteRead = true;
            std::istringstream stream(line);
            std::vector<std::string> coords;
            std::string currentVar;
            while (stream >> currentVar) {
                coords.push_back(currentVar);
            }
            coords.pop_back();
            coords.pop_back();
            coords.pop_back();
            tileHeight = std::stof(coords.back());
            coords.pop_back();
            tileWidth = std::stof(coords.back());
            coords.pop_back();
            coords.pop_back();
            originY = std::stof(coords.back());
            coords.pop_back();
            originX = std::stof(coords.back());
        } else if (!spriteRead) {
            std::istringstream stream(line);
            std::vector<std::string> currentLine;
            std::string currentVar;
            while (stream >> currentVar) {
                currentLine.push_back(currentVar);
            }
            spriteDict.emplace(
                currentLine[0][0],
                std::vector<std::string>(currentLine.begin() + 1, currentLine.end()));
        } else if (spriteRead) {
            // load actual map
            int currentChar = 0;
            for (char sprite : line) {
                if (spriteDict.contains(sprite)) {
                    ECSEngine::EntityID id = engine.GetEntityManager().CreateEntity("MapPart");
                    ECSEngine::SpriteID spriteid = engine.GetSpriteManager().RegisterTexture(
                        folder + spriteDict[sprite][0],
                        ECSEngine::Rect(ECSEngine::Point2D(std::stof(spriteDict[sprite][1]), std::stof(spriteDict[sprite][2])),
                         std::stof(spriteDict[sprite][3]),
                         std::stof(spriteDict[sprite][4])));
                    ECSEngine::SpriteComponent spriteComponent;
                    spriteComponent.textureID = spriteid;
                    spriteComponent.bounds = {tileWidth, tileHeight};
                    spriteComponent.worldSpace = true;
                    engine.GetEntityManager().AddComponent<ECSEngine::SpriteComponent>(
                        id, spriteComponent);
                    ECSEngine::LocationComponent location;
                    location.loc = {originX + (tileWidth * currentChar),
                                    originY + (tileHeight * currentLine)};
                    engine.GetEntityManager().AddComponent<ECSEngine::LocationComponent>(id, location);
                    if (dictionaryType == 2) {
                        ECSEngine::CollisionComponent collision;
                        collision.collisionBox = { ECSEngine::Point2D(std::stof(spriteDict[sprite][5]), std::stof(spriteDict[sprite][6])),
                                                  std::stof(spriteDict[sprite][7]), std::stof(spriteDict[sprite][8]) };
                        collision.staticEntity = 1;
                        engine.GetEntityManager().AddComponent<ECSEngine::CollisionComponent>(
                            id, collision);
                    }
                }
                currentChar++;
            }
            currentLine++;
        }
    }
    mapFile.close();
}

int main(int argc, char* argv[]) {
    if (argc >= 3 && argv[1] == std::string("-path")) {
        gResourcePath = argv[2];
    }
    std::cout << "Usage: " << argv[0] << " [-path resource_path]\n";
    std::cout << "Using resource path: " << gResourcePath << "\n";

    // Engine test code
    Engine engine(1024, 768, "Test Engine: Platformer.exe");

    // Load sounds

    // Load maps
    LoadMap(engine, gResourcePath, "sky.map");
    LoadMap(engine, gResourcePath, "world.map");

    // Place player

    // Setup score component

    // Setup Camera

    // Add all systems
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
