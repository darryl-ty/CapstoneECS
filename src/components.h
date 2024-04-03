#pragma once
#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <string>
#include <unordered_map>
#include "entity.h"

struct KingdomComponent{
    enum KingdomNature{
        Harmless = 0,
        Peaceful = 1,
        Neutral = 2,
        Opportunistic = 3,
        Evil = 4
    };
    KingdomNature aggressionLevel;
    std::string kingdomName = "Kingdom of ";
    int kingdomStrength;
    int kingdomTechLevel;
    ID rulerID = 0;
    std::unordered_map<ID, int> kingdomRelationships{{0, 0}};
};

struct KingdomNames{
    static inline std::vector<std::string> kingdomNames{"Avaloria","Eldoria","Dunmere","Rivendell","Stormgarde","Frostholm","Silverwood",
    "Shadowmere", "Ironhelm","Emberfell","Whiterun","Evergreen","Dragonspire","Blackwater","Sunhaven","Stonewatch","Goldenleaf",
    "Thornridge","Moonshadow","Highwind","Oakheart","Glimmerhold","Firebrand","Swiftwater","Moonstone","Starhaven","Thunderpeak",
    "Sablewood","Duskwood","Amberfall","Stormcrest","Silvershade","Hawthorn","Winterfell","Brightwater","Ashwood","Ravencrest",
    "Silentpeak","Darkhaven","Lion's Gate","Mistwood","Shadowvale","Ironwood","Evernight","Windsong","Swordbreak","Ebonreach",
    "Goldcrest","Shadowbrook","Whitewater","Dragonwatch","Stagwood","Emberglow","Stonehaven","Frostfall","Skyhold","Elmswood",
    "Swiftblade","Blackthorn","Stormwatch","Silverpeak","Bloodstone","Hearthfire","Misthaven","Shadowwood","Ironforge","Winterhold",
    "Greenhaven","Starfall","Ashenwood","Sunsetshire","Crestwood","Frostgate","Blackrock","Silverbane","Ravenwood","Stormshield",
    "Goldvale","Darkwater","Windmere","Thornkeep","Moonfall","Shadowridge","Silvershore","Briarwood","Dragonfire","Stonegate",
    "Frostblade","Shadowcliff","Ironclad","Greenwood","Ravenspire","Moonlight","Starbrook","Sunspear","Stormhold","Silverfall","Thornbriar",
    "Dragoncrest","Goldenreach"};

    static std::string getRandomKingdomName(){
        std::random_device randomDevice;
        std::mt19937 rng(randomDevice());

        std::uniform_int_distribution<uint64_t> aggroDist(0, kingdomNames.size()-1);

        auto name = kingdomNames.at(aggroDist(rng));
        auto itr = std::find(kingdomNames.begin(), kingdomNames.end(), name);
        kingdomNames.erase(itr);

        return name;

    }
};
struct KingdomAdjectives{
    static inline std::vector<std::string> harmlessAdj  {"innocuous", "benign", "non-threatening", "safe", "inoffensive",
                                                        "unobjectionable", "wholesome", "mild", "docile","gentle",
                                                        "benevolent", "tame", "amiable", "harmless"};
    static inline std::vector<std::string> peacefulAdj  {"serene", "tranquil", "calm", "harmonious", "placid", "gentle",
                                                        "peaceful", "quiet", "soothing", "relaxing", "balanced",
                                                        "untroubled", "tranquilizing", "reassuring", "idyllic"};
    static inline std::vector<std::string> neutralAdj   {"common", "usual", "typical", "standard", "ordinary",
                                                       "regular", "average", "routine", "conventional", "normal",
                                                       "traditional", "typified", "customary", "everyday", "familiar"};
    static inline std::vector<std::string> opportuneAdj {"exploitative", "calculating", "self-serving", "opportune",
                                                         "strategic", "ambitious", "resourceful", "advantageous",
                                                         "astute", "shrewd", "cunning", "machiavellian", "pragmatic",
                                                         "savvy", "tactical"};
    static inline std::vector<std::string> evilAdj      {"sinister", "malevolent", "diabolical", "malicious",
                                                         "wicked", "vicious", "depraved", "cruel", "nefarious",
                                                         "scheming", "malignant", "corrupt", "vile"};

    static std::string getAdjective(KingdomComponent::KingdomNature kingdomNature){
        std::random_device randomDevice;
        std::mt19937 rng(randomDevice());
        std::string adj;

        std::uniform_int_distribution<uint64_t> harmlessDist(0, harmlessAdj.size()-1);
        std::uniform_int_distribution<uint64_t> peacefulDist(0, peacefulAdj.size()-1);
        std::uniform_int_distribution<uint64_t> neutralDist(0, neutralAdj.size()-1);
        std::uniform_int_distribution<uint64_t> opportuneDist(0, opportuneAdj.size()-1);
        std::uniform_int_distribution<uint64_t> evilDist(0, evilAdj.size()-1);

        switch (kingdomNature) {
            case KingdomComponent::Harmless:
                adj = harmlessAdj.at(harmlessDist(rng));
                return adj;
            case KingdomComponent::Peaceful:
                adj = peacefulAdj.at(peacefulDist(rng));
                return adj;
            case KingdomComponent::Neutral:
                adj = neutralAdj.at(neutralDist(rng));
                return adj;
            case KingdomComponent::Opportunistic:
                adj = opportuneAdj.at(opportuneDist(rng));
                return adj;
            case KingdomComponent::Evil:
                adj = evilAdj.at(evilDist(rng));
                return adj;
            default:
                return "";
        }
    }
};

struct LocationComponent{
    int locationX;
    int locationY;
};

struct RulerComponent{
    ID kingdomID;
};

struct WarComponent{
    ID initiator=-1;
    ID defender=-1;
};

struct CharacterComponent{
    enum Race{
        HUMAN = 0,
        ELF,
        DWARF,
        GOBLIN
    };
    Race race;
    std::string name;
    bool isAlive = true;
    int health;
    int damage;
};

struct CharacterNames{
    static inline std::vector<std::string> humanFirstNames{"Elana", "Alexander", "Isabella", "William", "Sophia", "James", "Olivia", "John",
                                             "Emma", "Michael", "Ava", "Christopher", "Mia", "Matthew", "Emily", "David",
                                             "Abigail", "Daniel", "Charlotte", "Andrew", "Harper", "Joseph", "Amelia",
                                             "Benjamin", "Evelyn", "Jacob", "Elizabeth", "Samuel", "Ella", "Nicholas",
                                             "Grace", "Ethan", "Chloe", "Ryan", "Victoria", "Anthony", "Scarlett", "Jonathan",
                                             "Madison", "Joshua", "Lily", "William", "Luna", "Logan", "Avery", "Gabriel", "Sofia"};
    static inline std::vector<std::string> elfFirstNames{"Thalorin", "Elara", "Aerendil", "Galadriel", "Aranor", "Elowen", "Thranduil",
                                      "Elenion", "Lúthien", "Finrod", "Aelar", "Celeborn", "Amara", "Legolas", "Evanya",
                                      "Caladrel", "Miriel", "Aragorn", "Elladan", "Arwen", "Thalion", "Elrond", "Aranel",
                                      "Faelar", "Elanor", "Valandil", "Nenya", "Farothel", "Galanor", "Aelarion", "Nimrodel",
                                      "Eldarin", "Eärendil", "Arasinya", "Thelorn", "Noldorin", "Calenon", "Alathiel", "Elessar",
                                      "Thalorian", "Finduilas", "Aramil", "Elariana", "Celebrian", "Thrandor", "Maedhros", "Silvain",
                                      "Eowyn", "Erendis", "Aranelle"};
    static inline std::vector<std::string> dwarfFirstNames{"Thorin", "Gimli", "Balin", "Dwalin", "Fili", "Kili", "Bofur", "Bifur",
                                             "Bombur", "Oin", "Gloin", "Dori", "Nori", "Ori", "Dain", "Durin", "Thrain",
                                             "Dori", "Dain", "Thorin", "Balin", "Gloin", "Gimli", "Dwalin", "Bifur", "Fili",
                                             "Nori", "Ori", "Kili", "Bombur", "Bofur", "Oin", "Thror", "Dain", "Thrain",
                                             "Durin", "Grimbeard", "Dain", "Thrain", "Thror", "Durin", "Borin", "Gror",
                                             "Thorin", "Balin", "Dwalin", "Fili", "Kili"};
    static inline std::vector<std::string> goblinFirstNames{"Grishnakh", "Ugluk", "Azog", "Gollum", "Shagrat", "Gorbag", "Lugdush",
                                              "Snaga", "Muzgash", "Skorg", "Narzug", "Kruk", "Gorgol", "Zog", "Snaga",
                                              "Grubluk", "Urzul", "Klurg", "Ghurk", "Nargol", "Krag", "Urg", "Ghak", "Morgol",
                                              "Zug", "Thulg", "Grish", "Rogul", "Grolg", "Thug", "Ruk", "Nurgle", "Gimbul",
                                              "Snarzog", "Grag", "Gorm", "Gruk", "Gurn", "Mogul", "Kurz", "Snik", "Nagruk",
                                              "Gothmog", "Nargul", "Thurg", "Gurk", "Grog"};

    static std::string getCharacterName(CharacterComponent::Race race){
        std::random_device randomDevice;
        std::mt19937 rng(randomDevice());

        std::uniform_int_distribution<uint64_t> humanDist(0, humanFirstNames.size()-1);
        std::uniform_int_distribution<uint64_t> elfDist(0, elfFirstNames.size()-1);
        std::uniform_int_distribution<uint64_t> dwarfDist(0, dwarfFirstNames.size()-1);
        std::uniform_int_distribution<uint64_t> goblinDist(0, goblinFirstNames.size()-1);

        switch (race) {
            case CharacterComponent::Race::HUMAN:
                return humanFirstNames.at(humanDist(rng));
            case CharacterComponent::Race::ELF:
                return elfFirstNames.at(elfDist(rng));
            case CharacterComponent::Race::DWARF:
                return dwarfFirstNames.at(dwarfDist(rng));
            case CharacterComponent::Race::GOBLIN:
                return goblinFirstNames.at(goblinDist(rng));
            default:
                return "";
        }
    }
};

#endif
