#pragma once
#ifndef TILE_H
#define TILE_H

#include <string_view>

class Tile {
    float m_tileValue;

public:
    Tile(float tileValue) : m_tileValue(tileValue){}
    virtual ~Tile();
    virtual void describeTile() = 0;

};


#endif
