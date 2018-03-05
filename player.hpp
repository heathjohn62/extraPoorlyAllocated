#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    Board board;
    Side player_side;
    Side opponent_side;

    int bestX = -1;
    int bestY = -1;


    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;

  private:
    int getBoardScore(Board * b, Side side);
};

static int board_weights[8][8]=
    {{20, -3, 1, 1, 1, 1, -3, 20},
     {-3, -5, 1, 1, 1, 1, -5, -3},
     { 4,  1, 1, 1, 1, 1,  1,  4},
     { 4,  1, 1, 1, 1, 1,  1,  4},
     { 4,  1, 1, 1, 1, 1,  1,  4},
     { 4,  1, 1, 1, 1, 1,  1,  4},
     {-3, -5, 1, 1, 1, 1, -5, -3},
     {20, -3, 1, 1, 1, 1, -3, 20}};




#endif
