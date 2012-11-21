//
//  CEnemyClyde.h
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/19/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#ifndef __PacMan__CEnemyClyde__
#define __PacMan__CEnemyClyde__

#include "CEnemy.h"

class CEnemyClyde : public CEnemy {
public:
    CEnemyClyde(CMap *map);
    
    void setPlayerPositionAndDirection(int x, int y, Direction direction);
};

#endif /* defined(__PacMan__CEnemyClyde__) */
