#include "globals.h"

#include "simulator.h"
#include "statistics.h"
#include "io.h"


int main(int argc, char const *argv[])
{
    PlantSeeds(1234567890);

    int i;
    System sys;

    FILE *cycle = fopen("cycle_interval.csv", "w");
    fprintf(cycle, "INF,MEAN,SUP\n");

    for (i = 0; i < VALIDATE_N; i++){
        simulate(&sys);
        report(sys);

        export_interval(cycle, sys.statistics.mean_cycle_time, sys.statistics.semi_interval_cycle_time);
    }

    fclose(cycle);
    return 0;
}
