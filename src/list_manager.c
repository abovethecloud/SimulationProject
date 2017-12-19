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

  Node *next_event = (*pointer_to_fel);
  if ( ((*pointer_to_fel)->next) ) {
    (*pointer_to_fel) = (*pointer_to_fel)->next;
    (*pointer_to_fel)->previous = NULL;
  } else
    *pointer_to_fel = NULL;

  next_event->next = NULL;
  next_event->previous = NULL;

  return next_event;
}

void schedule(Node* node_event, Tree *pointer_to_fel)
{
  int n_equal = 0;
  int ev_type = node_event->event.type;
  long double ev_time = node_event->event.occur_time;

  Node *first_equal = NULL, *last_equal = NULL;
  Node *prev = NULL, *nex = NULL;
  Node *first = NULL, *last = NULL;

  /* If FEL is empty, set FEL to new event */
  if (!*pointer_to_fel){
    (*pointer_to_fel) = node_event;

    // Initialize next and previous
    (*pointer_to_fel)->next = NULL;
    (*pointer_to_fel)->previous = NULL;
    return;
  }

  /* Search for events at the same time */
  Node *temp = *pointer_to_fel;
  first = temp;

  while (temp) {
    if (approx_equal(temp->event.occur_time, ev_time)) {
      n_equal++;  // Counts events at same time
      if (n_equal == 1) {
        first_equal = temp;  // Save pointer to first contemporary event
      }
      last_equal = temp;  // Save pointer to last contemporary event
    }
    last = temp;

    temp = temp->next;  // Consider next event
  }

  temp = NULL;

  // TODO: Maybe it is possible to check if < first or > last to save time

  if (n_equal == 0) {
    // Check if before the first
    if (ev_time < first->event.occur_time) {
      temp = *pointer_to_fel;  // Save current first address
      *pointer_to_fel = node_event;  // Update to new first address

      (*pointer_to_fel)->next = temp;
      (*pointer_to_fel)->previous = NULL;
      temp->previous = (*pointer_to_fel);
      return;
    }
    // Check if later than last
    if (ev_time > last->event.occur_time) {
      last->next = node_event;
      node_event->previous = last;
      node_event->next = NULL;
      return;
    }

    temp = *pointer_to_fel;
    while (temp){
      if (ev_time < temp->event.occur_time) {  // If new_event is before temp, prepare to insert between temp->previous and temp
        prev = temp->previous;
        nex = temp;
        break;
      }
      /*
      if (!(temp->next)) {  // If temp is last element, prepare to insert after temp and before NULL
        prev = temp;
        nex = NULL;
        break;
      }
      */
      temp = temp->next;
    }

    // Insert node
    node_event->previous = prev;
    node_event->next = nex;

    // complete link with previous and next
    if (prev)
      prev->next = node_event;
    if (nex)
      nex->previous = node_event;
  }

  if (n_equal > 0) {
    if (ev_type == DEPARTURE) {
      prev = first_equal->previous;
      nex = first_equal;
    }
    if (ev_type == ARRIVAL){
      prev = last_equal;
      nex = last_equal->next;
    }

    // Insert node
    node_event->previous = prev;
    node_event->next = nex;

    // complete link with previous and next
    if (prev)
      prev->next = node_event;
    if (nex)
      nex->previous = node_event;
  }

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
