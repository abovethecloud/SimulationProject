#include "io.h"


void report(System sys)
{
    fprintf(stderr, "Mean cycle time: %Lf\n", sys.statistics.mean_cycle_time);
    fprintf(stderr, "Cycle time semi-interval: %Lf\n", sys.statistics.semi_interval_cycle_time);
    fprintf(stderr, "CONFIDENCE INTERVAL for CYCLE TIME: [%Lf, %Lf]\n", sys.statistics.mean_cycle_time - sys.statistics.semi_interval_cycle_time, sys.statistics.mean_cycle_time + sys.statistics.semi_interval_cycle_time);

    fprintf(stderr, "Mean manufacturing time: %Lf\n", sys.statistics.mean_manufacturing_time);
    fprintf(stderr, "Manufacturing time semi-interval: %Lf\n", sys.statistics.semi_interval_manufacturing_time);
    fprintf(stderr, "CONFIDENCE INTERVAL for MANUFACTURING TIME: [%Lf, %Lf]\n", sys.statistics.mean_manufacturing_time - sys.statistics.semi_interval_manufacturing_time, sys.statistics.mean_manufacturing_time + sys.statistics.semi_interval_manufacturing_time);

    int serv_stat = 1;
    fprintf(stderr, "Mean service time at station %d: %Lf\n", serv_stat, sys.statistics.mean_service_time[serv_stat]);
    fprintf(stderr, "CONFIDENCE INTERVAL for SERVICE TIME at station %d: [%Lf, %Lf]\n", serv_stat, sys.statistics.mean_service_time[serv_stat] - sys.statistics.semi_interval_service_time[serv_stat], sys.statistics.mean_service_time[serv_stat] + sys.statistics.semi_interval_service_time[serv_stat]);
}

void export_interval(FILE *fout, long double mean, long double semi_interval)
{
    fprintf(fout, "%Lf,%Lf,%Lf\n", mean-semi_interval, mean, mean+semi_interval);
}

void print_queue(Queue queue)
{
    int i = 0;
    Node *next = queue.tail;
    if (!next)
        fprintf(stderr, "\t\tEMPTY QUEUE\n");
    while (next)
    {
        //fprintf(stderr, "---- QUEUE: ----\n");
        EventNotice event = next->event;
        fprintf(stderr, "\t\tEvent in position %d:\n", i);
        fprintf(stderr, "\t\t\tEvent type: %d\n", event.type);
        next = next->previous;
        i++;
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n\n");
}

void print_tree(Tree tree)
{
    /* This function is not structured to print a tree. DEPRECATED */
    int i = 0;
    Node *first = tree;
    if (!first)
        fprintf(stderr, "EMPTY FEL\n");
    else
    {
        fprintf(stderr, "---- FEL: ----\n");
        Node *point_to_next_node_in_tree = first;
        while (point_to_next_node_in_tree)
        {
            EventNotice event = point_to_next_node_in_tree->event;
            fprintf(stderr, "Event in position %d:\n", i);
            fprintf(stderr, "\tEvent type: %d\n", event.type);
            fprintf(stderr, "\tEvent time: %Lf\n", event.occur_time);
            fprintf(stderr, "\tEvent in station: %d\n", event.station);
            point_to_next_node_in_tree = point_to_next_node_in_tree->previous;
            i++;
            fprintf(stderr, "\n");
        }
    }
    fprintf(stderr, "\n");
}

void print_fel_v(Node *fel) {
    int i = 0;
    Node *next = fel;
    if (!fel)
        fprintf(stderr, "EMPTY FEL\n");
    else {
        fprintf(stderr, "---- FEL: ----\n");
        while (next) {
            EventNotice event = next->event;
            fprintf(stderr, "Event in position %d:\n", i);
            fprintf(stderr, "\tEvent name: %s\n", event.name);
            fprintf(stderr, "\tEvent type: %d\n", event.type);
            fprintf(stderr, "\tEvent time: %Lf\n", event.occur_time);
            fprintf(stderr, "\tEvent in station: %d\n", event.station);
            next = next->next;
            i++;
        }
    }
}

void print_fel_h(Node *fel)
{
    fprintf(stderr, "------------------------------- FEL -------------------------------\n");

    if (!fel) {  // This should never happen
        fprintf(stderr, "EMPTY FEL\n");
        return;
    }

    char *type[3] = {"ARR", "DEP", "SELF"};  // To print the type from the numerical value

    Node *node;  // Pointer to node which will scan through the nodes in the FEL
    int  i, j, lim;     /* j: goes from 0 to the number of clients
                         * i: goes from 0 to the # of clients in intervals of (max # of events to show per row)
                         * lim: the min[(j + MAX_EV_COL), (N_CLIENTS)]. Used to stop i from surpassing N_CLIENTS */

    /* External cycle: j goes from 0 to the # of clients in jumps of MAX_EV_COL (max number of events per column) */
    for (j = 0; j < N_CLIENTS; j+=MAX_EV_COL)
    {
        lim = ((j + MAX_EV_COL)>N_CLIENTS)? N_CLIENTS : (j + MAX_EV_COL);  // lim assures i<N_CLIENTS and i<(j + MAX_EV_COL)
        i = j;  // for each row, print from where left off

        for (node = fel; node && (i < lim); node = node->next)  // Scan through nodes in FEL in batches of MAX_EV_COL
            fprintf(stderr, "---- EVENT #%d ----\t", i++);  // Prints event number and increments it
        fprintf(stderr, "\n");
        i = j;
        for (node = fel; node && (i < lim); node = node->next){
            fprintf(stderr, "Name: %s\t\t", node->event.name);
            i++;
        }
        fprintf(stderr, "\n");
        i = j;
        for (node = fel; node && (i < lim); node = node->next){
            fprintf(stderr, "Type: %s\t\t", type[node->event.type]);
            i++;
        }
        fprintf(stderr, "\n");
        i = j;
        for (node = fel; node && (i < lim); node = node->next){
            fprintf(stderr, "Stat: %d\t\t\t", node->event.station);
            i++;
        }
        fprintf(stderr, "\n");
        i = j;
        for (node = fel; node && (i < lim); node = node->next){
            fprintf(stderr, "Time: %-17.5Lf\t", node->event.occur_time);
            i++;
        }
        fprintf(stderr, "\n");
    }

}

void print_station(Station *stations, int index)
{
    Station station = stations[index];
    fprintf(stderr, "Station %d:\n", index);
    fprintf(stderr, "\tjobs_in_service: %d\n", station.jobs_in_service);
    fprintf(stderr, "\tjobs_in_queue: %d\n", station.jobs_in_queue);
    fprintf(stderr, "\tQueue:\n");
    print_queue(stations[index].queue);
}

void print_all_stations_v(Station *stations)
{
    int i;
    for (i = 0; i < N_STATIONS; i++)
        print_station(stations, i);
}

void print_all_stations_h(Station *stations)
{
    fprintf(stderr, "----------------------------- STATIONS -----------------------------\n");
    int i, j, lim;
    for (j = 0; j < N_STATIONS; j+=MAX_STAT_COL)
    {
        lim = ((j + MAX_STAT_COL)>N_STATIONS)? N_STATIONS : (j + MAX_STAT_COL);
        i = j;
        for (i = j; i < lim; i++)
            fprintf(stderr, "------- Station %d -------\t", i);

        fprintf(stderr, "\n");
        for (i = j; i < lim; i++)
            fprintf(stderr, "in service:\t%d\t\t", stations[i].jobs_in_service);
        fprintf(stderr, "\n");
        for (i = j; i < lim; i++)
            fprintf(stderr, "in queue:\t%d\t\t", stations[i].jobs_in_queue);
        fprintf(stderr, "\n");
        for (i = j; i < lim; i++)
            fprintf(stderr, "tot arrivals:\t%d\t\t", stations[i].measures.arrivals_n);
        fprintf(stderr, "\n");
        for (i = j; i < lim; i++)
            fprintf(stderr, "tot departures:\t%d\t\t", stations[i].measures.departures_n);
        fprintf(stderr, "\n");
        for (i = j; i < lim; i++)
            fprintf(stderr, "waiting area:\t%-16.5Lf", stations[i].measures.waiting_area);
        fprintf(stderr, "\n");
        fprintf(stderr, "\n");
    }
}

void system_recap(System sys)
{
    fprintf(stderr, "============================================================================\n");
    fprintf(stderr, "========================= Clock = %16.5Lf =========================\n", clock);
    fprintf(stderr, "========================= Event_N %16d =========================\n", sys.event_counter);
    fprintf(stderr, "============================================================================\n");
    fprintf(stderr, "\n");
    print_all_stations_h(sys.stations);
    fprintf(stderr, "\n");
    print_fel_h(sys.fel);
    fprintf(stderr, "\n\n");
}
