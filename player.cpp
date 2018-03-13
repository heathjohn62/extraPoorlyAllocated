#include "player.hpp"
#include <time.h>
#include "BoardQueue.hpp"
#include <math.h>
#include <climits>
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
  if (opponentsMove != NULL) {
    cerr << "Opponent: " << opponentsMove->x << ", " << opponentsMove->y << endl;
  }

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
    alpha = -INT_MAX;
    beta = INT_MAX;
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
      cerr << "Moving at " << to_move_x << ", " << to_move_y << endl;
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



  int current_mobility(int oppMoves, int usMoves) {
    return (int) (1000 * ((double)(usMoves - oppMoves))/((double)(oppMoves + usMoves + 2))) ;
  }
  int Player::in_stability(Board * b, Side side) {
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



  int potential_mobility(Board * b, Side side) {
    Side opposite = (side == WHITE) ? BLACK : WHITE;
    int m1Us = 0;
    int m1Opp = 0;
    int m2Us = 0;
    int m2Opp = 0;
    int m3Us = 0;
    int m3Opp = 0;
    vector<int> m2markedU;
    vector<int> m2markedO;
    vector<int> m1marked;

    for(int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (b->get(side, i, j)) {//Metric 2 + 3 for opp
          for (int di = -1; di <= 1; di ++) {
            for (int dj = -1; dj <= 1; dj++) {
              if (b->onBoard(i + di, j + dj) && !(b->occupied(i + di, j + dj))) {
                m3Us++;
                int ind = 8 * i + j;
                if(std::find(m2markedU.begin(), m2markedU.end(), ind) != m2markedU.end()) {

                } else {
                  m2Us++;
                  m2markedU.push_back(ind);
                }
              }
            }
          }
        } else if (b->get(opposite, i, j)) { //Metric 2 + 3 for opp
          for (int di = -1; di <= 1; di ++) {
            for (int dj = -1; dj <= 1; dj++) {
              if (b->onBoard(i + di, j + dj) && !(b->occupied(i + di, j + dj))) {
                m3Opp++;
                int ind = 8 * i + j;
                if(std::find(m2markedO.begin(), m2markedO.end(), ind) != m2markedO.end()) {

                } else {
                  m2Opp++;
                  m2markedO.push_back(ind);
                }
              }
            }
          }
        } else { //Metric 1
          for (int di = -1; di <= 1; di ++) {
            for (int dj = -1; dj <= 1; dj++) {
              if (b->onBoard(i + di, j + dj) && (b->occupied(i + di, j + dj))) {
                int ind = 8 * i + j;
                if(std::find(m2markedO.begin(), m2markedO.end(), ind) != m2markedO.end()) {

                } else {
                  if (b->get(side, i + di, j + dj)) {
                    m1Us++;
                  } else {
                    m1Opp++;
                  }
                  m1marked.push_back(ind);
                }
              }
            }
          }
        }
      }
    }


    return current_mobility(m1Opp, m1Us) + current_mobility(m2Opp, m2Us) + current_mobility(m3Opp, m3Us);
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
         int oppMoves = b->int_hasMoves(opposite);
         int usMoves = b->int_hasMoves(side);

         //  if (player_side == WHITE) {
         // //    int eval = esac(b->countTotal()) * edge_stability(b, side) +
         // //            cmac(b->countTotal()) * current_mobility(oppMoves, usMoves) +
         // //            36 * in_stability(b, side) +
         // //            99 * potential_mobility(b, side);
         // //    return eval;
         // return  100 * in_stability(b, side)+
         //
         //         cmac(b->countTotal()) * current_mobility(oppMoves, usMoves)
         //         + 99 * potential_mobility(b, side);
         //  } else {



         //double move_dif = ((double)(usMoves - oppMoves)) / ((double)(usMoves + oppMoves + 2));
         //cerr << (int)(move_dif * 16) << endl; ((int)(16 * move_dif) +
         return  110 * in_stability(b, side)+
                (esac(b->countTotal())/3) * edge_stability(b, side) +
                 cmac(b->countTotal()) * current_mobility(oppMoves, usMoves)
                 + 99 * potential_mobility(b, side);
        //}

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
        return getBoardScore(b, curr);

      }

      int best_score = -INT_MAX;
      for(int i = 0; i < 8; i++) {
        bool we_should_break = false;
        for (int j = 0; j < 8; j++) {
          Move potential(i, j);
          if (b->checkMove(&potential, curr)) {
            Board * copy = b->copy();
            copy->doMove(&potential, curr);
            Side opposite = (curr == WHITE) ? BLACK : WHITE;
            //Simple Negamax (from rec slides)
            int score = -recursiveMoveFind(copy, depth + 1, opposite);

            if (score > best_score || best_score == -INT_MAX) {
              best_score = score;
              if (depth == 0) {
                to_move_x = potential.x;
                to_move_y = potential.y;
              }
            }
            alpha = (alpha > score) ? alpha : score;
            if (alpha >= beta) {
              we_should_break = true;
              break;
            }



            delete(copy);
          }
        }
        if (we_should_break) {
          break;
        }
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


    int Player::edge_stability(Board * b, Side s) {
      Side opp = (s == WHITE) ? BLACK : WHITE;
      int stability = 0;
      unordered_map<string,string>::const_iterator got;
      //Edge 1
      string key = "";
      for (int i = 0; i < 8; i++) {
        if (b->get(WHITE, i, 0)) {
          key.append(WHITE_COL);
        } else if (b->get(BLACK, i, 0)) {
          key.append(BLACK_COL);
        } else {
          key.append(BLANK);
        }
      }
      got = edge_combos.find(key);
      string val = got->second;
      for (int i = 0; i < 8; i++) {
        int to_add = -1;
        string temp = val.substr(i, 1);
        if (temp.compare(STABLE)) {
            if (i == 0 || i == 7) {
              to_add = 700;
            } else if (i == 1 || i == 6) {
              to_add = 1200;
            } else {
              to_add = 1000;
            }
        } else if (temp.compare(SEMI_STABLE)) {

            to_add = 200;
        } else {
            if (i == 1 || i == 6) {
              to_add = -25;
            } else {
              to_add = 50;
            }
        }
        if (b->get(s, i, 0)) {
          stability += to_add;
        } else if (b->get(opp, i, 0)) {
          stability -= to_add;
        }

      }
      //Edge 2
      key = "";
      for (int i = 0; i < 8; i++) {
        if (b->get(WHITE, i, 7)) {
          key.append(WHITE_COL);
        } else if (b->get(BLACK, i, 7)) {
          key.append(BLACK_COL);
        } else {
          key.append(BLANK);
        }
      }
      got = edge_combos.find(key);
      val = got->second;
      for (int i = 0; i < 8; i++) {
        int to_add = -1;
        string temp = val.substr(i, 1);
        if (temp.compare(STABLE)) {
            if (i == 0 || i == 7) {
              to_add = 700;
            } else if (i == 1 || i == 6) {
              to_add = 1200;
            } else {
              to_add = 1000;
            }
        } else if (temp.compare(SEMI_STABLE)) {

            to_add = 200;
        } else {
            if (i == 1 || i == 6) {
              to_add = -25;
            } else {
              to_add = 50;
            }
        }
        if (b->get(s, i, 7)) {
          stability += to_add;
        } else if (b->get(opp, i, 7)) {
          stability -= to_add;
        }

      }
      //Edge 3
      key = "";
      for (int i = 0; i < 8; i++) {
        if (b->get(WHITE, 0, i)) {
          key.append(WHITE_COL);
        } else if (b->get(BLACK, 0, i)) {
          key.append(BLACK_COL);
        } else {
          key.append(BLANK);
        }
      }
      got = edge_combos.find(key);
      val = got->second;
      for (int i = 0; i < 8; i++) {
        int to_add = -1;
        string temp = val.substr(i, 1);
        if (temp.compare(STABLE)) {
            if (i == 0 || i == 7) {
              to_add = 700;
            } else if (i == 1 || i == 6) {
              to_add = 1200;
            } else {
              to_add = 1000;
            }
        } else if (temp.compare(SEMI_STABLE)) {

            to_add = 200;
        } else {
            if (i == 1 || i == 6) {
              to_add = -25;
            } else {
              to_add = 50;
            }
        }
        if (b->get(s, 0, i)) {
          stability += to_add;
        } else if (b->get(opp, 0, i)) {
          stability -= to_add;
        }

      }
      //Edge 4
      key = "";
      for (int i = 0; i < 8; i++) {
        if (b->get(WHITE, 7, i)) {
          key.append(WHITE_COL);
        } else if (b->get(BLACK, 7, i)) {
          key.append(BLACK_COL);
        } else {
          key.append(BLANK);
        }
      }
      got = edge_combos.find(key);
      val = got->second;
      for (int i = 0; i < 8; i++) {
        int to_add = -1;
        string temp = val.substr(i, 1);
        if (temp.compare(STABLE)) {
            if (i == 0 || i == 7) {
              to_add = 700;
            } else if (i == 1 || i == 6) {
              to_add = 1200;
            } else {
              to_add = 1000;
            }
        } else if (temp.compare(SEMI_STABLE)) {

            to_add = 200;
        } else {
            if (i == 1 || i == 6) {
              to_add = -25;
            } else {
              to_add = 50;
            }
        }
        if (b->get(s, 7, i)) {
          stability += to_add;
        } else if (b->get(opp, 7, i)) {
          stability -= to_add;
        }

      }

      //Here we will deal with the X-squares, very important to get right.
      if (!b->occupied(0,0)) { //Corner 1
        Move m(0,0);
        if (b->get(s, 1, 1)) {
          if (b->checkMove(&m, opp)) {
            stability -= 1200;
          } else {
            stability -= 600;
          }
        } else if (b->get(opp, 1, 1)) {
          if (b->checkMove(&m, s)) {
            stability += 1200;
          } else {
            stability += 600;
          }
        }
      }

      if (!b->occupied(0,7)) { //Corner 2
        Move m(0,7);
        if (b->get(s, 1, 6)) {
          if (b->checkMove(&m, s)) {
            stability -= 1200;
          } else {
            stability -= 600;
          }
        } else if (b->get(opp, 1, 6)) {
          if (b->checkMove(&m, s)) {
            stability += 1200;
          } else {
            stability += 600;
          }
        }
      }

      if (!b->occupied(7,0)) { //Corner 3
        Move m(7,0);
        if (b->get(s, 6, 1)) {
          if (b->checkMove(&m, s)) {
            stability -= 1200;
          } else {
            stability -= 600;
          }
        } else if (b->get(opp, 6, 1)) {
          if (b->checkMove(&m, s)) {
            stability += 1200;
          } else {
            stability += 600;
          }
        }
      }

      if (!b->occupied(7,7)) { //Corner 4
        Move m(7,7);
        if (b->get(s, 6, 6)) {
          if (b->checkMove(&m, s)) {
            stability -= 1200;
          } else {
            stability -= 600;
          }
        } else if (b->get(opp, 6, 6)) {
          if (b->checkMove(&m, s)) {
            stability += 1200;
          } else {
            stability += 600;
          }
        }
      }

      return stability;
    }
