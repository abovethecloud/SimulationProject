#ifndef IO_H
#define IO_H

#include "globals.h"

void report();
void print_queue(Queue queue);
void print_tree(Tree tree);
void print_fel(Node *fel);
void print_station(Station *stations, int index);
void print_all_stations(Station *stations);
void system_recap(System sys);

#endif
