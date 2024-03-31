#pragma once
#ifndef SYSTEMS_H
#define SYSTEMS_H
#include "entitymanager.h"

struct KingdomSystem{
    static void update(EntityManager& entityManager){
        for (auto& entity : ENTITY_SET){
            if (entityManager.getComponent<KingdomComponent>(entity).aggressionLevel == KingdomComponent::AggressionLevel::Evil){
                std::cout << "Entity " << entity << " is evil!" << std::endl;
            }
        }
    }
};

#endif
