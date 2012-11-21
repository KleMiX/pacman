//
//  CEnemyPinky.cpp
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/19/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#include "CEnemyPinky.h"

CEnemyPinky::CEnemyPinky(CMap *map) : CEnemy(map)
{
    // loading Blinky sprites
    char directions[4] = { 'u', 'd', 'l', 'r' };
    char filename[50];
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            sprintf(filename, "pinky_%c%d.png", directions[i], j + 1);
            CEntity::loadSprite(filename, i, j);
        }
    }
    
    // setting starting position and scatter phase corner individually for every ghost
    _startX = 13;
    _startY = 17;
    _scatterX = 2;
    _scatterY = 0;
    _chaseX = 0;
    _chaseY = 0;
    
    Reset(); // resets values and starts movement
}

void CEnemyPinky::setPlayerPositionAndDirection(int x, int y, Direction direction)
{
    // trying to get ahead of player
    switch (direction) {
        case DirectionUp:
            x -= 4; // original bug in pacman
            y -= 4;
            break;
        case DirectionDown:
            y += 4;
            break;
        case DirectionLeft:
            x -= 4;
            break;
        case DirectionRight:
            x += 4;
            break;
        default:
            break;
    }
    
    _chaseX = x;
    _chaseY = y;
}