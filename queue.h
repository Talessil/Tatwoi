#ifndef COMPILADOR_CPM_QUEUE_H
#define COMPILADOR_CPM_QUEUE_H

typedef struct QueueNode {
  struct QueueNode *prev;
  void *value;
  struct QueueNode *next;
} queue_node;

class Queue {
  private:
    queue_node *first;
    queue_node *last;
    int size;
  public:
    Queue();
    ~Queue();
    void enqueue(void *value);
    void *dequeue();
    int get_size();
};

#endif //COMPILADOR_CPM_QUEUE_H
