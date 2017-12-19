#ifndef GLOBALS_H  // Protection against multiple inclusion
#define GLOBALS_H


/*========== GLOBAL INCLUSIONS ==========*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "rngs.h"
#include "rvgs.h"
/*=======================================*/


/*==================== EDIT THOSE VALUES ACCORDINGLY ====================*/
// Remember to update the following values as stations and clients are added!!!
#define N_STATIONS 6
//#define END_TIME 10000000
#define N_EVENTS_PER_CYCLE 10000
#define N_CLIENTS 10

#define MIN_REG_N 30

// Graphical parameters. Number of horizontal stations or events per row
#define MAX_STAT_COL 6  // Max number of stations per row
#define MAX_EV_COL 10  // Max number of events per row

// Comment or uncomment the following line to (de)activate general step-by-step prints throughout the simulator
//#define DEBUG
// Comment or uncomment the following line to (de)activate debug for current bugs
#define CURR_DEBUG
/*=======================================================================*/


/*========== DEFINITIONS FOR TYPE OF EVENT ==========*/
#define ARRIVAL 0
#define DEPARTURE 1
#define END 2  // Deprecated - Not in use anymore
/*===================================================*/


typedef struct event_notice
{
    char name[256];
    int type;
    int station;
    long double create_time;
    long double occur_time;
    long double arrival_time;
    long double service_time;
} EventNotice;

typedef struct node
{
    EventNotice event;
    struct node* previous;   // Pointer to the previous node
    struct node* next;       // Pointer to the next node
} Node;

typedef struct queue
{
    Node* head;
    Node* tail;
} Queue;

typedef Node* Tree;

typedef struct statistic_measures
{
    long double mean_number_jobs[N_STATIONS];

    long double mean_waiting_time[N_STATIONS];
    long double del_waiting_time[N_STATIONS];
    long double semi_interval_waiting_time[N_STATIONS];

    long double mean_throughput[N_STATIONS];
} Statistics;

typedef struct measurements
{
    int arrivals_n;
    int departures_n;
    long double waiting_area;
} Measurements;

typedef struct run_means
{
    long double sum_observation_time;
    long double squared_sum_observation_time;
    long double mean_observation_time;

    int sum_arrivals[N_STATIONS];
    long double squared_sum_arrivals[N_STATIONS];
    long double mean_arrivals[N_STATIONS];

    int sum_departures[N_STATIONS];
    long double squared_sum_departures[N_STATIONS];
    long double mean_departures[N_STATIONS];

    long double sum_waiting_area[N_STATIONS];
    long double squared_sum_waiting_area[N_STATIONS];
    long double mean_waiting_area[N_STATIONS];

    long double sum_of_product_waiting_and_arrivals[N_STATIONS];
} Means;

typedef struct station
{
    char type;
    char distribution;
    long double parameters[2];
    long double prob_to_stations[N_STATIONS];
    Queue queue;
    int jobs_in_service;
    int jobs_in_queue;
    int server_n;  // Only for servers
    long double coffe_prob;
    char coffe_distribution;
    long double coffe_parameter;
    Measurements measures;
} Station;

typedef struct system
{
    int event_counter;
    Station *stations;
    Station *initialized_stations;
    Tree fel;
    Statistics statistics;
} System;


extern Node* available;
extern long double clock;
extern long double oldclock;
extern long double T;
extern long double meanT;
extern long double mean_arrivals_at1;
extern int reg_cycle_n;

#endif
