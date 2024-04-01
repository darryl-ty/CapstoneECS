#pragma once
#ifndef SYSTEMS_H
#define SYSTEMS_H
#include "entitymanager.h"
#include "componentmanager.h"

static int START_YEAR;

struct KingdomSystem{
    static void establishKingdoms(EntityManager* entityManager, ComponentManager* componentManager){
        for (auto& entity : componentManager->getEntities<KingdomComponent>()){
           std::cout << "Year " << START_YEAR << ": The "
           << KingdomAdjectives::getAdjective(entityManager->getComponent<KingdomComponent>(entity).aggressionLevel)
           << " " << entityManager->getComponent<KingdomComponent>(entity).kingdomName
           << " was established. May the sovereign "
           << entityManager->getComponent<CharacterComponent>(entityManager->getComponent<KingdomComponent>(entity).rulerID).name
           << " reign supreme for eons!" << std::endl;
        }
    }
};

#endif
