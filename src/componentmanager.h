#pragma once
#ifndef COMPONENTMANAGER_H
#define COMPONENTMANAGER_H
#include <iostream>
#include "components.h"
#include "entity.h"

class ComponentManager {

    template<class T>
    std::set<ID>& entities();
    template<class T>
    [[nodiscard]]
    const std::set<ID>& entities() const;

    std::set<ID> m_locationEntities;
    std::set<ID> m_kingdomEntities;
    std::set<ID> m_rulerEntities;
    std::set<ID> m_warEntities;
    std::set<ID> m_characterEntities;

public:

    template<class T>
    void addEntity(ID entity){
        entities<T>().insert(entity);
    }

    template<class T>
    [[nodiscard]]
    const std::set<ID>& getEntities() const {
        return entities<T>();
    }

};

template<>
std::set<ID>& ComponentManager::entities<LocationComponent>() {
    return m_locationEntities;
}
template<>
std::set<ID>& ComponentManager::entities<KingdomComponent>() {
    return m_kingdomEntities;
}
template<>
std::set<ID>& ComponentManager::entities<RulerComponent>() {
    return m_rulerEntities;
}
template<>
std::set<ID>& ComponentManager::entities<WarComponent>() {
    return m_warEntities;
}
template<>
std::set<ID>& ComponentManager::entities<CharacterComponent>() {
    return m_characterEntities;
}

template<>
const std::set<ID>& ComponentManager::entities<LocationComponent>() const {
    return m_locationEntities;
}
template<>
const std::set<ID>& ComponentManager::entities<KingdomComponent>() const {
    return m_kingdomEntities;
}
template<>
const std::set<ID>& ComponentManager::entities<RulerComponent>() const {
    return m_rulerEntities;
}
template<>
const std::set<ID>& ComponentManager::entities<WarComponent>() const {
    return m_warEntities;
}
template<>
const std::set<ID>& ComponentManager::entities<CharacterComponent>() const {
    return m_characterEntities;
}

#endif
