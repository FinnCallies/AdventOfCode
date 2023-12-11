#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>


struct race {
    uint32_t time;
    uint64_t distance;
};


void main(int argc, char* argv[])
{
    struct race race;
    int result = 0;
    uint32_t count = 0;
    uint64_t start = 0;
    uint64_t end = 0;


    // race.time = 71530;
    // race.distance = 940200;
    race.time = 54708275;
    race.distance = 239114212951253;

    printf("t: %u, d: %ld\n", race.time, race.distance);
    printf("\n");

    for (uint64_t i = 1; i < race.time && start == 0; i++) {
        uint64_t d = (race.time - i) * i;
        if (d > race.distance) 
            start = i;
    }
    printf("start: %ld\n", start);
    for (uint64_t i = race.time; i > start && end == 0; i--) {
        uint64_t d = (race.time - i) * i;
        if (d > race.distance) 
            end = i;
    }
    printf("end: %ld\n", end);
    

    printf("\nresult: %ld\n", end - start + 1);
}