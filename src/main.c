#include "globals.h"

#include "simulator.h"
#include "statistics.h"
#include "io.h"


int main(int argc, char const *argv[])
{
    PlantSeeds(123456789);

    int i;
    System sys;

    for (i = 0; i < VALIDATE_N; i++){
        simulate(&sys);
        report(sys);
    }

    return 0;
}
