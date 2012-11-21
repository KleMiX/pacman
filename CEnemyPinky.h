//
//  CEnemyPinky.h
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/19/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#ifndef __PacMan__CEnemyPinky__
#define __PacMan__CEnemyPinky__

#include "CEnemy.h"

class CEnemyPinky : public CEnemy {
public:
    CEnemyPinky(CMap *map);
    
    void setPlayerPositionAndDirection(int x, int y, Direction direction);
};

#endif /* defined(__PacMan__CEnemyPinky__) */
