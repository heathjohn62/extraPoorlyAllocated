#ifndef Board_Queue_H
#define Board_Queue_H

#include "common.hpp"
#include "board.hpp"



struct BoardState {

  Board * board;
  int depth;
  Move * ancestor;

  BoardState(Board * bo, int de, Move * anc) {
    board = bo;
    depth = de;
    ancestor = anc;
  }

  ~BoardState() {
    delete(board);
    delete(ancestor);
  }
};
/**
 * @brief Simple queue item for Board objects.
 */
struct queueitem {
    BoardState *bs;
    queueitem *next;
};

/**
 * @brief Encapsulates a queue for Board objects.
 */
class BoardQueue
{
public:

    BoardQueue();

    ~BoardQueue();

    void enqueue(BoardState * bs);
    BoardState * dequeue();
    queueitem * peek();
    bool is_empty();

private:
    queueitem *front, *rear;
    void init();
    void deinit();
};
#endif
