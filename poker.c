#include "poker.h"
int initializeDeck(struct DeckOfCards *deck) {
   int i;
   for (i=0; i<52; i++) {
      if (i%4 == HEART) deck->card[i].suit = HEART; 
         else if (i%4 == SPADE) deck->card[i].suit = SPADE; 
            else if (i%4 == DIAMOND) deck->card[i].suit = DIAMOND; 
               else if (i%4 == CLUB) deck->card[i].suit = CLUB; 
      deck->card[i].value = ((i / 4) + 2); 
   }
   deck->nextCard = 0;
   return(1);  
}

int shuffleDeck(struct DeckOfCards *deck) {
   int i, j, swapA, swapB;
   struct SingleCard tmp;
   for (j = 0; j < 6; j++) {
      for (i=0; i<52; i++) {
         swapA = RAND(0, 51);
	 swapB = RAND(0, 51);
         tmp = deck->card[swapA];
         deck->card[swapA] = deck->card[swapB];
         deck->card[swapB] = tmp;
      }
   }
   return(1);  
}

int showCardValue(short value) {
   if (value == EMPTY) {
      fprintf(stdout, "  X");
      return(0);
   }
   else if (value == 11)
      fprintf(stdout, "  J");
   else if (value == 12)
      fprintf(stdout, "  Q");
   else if (value == 13)
      fprintf(stdout, "  K");
   else if (value == 14)
      fprintf(stdout, "  A");
   else fprintf(stdout, "%3d", value);
}

int showCard(struct SingleCard *card) {

   showCardValue(card->value); 

   if (card->suit == SPADE)
      fprintf(stdout, "S");
   else if (card->suit == HEART)
      fprintf(stdout, "H");
   else if (card->suit == CLUB)
      fprintf(stdout, "C");
   else if (card->suit == DIAMOND)
      fprintf(stdout, "D");
   else fprintf(stdout, "X");
   fprintf(stdout, " ");
}

int showDeck(struct DeckOfCards *deck) {
   int i;
   fprintf(stdout, "--- DECK ---\n"); 
   for (i=0; i<52; i++) {
      showCard(&(deck->card[i]));
      if (!(i%10)) fprintf(stdout, "\n");
   }
   fprintf(stdout, "\n");
   return(1);  
}

int showCommunityCards(struct CommunityCards *community)
{
   int i;

   fprintf(stdout, "--- COMMUNITY CARDS: ");
   showCard(&(community->card[0]));
   showCard(&(community->card[1]));
   showCard(&(community->card[2]));
   showCard(&(community->card[3]));
   showCard(&(community->card[4]));
   fprintf(stdout, "\n");
}

struct SingleCard getNextCard(struct DeckOfCards *deck) 
{
   struct SingleCard returnedCard; 

   if (deck->nextCard >= 52) {
      returnedCard.value = EMPTY;    
      returnedCard.suit = EMPTY;    
   }
   else {
      returnedCard = deck->card[deck->nextCard]; 
      deck->nextCard++;
   }
   return(returnedCard); 
}

int flop(struct CommunityCards *community, struct DeckOfCards *deck) 
{
   struct SingleCard emptyCard; 
   emptyCard.value = EMPTY;    
   emptyCard.suit = EMPTY;    

   community->card[0] = getNextCard(deck); 
   community->card[1] = getNextCard(deck); 
   community->card[2] = getNextCard(deck); 
   community->card[3] = emptyCard; 
   community->card[4] = emptyCard; 
}

int turn(struct CommunityCards *community, struct DeckOfCards *deck) 
{
   community->card[3] = getNextCard(deck); 
}

int river(struct CommunityCards *community, struct DeckOfCards *deck) 
{
   community->card[4] = getNextCard(deck);
}

int dealHands(struct TableOfPlayers *table, struct DeckOfCards *deck)
{
   int i;
   for (i=0; i<NUM_PLAYERS; i++) {
      table->hand[i].card[0] = getNextCard(deck);
      table->hand[i].card[1] = getNextCard(deck);
   }
}

int showPlayerHand(struct PlayerHand *hand)
{
   fprintf(stdout, "--- PLAYER CARDS: ");
   showCard(&(hand->card[0]));	 
   showCard(&(hand->card[1]));	 
   fprintf(stdout, "\n");
   return(1); 
}

int showSevenCardHand(struct SevenCardHand *hand)
{
   int i;
   for (i=0; i<7; i++)
      showCard(&(hand->card[i]));	 
   return(1); 
}

int sortSevenCardHand(struct SevenCardHand *hand)
{
   int i, j;
   struct SingleCard tmp;
   /* Quick and dirty Bubble Sort */
   for (i=0; i<6; i++) 
      for (j=i+1; j<7; j++) 
         if (hand->card[j].value > hand->card[i].value) {
            tmp = hand->card[j]; 
            hand->card[j] = hand->card[i]; 
            hand->card[i] = tmp; 
         }
   return(1);
}

int showTable(struct TableOfPlayers *table)
{
   int i;
   fprintf(stdout, "--- Table of Players ---\n");
   for (i=0; i<NUM_PLAYERS; i++) {
      fprintf(stdout, "Player %d's Hand:\n", i);
      showPlayerHand(&(table->hand[i]));
   }
   return(1); 
}


int fourofakind( struct SevenCardHand *hand)
{
   int i;
   int four = 0;
   int kicker = 0;
   for (i=3; i<7; i++) {
      if ((hand->card[i].value == hand->card[i-1].value) && 
          (hand->card[i].value == hand->card[i-2].value) &&
          (hand->card[i].value == hand->card[i-3].value)) {
	 four = hand->card[i].value;  
	 break;
      }
   }
   if (four) {
      if (hand->card[0].value != four)
         kicker = hand->card[0].value; 
      else
         kicker = hand->card[4].value; 
      return (FOUR_OF_A_KIND + four * VALUE1 + kicker * VALUE2); 
   }
   return (0);
}

int fullhouse(struct SevenCardHand *hand)
{
   int i; 
   int triple = 0; 
   int pair = 0; 

   for (i=2; i<7; i++) {
      if ((hand->card[i].value == hand->card[i-1].value) && 
          (hand->card[i].value == hand->card[i-2].value))
         triple = hand->card[i].value; 
   }
   for (i=1; i<7; i++) {
      if ((hand->card[i].value == hand->card[i-1].value) && 
          (hand->card[i].value != triple))
         pair = hand->card[i].value; 
   }
   if (triple && pair) 
      return (FULL_HOUSE + (triple * VALUE1) + pair * VALUE2); 
   else 
      return (0);
}

int flush(struct SevenCardHand *hand)
{
   int num_clubs = 0;
   int num_hearts = 0;
   int num_diamonds = 0;
   int num_spades = 0;
   int suit = -1;
   int cardvals[5]; 
   int i;
   int j; 

   for (i=0; i<7; i++) {
      if (hand->card[i].suit == DIAMOND) num_diamonds++;
      else if (hand->card[i].suit == HEART) num_hearts++;
      else if (hand->card[i].suit == CLUB) num_clubs++;
      else if (hand->card[i].suit == SPADE) num_spades++;
   }
   if (num_spades >= 5) 
      suit = SPADE; 
   else if (num_hearts >= 5) 
      suit = HEART; 
   else if (num_diamonds >= 5)
      suit = DIAMOND; 
    else if (num_clubs >= 5)
      suit = FLUSH;  

   if (suit != -1) { 
      j = 0; 
      for (i=0; i<7; i++) {
         if (hand->card[i].suit == suit) {
            cardvals[j++] = hand->card[i].value; 
	    if (j>5) break;
         } 
      } 
      return(FLUSH + cardvals[0] * VALUE1 + cardvals[1] * VALUE2 + 
                     cardvals[2] * VALUE3 + cardvals[3] * VALUE4 + 
                     cardvals[4] * VALUE5);  
   }
   else {
      return(0);
   }
}

int straight( struct SevenCardHand *hand)
{
   int i, count, current, start; 
   short highcard;
   // Look for straight starting at card 0
   highcard = current = hand->card[0].value;
   count = 1; 
   for (i=1; i<7; i++) {
      if (hand->card[i].value == (current-1)) {
	 if (++count == 5)
            return (STRAIGHT + highcard * VALUE1); 
	 else {
            current--; 
	 }
      }
      else {
         if (hand->card[i].value != current) {
            count = 1; 
            highcard = current = hand->card[i].value;
         }
      }
   }
   return(0); 
}

int threeofakind( struct SevenCardHand *hand)
{
   int i;
   int three = 0;
   int kicker1 = 0; 
   int kicker2 = 0; 

   for (i=2; i<7; i++) {
      if ((hand->card[i].value == hand->card[i-1].value) && 
          (hand->card[i].value == hand->card[i-2].value))
	      three = hand->card[i].value; 
   }
   if (three) {
      for (i=0; i<7; i++) {
         if (hand->card[i].value != three) {
             kicker1 = hand->card[i].value; 
             break;
         }
      }
      for (i=0; i<7; i++) {
         if ((hand->card[i].value != three) &&
             (hand->card[i].value != kicker1)) {
             kicker2 = hand->card[i].value; 
             break;
         }
      }
      return (THREE_OF_A_KIND + three * VALUE1 + kicker1 * VALUE2 + 
		   kicker2 * VALUE3 ); 
   }
   else 
      return (0);
}

int twopair( struct SevenCardHand *hand)
{
   int i; 
   int firstpair = 0; 
   int secondpair = 0; 
   int kicker = 0; 

   for (i=1; i<7; i++) {
      if ((hand->card[i].value == hand->card[i-1].value)) {
         firstpair = hand->card[i].value; 
	 break;
      }
   }
   if (firstpair) {
      for (i=i; i<7; i++) {
         if ((hand->card[i].value == hand->card[i-1].value) && 
             (hand->card[i].value != firstpair)) {
            secondpair = hand->card[i].value; 
	    break;
         }
      }
      if (secondpair) {
	 for (i=0; i<7; i++) {
            if ((hand->card[i].value != firstpair) &&
                (hand->card[i].value != secondpair)) {
               kicker = hand->card[i].value; 
	       break;
            }
         }
         if (firstpair && secondpair) 
            return (TWO_PAIR + (firstpair * VALUE1) + (secondpair * VALUE2) +
		      (kicker * VALUE3)); 
      }
   }
   return (0);
}

int pair(struct SevenCardHand *hand)
{
   int i;
   int rval = 0; 
   int pair = 0;
   int kicker1 = 0; 
   int kicker2 = 0; 
   int kicker3 = 0; 

   for (i=1; i<7; i++) {
      if ((hand->card[i].value == hand->card[i-1].value)) {
         pair = (hand->card[i].value); 
	 break;
      }
   }
   if (pair) {
      for (i=0; i<7; i++) {
         if (hand->card[i].value != pair) {
            kicker1 = hand->card[i].value;
	    break;
         }
      }
      for (i=1; i<7; i++) {
         if ((hand->card[i].value != pair) && 
             (hand->card[i].value != kicker1)) {
            kicker2 = hand->card[i].value;
	    break;
         }
      }
      for (i=2; i<7; i++) {
         if ((hand->card[i].value != pair) && 
             (hand->card[i].value != kicker1) &&
             (hand->card[i].value != kicker2)) {
            kicker3 = hand->card[i].value;
	    break;
         }
      }
      return(PAIR + (pair * VALUE1) + (kicker1 * VALUE2) + 
		  (kicker2 * VALUE3) + kicker3 * VALUE4); 
   }
   else
      return(0);
}

int highcard( struct SevenCardHand *hand)
{
   return(HIGH_CARD + 
          hand->card[0].value * VALUE1 + 
          hand->card[1].value * VALUE2 + 
          hand->card[2].value * VALUE3 + 
          hand->card[3].value * VALUE4 + 
          hand->card[4].value * VALUE5);
}

int makeSevenCardHand(struct PlayerHand *hand, 
		struct CommunityCards *community, 
		struct SevenCardHand *sevenCardHand)
{
   int i;
   short cardValue = EMPTY;

   for (i=0; i<5; i++) 
      sevenCardHand->card[i] = community->card[i]; 
   sevenCardHand->card[5] = hand->card[0]; 
   sevenCardHand->card[6] = hand->card[1]; 

   return(1);
}

int getValue1(int score) 
{
   score %= TYPEVALUE; 

}

int describe(int score) 
{
   int handtype; 
   handtype = ((int) (score / TYPEVALUE)) * TYPEVALUE; 
   switch (handtype) {
	   case ROYAL_FLUSH     : 
		   fprintf(stdout, "Royal Flush ");
		   break;
	   case STRAIGHT_FLUSH  : 
		   showCardValue(getvalue1(score)); 
		   fprintf(stdout, " High Straight Flush ");
		   break;
	   case FOUR_OF_A_KIND  : 
		   fprintf(stdout, "Four ");
		   showCardValue(getvalue1(score)); 
		   fprintf(stdout, "'s with a ");
		   showCardValue(getvalue2(score)); 
		   fprintf(stdout, " kicker");
		   break;
	   case FULL_HOUSE      : 
		   fprintf(stdout, "Full House : ");
		   showCardValue(getvalue1(score)); 
		   fprintf(stdout, "'s over ");
		   showCardValue(getvalue2(score)); 
		   fprintf(stdout, "'s");
		   break;
	   case FLUSH           : 
		   showCardValue(getvalue1(score)); 
		   fprintf(stdout, " High Flush ");
		   fprintf(stdout, " (with ");
		   showCardValue(getvalue2(score)); 
		   showCardValue(getvalue3(score)); 
		   showCardValue(getvalue4(score)); 
		   showCardValue(getvalue5(score)); 
		   fprintf(stdout, ")");
		   break;
	   case STRAIGHT        : 
		   showCardValue(getvalue1(score)); 
		   fprintf(stdout, " High Straight ");
		   break;
	   case THREE_OF_A_KIND : 
		   fprintf(stdout, "Three ");
		   showCardValue(getvalue1(score)); 
		   fprintf(stdout, "'s : ");
		   fprintf(stdout, " (with ");
		   showCardValue(getvalue2(score)); 
		   showCardValue(getvalue3(score)); 
		   fprintf(stdout, ")");
		   break;
	   case TWO_PAIR        : 
		   fprintf(stdout, "Two Pair : ");
		   showCardValue(getvalue1(score)); 
		   fprintf(stdout, "'s and ");
		   showCardValue(getvalue2(score)); 
		   fprintf(stdout, "'s with a ");
		   showCardValue(getvalue3(score)); 
		   fprintf(stdout, " kicker");
		   break;
	   case PAIR            : 
		   fprintf(stdout, "One Pair of ");
		   showCardValue(getvalue1(score)); 
		   fprintf(stdout, "'s ");
		   fprintf(stdout, " (with ");
		   showCardValue(getvalue2(score)); 
		   showCardValue(getvalue3(score)); 
		   showCardValue(getvalue4(score)); 
		   fprintf(stdout, ")");
		   break;
	   case HIGH_CARD       : 
		   fprintf(stdout, "High Card : ");
		   showCardValue(getvalue1(score)); 
		   fprintf(stdout, " (with ");
		   showCardValue(getvalue2(score)); 
		   showCardValue(getvalue3(score)); 
		   showCardValue(getvalue4(score)); 
		   showCardValue(getvalue5(score)); 
		   fprintf(stdout, ")");
		   break;
   }
   return (1); 
}

int evaluateHand(struct SevenCardHand *sevenCardHand) 
{
   int score = 0; 
   
   sortSevenCardHand(sevenCardHand);

   /* Check for Royal Flush */
   /* Check for Straight Flush */

   if ((score = royalflush(sevenCardHand)) > 0) 
	   return(score);
   if ((score = straightflush(sevenCardHand)) > 0) 
	   return(score);
   if ((score = fourofakind(sevenCardHand)) > 0) 
	   return(score);
   if ((score = fullhouse(sevenCardHand)) > 0) 
	   return(score);
   if ((score = flush(sevenCardHand)) > 0) 
	   return(score);
   if ((score = straight(sevenCardHand)) > 0) 
	   return(score);
   if ((score = threeofakind(sevenCardHand)) > 0) 
	   return(score);
   if ((score = twopair(sevenCardHand)) > 0)
	   return(score);
   if ((score = pair(sevenCardHand)) > 0) 
	   return(score);
   if ((score = highcard(sevenCardHand)) > 0)
	   return(score);
}

int sortAllHands(struct AllHands *allHands, int handcount) 
{
   int i, j;
   struct AllHands tmp;
   /* Quick and dirty Bubble Sort */
   for (i=0; i<handcount-1; i++) {
      for (j=i+1; j<handcount; j++) {
         if (allHands[j].score > allHands[i].score) {
            tmp = allHands[j]; 
            allHands[j] = allHands[i]; 
            allHands[i] = tmp; 
         }
      }	       
   }
   return(1);
}

int showAllHands(struct AllHands *allHands, int handcount) 
{
   int i;
   for (i=0; i<handcount; i++) {
	 fprintf(stdout, "HAND %7d: ", i); 
	 showSevenCardHand(&allHands[i].sevenCardHand);
	 fprintf(stdout, " | SCORE: %7d ", allHands[i].score); 
	 fprintf(stdout, " | DESCRIPTION: "); 
	 describe(allHands[i].score); 
	 fprintf(stdout, "\n"); 
   }
}

