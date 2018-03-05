#include "player.hpp"
#include <time.h>
#include "BoardQueue.hpp"
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

    time(&t);
    //First make the oponents move on our board.
    board.doMove(opponentsMove, opponent_side);
    BoardQueue * q = new BoardQueue();
    
    
    bestX = -1; // Reset our best Move spot variables.
    bestY = -1;


    for (int i = 0; i < 8; i++)
    { //Iterate through board looking for moves.
      for (int j = 0; j < 8; j++)
      {
        Move * m = new Move(i, j);
        if (board.checkMove(m, player_side))
        {
          // Create a copy of the board to avoid messing up the board.
          Board * copy  = board.copy();
          copy->doMove(m, player_side); //Make the move on the copy and then evaluate it.
          q->enqueue(new BoardState(copy, 1, m));

          }
        }
      }
    
    BFS(31.0, q);
    std::cerr << "I get here without fault" << std::endl;
    
    if (bestX == -1 && bestY == -1) { //Indicates we have no valid moves.

      cerr << "PLAYER PASS" << endl;
      return nullptr;

    } else {

      // This will tell the framework to make the best move.
      Move * toMake = new Move(bestX, bestY);
      board.doMove(toMake, player_side);
      return toMake;

    }
  }
    /**
     * Performs a breadth first search of the board queue, updating the
     * best move as it is found.
     *
     * @param limit A double describing the number of seconds remaining
     * that the program has to calculate a move.
     * @param q A queue of board objects, among other relevant information.
     */
    void Player::BFS(double limit, BoardQueue * q)
    {
        int depth = 1;
        int nextBestScore = -1000;
        int min_score;
        BoardState * min;
        bool initialized = false;
        
        while (true)
        {
            std::cerr << "looping!" << endl;
            BoardState * b = q->dequeue();
            // This deals with enemy moves. The enemy is assumed to make the
            // best immeditate move for itself. The minimum score move from
            // each ancestor is added to the queue.
            if (b->depth % 2 == 0)
            {
                // If there was an ancestor or depth change, enqueue the old min
                // and find another min.
                if ((min->ancestor != b->ancestor && initialized) || (b->depth > depth))
                {
                    if (b->depth > depth)
                    {
                        depth = b->depth;
                        min = b;
                    }
                    else
                    {
                        // I want to enqueue all possible moves that start
                        // from this board state!
                        enqueue_boardState(min, q);
                        // In this situtation, the player is making these moves
                        delete min;

                        min_score = 1000;
                        min = b;
                    }
                }
                int tempScore = getBoardScore((b->board), player_side);
                if (tempScore < min_score)
                {
                    min = b;
                    min_score = tempScore;
                    initialized = true;
                }
            }
            // Now to deal with friendly, odd-depth moves!
            else
            {
                if (b->depth > depth)
                {
                    //If we get to a new depth, update new bests!
                    bestX = nextBestX;
                    bestY = nextBestY;
                    nextBestX = -1;
                    nextBestY = -1;
                    nextBestScore = -1000;
                    enqueue_boardState(min, q);
                    min_score = 1000;
                }
                if (difftime(time(NULL), t) > limit + 1)
                {
                    break; // Program runs out of time
                }
                // Get Scores, and update if the best potential move is reached.
                int tempScore = getBoardScore((b->board), player_side);
                std::cerr<<"Front before resetting scores: " << q->peek() << endl;
                if (tempScore > nextBestScore)
                {
                    nextBestScore = tempScore;
                    nextBestY = (b->ancestor)->getY();
                    std::cerr<<"nextBestY: " << nextBestY << std::endl;
                    std::cerr<<"Front: " << q->peek() << std::endl;
                    nextBestX = (b->ancestor)->getX();
                    std::cerr<<"nextBestX: " << nextBestX << std::endl;
                    std::cerr<<"Front: " << q->peek() << std::endl;
                    
                }
                std::cerr<<"Front after resetting scores: " << q->peek() << endl;
                // Iterate through board looking for all possible enemy moves.
                // Queue all enemy moves.
                enqueue_boardState(b, q);
                if (min != b)
                {
                    delete b;
                }
            }
        }
    }




    /**
     * Returns a metric of the state of the board, where higher numbers are
     * better with respect to the side given as a parameter.
     *
     * @param board A board pointer which references the board to be evaluated
     * @param side The side with which the board is evaluated with respect to.
     */
    int Player::getBoardScore(Board * b, Side side)
    {
      int board_weights[8][8]=
          {{20, -3, 4, 4, 4, 4, -3, 20},
           {-3, -5, 1, 1, 1, 1, -5, -3},
           { 4,  1, 1, 1, 1, 1,  1,  4},
           { 4,  1, 1, 1, 1, 1,  1,  4},
           { 4,  1, 1, 1, 1, 1,  1,  4},
           { 4,  1, 1, 1, 1, 1,  1,  4},
           {-3, -5, 1, 1, 1, 1, -5, -3},
           {20, -3, 4, 4, 4, 4, -3, 20}};
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
                if (b->get(side, i, j))
                {
                    boardScore += board_weights[i][j];
                }
                else if (b->get(opposite, i, j))
                {
                    boardScore -= board_weights[i][j];
                }
            }
        }
        return boardScore;
    }

    void Player::enqueue_boardState(BoardState * bs, BoardQueue * q) 
    {
      int n_depth = (bs->depth) + 1;
      Side n_side = (n_depth % 2 == 0) ? BLACK : WHITE;
      Move * anc = bs->ancestor;
      Board * old = bs->board;

      for (int i = 0; i < 8; i++) 
      {
        for (int j = 0; j < 8; j++) 
        {
          Move * m = new Move(i, j);
          if (old->checkMove(m, n_side)) 
          {
            Board * copy  = old->copy();
            copy->doMove(m, n_side); //Make the move on the copy and then evaluate it.
            BoardState * to_queue = new BoardState(copy, n_depth, anc);
            q->enqueue(to_queue);
          }
        }
      }
    }
