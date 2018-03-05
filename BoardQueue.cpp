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
    /* Do the enqueue operation. */
    std::cerr << "Enqueueing" << std::endl;
  queueitem * temp = new queueitem();
  std::cerr << "Pointer to enqueue: " << temp << std::endl;
  temp->bs = bs;
  if (front == nullptr) {
    std::cerr<<"Front was null" << std::endl;
    temp->next = rear;
    front = temp;
  } else if (rear == nullptr) {
    std::cerr<<"Rear was null" << std::endl;
    temp->next = nullptr;
    rear = temp;
    front->next = rear;
  } else {
    std::cerr<<"Appended normally" << std::endl;
    temp->next = nullptr;
    rear->next = temp;
    rear = temp;
  }
  std::cerr << "Front after enqueue: " << front << std::endl;
}

/**
 * @brief Dequeues an item by removing it from the top.
 *
 * @return The dequeued Board.
 */
BoardState * BoardQueue::dequeue()
{
    std::cerr << "Dequeueing" << std::endl;
    BoardState * bs = front->bs; // Will seg-fault if empty
    
    front = front->next;
    std::cerr <<  "Front after dequeue: " << front << std::endl;
    
    
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
    /* TODO: Write your peek code here! */
    return front;
}

/**
 * @brief Returns true is the queue is empty, false otherwise.
 *
 * @return Boolean indicating whether the queue is empty.
 */
bool BoardQueue::is_empty()
{
    /* TODO: Is the queue empty??? */

    bool b = (front == nullptr && rear == nullptr);
    return b;
}
