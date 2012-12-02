//
//  CText.h
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/20/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#ifndef __PacMan__CText__
#define __PacMan__CText__

#include <SDL_ttf/SDL_ttf.h>

class CText {
public:
    CText();
    ~CText();
    void setPosition(int x, int y);
    int getPositionX();
    int getPositionY();
    
    void Draw(SDL_Surface* dest, const char* text);
    
private:
    SDL_Surface* _surface;
    TTF_Font* _font;
    SDL_Rect pos;
    SDL_Color color;
};

#endif /* defined(__PacMan__CText__) */
