#include <random>
#include <cmath>
#include <thread>
#include <filesystem>
#include <QApplication>
#include <QTableWidget>
#include <QHeaderView>
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QDialogButtonBox>
#include <QDir>
#include "entitymanager.h"
#include "componentmanager.h"
#include "systems.h"
#include "../include/PerlinNoise.hpp"
#include "ui/mainwindow.h"
#include "ui/worldcreationwindow.h"
#include "ui/startwindow.h"

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
void createKingdomEntities(EntityManager* entityManager, ComponentManager* componentManager, int worldSize){
    // This signifies the sqrt of the world size and I will take the
    // ceiling of that value to determine how many kingdoms to generate.
    int numOfKingdoms = ceil(sqrt(worldSize));

    auto itr = ENTITY_SET.begin();
    for (int i = 0; i < numOfKingdoms; i++){
        entityManager->addComponent(*itr, KingdomComponent{});
        componentManager->addEntity<KingdomComponent>(*itr);
        itr++;
    }
}
void nameWorld(){ // TODO: FIGURE THIS OUT
    std::random_device device;
    std::mt19937 rng(device());
    std::vector<std::string> worldNames{"Eldoria", "Avaloria", "Arcadia", "Lumina", "Celestria", "Mythoria", "Drakoria",
                             "Sylvanoria", "Valoria", "Eldoria", "Azurea", "Solstice", "Evercrest", "Emberlyn",
                             "Frostfall", "Shadowmere", "Verdantia", "Elysium", "Astravia", "Runevale", "Emberwind",
                             "Celestium", "Seraphia", "Eldergrove", "Stormhold", "Wyldewood", "Thunderspire", "Arcanum",
                             "Crystalis", "Portend"};
    std::uniform_int_distribution<uint64_t> worldNameDist(0, worldNames.size()-1);
    auto worldName = worldNameDist(rng);
//    std::filesystem::path worldPath{"C:\\Users\\Darryl Jr\\CLionProjects\\ECSExplorations\\src\\worlds\\" + worldNames.at(worldName) + "\\" + worldNames.at(worldName) + ".txt"};
//    std::filesystem::create_directory(worldPath);

    HEADER_ADDR->setText(QString::fromStdString(((worldName%2 == 0) ? "World of " : "Planes of ") + worldNames.at(worldName)));
    worldHistoryFile = static_cast<std::basic_ofstream<char>>(worldNames.at(worldName) + "WorldHistory" + std::to_string(START_YEAR) + ".txt");

    if (!worldHistoryFile.is_open())
        std::cerr << "Error: Could not open world history file. Data may be lost." << std::endl;

}
void populateEntitySet(){
    for (int i = 0; i < MAX_POSSIBLE_ENTITIES; i++){
        Entity entity;
    }
}
void worldSetup(EntityManager* entityManager, ComponentManager* componentManager, int worldWidth, int worldHeight, int worldSeed) {
    populateEntitySet();
    nameWorld();
    createKingdomEntities(entityManager, componentManager, worldWidth);
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
    std::thread kingdomProgThread([entityManager, componentManager] { return KingdomSystem::kingdomProgress(entityManager, componentManager);});
    std::thread kingdomDiploThread([entityManager, componentManager] { return KingdomSystem::kingdomDiplomacy(entityManager, componentManager);});
    std::thread warThread([entityManager, componentManager] { return WarSystem::simulateWar(entityManager, componentManager);});
    std::thread characterThread([entityManager, componentManager] { return CharacterSystem::characterActions(entityManager, componentManager);});


    kingdomProgThread.join();
    kingdomDiploThread.join();
    warThread.join();
    characterThread.join();
}
void gameLoop(int worldWidth, int worldHeight, int worldSeed) {
    EntityManager entityManager;
    ComponentManager componentManager;


    WORLD_WIDTH = worldWidth;
    WORLD_HEIGHT = worldHeight;
    START_YEAR = yearSetup();

    worldSetup(&entityManager, &componentManager, worldWidth, worldHeight, worldSeed);

    KingdomSystem::establishKingdoms(&entityManager, &componentManager);

    for(YEAR = START_YEAR+1; YEAR < START_YEAR+10; YEAR++){
        simulateWorldYear(&entityManager, &componentManager);
    }
    worldHistoryFile.close();
}

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    std::random_device device;
    std::mt19937 rng(device());

    std::uniform_int_distribution<uint32_t> perlinDist(1, 1500);

    StartScreen startScreen;
    startScreen.show();

    WorldCreationDialog worldCreationDialog;
    MainWorldView mainWorldView(100, 100, siv::PerlinNoise {perlinDist(rng)});
    TEXT_ADDR = mainWorldView.getTextAddr();
    HEADER_ADDR = mainWorldView.getHeaderAddr();

    // Connect signals and slots for world creation dialog
    QObject::connect(&worldCreationDialog, &WorldCreationDialog::worldCreated, [&](int width, int height, QString seed) {
        uint32_t perlinSeed = seed.toInt();
        if (seed.size() < 1){
            perlinSeed = perlinDist(rng);
            mainWorldView.updateWorld(width, height, siv::PerlinNoise {perlinSeed});
        } else {
            mainWorldView.updateWorld(width, height, siv::PerlinNoise {perlinSeed});
        }
        mainWorldView.show();


        // Close the world creation dialog
        worldCreationDialog.close();
        startScreen.close();
        std::thread gameThread(gameLoop, width, height, seed.toInt());
        gameThread.detach();
    });

    // Connect signals and slots for start screen
    QObject::connect(&startScreen, &StartScreen::createWorldClicked, [&]() {
        // Show the world creation dialog when "Create World" is clicked
        worldCreationDialog.show();
    });

    QObject::connect(&startScreen, &StartScreen::loadWorldHistoryClicked, [&](){
        QString fileName = QFileDialog::getOpenFileName(nullptr, "Select Text File", QDir::currentPath(), "Text Files (*.txt)");

        // Check if a file was selected
        if (!fileName.isEmpty()) {
            // Create a new window (QWidget)
            QWidget *window = new QWidget;
            window->setAttribute(Qt::WA_DeleteOnClose); // Delete the window when closed

            // Create a layout for the window
            QVBoxLayout *layout = new QVBoxLayout(window);

            // Create a QTextEdit widget
            QTextEdit *textEdit = new QTextEdit;
            textEdit->setReadOnly(true); // Make it read-only so the user can't edit the content

            // Add the QTextEdit widget to the layout
            layout->addWidget(textEdit);

            // Open the file and read its content
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QByteArray buffer = file.readAll();
                file.close();
                textEdit->setPlainText(QString(buffer)); // Set the content of the QTextEdit widget
            } else {
                QMessageBox::warning(nullptr, "Error", "Failed to open file: " + file.errorString());
                delete window; // Delete the window if there's an error
                return 1;
            }

            // Add a button box with an OK button to close the window
            QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
            layout->addWidget(buttonBox);
            QObject::connect(buttonBox, &QDialogButtonBox::accepted, window, &QWidget::close);

            // Set the window title
            window->setWindowTitle("History Viewer");
            window->showMaximized();

            // Show the window
            window->show();
        }

    });

    QObject::connect(&startScreen, &StartScreen::quitClicked, [&]() {
        // Quit the application when "Quit" is clicked
        QApplication::quit();
    });

    return a.exec();
}
