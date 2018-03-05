#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    player_side = side;
    if (side == WHITE)
    {
        opponent_side = BLACK;
    }
    else
    {
        opponent_side = WHITE;
    }
    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}



/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    //First make the oponents move on our board.
    board.doMove(opponentsMove, opponent_side);


    bestX = -1; // Reset our best Move spot variables.
    bestY = -1;

    // Create a copy of the board to avoid messing up the board.
    Board * copy  = board.copy();

    int bestScore = 0;
    for (int i = 0; i < 8; i++) { //Iterate through board looking for moves.
      for (int j = 0; j < 8; j++) {
        Move m(i, j);
        if (checkMove(&m, player_side)) {

          copy->doMove(&m); //Make the move on the copy and then evaluate it.
          int score = getBoardScore(copy);

          if (score > bestScore) { //If the score is good, save the move.
            bestScore = score;
            bestX = i;
            bestY = j;
          }
        }
      }
    }



    if (bestX == -1 && bestY == -1) { //Indicates we have no valid moves.

      cerr << "PLAYER PASS" << endl;
      return nullptr;

    } else {
      // This will tell the framework to make the best move.
      return new Move(bestX, bestY);

    }
}
