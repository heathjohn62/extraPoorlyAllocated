#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
#include "BoardQueue.hpp"
using namespace std;

//Change this define to true to do BFS, otherwise will do recursively
#define DOING_BFS false
#define MAX_DEPTH 6

class Player {

public:
    Player(Side side);
    ~Player();
    Board board;
    Side player_side;
    Side opponent_side;
    time_t t;

    //Alex's global variables.
    int to_move_x;
    int to_move_y;

    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;

  private:
    int getBoardScore(Board * b, Side side);
    int * BFS(double limit, BoardQueue * q);
    void enqueue_boardState(BoardState * bs, BoardQueue * q);

    //Alex's
    int recursiveMoveFind(Board * b, int depth);


};


#endif
