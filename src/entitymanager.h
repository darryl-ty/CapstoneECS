#pragma once
#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include <unordered_map>
#include <iostream>
#include "components.h"
#include "entity.h"

class EntityManager {
    template<class T>
    std::unordered_map<ID, T>& components();

    std::unordered_map<ID, LocationComponent> m_locationComponent;
    std::unordered_map<ID, KingdomComponent> m_kingdomComponent;
    std::unordered_map<ID, RulerComponent> m_rulerComponent;
    std::unordered_map<ID, WarComponent> m_warComponent;
    std::unordered_map<ID, CharacterComponent> m_characterComponent;

public:
    template<class T>
    void addComponent(ID entity, T component){
        components<T>()[entity] = component;
    }

    template<class T>
    T& getComponent(ID entity){
        return components<T>()[entity];
    }
};
template<>
std::unordered_map<ID, LocationComponent>& EntityManager::components<LocationComponent>() {
    return m_locationComponent;
}
template<>
std::unordered_map<ID, KingdomComponent>& EntityManager::components<KingdomComponent>() {
    return m_kingdomComponent;
}
template<>
std::unordered_map<ID, RulerComponent>& EntityManager::components<RulerComponent>() {
    return m_rulerComponent;
}
template<>
std::unordered_map<ID, WarComponent>& EntityManager::components<WarComponent>() {
    return m_warComponent;
}
template<>
std::unordered_map<ID, CharacterComponent>& EntityManager::components<CharacterComponent>() {
    return m_characterComponent;
}
#endif
