#include "statistics.h"

void compute_statistics(System *sys)
{
    int i;
    for (i = 0; i < N_STATIONS; i++){
        sys->statistics.mean_number_jobs[i] = sys->stations[i].measures.waiting_area/T;
        sys->statistics.mean_waiting_time[i] = sys->stations[i].measures.waiting_area/((double) sys->stations[i].measures.arrivals_n);
        sys->statistics.throughput[i] = ((double) sys->stations[i].measures.departures_n)/T;
    }
}

void update_stations_measurements(System *sys, double delta)
{
    int i;
    for (i = 0; i < N_STATIONS; i++){
        sys->stations[i].measures.waiting_area += delta*(sys->stations[i].jobs_in_service + sys->stations[i].jobs_in_queue);
    }
}
