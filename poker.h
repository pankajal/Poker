#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include<string.h>
#include <pthread.h>
#include<signal.h>

#define SPADE    3
#define HEART    2 
#define CLUB     1 
#define DIAMOND  0 

#define TYPEVALUE 3200000
#define VALUE1     160000 
#define VALUE2       8000 
#define VALUE3        400 
#define VALUE4         20  
#define VALUE5          1

#define getvalue1(score) (((int)(score%TYPEVALUE))/VALUE1)
#define getvalue2(score) (((int)(score%VALUE1))/VALUE2)
#define getvalue3(score) (((int)(score%VALUE2))/VALUE3)
#define getvalue4(score) (((int)(score%VALUE3))/VALUE4)
#define getvalue5(score) (((int)(score%VALUE4))/VALUE5)

#define ROYAL_FLUSH     (10 * TYPEVALUE)
#define STRAIGHT_FLUSH  ( 9 * TYPEVALUE)
#define FOUR_OF_A_KIND  ( 8 * TYPEVALUE)
#define FULL_HOUSE      ( 7 * TYPEVALUE)
#define FLUSH           ( 6 * TYPEVALUE)
#define STRAIGHT        ( 5 * TYPEVALUE)
#define THREE_OF_A_KIND ( 4 * TYPEVALUE)
#define TWO_PAIR        ( 3 * TYPEVALUE)
#define PAIR            ( 2 * TYPEVALUE)
#define HIGH_CARD       ( 1 * TYPEVALUE)
#define EMPTY               -1

#define NUM_PLAYERS   6
#define NUM_HANDS   100 
#define BET_MONEY 10000

#define RAND(lowest,highest) (((rand()>>6)%(highest-lowest+1))+lowest)
#define NUM_TABLES 10

static pthread_mutex_t m[NUM_PLAYERS],m1;
static pthread_cond_t c[NUM_PLAYERS],c1;


struct SingleCard {
   short suit; 
   short value;  /* Jack = 11, Queen = 12, King = 13, Ace = 14 */ 
}; 

struct PlayerHand {
   struct SingleCard card[2]; 
}; 

struct SevenCardHand {
   struct SingleCard card[7]; 
}; 

struct CommunityCards {
   struct SingleCard card[5];
}; 

struct DeckOfCards {
   struct SingleCard card[52];
   int nextCard; 
}; 

struct AllHands {
   struct SevenCardHand sevenCardHand;
   int score;
};

struct TableOfPlayers {
   struct PlayerHand hand[NUM_PLAYERS];
}; 

struct Player
{
	long money;
	int out;
	int fold;
	int flag;
	int Id;
	int some_cond;
	pthread_mutex_t m;
	pthread_cond_t c;
	struct Table *tableB;
	int controlFlg;
};

struct Table
{
	int cou;
	long Minimum;
	long pot_money;
	int flag[NUM_PLAYERS];
	int round;
	int winner;
	int count1;
	int last_fold;
	int TableId;
	struct Player *player[NUM_PLAYERS];
	int sblind;
	int Bblind;
	pthread_mutex_t m1;
	pthread_cond_t c1;
	pthread_mutex_t Mutex;
	int controlFlg;
};

struct Queue
{
	struct Player *p[10];
	int index;
	pthread_mutex_t m3;
};

struct Queue queue;


//int some_cond = 0;
		

struct Table tables[NUM_TABLES];
int tableIndex[NUM_TABLES];

void PlayPoker(int);
int freePos (struct Table *table1);
int flop(struct CommunityCards *community, struct DeckOfCards *deck) ;
int turn(struct CommunityCards *community, struct DeckOfCards *deck) ;
int river(struct CommunityCards *community, struct DeckOfCards *deck) ;
int dealHands(struct TableOfPlayers *table, struct DeckOfCards *deck);
int showPlayerHand(struct PlayerHand *hand);
int showSevenCardHand(struct SevenCardHand *hand);
int sortSevenCardHand(struct SevenCardHand *hand);
int showTable(struct TableOfPlayers *table);
int royalflush( struct SevenCardHand *hand);
int straightflush( struct SevenCardHand *hand);
int fourofakind( struct SevenCardHand *hand);
int fullhouse(struct SevenCardHand *hand);
int flush(struct SevenCardHand *hand);
int straight( struct SevenCardHand *hand);   
int threeofakind( struct SevenCardHand *hand);
int twopair( struct SevenCardHand *hand);
int pair(struct SevenCardHand *hand);  
int highcard( struct SevenCardHand *hand);
int makeSevenCardHand(struct PlayerHand *hand,
                struct CommunityCards *community,
                struct SevenCardHand *sevenCardHand);
int getValue1(int score) ;
int describe(int score) ;
int evaluateHand(struct SevenCardHand *sevenCardHand) ;
int sortAllHands(struct AllHands *allHands, int handcount) ;
int showAllHands(struct AllHands *allHands, int handcount) ;

