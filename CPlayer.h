//
//  CPlayer.h
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/18/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#ifndef __PacMan__CPlayer__
#define __PacMan__CPlayer__

#include "CEntity.h"

class CPlayer : public CEntity {
public:
    CPlayer(CMap *map);
    
    void Update();
    void Move(Direction direction);
    void Reset();
    bool isEnergized();
    void resetEnergize();
    void resetDots();
    int dotsEaten();
    
    Direction getDirection();
private:
    int _consumedFood;
    bool _energized;
    
    void consumeFood(); // consumes dots, energizers and cherrys
    int sign(int eq);
};

#endif /* defined(__PacMan__CPlayer__) */
