#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char colors[] = {'R', 'G', 'B', 'Y'};

struct Card {
    char color;
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
            stack->cards[index].color = colors[i];
            stack->cards[index].value = value;
            index++;
            stack->cards[index].color = colors[i];
            stack->cards[index].value = value;
            index++;
        }

        stack->cards[index].color = colors[i];
        stack->cards[index].value = 0;
        index++;

        stack->cards[index].color = colors[i];
        stack->cards[index].value = 10;
        index++;

        stack->cards[index].color = colors[i];
        stack->cards[index].value = 11;
        index++;

        stack->cards[index].color = colors[i];
        stack->cards[index].value = 12;
        index++;
    }

    for (int i = 0; i < 4; i++) {
        stack->cards[index].color = 'N';
        stack->cards[index].value = 13;
        index++;
        stack->cards[index].color = 'N';
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

void printPlayerStack(struct CardStack *playerStack, char *playerName) {
    printf("%s's Card Stack:\n", playerName);
    for (int i = 0; i < playerStack->size; i++) {
        printf("Color: %c, Value: %d\n", playerStack->cards[i].color, playerStack->cards[i].value);
    }
    printf("\n");
}

void printMainCardStack(struct CardStack *mainStack) {
    printf("Main Card Stack:\n");
    for (int i = 0; i < mainStack->size; i++) {
        printf("Color: %c, Value: %d\n", mainStack->cards[i].color, mainStack->cards[i].value);
    }
    printf("\n");
}

int main() {
    srand(time(NULL));

    struct CardStack mainDeck;
    initializeCardStack(&mainDeck);

    shuffleCardStack(&mainDeck);

    int numPlayers = 4;
    int cardsPerPlayer = 7;

    struct CardStack players[numPlayers];

    distributeCards(&mainDeck, players, numPlayers, cardsPerPlayer);

    printPlayerStack(&players[0], "Player 1");
    printPlayerStack(&players[0], "Player 2");

    printMainCardStack(&mainDeck);

    return 0;
}