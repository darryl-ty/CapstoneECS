#include <random>
#include <cmath>
#include <thread>
#include <filesystem>
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
        entityManager->getComponent<KingdomComponent>(i).aggressionLevel = static_cast<KingdomComponent::KingdomNature>(aggroDist(rng));
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
void nameWorld(){
    std::random_device device;
    std::mt19937 rng(device());
    std::vector<std::string> worldNames{"Eldoria", "Avaloria", "Arcadia", "Lumina", "Celestria", "Mythoria", "Drakoria",
                             "Sylvanoria", "Valoria", "Eldoria", "Azurea", "Solstice", "Evercrest", "Emberlyn",
                             "Frostfall", "Shadowmere", "Verdantia", "Elysium", "Astravia", "Runevale", "Emberwind",
                             "Celestium", "Seraphia", "Eldergrove", "Stormhold", "Wyldewood", "Thunderspire", "Arcanum",
                             "Crystalis"};
    std::uniform_int_distribution<uint64_t> worldNameDist(0, worldNames.size()-1);
    auto worldName = worldNameDist(rng);
    std::filesystem::path worldPath{"/worlds/" + worldNames.at(worldName) + "/" + worldNames.at(worldName) + ".txt"};

    worldHistoryFile = worldPath;

    if (!worldHistoryFile.is_open())
        std::cerr << "Error: Could not open world history file. Data may be lost." << std::endl;

}
void populateEntitySet(){
    for (int i = 0; i < MAX_POSSIBLE_ENTITIES; i++){
        Entity entity;
    }
}
void worldSetup(EntityManager* entityManager, ComponentManager* componentManager) {
    populateEntitySet();
    nameWorld();
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
void simulateWorldYear(EntityManager* entityManager, ComponentManager* componentManager){
    std::thread thread1([entityManager, componentManager] { return KingdomSystem::kingdomProgress(entityManager, componentManager);});
//    std::thread thread2([entityManager, componentManager] { return CharacterSystem::characterActions(entityManager, componentManager);});


    thread1.join();
//    thread2.join();
}
void gameLoop() {
    EntityManager entityManager;
    ComponentManager componentManager;

    START_YEAR = yearSetup();
    worldSetup(&entityManager, &componentManager);

    KingdomSystem::establishKingdoms(&entityManager, &componentManager);

    for(YEAR = START_YEAR+1; YEAR < START_YEAR+10; YEAR++){
        simulateWorldYear(&entityManager, &componentManager);
    }
    worldHistoryFile.close();
}

int main() {
    gameLoop();


    return 0;
}
