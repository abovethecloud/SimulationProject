#ifndef GLOBALS_H
#define GLOBALS_H


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "rngs.h"
#include "rvgs.h"


#define ARRIVAL 0
#define DEPARTURE 1
#define END 2

#define N_STATIONS 2
#define END_TIME 500

#define FILENAME   "./data/ssq1.dat"  /* input data file */


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
} Station;

typedef struct statistic_measures
{
    int narr, ncom;
    int job_number, event_counter, node_number;
} Statistics;

typedef struct system
{
    int event_counter;
    Station *stations;
    Station *initialized_stations;
    Tree fel;
} System;


extern Node* available;


#endif
