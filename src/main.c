#include "globals.h"

#include "simulator.h"
#include "statistics.h"
#include "io.h"


int main(int argc, char const *argv[])
{
    PlantSeeds(123456789);

    int i;
    System *sys_point;

    FILE *cycle = fopen("cycle_interval.csv", "w");
    fprintf(cycle, "INF,MEAN,SUP\n");

    for (i = 0; i < VALIDATE_N; i++){
        sys_point = calloc(1, sizeof(System));
        N_events_stop = N_EVENTS_PER_CYCLE;
        clock = 0.0;
        oldclock = 0.0;
        T = 0.0;
        consumption_time = CONSUMPTION_TIME;
        reg_cycle_n = MIN_REG_N;
        reached_end = 0;
        debug = 0;

        simulate(sys_point);
        report(*sys_point);

        export_interval(cycle, sys_point->statistics.mean_cycle_time, sys_point->statistics.semi_interval_cycle_time);
        free(sys_point);
    }

    fclose(cycle);
    return 0;
}
