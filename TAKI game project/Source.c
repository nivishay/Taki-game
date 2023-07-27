//Niv ishay 
//318879285

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include<string.h>

//genaral defines
#define MAX_NAME_LENGTH 20 
#define MAX_TYPE_CARD_LENGTH 7
#define ZERO_ASCI 48

//cards realted
#define BASE_NUM_OF_CARDS 4
#define MAX_TYPE_CARD_LENGTH 7
#define TAKE_CARD -1

//cards color
#define RED  1
#define YELLOW 2
#define BLUE 3
#define GREEN 4

// uniqe cards 
#define PLUS_CARD 10
#define STOP_CARD 11
#define CHANGE_DIRECTION_CARD 12
#define TAKI_CARD 13
#define CHANGE_COLOR_CARD 14

//stats of cards
#define CARDS_TYPE 14

typedef struct card // this a type of card. each will be recognize by Color and the type of card. 
{
	char color;
	char card_type[MAX_TYPE_CARD_LENGTH];
}CARD;

typedef struct player 
{
	char name_of_player[MAX_NAME_LENGTH];
	CARD* cards; // dynamic array of cards(each card is type CARD)
	int cards_in_hand; //logical size of cards
	int max_cards; //physical size of cards 
}PLAYER;

typedef struct statistic
{
	char card_type[MAX_TYPE_CARD_LENGTH];
	int count;
}STATS;

// allocate a dynamic array of type PLAYER (the size of this array is the number of players)
PLAYER* get_number_of_players(PLAYER* players_data_p, int* number_of_players);

// allocate a dynamic array of type card for each player (the size of this array is the number of players)
void malloc_dynamic_cards(PLAYER* player, int num_of_players);

// this fucntion adds space in the heap for new cards to be drawn(if he reaches the current max)
void NoSpaceForCards(PLAYER* player, int player_num);

// this function is the turn of pplayer (check what move he chose to play an implemnt the corret function of this card)
void player_turn(int* playerChoice, PLAYER* players_of_taki, int* i, CARD* upper_card, bool* taki_card, STATS arr[]);

void game_statistic_calcualtor(STATS arr[]);

void print_game_statistic(STATS arr[]);

void free_player_arrays(PLAYER* player,int num);
////////////////////////////////
///get player names functions///
////////////////////////////////

//get the name from one player.
void single_player_name(PLAYER *player, int player_number);

//gets all of the players names.
void get_players_names(PLAYER *player, int number_of_players);

//////////////////////////////////////////////
///dealing cards functions and taking cards///
/////////////////////////////////////////////

 //this function use the give_a_card function to give each player 4 cards.
void first_dealing_of_cards(PLAYER* player, int number_of_players,STATS arr[]);

//this function gives a player a single card from the deck.
void give_a_card(PLAYER* player, int i, int card_index,STATS arr[]);

// this function returns and integer between 0 to cards in hand.
int CheckValidRoundChoice(PLAYER* player, int player_num, bool taki_card);

//this function lets the user choose what color to use.
void use_color_card(PLAYER* player, int player_num, int card_num);

////////////////////////////////////
///print cards to screen function///
////////////////////////////////////

// prints 1 card
void print_single_card(CARD* card);

//prints a single line depends on what card
void print_card_type_line(CARD* card);

//prints all of the cards in a single player hand
void print_current_cards_in_hand(PLAYER* player, int player_num);

// changing the upper card to the new used card
void change_upper_card(CARD* card);

////////////////////////////////////////////////////
///validation of player choice and cards check//////
////////////////////////////////////////////////////

// this function checks if the player choice is in the giving range of the cards in his hands
int CheckValidRoundChoice(PLAYER* player, int player_num,bool taki_Card);

// this function returns true if the player can use the card he chose
int check_player_turn_choice(CARD* Upper_card, PLAYER* player, int player_num,bool taki_card);

bool check_if_STOP_card(int playerChoice, PLAYER* player, int player_num);

bool check_if_COLOR_card(int playerChoice, PLAYER* player, int player_num);

bool check_if_PLUS_card(int playerChoice, PLAYER* player, int player_num);

bool check_if_change_DIR_card(int playerChoice, PLAYER* player, int player_num);

bool check_if_TAKI_card(int playerChoice, PLAYER* player, int player_num);

bool check_if_number_card(int playerChoice, PLAYER* player, int player_num);

void switchUpperCard(CARD* upper_card, PLAYER* player, int player_choice, int player_num);

void main()
{
	srand(time(NULL));
	printf("********** Welcome to TAKI game !!! **************\n");
	PLAYER* players_of_taki = NULL; 
	int number_of_players;
	CARD upper_card;// the current card being placed 
	bool game_over = false; // if a player win true. (end the game)
	STATS cards_stats[15] = {{"NOTYPE", 0},{"1", 0},{"2", 0},{"3", 0},{"4", 0},{"5", 0},{"6", 0},{"7", 0},{"8", 0},{"9", 0},{"PLUS", 0},{"STOP", 0},{"<->", 0},{"TAKI", 0},{"COLOR", 0}};
	int current_player=0;
	change_upper_card(&upper_card);// first random upper card.

	//main game loop varibles//
	int i=0; // i loops till the last player;
	int playerChoice;
	bool taki_choice = false; // taki card while loop
	int direction_of_turn = 1;// direction card
	 players_of_taki = get_number_of_players(players_of_taki,&number_of_players);	// pointer to the array of all of the players data 
     get_players_names(players_of_taki,number_of_players);
	 first_dealing_of_cards(players_of_taki, number_of_players,cards_stats);

	 while (!game_over) // play until someone wins (:
	 {
		 while (i < number_of_players && i > -1) // i>-1 is for the direction card
		 {
			 current_player = i;// this is so i can make changes to i and still have the current player number.
			 if (!taki_choice)
			 { 
				 player_turn(&playerChoice, players_of_taki, &i, &upper_card,&taki_choice,cards_stats);// almost all of the cards. play a card or take a card.
				 if (check_if_TAKI_card(playerChoice, players_of_taki, i))
				 {
					 taki_choice = true;
					 i -= direction_of_turn; // stay on current player
				 }
				 else if (check_if_STOP_card(playerChoice, players_of_taki, i))// stop card
				 {
					 if (number_of_players == 2)
					 {
						 i -= direction_of_turn;
					 }
					 else
						 i += direction_of_turn; // jump one player forward

					 if (number_of_players == 2 && players_of_taki[current_player].cards_in_hand)
						 give_a_card(players_of_taki, i, 0,cards_stats);
				 }
				 else if (check_if_PLUS_card(playerChoice,players_of_taki,i)) // plus card 
				 {
					 i -= direction_of_turn; // stay on current player
					 if (players_of_taki[current_player].cards_in_hand == 0)
						 give_a_card(players_of_taki, i, 0,cards_stats); // if the last card is plus give another card to the player	 
				 }
				 else if (check_if_change_DIR_card(playerChoice, players_of_taki, i))  //direaction card 
				 {
					 direction_of_turn *= -1;
				 }
				 else if (check_if_COLOR_card(playerChoice, players_of_taki, i))
				 {
					 use_color_card(players_of_taki, i, playerChoice);
				 }
			 }
			 else // still in taki rain
			 {
				 player_turn(&playerChoice, players_of_taki, &i, &upper_card,&taki_choice,cards_stats);
				 if (check_if_COLOR_card(playerChoice, players_of_taki, i))
				 {
					 use_color_card(players_of_taki,i,playerChoice);
					 taki_choice = false;
				 }
				 if(taki_choice==true)
					 i -= direction_of_turn; // stay on current player
			 }
			 if (playerChoice!= -1)  // if the player player a card 
			 {
				 switchUpperCard(&upper_card,players_of_taki,playerChoice,current_player);
				 players_of_taki[current_player].cards_in_hand--;
			 }
			 i += direction_of_turn;
		 }
		 if (players_of_taki[current_player].cards_in_hand == 0) // if  the current player won.
		 {
			 game_over = true;
			 printf("\nthe winner is....player %d! Congratualtions!\n", i);
			 print_game_statistic(cards_stats);
			 free_player_arrays(players_of_taki, number_of_players);

		 }
		 if (direction_of_turn == 1)   // if we reach the last player.(depends on the direcation)
			 i = 0;
		 else
			 i = number_of_players - 1;
	 }
}

void malloc_dynamic_cards(PLAYER* player, int num_of_players) 
{
	int i;
	for ( i = 0; i < num_of_players; i++)
	{
		player[i].cards = (CARD*)malloc(BASE_NUM_OF_CARDS * sizeof(CARD));
		if (player[i].cards == NULL)
			exit(1);
	}
}
PLAYER* get_number_of_players(PLAYER *players_of_taki,int *number_of_players) 
{ 
	printf("Please enter the number of players:\n ");  
	scanf("%d",number_of_players);

	while (*number_of_players < 1)  // checks if the user give a correct input
	{
		printf("Error: Invalid input. Please enter a positive integer.\n");
		printf("Please enter the number of players:\n ");
		scanf("%d",number_of_players);
	}

	players_of_taki = (PLAYER*)malloc(*number_of_players * sizeof(PLAYER));// if the user gave a correct input allocate place in heap as the number of players.
	if (players_of_taki == NULL)
		exit(1);
	malloc_dynamic_cards(players_of_taki, *number_of_players); // aloocating each player a dynamic array of cards.

	return players_of_taki; // returns the pointer value created in the heap
}

void single_player_name(PLAYER* player, int player_number)
{
	printf("please enter the first name of player #%d: ", player_number + 1); // the loop in get_players_names start from zero thats why +1.
	scanf("%s", player[player_number].name_of_player);
	while (strlen(player[player_number].name_of_player) > 20)
	{
		printf("Error: name is too long! please enter a shorter name.\n");
		printf("please enter the first name of player #%d: ", player_number + 1); // the loop in get_players_names start from zero thats why +1.
		scanf("%s", player[player_number].name_of_player);

	}
}

 void get_players_names(PLAYER *player,int number_of_players)
 {
	 int i;
	 for (i = 0;i < number_of_players;i++)  // this loop use the single player name function to get each player name 
	 {
		 single_player_name(player, i);
	 }
 }

 void first_dealing_of_cards(PLAYER* player,int number_of_players,STATS arr[])
 {
	 int i, j;
int dealing_4_cards = 4;
for (i = 0;i < number_of_players;i++) 
{
	player[i].cards_in_hand = 0; 
	for (j = 0;j < dealing_4_cards;j++)
	{
		give_a_card(player, i, j,arr);
	}
	player[i].max_cards = 4; //physical size
}
 }

 void give_a_card(PLAYER* player, int player_num, int card_index,STATS arr[])
 {
	 NoSpaceForCards(player, player_num); // allocate more space in array if needed.

	 int random_card_type;
	 random_card_type = 1 + rand() % 14; // random numbers between 1 and 14
	 arr[random_card_type].count++;
	 if (random_card_type < 10) // if the cards is 1-9 put the char '1' for example as a card type .
	 {
		 player[player_num].cards[card_index].card_type[0] = ZERO_ASCI + random_card_type;
		 player[player_num].cards[card_index].card_type[1] = '\0';  // because the card type is an array of chars i need to end it with a \0.
	 }
	 else
	 {
		 switch (random_card_type)
		 {
		 case PLUS_CARD:
			 strcpy(player[player_num].cards[card_index].card_type, "+");
			 break;
		 case STOP_CARD:
			 strcpy(player[player_num].cards[card_index].card_type, "STOP");
			 break;
		 case CHANGE_DIRECTION_CARD:
			 strcpy(player[player_num].cards[card_index].card_type, "<->");
			 break;
		 case TAKI_CARD:
			 strcpy(player[player_num].cards[card_index].card_type, "TAKI");
			 break;
		 case CHANGE_COLOR_CARD:
			 strcpy(player[player_num].cards[card_index].card_type, "COLOR");
		 }
	 }
	 int random_card_color = 1 + rand() % 4; // each number will get a represntion of color

	 if (strcmp(player[player_num].cards[card_index].card_type, "COLOR") == 0)
		 {
		 // the change color card has no color
		 player[player_num].cards[card_index].color = ' ';
		 }
	 else
		 switch (random_card_color)
		 {
		 case RED:
			 player[player_num].cards[card_index].color = 'R';
			 break;
		 case YELLOW:
			 player[player_num].cards[card_index].color = 'Y';
			 break;
		 case BLUE:
			 player[player_num].cards[card_index].color = 'B';
			 break;
		 case GREEN:
			 player[player_num].cards[card_index].color = 'G';
			 break;
		 }
	 player[player_num].cards_in_hand++; // logical size of cards array
 }

 void print_single_card(CARD* card) {
	 int i, j;
	 int const card_width = 9, card_length = 5;
	 printf("\n ******** ");
	 for (i = 0; i < card_length; i++) {
		 if (i == 2) {
			 print_card_type_line(card);
			 continue;
		 }
		 else {
			 printf("%s", "\n*        *");
		 }
	 }
	 printf("\n ******** \n");
 }

 void print_card_type_line(CARD* card) {
	 if (card->card_type[1] == '\0') {
		 printf("\n*   %c    *", card->card_type[0]);
		 printf("\n*   %c    *", card->color);
	 }
	 else if (strcmp(card->card_type, "STOP") == 0) {
		 printf("\n*  %s  *", card->card_type);
		 printf("\n*   %c    *", card->color);
	 }
	 else if (strcmp(card->card_type, "TAKI")==0)
	 {
	 printf("\n*  %s  *", card->card_type);
	 printf("\n*   %c    *", card->color);
	 }
	 else if (card->card_type == "+")
	 {
		 printf("\n*   %s   *", card->card_type);
		 printf("\n*   %c   *", card->color);
	 }
	 else if (strcmp(card->card_type, "<->")==0)
	 {
		 printf("\n*  %s   *", card->card_type);
		 printf("\n*   %c    *", card->color);
	 }
	 else if ((strcmp(card->card_type, "COLOR") == 0))
	 {
		 printf("\n*  %s *", card->card_type);
		 printf("\n*   %c    *", card->color);
	 }
 }

 void use_color_card(PLAYER* player, int player_num, int card_num)
 {
	 int color_choice;
	 printf("\nPlease enter youe color choice: ");
	 printf("\n1 - Yellow");
	 printf("\n2 - Red");
	 printf("\n3 - Blue");
	 printf("\n4 - Green");
	 scanf("%d",&color_choice);

	 // valid input check
	 while (color_choice != 1 && color_choice != 2 && color_choice != 3 && color_choice != 4)  
	 {
		 printf("\nplease pick a Valid color:");
		 printf("\n1 - Yellow");
		 printf("\n2 - Red");
		 printf("\n3 - Blue");
		 printf("\n4 - Green");
		 scanf("%d", color_choice);
	 }
	 switch (color_choice)
	 {
	 case 1:
		 player[player_num].cards[card_num].color = 'Y';
		 break;
	 case 2:
		 player[player_num].cards[card_num].color = 'R';
		 break;
	 case 3:
		 player[player_num].cards[card_num].color = 'B';
		 break;
	 case 4:
		 player[player_num].cards[card_num].color = 'G';
		 break;
	 default:
		 break;
	 }
 }

 void print_current_cards_in_hand(PLAYER* player,int player_num)
 {
	 int i, j;
	 for (i = 0;i < player[player_num].cards_in_hand;i++)
	 {
		 printf("Card #%d:\n", i + 1);
		 print_single_card(player[player_num].cards + i);
	 }
 }

 int CheckValidRoundChoice(PLAYER *player, int player_num,bool taki_card)
 {
	 int player_choice;
	 if (taki_card)
	 {
		 printf("Please enter 0 if you want to finish your turn\n");
	 }
	 else
		 printf("Please enter 0 if you want to take a card from the deck or 1-%d if you want to put one of your cards in the middle:", player[player_num].cards_in_hand);

	 scanf("%d", &player_choice);
	 while (player_choice > player[player_num].cards_in_hand)
	 {
		 printf("\n Invalid choice try again.\n");
		 if(taki_card)
			 printf("Please enter 0 if you want to finish your turn\n");
		 else
			 printf("Please enter 0 if you want to take a card from the deck or 1-%d if you want to put one of your cards in the middle:", player[player_num].cards_in_hand);
		 scanf("%d", &player_choice);
	 }
	 return player_choice-1;
 }

 void NoSpaceForCards(PLAYER* player, int player_num)
 {
	 CARD* new_cards = NULL;
	 if (player[player_num].cards_in_hand == player[player_num].max_cards)
	 {
		 new_cards = (CARD*)(realloc(player[player_num].cards, 2 * player[player_num].max_cards * sizeof(CARD)));// GIVING THE LOCAL POINTER A SPACE IN THE HEAP 2 TIMES THE CARDS MAX SIZE
		 if (new_cards != NULL)
		 {
			 player[player_num].cards = new_cards;
			 player[player_num].max_cards = 2 * player[player_num].max_cards;
		 }
		 else
		 {
			 printf("\nError: Failed to reallocate memory for player %d's cards.", player_num);
			 free(new_cards);
			 exit(1);
		 }
	 }
 }

 void change_upper_card(CARD* card)
 {

	 int random_card_type = 1 + rand() % 9; // random numbers between 1 and 9 
		 card->card_type[0] = ZERO_ASCI + random_card_type;
		 card->card_type[1] = '\0';// because the card type is an array of chars i need to end it with a \0.

		 int random_card_color = 1 + rand() % 4; // each number will get a represntion of color
			 switch (random_card_color)
			 {
			 case RED:
				 card->color = 'R';
				 break;
			 case YELLOW:
				 card->color = 'Y';
				 break;
			 case BLUE:
				 card->color= 'B';
				 break;
			 case GREEN:
				 card->color = 'G';
				 break;
			 }
	 }

 int check_player_turn_choice(CARD* Upper_card, PLAYER* player, int player_num,bool taki_card)
 {
	 bool ValidChoice = false;
	 int cardChoice;
	 while (!ValidChoice) // keep asking the player the choose a card that can be played
	 {
		 cardChoice = CheckValidRoundChoice(player, player_num,taki_card); // check if valid choice(not checking the card it self)
		 if (cardChoice == -1)
		 {
			 ValidChoice = true;
			 return -1;
		 }
		 else if(strcmp(player[player_num].cards[cardChoice].card_type, Upper_card->card_type) == 0 || player[player_num].cards[cardChoice].color == Upper_card->color)// ive put -1 be cause the cards count start from 1 but the cards in the array start from 0 
			 ValidChoice = true;
		 else if (strcmp(player[player_num].cards[cardChoice].card_type ,"COLOR")==0)
			 ValidChoice = true;
		 else
		 {
			 printf("\nInvalid card try again\n");
		 }

	 }
	 return cardChoice;
 }

 void switchUpperCard(CARD* upper_card, PLAYER* player, int player_choice,int player_num)
 {
	 int i; 
	 strcpy(upper_card->card_type,player[player_num].cards[player_choice].card_type); // changing the upper card to the player choice (player choice plus one brecause 
	 upper_card->color = player[player_num].cards[player_choice].color;
	 for ( i = player_choice; i < player->cards_in_hand; i++) // this while loop take every card 1 index back in the array (from the player chosen card forward)
	 {
		 player[player_num].cards[i].color = player[player_num].cards[i+1].color; 
		 strcpy(player[player_num].cards[i].card_type ,player[player_num].cards[i+1].card_type);
	 }
 }
 
 bool check_if_STOP_card(int playerChoice, PLAYER* player, int player_num) 
 {
	 if (strcmp(player[player_num].cards[playerChoice].card_type, "STOP") == 0)
		 return true;
	 else
		 return false;
 }
 bool check_if_change_DIR_card(int playerChoice, PLAYER* player, int player_num)
 {
	 if (strcmp(player[player_num].cards[playerChoice].card_type, "<->") == 0)
		 return true;
	 else
		 return false;
 }
 bool check_if_COLOR_card(int playerChoice, PLAYER* player, int player_num)
 {
	 if (strcmp(player[player_num].cards[playerChoice].card_type, "COLOR") == 0)
		 return true;
	 else
		 return false;
 }
 bool check_if_number_card(int playerChoice, PLAYER* player, int player_num)
 {
	 if (player[player_num].cards[playerChoice].card_type[1] == '\0'&& player[player_num].cards[playerChoice].card_type[0] !='+')
		 return true;
	 else
		 return false;
 }
 bool check_if_TAKI_card(int playerChoice, PLAYER* player, int player_num) 
 {
	 if (strcmp(player[player_num].cards[playerChoice].card_type, "TAKI") == 0)
		 return true;
	 else
		 return false;
 }
 bool check_if_PLUS_card(int playerChoice, PLAYER* player, int player_num) 
 {
	 if (strcmp(player[player_num].cards[playerChoice].card_type, "+") == 0)
		 return true;
	 else
		 return false;
 }

 void player_turn(int* playerChoice, PLAYER* players_of_taki, int* i,CARD* upper_card,bool *taki_card,STATS arr[])
 {
	 printf("Upper card: ");
	 print_single_card(upper_card); // printing UPPER_CARD
	 printf("\nplayer %s's turn:\n", players_of_taki[*i].name_of_player);
	 print_current_cards_in_hand(players_of_taki, *i);
	 *playerChoice = check_player_turn_choice(upper_card, players_of_taki, *i,*taki_card);	 

		 if (*playerChoice == TAKE_CARD&&!*taki_card)
			 give_a_card(players_of_taki, *i, players_of_taki[*i].cards_in_hand,arr);

		 if (*playerChoice == TAKE_CARD && *taki_card)
			 *taki_card = false;

 }
 
void game_statistic_calcualtor(STATS arr[])
 {
	int i, j;
	STATS temp;
	for ( i = 0; i < CARDS_TYPE; i++)
		for ( j = 0; j < CARDS_TYPE-i; j++)
		{
			if (arr[j].count > arr[j+1].count) 
			{
				temp = arr[j + 1];
				arr[j + 1] = arr[j];
				arr[j] = temp;
			}
		}
 }
 
void print_game_statistic(STATS arr[])
{
	int i;
	game_statistic_calcualtor(arr);
	printf("************ Game Statistic ***************\n");
	printf("Card # | Frequency\n");
	printf("__________________\n");
	for (int i = CARDS_TYPE;i>0; i--) {
		printf("%s\t|%d\n", arr[i].card_type, arr[i].count);
	}
}

void free_player_arrays(PLAYER* player,int num)
{
	int i;
	for ( i = 0; i < num; i++)
	{
		free(player[i].cards);
	}
	free(player);
}
	 


 

