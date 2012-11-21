//
//  CTimer.cpp
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/19/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#include "CTimer.h"
#include <SDL/SDL.h>

CTimer::CTimer()
{
    Stop();
}

void CTimer::Update()
{
    if (_running) {
        long diff = SDL_GetTicks() - _currentTime;
        
        if (diff > 1000) {
            _seconds += diff / 1000;
            _currentTime = SDL_GetTicks() + (diff % 1000);
        }
    }
}

bool CTimer::Running()
{
    return _running;
}

void CTimer::Reset()
{
    _currentTime = SDL_GetTicks();
    _seconds = 0;
}

void CTimer::Start()
{
    _running = true;
    _currentTime = SDL_GetTicks();
}

void CTimer::Stop()
{
    _running = false;
    Reset();
}

void CTimer::Pause()
{
    _running = false;
}

int CTimer::getTime()
{
    return _seconds;
}