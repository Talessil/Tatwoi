#include <cstdlib>
#include "queue.h"

Queue::Queue() {
  first = NULL;
  last = NULL;
  size = 0;
}

Queue::~Queue() {
  QueueNode *aux = first;
  while (aux != last) {
    QueueNode *aux2 = aux->next;
    free(aux);
    aux = aux2;
  }
  free(aux);
}

void Queue::enqueue(void *value) {
  QueueNode *new_node = (QueueNode *) malloc(sizeof(QueueNode));
  new_node->prev = last;
  new_node->value = value;
  new_node->next = NULL;
  if (last != NULL) {
    last->next = new_node;
    last = new_node;
  } else {
    last = new_node;
    first = new_node;
  }
  size++;
}

void *Queue::dequeue() {
  if (size == 0)
    return NULL;
  else {
    void *new_value = first->value;
    QueueNode *node = first;
    first = node->next;
    size--;
    if (size == 0)
      last = NULL;
    free(node);
    return new_value;
  }
}

int Queue::get_size() {
  return size;
}
