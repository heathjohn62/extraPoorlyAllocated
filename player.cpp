#include "player.hpp"
#include <time.h>
#include "BoardQueue.hpp"
#include <math.h>

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    player_side = side;
    srand (time(NULL));
    if (side == WHITE)
    {
        opponent_side = BLACK;
    }
    else
    {
        opponent_side = WHITE;
    }
     cerr << "GOING IN" << endl;
    for(int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        for (int k = 0; k < 3; k++) {
          for (int l = 0; l < 3; l++) {
            for (int m = 0; m < 3; m++) {
              for (int n = 0; n < 3; n++) {
                for (int o = 0; o < 3; o++) {
                  for (int p = 0; p < 3; p++) {
                    //cerr << "NESTED FOR" << endl;
                    string key = getString(i,j,k,l,m,n,o,p);
                    string val = get_stability(key);
                    //cerr << key << " " << endl;
                    edge_combos[key] = val;
                  }
                }
              }
            }
          }
        }
      }
    }
    unordered_map<string,string>::const_iterator got = edge_combos.find("wwb-b-w-");
    cerr << got->first << " is " << got->second << endl;
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
  // if (DOING_BFS) {
  //
  //   BoardQueue * q = new BoardQueue();
  //
  //   int valid_moves = 0;
  //   for (int i = 0; i < 8; i++)
  //   { //Iterate through board looking for moves.
  //     for (int j = 0; j < 8; j++)
  //     {
  //       Move * m = new Move(i, j);
  //       if (board.checkMove(m, player_side))
  //       {
  //         valid_moves++;
  //         // Create a copy of the board to avoid messing up the board.
  //         Board * copy  = board.copy();
  //         copy->doMove(m, player_side); //Make the move on the copy and then evaluate it.
  //         q->enqueue(new BoardState(copy, 1, m));
  //
  //         }
  //       }
  //     }
  //   int bestX = -1;
  //   int bestY = -1;
  //   if (valid_moves == 1)
  //   {
  //       BoardState * bs = q->dequeue();
  //       bestX = bs->ancestor->getX();
  //       bestY = bs->ancestor->getY();
  //       delete bs;
  //   }
  //   else if (valid_moves > 1)
  //   {
  //       int * best = BFS(31.0, q);
  //       bestX = best[0];
  //       bestY = best[1];
  //       delete[] best;
  //   }
  //
  //   if (bestX == -1 && bestY == -1)
  //   {
  //     //Indicates we have no valid moves.
  //     return nullptr;
  //   }
  //   else
  //   {
  //
  //     // This will tell the framework to make the best move.
  //     Move * toMake = new Move(bestX, bestY);
  //     board.doMove(toMake, player_side);
  //     return toMake;
  //
  //   }
  // } else { //If not doing BFS
    if (move_num == 1) {
      time(&t);
    }
    to_move_x = -1;
    to_move_y = -1;
    alpha = -100000;
    beta = 100000;
    if (board.countTotal() == 49 || board.countTotal() == 48) {
      cerr << "White Pieces going into endgame: " << board.countWhite() << endl;
    }
    recursiveMoveFind(board.copy(), 0, player_side);
    //cerr << "Current Move: " << move_num << endl;
    move_num++;
    if (move_num > 30) {
      cerr << "Time Taken: " << (difftime(time(NULL), t)) << " sec"<< endl;
      //cerr << "Prunes: " << prunes << endl;
    }
    //cerr << "MS LEFT: " << msLeft << " ms" << endl;
    if (to_move_x == -1 && to_move_y == -1) {
      cerr << "PLayer Pass" << endl;
      return nullptr;
    } else {
      Move * to_move = new Move(to_move_x, to_move_y);
      board.doMove(to_move, player_side);
      return to_move;
    }

  //}
}
/**
 * Performs a breadth first search of the board queue, updating the
 * best move as it is found.
 *
 * @param limit A double describing the number of seconds remaining
 * that the program has to calculate a move.
 * @param q A queue of board objects, among other relevant information.
 */
// int * Player::BFS(double limit, BoardQueue * q)
// {
//     time(&t);
//     int depth = 1;
//     int nextBestScore = -1000;
//     int min_score = 1000;
//     BoardState * min;
//     bool initialized = false;
//     int bestX = -1;
//     int bestY = -1;
//     int nextBestY = -1;
//     int nextBestX = -1;
//     while (q->peek())
//     {
//         BoardState * b = q->dequeue();
//         if (b->depth == 0)
//         {
//             // A little unsure why this works, or why this might happen
//             break;
//         }
//         if (b->depth > depth)
//         {
//             depth = b->depth;
//             if (depth % 2 == 0)
//             {
//                 //If we get to a new depth, update new bests!
//                 bestX = nextBestX;
//                 bestY = nextBestY;
//                 nextBestX = -1;
//                 nextBestY = -1;
//                 nextBestScore = -1000;
//                 min_score = 1000;
//             }
//             else
//             {
//                 enqueue_boardState(min, q);
//                 delete min;
//                 initialized = false;
//             }
//         }
//         // This deals with enemy moves. The enemy is assumed to make the
//         // best immeditate move for itself. The minimum score move from
//         // each ancestor is added to the queue.
//         if (depth % 2 == 0)
//         {
//             // If there was an ancestor or depth change, enqueue the old min
//             // and find another min.
//             if (initialized && min->ancestor != b->ancestor)
//             {
//                 // I want to enqueue all possible moves that start
//                 // from this board state!
//                 enqueue_boardState(min, q);
//                 // In this situtation, the player is making these moves
//                 delete min;
//                 min_score = 1000;
//                 min = b;
//             }
//             int tempScore = getBoardScore((b->board), player_side);
//             if (tempScore < min_score)
//             {
//                 min = b;
//                 min_score = tempScore;
//                 initialized = true;
//             }
//         }
//         // Now to deal with friendly, odd-depth moves!
//         else
//         {
//             if (difftime(time(NULL), t) > limit + 1)
//             {
//                 break; // Program runs out of time
//             }
//             if (q->peek() == 0)
//             {
//                 break;
//             }
//             // Get Scores, and update if the best potential move is reached.
//             int tempScore = getBoardScore((b->board), player_side);
//             if (tempScore > nextBestScore)
//             {
//                 nextBestScore = tempScore;
//                 nextBestX = b->ancestor->getX();
//                 nextBestY = b->ancestor->getY();
//             }
//             // Iterate through board looking for all possible enemy moves.
//             // Queue all enemy moves.
//             enqueue_boardState(b, q);
//             if (min != b)
//             {
//                 delete b;
//             }
//         }
//     }
//     if (depth > 1 || depth == 0)
//     {
//         int * a = new int[2];
//         a[0] = bestX;
//         a[1] = bestY;
//         return a;
//     }
//     else
//     {
//         int * a = new int[2];
//         a[0] = nextBestX;
//         a[1] = nextBestY;
//         return a;
//     }
// }

  string indivStability(int i, string key) {
    string col = key.substr(i, 1);
    if (col.compare(BLANK) != 0) {
      string opp = (col.compare(BLACK_COL) == 0) ? WHITE_COL : BLACK_COL;
      bool isBlankRight = false;
      bool isBlankLeft = false;
      bool isOppRight = false;
      bool isOppLeft = false;
      int closestBlankLeft = -1;
      int closestBlankRight = 9;
      int closestOppLeft = -1;
      int closestOppRight = 9;
      for (int j = 0; j < 8 ; j++) {
        if (j== i) continue;
        string temp = key.substr(j, 1);
        if (j < i && temp.compare(BLANK) == 0) {
          isBlankLeft = true;
          closestBlankLeft = j;
        } else if (j > i && temp.compare(BLANK) == 0) {
          isBlankRight = true;
          if (closestBlankRight == 9) {
            closestBlankRight = j;
          }
        } else if (j < i && opp.compare(temp) == 0) {
          isOppLeft = true;
          closestOppLeft = j;
        } else if (j > i && opp.compare(temp) == 0) {
          isOppRight = true;
          if (closestOppRight == 9) {
            closestOppRight = j;
          }
        }
      }
      if (!((isBlankRight && isOppLeft) || (isBlankLeft && isOppRight))
        || (!isBlankLeft && !isBlankRight)) {
        return STABLE;
      } else {
        if ((closestBlankLeft > closestOppLeft && closestBlankRight < closestOppRight)
              || (closestOppLeft > closestBlankLeft && closestOppRight < closestOppLeft)) {
              return SEMI_STABLE; //Not ideal, but not bad either
        }
        return UNSTABLE;
      }
    } else { //Find stability for blank squares (may just return SEMI_STABLE)
       return SEMI_STABLE;
    }
  }

  string Player::get_stability(string key) {
    string ret = STABLE; //corner will be stable.
    for (int i = 1; i < 7; i++) {
      ret.append(indivStability(i, key));
    }

    ret.append(STABLE); //Other corner will also be stable
    return ret;
  }


  int esac(int move_num) {
    return 300 + 6 * move_num;
  }
  int cmac(int move_num) {
    if (move_num <= 25) {
      return 50 + (2 * move_num);
    } else {
      return 75 + move_num;
    }
  }

    /**
     * Returns a metric of the state of the board, where higher numbers are
     * better with respect to the side given as a parameter.
     *
     * @param board A board pointer which references the board to be evaluated
     * @param side The side with which the board is evaluated with respect to.
     */
     int Player::getBoardScore(Board * b, Side side, int oppMoves)
     {
       Side opposite = (side == WHITE) ? BLACK : WHITE;
       if (b->countTotal() == 64) {
         return b->count(side) - b->count(opposite);
         /*
         This solves the board, but will only do it at a depth of MAX_DEPTH +
         EXTEND, which is cool bc this is a quick calculation so we can go deeper
         faster, and this will guarantee we WIN if it is forcibly possible at a
         depth of MAX_DEPTH + EXTEND.

         */
       } else {

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
         int num = b->int_hasMoves(side);
         return boardScore + (side == player_side ? num : -num);

        }

     }

    // void Player::enqueue_boardState(BoardState * bs, BoardQueue * q)
    // {
    //   int n_depth = (bs->depth) + 1;
    //   Side n_side = (n_depth % 2 == 0) ? opponent_side : player_side;
    //   Move * anc = bs->ancestor;
    //   Board * old = bs->board;
    //
    //   for (int i = 0; i < 8; i++)
    //   {
    //     for (int j = 0; j < 8; j++)
    //     {
    //       Move * m = new Move(i, j);
    //       if (old->checkMove(m, n_side))
    //       {
    //         Board * copy  = old->copy();
    //         copy->doMove(m, n_side); //Make the move on the copy and then evaluate it.
    //         BoardState * to_queue = new BoardState(copy, n_depth, anc);
    //         q->enqueue(to_queue);
    //       }
    //     }
    //   }
    // }



    int Player::recursiveMoveFind(Board *b, int depth, Side curr) {

      if ((depth == MAX_DEPTH && b->countTotal() < 64 - EXTEND) || (b->countTotal() == 64)) {
        //if (depth > MAX_DEPTH) cerr << "!!!" << depth << endl;
        int oppMoves = 0;//b->int_hasMoves();
        if (depth % 2 == 0) {
          return -1 * getBoardScore(b, curr, oppMoves);
        } else {
          return getBoardScore(b, curr, oppMoves);
        }
      }
      bool no_scores = true;
      int best_score = -100000;
      for(int i = 0; i < 8; i++) {
        bool we_should_break = false;
        for (int j = 0; j < 8; j++) {
          Move potential(i, j);
          if (b->checkMove(&potential, curr)) {
            Board * copy = b->copy();
            copy->doMove(&potential, curr);
            Side opposite = (curr == WHITE) ? BLACK : WHITE;

            int score = recursiveMoveFind(copy, depth + 1, opposite);

            if (curr == player_side) {
              alpha = alpha > score ? alpha : score; //MAXIMIZE ALPHA

              score *= -1; //For other stuff
            } else {
              alpha = alpha < score ? alpha : score; //MINIMIZE BETA
            }
            if (beta <= alpha && depth != 0) {
              //prunes += pow(7, (MAX_DEPTH - depth));
              we_should_break = true;
              break; //Don't search this move, prune the tree.
            }

            //score *= (curr == player_side) ? -1 : 1;

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
        if (we_should_break) {
          break;
        }
      }
      if (curr == player_side) {
        best_score *= -1;
      }
      return best_score;
    }


    string conv(int i) {
      switch (i) {
        case 0:
          return WHITE_COL;
          break;
        case 1:
          return BLACK_COL;
          break;
        case 2:
          return BLANK;
          break;
      }
      return nullptr; //Shouldnt get here
    }

    string Player::getString(int i, int j, int k, int l, int m, int n, int o, int p) {
      string to_ret = "";
      to_ret.append(conv(i));
      to_ret.append(conv(j));
      to_ret.append(conv(k));
      to_ret.append(conv(l));
      to_ret.append(conv(m));
      to_ret.append(conv(n));
      to_ret.append(conv(o));
      to_ret.append(conv(p));
      //cerr << "DOING SOMETHING" << endl;
      return to_ret;
    }
