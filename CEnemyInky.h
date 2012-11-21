//
//  CEnemyInky.h
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/19/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#ifndef __PacMan__CEnemyInky__
#define __PacMan__CEnemyInky__

#include "CEnemy.h"

class CEnemyInky : public CEnemy {
public:
    CEnemyInky(CMap *map);
    
    void setPlayerPositionAndDirection(int x, int y, Direction direction);
    void setBlinkyPos(int x, int y); // needed for AI
    
private:
    int _blinkyX, _blinkyY;
};

#endif /* defined(__PacMan__CEnemyInky__) */
