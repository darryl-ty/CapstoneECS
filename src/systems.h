#pragma once
#ifndef SYSTEMS_H
#define SYSTEMS_H
#include <sstream>
#include <fstream>
#include <mutex>
#include <random>
#include <cmath>
#include "entitymanager.h"
#include "componentmanager.h"

static int START_YEAR;
static int YEAR;
static std::ofstream worldHistoryFile;
std::mutex FILE_MUTEX;

struct KingdomSystem{
    static void establishKingdoms(EntityManager* entityManager, ComponentManager* componentManager){
        for (auto& entity : componentManager->getEntities<KingdomComponent>()){
           std::cout << "Year " << START_YEAR << ": The "
           << getKingdomAdjective(entityManager, entity) << " "
           << getKingdomName(entityManager, entity) << " was established. May the sovereign "
           << getKingdomRulerName(entityManager, entity) << " reign supreme for eons!" << std::endl;
            worldHistoryFile << "Year " << START_YEAR << ": The "
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
            if (entityManager->getComponent<WarComponent>(entity).defender == entity || entityManager->getComponent<WarComponent>(entity).initiator == entity) {
                    oss << "The " << getKingdomAdjective(entityManager, entity) << " "
                        << getKingdomName(entityManager, entity)
                        << " was too busy " << ((entityManager->getComponent<WarComponent>(entity).defender == entity)
                                                ? "defending their realm to make any significant research progress this year."
                                                : "attacking realms to make any significant research progress this year.")
                        << std::endl;
                    std::cout << oss.str();
                    worldHistoryFile << oss.str() << std::endl;

                    continue;
                }

            if (researchChange < -5) {
                oss << "The " << getKingdomAdjective(entityManager, entity) << " " << getKingdomName(entityManager, entity)
                << " research initiatives have proven fruitless, leading to the demise of valuable findings. "
                   "They must now reassess their approach and strive to prevent similar losses in the future." << std::endl;
                std::cout << oss.str();
                worldHistoryFile << oss.str() << std::endl;

                entityManager->getComponent<KingdomComponent>(entity).kingdomTechLevel-=10;
                entityManager->getComponent<KingdomComponent>(entity).kingdomStrength-=10;
            } else if (researchChange < 0){
                oss << "The " << getKingdomAdjective(entityManager, entity) << " " << getKingdomName(entityManager, entity)
                    << " research endeavors have yielded no tangible progress, leaving its knowledge base stagnant. "
                       "It's imperative for them to recalibrate their strategies and embark on a more fruitful path forward." << std::endl;
                std::cout << oss.str();
                worldHistoryFile << oss.str() << std::endl;

            } else if (researchChange < 9){
                oss << "The " << getKingdomAdjective(entityManager, entity) << " " << getKingdomName(entityManager, entity)
                    << " made considerable progress in their research efforts this year. With optimism and dedication, "
                       "they look forward to leveraging these accomplishments as a springboard for even greater achievements ahead." << std::endl;
                std::cout << oss.str();
                worldHistoryFile << oss.str() << std::endl;

                entityManager->getComponent<KingdomComponent>(entity).kingdomTechLevel+=2;
            } else{
                oss << "The " << getKingdomAdjective(entityManager, entity) << " " << getKingdomName(entityManager, entity)
                    << " announces a monumental breakthrough in research, particularly in areas with direct military implications. "
                       "This remarkable advancement underscores their commitment to innovation and reinforces their strategic prowess in safeguarding their realm." << std::endl;
                std::cout << oss.str();
                worldHistoryFile << oss.str() << std::endl;

                entityManager->getComponent<KingdomComponent>(entity).kingdomTechLevel+=10;
                entityManager->getComponent<KingdomComponent>(entity).kingdomStrength+=10;
            }

        }
    }
    static void kingdomDiplomacy(EntityManager* entityManager, ComponentManager* componentManager){
        std::random_device device;
        std::mt19937 rng(device());
        std::uniform_int_distribution<uint32_t> diploDist(1, 4);
        std::uniform_int_distribution<uint32_t> kingdomDist(0, componentManager->getEntities<KingdomComponent>().size()-1);
        for (auto& entity : componentManager->getEntities<KingdomComponent>()){
            std::ostringstream oss;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::lock_guard<std::mutex> lock(FILE_MUTEX);
            auto diploInteractionChance = diploDist(rng);
            auto kingdom = kingdomDist(rng);

            oss << "Year " << YEAR << ": ";
            if (diploInteractionChance > 1 || entity == kingdom
            || entityManager->getComponent<WarComponent>(entity).defender == entity
            || entityManager->getComponent<WarComponent>(entity).initiator == entity)
                continue;
            if (entityManager->getComponent<KingdomComponent>(entity).kingdomRelationships.at(kingdom) < 0){
                switch (diploInteractionChance % 2) {
                    case 0: // Friendly Interaction/repair relations
                        oss << "The " << getKingdomAdjective(entityManager, entity) << " "
                        << getKingdomName(entityManager, entity) << " has sent a delegation to the capital of the "
                        << getKingdomName(entityManager, kingdom) << ". It seems they are trying to promote "
                                                                     "better relations between the two civilizations." << std::endl;
                        std::cout << oss.str();
                        worldHistoryFile << oss.str() << std::endl;

                        entityManager->getComponent<KingdomComponent>(entity).kingdomRelationships.at(kingdom) += 1;
                        entityManager->getComponent<KingdomComponent>(kingdom).kingdomRelationships.at(entity) +=1;
                        break;
                    case 1: // War
                        oss << "War has broken out between the " << getKingdomAdjective(entityManager, entity) << " "
                        << getKingdomName(entityManager, entity) << " and the "
                        << getKingdomAdjective(entityManager, kingdom) << " "
                        << getKingdomName(entityManager, kingdom) << ". Let the drums of war sound!" << std::endl;

                        std::cout << oss.str();
                        worldHistoryFile << oss.str() << std::endl;

                        entityManager->getComponent<KingdomComponent>(entity).kingdomRelationships.at(kingdom) = -10;
                        entityManager->getComponent<KingdomComponent>(kingdom).kingdomRelationships.at(entity) = -10;

                        entityManager->addComponent(entity, WarComponent{entity,kingdom});
                        entityManager->addComponent(kingdom, WarComponent{entity,kingdom});
                        componentManager->addEntity<WarComponent>(entity);
                        componentManager->addEntity<WarComponent>(kingdom);

                        break;
                }
            } else {
                switch (diploInteractionChance % 3) {
                    case 0:
                        oss << "The " << getKingdomAdjective(entityManager, entity) << " "
                            << getKingdomName(entityManager, entity) << " pigeon-mailed a friendly letter to sovereign "
                            << getKingdomRulerName(entityManager, kingdom) << " of "
                            << getKingdomName(entityManager, kingdom) << "." << std::endl;
                        std::cout << oss.str();
                        worldHistoryFile << oss.str() << std::endl;

                        entityManager->getComponent<KingdomComponent>(entity).kingdomRelationships.at(kingdom) += 3;
                        entityManager->getComponent<KingdomComponent>(kingdom).kingdomRelationships.at(entity) +=3;
                        break;
                    case 1:
                        oss << "The sovereign " << getKingdomRulerName(entityManager, entity) << " of "
                            << getKingdomName(entityManager, entity) << " is hosting a party for the citizens of the "
                            << getKingdomName(entityManager, kingdom) << ". This will be a great chance for the two "
                            "civilizations and their people to get to know one another." << std::endl;
                        std::cout << oss.str();
                        worldHistoryFile << oss.str() << std::endl;

                        entityManager->getComponent<KingdomComponent>(entity).kingdomRelationships.at(kingdom) += 5;
                        entityManager->getComponent<KingdomComponent>(kingdom).kingdomRelationships.at(entity) +=5;
                        break;
                    case 2:
                        oss << "The " << getKingdomAdjective(entityManager, entity) << " "
                            << getKingdomName(entityManager, entity) << " cemented an alliance with the "
                            << getKingdomName(entityManager, kingdom) << " between the sovereigns  "
                            << getKingdomRulerName(entityManager, entity) << " and "
                            << getKingdomRulerName(entityManager, kingdom)
                            << " may they rule better together and til death do they part!" << std::endl;
                        std::cout << oss.str();
                        worldHistoryFile << oss.str() << std::endl;

                        entityManager->getComponent<KingdomComponent>(entity).kingdomRelationships.at(kingdom) += 10;
                        entityManager->getComponent<KingdomComponent>(kingdom).kingdomRelationships.at(entity) +=10;
                        break;
                }

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
        std::random_device device;
        std::mt19937 rng(device());
        std::uniform_int_distribution<uint32_t> actionDist(1, 10);
        for (auto& entity : componentManager->getEntities<CharacterComponent>()){
            std::ostringstream oss;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::lock_guard<std::mutex> lock(FILE_MUTEX);

            if (!entityManager->getComponent<CharacterComponent>(entity).isAlive)
                continue;

            oss << "Year " << YEAR << ": ";
            if (entityManager->getComponent<CharacterComponent>(entity).name == "Elana"){
                oss << "Elana took some time to stop and smell the roses." << std::endl;
                std::cout << oss.str();
                worldHistoryFile << oss.str() << std::endl;
                continue;
            }
            switch (actionDist(rng)) {
                case 1:
                    oss << "The " << getCharacterGender(entity) << " " << getCharacterRace(entityManager, entity)
                    << " " << getCharacterName(entityManager, entity) << " trained in the hinterlands developing"
                    << (getCharacterGender(entity) == "male" ? " his" : " her") << " skills!" << std::endl;
                    std::cout << oss.str();
                    worldHistoryFile << oss.str() << std::endl;

                    entityManager->getComponent<CharacterComponent>(entity).damage += 5;

                    break;
                case 2:
                    oss << getCharacterName(entityManager, entity) << ", the " << getCharacterGender(entity) << " "
                    << getCharacterRace(entityManager, entity) << " spent their time improving their looks." << std::endl;
                    std::cout << oss.str();
                    worldHistoryFile << oss.str() << std::endl;

                    entityManager->getComponent<CharacterComponent>(entity).health += 1;

                    break;
                case 3:
                    oss << getCharacterName(entityManager, entity) << ", the " << getCharacterGender(entity) << " "
                        << getCharacterRace(entityManager, entity) << " got lost in the wilds." << std::endl;
                    std::cout << oss.str();
                    worldHistoryFile << oss.str() << std::endl;
                    break;
                case 4:
                    oss << "Death claimed the " << getCharacterRace(entityManager, entity) << " "
                        << getCharacterName(entityManager, entity) << ". " << (getCharacterGender(entity) == "male" ? " He" : " She")
                        << " will be missed dearly..." << std::endl;
                    std::cout << oss.str();
                    worldHistoryFile << oss.str() << std::endl;

                    entityManager->getComponent<CharacterComponent>(entity).isAlive = false;

                    break;
                case 5:
                    oss << "The " << getCharacterGender(entity) << " " << getCharacterRace(entityManager, entity)
                        << " " << getCharacterName(entityManager, entity) << " decided to join a cult." << std::endl;
                    std::cout << oss.str();
                    worldHistoryFile << oss.str() << std::endl;
                    break;
                case 6:
                    oss << getCharacterName(entityManager, entity) << ", the " << getCharacterGender(entity) << " "
                        << getCharacterRace(entityManager, entity) << " became the blade-keeper of an ancient weapon!"
                        << (getCharacterGender(entity) == "male" ? " He" : " She") << " will defend it with their life!" << std::endl;
                    std::cout << oss.str();
                    worldHistoryFile << oss.str() << std::endl;

                    entityManager->getComponent<CharacterComponent>(entity).damage += 100;

                    break;
                case 7:
                    oss << getCharacterName(entityManager, entity) << ", the " << getCharacterRace(entityManager, entity)
                    << " lost their leg in a arm-wrestling competition. Let's hope they dont fall for that again." << std::endl;
                    std::cout << oss.str();
                    worldHistoryFile << oss.str() << std::endl;

                    entityManager->getComponent<CharacterComponent>(entity).health -= 5;

                    break;
                case 8:
                    oss << "The " << getCharacterGender(entity) << " " << getCharacterRace(entityManager, entity)
                        << " " << getCharacterName(entityManager, entity) << " pondered the meaning of life." << std::endl;
                    std::cout << oss.str();
                    worldHistoryFile << oss.str() << std::endl;
                    break;
                case 9:
                    oss << "The " << getCharacterGender(entity) << " " << getCharacterRace(entityManager, entity)
                        << " " << getCharacterName(entityManager, entity) << " got into an altercation with "
                        << (entityManager->getComponent<CharacterComponent>(entity-1).isAlive
                        ? getCharacterName(entityManager, entity-1)
                        : " their spouse") << "."  << std::endl;
                    std::cout << oss.str();
                    worldHistoryFile << oss.str() << std::endl;
                    break;
                case 10:
                    oss << getCharacterName(entityManager, entity) << ", the " << getCharacterGender(entity) << " "
                        << getCharacterRace(entityManager, entity) << " discovered the secrets of life and death." << std::endl;
                    std::cout << oss.str();
                    worldHistoryFile << oss.str() << std::endl;

                    entityManager->getComponent<CharacterComponent>(entity).health += 5000;

                    break;
                default:
                    break;
            }
        }
    }

private:
    static std::string getCharacterName(EntityManager* entityManager, ID entity){
        return entityManager->getComponent<CharacterComponent>(entity).name;
    }
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
    static void simulateWar(EntityManager* entityManager, ComponentManager* componentManager){
        for(auto& warID : componentManager->getEntities<WarComponent>()){
            if (warID %2 == 0)
                continue;
            std::ostringstream oss;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::lock_guard<std::mutex> lock(FILE_MUTEX);

            int attackerStr = getAttackerStr(entityManager, warID);
            int *attackerHealth = getAttackerHealth(entityManager, warID);
            int defenderStr = getDefenderStr(entityManager, warID);
            int *defenderHealth = getDefenderHealth(entityManager, warID);
            if(getAttackerName(entityManager, warID).size() < 12 || getDefenderName(entityManager, warID).size() < 12)
                continue;

            *defenderHealth -= attackerStr;

            oss << "Year " << YEAR << ": ";
            if (*defenderHealth <= 0){
                oss << "The " << getDefenderName(entityManager, warID) << " was overrun by the "
                << getAttackerName(entityManager, warID) << "! May God protect their people!" << std::endl;

                killDefender(entityManager, componentManager, warID);
                endWar(entityManager, componentManager, warID);

                std::cout << oss.str();
                worldHistoryFile << oss.str() << std::endl;
                continue;
            }

            *attackerHealth -= defenderStr;

            if (*attackerHealth <= 0){
                oss << "The " << getDefenderName(entityManager, warID) << " masterfully defended their realm from the "
                << getAttackerName(entityManager, warID) << "! Peace has once again been established!" << std::endl;

                endWar(entityManager, componentManager, warID);

                std::cout << oss.str();
                worldHistoryFile << oss.str() << std::endl;
                continue;
            }

            oss << "Both the " << getAttackerName(entityManager, warID) << " and the "
            << getDefenderName(entityManager, warID) << " have been engaged in a bloody war with each other this year. "
                                                        "Only time will tell who prevails!" << std::endl;
            std::cout << oss.str();
            worldHistoryFile << oss.str() << std::endl;
        }
    }
private:
    static int getAttackerStr(EntityManager *entityManager, const unsigned int &warID){
        return ceil(entityManager->getComponent<KingdomComponent>(entityManager->getComponent<WarComponent>(warID).initiator).kingdomStrength + (entityManager->getComponent<KingdomComponent>(entityManager->getComponent<WarComponent>(warID).initiator).kingdomTechLevel * 0.2));
    }
    static int getDefenderStr(EntityManager *entityManager, const unsigned int &warID){
        return ceil(entityManager->getComponent<KingdomComponent>(entityManager->getComponent<WarComponent>(warID).defender).kingdomStrength + (entityManager->getComponent<KingdomComponent>(entityManager->getComponent<WarComponent>(warID).defender).kingdomTechLevel * 0.2));
    }
    static int* getAttackerHealth(EntityManager *entityManager, const unsigned int &warID){
        return &entityManager->getComponent<KingdomComponent>(entityManager->getComponent<WarComponent>(warID).initiator).kingdomStrength;
    }
    static int* getDefenderHealth(EntityManager *entityManager, const unsigned int &warID){
        return &entityManager->getComponent<KingdomComponent>(entityManager->getComponent<WarComponent>(warID).defender).kingdomStrength;
    }
    static std::string getAttackerName(EntityManager *entityManager, ID warID){
        return entityManager->getComponent<KingdomComponent>(entityManager->getComponent<WarComponent>(warID).initiator).kingdomName;
    }
    static std::string getDefenderName(EntityManager *entityManager, ID warID){
        return entityManager->getComponent<KingdomComponent>(entityManager->getComponent<WarComponent>(warID).defender).kingdomName;
    }
    static void killDefender(EntityManager *entityManager, ComponentManager *componentManager, ID warID){
        componentManager->removeEntity<KingdomComponent>(entityManager->getComponent<WarComponent>(warID).defender);
        entityManager->removeComponent(entityManager->getComponent<WarComponent>(warID).defender, WarComponent{});
        entityManager->removeComponent(entityManager->getComponent<WarComponent>(warID).initiator, WarComponent{});
    }
    static void endWar(EntityManager *entityManager, ComponentManager *componentManager, ID warID){
        entityManager->removeComponent(entityManager->getComponent<WarComponent>(warID).initiator, WarComponent{entityManager->getComponent<WarComponent>(warID).initiator,entityManager->getComponent<WarComponent>(warID).defender});
        entityManager->removeComponent(entityManager->getComponent<WarComponent>(warID).defender, WarComponent{entityManager->getComponent<WarComponent>(warID).initiator,entityManager->getComponent<WarComponent>(warID).defender});
        componentManager->removeEntity<WarComponent>(entityManager->getComponent<WarComponent>(warID).initiator);
        componentManager->removeEntity<WarComponent>(entityManager->getComponent<WarComponent>(warID).defender);

    }


};

#endif
