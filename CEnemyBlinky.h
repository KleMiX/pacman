//
//  CEnemyBlinky.h
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/19/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#ifndef __PacMan__CEnemyBlinky__
#define __PacMan__CEnemyBlinky__

#include "CEnemy.h"

class CEnemyBlinky : public CEnemy {
public:
    CEnemyBlinky(CMap *map);
    
    void Reset();
    void setPlayerPositionAndDirection(int x, int y, Direction direction);
};

#endif /* defined(__PacMan__CEnemyBlinky__) */
