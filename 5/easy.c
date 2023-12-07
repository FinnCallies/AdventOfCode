#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>


#define SEEDS "seeds"
#define S2S "seed-to-soil"
#define S2F "soil-to-fertilizer"
#define F2W "fertilizer-to-water"
#define W2L "water-to-light"
#define L2T "light-to-temperature"
#define T2H "temperature-to-humidity"
#define H2L "humidity-to-location"


struct transitions {
    uint32_t seed;
    uint32_t soil;
    uint32_t fertilizer;
    uint32_t water;
    uint32_t light;
    uint32_t temperature;
    uint32_t humidity;
    uint32_t location;
};


uint32_t count_seeds(char* line) {
    uint32_t count = 0;
    for (uint32_t i = (strchr(line, ':') - line); i < strlen(line); i++) {
        if (line[i] >= '0' && line[i] <= '9') {
            count++;
            i = strchr(line + i, ' ') - line;
        }
    }
    return count;
}


void set_seeds(char* line, struct transitions* plants) {
    uint32_t counter = 0;
    for (uint32_t i = (strchr(line, ':') - line); i < strlen(line); i++) {
        if (line[i] >= '0' && line[i] <= '9') {
            uint32_t start = i;
            uint32_t end = start + 1;
            while (line[end] >= '0' && line[end] <= '9')
                end++;
            

            char* seed = (char*)malloc(sizeof(char) * (end - start));
            strncpy(seed, line + start, end - start);
            plants[counter].seed = atoi(seed);

            free(seed);
            counter++;
            i = end;
        }
    }
}


void parse_x2x(char* line, uint32_t* src, uint32_t* dst, uint32_t* length) 
{
    for (uint32_t i = 0; i < strlen(line); i++) {
        if (line[i] >= '0' && line[i] <= '9') {
            uint32_t start = i;
            uint32_t end = start + 1;
            while (line[end] >= '0' && line[end] <= '9')
                end++;

            char* num = (char*)malloc(sizeof(char) * (end - start));
            strncpy(num, line + start, end - start);
            
            if (strcmp(num, "0") == 0)
            {
                printf("num: %d\n", atoi(num));
            }
            

            if (*dst == UINT32_MAX)
                *dst = atoi(num);
            else if (*src == UINT32_MAX)
                *src = atoi(num);
            else if (*length == UINT32_MAX)
                *length = atoi(num);

            free(num);
            i = end;
        }
    }
    
}


void x2x(char* line, struct transitions* plants, uint32_t count, uint32_t x) {
    uint32_t src_start = UINT32_MAX;
    uint32_t dst_start = UINT32_MAX;
    uint32_t length = UINT32_MAX;

    parse_x2x(line, &src_start, &dst_start, &length);
    printf("x2x: dest: %d, src: %d, length: %d\n", dst_start, src_start, length);
    

    for (uint32_t i = 0; i < count; i++) {
        uint32_t current;

        switch (x) {
            case 1:
                current = plants[i].seed;
                break;
            case 2:
                current = plants[i].soil;
                break;
            case 3:
                current = plants[i].fertilizer;
                break;
            case 4:
                current = plants[i].water;
                break;
            case 5:
                current = plants[i].light;
                break;
            case 6:
                current = plants[i].temperature;
                break;
            case 7:
                current = plants[i].humidity;
                break;
            case 8:
                current = plants[i].location;
                break;
        }

        if (current >= src_start && current < src_start + length) {
            switch (x) {
                case 1:
                    plants[i].soil = dst_start + (current - src_start);
                    break;
                case 2:
                    plants[i].fertilizer = dst_start + (current - src_start);
                    break;
                case 3:
                    plants[i].water = dst_start + (current - src_start);
                    break;
                case 4:
                    plants[i].light = dst_start + (current - src_start);
                    break;
                case 5:
                    plants[i].temperature = dst_start + (current - src_start);
                    break;
                case 6:
                    plants[i].humidity = dst_start + (current - src_start);
                    break;
                case 7:
                    plants[i].location = dst_start + (current - src_start);
                    break;
            }
        }
    }
    
}


uint32_t get_lowest_location(struct transitions* plants, uint32_t count) {
    uint32_t lowest = plants[0].location;
    for (uint32_t i = 1; i < count; i++) {
        if (plants[i].location < lowest)
            lowest = plants[i].location;
    }
    return lowest;
}


void pruint32_t_transition(struct transitions* plants, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        printf("%d %d %d %d %d %d %d %d\n", plants[i].seed, plants[i].soil, plants[i].fertilizer, plants[i].water, plants[i].light, plants[i].temperature, plants[i].humidity, plants[i].location);
    }
    printf("\n");
}


void fill(struct transitions* trns, uint32_t count, uint32_t dest) {
    for (uint32_t i = 0; i < count; i++) {
        switch (dest)
        {
        case 1:
            if (trns[i].soil == UINT32_MAX)
                trns[i].soil = trns[i].seed;
            break;
        case 2:
            if (trns[i].fertilizer == UINT32_MAX)
                trns[i].fertilizer = trns[i].soil;
            break;
        case 3:
            if (trns[i].water == UINT32_MAX)
                trns[i].water = trns[i].fertilizer;
            break;
        case 4:
            if (trns[i].light == UINT32_MAX)
                trns[i].light = trns[i].water;
            break;
        case 5:
            if (trns[i].temperature == UINT32_MAX)
                trns[i].temperature = trns[i].light;
            break;
        case 6:
            if (trns[i].humidity == UINT32_MAX)
                trns[i].humidity = trns[i].temperature;
            break;
        case 7:
            if (trns[i].location == UINT32_MAX)
                trns[i].location = trns[i].humidity;
            break;
        
        default:
            break;
        }
    }
}


void main(uint32_t argc, char* argv[])
{
    FILE* fp;
    uint32_t sum = 0;
    uint32_t counter = 1;
    char* line = NULL;
    size_t len = 0;
    struct transitions* trns = NULL;
    uint32_t dest = 0;
    uint32_t seed_count = 0;

    fp = fopen(argv[1], "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while (getline(&line, &len, fp) != UINT32_MAX && counter < 3) {
        if (strncmp(line, SEEDS, strlen(SEEDS)) == 0) {
            seed_count = count_seeds(line);
            printf("%d\n", seed_count);
            trns = (struct transitions*)malloc(sizeof(struct transitions) * seed_count);
            if (trns == NULL) {
                printf("Error allocating memory\n");
                exit(EXIT_FAILURE);
            }
            for (uint32_t i = 0; i < seed_count; i++)
            {
                trns[i].seed = UINT32_MAX;
                trns[i].soil = UINT32_MAX;
                trns[i].fertilizer = UINT32_MAX;
                trns[i].water = UINT32_MAX;
                trns[i].light = UINT32_MAX;
                trns[i].temperature = UINT32_MAX;
                trns[i].humidity = UINT32_MAX;
                trns[i].location = UINT32_MAX;

            }
            // pruint32_t_transition(trns, seed_count);
            
            
            set_seeds(line, trns);

            // pruint32_t_transition(trns, seed_count);

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
            x2x(line, trns, seed_count, dest);
            pruint32_t_transition(trns, seed_count);
        } else {
            fill(trns, seed_count, dest);
            pruint32_t_transition(trns, seed_count);        
        }
        counter++;
    }
    fill(trns, seed_count, 7);
    pruint32_t_transition(trns, seed_count);

    printf("    => %d\n\n", get_lowest_location(trns, seed_count));
    


    fclose(fp);
    if (line)
        free(line);
    if (trns)
        free(trns);
    EXIT_SUCCESS;
}