#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* array[9] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

int check4num(char* line) {
    int length = strlen(line);
    if (length < 3)
        return 0;
    
    for (int i = 0; i < 9; i++) {
        if (strncmp(line, array[i], strlen(array[i])) == 0) {
            return i + 1;
        }
    }

    return 0;
}


int line2num(char* line) {
    char str[2] = { 0 };
    int num = 0;
    int length = strlen(line);
    /*
        int length = ((int)(strchr(line, '\n') - line)) - 1;
    
        custom strlen function
        does not work for last line, because of the missing \n
        the string probably ends with \0 though, but not tested 
    */ 
    
    for (int i = 0; i < length; i++) {
        if (line[i] >= '0' && line[i] <= '9') {
            str[0] = line[i];
            printf("[%d] : \"%c\", ", i, line[i]);
            i = length;
        } else {
            int check = check4num(line + i);
            if (check != 0) {
                str[0] = check + '0';
                printf("[%d] : \"%c\", ", i, str[0]);
                i = length;
            }
        }
    }
    
    for (int i = length - 1; i > -1; i--) {
        if (line[i] >= '0' && line[i] <= '9') {
            str[1] = line[i];
            printf("[%d] : \"%c\", ", i, line[i]);
            i = -1;
        } else {
            int check = check4num(line + i);
            if (check != 0) {
                str[1] = check + '0';
                printf("[%d] : \"%c\", ", i, str[1]);
                i = -1;
            }
        }
    }

    num = atoi(str);
    printf("num: %d, ", num);

    return num;
}


void main(int argc, char* argv[])
{
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    int sum = 0;
    int counter = 1;

    fp = fopen(argv[1], "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);



    while (getline(&line, &len, fp) != -1) {
        printf("%d: ", counter);
        int num = line2num(line);
        sum += num;
        printf("sum: %d\n", sum);
        counter++;
    }

    printf("sum: %d\n", sum);
}