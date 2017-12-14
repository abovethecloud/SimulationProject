#ifndef LIST_MANAGER_H
#define LIST_MANAGER_H

#include "globals.h"
#include "utils.h"

Node* get_new_node(Node *available);
Node* event_pop(Tree *pointer_to_fel);
void schedule(Node* node_event, Tree *pointer_to_fel);
void scheduleA(Node* node_event, Tree *pointer_to_fel);
void recursive_insertion(Tree *t3, Node *node_event);
void enqueue(Node* new_inline, Station *station);
Node* dequeue(Station *station);
void  return_node(Node* item);

#endif
