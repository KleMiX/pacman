//
//  Loop.h
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/17/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#ifndef __PacMan__Loop__
#define __PacMan__Loop__

#include <SDL/SDL.h>
#include "CText.h"
#include "CTimer.h"
#include "CMap.h"
#include "CPlayer.h"
#include "CEnemyBlinky.h"
#include "CEnemyClyde.h"
#include "CEnemyPinky.h"
#include "CEnemyInky.h"

enum GameState {
    GameStateNone = 0,
    GameStateNewGame,
    GameStateGame,
    GameStateNewLevel,
    GameStatePaused,
    GameStateOver,
};

// main game class - loop of the game
class CLoop {
private:
    // system variables
    bool _running;
    SDL_Surface* _screenSurface;
    GameState _state;
    
    // Game world objects
    CMap* _map;
    CPlayer* _player;
    CEnemyBlinky* _blinky;
    CEnemyClyde* _clyde;
    CEnemyInky* _inky;
    CEnemyPinky* _pinky;
    
    // Game statistics
    int _lives;
    int _dotsEaten;
    int _dotsEatenAfterLife;
    int _score;
    int _highscore;
    int _ghostsEaten;
    
    // Game HUD
    CSprite* _lifeHud;
    CText* _textHud;
    
    // Game state timers
    CTimer* _chaseTimer;
    CTimer* _frightTimer;
    CTimer* _scatterTimer;
    CTimer* _slackTimer;
    CTimer* _pointsTimer;
    int _scatterPhases;
    EnemyState _enemyState;
    EnemyState _oldEnemyState;
    Direction _directionPressed;

    // helper methods
    void changeState(GameState state);
    void changeEnemyState(EnemyState state);
    void startChaseMode();
    void startScatterMode();
    void startFrightenMode();
    void resetPositions();
    
public:
    CLoop();
    
    // actual loop implementation methods
    int OnExecute();
    bool OnInit();
    void OnEvent(SDL_Event* Event);
    void OnLoop();
    void OnRender();
    void OnCleanup();
};

#endif /* defined(__PacMan__Loop__) */
