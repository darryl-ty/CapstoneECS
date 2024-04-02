#pragma once
#ifndef SYSTEMS_H
#define SYSTEMS_H
#include <sstream>
#include <mutex>
#include "entitymanager.h"
#include "componentmanager.h"

static int START_YEAR;
std::mutex FILE_MUTEX;

struct KingdomSystem{
    static void establishKingdoms(EntityManager* entityManager, ComponentManager* componentManager){
        for (auto& entity : componentManager->getEntities<KingdomComponent>()){
           std::cout << "Year " << START_YEAR << ": The "
           << KingdomAdjectives::getAdjective(entityManager->getComponent<KingdomComponent>(entity).aggressionLevel)
           << " " << entityManager->getComponent<KingdomComponent>(entity).kingdomName<< " was established. May the sovereign "
           << getKingdomRulerName(entityManager, entity) << " reign supreme for eons!" << std::endl;
        }
    }
    static void kingdomProgress(EntityManager* entityManager, ComponentManager* componentManager){
        for (auto& entity : componentManager->getEntities<KingdomComponent>()){
            std::lock_guard<std::mutex> lock(FILE_MUTEX);

        }
    }

private:
    static std::string getKingdomRulerName(EntityManager* entityManager, ID entity){
        return entityManager->getComponent<CharacterComponent>(entityManager->getComponent<KingdomComponent>(entity).rulerID).name;
    }
};

struct CharacterSystem{

};

struct WarSystem{

};

#endif
