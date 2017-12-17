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
#define N_STATIONS 2
#define END_TIME 10000000
#define N_CLIENTS 10

// Graphical parameters. Number of horizontal stations or events per row
#define MAX_STAT_COL 5  // Max number of stations per row
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
    double create_time;
    double occur_time;
    double arrival_time;
    double service_time;
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
    double area_jobs;
    double mean_number_jobs;
    double waiting_area;
    double mean_waiting_time;
} Statistics;

typedef struct station
{
    char type;
    char distribution;
    double parameter;
    double prob_to_stations[N_STATIONS];
    Queue queue;
    int jobs_in_service;
    int jobs_in_queue;
    int arrivals_n;
    int departures_n;
    int server_n;  // Only for servers
    double coffe_prob;
    char coffe_distribution;
    double coffe_parameter;
    Statistics statistics;
} Station;

typedef struct system
{
    int event_counter;
    Station *stations;
    Station *initialized_stations;
    Tree fel;
} System;


extern Node* available;
extern double clock;

#endif
