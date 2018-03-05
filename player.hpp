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
    void BFS(double limit, BoardQueue &q);


    void enqueue_boardState(BoardState * bs, BoardQueue &q) {

      int n_depth = (bs->depth) + 1;
      Side n_side = (n_depth & 2 == 0) ? BLACK : WHITE;
      Move * anc = bs->ancestor;
      Board * old = bs->board;

      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          Move m(i, j);
          if (old.checkMove(&m, n_side)) {
            Board * copy  = old->copy();
            copy->doMove(&m, n_side); //Make the move on the copy and then evaluate it.
            q->enqueue(new BoardState(copy, n_depth, anc));
          }
        }
      }
    }

};

static int board_weights[8][8]=
    {{20, -3, 4, 4, 4, 4, -3, 20},
     {-3, -5, 1, 1, 1, 1, -5, -3},
     { 4,  1, 1, 1, 1, 1,  1,  4},
     { 4,  1, 1, 1, 1, 1,  1,  4},
     { 4,  1, 1, 1, 1, 1,  1,  4},
     { 4,  1, 1, 1, 1, 1,  1,  4},
     {-3, -5, 1, 1, 1, 1, -5, -3},
     {20, -3, 4, 4, 4, 4, -3, 20}};




#endif
