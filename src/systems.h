#pragma once
#ifndef SYSTEMS_H
#define SYSTEMS_H
#include <sstream>
#include <fstream>
#include <mutex>
#include <random>
#include "entitymanager.h"
#include "componentmanager.h"

static int START_YEAR;
std::mutex FILE_MUTEX;

struct KingdomSystem{
    static void establishKingdoms(EntityManager* entityManager, ComponentManager* componentManager){
        for (auto& entity : componentManager->getEntities<KingdomComponent>()){
           std::cout << "Year " << START_YEAR << ": The "
           << getKingdomAdjective(entityManager, entity) << " "
           << getKingdomName(entityManager, entity) << " was established. May the sovereign "
           << getKingdomRulerName(entityManager, entity) << " reign supreme for eons!" << std::endl;
        }
    }
    static void kingdomProgress(EntityManager* entityManager, ComponentManager* componentManager){
        std::ostringstream oss;
        std::random_device device;
        std::mt19937 rng(device());
        
        std::uniform_int_distribution<int> researchDist(-10, 10);
        for (auto& entity : componentManager->getEntities<KingdomComponent>()){
            int researchChange = researchDist(rng);
            std::lock_guard<std::mutex> lock(FILE_MUTEX);
            oss << "Year: " << START_YEAR;
            if (researchChange <= -5) {
                oss << "The " << getKingdomAdjective(entityManager, entity) << getKingdomName(entityManager, entity)
                << " research initiatives have proven fruitless, leading to the demise of valuable findings. "
                   "They must now reassess their approach and strive to prevent similar losses in the future." << std::endl;

                entityManager->getComponent<KingdomComponent>(entity).kingdomTechLevel-=10;
                entityManager->getComponent<KingdomComponent>(entity).kingdomStrength-=10;
            } else if (researchChange <= 0){
                oss << "The " << getKingdomAdjective(entityManager, entity) << getKingdomName(entityManager, entity)
                    << " research endeavors have yielded no tangible progress, leaving its knowledge base stagnant. "
                       "It's imperative for them to recalibrate their strategies and embark on a more fruitful path forward." << std::endl;

            } else if (researchChange <= 8){
                oss << "The " << getKingdomAdjective(entityManager, entity) << getKingdomName(entityManager, entity)
                    << " made considerable progress in their research efforts this year. With optimism and dedication, "
                       "they look forward to leveraging these accomplishments as a springboard for even greater achievements ahead." << std::endl;

                entityManager->getComponent<KingdomComponent>(entity).kingdomTechLevel+=2;
            } else{
                oss << "The " << getKingdomAdjective(entityManager, entity) << getKingdomName(entityManager, entity)
                    << " announces a monumental breakthrough in research, particularly in areas with direct military implications. "
                       "This remarkable advancement underscores their commitment to innovation and reinforces their strategic prowess in safeguarding our realm." << std::endl;

                entityManager->getComponent<KingdomComponent>(entity).kingdomTechLevel+=10;
                entityManager->getComponent<KingdomComponent>(entity).kingdomStrength+=10;
            }

        }
    }

private:
    static std::string getKingdomRulerName(EntityManager* entityManager, ID entity){
        return entityManager->getComponent<CharacterComponent>(entityManager->getComponent<KingdomComponent>(entity).rulerID).name;
    }
    static std::string getKingdomAdjective(EntityManager* entityManager, ID entity){
        return KingdomAdjectives::getAdjective(entityManager->getComponent<KingdomComponent>(entity).aggressionLevel);
    }
    static std::string getKingdomName(EntityManager* entityManager, ID entity){
        return entityManager->getComponent<KingdomComponent>(entity).kingdomName;
    }
};

struct CharacterSystem{
    static void characterActions(EntityManager* entityManager, ComponentManager* componentManager){
        std::ostringstream oss;
        for (auto& entity : componentManager->getEntities<CharacterComponent>()){
            std::lock_guard<std::mutex> lock(FILE_MUTEX);
        }
    }

private:

};

struct WarSystem{

};

#endif
