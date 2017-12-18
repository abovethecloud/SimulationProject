#include "statistics.h"

void compute_statistics(System *sys, Means means)
{
    int i;
    for (i = 0; i < N_STATIONS; i++){
        sys->statistics.mean_number_jobs[i] = means.sum_waiting_area[i]/means.sum_observation_time;
        sys->statistics.mean_squared_number_jobs[i] = means.squared_sum_waiting_area[i]/means.sum_observation_time;

        sys->statistics.mean_waiting_time[i] = means.sum_waiting_area[i]/((double) means.sum_arrivals[i]);
        sys->statistics.mean_squared_waiting_time[i] = means.squared_sum_waiting_area[i]/((double) means.sum_arrivals[i]);

        sys->statistics.mean_throughput[i] = ((double) means.sum_departures[i])/means.sum_observation_time;
        sys->statistics.mean_squared_throughput[i] = ((double) means.squared_sum_departures[i])/means.sum_observation_time;
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
    means->mean_observation_time = (run*means->mean_observation_time + T)/(run+1);
    means->sum_observation_time += T;
    means->squared_sum_observation_time += T*T;

    int i;
    for (i = 0; i < N_STATIONS; i++)
    {
        means->mean_waiting_area[i] = (run*means->mean_waiting_area[i] + stations[i].measures.waiting_area)/((double)(run+1));
        means->sum_waiting_area[i] += stations[i].measures.waiting_area;
        means->squared_sum_waiting_area[i] += stations[i].measures.waiting_area * stations[i].measures.waiting_area;

        means->mean_departures[i] = (run*means->mean_departures[i] + stations[i].measures.departures_n)/((double)(run+1));
        means->sum_departures[i] += stations[i].measures.departures_n;
        means->squared_sum_departures[i] += stations[i].measures.departures_n * stations[i].measures.departures_n;

        means->mean_arrivals[i] = (run*means->mean_arrivals[i] + stations[i].measures.arrivals_n)/((double)(run+1));
        means->sum_arrivals[i] += stations[i].measures.arrivals_n;
        means->squared_sum_arrivals[i] += stations[i].measures.arrivals_n * stations[i].measures.arrivals_n;
    }

}
