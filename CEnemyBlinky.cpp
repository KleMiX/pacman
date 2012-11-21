//
//  CEnemyBlinky.cpp
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/19/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#include "CEnemyBlinky.h"

CEnemyBlinky::CEnemyBlinky(CMap *map) : CEnemy(map)
{
    // loading Blinky sprites
    char directions[4] = { 'u', 'd', 'l', 'r' };
    char filename[50];
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            sprintf(filename, "blinky_%c%d.png", directions[i], j + 1);
            CEntity::loadSprite(filename, i, j);
        }
    }

    // setting starting position and scatter phase corner individually for every ghost 
    _startX = 13;
    _startY = 14;
    _scatterX = 25;
    _scatterY = 0;
    _chaseX = 0;
    _chaseY = 0;
    
    Reset(); // resets values and starts movement
}

void CEnemyBlinky::Reset()
{
    CEnemy::Reset();
    
    _direction = DirectionLeft; // he's different
}

void CEnemyBlinky::setPlayerPositionAndDirection(int x, int y, Direction direction)
{
    _chaseX = x;
    _chaseY = y;
}