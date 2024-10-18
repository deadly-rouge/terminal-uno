#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <unistd.h>

#define MAX_PLAYERS 7

// Card structure
typedef struct Card {
    char col; // Color (R, Y, G, B, N for wild)
    char color[10]; // Colored string for display
    int value; // Number (0-9) or action (10-14)
} Card;

// Card stack structure
typedef struct CardStack {
    Card cards[108];
    int size;
} CardStack;

// Player structure
typedef struct Player {
    char name[20];
    int id; // Player ID
    CardStack cards;
    struct Player* next;
    struct Player* prev;
} Player;

// Function to initialize a card stack
void initializeCardStack(CardStack* stack) {
    stack->size = 0;

    // Add number cards
    for (char color = 'R'; color <= 'B'; color++) {
        for (int value = 0; value <= 9; value++) {
            stack->cards[stack->size].col = color;
            if (color == 'R') {
                strcpy(stack->cards[stack->size].color, "\033[0;31mR\033[0m");
            } else if (color == 'Y') {
                strcpy(stack->cards[stack->size].color, "\033[0;33mY\033[0m");
            } else if (color == 'G') {
                strcpy(stack->cards[stack->size].color, "\033[0;32mG\033[0m");
            } else {
                strcpy(stack->cards[stack->size].color, "\033[0;34mB\033[0m");
            }
            stack->cards[stack->size].value = value;
            stack->size++;
        }
    }

    // Add action cards (Skip, Reverse, Draw 2)
    for (char color = 'R'; color <= 'B'; color++) {
        for (int i = 10; i <= 12; i++) {
            stack->cards[stack->size].col = color;
            if (color == 'R') {
                strcpy(stack->cards[stack->size].color, "\033[0;31mR\033[0m");
            } else if (color == 'Y') {
                strcpy(stack->cards[stack->size].color, "\033[0;33mY\033[0m");
            } else if (color == 'G') {
                strcpy(stack->cards[stack->size].color, "\033[0;32mG\033[0m");
            } else {
                strcpy(stack->cards[stack->size].color, "\033[0;34mB\033[0m");
            }
            stack->cards[stack->size].value = i;
            stack->size++;
        }
    }

    // Add wild cards
    for (int i = 0; i < 4; i++) {
        stack->cards[stack->size].col = 'N';
        strcpy(stack->cards[stack->size].color, "\033[0;37mWild\033[0m");
        stack->cards[stack->size].value = 13; // Wild card
        stack->size++;
    }

    // Add wild draw four cards
    for (int i = 0; i < 4; i++) {
        stack->cards[stack->size].col = 'N';
        strcpy(stack->cards[stack->size].color, "\033[0;37mWild\033[0m");
        stack->cards[stack->size].value = 14; // Wild Draw Four card
        stack->size++;
    }
}

// Function to shuffle a card stack
void shuffleCardStack(CardStack* stack) {
    for (int i = stack->size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Card temp = stack->cards[i];
        stack->cards[i] = stack->cards[j];
        stack->cards[j] = temp;
    }
}

// Function to deal a card from a stack to another stack
void dealCard(CardStack* from, CardStack* to) {
    if (from->size > 0) {
        to->cards[to->size] = from->cards[from->size - 1];
        from->size--;
        to->size++;
    }
}

// Function to print a card stack
void printCardStack(CardStack* stack) {
    for (int i = 0; i < stack->size; i++) {
        printf("[%s%d] ", stack->cards[i].color, stack->cards[i].value);
    }
    printf("\n");
}

// Function to print a player's hand
void printPlayerStack(CardStack* stack) {
    for (int i = 0; i < stack->size; i++) {
        printf("[%c%d] ", stack->cards[i].col, stack->cards[i].value);
    }
    printf("\n");
}

// Function to insert a player into the linked list
Player* insert_player(Player* head, char* name) {
    Player* newPlayer = (Player*)malloc(sizeof(Player));
    strcpy(newPlayer->name, name);
    newPlayer->id = 0; // Initialize ID to 0 for now
    newPlayer->cards.size = 0;
    newPlayer->next = NULL;
    newPlayer->prev = NULL;

    if (head == NULL) {
        head = newPlayer;
        head->next = head;
        head->prev = head;
        return head;
    }

    // Insert at the end of the list
    Player* current = head;
    while (current->next != head) {
        current = current->next;
    }
    current->next = newPlayer;
    newPlayer->prev = current;
    newPlayer->next = head;
    head->prev = newPlayer;

    return head;
}

// Function to search for a player by name
void search_player_name(Player* head, int id) {
    Player* current = head;
    do {
        if (current->id == id) {
            printf("%s", current->name);
            return;
        }
        current = current->next;
    } while (current != head);
}

// Function to distribute cards to players
void distributeCards(CardStack* mainDeck, CardStack* players, int numPlayers, int cardsPerPlayer) {
    for (int i = 0; i < numPlayers; i++) {
        for (int j = 0; j < cardsPerPlayer; j++) {
            dealCard(mainDeck, &players[i]);
        }
    }
}

// Function to check if a player can play a card
int canPlay(char C, int N, CardStack playerStack, Card lastPlayed, int sandwiching) {
    // Check if the player has the card
    for (int i = 0; i < playerStack.size; i++) {
        if (playerStack.cards[i].col == C && playerStack.cards[i].value == N) {
            return 1; // Player has the card
        }
    }

    // Check if the card matches the last played card's color or number
    if (lastPlayed.col == C || lastPlayed.value == N || lastPlayed.col == 'N') {
        return 1; // Card can be played
    }

    // If sandwiching is ON, allow playing cards of the same color as last played
    if (sandwiching && C == lastPlayed.col) {
        return 1; // Card can be played
    }

    return 0; // Card cannot be played
}

// Function to play a card
void playCard(CardStack* playerStack, CardStack* discardPile, char C, int N) {
    for (int i = 0; i < playerStack->size; i++) {
        if (playerStack->cards[i].col == C && playerStack->cards[i].value == N) {
            discardPile->cards[discardPile->size] = playerStack->cards[i];
            discardPile->size++;
            // Remove the card from the player's hand
            for (int j = i; j < playerStack->size - 1; j++) {
                playerStack->cards[j] = playerStack->cards[j + 1];
            }
            playerStack->size--;
            break;
        }
    }
}

// Arrow function for menu
void arrow1(int optPos, int curPos) {
    if (optPos == curPos)
        printf(">");
}

// Menu description function
void menuDesc(int i, int x) {
    if (i == 1 && x == 1)
        printf("    (Start a new game)\n\n");
    else if (i == 2 && x == 2)
        printf("      (New to the Game?)\n\n");
    else if (i == 3 && x == 3)
        printf("          (The Devs)\n\n");
    else if (i == 1 && x == 1)
        printf("    (Play cards of same color consecutively)\n\n");
    else if (i == 2 && x == 2)
        printf("      (Play in reverse order)\n\n");
    else
        printf("\n\n");
}

// Function to render the main menu
char renderMainMenu() {
    int x = 1;
    while (1) {
        system("cls");
        if (x < 1) x = 4;
        if (x > 4) x = 1;
        printf("========== UNO ==========\n");
        arrow1(1, x); printf(" \033[0;32m[c]\033[0m Create a Session"); menuDesc(1, x);
        arrow1(2, x); printf(" \033[0;34m[h]\033[0m How to play"); menuDesc(2, x);
        arrow1(3, x); printf(" \033[0;33m[r]\033[0m Credits"); menuDesc(3, x);
        arrow1(4, x); printf(" \033[0;31m[s]\033[0m Settings"); menuDesc(4, x);
        arrow1(5, x); printf(" \033[0;31m[x]\033[0m Quit\n");
        printf("[Use Arrow keys to select]");
        char ch = getch();
        switch (ch) {
            case 72: x--; break;
            case 75: x--; break;
            case 80: x++; break;
            case 77: x++; break;
            case 13:
                if (x == 1) return 'c';
                if (x == 2) return 'h';
                if (x == 3) return 'r';
                if (x == 4) return 's';
                if (x == 5) return 'x';
                break;
        }
    }
}

// Function to render the settings menu
char display_settings_menu() {
    int x = 1;
    while (1) {
        system("cls");
        if (x < 1) x = 3;
        if (x > 3) x = 1;
        printf("========== UNO Settings ==========\n");
        arrow1(1, x); printf(" \033[0;32m[s]\033[0m Sandwiching (ON)"); menuDesc(1, x);
        arrow1(2, x); printf(" \033[0;34m[r]\033[0m Reverse Rule (OFF)"); menuDesc(2, x);
        arrow1(3, x); printf(" \033[0;33m[b]\033[0m Back to Main Menu\n");
        printf("[Use Arrow keys to select]");
        char ch = getch();
        switch (ch) {
            case 72: x--; break;
            case 75: x--; break;
            case 80: x++; break;
            case 77: x++; break;
            case 13:
                if (x == 1) return 's';
                if (x == 2) return 'r';
                if (x == 3) return 'b';
                break;
        }
    }
}

// Function to display how to play the game
void display_how_to_play() {
    printf("\n\n                                                               \033[0;34mHOW TO PLAY\033[0m                                                \n\n                                                \033[0;31m*****************************************\033[0m\n\nUNO is a popular card game that has been enjoyed by people across the globe. The game was originally developed in 1971.\n\nThe main objective of UNO is to be the first player to play all your cards.\nUNO is a game that blends elements of strategy with a touch of luck.\n\nA standard UNO deck consists of 108 cards.\nThese cards are divided into four colors: Red, Yellow, Green, and Blue (depicted as R, Y, G, B in-game).\n\nEach color has numbers from 0 to 9, as well as special action cards.\nThe special cards are:\n\033[0;33mSkip:\033[0m skipping a player's turn\n\033[0;33mReverse:\033[0m reversing the order of play\n\033[0;33mDraw 2:\033[0m forcing the next player to draw 2 cards\n\033[0;33mWild card:\033[0m Allows players to change the color of the card to be played\n\033[0;33mWild Draw Four:\033[0m Similar to the wild card but forces the next player to draw 4 cards and forfeit their turn\n\n\033[0;32mSetup:\033[0m\n1. First shuffle the deck of cards and deal 7 cards to each player.\n2. Next place the remaining deck face down on the table to form the draw pile.\n3. Finally, turn the top card from the draw pile face up next to create a discard pile and start the game.\n\n\033[0;32mGameplay:\033[0m\n- First, the players take turns playing a card that matches the top card of the discard pile in either color or number.\n- If a player cannot play the card they have, they must draw a card from the draw pile. If that card can be played, they may play it immediately. Otherwise, their turn comes to an end.\n\n- When a player has only one card left, they must say 'UNO' and alert the other players. \nIf they fail to do so and get caught, it results in a penalty of drawing two cards.\n- The first player to play all their cards wins.\n\n                                                \033[0;31m*****************************************\033[0m");
}

int main() {
    Player* head = NULL;
    int number_of_players, action, dir, N;
    char name[20], C;
    char menuChoice;
    CardStack players[MAX_PLAYERS];
    CardStack mainDeck;
    CardStack discardPile;
    discardPile.size = 0;
    initializeCardStack(&mainDeck);
    shuffleCardStack(&mainDeck);
    dir = 0;
    int sandwiching = 0;
    int reverseRule = 0;

    while (1) {
        menuChoice = renderMainMenu();
        switch (menuChoice) {
            case 'c':
                srand(time(NULL));
                system("cls");
                printf("=== PLAYERS ==========================\n");
                printf("Enter number of players in session: ");
                scanf("%d", &number_of_players);
                if (number_of_players < 2 || number_of_players > MAX_PLAYERS) {
                    printf("Invalid number of players. Please enter between 2 and 7.\n");
                    continue;
                }
                for (int i = 0; i < number_of_players; i++) {
                    printf("[Player %d] Name: ", i + 1);
                    scanf("%s", name);
                    head = insert_player(head, name);
                    head->id = i; // Assign player ID
                }
                distributeCards(&mainDeck, players, number_of_players, 7);
                int win = -1;
                Player* aux = head;
                int wild = 0;
                dealCard(&mainDeck, &discardPile);
                while (win == -1) {
                    system("cls");
                    printf("=== %s's turn ==========\n\nEveryone else look away!\n\n[press ANY key]\n", aux->name);
                    getch();
                    printf("Last Played Card: ");
                    printf("[%s%d]\n", discardPile.cards[discardPile.size - 1].color, discardPile.cards[discardPile.size - 1].value);
                    printf("\n\nYour hand: ");
                    printPlayerStack(&players[aux->id]);
                    printf("Action: [1] Play card [2] Draw card: ");
                    scanf("%d", &action);
                    if (action == 1) {
                        printf("Enter Card Code: ");
                        scanf(" %c%d", &C, &N);
                        if (canPlay(C, N, players[aux->id], discardPile.cards[discardPile.size - 1], sandwiching) || wild) {
                            // If sandwiching is ON, check if the card is the same color as the last played
                            if (sandwiching && (C == discardPile.cards[discardPile.size - 1].col)) {
                                // Allow multiple card plays of the same color
                                while (canPlay(C, N, players[aux->id], discardPile.cards[discardPile.size - 1], sandwiching)) {
                                    playCard(&players[aux->id], &discardPile, C, N);
                                    if (players[aux->id].size != 0) {
                                        if (discardPile.cards[discardPile.size - 1].value == 10) {
                                            if (dir == 0)
                                                aux = aux->next;
                                            else
                                                aux = aux->prev;
                                        }
                                        if (discardPile.cards[discardPile.size - 1].value == 11) {
                                            if (dir == 0)
                                                dir = 1;
                                            else
                                                dir = 0;
                                        }
                                        if (discardPile.cards[discardPile.size - 1].value == 12) {
                                            if (dir == 0) {
                                                dealCard(&mainDeck, &players[aux->next->id]);
                                                dealCard(&mainDeck, &players[aux->next->id]);
                                            } else {
                                                dealCard(&mainDeck, &players[aux->prev->id]);
                                                dealCard(&mainDeck, &players[aux->prev->id]);
                                            }
                                            if (dir == 0)
                                                aux = aux->next;
                                            else
                                                aux = aux->prev;
                                        }
                                        if (discardPile.cards[discardPile.size - 1].value == 13) {
                                            printf("Enter Color [R,B,G,Y]: ");
                                            scanf(" %c", &C);
                                            switch (C) {
                                                case 'R':
                                                    strcpy(discardPile.cards[discardPile.size - 1].color, "\033[0;31mR\033[0m");
                                                    discardPile.cards[discardPile.size - 1].col = 'R';
                                                    break;
                                                case 'B':
                                                    strcpy(discardPile.cards[discardPile.size - 1].color, "\033[0;34mB\033[0m");
                                                    discardPile.cards[discardPile.size - 1].col = 'B';
                                                    break;
                                                case 'G':
                                                    strcpy(discardPile.cards[discardPile.size - 1].color, "\033[0;32mG\033[0m");
                                                    discardPile.cards[discardPile.size - 1].col = 'G';
                                                    break;
                                                case 'Y':
                                                    strcpy(discardPile.cards[discardPile.size - 1].color, "\033[0;33mY\033[0m");
                                                    discardPile.cards[discardPile.size - 1].col = 'Y';
                                                    break;
                                                default:
                                                    strcpy(discardPile.cards[discardPile.size - 1].color, "\033[0;31mR\033[0m");
                                                    discardPile.cards[discardPile.size - 1].col = 'R';
                                                    break;
                                            }
                                        }
                                        if (discardPile.cards[discardPile.size - 1].value == 14) {
                                            if (dir == 0) {
                                                dealCard(&mainDeck, &players[aux->next->id]);
                                                dealCard(&mainDeck, &players[aux->next->id]);
                                                dealCard(&mainDeck, &players[aux->next->id]);
                                                dealCard(&mainDeck, &players[aux->next->id]);
                                            } else {
                                                dealCard(&mainDeck, &players[aux->prev->id]);
                                                dealCard(&mainDeck, &players[aux->prev->id]);
                                                dealCard(&mainDeck, &players[aux->prev->id]);
                                                dealCard(&mainDeck, &players[aux->prev->id]);
                                            }
                                            printf("Enter Color [R,B,G,Y]: ");
                                            scanf(" %c", &C);
                                            switch (C) {
                                                case 'R':
                                                    strcpy(discardPile.cards[discardPile.size - 1].color, "\033[0;31mR\033[0m");
                                                    discardPile.cards[discardPile.size - 1].col = 'R';
                                                    break;
                                                case 'B':
                                                    strcpy(discardPile.cards[discardPile.size - 1].color, "\033[0;34mB\033[0m");
                                                    discardPile.cards[discardPile.size - 1].col = 'B';
                                                    break;
                                                case 'G':
                                                    strcpy(discardPile.cards[discardPile.size - 1].color, "\033[0;32mG\033[0m");
                                                    discardPile.cards[discardPile.size - 1].col = 'G';
                                                    break;
                                                case 'Y':
                                                    strcpy(discardPile.cards[discardPile.size - 1].color, "\033[0;33mY\033[0m");
                                                    discardPile.cards[discardPile.size - 1].col = 'Y';
                                                    break;
                                                default:
                                                    strcpy(discardPile.cards[discardPile.size - 1].color, "\033[0;31mR\033[0m");
                                                    discardPile.cards[discardPile.size - 1].col = 'R';
                                                    break;
                                            }
                                            if (dir == 0)
                                                aux = aux->next;
                                            else
                                                aux = aux->prev;
                                        }
                                    } else {
                                        win = aux->id;
                                        break;
                                    }
                                    if (dir == 0)
                                        aux = aux->next;
                                    else
                                        aux = aux->prev;
                                }
                            } else {
                                playCard(&players[aux->id], &discardPile, C, N);
                                if (players[aux->id].size != 0) {
                                    if (discardPile.cards[discardPile.size - 1].value == 10) {
                                        if (dir == 0)
                                            aux = aux->next;
                                        else
                                            aux = aux->prev;
                                    }
                                    if (discardPile.cards[discardPile.size - 1].value == 11) {
                                        if (dir == 0)
                                            dir = 1;
                                        else
                                            dir = 0;
                                    }
                                    if (discardPile.cards[discardPile.size - 1].value == 12) {
                                        if (dir == 0) {
                                            dealCard(&mainDeck, &players[aux->next->id]);
                                            dealCard(&mainDeck, &players[aux->next->id]);
                                        } else {
                                            dealCard(&mainDeck, &players[aux->prev->id]);
                                            dealCard(&mainDeck, &players[aux->prev->id]);
                                        }
                                        if (dir == 0)
                                            aux = aux->next;
                                        else
                                            aux = aux->prev;
                                    }
                                    if (discardPile.cards[discardPile.size - 1].value == 13) {
                                        printf("Enter Color [R,B,G,Y]: ");
                                        scanf(" %c", &C);
                                        switch (C) {
                                            case 'R':
                                                strcpy(discardPile.cards[discardPile.size - 1].color, "\033[0;31mR\033[0m");
                                                discardPile.cards[discardPile.size - 1].col = 'R';
                                                break;
                                            case 'B':
                                                strcpy(discardPile.cards[discardPile.size - 1].color, "\033[0;34mB\033[0m");
                                                discardPile.cards[discardPile.size - 1].col = 'B';
                                                break;
                                            case 'G':
                                                strcpy(discardPile.cards[discardPile.size - 1].color, "\033[0;32mG\033[0m");
                                                discardPile.cards[discardPile.size - 1].col = 'G';
                                                break;
                                            case 'Y':
                                                strcpy(discardPile.cards[discardPile.size - 1].color, "\033[0;33mY\033[0m");
                                                discardPile.cards[discardPile.size - 1].col = 'Y';
                                                break;
                                            default:
                                                strcpy(discardPile.cards[discardPile.size - 1].color, "\033[0;31mR\033[0m");
                                                discardPile.cards[discardPile.size - 1].col = 'R';
                                                break;
                                        }
                                    }
                                    if (discardPile.cards[discardPile.size - 1].value == 14) {
                                        if (dir == 0) {
                                            dealCard(&mainDeck, &players[aux->next->id]);
                                            dealCard(&mainDeck, &players[aux->next->id]);
                                            dealCard(&mainDeck, &players[aux->next->id]);
                                            dealCard(&mainDeck, &players[aux->next->id]);
                                        } else {
                                            dealCard(&mainDeck, &players[aux->prev->id]);
                                            dealCard(&mainDeck, &players[aux->prev->id]);
                                            dealCard(&mainDeck, &players[aux->prev->id]);
                                            dealCard(&mainDeck, &players[aux->prev->id]);
                                        }
                                        printf("Enter Color [R,B,G,Y]: ");
                                        scanf(" %c", &C);
                                        switch (C) {
                                            case 'R':
                                                strcpy(discardPile.cards[discardPile.size - 1].color, "\033[0;31mR\033[0m");
                                                discardPile.cards[discardPile.size - 1].col = 'R';
                                                break;
                                            case 'B':
                                                strcpy(discardPile.cards[discardPile.size - 1].color, "\033[0;34mB\033[0m");
                                                discardPile.cards[discardPile.size - 1].col = 'B';
                                                break;
                                            case 'G':
                                                strcpy(discardPile.cards[discardPile.size - 1].color, "\033[0;32mG\033[0m");
                                                discardPile.cards[discardPile.size - 1].col = 'G';
                                                break;
                                            case 'Y':
                                                strcpy(discardPile.cards[discardPile.size - 1].color, "\033[0;33mY\033[0m");
                                                discardPile.cards[discardPile.size - 1].col = 'Y';
                                                break;
                                            default:
                                                strcpy(discardPile.cards[discardPile.size - 1].color, "\033[0;31mR\033[0m");
                                                discardPile.cards[discardPile.size - 1].col = 'R';
                                                break;
                                        }
                                        if (dir == 0)
                                            aux = aux->next;
                                        else
                                            aux = aux->prev;
                                    }
                                } else {
                                    win = aux->id;
                                    break;
                                }
                                if (dir == 0)
                                    aux = aux->next;
                                else
                                    aux = aux->prev;
                            }
                        } else {
                            printf("\n[!] Invalid Card :/");
                            continue;
                        }
                    } else if (action == 2) {
                        dealCard(&mainDeck, &players[aux->id]);
                        if (dir == 0)
                            aux = aux->next;
                        else
                            aux = aux->prev;
                    }
                }
                system("cls");
                printf("==== CONGRATULATIONS ");
                search_player_name(head, win);
                printf(" !!! ====\n\n");
                printf("You've have won this game\n");
                sleep(5);
                break;
            case 'h':
                system("cls");
                display_how_to_play();
                printf("\n[PRESS ANY KEY TO CONTINUE]\n");
                getch();
                system("cls");
                break;
            case 'r':
                system("cls");
                printf("=== DEVELOPED BY ===\n");
                // ... (Your credits)
                sleep(10);
                system("cls");
                break;
            case 's':
                char settingsChoice = display_settings_menu();
                switch (settingsChoice) {
                    case 's':
                        sandwiching = !sandwiching;
                        if (sandwiching) {
                            printf("\nSandwiching is now ON.\n");
                        } else {
                            printf("\nSandwiching is now OFF.\n");
                        }
                        sleep(2);
                        break;
                    case 'r':
                        reverseRule = !reverseRule;
                        if (reverseRule) {
                            printf("\nReverse Rule is now ON.\n");
                        } else {
                            printf("\nReverse Rule is now OFF.\n");
                        }
                        sleep(2);
                        break;
                    case 'b':
                        break;
                }
                break;
            case 'x':
                break;
        }
        if (menuChoice == 'x')
            break;
    }
    return 0;
}
