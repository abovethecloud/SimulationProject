#include "statistics.h"

void compute_statistics(System *sys, Means means)
{
    int i;
    for (i = 0; i < N_STATIONS; i++){
        sys->statistics.mean_number_jobs[i] = means.sum_waiting_area[i]/means.sum_observation_time;

        sys->statistics.mean_waiting_time[i] = means.sum_waiting_area[i]/((double) means.sum_arrivals[i]);

        sys->statistics.mean_throughput[i] = ((double) means.sum_departures[i])/means.sum_observation_time;

        sys->statistics.del_waiting_time[i] =
            sqrt((double) reg_cycle_n/(reg_cycle_n -1)) *
            sqrt(means.squared_sum_waiting_area[i] - 2*(sys->statistics.mean_waiting_time[i])*(means.sum_of_product_waiting_and_arrivals[i]) + (sys->statistics.mean_waiting_time[i])*(sys->statistics.mean_waiting_time[i])*(means.squared_sum_arrivals[i])) /
            (double) means.sum_arrivals[i];

/*
        fprintf(stderr, "coeff: %lf\n", (double) reg_cycle_n/(reg_cycle_n -1));
        fprintf(stderr, "Saa: %lf\n", means.squared_sum_waiting_area[i]);
        fprintf(stderr, "-2 * r * Sav: %lf\n", - 2*(sys->statistics.mean_waiting_time[i])*(means.sum_of_product_waiting_and_arrivals[i]));
        fprintf(stderr, "r^2 * Svv: %lf\n", (sys->statistics.mean_waiting_time[i])*(sys->statistics.mean_waiting_time[i])*(means.squared_sum_arrivals[i]));
        fprintf(stderr, "NUM: %lf\n", means.squared_sum_waiting_area[i] - 2*(sys->statistics.mean_waiting_time[i])*(means.sum_of_product_waiting_and_arrivals[i]) + (sys->statistics.mean_waiting_time[i])*(sys->statistics.mean_waiting_time[i])*(means.squared_sum_arrivals[i]));
        fprintf(stderr, "Sv: %d\n", means.sum_arrivals[i]);
*/
        sys->statistics.semi_interval_waiting_time[i] = 1.96*sys->statistics.del_waiting_time[i];
    }
}

void update_stations_measurements(System *sys, double delta)
{
    /* Arrivals and departures are updated in the functions arrival and departure */
    int i;
    for (i = 0; i < N_STATIONS; i++){
        sys->stations[i].measures.waiting_area += delta*((double) sys->stations[i].jobs_in_service + sys->stations[i].jobs_in_queue);
    }
}

void update_mean_measures(Means *means, Station *stations, int run)
{
    //means->mean_observation_time = (run*means->mean_observation_time + T)/(run+1);
    means->sum_observation_time += T;
    means->squared_sum_observation_time += T*T;

    int i;
    for (i = 0; i < N_STATIONS; i++)
    {
        //means->mean_waiting_area[i] = (run*means->mean_waiting_area[i] + stations[i].measures.waiting_area)/((double)(run+1));
        means->sum_waiting_area[i] += stations[i].measures.waiting_area;
            //fprintf(stderr, "SUM: %40.20lf\n", means->sum_waiting_area[i]);
        means->squared_sum_waiting_area[i] += pow(stations[i].measures.waiting_area, 2);
        //fprintf(stderr, "SQUARED SUM = %40.20lf\n", means->squared_sum_waiting_area[i]);
        //getchar();

        //means->mean_departures[i] = (run*means->mean_departures[i] + stations[i].measures.departures_n)/((double)(run+1));
        means->sum_departures[i] += stations[i].measures.departures_n;
        means->squared_sum_departures[i] += (double) stations[i].measures.departures_n * stations[i].measures.departures_n;

        //means->mean_arrivals[i] = (run*means->mean_arrivals[i] + stations[i].measures.arrivals_n)/((double)(run+1));
        means->sum_arrivals[i] += stations[i].measures.arrivals_n;
        means->squared_sum_arrivals[i] += (double) stations[i].measures.arrivals_n * stations[i].measures.arrivals_n;

        means->sum_of_product_waiting_and_arrivals[i] += stations[i].measures.waiting_area * ((double) stations[i].measures.arrivals_n);
    }

}
