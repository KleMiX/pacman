//
//  CText.cpp
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/20/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#include "CText.h"
#include <assert.h>

CText::CText()
{
    color.r = color.g = color.b = 255; // white
    // loading and storing our image
    _font = TTF_OpenFont("Arial.ttf", 16);
}

void CText::Draw(SDL_Surface* dest, const char* text)
{
    assert(dest && text);
    
    _surface = TTF_RenderText_Solid(_font, text, color);
    SDL_BlitSurface(_surface, NULL, dest, &pos);
    SDL_FreeSurface(_surface);
}

CText::~CText()
{
    TTF_CloseFont(_font);
}

void CText::setPosition(int x, int y)
{
    pos.x = x;
    pos.y = y;
}

int CText::getPositionX()
{
    return pos.x;
}

int CText::getPositionY()
{
    return pos.y;
}