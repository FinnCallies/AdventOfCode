#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* chars = "0123456789";


int getnum(char* line, int idx, int* n)
{
    int start = 0;
    int end = 0;
    while (line[idx] >= '0' && line[idx] <= '9' && idx > 0) {
        idx--;
    }
    if (line[idx] >= '0' && line[idx] <= '9') {
        start = idx;
    } else {
        start = idx + 1;
    }

    idx = start;
    while (line[idx] >= '0' && line[idx] <= '9' && idx < strlen(line)) {
        idx++;
    }
    if (line[idx] >= '0' && line[idx] <= '9') {
        end = idx;
    } else {
        end = idx - 1;
    }
    
    char* num = NULL;
    num = calloc(sizeof(char), end - start + 1);
    strncpy(num, line + start, end - start + 1);

    *n = atoi(num);

    // printf("start = %d(%c), end = %d(%c), num = %s, ", start, line[start], end, line[end], num);
    

    return end;
}


void check_line(char* line, int* nums, int idx)
{
    int length = strlen(line);
    if (line == NULL)
        return;

    printf("line = %c%c%c, ", line[idx - 1], line[idx], line[idx + 1]);

    for (int i = (idx - 1); i < (idx + 2) && i < length; i++) {
        if (line[i] >= '0' && line[i] <= '9') {
            printf("i = %c, ", line[i]);
            int temp = 0;
            i = getnum(line, i, &temp);
            printf("temp = %d", temp);
            if (nums[0] == 0) {
                nums[0] = temp;
            } else {
                nums[1] = temp;
            }
        }
    }
    printf("\n");
    return;
}


int compute_line(char* previous, char* current, char* next)
{
    int sum = 0;
    int length = strlen(current);
    
    for (int i = 0; i < length; i++) {
        if (current[i] == '*') {
            int prod = 0;
            int count = 0;
            int nums[6] = { 0, 0, 0, 0, 0, 0 };
            int idx = 0;

            check_line(current, &nums[idx], i);
            while (nums[idx] != 0) {
                idx++;
            }

            check_line(previous, &nums[idx], i);
            while (nums[idx] != 0) {
                idx++;
            }

            check_line(next, &nums[idx], i);
            while (nums[idx] != 0) {
                idx++;
            }

            // printf("sum2 = %d, count = %d\n", sum2, count);
            for (int i = 0; i < idx; i++)
            {
                printf("+%d ", nums[i]);
            }
            
            if (idx != 2) {
                prod = 0;
            } else {
                printf("%d * %d = ", nums[0], nums[1]);
                prod = nums[0] * nums[1];
                printf("%d\n", prod);
            }
            sum += prod;
            printf("\n");
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
    while (current_line != NULL/*  && counter < 16 */) {
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