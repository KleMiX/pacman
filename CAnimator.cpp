//
//  CAnimator.cpp
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/18/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#include "CAnimator.h"
#include <SDL/SDL.h>

CAnimator::CAnimator()
{
    _currentFrame = 0;
    _maxFrames = 0;
    _frameDelay = 100; // 100 milliseconds is 10 fps
    _oldTime = 0;
}

void CAnimator::Update()
{
    if(_oldTime + _frameDelay > SDL_GetTicks()) {
        return; // a counter that does return if our frame delay isn't passed
    }
    
    // update the time and frame
    _oldTime = SDL_GetTicks();
    _currentFrame++;
    
    if(_currentFrame >= _maxFrames) {
        _currentFrame = 0; // be sure to reset frames if we reach max
    }
}

void CAnimator::setFrameDelay(int delay)
{
    _frameDelay = delay;
}

void CAnimator::setCurrentFrame(int frame)
{
    _currentFrame = frame;
}

int CAnimator::getCurrentFrame()
{
    return _currentFrame;
}

void CAnimator::setMaxFrames(int frameCount)
{
    _maxFrames = frameCount;
}

int CAnimator::getMaxFrames()
{
    return _maxFrames;
}
