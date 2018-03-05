#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
#include "BoardQueue.hpp"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    Board board;
    Side player_side;
    Side opponent_side;
    time_t t;

    int bestX = -1;
    int bestY = -1;
    int nextBestX = -1;
    int nextBestY = -1;


    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;

  private:
    int getBoardScore(Board * b, Side side);
    void BFS(double limit, BoardQueue * q);
    void enqueue_boardState(BoardState * bs, BoardQueue * q);


    

};


#endif
