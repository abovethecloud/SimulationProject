#include "statistics.h"

long double t_st = 1.699; // for 90%, 2.04522964 for 95%

void compute_statistics(System *sys, Means means)
{
    int i;
    for (i = 0; i < N_STATIONS; i++){
        sys->statistics.mean_number_jobs[i] = means.sum_waiting_area[i]/means.sum_observation_time;

        sys->statistics.mean_waiting_time[i] = means.sum_waiting_area[i]/((long double) means.sum_departures[i]);

        sys->statistics.mean_throughput[i] = ((long double) means.sum_departures[i])/means.sum_observation_time;

        sys->statistics.del_waiting_time[i] =
            sqrtl((long double) reg_cycle_n/(reg_cycle_n - 1)) *
            sqrtl(means.squared_sum_waiting_area[i] - 2*(sys->statistics.mean_waiting_time[i])*(means.sum_of_product_waiting_and_departures[i]) + (sys->statistics.mean_waiting_time[i])*(sys->statistics.mean_waiting_time[i])*(means.squared_sum_departures[i])) /
            (long double) means.sum_departures[i];

        sys->statistics.semi_interval_waiting_time[i] = t_st*sys->statistics.del_waiting_time[i];
    }
    sys->statistics.mean_cycle_time = cyclemeasures.sum_cycle_time/cyclemeasures.sum_cycle_n;
    sys->statistics.del_cycle_time =
        sqrtl((long double) reg_cycle_n/(reg_cycle_n - 1)) *
        sqrtl(cyclemeasures.squared_sum_cycle_time - 2*(sys->statistics.mean_cycle_time)*(cyclemeasures.sum_product_cycletime_cyclen) + (sys->statistics.mean_cycle_time)*(sys->statistics.mean_cycle_time)*cyclemeasures.squared_sum_cycle_n) /
        (long double) cyclemeasures.sum_cycle_n;
    sys->statistics.semi_interval_cycle_time = t_st*sys->statistics.del_cycle_time;


    sys->statistics.mean_manufacturing_time = cyclemeasures.sum_manufacturing_time/cyclemeasures.sum_manufacturing_n;
    sys->statistics.del_manufacturing_time =
        sqrtl((long double) reg_cycle_n/(reg_cycle_n - 1)) *
        sqrtl(cyclemeasures.squared_sum_manufacturing_time - 2*(sys->statistics.mean_manufacturing_time)*(cyclemeasures.sum_product_manufacturingtime_manufacturingn) + powl(sys->statistics.mean_manufacturing_time, 2.0)*cyclemeasures.squared_sum_manufacturing_n) /
        (long double) cyclemeasures.sum_manufacturing_n;
        sys->statistics.semi_interval_manufacturing_time = t_st * sys->statistics.del_manufacturing_time;
}

void update_stations_measurements(System *sys, long double delta)
{
    /* Arrivals and departures are updated in the functions arrival and departure */
    int i;
    for (i = 0; i < N_STATIONS; i++){
        sys->stations[i].measures.waiting_area += delta*((long double) sys->stations[i].jobs_in_service + sys->stations[i].jobs_in_queue);
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
        //means->mean_waiting_area[i] = (run*means->mean_waiting_area[i] + stations[i].measures.waiting_area)/((long double)(run+1));
        means->sum_waiting_area[i] += stations[i].measures.waiting_area;
        means->squared_sum_waiting_area[i] += powl(stations[i].measures.waiting_area, (long double)2.0);

        //means->mean_departures[i] = (run*means->mean_departures[i] + stations[i].measures.departures_n)/((long double)(run+1));
        means->sum_departures[i] += stations[i].measures.departures_n;
        means->squared_sum_departures[i] += (long double) stations[i].measures.departures_n * stations[i].measures.departures_n;

        //means->mean_arrivals[i] = (run*means->mean_arrivals[i] + stations[i].measures.arrivals_n)/((long double)(run+1));
        means->sum_arrivals[i] += stations[i].measures.arrivals_n;
        means->squared_sum_arrivals[i] += (long double) stations[i].measures.arrivals_n * stations[i].measures.arrivals_n;

        means->sum_of_product_waiting_and_arrivals[i] += stations[i].measures.waiting_area * ((long double) stations[i].measures.arrivals_n);
        means->sum_of_product_waiting_and_departures[i] += stations[i].measures.waiting_area * ((long double) stations[i].measures.departures_n);

    }

    long double temp_cycle_n_sum = cyclemeasures.sum_cycle_n - sqrtl(cyclemeasures.squared_sum_cycle_n);
    cyclemeasures.squared_sum_cycle_n += powl(temp_cycle_n_sum, 2);
    long double temp_manufacturing_n_sum = cyclemeasures.sum_manufacturing_n - sqrtl(cyclemeasures.squared_sum_manufacturing_n);
    cyclemeasures.squared_sum_manufacturing_n += powl(temp_manufacturing_n_sum, 2);

    long double temp_cycle_time_sum = cyclemeasures.sum_cycle_time - sqrtl(cyclemeasures.squared_sum_cycle_time);
    cyclemeasures.squared_sum_cycle_time += powl(temp_cycle_time_sum, 2);
    long double temp_manufacturing_time_sum = cyclemeasures.sum_manufacturing_time - sqrtl(cyclemeasures.squared_sum_manufacturing_time);
    cyclemeasures.squared_sum_manufacturing_time += powl(temp_manufacturing_time_sum, 2);

    cyclemeasures.sum_product_cycletime_cyclen += temp_cycle_time_sum * temp_cycle_n_sum;
    cyclemeasures.sum_product_manufacturingtime_manufacturingn += temp_manufacturing_time_sum * temp_manufacturing_n_sum;
}
