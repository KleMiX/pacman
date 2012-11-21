//
//  CSprite.cpp
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/17/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#include "CSprite.h"
#include <SDL_image/SDL_image.h>
#include <assert.h>

CSprite::CSprite(const char* filename)
{
    // loading and storing our image
    SDL_Surface* temp = NULL;
    
    temp = IMG_Load(filename);
    assert(temp);
    
    _surface = SDL_DisplayFormatAlpha(temp);
    SDL_FreeSurface(temp);
}

void CSprite::Draw(SDL_Surface* dest)
{
    assert(dest);
    SDL_BlitSurface(_surface, NULL, dest, &pos);
}

CSprite::~CSprite()
{
    SDL_FreeSurface(_surface);
}

void CSprite::setPosition(int x, int y)
{
    pos.x = x;
    pos.y = y;
}

int CSprite::getPositionX()
{
    return pos.x;
}

int CSprite::getPositionY()
{
    return pos.y;
}