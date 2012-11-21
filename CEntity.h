//
//  CEntity.h
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/19/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#ifndef __PacMan__CEntity__
#define __PacMan__CEntity__

#include "CSprite.h"
#include "CMap.h"
#include "CAnimator.h"

enum Direction {
    DirectionUp = 0,
    DirectionDown = 1,
    DirectionLeft = 2,
    DirectionRight = 3,
    DirectionNone = 4,
};

class CEntity {
public:
    CEntity(CMap *map);
    ~CEntity();
    
    void Draw(SDL_Surface *dest);
    void Update();
    
    void Move(Direction direction);
    
    int getXposition();
    int getYposition();
    
    virtual void Reset() = 0;
    
protected:
    int _entitySpeed; // milliseconds
    int _pixelX, _pixelY; // drawing coordinates
    int _targetX, _targetY; // target to move
    bool _moving;
    Direction _direction;
    
    CMap* _map;
    CAnimator* _animator;
    
    virtual bool ableToMove(int xAdd, int yAdd);
    virtual int sign(int eq); // override if need to change speed drastically
    void loadSprite(const char* filename, int direction, int frame);
    void reverseDirection();
    bool isReverse(Direction direction);
    
private:
    long _ticks; // used for speed check
    CSprite* _entitySprite[4][2];
};

#endif /* defined(__PacMan__CEntity__) */
