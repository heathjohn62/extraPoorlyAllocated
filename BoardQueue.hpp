#include "common.hpp"
#include "board.hpp"



struct BoardState {

  Board * b;
  int depth;
  Move * ancestor;

  BoardState(Board * bo, int de, Move * anc) {
    b = bo;
    depth = de;
    ancestor = anc;
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
    BoardState * peek();
    bool is_empty();

private:
    queueitem *front, *rear;

    void init();
    void deinit();
    void do_enqueue(BoardState bs);
    BoardState do_dequeue();
};