//
//  CPlayer.cpp
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/18/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#include "CPlayer.h"

// drift range in pixels to make a turn before an actual turn
static const int CORNERING = 5;

CPlayer::CPlayer(CMap *map) : CEntity(map)
{
    // loading pac man sprites
    char directions[4] = { 'u', 'd', 'l', 'r' };
    char filename[50];
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            sprintf(filename, "pm%c%d.png", directions[i], j + 1);
            CEntity::loadSprite(filename, i, j);
        }
    }
    
    _consumedFood = 0; // won't be reset in reset
    Reset(); // resets values and starts movement
}

void CPlayer::Update()
{
    CEntity::Update();
    consumeFood(); // try to eat anything beneath us
    
    if (!_moving) {
        Move(_direction); // try to move if we stopped
    }
}

Direction CPlayer::getDirection()
{
    return _direction;
}

void CPlayer::Move(Direction direction)
{
    if (_moving) // since player will stop only hitting a wall
    {
        // just reverse
        if (isReverse(direction)) {
            reverseDirection();
        }
        
        // current direction in grid
        int currXAdd = _direction == DirectionRight ? 1 : _direction == DirectionLeft ? -1 : 0;
        int currYAdd = _direction == DirectionDown ? 1 : _direction == DirectionUp ? -1 : 0;
        
        // proposed direction in grid
        int xAdd = direction == DirectionRight ? 1 : direction == DirectionLeft ? -1 : 0;
        int yAdd = direction == DirectionDown ? 1 : direction == DirectionUp ? -1 : 0;
        
        // checking every direction including cornering
        bool yRight = yAdd != 0 && currXAdd == 1 && (_pixelX % TILE_SIZE - TILE_SIZE * 0.5f) <= CORNERING;
        bool yLeft = yAdd != 0 && currXAdd == -1 && (_pixelX % TILE_SIZE + TILE_SIZE * 0.5f) >= TILE_SIZE - CORNERING;
        
        bool xDown = xAdd != 0 && currYAdd == 1 && (_pixelY % TILE_SIZE - TILE_SIZE * 0.5f) <= CORNERING;
        bool xUp = xAdd != 0 && currYAdd == -1 && (_pixelY % TILE_SIZE + TILE_SIZE * 0.5f) >= TILE_SIZE - CORNERING;
        
        if ((yRight || yLeft || xDown || xUp) && ableToMove(xAdd, yAdd)) // make a turn if we can
        {
            _direction = direction;
            _moving = true;
            
            _targetX = CMap::getXcoordinate(CMap::getTileXcoordinate(_pixelX) + xAdd);
            _targetY = CMap::getYcoordinate(CMap::getTileYcoordinate(_pixelY) + yAdd);
        }
        
        return;
    }
    
    CEntity::Move(direction); // if we stopped and player is pressing a turn
}

// used to note if player picks up energizer buff
bool CPlayer::isEnergized()
{
    return _energized;
}

// used to reset energized buff when we activate fright phase in main loop
void CPlayer::resetEnergize()
{
    _energized = false;
}

int CPlayer::dotsEaten()
{
    return _consumedFood; // dots picked up
}

void CPlayer::resetDots()
{
    _consumedFood = 0; // resetting dot tracker when new lvl comes or game is over
}

int CPlayer::sign(int v)
{
    // left to change pacman speed for testing
    return v > 0 ? 2 : (v < 0 ? -2 : 0);
}

void CPlayer::consumeFood()
{
    int x = CMap::getTileXcoordinate(_pixelX);
    int y = CMap::getTileYcoordinate(_pixelY);
    
    if (_map->getObjectAt(x, y) & MapObjectBall) {
        // if there's a ball beneath us, consume it and remove from the map
        _consumedFood++;
        
        _map->setObjectAt(_map->getObjectAt(x, y) ^ MapObjectBall, x, y);
    }
    if (_map->getObjectAt(x, y) & MapObjectEnergizer) {
        // if there's an energizer beneath us, consume it, add energize buff and remove from the map
        _consumedFood++;
        _energized = true;
        
        _map->setObjectAt(_map->getObjectAt(x, y) ^ MapObjectEnergizer, x, y);
    }
}

void CPlayer::Reset()
{
    // resetting positions and buffs
    _energized = false;
    _moving = false;
    _pixelX = CMap::getXcoordinate(14) - TILE_SIZE * 0.5f;
    _pixelY = CMap::getYcoordinate(26);
    
    Move(DirectionLeft); // start moving right away
}
