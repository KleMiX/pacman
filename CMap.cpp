//
//  CMap.cpp
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/17/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#include "CMap.h"
#include <assert.h>

// default level made up from MapObject
static const int defaultLevel[MAP_WIDTH][MAP_HEIGHT] =
{   { 0, 0, 0, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  5,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2, 2, 2 },
    { 0, 0, 0, 2, 17, 17, 33, 17, 17, 17, 17, 17,  2,  0,  0,  0,  2,  5,  2,  0,  0,  0,  2, 17, 17, 17, 33,  2,  2, 17, 17, 17, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17,  2,  2,  2, 17,  2,  2, 17,  2,  0,  0,  0,  2,  5,  2,  0,  0,  0,  2, 17,  2,  2, 17,  2,  2, 17,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17,  2,  2,  2, 17,  2,  2, 17,  2,  0,  0,  0,  2,  5,  2,  0,  0,  0,  2, 17,  2,  2, 17, 17, 17, 17,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17,  2,  2,  2, 17,  2,  2, 17,  2,  0,  0,  0,  2,  5,  2,  0,  0,  0,  2, 17,  2,  2,  2,  2,  2, 17,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17,  2,  2,  2, 17,  2,  2, 17,  2,  2,  2,  2,  2,  5,  2,  2,  2,  2,  2, 17,  2,  2,  2,  2,  2, 17,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17,  2,  2,  2, 17,  2,  2,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2, 17,  2,  2, 17,  2,  2,  2,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17,  2,  2,  2, 17,  2,  2,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2, 17,  2,  2, 17,  2,  2,  2,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17,  2,  2,  2, 17, 17, 17, 17,  2,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1, 17,  2,  2, 17, 17, 17, 17,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17,  2,  2,  2, 17,  2,  2, 17,  2,  2,  1,  2,  2,  2,  2,  2,  1,  2,  2, 17,  2,  2, 17,  2,  2, 17,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17,  2,  2,  2, 17,  2,  2, 17,  2,  2,  1,  2,  2,  1,  2,  2,  1,  2,  2, 17,  2,  2, 17,  2,  2, 17,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17, 17, 17, 17, 17,  2,  2, 17,  1,  1,  9,  2,  2,  1,  2,  2,  1,  2,  2, 17, 17, 17, 25,  2,  2, 17, 17, 17, 17, 2, 0, 0 },
    { 0, 0, 0, 2,  2,  2,  2,  2, 17,  2,  2,  2,  2,  2,  1, 64, 64, 64,  2,  2,  1,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2,  2,  2,  2,  2, 17,  2,  2,  2,  2,  2,  1, 64, 64, 64,  2,  2,  1,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17, 17, 17, 17, 17,  2,  2, 17,  1,  1,  9,  2,  2,  1,  2,  2,  1,  2,  2, 17, 17, 17, 25,  2,  2, 17, 17, 17, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17,  2,  2,  2, 17,  2,  2, 17,  2,  2,  1,  2,  2,  2,  2,  2,  1,  2,  2, 17,  2,  2, 17,  2,  2, 17,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17,  2,  2,  2, 17,  2,  2, 17,  2,  2,  1,  2,  2,  2,  2,  2,  1,  2,  2, 17,  2,  2, 17,  2,  2, 17,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17,  2,  2,  2, 17, 17, 17, 17,  2,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1, 17,  2,  2, 17, 17, 17, 17,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17,  2,  2,  2, 17,  2,  2,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2, 17,  2,  2, 17,  2,  2,  2,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17,  2,  2,  2, 17,  2,  2,  2,  2,  2,  2,  2,  2,  1,  2,  2,  2,  2,  2, 17,  2,  2, 17,  2,  2,  2,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17,  2,  2,  2, 17,  2,  2, 17,  2,  2,  2,  2,  2,  5,  2,  2,  2,  2,  2, 17,  2,  2,  2,  2,  2, 17,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17,  2,  2,  2, 17,  2,  2, 17,  2,  0,  0,  0,  2,  5,  2,  0,  0,  0,  2, 17,  2,  2,  2,  2,  2, 17,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17,  2,  2,  2, 17,  2,  2, 17,  2,  0,  0,  0,  2,  5,  2,  0,  0,  0,  2, 17,  2,  2, 17, 17, 17, 17,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17,  2,  2,  2, 17,  2,  2, 17,  2,  0,  0,  0,  2,  5,  2,  0,  0,  0,  2, 17,  2,  2, 17,  2,  2, 17,  2,  2, 17, 2, 0, 0 },
    { 0, 0, 0, 2, 17, 17, 33, 17, 17, 17, 17, 17,  2,  0,  0,  0,  2,  5,  2,  0,  0,  0,  2, 17, 17, 17, 33,  2,  2, 17, 17, 17, 17, 2, 0, 0 },
    { 0, 0, 0, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  5,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2, 2, 2 }  };

// our current - active map (so balls and energizers can be removed)
static int activeLevel[MAP_WIDTH][MAP_HEIGHT];

int CMap::getXcoordinate(int xtile)
{
    // simple conversion and adding offset to center the coord
    return xtile * TILE_SIZE - OBJECT_OFFSET;
}

int CMap::getYcoordinate(int ytile)
{
    // simple conversion and adding offset to center the coord
    return ytile * TILE_SIZE - OBJECT_OFFSET;
}

int CMap::getTileXcoordinate(int x)
{
    // simple conversion and adding offset to center the coord
    return (x + OBJECT_OFFSET) / TILE_SIZE;
}

int CMap::getTileYcoordinate(int y)
{
    // simple conversion and adding offset to center the coord
    return (y + OBJECT_OFFSET) / TILE_SIZE;
}

void CMap::Draw(SDL_Surface *dest)
{
    _background->Draw(dest);
    
    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            // checking every tile of the world and drawing a ball or energizer if we find them
            if (activeLevel[i][j] & MapObjectBall) {
                _dot->setPosition(getXcoordinate(i), getYcoordinate(j));
                _dot->Draw(dest);
            }
            else if (activeLevel[i][j] & MapObjectEnergizer) {
                _energizer->setPosition(getXcoordinate(i), getYcoordinate(j));
                _energizer->Draw(dest);
            }
        }
    }
}

int CMap::getObjectAt(int x, int y)
{
    // bounds hack for tunnel traveling
    if (x < 0) {
        x = MAP_WIDTH + x;
    }
    else if (x >= MAP_WIDTH) {
        x = x - MAP_WIDTH;
    }
    
    if (y < 0) {
        y = MAP_HEIGHT + y;
    }
    else if (y >= MAP_HEIGHT) {
        y = y - MAP_HEIGHT;
    }
    
    assert(x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT);
    return activeLevel[x][y];
}

void CMap::setObjectAt(int object, int x, int y)
{
    // used to remove balls and energizers
    assert(x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT);
    activeLevel[x][y] = object;
}

void CMap::Reset()
{
    // initializing our level with default values
    for (int i = 0; i < MAP_WIDTH; i++) {
        for (int j = 0; j < MAP_HEIGHT; j++) {
            activeLevel[i][j] = defaultLevel[i][j];
        }
    }
}

CMap::CMap()
{
    // just a background image of the maze, enough for pacman
    _background = new CSprite("background.png");
    _background->setPosition(0, 0);
    
    _dot = new CSprite("dot.png");
    _energizer = new CSprite("energizer.png");
    
    Reset();
}

CMap::~CMap()
{
    delete _background;
    _background = NULL;
    
    delete _dot;
    _dot = NULL;
    
    delete _energizer;
    _energizer = NULL;
}
