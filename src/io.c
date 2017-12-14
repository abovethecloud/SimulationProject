#include "io.h"


void report()
{

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
            fprintf(stderr, "\tEvent time: %lf\n", event.occur_time);
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
            fprintf(stderr, "\tEvent time: %lf\n", event.occur_time);
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

    char *type[4] = {"ARR", "DEP", "END"};
    Node *node;
    int i = 0;
    for (node = fel; node; node = node->next)
        fprintf(stderr, "--EVENT #%d--\t", i++);  // Prints event # and increments it
    fprintf(stderr, "\n");
    for (node = fel; node; node = node->next)
        fprintf(stderr, "Name:\t%s\t", node->event.name);
    fprintf(stderr, "\n");
    for (node = fel; node; node = node->next)
        fprintf(stderr, "Type:\t%s\t", type[node->event.type]);
    fprintf(stderr, "\n");
    for (node = fel; node; node = node->next)
        fprintf(stderr, "Stat:\t%d\t", node->event.station);
    fprintf(stderr, "\n");
    for (node = fel; node; node = node->next)
        fprintf(stderr, "Time:\t%3.3lf\t", node->event.occur_time);
    fprintf(stderr, "\n");
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
    int i;
    for (i = 0; i < N_STATIONS; i++)
        fprintf(stderr, "Station %d\t\t\t\t", i);
    fprintf(stderr, "\n");
    for (i = 0; i < N_STATIONS; i++)
        fprintf(stderr, "\tin service:\t%d\t\t", stations[i].jobs_in_service);
    fprintf(stderr, "\n");
    for (i = 0; i < N_STATIONS; i++)
        fprintf(stderr, "\tin queue:\t%d\t\t", stations[i].jobs_in_queue);
    fprintf(stderr, "\n");
    for (i = 0; i < N_STATIONS; i++)
        fprintf(stderr, "\ttot arrivals:\t%d\t\t", stations[i].arrivals_n);
    fprintf(stderr, "\n");
    for (i = 0; i < N_STATIONS; i++)
        fprintf(stderr, "\ttot departures:\t%d\t\t", stations[i].departures_n);
    fprintf(stderr, "\n");
    for (i = 0; i < N_STATIONS; i++)
        fprintf(stderr, "\tarea_jobs:\t%3.3lf\t", stations[i].statistics.area_jobs);
    fprintf(stderr, "\n");
}

void system_recap(System sys)
{
    fprintf(stderr, "\n\n");
    fprintf(stderr, "====================================================================\n");
    fprintf(stderr, "========================= Clock = %lf =========================\n", clock);
    fprintf(stderr, "====================================================================\n");
    fprintf(stderr, "\n");
    print_all_stations_h(sys.stations);
    fprintf(stderr, "\n");
    print_fel_h(sys.fel);
}
