#include "WindowManager.h"

namespace ECSEngine {

WindowManager::WindowManager(unsigned int width, unsigned int height, const std::string& title)
    : mWindow(std::make_shared<sf::RenderWindow>(sf::VideoMode({width, height}), title)), mWindowSize(width, height) {}

std::shared_ptr<sf::RenderWindow> WindowManager::GetWindow() const { return mWindow; }

Rect WindowManager::GetWindowBounds() const {
    return Rect({0, 0}, mWindowSize.x, mWindowSize.y);
}

void WindowManager::DebugDrawRect(const Rect& r, int red, int green, int blue, int alpha) {}

void WindowManager::DebugFrameRect(const Rect& r, int red, int green, int blue, int alpha) {}

Rect WindowManager::WindowToWorld(const Rect& rect) const { 
    return Rect(WindowToWorld(rect.topLeft), rect.width * mWorldUnitsPerPixel,
                rect.height * mWorldUnitsPerPixel);
}

Point2D WindowManager::WindowToWorld(const Point2D& pt) const { 
    sf::Vector2f viewCenterWindow = sf::Vector2f(mViewCenterWorld.x, mViewCenterWorld.y) / mWorldUnitsPerPixel;
    sf::Vector2f originViewCenter = mWindowSize * 0.5f;
    sf::Vector2f movementDiff = viewCenterWindow - originViewCenter;
    sf::Vector2f result = (sf::Vector2f(pt.x, pt.y) + movementDiff) * mWorldUnitsPerPixel;
    return Point2D(result.x, result.y); 
}

Rect WindowManager::WorldToWindow(const Rect& rect) const { 
    return Rect(WorldToWindow(rect.topLeft), rect.width / mWorldUnitsPerPixel, 
        rect.height / mWorldUnitsPerPixel); 
}

Point2D WindowManager::WorldToWindow(const Point2D& pt) const { 
    sf::Vector2f viewCenterWindow = sf::Vector2f(mViewCenterWorld.x, mViewCenterWorld.y) / mWorldUnitsPerPixel;
    sf::Vector2f originViewCenter = mWindowSize * 0.5f;
    sf::Vector2f movementDiff = viewCenterWindow - originViewCenter;
    sf::Vector2f result = sf::Vector2f(pt.x, pt.y) / mWorldUnitsPerPixel - movementDiff;

    return Point2D(result.x, result.y); 
}

void WindowManager::SetCamera(const Point2D& worldPt, const Point2D& screenPt) {
    sf::Vector2f differenceWorld =
        (sf::Vector2f(screenPt.x, screenPt.y) - (mWindowSize * 0.5f)) * mWorldUnitsPerPixel;

    mViewCenterWorld = Point2D(worldPt.x - differenceWorld.x, worldPt.y - differenceWorld.y);
}

Point2D WindowManager::GetWorldCenter() const { return mViewCenterWorld; }

float WindowManager::GetWorldScale() const { return mWorldUnitsPerPixel; }

void WindowManager::SetWorldCenter(const Point2D& worldCenter) { mViewCenterWorld = worldCenter; }

void WindowManager::SetWorldScale(float worldUnitsPerPixel) {
    mWorldUnitsPerPixel = worldUnitsPerPixel;
}

void WindowManager::SetWorldRect(const Rect& worldCameraRect) {}

float WindowManager::WindowToWorld(float x) const { return 0; }

float WindowManager::WorldToWindow(float x) const { return 0; }

}  // namespace ECSEngine
