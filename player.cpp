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
Move *Player::doMove(Move *opponentsMove, int msLeft)
{
  //First make the oponents move on our board.
  board.doMove(opponentsMove, opponent_side);

  if (DOING_BFS) {

    BoardQueue * q = new BoardQueue();

    int valid_moves = 0;
    for (int i = 0; i < 8; i++)
    { //Iterate through board looking for moves.
      for (int j = 0; j < 8; j++)
      {
        Move * m = new Move(i, j);
        if (board.checkMove(m, player_side))
        {
          valid_moves++;
          // Create a copy of the board to avoid messing up the board.
          Board * copy  = board.copy();
          copy->doMove(m, player_side); //Make the move on the copy and then evaluate it.
          q->enqueue(new BoardState(copy, 1, m));

          }
        }
      }
    int bestX = -1;
    int bestY = -1;
    if (valid_moves == 1)
    {
        BoardState * bs = q->dequeue();
        bestX = bs->ancestor->getX();
        bestY = bs->ancestor->getY();
        delete bs;
    }
    else if (valid_moves > 1)
    {
        int * best = BFS(31.0, q);
        bestX = best[0];
        bestY = best[1];
        delete[] best;
    }

    if (bestX == -1 && bestY == -1)
    {
      //Indicates we have no valid moves.
      return nullptr;
    }
    else
    {

      // This will tell the framework to make the best move.
      Move * toMake = new Move(bestX, bestY);
      board.doMove(toMake, player_side);
      return toMake;

    }
  } else { //If not doing BFS
    to_move_x = -1;
    to_move_y = -1;

    recursiveMoveFind(board.copy(), 0);

    if (to_move_x == -1 && to_move_y == -1) {
      return nullptr;
    } else {
      Move * to_move = new Move(to_move_x, to_move_y);
      board.doMove(to_move, player_side);
      return to_move;
    }
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
int * Player::BFS(double limit, BoardQueue * q)
{
    time(&t);
    int depth = 1;
    int nextBestScore = -1000;
    int min_score = 1000;
    BoardState * min;
    bool initialized = false;
    int bestX = -1;
    int bestY = -1;
    int nextBestY = -1;
    int nextBestX = -1;
    while (q->peek())
    {
        BoardState * b = q->dequeue();
        if (b->depth == 0)
        {
            // A little unsure why this works, or why this might happen
            break;
        }
        if (b->depth > depth)
        {
            depth = b->depth;
            if (depth % 2 == 0)
            {
                //If we get to a new depth, update new bests!
                bestX = nextBestX;
                bestY = nextBestY;
                nextBestX = -1;
                nextBestY = -1;
                nextBestScore = -1000;
                min_score = 1000;
            }
            else
            {
                enqueue_boardState(min, q);
                delete min;
                initialized = false;
            }
        }
        // This deals with enemy moves. The enemy is assumed to make the
        // best immeditate move for itself. The minimum score move from
        // each ancestor is added to the queue.
        if (depth % 2 == 0)
        {
            // If there was an ancestor or depth change, enqueue the old min
            // and find another min.
            if (initialized && min->ancestor != b->ancestor)
            {
                // I want to enqueue all possible moves that start
                // from this board state!
                enqueue_boardState(min, q);
                // In this situtation, the player is making these moves
                delete min;
                min_score = 1000;
                min = b;
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
            if (difftime(time(NULL), t) > limit + 1)
            {
                break; // Program runs out of time
            }
            if (q->peek() == 0)
            {
                break;
            }
            // Get Scores, and update if the best potential move is reached.
            int tempScore = getBoardScore((b->board), player_side);
            if (tempScore > nextBestScore)
            {
                nextBestScore = tempScore;
                nextBestX = b->ancestor->getX();
                nextBestY = b->ancestor->getY();
            }
            // Iterate through board looking for all possible enemy moves.
            // Queue all enemy moves.
            enqueue_boardState(b, q);
            if (min != b)
            {
                delete b;
            }
        }
    }
    if (depth > 1 || depth == 0)
    {
        int * a = new int[2];
        a[0] = bestX;
        a[1] = bestY;
        return a;
    }
    else
    {
        int * a = new int[2];
        a[0] = nextBestX;
        a[1] = nextBestY;
        return a;
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
          {{200, -50, 10, 5, 5, 10, -50, 200},
           {-50, -50, 2, 2, 2, 2, -50, -50},
           { 10,  2, 2, 2, 2, 2,  2,  10},
           { 5,  2, 2, 2, 2, 2,  2,  5},
           { 5,  2, 2, 2, 2, 2,  2,  5},
           { 10,  2, 2, 2, 2, 2,  2,  10},
           {-50, -50, 2, 2, 2, 2, -50, -50},
           {200, -50, 10, 5, 5, 10, -50, 200}};
        Side opposite = (side == WHITE) ? BLACK : WHITE;
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
      Side n_side = (n_depth % 2 == 0) ? opponent_side : player_side;
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



    int Player::recursiveMoveFind(Board *b, int depth) {
      Side curr = (depth % 2 == 0) ? player_side : opponent_side;

      if (depth == MAX_DEPTH) {
        if (MAX_DEPTH % 2 == 0) {
          return -1 * getBoardScore(b, curr);
        } else {
          return getBoardScore(b, curr);
        }
      }
      bool no_scores = true;
      int best_score = -10000;
      for(int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
          Move potential(i, j);
          if (b->checkMove(&potential, curr)) {
            Board * copy = b->copy();
            copy->doMove(&potential, curr);

            int score = recursiveMoveFind(copy, depth + 1);
            if (curr == player_side) {
              score *= -1;
            }
            if (no_scores || score > best_score) {
              no_scores = false;
              best_score = score;
              if (depth == 0) {
                to_move_x = potential.x;
                to_move_y = potential.y;
              }
            }
            delete(copy);
          }
        }
      }
      if (curr == player_side) {
        best_score *= -1;
      }
      return best_score;
    }
