#include "BoardQueue.hpp"
#include <iostream>

// Constructor
BoardQueue::BoardQueue()
{
    init();
}


/**
 * @brief Initializes the queue.
 */
void BoardQueue::init()
{
  front = nullptr;
  rear = nullptr;
}

/**
 * @brief Deinitializes the queue.
 */
BoardQueue::~BoardQueue()
{
    deinit();
}

/**
 * @brief Deinitializes the queue (student-implemented).
 */
void BoardQueue::deinit()
{
    if (!this->is_empty()) {
      if (rear == nullptr) {
        delete(front);
      } else {

        while (front->next != rear) {
          queueitem * temp = front;
          front = front->next;
          delete(temp);
        }
        delete(front);
        delete(rear);
      }
    }
}

/**
 * @brief Enqueues an item by adding it to the bottom of the
 * queue.
 *
 * @param[in] b Board to enqueue onto the queue.
 */
void BoardQueue::enqueue(BoardState * bs)
{
  queueitem * temp = new queueitem();
  temp->bs = bs;
  if (front == nullptr) {
    temp->next = rear;
    front = temp;
  } else if (rear == nullptr) {
    temp->next = nullptr;
    rear = temp;
    front->next = rear;
  } else {
    temp->next = nullptr;
    rear->next = temp;
    rear = temp;
  }
}

/**
 * @brief Dequeues an item by removing it from the top.
 *
 * @return The dequeued Board.
 */
BoardState * BoardQueue::dequeue()
{
    BoardState * bs = front->bs; // Will seg-fault if empty
    front = front->next;
    return bs;
}

/**
 * @brief Returns the item at the front of the queue without
 * removing it.
 *
 * @return The first queueitem in the queue.
 */
queueitem * BoardQueue::peek()
{
    return front;
}

/**
 * @brief Returns true is the queue is empty, false otherwise.
 *
 * @return Boolean indicating whether the queue is empty.
 */
bool BoardQueue::is_empty()
{
    bool b = (front == nullptr && rear == nullptr);
    return b;
}
