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

void print_station(Station *stations, int index)
{
    Station station = stations[index];
    fprintf(stderr, "STATION %d:\n", index);
    fprintf(stderr, "\tjobs_in_service: %d\n", station.jobs_in_service);
    fprintf(stderr, "\tjobs_in_queue: %d\n", station.jobs_in_queue);
    fprintf(stderr, "\tQueue:\n");
    print_queue(stations[index].queue);
}

void print_all_stations(Station *stations)
{
    int i;
    for (i = 0; i < N_STATIONS; i++)
    {
        print_station(stations, i);
    }
}
