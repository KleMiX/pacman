//
//  CSprite.h
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/17/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#ifndef __PacMan__CSprite__
#define __PacMan__CSprite__

#include <SDL/SDL.h>

class CSprite {
public:
    CSprite(const char* file);
    ~CSprite();
    void setPosition(int x, int y);
    int getPositionX();
    int getPositionY();
    
    void Draw(SDL_Surface* dest);
    
private:
    SDL_Surface* _surface = NULL;
    SDL_Rect pos;
};

#endif /* defined(__PacMan__CSprite__) */
