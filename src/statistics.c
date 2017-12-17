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

void update_mean_measures(Means *means, Station *stations, int run)
{
    int i;
    for (i = 0; i < N_STATIONS; i++)
    {
    means->mean_observation_time = (run*means->mean_observation_time + T)/(run+1);
    means->mean_waiting_area[i] = (run*means->mean_waiting_area[i] + stations[i].measures.waiting_area)/(run+1);
    means->mean_departures[i] = (run*means->mean_departures[i] + stations[i].measures.departures_n)/(run+1);
    means->mean_arrivals[i] = (run*means->mean_arrivals[i] + stations[i].measures.arrivals_n)/(run+1);
    }

}
