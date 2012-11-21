//
//  Loop.cpp
//  PacMan
//
//  Created by Vsevolod Klementjev on 11/17/12.
//  Copyright (c) 2012 Vsevolod Klementjev. All rights reserved.
//

#include "CLoop.h"
#include <SDL_ttf/SDL_ttf.h>

// scatter phase lengths in seconds
const int FIRST_SCATTER = 7;
const int SECOND_SCATTER = 7 + FIRST_SCATTER;
const int THIRD_SCATTER = 5 + SECOND_SCATTER;
const int FOURTH_SCATTER = 5 + THIRD_SCATTER;

// chase phase lengths in seconds
const int FIRST_CHASE = 20;
const int SECOND_CHASE = 20 + FIRST_CHASE;
const int THIRD_CHASE = 20 + SECOND_CHASE;

// fright phase length in seconds
const int FRIGHT_TIME = 6;

// time in seconds, how long PacMan can slack by not eating dots, till next ghost is activated
const int SLACK_TIME = 4;

// how many dots should PacMan eat till given ghost will be free
const int INKY_DOTS_EXIT = 30;
const int CLYDE_DOTS_EXIT = 60 + INKY_DOTS_EXIT;

// how many dots should PacMan eat after life loss, till given ghost will be free
const int PINKY_DOTS_RESPAWN = 7;
const int INKY_DOTS_RESPAWN = 17;
const int CLYDE_DOTS_RESPAWN = 32;

// lives at the start of the game
const int BASE_LIVES = 3;

// score per ghost when being ate by PacMan
const int GHOST_SCORE = 200;

void CLoop::startChaseMode()
{
    // resets timers and puts enemies to chase mode
    _scatterTimer->Pause();
    _chaseTimer->Start();
    
    changeEnemyState(EnemyStateChase);
}

void CLoop::startScatterMode()
{
    // resets timers, counts new scatter phase and puts enemies to scatter mode
    _scatterPhases++;
    
    _chaseTimer->Pause();
    _scatterTimer->Start();
    
    changeEnemyState(EnemyStateScatter);
}

void CLoop::startFrightenMode()
{
    // pauses timers and puts enemies to fright mode
    _chaseTimer->Pause();
    _scatterTimer->Pause();
    _frightTimer->Start();
    
    changeEnemyState(EnemyStateFrighten);
}

void CLoop::OnLoop()
{
    // actual what to do if it's a game now
    if (_state == GameStateGame)
    {
        // player stuff, counting eaten dots
        if (_directionPressed != DirectionNone) {
            _player->Move(_directionPressed); // user is holding button down
        }
        _player->Update();
        
        // a new dot has been eaten
        if (_dotsEaten != _player->dotsEaten()) {
            _slackTimer->Reset(); // player is active
            if (_dotsEatenAfterLife) {
                _dotsEatenAfterLife++; // count first dots eaten after new life is started
            }
        }
        _dotsEaten = _player->dotsEaten();
        
        
        // win check (maybe should get that number from map, counted from static map, but since we won't change it...)
        if (_dotsEaten == 244) {
            changeState(GameStateNewLevel);
        }
        
        // let ghosts know where player is
        _blinky->setPlayerPositionAndDirection(_player->getXposition(), _player->getYposition(), _player->getDirection());
        _inky->setBlinkyPos(_blinky->getXposition(), _blinky->getYposition());
        _inky->setPlayerPositionAndDirection(_player->getXposition(), _player->getYposition(), _player->getDirection());
        _pinky->setPlayerPositionAndDirection(_player->getXposition(), _player->getYposition(), _player->getDirection());
        _clyde->setPlayerPositionAndDirection(_player->getXposition(), _player->getYposition(), _player->getDirection());
        
        // updating ghosts
        _blinky->Update();
        _inky->Update();
        _pinky->Update();
        _clyde->Update();
        
        // time
        _chaseTimer->Update();
        _frightTimer->Update();
        _scatterTimer->Update();
        _slackTimer->Update();
        _pointsTimer->Update();
        
        // switching states as we need
        if (_enemyState == EnemyStateScatter) {
            if ((_scatterPhases == 1 && _scatterTimer->getTime() >= FIRST_SCATTER) ||
                (_scatterPhases == 2 && _scatterTimer->getTime() >= SECOND_SCATTER) ||
                (_scatterPhases == 3 && _scatterTimer->getTime() >= THIRD_SCATTER) ||
                (_scatterPhases == 4 && _scatterTimer->getTime() >= FOURTH_SCATTER))
            {
                startChaseMode();
            }
        }
        else if (_enemyState == EnemyStateChase && _scatterPhases < 4) {
            if ((_scatterPhases == 1 && _chaseTimer->getTime() >= FIRST_CHASE) ||
                (_scatterPhases == 2 && _chaseTimer->getTime() >= SECOND_CHASE) ||
                (_scatterPhases == 3 && _chaseTimer->getTime() >= THIRD_CHASE))
            {
                startScatterMode();
            }
        }
        else if (_enemyState == EnemyStateFrighten) {
            if (_frightTimer->getTime() >= FRIGHT_TIME) {
                _frightTimer->Stop();
                
                // switching to the state that was active before fright
                if (_oldEnemyState == EnemyStateScatter) {
                    startScatterMode();
                    _scatterPhases--; // don't forget we mess up
                }
                else {
                    startChaseMode();
                }
            }
        }
        
        // Free the inky
        if (_dotsEaten >= INKY_DOTS_EXIT && _inky->currentState() == EnemyStateNone && _dotsEatenAfterLife == 0) {
            _inky->changeState(_enemyState);
        }
        if (_dotsEatenAfterLife >= INKY_DOTS_RESPAWN && _inky->currentState() == EnemyStateNone) {
            _inky->changeState(_enemyState);
        }
        
        // free the clyde
        if (_dotsEaten >= CLYDE_DOTS_EXIT && _clyde->currentState() == EnemyStateNone && _dotsEatenAfterLife == 0) {
            _clyde->changeState(_enemyState);
        }
        if (_dotsEatenAfterLife >= CLYDE_DOTS_RESPAWN && _clyde->currentState() == EnemyStateNone) {
            _clyde->changeState(_enemyState);
        }
        
        // free the pinky (after life loss he's not exiting at start
        if (_dotsEatenAfterLife >= PINKY_DOTS_RESPAWN && _pinky->currentState() == EnemyStateNone) {
            _pinky->changeState(_enemyState);
        }
        
        // free some ghosts if PacMan is not eating dots for 4 secs
        if (_slackTimer->getTime() >= 4) {
            if (_pinky->currentState() == EnemyStateNone) { // freeing in order pinky > inky > clyde
                _pinky->changeState(_enemyState);
            }
            else if (_inky->currentState() == EnemyStateNone) {
                _inky->changeState(_enemyState);
            }
            else if (_clyde->currentState() == EnemyStateNone) {
                _clyde->changeState(_enemyState);
            }
            
            _slackTimer->Reset(); // reset the time
        }
        
        // fright phase start
        if (_player->isEnergized()) {
            _ghostsEaten = 0;
            _player->resetEnergize(); // consume the buff
            _score += 40;
            startFrightenMode();
        }
        
        // clearer view
        int px = _player->getXposition();
        int py = _player->getYposition();
        int bx = _blinky->getXposition();
        int by = _blinky->getYposition();
        int ix = _inky->getXposition();
        int iy = _inky->getYposition();
        int pix = _pinky->getXposition();
        int piy = _pinky->getYposition();
        int cx = _clyde->getXposition();
        int cy = _clyde->getYposition();
        CEnemy* collidedEnemy = NULL;
        
        // Collision with the ghost
        if (px == bx && py == by && _blinky->currentState() != EnemyStateEyes) {
            collidedEnemy = _blinky;
        }
        if (px == ix && py == iy && _inky->currentState() != EnemyStateEyes) {
            collidedEnemy = _inky;
        }
        if (px == pix && py == piy && _pinky->currentState() != EnemyStateEyes) {
            collidedEnemy = _pinky;
        }
        if (px == cx && py == cy && _clyde->currentState() != EnemyStateEyes) {
            collidedEnemy = _clyde;
        }
        
        // collision results -> who eats who
        if (collidedEnemy) {
            if (collidedEnemy->currentState() == EnemyStateFrighten) {
                collidedEnemy->RetreatHome();
                _ghostsEaten++;
                
                _score += _ghostsEaten * GHOST_SCORE; // bonus score for additional ghosts being ate
                _pointsTimer->Start();
            }
            else {
                changeState(GameStatePaused);
            }
        }
        
        // stop showing score of eaten ghost after 2 secs
        if (_pointsTimer->getTime() >= 2) {
            _pointsTimer->Stop();
        }
    }
}

void CLoop::OnEvent(SDL_Event* Event) {
    // system stuff
    if(Event->type == SDL_QUIT) {
        _running = false;
    }
    else if (Event->type == SDL_KEYUP) {
        if (Event->key.keysym.sym == SDLK_ESCAPE) {
            _running = false; // quit on Escape
            return;
        }
        else if (_state != GameStateGame) {
            changeState(GameStateGame); // Launch the game on any key ...
            OnEvent(Event); // ... and process key press now
            return;
        }
        
        if (_state == GameStateGame) { // letting our direction know that key is no longer pressed
            _directionPressed = DirectionNone;
        }
    }
    else if (Event->type == SDL_KEYDOWN && _state == GameStateGame) 
    {
        // moving pacman if game is in progress, last key down will try to move pacman in that direction
        switch (Event->key.keysym.sym) {
            case SDLK_UP:
                _player->Move(DirectionUp);
                _directionPressed = DirectionUp;
                break;
            case SDLK_DOWN:
                _player->Move(DirectionDown);
                _directionPressed = DirectionDown;
                break;
            case SDLK_LEFT:
                _player->Move(DirectionLeft);
                _directionPressed = DirectionLeft;
                break;
            case SDLK_RIGHT:
                _player->Move(DirectionRight);
                _directionPressed = DirectionRight;
                break;
            default:
                break;
        }
    }
}

bool CLoop::OnInit() {
    // init system stuff
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }
    
    if((_screenSurface = SDL_SetVideoMode(448, 576, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
        return false;
    }
    
    if (TTF_Init() < 0) {
        return false;
    }
    
    // init our game objects
    _map = new CMap();
    _player = new CPlayer(_map);
    _blinky = new CEnemyBlinky(_map);
    _inky = new CEnemyInky(_map);
    _pinky = new CEnemyPinky(_map);
    _clyde = new CEnemyClyde(_map);
    
    // timers
    _chaseTimer = new CTimer();
    _frightTimer = new CTimer();
    _scatterTimer = new CTimer();
    _slackTimer = new CTimer();
    _pointsTimer = new CTimer();
    
    // stats
    _state = GameStateNewGame;
    _lives = BASE_LIVES; // TODO: 
    _dotsEaten = 0;
    _dotsEatenAfterLife = 0; // TODO:
    _score = 0; // TODO: 
    _highscore = 0; // TODO: 
    _ghostsEaten = 0; // TODO:
    _directionPressed = DirectionNone;
    
    // HUD
    _lifeHud = new CSprite("pml2.png");
    _textHud = new CText();
    
    return true;
}

void CLoop::resetPositions()
{
    // return to start positions and states
    _player->Reset(); 
    _blinky->Reset();
    _inky->Reset();
    _pinky->Reset();
    _clyde->Reset();
}

void CLoop::changeState(GameState state)
{
    // == Start of the (not new - any) game ==
    if (state == GameStateGame) {
        _player->Move(DirectionLeft); // start moving player
        
        // letting know where player
        _blinky->setPlayerPositionAndDirection(_player->getXposition(), _player->getYposition(), _player->getDirection());
        _inky->setPlayerPositionAndDirection(_player->getXposition(), _player->getYposition(), _player->getDirection());
        _pinky->setPlayerPositionAndDirection(_player->getXposition(), _player->getYposition(), _player->getDirection());
        _clyde->setPlayerPositionAndDirection(_player->getXposition(), _player->getYposition(), _player->getDirection());
        
        // Activate Blinky now
        _blinky->changeState(EnemyStateScatter);
        
        if (_state != GameStatePaused) { // dont let pinky go if it's a life loss
            _pinky->changeState(EnemyStateScatter);
        }
        
        _scatterTimer->Start();
        _slackTimer->Start();
        _slackTimer->Reset();
        _scatterPhases = 1;
        _enemyState = EnemyStateScatter;
        _oldEnemyState = EnemyStateScatter;
    }
    
    // == Start of the NEW game ==
    if (state == GameStateGame && (_state == GameStateNewGame || _state == GameStateOver)) {
        _lives = BASE_LIVES;
    }
    
    // == Level completed or lost all lives (will reset score below) ==
    if (state == GameStateNewLevel || state == GameStateOver) {
        resetPositions();
        _map->Reset(); // will place new dots and energizers
        
        _chaseTimer->Stop();
        _frightTimer->Stop();
        _scatterTimer->Stop();
        _slackTimer->Stop();
        
        _score += _dotsEaten * 10;
        _player->resetDots();
        _dotsEaten = 0;
        _dotsEatenAfterLife = 0;
        
        if (_score > _highscore) {
            _highscore = _score;
        }
    }
    
    // == Lost all lives ==
    if (state == GameStateOver) {
        _score = 0;
    }
    
    // == Lost a life ==
    if (state == GameStatePaused) {
        _lives--;
        if (_lives < 0) {
            changeState(GameStateOver);
            return;
        }
        
        resetPositions();
        
        // pausing all events
        _chaseTimer->Pause();
        _frightTimer->Pause();
        _scatterTimer->Pause();
    }

    // == Start after life loss ==
    if (_state == GameStatePaused) {
        _dotsEatenAfterLife = 1; // start this counter by assigning 1
    }
    
    // temporal timer will be stopped always
    _pointsTimer->Stop();
    _directionPressed = DirectionNone;

    _state = state;
}

void CLoop::changeEnemyState(EnemyState state)
{
    _oldEnemyState = _enemyState; // used to return back to this state after frighten
    _enemyState = state;
    
    // change all ghost states if they are active
    if (_blinky->currentState() != EnemyStateNone && _blinky->currentState() != EnemyStateEyes) _blinky->changeState(_enemyState);
    if (_inky->currentState() != EnemyStateNone && _inky->currentState() != EnemyStateEyes) _inky->changeState(_enemyState);
    if (_pinky->currentState() != EnemyStateNone && _pinky->currentState() != EnemyStateEyes) _pinky->changeState(_enemyState);
    if (_clyde->currentState() != EnemyStateNone && _clyde->currentState() != EnemyStateEyes) _clyde->changeState(_enemyState);
}

void CLoop::OnRender() {
    // map and all entities
    _map->Draw(_screenSurface);
    _blinky->Draw(_screenSurface);
    _inky->Draw(_screenSurface);
    _pinky->Draw(_screenSurface);
    _clyde->Draw(_screenSurface);
    _player->Draw(_screenSurface);
    
    // Life HUD
    for (int i = 0; i < _lives * 2; i += 2) {
        _lifeHud->setPosition(CMap::getXcoordinate(2 + i), CMap::getYcoordinate(34) + TILE_SIZE * 0.5f);
        _lifeHud->Draw(_screenSurface);
    }
    
    // TEXT HUD: drawing various non-interesting stuff on the screen
    char scores[50];
    _textHud->setPosition(CMap::getXcoordinate(3), CMap::getYcoordinate(0) + TILE_SIZE * 0.5f);
    _textHud->Draw(_screenSurface, "Score");
    
    _textHud->setPosition(CMap::getXcoordinate(10), CMap::getYcoordinate(0) + TILE_SIZE * 0.5f);
    _textHud->Draw(_screenSurface, "Highscore");
    
    _textHud->setPosition(CMap::getXcoordinate(3), CMap::getYcoordinate(1) + TILE_SIZE * 0.5f);
    sprintf(scores, "%d", _score + _player->dotsEaten() * 10);
    _textHud->Draw(_screenSurface, scores);
    
    _textHud->setPosition(CMap::getXcoordinate(10), CMap::getYcoordinate(1) + TILE_SIZE * 0.5f);
    sprintf(scores, "%d", _highscore);
    _textHud->Draw(_screenSurface, scores);
    
    _textHud->setPosition(CMap::getXcoordinate(11), CMap::getYcoordinate(20) + TILE_SIZE * 0.5f);
    if (_state == GameStateNewGame) {
        _textHud->Draw(_screenSurface, "Press any key ...");
    }
    else if (_state == GameStateOver) {
        _textHud->Draw(_screenSurface, "! GAME OVER !");
    }
    else if (_state == GameStatePaused) {
        _textHud->Draw(_screenSurface, "Try again ...");
    }
    else if (_state == GameStateNewLevel) {
        _textHud->Draw(_screenSurface, "Level completed");
    }
    
    // points after eating ghost
    if (_pointsTimer->Running() && _ghostsEaten) {
        _textHud->setPosition(CMap::getXcoordinate(_player->getXposition()), CMap::getYcoordinate(_player->getYposition()));
        
        sprintf(scores, "%d", _ghostsEaten * GHOST_SCORE);
        _textHud->Draw(_screenSurface, scores);
    }
    
    SDL_Flip(_screenSurface);
}

void CLoop::OnCleanup() {
    delete _map;
    _map = NULL;
    
    delete _player;
    _player = NULL;
    
    delete _blinky;
    _blinky = NULL;
    delete _inky;
    _inky = NULL;
    delete _pinky;
    _pinky = NULL;
    delete _clyde;
    _clyde = NULL;
    
    delete _lifeHud;
    _lifeHud = NULL;
    delete _textHud;
    _textHud = NULL;
    
    delete _scatterTimer;
    _scatterTimer = NULL;
    delete _chaseTimer;
    _chaseTimer = NULL;
    delete _frightTimer;
    _frightTimer = NULL;
    delete _slackTimer;
    _slackTimer = NULL;
    delete _pointsTimer;
    _pointsTimer = NULL;
    
    SDL_Quit();
    TTF_Quit();
}

CLoop::CLoop() {
    _screenSurface = NULL;
    _running = true;
}

// actual loop
int CLoop::OnExecute() {
    if(OnInit() == false) {
        return -1;
    }
    
    SDL_Event Event;
    
    while(_running) {
        while(SDL_PollEvent(&Event)) {
            OnEvent(&Event);
        }
        
        OnLoop();
        OnRender();
    }
    
    OnCleanup();
    
    return 0;
}