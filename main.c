#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "player.h"
#include "cards.h"
#include <unistd.h>

void arrow1(int optPos, int curPos){
        if(optPos==curPos)printf(">");
};

void menuDesc(int i,int x){
        if(i == 1 && x == 1 )printf("    (Start a new game)\n\n");
        else if(i == 2 && x == 2 )printf("      (New to the Game?)\n\n");
        else if(i == 3 && x == 3 )printf("          (The Devs)\n\n");
        else printf("\n\n");
};

char renderMainMenu() {
    int x = 1;
    while(1){
        system("cls");
        if(x<1)x=4;
        if(x>4)x=1;
        printf("========== UNO ==========\n");
        arrow1(1,x);printf(" \033[0;32m[c]\033[0m Create a Session");menuDesc(1,x);
        arrow1(2,x);printf(" \033[0;34m[h]\033[0m How to play");menuDesc(2,x);
        arrow1(3,x);printf(" \033[0;33m[r]\033[0m Credits");menuDesc(3,x);
        arrow1(4,x);printf(" \033[0;31m[x]\033[0m Quit\n");
        printf("[Use Arrow keys to select]");
        char ch = getch();
        switch(ch){
            case 72: x--;break;
            case 75: x--;break;
            case 80: x++;break;
            case 77: x++;break;
            case 13:
            if(x==1)return 'c';
            if(x==2)return 'h';
            if(x==3)return 'r';
            if(x==4)return 'x';
            break;
        }
    }
}

int canPlay(char C, int N, struct CardStack stk,struct Card last){
    int chk1=0,chk2=0;
    for (int i = 0; i < stk.size; i++) {
        if((stk.cards[i].col == C) && (stk.cards[i].value == N)) {chk1 = 1;break;}
    }
    if((last.col == C) || (last.value == N) || (last.col = 'N')) chk2 = 1;

    if(chk1 && chk2){
        return 1;
    }else return 0;
}

int main(){
    

    PLAYER* head = NULL;

    int number_of_players,action,dir,N;
    char name[20],C;
    char menuChoice;
    struct CardStack players[7];
    
    struct Card lastPlayed;

    while(1){
        menuChoice = renderMainMenu();
        switch (menuChoice)
        {
        case 'c':
            srand(time(NULL));
            struct CardStack mainDeck;
            struct CardStack discardPile;
            discardPile.size = 0;
            initializeCardStack(&mainDeck);
            shuffleCardStack(&mainDeck);
            dir = 0;
            system("cls");
            printf("=== PLAYERS ==========================\n");
            printf("Enter number of players in session: ");
            scanf("%d",&number_of_players);
            
            for(int i=0;i<number_of_players;i++){
                printf("[Player %d] Name: ",i+1);
                scanf("%s",name);
                head = insert_player(head,name);
            }
            distributeCards(&mainDeck, players, number_of_players, 7);
            int win = -1;
            PLAYER* aux = head;
            int wild = 0;
            dealCard(&mainDeck,&discardPile);
            while(win==-1){
                system("cls");
                printf("=== %s's turn ==========\n\nEveryone else look away!\n\n[press ANY key]\n",aux->name);
                getch();
                printf("Last Played Card: ");
                printf("[%s%d]\n",discardPile.cards[discardPile.size-1].color,discardPile.cards[discardPile.size-1].value);
                printf("\n\nYour hand: ");
                printPlayerStack(&players[aux->id]);
                printf("Action: [1] Play card [2] Draw card: ");
                scanf("%d",&action);
                if(action == 1){
                    printf("Enter Card Code: ");
                    scanf(" %c%d",&C,&N);
                    if(canPlay(C,N,players[aux->id],discardPile.cards[discardPile.size-1])|| wild){
                        playCard(&players[aux->id],&discardPile,C,N);
                        if(players[aux->id].size != 0){
                            if(discardPile.cards[discardPile.size-1].value == 10){
                                if(dir==0)aux=aux->next;
                                else aux= aux->prev;
                            }
                            if(discardPile.cards[discardPile.size-1].value == 11){
                                if(dir == 0)dir = 1;
                                else dir = 0;
                            }
                            if(discardPile.cards[discardPile.size-1].value == 12){
                                if(dir == 0){
                                    dealCard(&mainDeck,&players[aux->next->id]);
                                    dealCard(&mainDeck,&players[aux->next->id]);
                                }else{
                                    dealCard(&mainDeck,&players[aux->prev->id]);
                                    dealCard(&mainDeck,&players[aux->prev->id]);
                                }
                                if(dir==0)aux=aux->next;
                                else aux= aux->prev;
                            }
                            if(discardPile.cards[discardPile.size-1].value == 13){
                                printf("Enter Color [R,B,G,Y]: ");
                                scanf(" %c",&C);
                                switch(C){
                                    case 'R':
                                    strcpy(discardPile.cards[discardPile.size-1].color,"\033[0;31mR\033[0m");
                                    discardPile.cards[discardPile.size-1].col = 'R';
                                    break;
                                    case 'B':
                                    strcpy(discardPile.cards[discardPile.size-1].color,"\033[0;34mB\033[0m");
                                    discardPile.cards[discardPile.size-1].col = 'B';
                                    break;
                                    case 'G':
                                    strcpy(discardPile.cards[discardPile.size-1].color,"\033[0;32mG\033[0m");
                                    discardPile.cards[discardPile.size-1].col = 'G';
                                    break;
                                    case 'Y':
                                    strcpy(discardPile.cards[discardPile.size-1].color,"\033[0;33mY\033[0m");
                                    discardPile.cards[discardPile.size-1].col = 'Y';
                                    break;
                                    default:
                                    strcpy(discardPile.cards[discardPile.size-1].color,"\033[0;31mR\033[0m");
                                    discardPile.cards[discardPile.size-1].col = 'R';
                                    break;
                                }
                                
                            }
                            if(discardPile.cards[discardPile.size-1].value == 14){
                                if(dir == 0){
                                    dealCard(&mainDeck,&players[aux->next->id]);
                                    dealCard(&mainDeck,&players[aux->next->id]);
                                    dealCard(&mainDeck,&players[aux->next->id]);
                                    dealCard(&mainDeck,&players[aux->next->id]);
                                }else{
                                    dealCard(&mainDeck,&players[aux->prev->id]);
                                    dealCard(&mainDeck,&players[aux->prev->id]);
                                    dealCard(&mainDeck,&players[aux->prev->id]);
                                    dealCard(&mainDeck,&players[aux->prev->id]);
                                }
                                printf("Enter Color [R,B,G,Y]: ");
                                scanf(" %c",&C);
                                switch(C){
                                    case 'R':
                                    strcpy(discardPile.cards[discardPile.size-1].color,"\033[0;31mR\033[0m");
                                    discardPile.cards[discardPile.size-1].col = 'R';
                                    break;
                                    case 'B':
                                    strcpy(discardPile.cards[discardPile.size-1].color,"\033[0;34mB\033[0m");
                                    discardPile.cards[discardPile.size-1].col = 'B';
                                    break;
                                    case 'G':
                                    strcpy(discardPile.cards[discardPile.size-1].color,"\033[0;32mG\033[0m");
                                    discardPile.cards[discardPile.size-1].col = 'G';
                                    break;
                                    case 'Y':
                                    strcpy(discardPile.cards[discardPile.size-1].color,"\033[0;33mY\033[0m");
                                    discardPile.cards[discardPile.size-1].col = 'Y';
                                    break;
                                    default:
                                    strcpy(discardPile.cards[discardPile.size-1].color,"\033[0;31mR\033[0m");
                                    discardPile.cards[discardPile.size-1].col = 'R';
                                    break;
                                }
                                if(dir==0)aux=aux->next;
                                else aux= aux->prev;
                            }
                        }else{
                            win = aux->id;
                            break;
                        }
                        if(dir==0)aux=aux->next;
                        else aux= aux->prev;
                    }else{
                        printf("\n[!] Invalid Card :/");
                        continue;
                    }

                    
                }else if(action == 2){
                    dealCard(&mainDeck,&players[aux->id]);
                    if(dir==0)aux=aux->next;
                    else aux= aux->prev;
                }

                

            }
            system("cls");
            printf("==== CONGRATULATIONS ");
            search_player_name(head,win);
            printf(" !!! ====\n\n");
            printf("You've have won this game\n");
            sleep(5);
            break;
        case 'x':
        break;
        }
        if(menuChoice == 'x')break;
    }
}