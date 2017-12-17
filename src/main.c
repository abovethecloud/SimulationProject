#include "globals.h"

#include "simulator.h"
#include "statistics.h"
#include "io.h"


int main(int argc, char const *argv[])
{
    PlantSeeds(1);

    System sys;
    simulate(&sys);

    report();

    return 0;
}
