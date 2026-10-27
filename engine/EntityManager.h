#pragma once

#include <cassert>
#include <iostream>
#include <limits>
#include <type_traits>
#include <vector>

#include "Component.h"
#include "Entity.h"

namespace ECSEngine {

template <typename... Components>
class EntityManager {
public:
    static constexpr size_t kNullComponent = std::numeric_limits<size_t>::max();
    [[nodiscard]] EntityID CreateEntity(const std::string& name) { 
        EntityID id;
        if (!mFreeEntities.empty()) { //if there are entities that are free, reuse free slot
            id = mFreeEntities.back();
            mFreeEntities.pop_back();
            mEntities[id] = Entity(id, name);
            mComponentIndices[id].fill(kNullComponent);
            mEntityValid[id] = true;
        } else {
            id = mEntities.size();
            mEntities.push_back(Entity(id, name));
            std::array <size_t, sizeof...(Components)> indices;
            indices.fill(kNullComponent);
            mComponentIndices.push_back(indices);
            mEntityValid.push_back(true);
        }
        return id;
    }

    void RemoveEntity(EntityID entity) { 
        if (!(entity < mEntityValid.size()) || !(mEntityValid[entity])) {
            return;
        }
        (RemoveComponent<Components>(entity), ...);
        mEntityValid[entity] = false;
        mFreeEntities.push_back(entity);
    }

    template <typename T>
    void AddComponent(EntityID entity, T component) {
        auto& storage = GetComponentStorage<T>(); //get storage of this component type

        std::array<bool, sizeof...(Components)> matches = {std::is_same_v<T, Components>...};
        size_t typeIndex = kNullComponent;
        for (size_t i = 0; i < matches.size(); i++) {
            if (matches[i]) {
                typeIndex = i; //find component type's index in this entity's components
                break;
            }
        }
        if (typeIndex == kNullComponent) {
            return;
        }
        size_t& componentIndex = mComponentIndices[entity][typeIndex]; //get entity's index for component type

        if (componentIndex != kNullComponent) { //replace if component exists
            storage.mStorage[componentIndex] = component;
            storage.mValid[componentIndex] = true;
            return;
        }

        if (!storage.mFreeList.empty()) { //reuse freed component slot
            componentIndex = storage.mFreeList.back();
            storage.mFreeList.pop_back();
            storage.mStorage[componentIndex] = component;
            storage.mValid[componentIndex] = true;
        } else { //add new component slot
            componentIndex = storage.mStorage.size();
            storage.mStorage.push_back(component);
            storage.mValid.push_back(true);
        }
    }

    template <typename T>
    void RemoveComponent(EntityID entity) {
        auto& storage = GetComponentStorage<T>();
        std::array<bool, sizeof...(Components)> matches = {std::is_same_v<T, Components>...};
        size_t typeIndex = kNullComponent;
        for (size_t i = 0; i < matches.size(); i++) {
            if (matches[i]) {
                typeIndex = i;
                break;
            }
        }
        size_t& componentIndex = mComponentIndices[entity][typeIndex];

        if (componentIndex == kNullComponent) { //if component is already null
            return;
        }
        storage.mValid[componentIndex] = false; //mark not valid
        storage.mFreeList.push_back(componentIndex); //mark as free
        componentIndex = kNullComponent; //mark component null
    }

    template <typename T>
    bool HasComponent(EntityID entity) const {
        auto& storage = GetComponentStorage<T>();
        std::array<bool, sizeof...(Components)> matches = {std::is_same_v<T, Components>...};
        size_t typeIndex = kNullComponent;
        for (size_t i = 0; i < matches.size(); i++) {
            if (matches[i]) {
                typeIndex = i;
                break;
            }
        }
        size_t& componentIndex = mComponentIndices[entity][typeIndex];

        return componentIndex != kNullComponent;
    }

    template <typename T>
    T& GetComponent(EntityID entity) {
        auto& storage = GetComponentStorage<T>();
        std::array<bool, sizeof...(Components)> matches = {std::is_same_v<T, Components>...};
        size_t typeIndex = kNullComponent;
        for (size_t i = 0; i < matches.size(); i++) {
            if (matches[i]) {
                typeIndex = i;
            }
        }
        size_t& componentIndex = mComponentIndices[entity][typeIndex];

        return storage.mStorage[componentIndex];
    }

    template <typename T>
    ComponentStorage<T>& GetComponentStorage() {
        return std::get<ComponentStorage<T>>(mComponents);
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
    std::vector<bool> mEntityValid;
};

}  // namespace ECSEngine
