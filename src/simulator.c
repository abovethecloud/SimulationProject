#include "simulator.h"

#define DEBUG


const double End_time = END_TIME;
Node* available;
double clock = 0;
int reached_end = 0;

void simulate()
{
    System true_sys;
    System *sys = &true_sys;

    /* Initialize system */
    initialize(sys);

#ifdef DEBUG
    /* Print DEBUG */
    system_recap(true_sys);
    getchar();
#endif

    /* Run and print report at every cycle */
    while (!engine(sys))
    {
        if (sys->event_counter == 10)  // Check that all 10 customers have arrived in delay station. Initial conditions.
        {
            copy_stations(sys->stations, &(sys->initialized_stations));
        }

#ifdef DEBUG
        /* Print DEBUG */
        system_recap(true_sys);
        getchar();
#endif

    }

    // Compute final statistics
    sys->stations[0].statistics.mean_number_jobs = sys->stations[0].statistics.area_jobs / (clock);
    sys->stations[1].statistics.mean_number_jobs = sys->stations[1].statistics.area_jobs / (clock);

    fprintf(stderr, "Mean number of Jobs at station 0: %lf\n", sys->stations[0].statistics.mean_number_jobs);
    fprintf(stderr, "Mean number of Jobs at station 1: %lf\n", sys->stations[1].statistics.mean_number_jobs);
    fprintf(stderr, "Mean number of Jobs in system: %lf\n", sys->stations[1].statistics.mean_number_jobs + sys->stations[0].statistics.mean_number_jobs);

    fprintf(stderr, "N_dep: %d\n", sys->stations[1].departures_n);
    fprintf(stderr, "N_arr: %d\n", sys->stations[1].arrivals_n);
    fprintf(stderr, "Final clock: %lf\n", clock);
    fprintf(stderr, "Throughput: %lf\n", sys->stations[1].departures_n/clock);

}

void initialize(System *sys_point)
{
    /* Initialize clock, FEL and node pool and allocate memory for stations */
    clock = 0;
    sys_point->event_counter = 0;
    initialize_stations(&(sys_point->stations));
    sys_point->fel = NULL;

    starting_events(&(sys_point->fel), (sys_point->stations));
}

void initialize_stations(Station **pointer_to_stations)
{
    *pointer_to_stations = calloc(N_STATIONS, sizeof(Station));
    Station *stat = *pointer_to_stations;

    stat[0].type = 'D';
    stat[0].distribution = 'e';
    stat[0].parameter = 300.0;
    stat[0].prob_to_stations[0] = 0.0;
    stat[0].prob_to_stations[1] = 1.0;
    stat[0].queue.head = NULL;
    stat[0].queue.tail = NULL;
    stat[0].jobs_in_service = 0;
    stat[0].jobs_in_queue = 0;
    stat[0].arrivals_n = 0;
    stat[0].departures_n = 0;
    stat[0].server_n = 0;  // Does not apply
    stat[0].coffe_prob = 0.0;  // Does not apply
    stat[0].coffe_distribution = '\0';  // Does not apply
    stat[0].coffe_parameter = 0.0;  // Does not apply
    stat[0].statistics.area_jobs = 0.0;
    stat[0].statistics.mean_number_jobs = 0.0;

    stat[1].type = 'S';
    stat[1].distribution = 'e';
    stat[1].parameter = 40.0;
    stat[1].prob_to_stations[0] = 1.0;
    stat[1].prob_to_stations[1] = 0.0;
    stat[1].queue.head = NULL;
    stat[1].queue.tail = NULL;
    stat[1].jobs_in_service = 0;
    stat[1].jobs_in_queue = 0;
    stat[1].arrivals_n = 0;
    stat[1].departures_n = 0;
    stat[1].server_n = 1;
    stat[1].coffe_prob = 0.0;
    stat[1].coffe_distribution = 'e';
    stat[1].coffe_parameter = 10;
    stat[1].statistics.area_jobs = 0.0;
    stat[1].statistics.mean_number_jobs = 0.0;
}

void starting_events(Tree *pointer_to_fel, Station *stations)
{
    /* Initialize new node to be END event */

    /*
    Node *end_notice = get_new_node(available);
    sprintf(end_notice->event.name, "END");
    end_notice->event.type = END;
    end_notice->event.station = -1;  // OUTSIDE
    end_notice->event.create_time = clock;
    end_notice->event.occur_time = End_time;
    end_notice->next = NULL;
    end_notice->previous = NULL;
    */

    /*
    Node *first_notice = get_new_node(available);
    sprintf(first_notice->event.name, "J");
    first_notice->event.type = ARRIVAL;
    first_notice->event.station = 0;  // First arrival to station 0
    first_notice->event.create_time = clock;
    first_notice->event.occur_time = 0.0;
    first_notice->next = NULL;
    first_notice->previous = NULL;

    Node *second_notice = get_new_node(available);
    sprintf(second_notice->event.name, "J");
    second_notice->event.type = ARRIVAL;
    second_notice->event.station = 0;  // Second arrival to station 1
    second_notice->event.create_time = clock;
    second_notice->event.occur_time = 0.0;
    second_notice->next = NULL;
    second_notice->previous = NULL;

    Node *third_notice = get_new_node(available);
    sprintf(third_notice->event.name, "J");
    third_notice->event.type = ARRIVAL;
    third_notice->event.station = 0;  // Second arrival to station 1
    third_notice->event.create_time = clock;
    third_notice->event.occur_time = 0.0;
    third_notice->next = NULL;
    third_notice->previous = NULL;
    */


    int i;
    Node *new_notice;
    for (i = 0; i < 10; i++)
    {
        new_notice = get_new_node(available);
        sprintf(new_notice->event.name, "J%d", i);
        new_notice->event.type = ARRIVAL;
        new_notice->event.station = 0;  // First arrival to station 0
        new_notice->event.create_time = clock;
        new_notice->event.occur_time = 0.0;
        new_notice->next = NULL;
        new_notice->previous = NULL;
        schedule(new_notice, pointer_to_fel);
    }

    /* Schedule END event in FEL */
    //schedule(end_notice, pointer_to_fel);


    /*
    schedule(first_notice, pointer_to_fel);
    schedule(second_notice, pointer_to_fel);
    schedule(third_notice, pointer_to_fel);
    */
}

int engine(System *sys)
{
    Station *stations = sys->stations;
    Tree *pointer_to_fel = &(sys->fel);

    /* Initializations */
    int halt = 0;

    /* Get next event from FEL */
    Node* new_event = event_pop(pointer_to_fel);
    /*
    fprintf(stderr, "NEW TIME: %lf\n", new_event->event.occur_time);
    fprintf(stderr, "NEW NAME: %s\n", new_event->event.name);
    */

    /* update clock and check if reached End_time */
    double oldtime = clock;
    double delta = update_clock(new_event, oldtime);
    if (clock >= End_time)
        reached_end = 1;

    stations[0].statistics.area_jobs += delta*(stations[0].jobs_in_service + stations[0].jobs_in_queue);
    stations[1].statistics.area_jobs += delta*(stations[1].jobs_in_service + stations[1].jobs_in_queue);

    switch(new_event->event.type)
    {
        case ARRIVAL:
            arrival(new_event, stations, pointer_to_fel);
        break;
        case DEPARTURE:
            departure(new_event, stations, pointer_to_fel);
        break;
        case END:
            fprintf(stderr, "WHAT? END does not exist anymore.\n");
        break;
    }

    sys->event_counter++;

    if (reached_end)
    {
        if (compare_stations_state(sys->stations, sys->initialized_stations))
        {
            halt = 1;
        }
    }

    return halt;
}

void arrival(Node* node_event, Station *stations, Tree *pointer_to_fel)
{
    int station_index = node_event->event.station;
    char station_type = stations[station_index].type;

    stations[station_index].arrivals_n++;

    switch (station_type)
    {
        case 'S':
            arrival_at_server(node_event, stations, pointer_to_fel);
        break;
        case 'D':
            arrival_at_delay(node_event, stations, pointer_to_fel);
        break;
    }
}

void arrival_at_delay(Node* node_event, Station *stations, Tree *pointer_to_fel)
{
    /* Increase number of jobs in serice */
    int station_index = node_event->event.station;
    stations[station_index].jobs_in_service++;

    /* Schedule a departure without queuing */
    node_event->event.type = DEPARTURE;
    // node_event->event.station non cambia

    node_event->event.occur_time = clock + station_random_time(stations, station_index);
    schedule(node_event, pointer_to_fel);
}

void arrival_at_server(Node* node_event, Station *stations, Tree *pointer_to_fel)
{
    int station_index = node_event->event.station;
    int station_full = 0;

    /* Determine wether station is full */
    if (stations[station_index].jobs_in_service == stations[station_index].server_n)
        station_full = 1;

    node_event->event.arrival_time = clock;
    if (!station_full)
    {
        /* Process arrival at non-full server */
        stations[station_index].jobs_in_service++;  // Add a job in service

        /* Schedule a departure from same station after service time */
        node_event->event.type = DEPARTURE;

        node_event->event.occur_time = clock + station_random_time(stations, station_index);
        schedule(node_event, pointer_to_fel);
    }
    else {
        /* Process arrival at full server */
        enqueue(node_event, &stations[node_event->event.station]);
        stations[station_index].jobs_in_queue++;
    }
}

void departure(Node* node_event, Station *stations, Tree *pointer_to_fel)
{
    int station_index = node_event->event.station;
    char station_type = stations[station_index].type;

    stations[station_index].departures_n++;

    switch (station_type)
    {
        case 'S':
            departure_from_server(node_event, stations, pointer_to_fel);
        break;
        case 'D':
            departure_from_delay(node_event, stations, pointer_to_fel);
        break;
    }
}

void departure_from_delay(Node* node_event, Station *stations, Tree *pointer_to_fel)
{
    /* Decrease number of jobs in service */
    int station_index = node_event->event.station;
    stations[station_index].jobs_in_service--;

    /* Schedule arrival in next station ignoring queuing */
    node_event->event.type = ARRIVAL;
    node_event->event.station = next_station(stations, station_index);
    // "node_event->event.occur_time" resta uguale
    schedule(node_event, pointer_to_fel);
}


void departure_from_server(Node* node_event, Station *stations, Tree *pointer_to_fel)
{
    int station_index = node_event->event.station;
    double coffe_length = 0;

    stations[station_index].jobs_in_service--;  // Decrease the number of jobs in service for the station

    /* If there is something in queue for the station, schedule the arrival at same station */
    Node* next_job;
    if (stations[station_index].queue.tail) {
        /* Process departure from a server with a queue by dequeuing and immedeatly scheduling another departure from the same server */
        next_job = dequeue(&stations[node_event->event.station]);
        stations[station_index].jobs_in_queue--;  // TODO: This actually reduces the number of jobs in queue before arrival

        /* Process arrival at non-full server */
        stations[station_index].jobs_in_service++;  // Add a job in service

        /* In case of a coffe break, schedule a longer repair to take the pause into account */
        coffe_length = coffe_break(stations, station_index);

        /* Schedule a departure from same station after service time and maybe coffe break */
        next_job->event.type = DEPARTURE;
        next_job->event.occur_time = clock + coffe_length + station_random_time(stations, station_index);
        schedule(next_job, pointer_to_fel);
        /*
        next_job->event.type = ARRIVAL;
        next_job->event.occur_time = clock;  // TODO: GENERALIZE
        schedule(next_job, pointer_to_fel);
        */
    }

    /* Schedule the arrival at next station */
    node_event->event.type = ARRIVAL;
    node_event->event.station = next_station(stations, station_index);  //TODO: GENERALIZE to NEXT STATION
    // "node_event->event.occur_time" resta uguale
    schedule(node_event, pointer_to_fel);
}

double update_clock(Node* new_event, double oldtime)
{
    double delta = 0;
    clock = new_event->event.occur_time;
    /* If clock is set to End_time, reset it to last time to avoid altering statistics */
    if (clock == End_time)
    {
        clock = oldtime;
    }
    delta = clock - oldtime;
    return delta;
}

int next_station(Station *stations, int current_station)
{
    int i;
    SelectStream(255);  // Set separate stream for extraction of next station (255 since it's the last - and of course free - stream)
    double extraction = Uniform(0.0, 1.0);
    double *prob_to_stations = stations[current_station].prob_to_stations;

    double cumulative_prob = 0.0;

    for (i = 0; i < N_STATIONS; i++)
    {
        cumulative_prob += prob_to_stations[i];
        if (extraction <= cumulative_prob)
            return i;
        //if (approx_equal(stations[current_station].prob_to_stations[i], 1.0))
            //return i;
    }
    return -1;
}

double station_random_time(Station *stations, int station_index)
{
    double service_time = 0;

    /* Select separate stream for each station arrival */
    SelectStream(station_index);

    switch (stations[station_index].distribution)
    {
        case 'e':
            service_time = Exponential(stations[station_index].parameter);
        break;
    }
    return service_time;
}

double coffe_break(Station *stations, int station_index)
{
    double coffe_length = 0;

    SelectStream(station_index + N_STATIONS);  // Avoid overlappings
    if (Uniform(0, 1) < stations[station_index].coffe_prob)
    {
        switch (stations[station_index].coffe_distribution)
        {
            case 'e':
                SelectStream(station_index + 2*N_STATIONS);  // Avoid overlappings (second level)
                coffe_length = Exponential(stations[station_index].coffe_parameter);
            break;
        }

    }
    return coffe_length;
}

void copy_stations(Station *stations, Station **new_stations_address)
{
    int i;
    Station *stat = stations;

    *new_stations_address = calloc(N_STATIONS, sizeof(Station));

    for (i = 0; i < N_STATIONS; i++)
    {
        (*new_stations_address)[i].type =                   stat[i].type;
        (*new_stations_address)[i].distribution =           stat[i].distribution;
        (*new_stations_address)[i].parameter =              stat[i].parameter;
        (*new_stations_address)[i].prob_to_stations[0] =    stat[i].prob_to_stations[0];
        (*new_stations_address)[i].prob_to_stations[1] =    stat[i].prob_to_stations[1];
        (*new_stations_address)[i].queue.head =             stat[i].queue.head;
        (*new_stations_address)[i].queue.tail =             stat[i].queue.tail;
        (*new_stations_address)[i].jobs_in_service =        stat[i].jobs_in_service;
        (*new_stations_address)[i].jobs_in_queue =          stat[i].jobs_in_queue;
        (*new_stations_address)[i].arrivals_n =             stat[i].arrivals_n;
        (*new_stations_address)[i].departures_n =           stat[i].departures_n;
        (*new_stations_address)[i].server_n =               stat[i].server_n;
        (*new_stations_address)[i].coffe_prob =             stat[i].coffe_prob;
        (*new_stations_address)[i].coffe_distribution =     stat[i].coffe_distribution;
        (*new_stations_address)[i].coffe_parameter =        stat[i].coffe_parameter;
    }

}

int compare_stations_state(Station *s1, Station *s2)
{
    int i;
    int equal = 1;

    for (i = 0; i < N_STATIONS; i++)
    {
        if ( (s1[i].jobs_in_service != s2[i].jobs_in_service) ||
             (s1[i].jobs_in_queue != s2[i].jobs_in_queue)
           ) {
            equal = 0;
        }
    }
    return equal;
}
