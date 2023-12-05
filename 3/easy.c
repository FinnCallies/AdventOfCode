#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* chars = "0123456789.\n\0";


int check_line(char* line, int s, int e)
{
    if (line == NULL)
        return 0;

    int length = strlen(line);
    for (int i = s; i < length && i < e; i++) {
        int count = 0;
        for (int j = 0; j < strlen(chars); j++) {
            if (line[i] == chars[j]) {
                count++;
                j = strlen(chars);
            }
        }
        if (count < 1)
            return 1;
    }
    return 0;
}


int compute_line(char* previous, char* current, char* next)
{
    int sum = 0;
    int length = strlen(current);
    
    for (int i = 0; i < length; i++) {
        if (current[i] >= '0' && current[i] <= '9') {
            char* num = NULL;
            int start = i;
            while (current[i] >= '0' && current[i] <= '9') {
                i++;
            }
            int end = i;

            num = calloc(sizeof(char), end - start + 1);
            strncpy(num, current + start, end - start);
            printf("start: %d, end: %d, num: %s", start, end, num);

            if (start > 0)
                start--;
            
            if (end < strlen(current))
                end++;
            
            if (check_line(current, start, end) == 1 || check_line(previous, start, end) == 1 || check_line(next, start, end) == 1) {
                sum += atoi(num);
                printf(" => sum = %d\n", sum);
            } else {
                // printf("%s, ", num);
                printf("\n");
            }
            


            free(num);
        }
    }
    

    return sum;
}


void main(int argc, char* argv[])
{
    FILE* fp;
    char* previous_line = NULL;
    char* current_line = NULL;
    char* next_line = NULL;
    size_t len = 0;
    int sum = 0;
    int counter = 1;

    fp = fopen(argv[1], "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);


    if (getline(&current_line, &len, fp) == -1) {
        printf("error: file is empty\n");
        exit(EXIT_FAILURE);
    }
    while (current_line != NULL /* && counter < 39 */) {
        if (getline(&next_line, &len, fp) == -1) {
            next_line = NULL;
        }

        // printf("%d: ", counter);
        sum += compute_line(previous_line, current_line, next_line);
        // printf("\n");

        if (previous_line != NULL)
            free(previous_line);
        previous_line = malloc(sizeof(char) * strlen(current_line));
        strcpy(previous_line, current_line);
        
        if (current_line != NULL)
            free(current_line);
        if (next_line != NULL) {
            current_line = malloc(sizeof(char) * strlen(next_line));
            strcpy(current_line, next_line);
        } else {
            current_line = NULL;
        }

        printf("%d: sum = %d\n", counter, sum);

        counter++;
    }
    fclose(fp);
    free(previous_line);
    free(current_line);

    printf("\n=> sum = %d\n", sum);
}