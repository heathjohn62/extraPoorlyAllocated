#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <algorithm> //Only used for finding an element in a vector.
#include <iostream>
#include "common.hpp"
#include "board.hpp"
#include "BoardQueue.hpp"
#include <unordered_map>
#include <string>

using namespace std;

//Change this define to true to do BFS, otherwise will do recursively
#define DOING_BFS false
#define STABLE "s"
#define SEMI_STABLE "t"
#define UNSTABLE "u"
#define WHITE_COL "w"
#define BLACK_COL "b"
#define BLANK "-"
#define MAX_DEPTH 6
#define EXTEND (15 - MAX_DEPTH) //EXTRA DEPTH FOR ENDGAME, VERY GOOD FOR SOLVING THESE ENDGAMES

class Player {

public:
    Player(Side side);
    ~Player();
    Board board;
    Side player_side;
    Side opponent_side;
    time_t t;

    int move_num = 1;
    //Alex's global variables.
    int to_move_x;
    int to_move_y;
    int alpha; //For alpha-beta pruning.
    int beta;
    //int prunes = 0;
    Move *doMove(Move *opponentsMove, int msLeft);
    int edge_stability(Board * b, Side s);
    int in_stability(Board * b, Side side);
    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;

    unordered_map<string, string> edge_combos;

  private:
    int getBoardScore(Board * b, Side side);
    int * BFS(double limit, BoardQueue * q);
    void enqueue_boardState(BoardState * bs, BoardQueue * q);
    string get_stability(string key);
    string getString(int i, int j, int k, int l, int m, int n, int o, int p);
    //Alex's
    int recursiveMoveFind(Board * b, int depth, Side curr);
    int board_weights[8][8] =
    {{120, -20, 20, 5, 5, 20, -20, 120},
    {-20, -40, -5, -5, -5, -5, -40, -20},
    { 20,  -5, 15, 3, 3, 15,  -5,  20},
    { 5,  -5, 3, 0, 0, 3,  -5,  5},
    { 5,  -5, 3, 0, 0, 3,  -5,  5},
    { 20,  -5, 15, 3, 3, 15,  -5,  20},
    {-20, -40, -5, -5, -5, -5, -40, -20},
    {120, -20, 20, 5, 5, 20, -20, 120},};

};


#endif
