#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int compute_card(char* card)
{
    char* start_win = strchr(card, ':') + 1;
    char* end_win = strchr(card, '|');
    char* start_my = end_win + 1;
    char* end_my = card + strlen(card) - 1;
    int count = 0;

    char* win = malloc(sizeof(char) * (end_win - start_win));
    strncpy(win, start_win, end_win - start_win);

    char* my = malloc(sizeof(char) * (end_my - start_my));
    strncpy(my, start_my, end_my - start_my);

    char* num = NULL;
    for (int i = 0; i < strlen(my); i++) {
        if (my[i] >= '0' && my[i] <= '9') {
            int start = i;
            char* number = NULL;
            while (my[i] >= '0' && my[i] <= '9') {
                i++;
            }
            number = malloc(sizeof(char) * (i - start + 2));
            strncpy(number + 1, my + start, i - start);
            number[0] = ' ';
            number[strlen(number)] = ' ';
            // printf("number: \"%s\"\n", number);
            
            if (strstr(win, number) != NULL) {
                count++;
                // printf("number in win: %s\n", number);
            }
        }
    }
    // printf("count: %d\n", count);
    return count;
}


void main(int argc, char* argv[])
{
    FILE* fp;
    int sum = 0;
    int counter = 1;
    char* card = NULL;
    size_t len = 0;
    int ret = 0;

    int size = 203;
    int* list = calloc(size, sizeof(int));

    fp = fopen(argv[1], "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    for (int i = 0; i < size; i++)
    {
        list[i] = 1;
    }
    

    while (getline(&card, &len, fp) != -1 /* && counter < 2 */) {
        ret = compute_card(card);
        // printf("%d: ret: %d\n", counter, ret);
        for (int i = 0; i < ret; i++) {
            list[counter + i] += list[counter - 1];
        }
        
        
        counter++;
    }

    for (int i = 0; i < size; i++) {
        sum += list[i];
    }
    printf("%d\n", sum);
}