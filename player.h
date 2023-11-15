#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
int init_id = 0;
char data[500];

typedef struct pStr{
    char name[20];
    int id;
    char played[3];
    
} pStr;


typedef struct player_node   
{
    int id;
    char name[20];
    int remaining_cards;
    struct player_node* next;
    struct player_node* prev;
}PLAYER;

PLAYER* create_player(int id, char name[])     //create player node
{
    PLAYER* new_player = (PLAYER*) malloc(sizeof(PLAYER));
    new_player->id = id;
    strcpy(new_player->name, name);
    new_player->remaining_cards = 7;
    new_player->next = NULL;
    new_player->prev = NULL;
    return new_player;
}

PLAYER* insert_player(PLAYER* head, char name[])      //inserting a new player with appended id
{
    PLAYER* newplayer = create_player(init_id++, name);
    if(head == NULL)
    {
        head = newplayer;
        head->next = head;
        head->prev = head;
    }
    else
    {
        PLAYER* curr = head->prev;
        curr->next = newplayer;
        newplayer->next = head;
        newplayer->prev = curr;
        head->prev = newplayer;
    }
    return head;
}



void display(PLAYER* head)
{
    if(head == NULL)
        printf("List is empty\n");
    else
    {
    PLAYER *temp = head;
    while(temp->next != head)
    {
        printf("player_id: %d    player_name: %s    remaining cards: %d\n", temp->id, temp->name, temp->remaining_cards);
        temp = temp->next;
    }
    printf("player_id: %d    player_name: %s    remaining cards: %d\n", head->prev->id, head->prev->name, head->prev->remaining_cards);
    }
}


void search_player_name(PLAYER* head, int id){
    PLAYER* current = head;
    while (current->next != head)
    {
    if (current->id == id){
        printf("%s",current->name);
        break;
    }
    current = current->next;
    }
}


// (Arnab:1[7],Arun:2[7])|1|Y3|B7

// int main(){
//     PLAYER* head = NULL;
//     char name[20];
//     for(int i=0;i<3;i++){
//         printf("Enter name: ");
//         scanf("%s",name);
//         head = insert_player(head,name);
//     }
//     char str[1024] = {'\0'};
//     genNamePS(head,str);
//     printf("%s",str);
    
// }