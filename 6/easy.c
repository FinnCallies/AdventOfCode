#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>


struct race {
    int time;
    int distance;
};


int count_races(char* filename)
{
    FILE* file;
    char* line = NULL;
    size_t len = 0;
    int count = 0;


    file = fopen(filename, "r");
    if (file == NULL) 
        exit(EXIT_FAILURE);

    getline(&line, &len, file);
    for (int i = (strchr(line, ':') - line); i < strlen(line); i++) {
        int s = 0;
        int e = 0;
        if (line[i] >= '0' && line[i] <= '9') {
            while (line[i] >= '0' && line[i] <= '9') 
                i++;
            count++;
        }
    }

    free(line);
    fclose(file);
    return count;
}


void set_races(struct race* r, int c, char* filename)
{
    FILE* file;
    char* line = NULL;
    size_t len = 0;
    int count = 0;

    file = fopen(filename, "r");
    if (file == NULL) 
        exit(EXIT_FAILURE);

    while (getline(&line, &len, file) != -1) {
        for (int i = (strchr(line, ':') - line); i < strlen(line); i++) {
            if (line[i] >= '0' && line[i] <= '9') {
                int s = i;
                int e = i + 1;
                char* tmp = NULL;
                int num;
                while (line[e] >= '0' && line[e] <= '9') 
                    e++;
                tmp = (char*)malloc(sizeof(char) * (e - s));
                strncpy(tmp, line + s, e - s);
                num = atoi(tmp);

                if (r[count].time == 0) 
                    r[count].time = num;
                else 
                    r[count].distance = num;

                i = e;
                count++;
                free(tmp);
            }
        }
        count = 0;
    }
    

    free(line);
    fclose(file);
}


void print_races(struct race* r, int c)
{
    for (int i = 0; i < c; i++) 
        printf("t: %d, d: %d\n", r[i].time, r[i].distance);
    printf("\n");
}


int compute(struct race* r, int c)
{
    int ret = 1;

    for (int i = 0; i < c; i++) {
        struct race race = r[i];
        int result = 0;
        int count = 0;

        for (int i = 1; i < race.distance; i++) {
            int d = (race.time - i) * i;
            if (d > race.distance)
                count++;
        }
        ret *= count;
    }

    return ret;
}


void main(int argc, char* argv[])
{
    struct race* races = NULL;
    int count_r = 0;


    count_r = count_races(argv[1]);
    races = (struct race*)malloc(sizeof(struct race*) * count_r);
    for (int i = 0; i < count_r; i++) {
        races[i].time = 0;
        races[i].distance = 0;
    }
    
    set_races(races, count_r, argv[1]);
    print_races(races, count_r);

    printf("%d\n", compute(races, count_r));
}