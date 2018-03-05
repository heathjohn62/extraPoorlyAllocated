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

    int board_weights[8][8]=
    {{20, -3, 1, 1, 1, 1, -3, 20},
     {-3, -5, 1, 1, 1, 1, -5, -3},
     { 4,  1, 1, 1, 1, 1,  1,  4},
     { 4,  1, 1, 1, 1, 1,  1,  4},
     { 4,  1, 1, 1, 1, 1,  1,  4},
     { 4,  1, 1, 1, 1, 1,  1,  4},
     {-3, -5, 1, 1, 1, 1, -5, -3},
     {20, -3, 1, 1, 1, 1, -3, 20}};
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif

/**
 * Returns a metric of the state of the board, where higher numbers are
 * better with respect to the side given as a parameter.
 *
 * @param board A board pointer which references the board to be evaluated
 * @param side The side with which the board is evaluated with respect to.
 */
int getBoardScore(Board * board, Side side)
{
    Side opposite;
    if (side == WHITE)
    {
        opposite = BLACK;
    }
    else
    {
        opposite = WHITE;
    }
    int boardScore = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board.get(side, i, j))
            {
                boardScore += board_weights[i][j];
            }
            else if (board.get(opposite, i, j))
            {
                boardScore -= board_weights[i][j];
            }
        }
    }
    return boardScore;
}
