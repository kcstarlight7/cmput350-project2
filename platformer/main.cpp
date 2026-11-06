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

//std::string gResourcePath = "../assets/";
std::string gResourcePath = "..\\..\\assets\\";
#define COMPONENTS                                                                             \
    ECSEngine::LocationComponent, ECSEngine::MovementComponent, ECSEngine::CollisionComponent, \
        ECSEngine::SpriteComponent, ECSEngine::SpawnComponent, ECSEngine::CameraComponent,     \
        ECSEngine::CameraFollower, ECSEngine::InputComponent, ECSEngine::GravityComponent,     \
        ECSEngine::CameraShake, ECSEngine::ScoreComponent, ECSEngine::RespawnComponent
using Engine = ECSEngine::ECSEngine<COMPONENTS>;

static void LoadMap(Engine& engine, const std::string& path, const std::string& file) {
    std::filesystem::path fullPath = std::filesystem::path(gResourcePath) / file;
    //std::string fullPath = path + file;
    //while (fullPath[0] == '.') {
        //fullPath.erase(0, 1);
    //}
    //fullPath.erase(0, 1);
    std::cout << "Path: " << fullPath << "\n";
    std::cout << "Current path: " << std::filesystem::current_path() << "\n";
    std::ifstream mapFile(fullPath);
    if (!mapFile.is_open()) {
        std::cout << "Failed to open map!\n";
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
    int mapGridHeight = 0;
    int mapGridWidth = 0;
    while (std::getline(mapFile, line)) {
        //std::cout << "Current line: " << line << "\n";
        if (line.substr(0, 10) == "dictionary") {
            if (line.substr(11, 1) == "1") {
                dictionaryType = 1;
            } else {
                dictionaryType = 2;
            }

            //std::cout << "Read: dictionary type: " << dictionaryType << "\n";

        } else if (line.substr(0, 3) == "map") {
            spriteRead = true;
            std::istringstream stream(line);
            std::vector<std::string> coords;
            std::string currentVar;
            while (stream >> currentVar) {
                coords.push_back(currentVar);
            }
            mapGridHeight = std::stoi(coords.back());
            coords.pop_back(); //size height
            mapGridWidth = std::stoi(coords.back());
            coords.pop_back(); //size width
            coords.pop_back(); //"size"
            tileHeight = std::stoi(coords.back());
            coords.pop_back(); //pops tile height
            tileWidth = std::stoi(coords.back());
            coords.pop_back(); //pops tile width
            coords.pop_back(); //"tile"
            originY = std::stoi(coords.back()); 
            coords.pop_back(); //pops map origin y
            originX = std::stoi(coords.back());

            //std::cout << "Read: map \n"
            //          << "map grid height: " << mapGridHeight << "\n"
            //          << "map grid width: " << mapGridWidth << "\n"
            //          << "tile height: " << tileHeight << "\n"
            //          << "tile width: " << tileWidth << "\n"
            //          << "originY: " << originY << "\n"
            //          << "originX: " << originX << "\n";
        } else if (!spriteRead) {
            std::istringstream stream(line);
            std::vector<std::string> currentLineContent;
            std::string currentVar;
            while (stream >> currentVar) {
                currentLineContent.push_back(currentVar);
            }
            spriteDict.emplace(
                currentLineContent[0][0],
                std::vector<std::string>(currentLineContent.begin() + 1, currentLineContent.end()));


        } else if (spriteRead) {
            // load actual map
            int currentChar = 0;
            for (char sprite : line) {
                if (spriteDict.contains(sprite)) {
                    ECSEngine::LocationComponent location;
                    location.loc = {originX + (tileWidth * currentChar),
                                    originY + (tileHeight * currentLine)};
                    ECSEngine::EntityID id = engine.GetEntityManager().CreateEntity("MapPart");
                    ECSEngine::Rect boundRect(ECSEngine::Point2D(std::stof(spriteDict[sprite][1]), std::stof(spriteDict[sprite][2])),
                         std::stof(spriteDict[sprite][3]),
                         std::stof(spriteDict[sprite][4]));
                    //std::cout << "Registered sprite texture.\n"
                    //          << "path: " << gResourcePath + spriteDict[sprite][0] << "\n"
                    //          << "tl: "
                    //          << ECSEngine::Point2D(std::stof(spriteDict[sprite][1]),
                    //                                std::stof(spriteDict[sprite][2]))
                    //          << "\n"
                    //          << "width: " << std::stof(spriteDict[sprite][3]) << "\n"
                    //          << "height: " << std::stof(spriteDict[sprite][4]) << "\n";

                    ECSEngine::SpriteID spriteid = engine.GetSpriteManager().RegisterTexture(
                        gResourcePath + spriteDict[sprite][0], boundRect);

                    ECSEngine::SpriteComponent spriteComponent;
                    spriteComponent.textureID = spriteid;
                    spriteComponent.bounds = {tileWidth, tileHeight};
                    spriteComponent.worldSpace = true;
                    
                    engine.GetEntityManager().AddComponent<ECSEngine::SpriteComponent>(
                        id, spriteComponent);

                    //std::cout << "Location: " << location.loc.x << ", " << location.loc.y << "\n";
                    //std::cout << "cur char: " << currentChar << ", " << currentLine << "\n"; 
                    engine.GetEntityManager().AddComponent<ECSEngine::LocationComponent>(id, location);
                    if (dictionaryType == 2) {
                        ECSEngine::CollisionComponent collision;
                        collision.collisionBox = { ECSEngine::Point2D(std::stof(spriteDict[sprite][5]), std::stof(spriteDict[sprite][6])),
                                                  std::stof(spriteDict[sprite][7]), std::stof(spriteDict[sprite][8]) };
                        collision.staticEntity = 1;
                        engine.GetEntityManager().AddComponent<ECSEngine::CollisionComponent>(
                            id, collision);
                    }

                    if (sprite == 'S') {
                        ECSEngine::SpawnComponent spawn;
                        spawn.entityID = id;
                        spawn.spawnType = "Star";
                        spawn.timeBetweenSpawns = 600;
                        spawn.timeToNextSpawn = 0;
                        spawn.totalSpawns = 5;
                        engine.GetEntityManager().AddComponent<ECSEngine::SpawnComponent>(id, spawn);
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
    engine.GetSoundManager().RegisterSound(gResourcePath + "error_008.ogg", "error");
    engine.GetSoundManager().RegisterSound(gResourcePath + "footstep_grass_003.ogg", "grass_footstep");
    engine.GetSoundManager().RegisterSound(gResourcePath + "footstep_snow_001.ogg", "snow_footstep");
    engine.GetSoundManager().RegisterSound(gResourcePath + "laserLarge_000.ogg", "laser");
    engine.GetSoundManager().RegisterSound(gResourcePath + "sfx_gem.ogg", "sfx_gem");
    engine.GetSoundManager().RegisterSound(gResourcePath + "sfx_jump-high.ogg", "high_jump");
    engine.GetSoundManager().RegisterSound(gResourcePath + "sfx_jump.ogg", "jump");

    // Load maps
    LoadMap(engine, gResourcePath, "sky.map");
    LoadMap(engine, gResourcePath, "world.map");

    // Place player
    ECSEngine::EntityID player = engine.GetEntityManager().CreateEntity("player");


    // player comps
    ECSEngine::SpriteComponent pSpriteComp;
    pSpriteComp.textureID = engine.GetSpriteManager().RegisterTexture(
        gResourcePath + "spritesheet-characters-default.png",
        ECSEngine::Rect(ECSEngine::Point2D(128 * 0, 128 * 5), 128, 128));
    pSpriteComp.bounds = {64, 64};
    pSpriteComp.worldSpace = true;


    ECSEngine::CollisionComponent pCollisionComp;
    pCollisionComp.collisionBox = ECSEngine::Rect(ECSEngine::Point2D(0, 0), 64, 64);

    ECSEngine::CameraComponent pCameraComp;
    sf::Vector2u windowSize = engine.GetWindowManager().GetWindow()->getSize();
    float softExc = 0.3f;
    float hardExc = 0.1f;
    pCameraComp.position = ECSEngine::Point2D(800, 0);
    pCameraComp.softExclusion = ECSEngine::Rect(ECSEngine::Point2D(windowSize.x * softExc, 0.f),
                                                windowSize.x * (softExc * 2), windowSize.y);
    pCameraComp.hardExclusion = ECSEngine::Rect(ECSEngine::Point2D(windowSize.x * hardExc, 0.f),
                                                windowSize.x * (hardExc * 2), windowSize.y);
    pCameraComp.worldUnitsPerPixel = 1;
    pCameraComp.speed = 1;
    
    ECSEngine::CameraFollower pCameraFollower;
    pCameraFollower.entityID = player;

    ECSEngine::InputComponent pInputComp(player);
    ECSEngine::LocationComponent pLocationComp;
    pLocationComp.loc = ECSEngine::Point2D(1024 * 0.7f, 768 * 0.5f);

    ECSEngine::MovementComponent pMovementComp;
    pMovementComp.velocity = 1;
    ECSEngine::GravityComponent pGravityComp(ECSEngine::Point2D(1.0f, 1.0f));

    engine.GetEntityManager().AddComponent(player, pLocationComp);
    engine.GetEntityManager().AddComponent(player, pSpriteComp);
    engine.GetEntityManager().AddComponent(player, pCollisionComp);

    engine.GetEntityManager().AddComponent(player, pCameraFollower); //dies
    
    engine.GetEntityManager().AddComponent(player, pGravityComp);
    engine.GetEntityManager().AddComponent(player, pMovementComp);
    engine.GetEntityManager().AddComponent(player, pInputComp);
    


    // Setup score component
    ECSEngine::EntityID score1 = engine.GetEntityManager().CreateEntity("score_1");
    ECSEngine::EntityID score2 = engine.GetEntityManager().CreateEntity("score_2");
    ECSEngine::EntityID score3 = engine.GetEntityManager().CreateEntity("score_3");
    ECSEngine::ScoreComponent scoreComp;
    std::cout << "setting digitID" << "\n";
    scoreComp.digitID[0] = score1;
    scoreComp.digitID[1] = score2;
    scoreComp.digitID[2] = score3;
    std::cout << "registering texture" << "\n";
    scoreComp.digitTextureID[9] = engine.GetSpriteManager().RegisterTexture(
        gResourcePath + "spritesheet-tiles-default.png", 
        ECSEngine::Rect(ECSEngine::Point2D(832, 64*4), 64, 64)); 
    scoreComp.digitTextureID[8] = engine.GetSpriteManager().RegisterTexture(
        gResourcePath + "spritesheet-tiles-default.png",
        ECSEngine::Rect(ECSEngine::Point2D(832, 64*5), 64, 64)); 
    scoreComp.digitTextureID[7] = engine.GetSpriteManager().RegisterTexture(
        gResourcePath + "spritesheet-tiles-default.png",
        ECSEngine::Rect(ECSEngine::Point2D(832, 64*6), 64, 64)); 
    scoreComp.digitTextureID[6] = engine.GetSpriteManager().RegisterTexture(
        gResourcePath + "spritesheet-tiles-default.png",
        ECSEngine::Rect(ECSEngine::Point2D(832, 64*7), 64, 64)); 
    scoreComp.digitTextureID[5] = engine.GetSpriteManager().RegisterTexture(
        gResourcePath + "spritesheet-tiles-default.png",
        ECSEngine::Rect(ECSEngine::Point2D(832, 64 * 8), 64, 64)); 
    scoreComp.digitTextureID[4] = engine.GetSpriteManager().RegisterTexture(
        gResourcePath + "spritesheet-tiles-default.png",
        ECSEngine::Rect(ECSEngine::Point2D(832, 64 * 9), 64, 64)); 
    scoreComp.digitTextureID[3] = engine.GetSpriteManager().RegisterTexture(
        gResourcePath + "spritesheet-tiles-default.png",
        ECSEngine::Rect(ECSEngine::Point2D(832, 64 * 10), 64, 64)); 
    scoreComp.digitTextureID[2] = engine.GetSpriteManager().RegisterTexture(
        gResourcePath + "spritesheet-tiles-default.png",
        ECSEngine::Rect(ECSEngine::Point2D(832, 64 * 11), 64, 64)); 
    scoreComp.digitTextureID[1] = engine.GetSpriteManager().RegisterTexture(
        gResourcePath + "spritesheet-tiles-default.png",
        ECSEngine::Rect(ECSEngine::Point2D(832, 64 * 12), 64, 64)); 
    scoreComp.digitTextureID[0] = engine.GetSpriteManager().RegisterTexture(
        gResourcePath + "spritesheet-tiles-default.png",
        ECSEngine::Rect(ECSEngine::Point2D(832, 64 * 13), 64, 64)); 

    ECSEngine::SpriteComponent scoreSpriteComp1;
    scoreSpriteComp1.textureID = scoreComp.digitTextureID[0];
    scoreSpriteComp1.bounds = {64, 64};
    scoreSpriteComp1.worldSpace = false;
    engine.GetEntityManager().AddComponent(score1, scoreSpriteComp1);

    ECSEngine::SpriteComponent scoreSpriteComp2;
    scoreSpriteComp2.textureID = scoreComp.digitTextureID[0];
    scoreSpriteComp2.bounds = {64, 64};
    scoreSpriteComp2.worldSpace = false;
    engine.GetEntityManager().AddComponent(score2, scoreSpriteComp2);

    ECSEngine::SpriteComponent scoreSpriteComp3;
    scoreSpriteComp3.textureID = scoreComp.digitTextureID[0];
    scoreSpriteComp3.bounds = {64, 64};
    scoreSpriteComp3.worldSpace = false;
    engine.GetEntityManager().AddComponent(score3, scoreSpriteComp3);

    std::cout << "setting locations" << "\n";
    scoreComp.score = 0;
    ECSEngine::LocationComponent score1Loc;
    score1Loc.loc = ECSEngine::Point2D(0, 0);
    ECSEngine::LocationComponent score2Loc;
    score2Loc.loc = ECSEngine::Point2D(50, 0);
    ECSEngine::LocationComponent score3Loc;
    score3Loc.loc = ECSEngine::Point2D(100, 0);
    std::cout << "adding components" << "\n";

    engine.GetEntityManager().AddComponent(score1, score1Loc);
    engine.GetEntityManager().AddComponent(score2, score2Loc);
    engine.GetEntityManager().AddComponent(score3, score3Loc);

    engine.GetEntityManager().AddComponent(score1, scoreComp);
    engine.GetEntityManager().AddComponent(score2, scoreComp);
    engine.GetEntityManager().AddComponent(score3, scoreComp);

    // Setup Camera
    ECSEngine::EntityID camera = engine.GetEntityManager().CreateEntity("Camera");
    engine.GetEntityManager().AddComponent(camera, pCameraComp);
    //engine.GetWindowManager().SetCamera(
    //    ECSEngine::Point2D((float)windowSize.x / 2, (float)windowSize.y / 2),
    //    ECSEngine::Point2D((float)windowSize.x / 2, (float)windowSize.y / 2));

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
