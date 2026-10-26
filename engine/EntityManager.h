#pragma once

#include <cassert>
#include <iostream>
#include <limits>
#include <type_traits>

#include "Component.h"
#include "Entity.h"

namespace ECSEngine {

template <typename... Components>
class EntityManager {
public:
    static constexpr size_t kNullComponent = std::numeric_limits<size_t>::max();
    [[nodiscard]] EntityID CreateEntity(const std::string& name) { 
        EntityID id = mEntities.size();
        mEntities.push_back(Entity(id, name));
        return id;
    }

    void RemoveEntity(EntityID entity) {}

    template <typename T>
    void AddComponent(EntityID entity, T component) {
        auto componentIndex = std::find(mComponents[entity].mStorage.begin(),
                                        mComponents[entity].mStorage.end(), component);
        if (componentIndex != mComponents.end()) {
            
        }
    }

    template <typename T>
    void RemoveComponent(EntityID entity) {}

    template <typename T>
    bool HasComponent(EntityID entity) const {
        return false;
    }

    template <typename T>
    T& GetComponent(EntityID entity) {
        // Re-write code
        static T dummy;
        return dummy;
    }

    template <typename T>
    ComponentStorage<T>& GetComponentStorage() {
        // Re-write code
        static ComponentStorage<T> dummy;
        return dummy;
    }

    using tEntity = std::vector<Entity>;
    using const_iterator = tEntity::const_iterator;
    const_iterator cbegin() const { return mEntities.begin(); }
    const_iterator cend() const { return mEntities.end(); }
    const_iterator begin() { return mEntities.begin(); }
    const_iterator end() { return mEntities.end(); }

private:
    std::tuple<ComponentStorage<Components>...> mComponents;
    std::vector<Entity> mEntities;
    std::vector<std::array<size_t, sizeof...(Components)>> mComponentIndices;
    std::vector<size_t> mFreeEntities;
};

}  // namespace ECSEngine
