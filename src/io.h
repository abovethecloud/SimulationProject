#ifndef IO_H
#define IO_H

#include "globals.h"

void report();
void print_queue(Queue queue);
void print_tree(Tree tree);
void print_fel_v(Node *fel);
void print_fel_h(Node *fel);
void print_station(Station *stations, int index);
void print_all_stations_h(Station *stations);
void print_all_stations_v(Station *stations);
void system_recap(System sys);

#endif
