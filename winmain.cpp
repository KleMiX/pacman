#include "CLoop.h"

// windows main function, just starts the game
int main(int argc, char** argv){
    CLoop* loop = new CLoop();

	loop->OnExecute();

    return 0;
}