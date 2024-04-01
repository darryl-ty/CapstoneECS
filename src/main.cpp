#include <iostream>
#include <random>
#include <cmath>
#include "entitymanager.h"
#include "componentmanager.h"
#include "systems.h"


void setRandomRuler(EntityManager* entityManager, ComponentManager* componentManager, std::mt19937 rng, size_t i) {
    std::uniform_int_distribution<uint32_t> rulerDist(componentManager->getEntities<KingdomComponent>().size(),
                                                      ENTITY_SET.size());

    auto ruler = rulerDist(rng);
    if (entityManager->getComponent<RulerComponent>(ruler).kingdomID > 0) {
        return setRandomRuler(entityManager, componentManager, rng, i);
    } else {
        entityManager->getComponent<KingdomComponent>(i).rulerID = ruler;
        entityManager->addComponent(ruler, RulerComponent{static_cast<ID>(i)});
        componentManager->addEntity<RulerComponent>(ruler);
    }


}
void setKingdomRelationships(EntityManager* entityManager, ComponentManager* componentManager, std::mt19937 rng) {
    std::uniform_int_distribution<int> relationshipDist(-10, 10);

    for (size_t i = 0; i < componentManager->getEntities<KingdomComponent>().size(); i++){
        for (size_t j = 0; j < componentManager->getEntities<KingdomComponent>().size()-1; j++){
            if (i == j){
                continue;
            }
            auto relationValue = relationshipDist(rng);
            entityManager->getComponent<KingdomComponent>(i).kingdomRelationships.emplace(j, relationValue);
            entityManager->getComponent<KingdomComponent>(j).kingdomRelationships.emplace(i, relationValue);
        }
    }
}
void populateKingdoms(EntityManager* entityManager, ComponentManager* componentManager){
    std::random_device randomDevice;
    std::mt19937 rng(randomDevice());

    std::uniform_int_distribution<int> aggroDist(0, 4);
    std::uniform_int_distribution<int> strengthDist(1, 50);
    std::uniform_int_distribution<int> techDist(1, 20);
    
    for (size_t i = 0; i < componentManager->getEntities<KingdomComponent>().size(); i++){
        entityManager->getComponent<KingdomComponent>(i).aggressionLevel = static_cast<KingdomComponent::AggressionLevel>(aggroDist(rng));
        entityManager->getComponent<KingdomComponent>(i).kingdomName.append(KingdomNames::getRandomKingdomName());
        entityManager->getComponent<KingdomComponent>(i).kingdomStrength = strengthDist(rng);
        entityManager->getComponent<KingdomComponent>(i).kingdomTechLevel = techDist(rng);

        setRandomRuler(entityManager, componentManager, rng, i);
    }

    setKingdomRelationships(entityManager, componentManager, rng);
}
void populateCharacters(EntityManager* entityManager, ComponentManager* componentManager){
    std::random_device randomDevice;
    std::mt19937 rng(randomDevice());

    std::uniform_int_distribution<int> raceDist(0, 3);
    std::uniform_int_distribution<int> healthDist(1, 15);
    std::uniform_int_distribution<int> damageDist(1, 5);
    for (size_t i = componentManager->getEntities<KingdomComponent>().size(); i < ENTITY_SET.size(); i++){
        entityManager->getComponent<CharacterComponent>(i).race = static_cast<CharacterComponent::Race>(raceDist(rng));
        entityManager->getComponent<CharacterComponent>(i).name = CharacterNames::getCharacterName(entityManager->getComponent<CharacterComponent>(i).race);
        entityManager->getComponent<CharacterComponent>(i).health = healthDist(rng);
        entityManager->getComponent<CharacterComponent>(i).damage = damageDist(rng);
    }


}
void createCharacterEntities(EntityManager* entityManager, ComponentManager* componentManager){
    for (size_t i = componentManager->getEntities<KingdomComponent>().size(); i < ENTITY_SET.size(); i++){
        entityManager->addComponent(i,CharacterComponent{});
        componentManager->addEntity<CharacterComponent>(i);
    }
}
void createKingdomEntities(EntityManager* entityManager, ComponentManager* componentManager){
    // This signifies the sqrt of the world size and I will take the
    // ceiling of that value to determine how many kingdoms to generate.
    int kingdoms = 10;
    int numOfKingdoms = ceil(kingdoms);

    auto itr = ENTITY_SET.begin();
    for (int i = 0; i < numOfKingdoms; i++){
        entityManager->addComponent(*itr, KingdomComponent{});
        componentManager->addEntity<KingdomComponent>(*itr);
        itr++;
    }
}
void populateEntitySet(){
    for (int i = 0; i < MAX_POSSIBLE_ENTITIES; i++){
        Entity entity;
    }
}
void worldSetup(EntityManager* entityManager, ComponentManager* componentManager) {
    populateEntitySet();
    createKingdomEntities(entityManager, componentManager);
    createCharacterEntities(entityManager, componentManager);
    populateCharacters(entityManager, componentManager);
    populateKingdoms(entityManager, componentManager);
}
int yearSetup(){
    std::random_device randomDevice;
    std::mt19937 rng(randomDevice());

    std::uniform_int_distribution<int> yearDist(0, 10000);

    return yearDist(rng);
}
void gameLoop() {
    EntityManager entityManager;
    ComponentManager componentManager;

    worldSetup(&entityManager, &componentManager);
    START_YEAR = yearSetup();

    KingdomSystem::establishKingdoms(&componentManager);
}

int main() {
    gameLoop();


    return 0;
}
