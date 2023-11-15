#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char colors[][18] = {"\033[0;31mR\033[0m\0", "\033[0;32mG\033[0m\0", "\033[0;34mB\033[0m\0", "\033[0;33mY\033[0m\0"};
char col[] = {'R','G','B','Y'};
struct Card {
    char color[19];
    char col;
    int value;
};

struct CardStack {
    struct Card cards[108];
    int size;
};

void initializeCardStack(struct CardStack *stack) {
    int index = 0;

    for (int i = 0; i < 4; i++) {
        for (int value = 1; value <= 9; value++) {
            strcpy(stack->cards[index].color,colors[i]);
            stack->cards[index].col = col[i];
            stack->cards[index].value = value;
            index++;
            strcpy(stack->cards[index].color,colors[i]);
            stack->cards[index].col = col[i];
            stack->cards[index].value = value;
            index++;
        }

        strcpy(stack->cards[index].color,colors[i]);
        stack->cards[index].col = col[i];
        stack->cards[index].value = 0;
        index++;

        strcpy(stack->cards[index].color,colors[i]);
        stack->cards[index].col = col[i];
        stack->cards[index].value = 10;
        index++;

        strcpy(stack->cards[index].color,colors[i]);
        stack->cards[index].col = col[i];
        stack->cards[index].value = 11;
        index++;

        strcpy(stack->cards[index].color,colors[i]);
        stack->cards[index].col = col[i];
        stack->cards[index].value = 12;
        index++;
    }

    for (int i = 0; i < 4; i++) {
        strcpy(stack->cards[index].color,"N\0");
        stack->cards[index].col = 'N';
        stack->cards[index].value = 13;
        index++;
        strcpy(stack->cards[index].color,"N\0");
        stack->cards[index].col = 'N';
        stack->cards[index].value = 14;
        index++;
    }
    stack->size = index;
}

void shuffleCardStack(struct CardStack *stack) {
    for (int i = 0; i < stack->size; i++) {
        int j = rand() % stack->size;
        struct Card temp = stack->cards[i];
        stack->cards[i] = stack->cards[j];
        stack->cards[j] = temp;
    }
}

void distributeCards(struct CardStack *deck, struct CardStack *players, int numPlayers, int cardsPerPlayer) {
    for (int i = 0; i < numPlayers; i++) {
        players[i].size = 0;
    }

    int currentPlayer = 0;
    for (int i = 0; i < numPlayers * cardsPerPlayer; i++) {
        players[currentPlayer].cards[players[currentPlayer].size++] = deck->cards[i];
        currentPlayer = (currentPlayer + 1) % numPlayers;
    }
}

void printPlayerStack(struct CardStack *playerStack) {
    int cnt = 0;
    for (int i = 0; i < playerStack->size; i++) {
        cnt++;
        printf("[%s%d] ", playerStack->cards[i].color, playerStack->cards[i].value);
    }
    printf("\nNo. of Cards: %d\n",cnt);
    printf("\n");
}

void printMainCardStack(struct CardStack *mainStack) {
    printf("Main Card Stack:\n");
    int cnt = 0;
    for (int i = 0; i < mainStack->size; i++) {
        cnt++;
        printf("Color: %s, Value: %d\n", mainStack->cards[i].color, mainStack->cards[i].value);
    }
    printf("No. of Cards: %d\n",cnt);
    printf("\n");
}

// Function to pop a card from the main deck and push it onto a player's deck
void dealCard(struct CardStack *mainDeck, struct CardStack *playerDeck) {
    if (mainDeck->size > 0) {
        // Pop a card from the main deck
        struct Card dealtCard = mainDeck->cards[--(mainDeck->size)];

        // Push the card onto the player's deck
        playerDeck->cards[playerDeck->size++] = dealtCard;
    } else {
        printf("Error: Main deck is empty.\n");
    }
}

void playCard(struct CardStack *s1, struct CardStack *s2, char C, int N) {
    int c1 = s1->size;
    int c2 = s2->size;
    struct Card card;
    card.col = C;
    card.value = N;
    // Find the index of the card in s1
    int indexOfCard = -1;
    for (int i = 0; i < c1; i++) {
        if (s1->cards[i].col == card.col && s1->cards[i].value == card.value) {
            indexOfCard = i;
            break;
        }
    }

    // If the card is found in s1
    if (indexOfCard != -1) {
        // Remove the card from s1
        struct Card removedCard = s1->cards[indexOfCard];
        for (int i = indexOfCard; i < c1 - 1; i++) {
            s1->cards[i] = s1->cards[i + 1];
        }
        s1->size--;

        // Append the removed card to s2
        s2->cards[c2++] = removedCard;
        s2->size++;
    } else {
        printf("Card not found in s1\n");
    }
}

// int main() {
//     srand(time(NULL));

//     struct CardStack mainDeck;
//     initializeCardStack(&mainDeck);

//     shuffleCardStack(&mainDeck);

//     int numPlayers = 4;
//     int cardsPerPlayer = 7;

//     struct CardStack players[numPlayers];

//     distributeCards(&mainDeck, players, numPlayers, cardsPerPlayer);

//     printPlayerStack(&players[0], "Player 1");
//     printPlayerStack(&players[0], "Player 2");

//     printMainCardStack(&mainDeck);

//     return 0;
// }