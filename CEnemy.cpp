//
//  CEnemy.cpp
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/19/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#include "CEnemy.h"
#include <math.h>

inline int min(int a, int b)
{
    return a < b ? a : b; // returns minimal number from the given
}

CEnemy::CEnemy(CMap* map) : CEntity(map)
{
    // loading frightened ghost
    _frightenedSprite[0] = new CSprite("fright1.png");
    _frightenedSprite[1] = new CSprite("fright2.png");
    
    // loading eyes
    char directions[4] = { 'u', 'd', 'l', 'r' };
    char filename[50];
    
    for (int i = 0; i < 4; i++) {
        sprintf(filename, "eyes_%c.png", directions[i]);
        _eyesSprite[i] = new CSprite(filename);
    }
}

bool CEnemy::ableToMove(int xAdd, int yAdd)
{
    // inside the house
    bool gate = _map->getObjectAt(CMap::getTileXcoordinate(_pixelX + TILE_SIZE * 0.5f),
                                              CMap::getTileYcoordinate(_pixelY + TILE_SIZE * 0.5f)) & MapObjectGate;
    bool house = (xAdd != 0) && _map->getObjectAt(CMap::getTileXcoordinate(_pixelX + TILE_SIZE * 0.5f) + xAdd,
                                                             CMap::getTileYcoordinate(_pixelY + TILE_SIZE * 0.5f)) & MapObjectGate;

    // road and checking gate and house
    bool able = CEntity::ableToMove(xAdd, yAdd) || (gate && yAdd < 0) || (house && !gate);

    if (_state == EnemyStateFrighten) {
        return able;
    }
    else if (_state == EnemyStateEyes) {
        // a little bit different house situation when returning as eyes
        house = _map->getObjectAt(CMap::getTileXcoordinate(_pixelX + TILE_SIZE * 0.5f) + xAdd,
                                  CMap::getTileYcoordinate(_pixelY + TILE_SIZE * 0.5f) + yAdd) & MapObjectGate;
        
        return CEntity::ableToMove(xAdd, yAdd) || (gate && yAdd > 0) || house;
    }
    else {
        // there are 4 tiles where ghost can't go upwards
        bool restriction = yAdd < 0 && _map->getObjectAt(CMap::getTileXcoordinate(_pixelX + TILE_SIZE * 0.5f),
                                             CMap::getTileYcoordinate(_pixelY + TILE_SIZE * 0.5f)) & MapObjectRestriction;
        return able && !restriction;
    }
}

int CEnemy::distance(int targetx, int targety, int x, int y)
{
    // plain distance, non euclidean
    return (int)sqrtf(powf(targetx - x, 2) + powf(targety - y, 2));
}

Direction CEnemy::chooseDirection(int targetx, int targety)
{
    int currentTileX = getXposition();
    int currentTileY = getYposition();
    
    // gathering distances, adding bonus distance for invert directions so they can't turn back, adding bonus if we can't move there aswell
    int upWay = distance(targetx, targety, currentTileX, currentTileY - 1) + (_direction == DirectionDown ? 1000 : 0) + (ableToMove(0, -1) ? 0 : 1000);
    int leftWay = distance(targetx, targety, currentTileX - 1, currentTileY) + (_direction == DirectionRight ? 1000 : 0) + (ableToMove(-1, 0) ? 0 : 1000);
    int downWay = distance(targetx, targety, currentTileX, currentTileY + 1) + (_direction == DirectionUp ? 1000 : 0) + (ableToMove(0, 1) ? 0 : 1000);
    int rightWay = distance(targetx, targety, currentTileX + 1, currentTileY) + (_direction == DirectionLeft ? 1000 : 0) + (ableToMove(1, 0) ? 0 : 1000);
    
    // finding best distance, ie shortest one, ie without added bonus distances aswell
    int bestDistance = min(upWay, min(leftWay, min(downWay, rightWay)));
    
    if (upWay == bestDistance) {
        return DirectionUp;
    }
    else if (leftWay == bestDistance) {
        return DirectionLeft;
    }
    else if (downWay == bestDistance) {
        return DirectionDown;
    }
    
    return DirectionRight;
}

void CEnemy::Update()
{
    CEntity::Update(); // animation and movement stuff
    
    // handling new movement in different states
    if (!_moving && _state == EnemyStateScatter)
    {
        Move(chooseDirection(_scatterX, _scatterY)); // try to move to our scatter corner
    }
    else if (!_moving && _state == EnemyStateChase)
    {
        Move(chooseDirection(_chaseX, _chaseY)); // try to move to our chase position (different for every ghost)
    }
    else if (!_moving && _state == EnemyStateFrighten)
    {
        while (true) { // just move in random direction
            int direction = rand() % 4;
            if (!isReverse((Direction)direction)) {
                Move((Direction)direction);
                if (_moving) {
                    break;
                }
            }
        }
    }
    else if (!_moving && _state == EnemyStateEyes)
    {
        if (getXposition() == _startX && getYposition() == _startY) {
            Reset(); // reset our states when home reached
            
            _state = EnemyStateChase;
            Update(); // start chase over
        }
        else {
            Move(chooseDirection(_startX, _startY)); // go home
        }
    }
}

void CEnemy::RetreatHome()
{
    if (_state != EnemyStateEyes) {
        changeState(EnemyStateEyes); // we got eaten
        Move(chooseDirection(_startX, _startY));
    }
}

void CEnemy::Draw(SDL_Surface *dest)
{
    if (_state == EnemyStateFrighten) {
        // draw frightened ghost instead
        int frame = _animator->getCurrentFrame();
        _frightenedSprite[frame]->setPosition(_pixelX, _pixelY);
        _frightenedSprite[frame]->Draw(dest);
    }
    else if (_state == EnemyStateEyes) {
        // draw eyes instead, they have no animation
        _eyesSprite[_direction]->setPosition(_pixelX, _pixelY);
        _eyesSprite[_direction]->Draw(dest);
    }
    else {
        CEntity::Draw(dest);
    }
}

EnemyState CEnemy::currentState()
{
    return _state;
}

void CEnemy::changeState(EnemyState newState)
{
    // reversing direction if we were not frightened or inactive
    if (_state != EnemyStateFrighten && _state != EnemyStateNone) {
        reverseDirection();
    }
    
    _state = newState;
}

void CEnemy::Reset()
{
    // reseting all positions to default
    _state = EnemyStateNone;
    _direction = DirectionUp;
    _moving = false;
    _chaseX = 0;
    _chaseY = 0;
    _pixelX = CMap::getXcoordinate(_startX) + TILE_SIZE * 0.5f;
    _pixelY = CMap::getYcoordinate(_startY);
}

CEnemy::~CEnemy()
{
    for (int i = 0; i < 2; i++) {
        delete _frightenedSprite[i];
        _frightenedSprite[i] = NULL;
    }
    
    for (int i = 0; i < 4; i++) {
        delete _eyesSprite[i];
        _eyesSprite[i] = NULL;
    }
}