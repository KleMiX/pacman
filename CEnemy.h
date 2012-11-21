//
//  CEnemy.h
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/19/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#ifndef __PacMan__CEnemy__
#define __PacMan__CEnemy__

#include "CEntity.h"

enum EnemyState {
    EnemyStateNone = 0,
    EnemyStateChase,
    EnemyStateScatter,
    EnemyStateFrighten,
    EnemyStateEyes,
};

class CEnemy : public CEntity {
public:
    CEnemy(CMap* map);
    ~CEnemy();
    void Update();
    
    void changeState(EnemyState state);
    EnemyState currentState();
    virtual void setPlayerPositionAndDirection(int x, int y, Direction direction) = 0;
    virtual void Reset(); // Blinky will override to change direction he is looking
    void RetreatHome();
    void Draw(SDL_Surface *dest);
    
protected:
    int _startX, _startY; // starting position
    int _chaseX, _chaseY;
    int _scatterX, _scatterY; // scatter corner pos
    
    EnemyState _state;
    
private:
    CSprite* _frightenedSprite[2];
    CSprite* _eyesSprite[4]; // sprites for another states
    
    int distance(int targetx, int targety, int x, int y);
    Direction chooseDirection(int targetx, int targety);
    
    bool ableToMove(int xAdd, int yAdd);
};

#endif /* defined(__PacMan__CEnemy__) */
