#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>


#define SEEDS "seeds"
#define S2S "seed-to-soil"
#define S2F "soil-to-fertilizer"
#define F2W "fertilizer-to-water"
#define W2L "water-to-light"
#define L2T "light-to-temperature"
#define T2H "temperature-to-humidity"
#define H2L "humidity-to-location"


struct transitions {
    int* sizes;
    struct transition** transitions;
};


struct transition {
    uint32_t destination;
    uint32_t source;
    uint32_t length;
};


struct init_seeds {
    uint32_t seed;
    uint32_t length;
};


uint32_t count_seeds(char* line) {
    uint32_t length = 0;
    uint32_t start = 0;
    int s = 0;
    int e = 0;
    char* ptr;
    // FIXME

    uint32_t count = 0;
    for (int i = (strchr(line, ':') - line); i < strlen(line); i++) {
        if (line[i] >= '0' && line[i] <= '9') {
            if (start == 0) {
                start = 1;
                while (line[i] >= '0' && line[i] <= '9')
                    i++;
                count ++;
            } else {
                s = i;
                e = i + 1;
                while (line[e] >= '0' && line[e] <= '9')
                    e++;

                char* num = (char*)malloc(sizeof(char) * (e - s));
                strncpy(num, line + s, e - s);
                length += strtoul(num, &ptr, 10);
                start = 0;
                i = e;
                free(num);
            }
        }
    }
    return count;
}


uint32_t count_init_seeds(struct init_seeds* s, uint32_t c)
{
    uint32_t count = 0;
    for (int i = 0; i < c; i++)
        count += s[i].length;
    
    return count;
}


void set_seeds(char* line, struct init_seeds* seeds) {
    uint32_t count = 0;
    uint32_t start = 0;
    int s = 0;
    int e = 0;
    char* ptr;
    // FIXME

    for (int i = (strchr(line, ':') - line); i < strlen(line); i++) {
        if (line[i] >= '0' && line[i] <= '9') {
            s = i;
            e = i + 1;
            while (line[e] >= '0' && line[e] <= '9')
                e++;

            char* num = (char*)malloc(sizeof(char) * (e - s));
            strncpy(num, line + s, e - s);

            if (seeds[count].seed == UINT32_MAX)
                seeds[count].seed = strtoul(num, &ptr, 10);
            else {
                seeds[count].length = strtoul(num, &ptr, 10);
                count++;
            }

            start = 0;
            i = e;
            free(num);
        }
    }
}


void set_pointer(struct transitions* transitions, struct init_seeds** iseeds, uint32_t* seeds, char* filename)
{
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    uint32_t counter = 0;
    uint32_t dest = 0;


    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while (getline(&line, &len, fp) != -1 /* && counter < 3 */) {
        if (strncmp(line, SEEDS, strlen(SEEDS)) == 0) {
            *seeds = count_seeds(line);
            *iseeds = (struct init_seeds*)malloc(sizeof(struct init_seeds) * (*seeds));
            if (*iseeds == NULL) {
                printf("Error allocating memory\n");
                exit(EXIT_FAILURE);
            }
            for (uint32_t i = 0; i < *seeds; i++)
            {
                (*iseeds)[i].seed = UINT32_MAX;
                (*iseeds)[i].length = UINT32_MAX;
            }
            set_seeds(line, *iseeds);
        } else if (strncmp(line, S2S, strlen(S2S)) == 0) {
            dest = 1;
        } else if (strncmp(line, S2F, strlen(S2F)) == 0) {
            dest = 2;
        } else if (strncmp(line, F2W, strlen(F2W)) == 0) {
            dest = 3;
        } else if (strncmp(line, W2L, strlen(W2L)) == 0) {
            dest = 4;
        } else if (strncmp(line, L2T, strlen(L2T)) == 0) {
            dest = 5;
        } else if (strncmp(line, T2H, strlen(T2H)) == 0) {
            dest = 6;
        } else if (strncmp(line, H2L, strlen(H2L)) == 0) {
            dest = 7;
        } else if (strcmp(line, "\n") != 0) {
            counter++;
        } else {
            if (counter != 0) {
                transitions->transitions[dest - 1] = (struct transition*)malloc(sizeof(struct transition) * counter);
                for (int i = 0; i < counter; i++) {
                    transitions->transitions[dest - 1][i].destination = UINT32_MAX;
                    transitions->transitions[dest - 1][i].source = UINT32_MAX;
                    transitions->transitions[dest - 1][i].length = UINT32_MAX;
                }
                transitions->sizes[dest - 1] = counter;
                // printf("transition %ld set with %ld elements\n", dest, counter);
                counter = 0; 
            }
        }
    }
    transitions->transitions[dest - 1] = (struct transition*)malloc(sizeof(struct transition) * counter);
    for (int i = 0; i < counter; i++) {
        transitions->transitions[dest - 1][i].destination = UINT32_MAX;
        transitions->transitions[dest - 1][i].source = UINT32_MAX;
        transitions->transitions[dest - 1][i].length = UINT32_MAX;
    }
    transitions->sizes[dest - 1] = counter;
    // printf("transition %ld set with %ld elements\n", dest, counter);
    
    fclose(fp);
    if (line)
        free(line);
}


void set_values(struct transitions* transitions, struct init_seeds* iseeds, char* filename)
{
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    uint32_t counter = 0;
    uint32_t dest = 0;


    fp = fopen(filename, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while (getline(&line, &len, fp) != -1 /* && counter < 3 */) {
        if (strncmp(line, S2S, strlen(S2S)) == 0) {
            dest = 1;
        } else if (strncmp(line, S2F, strlen(S2F)) == 0) {
            dest = 2;
        } else if (strncmp(line, F2W, strlen(F2W)) == 0) {
            dest = 3;
        } else if (strncmp(line, W2L, strlen(W2L)) == 0) {
            dest = 4;
        } else if (strncmp(line, L2T, strlen(L2T)) == 0) {
            dest = 5;
        } else if (strncmp(line, T2H, strlen(T2H)) == 0) {
            dest = 6;
        } else if (strncmp(line, H2L, strlen(H2L)) == 0) {
            dest = 7;
        } else if (strcmp(line, "\n") != 0) {
            if (dest != 0) {
                for (int i = 0; i < strlen(line); i++) {
                    if (line[i] >= '0' && line[i] <= '9') {
                        char* ptr;
                        uint32_t start = i;
                        uint32_t end = start + 1;
                        while (line[end] >= '0' && line[end] <= '9')
                            end++;

                        char* num = (char*)malloc(sizeof(char) * (end - start));
                        strncpy(num, line + start, end - start);
                        

                        if (transitions->transitions[dest - 1][counter].destination == UINT32_MAX)
                            transitions->transitions[dest - 1][counter].destination = strtoul(num, &ptr, 10);
                        else if (transitions->transitions[dest - 1][counter].source == UINT32_MAX)
                            transitions->transitions[dest - 1][counter].source = strtoul(num, &ptr, 10);
                        else if (transitions->transitions[dest - 1][counter].length == UINT32_MAX)
                            transitions->transitions[dest - 1][counter].length = strtoul(num, &ptr, 10);

                        free(num);
                        i = end;
                    }
                }
            } 
            counter++;
        } else {
            counter = 0;
        }
    }
    
    fclose(fp);
    if (line)
        free(line);
}


uint32_t x2x(struct transitions* t, uint32_t seed, int source) {
    if (source == 7) 
        return seed;
    uint32_t destination = UINT32_MAX;
    for (int i = 0; i < t->sizes[source]; i++) {
        if (t->transitions[source][i].source <= seed && t->transitions[source][i].source + t->transitions[source][i].length > seed) {
            destination = t->transitions[source][i].destination + seed - t->transitions[source][i].source;
            // printf(" -> %ld", destination);
            return x2x(t, destination, source + 1);
        }        
    }
    // printf(" -> %ld", seed);
    return x2x(t, seed, source + 1);
}


uint32_t compute(struct transitions* transitions, struct init_seeds* iseeds, int seed_count)
{
    int idx = 0;
    uint32_t counter = 0;
    uint32_t lowest_location = UINT32_MAX;

    while (idx < seed_count) {
        printf("seed: %ld\n", iseeds[idx].seed);
        struct init_seeds* current_seeds = &iseeds[idx];
        for (int i = 0; i < current_seeds->length; i++) {
            uint32_t current_location = UINT32_MAX;
            uint32_t current_seed = current_seeds->seed + i;
            // get location of current seed
            // printf("%ld", current_seed);
            current_location = x2x(transitions, current_seed, 0);
            // printf("\n");

            if (current_location < lowest_location)
                lowest_location = current_location;

            counter++;
            uint32_t end = count_init_seeds(iseeds, seed_count);
            if (counter % 10000000 == 0)
                printf("%ld/%ld     (%f\%)\n", counter, end, (float)counter / (float)end * 100.0);
            
        }
        idx++;
    }
    return lowest_location;
}


void print_init_seeds(struct init_seeds* seeds, uint32_t count) {
    printf("initial seeds set:\n");
    for (uint32_t i = 0; i < count; i++) {
        printf("    %ld -> %ld\n", seeds[i].seed, seeds[i].seed + seeds[i].length - 1);
    }
    printf("\n");
}


void print_transition(struct transition* transition) {
    printf("destination: %ld, source: %ld, length: %ld\n", transition->destination, transition->source, transition->length);
}


void main(uint32_t argc, char* argv[])
{
    uint32_t result = UINT32_MAX;
    uint32_t seed_count = 0;
    struct init_seeds* iseeds = NULL;
    struct transitions* transitions = (struct transitions*)malloc(sizeof(struct transitions));
    transitions->sizes = (int*)malloc(sizeof(int) * 7);
    transitions->transitions = (struct transition**)malloc(sizeof(struct transition*) * 7);

    set_pointer(transitions, &iseeds, &seed_count, argv[1]);
    printf("pointer for transitions set:\n");
    for (int i = 0; i < 7; i++) {
        printf("    transition %ld set with %ld elements\n", i + 1, transitions->sizes[i]);
    }
    printf("\n");

    set_values(transitions, iseeds, argv[1]);
    print_init_seeds(iseeds, seed_count);
    print_transition(&transitions->transitions[6][1]);

    result = compute(transitions, iseeds, seed_count);
    printf("result: %ld\n", result);


    free(transitions->sizes);
    for (int i = 0; i < 7; i++)
        free(transitions->transitions[i]);
    free(transitions);
    
    if (iseeds)
        free(iseeds);
    exit(EXIT_SUCCESS);
}
