#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "globals.h"
#include "list_manager.h"
#include "io.h"
#include "utils.h"
#include "statistics.h"

/*
 * Function:  simulate
 * --------------------
 * simulates the scenario.
 * calls "initialize" to initialize the structure.
 */
void simulate(System *sys);

/*
 * Function:  initialize
 * --------------------
 * initializes the stations of the project.
 */
void initialize(System *sys_point);

void initialize_stations(Station **pointer_to_stations);

void reset_stations_measurements(Station *stations);

void starting_events(Tree *pointer_to_fel, Station *stations);

long double getArrivalFromOutside();

long double getServiceFromOutside();

/*
 * Function: engine
 * --------------------
 * pops next event from FEL and calls function to handle it depending on
 * its type.
 *
 *  returns: halt condition. 1 if the simulation has to stop, 0 if it has to continue
 */
int engine(System *sys);

void arrival(Node* node_event, Station *stations, Tree *pointer_to_fel);

void arrival_at_delay(Node* node_event, Station *stations, Tree *pointer_to_fel);

void arrival_at_server(Node* node_event, Station *stations, Tree *pointer_to_fel);

void arrival_at_M1(Node* node_event, Station *stations, Tree *pointer_to_fel);

void self_transition(Node* node_event, Station *stations, Tree *pointer_to_fel);

void departure(Node* node_event, Station *stations, Tree *pointer_to_fel);

void departure_from_delay(Node* node_event, Station *stations, Tree *pointer_to_fel);

void departure_from_server(Node* node_event, Station *stations, Tree *pointer_to_fel);

void departure_from_M1(Node* node_event, Station *stations, Tree *pointer_to_fel);

long double update_clock(Node* new_event, long double oldtime);

/*
 * Function:  next_station
 * --------------------
 * decides the next station for a client using the probabilities defined for the
 * current station.
 *
 * stations:          pointer to the array of stations (of size N_STATIONS)
 * current_station:   index of the current station in array "stations"
 *
 *  returns: index of next station
 */
int next_station(Station *stations, int current_station);

long double station_random_time(Station *stations, int station_index);

long double coffe_break(Station *stations, int station_index);

void copy_stations(Station *stations, Station **new_stations_address);

int compare_stations_state(Station *s1, Station *s2);

void set_renewal_state(System *sys_point);

#endif
