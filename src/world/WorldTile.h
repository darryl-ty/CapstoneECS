#pragma once
#ifndef WORLDTILE_H
#define WORLDTILE_H

#include <string>
#include <string_view>
#include <QApplication>
#include "Tile.h"

enum class TileType{
    OCEAN = 0,
    BEACH,
    LOW_PLAINS,
    PLAINS,
    HIGH_PLAINS,
    HILLS,
    MOUNTAINS
};

class WorldTile : public Tile {
    TileType m_TileType;
    Qt::GlobalColor m_worldTileColor;
    std::string m_worldTileType;

    void determineTileType(float tileValue);
public:
    WorldTile(float tileValue);
    ~WorldTile();

    void describeTile() override;
    Qt::GlobalColor getTileColor();
    std::string getTileType();

};


#endif
