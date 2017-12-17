#include "statistics.h"

void compute_statistics(System *sys)
{
    sys->stations[0].statistics.mean_number_jobs = sys->stations[0].statistics.area_jobs / (clock);
    sys->stations[1].statistics.mean_number_jobs = sys->stations[1].statistics.area_jobs / (clock);

    sys->stations[1].statistics.mean_waiting_time = sys->stations[1].statistics.waiting_area / ((double) sys->stations[1].arrivals_n);
}

void update_statistics(System *sys, double delta)
{
    sys->stations[0].statistics.area_jobs += delta*(sys->stations[0].jobs_in_service + sys->stations[0].jobs_in_queue);
    sys->stations[1].statistics.area_jobs += delta*(sys->stations[1].jobs_in_service + sys->stations[1].jobs_in_queue);
    sys->stations[1].statistics.waiting_area += delta*(sys->stations[1].jobs_in_service + sys->stations[1].jobs_in_queue);
}
