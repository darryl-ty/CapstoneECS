
#include <iostream>
#include "WorldTile.h"
#include "Tile.h"

WorldTile::WorldTile(float tileValue) : Tile(tileValue){
    determineTileType(tileValue);
}

WorldTile::~WorldTile()= default;

void WorldTile::describeTile() {
    switch (m_TileType) {
        case TileType::OCEAN:
            std::cout << "A vast " << m_worldTileType << "! Certainly don't want to be here without a boat." << std::endl;
            break;
        case TileType::BEACH:
            std::cout << "A " << m_worldTileType << " nestled between other biomes." << std::endl;
            break;
        case TileType::LOW_PLAINS:
            std::cout << "Rolling " << m_worldTileType << ". Who knows what could be hiding between the grass!" << std::endl;
            break;
        case TileType::PLAINS:
            std::cout << "Vast rolling " << m_worldTileType << std::endl;
            break;
        case TileType::HIGH_PLAINS:
            std::cout << "A " << m_worldTileType << " with hills on the horizon." << std::endl;
            break;
        case TileType::HILLS:
            std::cout << "Some small " << m_worldTileType << std::endl;
            break;
        case TileType::MOUNTAINS:
            std::cout << "A vast range of " << m_worldTileType << "." <<std::endl;
            break;
        default:
            std::cout << "A tile devoid of.. anything?" << std::endl;
            break;
    }
}

void WorldTile::determineTileType(float tileValue) {
    if (tileValue <= .15) {
        m_TileType = TileType::OCEAN;
        m_worldTileColor = Qt::blue;
        m_worldTileType = "Ocean";
    } else if (tileValue > .15 && tileValue <= .3){
        m_TileType = TileType::BEACH;
        m_worldTileColor = Qt::yellow;
        m_worldTileType = "Beach";
    } else if (tileValue > .3 && tileValue <= .35){
        m_TileType = TileType::LOW_PLAINS;
        m_worldTileColor = Qt::green;
        m_worldTileType = "Low Plains";
    } else if (tileValue > .35 && tileValue <= .45){
        m_TileType = TileType::PLAINS;
        m_worldTileColor = Qt::green;
        m_worldTileType = "Plains";
    } else if (tileValue > .45 && tileValue <= .5){
        m_TileType = TileType::HIGH_PLAINS;
        m_worldTileColor = Qt::darkGreen;
        m_worldTileType = "High Plateau";
    } else if (tileValue > .5 && tileValue <= .6){
        m_TileType = TileType::HILLS;
        m_worldTileColor = Qt::darkGreen;
        m_worldTileType = "Hills";
    } else {
        m_TileType = TileType::MOUNTAINS;
        m_worldTileColor = Qt::lightGray;
        m_worldTileType = "Mountains";
    }
}

Qt::GlobalColor WorldTile::getTileColor() {
    return m_worldTileColor;
}

std::string WorldTile::getTileType() {
    return m_worldTileType;
}
