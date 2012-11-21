//
//  CTimer.h
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/19/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#ifndef __PacMan__CTimer__
#define __PacMan__CTimer__

class CTimer {
private:
    long _currentTime;
    bool _running;
    int _seconds;
    
public:
    CTimer();
    
    void Update();
    void Reset();
    void Start();
    void Stop();
    void Pause();
    int getTime(); // seconds
    bool Running();
};

#endif /* defined(__PacMan__CTimer__) */
