#ifndef STATISTICS_H
#define STATISTICS_H

#include "globals.h"

void compute_statistics(System *sys);

void update_stations_measurements(System *sys, double delta);

void update_mean_measures(Means *means, Station *stations, int run);

#endif
