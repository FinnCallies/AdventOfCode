#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int max_red = 12;
int max_green = 13;
int max_blue = 14;


void parseGame(char* game, int* red, int* green, int* blue)
{
    int len = strlen(game);
    while (game != NULL)
    {
        char* next = strchr(game, ',');
        if (next != NULL) {
            *next = '\0';
        }

        char* value = strtok(game, " ");
        char* color = strtok(NULL, " ");

        // printf("color: \"%s\", value: \"%s\" |", color, value);

        if (strcmp(color, "red") == 0) {
            *red = atoi(value);
        }
        else if (strcmp(color, "green") == 0) {
            *green = atoi(value);
        }
        else if (strcmp(color, "blue") == 0) {
            *blue = atoi(value);
        }

        if (next != NULL) {
            game = next + 2;
        } else {
            game = NULL;
        }
    }
    
}


int isPossible(char* line)
{
    int highest_red = 0;
    int highest_green = 0;
    int highest_blue = 0;
    int len = strlen(line);
    if (line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }    



    char* game = strchr(line, ':') + 2;;
    while (game != NULL) {
        int red = 0;
        int green = 0;
        int blue = 0;
        char* next = strchr(game, ';');
        if (next != NULL) {
            *next = '\0';
        }

        // printf("game: \"%s\" => ", game);


        char* gameCPY = malloc(strlen(game));
        strcpy(gameCPY, game);
        parseGame(gameCPY, &red, &green, &blue);
        free(gameCPY);

        // printf("red: %d, green: %d, blue: %d", red, green, blue);

        if (red > highest_red) {
            highest_red = red;
        }
        if (green > highest_green) {
            highest_green = green;
        }
        if (blue > highest_blue) {
            highest_blue = blue;
        }        
        


        if (next != NULL) {
            game = next + 2;
        } else {
            game = NULL;
        }
    }

    return highest_red * highest_green * highest_blue;
}


void main(int argc, char* argv[])
{
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    int counter = 1;
    int sum = 0;

    fp = fopen(argv[1], "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);



    while (getline(&line, &len, fp) != -1) {
        // printf("line: %s", line);
        int ret = isPossible(line);
        printf("Game: %d = %d\n", counter, ret);
        sum += ret;

        counter++;
    }
    printf("sum: %d\n", sum);
}