#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define HAND_SIZE 5

#define T 10
#define J 1
#define Q 12
#define K 13
#define A 14

#define HIGH_CARD 0
#define PAIR 1
#define TWO_PAIR 2
#define THREE_OF_A_KIND 3
#define FULL_HOUSE 4
#define FOUR_OF_A_KIND 5
#define FIVE_OF_A_KIND 6


struct hand {
    int* cards;
    int bid;
    int type;
    int rank;
    int* joker;
};


int count_hands(char* filename)
{
    FILE* file;
    char* line = NULL;
    size_t len = 0;
    int count = 0;


    file = fopen(filename, "r");
    if (file == NULL) 
        exit(EXIT_FAILURE);

    while (getline(&line, &len, file) != -1) {
        count++;
    }
    

    free(line);
    fclose(file);
    return count;
}


void load_input(char* filename, struct hand* hands)
{
    FILE* file;
    char* line = NULL;
    size_t len = 0;
    int count = 0;


    file = fopen(filename, "r");
    if (file == NULL) 
        exit(EXIT_FAILURE);

    while (getline(&line, &len, file) != -1) {
        int idx = 0;
        for (int i = 0; i < strlen(line) && line[i] != ' '; i++) {
            if (line[i] >= '2' && line[i] <= '9') 
                hands[count].cards[i] = line[i] - '0';
            else {
                switch (line[i])
                {
                case 'T':
                    hands[count].cards[i] = T;
                    break;
                case 'J':
                    hands[count].cards[i] = J;
                    break;
                case 'Q':
                    hands[count].cards[i] = Q;
                    break;
                case 'K':
                    hands[count].cards[i] = K;
                    break;
                case 'A':
                    hands[count].cards[i] = A;
                    break;
                
                default:
                    break;
                }
            }
        }
        for (int i = (strchr(line, ' ') - line); i < strlen(line); i++) {
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
                hands[count].bid = num;

                i = e;
                free(tmp);
            }
        }
        count++;
    }
    

    free(line);
    fclose(file);
}


int get_types(struct hand h)
{
    int ret = 0;
    int* sorted = (int*)malloc(sizeof(int) * HAND_SIZE);
    int* counts = NULL;
    
    for (int i = 0; i < HAND_SIZE; i++) {
        sorted[i] = 0;
        int card = h.cards[i];
        int j = 0;
        while (sorted[j] < card && sorted[j] != 0) 
            j++;
        for (int i = HAND_SIZE - 1; i > j && i > 0; i--) {
            sorted[i] = sorted[i - 1];
        }
        sorted[j] = card;        
    }
    
    // printf("sorted: ");
    // for (int i = 0; i < HAND_SIZE; i++) 
        // printf("%d ", sorted[i]);
    
   
    int temp = 0;
    int counter = 0;
    for (int i = 0; i < HAND_SIZE; i++) {
        if (sorted[i] != temp) {
            temp = sorted[i];
            counter++;
        }
    }
    // printf("; counter: %d", counter);


    if (counter == 5 || counter == 4 || counter == 1)
    {
        // printf("\n");
        switch (counter)
        {
        case 5:
            ret = HIGH_CARD;
            goto out;
            break;
        case 4:
            ret = PAIR;
            goto out;
            break;
        case 1:
            ret = FIVE_OF_A_KIND;
            goto out;
            break;
        
        default:
            break;
        }
    } else if (counter == 2 || counter == 3) {
        counts = (int*)malloc(sizeof(int) * counter);
        for (int i = 0; i < counter; i++) 
            counts[i] = 0;
        counter = 0;
        temp = sorted[0];
        counts[counter]++;
        for (int i = 1; i < HAND_SIZE; i++) {
            if (sorted[i] != temp) {
                counter++;
                counts[counter]++;
                temp = sorted[i];
            } else {
                counts[counter]++;
            }
        }
        counter++;
        
        // printf("; counts: ");
        // for (int i = 0; i < counter; i++)
            // printf("%d ", counts[i]);
        // printf("\n");
        
        

        if (counter == 3) {
            for (int i = 0; i < counter; i++) {
                if (counts[i] == 3) {
                    ret = THREE_OF_A_KIND;
                    goto out;
                    break;
                } else if (counts[i] == 2) {
                    ret = TWO_PAIR;
                    goto out;
                    break;
                }
            }
        } else if (counter == 2) {
            for (int i = 0; i < counter; i++) {
                if (counts[i] == 4) {
                    ret = FOUR_OF_A_KIND;
                    goto out;
                    break;
                } else if (counts[i] == 3) {
                    ret = FULL_HOUSE;
                    goto out;
                    break;
                }
            }
        }
        
    }
    

    /*
    counter == 5 -> high card
    counter == 4 -> pair
    counter == 3 -> two pair || three of a kind
    counter == 2 -> full house || four of a kind
    counter == 1 -> five of a kind
    */
    
out:    
    free(sorted);
    free(counts);
    return ret;
}


// returns 1 if h1 > h2, -1 if h1 < h2, 0 if h1 == h2
int compare(struct hand h1, struct hand h2)
{
    for (int i = 0; i < HAND_SIZE; i++) {
        if (h1.cards[i] > h2.cards[i])
            return 1;
        else if (h1.cards[i] < h2.cards[i])
            return -1;
    }
    return 0;
}


void get_ranks(struct hand* h, int count)
{
    int type = HIGH_CARD;
    int idx = -1;
    int counter = 0;

    while (type <= FIVE_OF_A_KIND) {
        int remaining = 0;
        for (int i = 0; i < count; i++) {
            if (h[i].type == type && h[i].rank == -1) {
                remaining++;
                if (idx == -1) {
                    idx = i;
                } else if (compare(h[idx], h[i]) == 1) {
                    idx = i;
                }                 
            }
        }
        
        if (idx != -1) {
            counter++;
            h[idx].rank = counter;
            remaining--;
            idx = -1;
        }
        if (remaining < 1) 
            type++;
    }    
}


int x_in_ints(int* x, int c, int y)
{
    for (int i = 0; i < c; i++) {
        if (x[i] == y) 
            return 1;
    }
    return 0;
}


void set_joker(struct hand *h) 
{
    int count = 0;
    int count_non_joker = 0;
    int* joker = NULL;
    int* non_joker = NULL;
    int idx;
    int combinations = 0;
    
    for (int i = 0; i < HAND_SIZE; i++) 
        h->cards[i] == J ? count++ : count_non_joker++;
    

    
    if (count == 0)
        return;
    
    
    h->joker = (int*)malloc(sizeof(int) * count);
    for (int i = 0; i < count; i++) {
        h->joker[i] = 0;
    }


    if (count == 5) {
        for (int i = 0; i < count; i++) {
            h->joker[i] = A;
        }
    }


    non_joker = (int*)malloc(sizeof(int) * (count_non_joker));
    idx = 0;
    for (int i = 0; i < count_non_joker; i++) {
        if (h->cards[i] != J && x_in_ints(non_joker, count_non_joker, h->cards[i]) == 0) {
            non_joker[idx] = h->cards[i];
            idx++;
        }
    }


    if (idx == 1) {
        for (int i = 0; i < count; i++) {
            h->joker[i] = non_joker[0];
        }
    }


    switch (count)
    {
    case 1:
        combinations = 4;
        break;
    case 2:
        combinations = 6;
        break;
    case 3:
        combinations = 4;
        break;
    case 4:
        combinations = 1;
        break;
    
    default:
        break;
    }

    joker = (int*)malloc(sizeof(int) * count);
    for (int i = 0; i < combinations; i++) {
        joker[0] = non_joker[(i % idx) + 1];
        joker[1] = non_joker[(i / idx) + 1];
        // TBD
    }
    
}


void print_hand(struct hand h)
{
    for (int i = 0; i < HAND_SIZE; i++) {
        switch (h.cards[i])
        {
        case T:
            printf("T ");
            break;
        case J:
            printf("J ");
            break;
        case Q:
            printf("Q ");
            break;
        case K:
            printf("K ");
            break;
        case A:
            printf("A ");
            break;
        
        default:
            printf("%d ", h.cards[i]);
            break;
        }
    }
    printf("(bid: %d), type: %d, rank: %d\n", h.bid, h.type, h.rank);
}


void main(int argc, char* argv[])
{
    struct hand* hands = NULL;
    int count;
    int sum = 0;


    count = count_hands(argv[1]);
    printf("count of cards: %d\n", count);


    hands = malloc(count * sizeof(struct hand));
    for (int i = 0; i < count; i++) {
        hands[i].cards = malloc(HAND_SIZE * sizeof(int));
        hands[i].rank = -1;
    }
    
    load_input(argv[1], hands);


    for (int i = 0; i < count; i++) {
        hands[i].type = get_types(hands[i]);
        // print_hand(hands[i]);
    }

    get_ranks(hands, count);
    
    for (int i = 0; i < count; i++) {
        print_hand(hands[i]);
    }

    for (int i = 0; i < count; i++) {
        sum += hands[i].rank * hands[i].bid;
    }
    printf("sum: %d\n", sum);
}
