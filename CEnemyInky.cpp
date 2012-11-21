//
//  CEnemyInky.cpp
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/19/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#include "CEnemyInky.h"

CEnemyInky::CEnemyInky(CMap *map) : CEnemy(map)
{
    // loading Blinky sprites
    char directions[4] = { 'u', 'd', 'l', 'r' };
    char filename[50];
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            sprintf(filename, "inky_%c%d.png", directions[i], j + 1);
            CEntity::loadSprite(filename, i, j);
        }
    }
    
    // setting starting position and scatter phase corner individually for every ghost
    _startX = 11;
    _startY = 17;
    _scatterX = 27;
    _scatterY = 34;
    _chaseX = 0;
    _chaseY = 0;
    
    Reset(); // resets values and starts movement
}

// required for his personality
void CEnemyInky::setBlinkyPos(int x, int y)
{
    _blinkyX = x;
    _blinkyY = y;
}

void CEnemyInky::setPlayerPositionAndDirection(int x, int y, Direction direction)
{
    // setting target tile based on blinky's position and pacman's direction
    switch (direction) {
        case DirectionUp:
            x -= 2;  // original bug in pacman
            y -= 2;
            break;
        case DirectionDown:
            y += 2;
            break;
        case DirectionLeft:
            x -= 2;
            break;
        case DirectionRight:
            x += 2;
            break;
        default:
            break;
    }

    _chaseX = (x - _blinkyX) * 2 + _blinkyX;
    _chaseY = (y - _blinkyY) * 2 + _blinkyY;
}