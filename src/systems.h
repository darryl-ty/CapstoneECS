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
static int YEAR;
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
        std::random_device device;
        std::mt19937 rng(device());
        
        std::uniform_int_distribution<int> researchDist(-10, 10);
        for (auto& entity : componentManager->getEntities<KingdomComponent>()){
            std::ostringstream oss;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::lock_guard<std::mutex> lock(FILE_MUTEX);
            int researchChange = researchDist(rng);

            oss << "Year " << YEAR << ": ";
            if (entityManager->getComponent<WarComponent>(entity).defender == entity || entityManager->getComponent<WarComponent>(entity).initiator == entity){
                oss << "The " << getKingdomAdjective(entityManager, entity) << " " << getKingdomName(entityManager, entity)
                << " was too busy " << ((entityManager->getComponent<WarComponent>(entity).defender == entity)
                ? "defending their realm to make any significant research progress this year."
                : "attacking realms to make any significant research progress this year.") << std::endl;
                std::cout << oss.str();

                continue;
            }

            if (researchChange < -5) {
                oss << "The " << getKingdomAdjective(entityManager, entity) << " " << getKingdomName(entityManager, entity)
                << " research initiatives have proven fruitless, leading to the demise of valuable findings. "
                   "They must now reassess their approach and strive to prevent similar losses in the future." << std::endl;
                std::cout << oss.str();

                entityManager->getComponent<KingdomComponent>(entity).kingdomTechLevel-=10;
                entityManager->getComponent<KingdomComponent>(entity).kingdomStrength-=10;
            } else if (researchChange < 0){
                oss << "The " << getKingdomAdjective(entityManager, entity) << " " << getKingdomName(entityManager, entity)
                    << " research endeavors have yielded no tangible progress, leaving its knowledge base stagnant. "
                       "It's imperative for them to recalibrate their strategies and embark on a more fruitful path forward." << std::endl;
                std::cout << oss.str();

            } else if (researchChange < 9){
                oss << "The " << getKingdomAdjective(entityManager, entity) << " " << getKingdomName(entityManager, entity)
                    << " made considerable progress in their research efforts this year. With optimism and dedication, "
                       "they look forward to leveraging these accomplishments as a springboard for even greater achievements ahead." << std::endl;
                std::cout << oss.str();

                entityManager->getComponent<KingdomComponent>(entity).kingdomTechLevel+=2;
            } else{
                oss << "The " << getKingdomAdjective(entityManager, entity) << " " << getKingdomName(entityManager, entity)
                    << " announces a monumental breakthrough in research, particularly in areas with direct military implications. "
                       "This remarkable advancement underscores their commitment to innovation and reinforces their strategic prowess in safeguarding their realm." << std::endl;
                std::cout << oss.str();

                entityManager->getComponent<KingdomComponent>(entity).kingdomTechLevel+=10;
                entityManager->getComponent<KingdomComponent>(entity).kingdomStrength+=10;
            }

        }
    }
    static void kingdomDiplomacy(EntityManager* entityManager, ComponentManager* componentManager){
        for (auto& entity : componentManager->getEntities<KingdomComponent>()){

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
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::lock_guard<std::mutex> lock(FILE_MUTEX);
            oss << "Year " << YEAR << ": ";

            oss << entityManager->getComponent<CharacterComponent>(entity).name << " is happy to be alive!" << std::endl;
            std::cout << oss.str();
        }
    }

private:
    static std::string getCharacterRace(EntityManager* entityManager, ID entity){
        switch (entityManager->getComponent<CharacterComponent>(entity).race) {
            case CharacterComponent::HUMAN:
                return "human";
            case CharacterComponent::ELF:
                return "elf";
            case CharacterComponent::DWARF:
                return "dwarf";
            case CharacterComponent::GOBLIN:
                return "goblin";
            default:
                return "";
        }
    }
    static std::string getCharacterGender(ID entity){
        switch (entity % 2) {
            case 0:
                return "female";
            case 1:
                return "male";
            default:
                return "";
        }
    }

};

struct WarSystem{

};

#endif
