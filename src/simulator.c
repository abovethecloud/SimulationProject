#include "simulator.h"


//long double End_time = END_TIME;
long double N_events_stop = N_EVENTS_PER_CYCLE;
Node* available;
long double clock = 0.0;
long double oldclock = 0.0;
long double T = 0.0;
long double consumption_time = CONSUMPTION_TIME;
CycleMeasures cyclemeasures = {0};


int reg_cycle_n = MIN_REG_N;

int reached_end = 0;

void simulate(System *sys)
{
    int i;
    Means means = {0};

    /* Initialize system */
    initialize(sys);

    for (i = 0; i < reg_cycle_n+1; i++)
    {
        if (i>0){
            //End_time = clock + END_TIME;
            N_events_stop = sys->event_counter + N_EVENTS_PER_CYCLE;
            reached_end = 0;
            oldclock = clock;
            reset_stations_measurements(sys->stations);
        }

        /* Print report (if DEBUG is ON) and THEN run */
        do {
            #ifdef DEBUG  // Print DEBUG
            system_recap(*sys);
            fprintf(stderr, "CONSUMPTION TIME: %Lf\n", consumption_time);
            getchar();
            #endif
        } while (!engine(sys));

        // Update Measurements every Regeneration Cycle
        if (i > 0) {  // Change to ">" to avoid taking into account the first cycle
            T = clock - oldclock;
            update_mean_measures(&means, sys->stations, i);
            fprintf(stderr, "waiting area = %40.20Lf\n", means.squared_sum_waiting_area[1]);

            /* Final prints */
            system_recap(*sys);

            fprintf(stderr, "N_dep from Server: %d\n", sys->stations[1].measures.departures_n);
            fprintf(stderr, "N_arr to Server: %d\n", sys->stations[1].measures.arrivals_n);
            fprintf(stderr, "Final clock: %Lf\n", clock);
            fprintf(stderr, "Throughput of station 1: %Lf\n", sys->stations[1].measures.departures_n/T);
        }
    }
    compute_statistics(sys, means);
    //fprintf(stderr, "Mean number of Jobs at station 0: %Lf\n", sys->statistics.mean_number_jobs[0]);  // TODO: CHANGE
    //fprintf(stderr, "Mean number of Jobs at station 1: %Lf\n", sys->statistics.mean_number_jobs[1]);
    //fprintf(stderr, "Mean number of Jobs in system: %Lf\n", sys->statistics.mean_number_jobs[0] + sys->statistics.mean_number_jobs[1]);
    //fprintf(stderr, "Mean waiting: %Lf\n", sys->statistics.mean_waiting_time[1]);
    //fprintf(stderr, "Waiting semi_interval: %Lf\n", sys->statistics.semi_interval_waiting_time[1]);
    //fprintf(stderr, "CONFIDENCE INTERVAL: [%13.10Lf, %13.10Lf]\n", sys->statistics.mean_waiting_time[1]-sys->statistics.semi_interval_waiting_time[1], sys->statistics.mean_waiting_time[1]+sys->statistics.semi_interval_waiting_time[1]);
    fprintf(stderr, "Mean cycle time: %Lf\n", sys->statistics.mean_cycle_time);
    fprintf(stderr, "Cycle time semi-interval: %Lf\n", sys->statistics.semi_interval_cycle_time);
    fprintf(stderr, "CONFIDENCE INTERVAL: [%Lf, %Lf]\n", sys->statistics.mean_cycle_time - sys->statistics.semi_interval_cycle_time, sys->statistics.mean_cycle_time + sys->statistics.semi_interval_cycle_time);
}

void initialize(System *sys_point)
{
    /* Initialize clock, FEL and node pool and allocate memory for stations */
    clock = 0;
    oldclock = 0;
    sys_point->event_counter = 0;
    initialize_stations(&(sys_point->stations));
    sys_point->fel = NULL;

    starting_events(&(sys_point->fel), (sys_point->stations));
    set_renewal_state(sys_point);
}

void initialize_stations(Station **pointer_to_stations)
{
    *pointer_to_stations = calloc(N_STATIONS, sizeof(Station));
    Station *stat = *pointer_to_stations;

    stat[0].type = 'D';
    stat[0].distribution = 'e';
    stat[0].parameters[0] = 60.0;
    stat[0].parameters[1] = 0.0;
    stat[0].prob_to_stations[0] = 0.0;
    stat[0].prob_to_stations[1] = 1.0;
    stat[0].prob_to_stations[2] = 0.0;
    stat[0].prob_to_stations[3] = 0.0;
    stat[0].prob_to_stations[4] = 0.0;
    stat[0].prob_to_stations[5] = 0.0;
    stat[0].queue.head = NULL;
    stat[0].queue.tail = NULL;
    stat[0].jobs_in_service = 0;
    stat[0].jobs_in_queue = 0;
    stat[0].server_n = 0;  // Does not apply
    stat[0].coffe_prob = 0.0;  // Does not apply
    stat[0].coffe_distribution = '\0';  // Does not apply
    stat[0].coffe_parameter = 0.0;  // Does not apply

    stat[1].type = 'M';
    stat[1].distribution = 'E';
    stat[1].parameters[0] = 50.0;
    stat[1].parameters[1] = 250.0;
    stat[1].prob_to_stations[0] = 0.0;
    stat[1].prob_to_stations[1] = 0.0;
    stat[1].prob_to_stations[2] = 1.0;
    stat[1].prob_to_stations[3] = 0.0;
    stat[1].prob_to_stations[4] = 0.0;
    stat[1].prob_to_stations[5] = 0.0;
    stat[1].queue.head = NULL;
    stat[1].queue.tail = NULL;
    stat[1].jobs_in_service = 0;
    stat[1].jobs_in_queue = 0;
    stat[1].server_n = 1;
    stat[1].coffe_prob = 0.0;
    stat[1].coffe_distribution = '\0';
    stat[1].coffe_parameter = 0.0;

    stat[2].type = 'S';
    stat[2].distribution = 'e';
    stat[2].parameters[0] = 80.0;
    stat[2].parameters[1] = 0.0;
    stat[2].prob_to_stations[0] = 0.0;
    stat[2].prob_to_stations[1] = 0.0;
    stat[2].prob_to_stations[2] = 0.0;
    stat[2].prob_to_stations[3] = 0.2;
    stat[2].prob_to_stations[4] = 0.8;
    stat[2].prob_to_stations[5] = 0.0;
    stat[2].queue.head = NULL;
    stat[2].queue.tail = NULL;
    stat[2].jobs_in_service = 0;
    stat[2].jobs_in_queue = 0;
    stat[2].server_n = 1;
    stat[2].coffe_prob = 0.0;
    stat[2].coffe_distribution = '\0';
    stat[2].coffe_parameter = 0.0;

    stat[3].type = 'S';
    stat[3].distribution = 'e';
    stat[3].parameters[0] = 100.0;
    stat[3].parameters[1] = 0.0;
    stat[3].prob_to_stations[0] = 0.0;
    stat[3].prob_to_stations[1] = 1.0;
    stat[3].prob_to_stations[2] = 0.0;
    stat[3].prob_to_stations[3] = 0.0;
    stat[3].prob_to_stations[4] = 0.0;
    stat[3].prob_to_stations[5] = 0.0;
    stat[3].queue.head = NULL;
    stat[3].queue.tail = NULL;
    stat[3].jobs_in_service = 0;
    stat[3].jobs_in_queue = 0;
    stat[3].server_n = 1;
    stat[3].coffe_prob = 0.0;
    stat[3].coffe_distribution = '\0';
    stat[3].coffe_parameter = 0.0;

    stat[4].type = 'D';
    stat[4].distribution = 'e';
    stat[4].parameters[0] = 50.0;
    stat[4].parameters[1] = 0.0;
    stat[4].prob_to_stations[0] = 0.6;
    stat[4].prob_to_stations[1] = 0.0;
    stat[4].prob_to_stations[2] = 0.0;
    stat[4].prob_to_stations[3] = 0.0;
    stat[4].prob_to_stations[4] = 0.0;
    stat[4].prob_to_stations[5] = 0.4;
    stat[4].queue.head = NULL;
    stat[4].queue.tail = NULL;
    stat[4].jobs_in_service = 0;
    stat[4].jobs_in_queue = 0;
    stat[4].server_n = 0;  // Does not apply
    stat[4].coffe_prob = 0.0;  // Does not apply
    stat[4].coffe_distribution = '\0';  // Does not apply
    stat[4].coffe_parameter = 0.0;  // Does not apply

    stat[5].type = 'S';
    stat[5].distribution = 'e';
    stat[5].parameters[0] = 90.0;
    stat[5].parameters[1] = 0.0;
    stat[5].prob_to_stations[0] = 1.0;
    stat[5].prob_to_stations[1] = 0.0;
    stat[5].prob_to_stations[2] = 0.0;
    stat[5].prob_to_stations[3] = 0.0;
    stat[5].prob_to_stations[4] = 0.0;
    stat[5].prob_to_stations[5] = 0.0;
    stat[5].queue.head = NULL;
    stat[5].queue.tail = NULL;
    stat[5].jobs_in_service = 0;
    stat[5].jobs_in_queue = 0;
    stat[5].server_n = 1;
    stat[5].coffe_prob = 0.0;
    stat[5].coffe_distribution = '\0';
    stat[5].coffe_parameter = 0.0;

    reset_stations_measurements(stat);
}

void reset_stations_measurements(Station *stations)
{
    int i;
    for (i = 0; i < N_STATIONS; i++)
    {
        Measurements *measures = &(stations[i].measures);
        measures->arrivals_n = 0;
        measures->departures_n = 0;
        measures->waiting_area = 0.0;
    }
}

void starting_events(Tree *pointer_to_fel, Station *stations)
{
    Node *new_notice;

    new_notice = get_new_node(available);
    sprintf(new_notice->event.name, "J%d", 0);
    new_notice->event.type = ARRIVAL;
    new_notice->event.station = 0;  // First arrival to station 0
    new_notice->event.create_time = clock;
    new_notice->event.occur_time = 0.0;
    new_notice->event.service_time = 0.0;
    new_notice->next = NULL;
    new_notice->previous = NULL;
    schedule(new_notice, pointer_to_fel);

    new_notice = get_new_node(available);
    sprintf(new_notice->event.name, "J%d", 1);
    new_notice->event.type = ARRIVAL;
    new_notice->event.station = 0;  // First arrival to station 0
    new_notice->event.create_time = clock;
    new_notice->event.occur_time = 0.0;
    new_notice->event.service_time = 0.0;
    new_notice->next = NULL;
    new_notice->previous = NULL;
    schedule(new_notice, pointer_to_fel);

    new_notice = get_new_node(available);
    sprintf(new_notice->event.name, "J%d", 2);
    new_notice->event.type = ARRIVAL;
    new_notice->event.station = 2;  // First arrival to station 0
    new_notice->event.create_time = clock;
    new_notice->event.occur_time = 0.0;
    new_notice->event.service_time = 0.0;
    new_notice->next = NULL;
    new_notice->previous = NULL;
    schedule(new_notice, pointer_to_fel);

    new_notice = get_new_node(available);
    sprintf(new_notice->event.name, "J%d", 3);
    new_notice->event.type = ARRIVAL;
    new_notice->event.station = 2;  // First arrival to station 0
    new_notice->event.create_time = clock;
    new_notice->event.occur_time = 0.0;
    new_notice->event.service_time = 0.0;
    new_notice->next = NULL;
    new_notice->previous = NULL;
    schedule(new_notice, pointer_to_fel);

    new_notice = get_new_node(available);
    sprintf(new_notice->event.name, "J%d", 4);
    new_notice->event.type = ARRIVAL;
    new_notice->event.station = 2;  // First arrival to station 0
    new_notice->event.create_time = clock;
    new_notice->event.occur_time = 0.0;
    new_notice->event.service_time = 0.0;
    new_notice->next = NULL;
    new_notice->previous = NULL;
    schedule(new_notice, pointer_to_fel);

    new_notice = get_new_node(available);
    sprintf(new_notice->event.name, "J%d", 5);
    new_notice->event.type = ARRIVAL;
    new_notice->event.station = 3;  // First arrival to station 0
    new_notice->event.create_time = clock;
    new_notice->event.occur_time = 0.0;
    new_notice->event.service_time = 0.0;
    new_notice->next = NULL;
    new_notice->previous = NULL;
    schedule(new_notice, pointer_to_fel);

    new_notice = get_new_node(available);
    sprintf(new_notice->event.name, "J%d", 6);
    new_notice->event.type = ARRIVAL;
    new_notice->event.station = 3;  // First arrival to station 0
    new_notice->event.create_time = clock;
    new_notice->event.occur_time = 0.0;
    new_notice->event.service_time = 0.0;
    new_notice->next = NULL;
    new_notice->previous = NULL;
    schedule(new_notice, pointer_to_fel);

    new_notice = get_new_node(available);
    sprintf(new_notice->event.name, "J%d", 7);
    new_notice->event.type = ARRIVAL;
    new_notice->event.station = 4;  // First arrival to station 0
    new_notice->event.create_time = clock;
    new_notice->event.occur_time = 0.0;
    new_notice->event.service_time = 0.0;
    new_notice->next = NULL;
    new_notice->previous = NULL;
    schedule(new_notice, pointer_to_fel);

    new_notice = get_new_node(available);
    sprintf(new_notice->event.name, "J%d", 8);
    new_notice->event.type = ARRIVAL;
    new_notice->event.station = 4;  // First arrival to station 0
    new_notice->event.create_time = clock;
    new_notice->event.occur_time = 0.0;
    new_notice->event.service_time = 0.0;
    new_notice->next = NULL;
    new_notice->previous = NULL;
    schedule(new_notice, pointer_to_fel);

    new_notice = get_new_node(available);
    sprintf(new_notice->event.name, "J%d", 9);
    new_notice->event.type = ARRIVAL;
    new_notice->event.station = 5;  // First arrival to station 0
    new_notice->event.create_time = clock;
    new_notice->event.occur_time = 0.0;
    new_notice->event.service_time = 0.0;
    new_notice->next = NULL;
    new_notice->previous = NULL;
    schedule(new_notice, pointer_to_fel);
}

int engine(System *sys)
{
    Station *stations = sys->stations;
    Tree *pointer_to_fel = &(sys->fel);

    /* Initializations */
    int halt = 0;
    int event_in_0 = 0;

    /* Get next event from FEL */
    Node* new_event = event_pop(pointer_to_fel);

    /* update clock and check if reached End_time */
    long double oldtime = clock;
    long double delta = update_clock(new_event, oldtime);
    //if (clock >= End_time)
    if (sys->event_counter >= N_events_stop)
        reached_end = 1;
    if ((new_event->event.station == 0) && (new_event->event.type == ARRIVAL))
        event_in_0 = 1;

    update_stations_measurements(sys, delta);

    switch(new_event->event.type)
    {
        case ARRIVAL:
            #ifdef DEBUG
                fprintf(stderr, "Handled ARRIVAL at station %d\n", new_event->event.station);
            #endif
            arrival(new_event, stations, pointer_to_fel);
        break;
        case DEPARTURE:
            #ifdef DEBUG
                fprintf(stderr, "Handled DEPARTURE at station %d\n", new_event->event.station);
            #endif
            departure(new_event, stations, pointer_to_fel);
        break;
        case SELF_TRANSITION:
            #ifdef DEBUG
                fprintf(stderr, "Handled SELF_TRANSITION at station %d\n", new_event->event.station);
            #endif
            self_transition(new_event, stations, pointer_to_fel);
        break;
    }

    sys->event_counter++;

    if (reached_end)
    {
        if ((compare_stations_state(sys->stations, sys->initialized_stations)) && (event_in_0 == 1))
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

    stations[station_index].measures.arrivals_n++;

    // If arrival at UNLOAD, record it
    if (station_index == 4) {
        if (node_event->event.arrivals_at_Unload == 0)
            node_event->event.first_arrival_at_Unload = clock;

        node_event->event.last_arrival_at_Unload = clock;
        node_event->event.arrivals_at_Unload++;

        if ((node_event->event.arrivals_at_Unload > node_event->event.departures_from_Load) && (node_event->event.departures_from_Load > 0)){
            cyclemeasures.sum_manufacturing_n++;
            cyclemeasures.sum_manufacturing_time += (node_event->event.last_arrival_at_Unload - node_event->event.last_departure_from_Load);
        }
    }

    switch (station_type)
    {
        case 'S':
            arrival_at_server(node_event, stations, pointer_to_fel);
        break;
        case 'M':
            arrival_at_M1(node_event, stations, pointer_to_fel);
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

    /* Change into a departure from same station without queuing */
    node_event->event.type = DEPARTURE;

    if (node_event->event.service_time == 0.0){
            node_event->event.service_time = station_random_time(stations, station_index);
    }

    node_event->event.occur_time = clock + node_event->event.service_time;
    node_event->event.service_time = 0.0;  // If the event gets completed, reset its service time

    schedule(node_event, pointer_to_fel);  // Schedule the departure
}

void arrival_at_server(Node* node_event, Station *stations, Tree *pointer_to_fel)
{
    int station_index = node_event->event.station;
    int station_full = 0;

    /* Determine wether station is full */
    if (stations[station_index].jobs_in_service == stations[station_index].server_n)
        station_full = 1;

    node_event->event.arrival_time = clock;

    /* Process arrival at non-full server */
    if (!station_full)
    {
        stations[station_index].jobs_in_service++;  // Add a job in service

        /* Change into a departure from same station after service time */
        node_event->event.type = DEPARTURE;

        /* If a service time has not still be assigned to the job, it gets assigned based on the properties of the station,
         * otherwise keep going from the previous service time */
        if (node_event->event.service_time == 0.0){
            node_event->event.service_time = station_random_time(stations, station_index);
        }

        node_event->event.occur_time = clock + node_event->event.service_time;
        node_event->event.service_time = 0.0;  // If the event gets completed, reset its service time

        schedule(node_event, pointer_to_fel);  // Schedule departure
    }
    /* Process arrival at full server */
    else {
        enqueue(node_event, &stations[node_event->event.station]);
        stations[station_index].jobs_in_queue++;
    }
}

void arrival_at_M1(Node* node_event, Station *stations, Tree *pointer_to_fel)
{
    double served_time = 0.0;
    int station_index = node_event->event.station;
    int station_full = 0;

    /* Determine wether station is full */
    if (stations[station_index].jobs_in_service == stations[station_index].server_n)
        station_full = 1;

    node_event->event.arrival_time = clock;

    /* Process arrival at non-full server */
    if (!station_full)
    {
        stations[station_index].jobs_in_service++;  // Add a job in service

        /* If a service time has not still be assigned to the job, it gets assigned based on the properties of the station,
         * otherwise keep going from the previous service time */
        if (approx_equal(node_event->event.service_time, 0.0)){
            node_event->event.service_time = station_random_time(stations, station_index);
        }

        if (approx_equal(node_event->event.service_time, consumption_time)) {
            consumption_time = CONSUMPTION_TIME;  // reset consumption time and schedule departure
            /* Change into a departure from same station after service time */
            node_event->event.type = DEPARTURE;

            node_event->event.occur_time = clock + node_event->event.service_time;
            node_event->event.service_time = 0.0;  // If the event gets completed, reset its service time

            schedule(node_event, pointer_to_fel);  // Schedule departure

        } else if (node_event->event.service_time < consumption_time) {
            //consumption_time -= node_event->event.service_time;  // decrease remaining consumption time and schedule departure
            consumption_time = CONSUMPTION_TIME;  // reset consumption time and schedule departure BECAUSE NO MEMORY
            node_event->event.type = DEPARTURE;

            node_event->event.occur_time = clock + node_event->event.service_time;
            node_event->event.service_time = 0.0;  // If the event gets completed, reset its service time

            schedule(node_event, pointer_to_fel);  // Schedule departure

        } else if (node_event->event.service_time > consumption_time) {
            served_time = consumption_time;
            node_event->event.service_time -= served_time;  // decrease remaining service time by consumption time, then reset it
            consumption_time = CONSUMPTION_TIME;  // reset consumption time and reschedule arrival at same station

            node_event->event.type = SELF_TRANSITION;
            node_event->event.occur_time = clock + served_time;
            schedule(node_event, pointer_to_fel);  // Schedule Self transition

        }

    }
    /* Process arrival at full server */
    else {
        enqueue(node_event, &stations[node_event->event.station]);
        stations[station_index].jobs_in_queue++;
    }
}

void self_transition(Node* node_event, Station *stations, Tree *pointer_to_fel)
{
    int station_index = node_event->event.station;

    // stations[station_index].measures.departures_n++;
    stations[station_index].jobs_in_service--;

    stations[station_index].measures.arrivals_n--;  // PREVENTIVE: since a fake arrival is rescheduled in the same station

    Node* next_job;
    if (stations[station_index].queue.tail) {
        stations[station_index].measures.arrivals_n--;  // since an "arrival" is handled from the queue

        /* Process departure from a server with a queue by dequeuing and immedeatly scheduling another departure from the same server */
        next_job = dequeue(&stations[node_event->event.station]);
        stations[station_index].jobs_in_queue--;
        arrival(next_job, stations, pointer_to_fel);
    }

    /* Change departing job into arrival at same station at same time*/
    node_event->event.type = ARRIVAL;
    node_event->event.station = station_index;

    schedule(node_event, pointer_to_fel);  // Schedule arrival to next station
}

void departure(Node* node_event, Station *stations, Tree *pointer_to_fel)
{
    int station_index = node_event->event.station;
    char station_type = stations[station_index].type;
    long double lastt = 0.0;

    stations[station_index].measures.departures_n++;

    // If departure from LOAD, update event notice with time stamps
    if (station_index == 0) {
        if (node_event->event.departures_from_Load == 0)  // Only the first time set first arrival
            node_event->event.first_departure_from_Load = clock;

        lastt = node_event->event.last_departure_from_Load;

        node_event->event.last_departure_from_Load = clock;
        node_event->event.departures_from_Load++;

        if (node_event->event.departures_from_Load > 1){
            cyclemeasures.sum_cycle_n++;
            cyclemeasures.sum_cycle_time += (node_event->event.last_departure_from_Load - lastt);
        }
    }

    switch (station_type)
    {
        case 'S':
            departure_from_server(node_event, stations, pointer_to_fel);
        break;
        case 'M':
            departure_from_M1(node_event, stations, pointer_to_fel);
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

    /* Change into arrival in next station at same time of departure */
    node_event->event.type = ARRIVAL;
    node_event->event.station = next_station(stations, station_index);

    schedule(node_event, pointer_to_fel);  // Schedule ARRIVAL
}


void departure_from_server(Node* node_event, Station *stations, Tree *pointer_to_fel)
{
    int station_index = node_event->event.station;
    long double coffe_length = 0;

    stations[station_index].jobs_in_service--;  // Decrease the number of jobs in service for the station

    /* If there is something in queue for the station, schedule the arrival at same station */
    Node* next_job;
    if (stations[station_index].queue.tail) {
        /* Process departure from a server with a queue by dequeuing and immedeatly scheduling another departure from the same server */
        next_job = dequeue(&stations[node_event->event.station]);
        stations[station_index].jobs_in_queue--;

        /* Process arrival at non-full server (this is sure since there has just been a departure) */
        stations[station_index].jobs_in_service++;  // Add a job in service

        /* In case of a coffe break, schedule a longer repair to take the pause into account */
        coffe_length = coffe_break(stations, station_index);

        /* Schedule a departure from same station after service time and maybe coffe break */
        next_job->event.type = DEPARTURE;
        if (approx_equal(next_job->event.service_time, 0.0)){
            next_job->event.service_time = station_random_time(stations, station_index);
        }

        next_job->event.occur_time = clock + coffe_length + next_job->event.service_time;
        next_job->event.service_time = 0.0;  // If the event gets completed, reset its service time

        schedule(next_job, pointer_to_fel);
    }

    /* Change into arrival at next station */
    node_event->event.type = ARRIVAL;
    node_event->event.station = next_station(stations, station_index);

    schedule(node_event, pointer_to_fel);  // Schedule arrival to next station
}

void departure_from_M1(Node* node_event, Station *stations, Tree *pointer_to_fel)
{
    int station_index = node_event->event.station;

    stations[station_index].jobs_in_service--;  // Decrease the number of jobs in service for the station

    /* If there is something in queue for the station, schedule the arrival at same station */
    Node* next_job;
    if (stations[station_index].queue.tail) {
        /* Process departure from a server with a queue by dequeuing and immedeatly scheduling another departure from the same server */
        next_job = dequeue(&stations[node_event->event.station]);
        stations[station_index].jobs_in_queue--;
         stations[station_index].measures.arrivals_n--;
        arrival(next_job, stations, pointer_to_fel);
    }

    /* Change into arrival at next station */
    node_event->event.type = ARRIVAL;
    node_event->event.station = next_station(stations, station_index);

    schedule(node_event, pointer_to_fel);  // Schedule arrival to next station
}

long double update_clock(Node* new_event, long double oldtime)
{
    long double delta = 0.0;
    clock = new_event->event.occur_time;
    delta = clock - oldtime;
    return delta;
}

int next_station(Station *stations, int current_station)
{
    int i;
    SelectStream(255);  // Set separate stream for extraction of next station (255 since it's the last - and of course free - stream)
    long double extraction = Uniform(0.0, 1.0);
    long double *prob_to_stations = stations[current_station].prob_to_stations;

    long double cumulative_prob = 0.0;

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

long double station_random_time(Station *stations, int station_index)
{
    long double service_time = 0;

    /* Select separate stream for each station arrival */
    SelectStream(station_index);

    switch (stations[station_index].distribution)
    {
        case 'e':  // negative exponential
            service_time = Exponential(stations[station_index].parameters[0]);
        break;
        case 'E':  // hyper-exponential
            service_time = HyperExponential(0.8, stations[station_index].parameters[0], 0.2, stations[station_index].parameters[1]);
        break;
    }
    return service_time;
}

long double coffe_break(Station *stations, int station_index)
{
    long double coffe_length = 0;

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
        (*new_stations_address)[i].parameters[0] =          stat[i].parameters[0];
        (*new_stations_address)[i].parameters[1] =          stat[i].parameters[1];
        (*new_stations_address)[i].prob_to_stations[0] =    stat[i].prob_to_stations[0];
        (*new_stations_address)[i].prob_to_stations[1] =    stat[i].prob_to_stations[1];
        (*new_stations_address)[i].prob_to_stations[2] =    stat[i].prob_to_stations[2];
        (*new_stations_address)[i].prob_to_stations[3] =    stat[i].prob_to_stations[3];
        (*new_stations_address)[i].prob_to_stations[4] =    stat[i].prob_to_stations[4];
        (*new_stations_address)[i].prob_to_stations[5] =    stat[i].prob_to_stations[5];
        (*new_stations_address)[i].queue.head =             stat[i].queue.head;
        (*new_stations_address)[i].queue.tail =             stat[i].queue.tail;
        (*new_stations_address)[i].jobs_in_service =        stat[i].jobs_in_service;
        (*new_stations_address)[i].jobs_in_queue =          stat[i].jobs_in_queue;
        (*new_stations_address)[i].measures.arrivals_n =    stat[i].measures.arrivals_n;
        (*new_stations_address)[i].measures.departures_n =  stat[i].measures.departures_n;
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

void set_renewal_state(System *sys_point)
{
    copy_stations(sys_point->stations, &(sys_point->initialized_stations));
    sys_point->initialized_stations[0].jobs_in_service = 2;
    sys_point->initialized_stations[0].jobs_in_queue = 0;
    sys_point->initialized_stations[1].jobs_in_service = 0;
    sys_point->initialized_stations[1].jobs_in_queue = 0;
    sys_point->initialized_stations[2].jobs_in_service = 1;
    sys_point->initialized_stations[2].jobs_in_queue = 2;
    sys_point->initialized_stations[3].jobs_in_service = 1;
    sys_point->initialized_stations[3].jobs_in_queue = 1;
    sys_point->initialized_stations[4].jobs_in_service = 2;
    sys_point->initialized_stations[4].jobs_in_queue = 0;
    sys_point->initialized_stations[5].jobs_in_service = 1;
    sys_point->initialized_stations[5].jobs_in_queue = 0;
}
