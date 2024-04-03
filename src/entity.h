#pragma once
#ifndef ENTITY_H
#define ENTITY_H
#include <functional>
#include <iostream>
#include <cassert>
#include <set>

using ID = uint32_t;
static const uint32_t MAX_POSSIBLE_ENTITIES = 100;
static std::set<ID> ENTITY_SET;

class Entity{

    static inline uint32_t ID_NUM = 0;
    ID m_entityID;

public:
    Entity() : m_entityID(ID_NUM++){
        assert(ID_NUM <= MAX_POSSIBLE_ENTITIES && "Too many entities.");
        ENTITY_SET.emplace(m_entityID);
    }
    ~Entity()= default;

    [[nodiscard]]
    ID getID() const{
        return m_entityID;
    }
};

namespace std {
    template<> struct hash<Entity> {
        std::size_t operator()(const Entity& entity) const noexcept {
            return std::hash<ID>()(entity.getID());
        }
    };
}

#endif
