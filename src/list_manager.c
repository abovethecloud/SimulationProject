#include "list_manager.h"


Node* get_new_node(Node* available)
{
  Node *item;

  if (!available)
  {
    item = (Node*) calloc(1, sizeof(Node));
    //node_number++;
  }
  else
  {
    item = available;
    if (!available->next)
    {
      available = NULL;
    }
    else
    {
      available = available->next;
      available->previous = NULL;
    }
  }

  item->previous = NULL;
  item->next = NULL;

  item->event.name[0] = '\0';
  item->event.type = 0;
  item->event.station = 0;  /* between 0 and N_STATIONS-1 */
  item->event.create_time = 0;
  item->event.occur_time = 0.0;
  item->event.arrival_time = 0.0;
  item->event.service_time = 0.0;

  return item;
}

Node* event_pop(Tree *pointer_to_fel)
{
  Node *p;
  Tree *t = pointer_to_fel;

  while ((*t)->previous)
  {
    t = &((*t)->previous);
  }
  p = *t;
  *t=(*t)->next;
  p->next = NULL;
  return p;
}

void schedule(Node* node_event, Tree *pointer_to_fel)
{
  recursive_insertion(pointer_to_fel, node_event);
}

void recursive_insertion(Tree *t3, Node *node_event)
{
  /* NULL pointer in the tree */
  if (!*t3) {
    *t3 = node_event;
    return;
  }

  /* Non-empty FEL */
  if ((node_event)->event.occur_time < (*t3)->event.occur_time){
    recursive_insertion(&((*t3)->previous), node_event);
  }
  else if ((node_event)->event.occur_time == (*t3)->event.occur_time)
  {
    if (node_event->event.type == DEPARTURE){
      recursive_insertion(&((*t3)->previous), node_event);
    }
    else{
      recursive_insertion(&((*t3)->next), node_event);
    }
  }
  else {
    recursive_insertion(&((*t3)->next), node_event);
  }
}
/*
void schedule(Node* node_event, Tree *pointer_to_fel)
{
  // If FEL == NULL, FEL points to the node
  if (!*pointer_to_fel) {
    *pointer_to_fel = node_event;
    return;
  }

  // If an earlier event arrives (or a contemporary DEPARTURE), put it as first element of FEL and keep the chain
  if (
        ((node_event)->event.occur_time < (*pointer_to_fel)->event.occur_time) ||
        (((node_event)->event.occur_time == (*pointer_to_fel)->event.occur_time) && ((node_event)->event.type == DEPARTURE))
     )
  {
    (*pointer_to_fel)->previous = node_event;
    node_event->next = (*pointer_to_fel);
    (*pointer_to_fel) = node_event;
    return;
  }

  // If equal arrival or later event, put it later in the chain, do not change FEL pointer
  Node *pointer_to_node = (*pointer_to_fel);
  while (((node_event)->event.occur_time >= (pointer_to_node)->event.occur_time) && ())
  {
    if ()
    pointer_to_node = (pointer_to_node)->next;
  }

}
*/

void enqueue(Node* new_inline, Station *station){
  if (station->queue.head)
    station->queue.head->previous = new_inline;
  new_inline->next = station->queue.head;
  new_inline->previous = NULL;
  if (!station->queue.head)
    station->queue.tail = new_inline;
  station->queue.head = new_inline;
}

Node* dequeue(Station *station){
  Node *item;
  if (!station->queue.tail)
    return NULL;
  item = station->queue.tail;
  if (!station->queue.tail->previous){
    station->queue.tail = NULL;
    station->queue.head = NULL;
  }
  else{
    station->queue.tail = station->queue.tail->previous;
    station->queue.tail->next = NULL;
  }
  item->previous = NULL;
  item->next = NULL;
  return item;
}

void  return_node(Node* item){
  item->previous = NULL;
  item->next = NULL;
  item->event.type = 0;
  item->event.name[0] = '\0';
  item->event.service_time = 0.0;
  item->event.occur_time = 0.0;
  item->event.arrival_time = 0.0;
  if (available)
   available->previous = item;
 item->next = available;
 item->previous = NULL;
 available = item;
}
