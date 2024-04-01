#pragma once
#ifndef SYSTEMS_H
#define SYSTEMS_H
#include "entitymanager.h"
#include "componentmanager.h"

extern int START_YEAR;

struct KingdomSystem{
    static void establishKingdoms(ComponentManager* componentManager){
        for (auto& entity : componentManager->getEntities<KingdomComponent>()){
           std::cout << "Year " << START_YEAR << ": The ";
        }
    }
};

#endif
