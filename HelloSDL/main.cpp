//
//  main.cpp
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/17/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#include "main.h"
#include "CLoop.h"

int SDL_main (int argc, char **argv)
{
    // just launching our game loop
    CLoop theApp;
    return theApp.OnExecute();
}
