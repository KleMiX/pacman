//
//  CEntity.cpp
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/19/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#include "CEntity.h"
#include <assert.h>

CEntity::CEntity(CMap *map)
{
    _map = map;
    _direction = DirectionLeft;
    _animator = new CAnimator();
    _animator->setMaxFrames(2);
    
    _entitySpeed = 500 / TILE_SIZE; // 500 ms per tile
    
    _moving = false;
    _pixelX = 0;
    _pixelY = 0;
    _targetX = 0;
    _targetY = 0;
    _ticks = 0;
}

CEntity::~CEntity()
{
    _map = NULL; // we don't own that
    
    delete _animator;
    _animator = NULL;
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 2; j++) {
            delete _entitySprite[i][j];
            _entitySprite[i][j] = NULL;
        }
    }
}

void CEntity::loadSprite(const char* filename, int direction, int frame)
{
    assert(direction >= 0 && direction < 4 && frame >= 0 && frame < 2);
    _entitySprite[direction][frame] = new CSprite(filename);
}


void CEntity::Draw(SDL_Surface *dest)
{
    if (_moving) {
        // get our frame number from animator, do animation only while moving
        int frame = _animator->getCurrentFrame();
        _entitySprite[_direction][frame]->setPosition(_pixelX, _pixelY);
        _entitySprite[_direction][frame]->Draw(dest);
    }
    else {
        _entitySprite[_direction][0]->setPosition(_pixelX, _pixelY);
        _entitySprite[_direction][0]->Draw(dest);
    }
}

void CEntity::Update()
{
    _animator->Update();
    
    if (_ticks + _entitySpeed < SDL_GetTicks()) { // checking speed
        _ticks = SDL_GetTicks();
        
        if (_moving) {
            // actual speed add here
            _pixelX -= sign(_pixelX - _targetX);
            _pixelY -= sign(_pixelY - _targetY);
            
            // tunnel traveling, out of bounds hack
            if (_pixelX < 0) {
                _pixelX = MAP_WIDTH * TILE_SIZE + _pixelX;
                _targetX = MAP_WIDTH * TILE_SIZE + _targetX;
            }
            else if (_pixelX >= MAP_WIDTH * TILE_SIZE) {
                _pixelX = _pixelX - MAP_WIDTH * TILE_SIZE;
                _targetX = _targetX - MAP_WIDTH * TILE_SIZE;
            }
            if (_pixelY < 0) {
                _pixelY = MAP_HEIGHT * TILE_SIZE + _pixelY;
                _targetY = MAP_HEIGHT * TILE_SIZE + _targetY;
            }
            else if (_pixelY >= MAP_HEIGHT * TILE_SIZE) {
                _pixelY = _pixelY - MAP_HEIGHT * TILE_SIZE;
                _targetY = _targetY - MAP_HEIGHT * TILE_SIZE;
            }
            
            // stop movement if we reach target tile's center
            if (_pixelX == _targetX && _pixelY == _targetY) {
                _moving = false;
            }
        }
    }
}

void CEntity::reverseDirection()
{
    switch (_direction) {
        case DirectionUp:
            _direction = DirectionDown;
            break;
        case DirectionDown:
            _direction = DirectionUp;
            break;
        case DirectionLeft:
            _direction = DirectionRight;
            break;
        case DirectionRight:
            _direction = DirectionLeft;
            break;
        default:
            break;
    }
    
    _moving = true;
}

bool CEntity::ableToMove(int xAdd, int yAdd)
{
    // checking if there is a road in an xAdd and yAdd offset tile
    return _map->getObjectAt(CMap::getTileXcoordinate(_pixelX + TILE_SIZE * 0.5f * xAdd * xAdd) + xAdd,
                             CMap::getTileYcoordinate(_pixelY + TILE_SIZE * 0.5f * yAdd * yAdd) + yAdd) & MapObjectRoad;
}

bool CEntity::isReverse(Direction direction)
{
    if (_direction == DirectionDown && direction == DirectionUp) {
        return true;
    }
    if (_direction == DirectionUp && direction == DirectionDown) {
        return true;
    }
    if (_direction == DirectionRight && direction == DirectionLeft) {
        return true;
    }
    if (_direction == DirectionLeft && direction == DirectionRight) {
        return true;
    }
    
    return false;
}

void CEntity::Move(Direction direction)
{
    // x and y offsets representing direction in a tile grid
    int xAdd = direction == DirectionRight ? 1 : direction == DirectionLeft ? -1 : 0;
    int yAdd = direction == DirectionDown ? 1 : direction == DirectionUp ? -1 : 0;
    
    if (ableToMove(xAdd, yAdd)) // if there's a road, start moving
    {
        _direction = direction;
        _moving = true;
        
        // setting next tile's target so we know where to stop
        _targetX = CMap::getXcoordinate(CMap::getTileXcoordinate(_pixelX) + xAdd);
        _targetY = CMap::getYcoordinate(CMap::getTileYcoordinate(_pixelY) + yAdd);
    }
}

int CEntity::sign(int v)
{
    // returns sign and an actual speed buff (should be power of 2)
    return v > 0 ? 2 : (v < 0 ? -2 : 0);
}

int CEntity::getXposition()
{
    return CMap::getTileXcoordinate(_pixelX);
}

int CEntity::getYposition()
{
    return CMap::getTileYcoordinate(_pixelY);
}