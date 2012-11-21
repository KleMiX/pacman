//
//  CAnimator.h
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/18/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#ifndef __PacMan__CAnimator__
#define __PacMan__CAnimator__

class CAnimator {
private:
    int _currentFrame;
    int  _frameDelay; // milliseconds
    long _oldTime;
    int _maxFrames;
    
public:
    CAnimator();
    
    void Update();

    void setFrameDelay(int delay);
    void setCurrentFrame(int frame);
    int getCurrentFrame();
    void setMaxFrames(int frameCount);
    int getMaxFrames();
};

#endif /* defined(__PacMan__CAnimator__) */
