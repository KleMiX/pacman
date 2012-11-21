//
//  CMap.h
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/17/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#ifndef __PacMan__CMap__
#define __PacMan__CMap__

#include "CSprite.h"

// constants to define the level
static const int MAP_WIDTH = 28;
static const int MAP_HEIGHT = 36;
static const int TILE_SIZE = 16;
static const int OBJECT_OFFSET = 6;

// various objects that define our level and are static (i.e. won't move)
enum MapObject {
    MapObjectNone = 0,
    MapObjectRoad = 1,
    MapObjectWall = 2,
    MapObjectTunnel = 4,
    MapObjectRestriction = 8,
    MapObjectBall = 16,
    MapObjectEnergizer = 32,
    MapObjectGate = 64,
};

// class that defines a map-level around pacman and ghosts
class CMap {
public:
    CMap();
    ~CMap();
    
    // object manipulations
    int getObjectAt(int x, int y);
    void setObjectAt(int object, int x, int y);
    
    void Reset();
    
    // static methods for converting tile center coordinates to screen coordinates
    static int getXcoordinate(int xtile);
    static int getYcoordinate(int ytile);
    
    // static methods for converting screen coordinates to tile coordinates
    static int getTileXcoordinate(int xpixel);
    static int getTileYcoordinate(int ypixel);
    
    void Draw(SDL_Surface *dest);
    
private:
    // background of the map
    CSprite* _background;
    
    CSprite* _dot;
    CSprite* _energizer;
};

#endif /* defined(__PacMan__CMap__) */
