#include "poker.h"
int royalflush( struct SevenCardHand *hand)
{
    int i, count, current,current1, start; 
	short highcard;
   // Look for straight starting at card 0
	highcard = current = 14;
	current1 = hand->card[0].suit;
	count = 1; 
	for (i=1; i<7; i++) 
	{
      if (hand->card[i].value == (current-1)&& hand->card[i].suit==current1) 
	  {
		  if (++count == 5)
            return (ROYAL_FLUSH + hand->card[0].value * VALUE1 + hand->card[1].value * VALUE2 + 
                     hand->card[2].value * VALUE3 + hand->card[3].value * VALUE4 + 
                     hand->card[4].value * VALUE5); 
		  else 
		  {
            current--; 
		  }
      }
      else 
	  {
         if (hand->card[i].value != current ) {
            count = 1; 
            highcard = current = 14;
			current1 = hand->card[i].suit;         }
      }
   }
   return(0);
}

int straightflush( struct SevenCardHand *hand)
{
   int i, count, current,current1, start; 
	short highcard;
   // Look for straight starting at card 0
	highcard = current = hand->card[0].value;
	current1 = hand->card[0].suit;
	count = 1; 
	for (i=1; i<7; i++) 
	{
      if (hand->card[i].value == (current-1)&& hand->card[i].suit==current1) 
	  {
		  if (++count == 5)
            return (STRAIGHT_FLUSH + highcard * VALUE1); 
		  else 
		  {
            current--; 
		  }
      }
      else 
	  {
         if (hand->card[i].value != current ) {
            count = 1; 
            highcard = current = hand->card[i].value;
			current1 = hand->card[i].suit;         }
      }
   }
   return(0); 
}


int waitOnCond(int tabIndex)
{
  while(1)
  {
      pthread_mutex_lock(&tables[tabIndex].m1);
      if(tables[tabIndex].controlFlg == 1)
      {
        pthread_mutex_unlock(&tables[tabIndex].m1);
        tables[tabIndex].controlFlg = 0;
	break;
      }
      else
      {
        pthread_cond_wait(&tables[tabIndex].c1,&tables[tabIndex].m1);
        pthread_mutex_unlock(&tables[tabIndex].m1);
      }
  }
}



//void *PlayerThread(struct Player *p)
void *PlayerThread(void *ptr)
{
  
  int i,j;
  long temp;
  struct Player *p = (struct Player *)ptr;

 while(1)
 {
   while(1)
   {
     pthread_mutex_lock(&p->m);
     if(p->controlFlg == 1)
     {
       p->controlFlg = 0;
       pthread_mutex_unlock(&p->m);
       break;
     }
     else
     {
       pthread_cond_wait(&p->c,&p->m);
       pthread_mutex_unlock(&p->m);
     }
   }
   //printf("I am player:%u\n",pthread_self());
   /* check if it is selected as sBlind by looking at the round variable of table struct */
   if(p->tableB)
   {
    if(p->tableB->round==0) // implement the sBlind Functionality
	{
		//printf(" Player ID is %d\n", p->Id);
		if(p->tableB->sblind==p->Id)
		{
			if(p->money<=BET_MONEY/2)
			{
				p->tableB->pot_money+=p->money;
				p->money-=p->money;
				p->out=1;
				p->tableB->cou++;
			}
			else
			{
				p->money-= BET_MONEY/2;
				p->tableB->pot_money+=BET_MONEY/2;
				//printf(" sblind is %d \n",p->tableB->sblind);
				//return;
			}
		}
		else if(p->tableB->Bblind==p->Id)
		{
			if(p->money<=BET_MONEY) 
			{
				p->tableB->pot_money+=p->money;
				p->money-=p->money;
				p->out=1;
				p->tableB->cou++;
			}
			else
			{
			p->money-= BET_MONEY;
			p->tableB->pot_money+=BET_MONEY;
			//printf(" Bblind is %d \n",p->tableB->Bblind);
			//return;
			}
		}
	}
	
	else 
	{
		i= RAND(0,2);
		if(i==0)                              //call
		{
			if (p->tableB->round==1)
			{
				if(p->Id == p->tableB->sblind )
				{				
					temp=(p->tableB->Minimum-BET_MONEY/2);
					if(temp>=p->money)
					{
						p->tableB->pot_money+=p->money;
						p->money-=p->money;
						p->out=1;
						p->tableB->cou++;
						//pthread_exit((void*)NULL);
						//return;
					}
					else 
					{
						p->money-=(p->tableB->Minimum-BET_MONEY/2);
						p->tableB->pot_money+=(p->tableB->Minimum-BET_MONEY/2);
						//printf("%d player CALLs 1\n",p->Id);
						//return;
					}				
				}
				else if (p->Id == p->tableB->Bblind)
				{
					if(p->Id == p->tableB->Bblind )
					{				
						temp=(p->tableB->Minimum-BET_MONEY);
						if(temp>=p->money)
						{
							p->tableB->pot_money+=p->money;
							p->money-=p->money;
							p->out=1;
							p->tableB->cou++;
							//pthread_exit((void*)NULL);
							//return;
						}
						else 
						{
							p->money-=(p->tableB->Minimum-BET_MONEY);
							p->tableB->pot_money+=(p->tableB->Minimum-BET_MONEY);
							//printf("%d player CALLs 2\n",p->Id);
							//return;
						}				
					}
				}
				else
				{
					if(p->tableB->Minimum<=p->money)
					{
						if(p->tableB->Minimum==p->money) {p->out=1;p->tableB->cou++;}
						p->money-=p->tableB->Minimum;
						p->tableB->pot_money += p->tableB->Minimum;
						//printf("%d player CALLs 3\n",p->Id);
						//return;
						//break;
					}
					else
					{
						p->tableB->pot_money+=p->money;
						p->money-=p->money;
						p->out=1;
						p->tableB->cou++;
						//printf("%d player CALLs with OUT \n",p->Id);
						//pthread_exit((void*)NULL);
						//return;
					}
				}
			}
			else
			{
				if(p->tableB->Minimum<=p->money)
				{
					if(p->tableB->Minimum==p->money) {p->out=1;p->tableB->cou++;}
					p->money-=p->tableB->Minimum;
					p->tableB->pot_money += p->tableB->Minimum;
					//printf("%d player CALLs 3\n",p->Id);
					//return;
					//break;
				}
				else
				{
					p->tableB->pot_money+=p->money;
					p->money-=p->money;
					p->out=1;
					p->tableB->cou++;
					//printf("%d player CALLs with OUT \n",p->Id);
					//pthread_exit((void*)NULL);
					//return;
				}
			}
		}
		else if(i==1)                               // raise 
		{
			if (p->tableB->round==1)
			{
				if(p->Id == p->tableB->sblind )
				{				
					temp=(p->tableB->Minimum-BET_MONEY/2);
					if(temp>=p->money)
					{
						p->tableB->pot_money+=p->money;
						p->money-=p->money;
						p->out=1;
						p->tableB->cou++;
						//pthread_exit((void*)NULL);
						//return;
					}
					else 
					{
						p->money-=(p->tableB->Minimum-BET_MONEY/2);
						p->tableB->pot_money+=(p->tableB->Minimum-BET_MONEY/2);
						//printf("%d player CALLs\n",p->Id);
						//return;
					}				
				}
				else if (p->Id == p->tableB->Bblind)
				{
					if(p->Id == p->tableB->Bblind )
					{				
						temp=(p->tableB->Minimum-BET_MONEY);
						if(temp>=p->money)
						{
							p->tableB->pot_money+=p->money;
							p->money-=p->money;
							p->out=1;
							p->tableB->cou++;
							//pthread_exit((void*)NULL);
							//return;
						}
						else 
						{
							p->money-=(p->tableB->Minimum-BET_MONEY);
							p->tableB->pot_money+=(p->tableB->Minimum-BET_MONEY);
							//printf("%d player CALLs\n",p->Id);
							//return;
						}				
					}
				}
				else 
				{
					if(p->money > 2*p->tableB->Minimum) //&& fold[play]!=1)
					{
						p->money -= 2*p->tableB->Minimum;
						p->tableB->pot_money += 2*p->tableB->Minimum;
						p->tableB->Minimum = 2*p->tableB->Minimum;
						//printf("%d player RAISEs\n",p->Id);
					}
					else
					{
						 p->tableB->pot_money += p->money;
						 p->money -= p->money;
						 p->tableB->Minimum = 2*p->tableB->Minimum;
						 p->out=1;
						 p->tableB->cou++;
						 //printf("%d player RAISEs wid out\n",p->Id);
						// pthread_exit((void*)NULL);
						//return;
					}

				}
			}
			else 
			{
				if(p->money > 2*p->tableB->Minimum) //&& fold[play]!=1)
				{
					p->money -= 2*p->tableB->Minimum;
					p->tableB->pot_money += 2*p->tableB->Minimum;
					p->tableB->Minimum = 2*p->tableB->Minimum;
					//printf("%d player RAISEs\n",p->Id);
				}
				else
				{
					 p->tableB->pot_money += p->money;
					 p->money -= p->money;
					 p->tableB->Minimum = 2*p->tableB->Minimum;
					 p->out=1;
					 p->tableB->cou++;
					 //printf("%d player RAISEs wid out\n",p->Id);
					// pthread_exit((void*)NULL);
					//return;
				}
			}
		}
		else if(i==2)                                  //fold
		{                                         
			if (p->tableB->round==1)
			{
				if(p->Id == p->tableB->sblind )
				{				
					temp=(p->tableB->Minimum-BET_MONEY/2);
					if(temp>=p->money)
					{
						p->tableB->pot_money+=p->money;
						p->money-=p->money;
						p->out=1;
						p->tableB->cou++;
						//pthread_exit((void*)NULL);
						//return;
					}
					else 
					{
						p->money-=(p->tableB->Minimum-BET_MONEY/2);
						p->tableB->pot_money+=(p->tableB->Minimum-BET_MONEY/2);
						//printf("%d player CALLs\n",p->Id);
						//return;
					}				
				}
				else if (p->Id == p->tableB->Bblind)
				{
					if(p->Id == p->tableB->Bblind )
					{				
						temp=(p->tableB->Minimum-BET_MONEY);
						if(temp>=p->money)
						{
							p->tableB->pot_money+=p->money;
							p->money-=p->money;
							p->out=1;
							p->tableB->cou++;
							//pthread_exit((void*)NULL);
							//return;
						}
						else 
						{
							p->money-=(p->tableB->Minimum-BET_MONEY);
							p->tableB->pot_money+=(p->tableB->Minimum-BET_MONEY);
							//return;
						}				
					}
				}
				else
				{
					//printf("%d player FOLDs\n",p->Id);
					p->fold=1;
					p->tableB->last_fold=p->Id;
					p->tableB->count1++;


				}
			}
			else 
			{
				//printf("%d player FOLDs\n",p->Id);
				p->fold=1;
				p->tableB->last_fold=p->Id;
				p->tableB->count1++;
			}
		}
	}
   }
   else
     printf("Table was empty\n");


	/* Betting Rules Implementation */
	//printf(" Sending signal to master \n");
	pthread_mutex_lock(&p->tableB->m1);
	//printf("LOCK aCuer\n");
	//p->tableB->player[p->tableB->sblind]->some_cond = 1;
	p->tableB->controlFlg = 1;
	
	//some_cond=1;
	pthread_cond_signal(&p->tableB->c1);
	//printf("SIGNAL SENT \n");
	pthread_mutex_unlock(&p->tableB->m1);

/*	pthread_mutex_lock(&p->m);
	pthread_cond_wait(&p->c,&p->m);*/
 }
}





void *ControlThread(void *threadId)
{
  int i,j,k;
  //struct Table table1;

  //pthread_mutex_init(&m1,NULL);
  //pthread_cond_init(&c1,NULL);

  /* initialise the table */
  k= *(int*)threadId;
  tables[k].cou=0;
  tables[k].Minimum=10000;
  tables[k].pot_money=0;
  //table1.flag[]={0};
  tables[k].round=0;
  tables[k].winner=-1;
  tables[k].last_fold=-1;
  tables[k].TableId=*(int *)threadId;
  tables[k].controlFlg = 0;
  pthread_mutex_init(&tables[k].m1,NULL);
  pthread_mutex_init(&tables[k].Mutex,NULL);
  pthread_cond_init(&tables[k].c1,NULL);
	
  pthread_t players[NUM_PLAYERS];
  for(i=0;i<NUM_PLAYERS;i++)
  {
		//struct Player *p[i];
          struct Player *p = (struct Player*)malloc(sizeof(struct Player));
	  tables[k].player[i]=p;
		 /* initialise the player */
	   p->money=100000;
	   p->out=0;
	   p->fold=0;
	   p->flag=0;
	   p->tableB=&tables[k];
	   p->Id=i;
	   p->some_cond =0;
	   p->controlFlg = 0;

           pthread_mutex_init(&p->m,NULL);
	   pthread_cond_init(&p->c,NULL);

           if(pthread_create(&players[i], NULL, PlayerThread, (void *)p))
	   {
	        printf("ERROR; return code from pthread_create()  \n");
	        exit(-1);
           }
		/* for(j=0;j<NUM_PLAYERS;j++)
		{
			pthread_mutex_lock(&m[j]);
			pthread_cond_wait(&c[j],&m[j]);
		}*/
  }

	/* set cards */

	/* Blind selection process */
    tables[k].sblind = RAND(0,4);
    tables[k].Bblind = tables[k].sblind+1;

	//printf("In round %d \n",tables[k].round);
    
	
    pthread_mutex_lock(&tables[k].player[tables[k].sblind]->m);
    tables[k].player[tables[k].sblind]->controlFlg = 1;
    pthread_cond_signal(&tables[k].player[tables[k].sblind]->c);
    pthread_mutex_unlock(&tables[k].player[tables[k].sblind]->m);

    //printf("Small blind thread placed bet\n");
		
	/* wait for the blinds tofinish */
    waitOnCond(k);
    //printf("Small blind thread returned control\n");
   
	/*pthread_mutex_lock(&table1.player[table1.sblind]->c);
	pthread_cond_wait(&table1.player[table1.sblind]->c,&table1.player[table1.sblind]->m);*/
	
    pthread_mutex_lock(&tables[k].player[tables[k].Bblind]->m);
    tables[k].player[tables[k].Bblind]->controlFlg = 1;
    pthread_cond_signal(&tables[k].player[tables[k].Bblind]->c);
    pthread_mutex_unlock(&tables[k].player[tables[k].Bblind]->m);
    //printf("Big blind thread placed bet\n");
	
    waitOnCond(k);
	
    //for(j=0;j<NUM_PLAYERS;j++)
	//printf("%d player has %ld\n",tables[k].player[j]->Id,tables[k].player[j]->money);

	/* lock the blind for next rounds */
	/*pthread_mutex_lock(&table1.player[table1.Bblind]->m);
	pthread_cond_wait(&table1.player[table1.Bblind]->c,&table1.player[table1.Bblind]->m);
	pthread_mutex_unlock(&table1.player[table1.Bblind]->m);*/

    PlayPoker(k);
    //pthread_exit((void*)NULL);
}


//void PlayPoker(struct Table *table1)
void PlayPoker(int tabIndex)
{
	/*set the Cards */
   struct timeval tv;
   struct timezone tz;

   struct TableOfPlayers table;
   struct DeckOfCards deck;
   struct CommunityCards community;

   gettimeofday(&tv, &tz);
   srand((unsigned int) tv.tv_usec);

   int i,c,cc, j, k,frst=0,t=0,cp=0; 
   int winner;
   int winnerscore;

   struct SevenCardHand sevenCardHand; 
   int handScore[NUM_PLAYERS];

   struct AllHands *allHands;
   struct Table *table1 = &tables[tabIndex];

   allHands = (struct AllHands *) malloc(NUM_TABLES * NUM_HANDS * NUM_PLAYERS * sizeof(struct AllHands));
   
   int handcount = 0;
   while (table1->cou<5)
   {
       cp=0;
	   for(cc=0;cc<NUM_PLAYERS;cc++)
		   table1->player[cc]->flag=0;
	   table1->Minimum = 10000;
	   if(frst == 1)
	   {
		   if((table1->sblind+1)==6)
			table1->sblind=0;
		   else
			table1->sblind++;
			
		   table1->Bblind=table1->sblind+1;

		   if(table1->Bblind>5)
			table1->Bblind=0;
			for(k=0;k<NUM_PLAYERS;k++)
			  table1->player[k]->fold=0;
			
			if(table1->player[table1->sblind]->out==1 || table1->player[table1->Bblind]->out==1)
			{
				while(table1->player[table1->sblind]->out==1)
				{
					if((table1->sblind+1)==6)
						table1->sblind=0;
					else
						table1->sblind ++;
					if(table1->sblind==5)
						table1->Bblind=0;
					else
						table1->Bblind=table1->sblind+1;
				}
				while(table1->player[table1->Bblind]->out==1)
				{
					table1->Bblind++;
					if(table1->Bblind>5)
						table1->Bblind=0;
				}
			}
			pthread_mutex_lock(&table1->player[table1->sblind]->m);
			table1->player[table1->sblind]->controlFlg = 1;
			pthread_cond_signal(&table1->player[table1->sblind]->c);
			pthread_mutex_unlock(&table1->player[table1->sblind]->m);
			
			waitOnCond(tabIndex);

			pthread_mutex_lock(&table1->player[table1->Bblind]->m);
			table1->player[table1->Bblind]->controlFlg = 1;
			pthread_cond_signal(&table1->player[table1->Bblind]->c);
			pthread_mutex_unlock(&table1->player[table1->Bblind]->m);
			
			waitOnCond(tabIndex);
		}

		frst=1;
		table1->round=1;
		
		/* prepare to do the frst round functionality */
		initializeDeck(&deck);
		shuffleDeck(&deck);
		dealHands(&table, &deck);
		for (k=0;k<NUM_PLAYERS ;k++ )
		{
			if(table1->player[k]->out!=1 && table1->player[k]->fold!=1) 
			{
				pthread_mutex_lock(&table1->player[k]->m);
				table1->player[k]->controlFlg = 1;
				pthread_cond_signal(&table1->player[k]->c);
				pthread_mutex_unlock(&table1->player[k]->m);

				waitOnCond(tabIndex);

				//printf("%d player has %ld\n",table1->player[k]->Id,table1->player[k]->money);
			}
			//printf("Player %d has %ld\n",table1->player[k]->Id,table1->player[k]->money);
		}
		table1->round=2;
		flop(&community, &deck);
		//printf(" IN ROUND %d\n",table1->round);
		for (k=0;k<NUM_PLAYERS ;k++ )
		{
			if(table1->player[k]->out!=1 && table1->player[k]->fold!=1) 
			{
				pthread_mutex_lock(&table1->player[k]->m);
				table1->player[k]->controlFlg = 1;
				pthread_cond_signal(&table1->player[k]->c);
				pthread_mutex_unlock(&table1->player[k]->m);

				waitOnCond(tabIndex);
			
			}
			//printf("%d player has %ld\n",table1->player[k]->Id,table1->player[k]->money);
	    }
	  table1->round=3;
	  //printf(" IN ROUND %d\n",table1->round);
	  turn(&community, &deck);
	  for (k=0;k<NUM_PLAYERS ;k++ )
	  {
			if(table1->player[k]->out!=1 && table1->player[k]->fold!=1) 
			{
				pthread_mutex_lock(&table1->player[k]->m);
				table1->player[k]->controlFlg = 1;
				pthread_cond_signal(&table1->player[k]->c);
				pthread_mutex_unlock(&table1->player[k]->m);
				
				waitOnCond(tabIndex);
			}
			//printf("%d player has %ld\n",table1->player[k]->Id,table1->player[k]->money);
	  }
	  table1->round=4;
	  river(&community, &deck);
	  //printf(" IN ROUND %d\n",table1->round);
	  for (k=0;k<NUM_PLAYERS ;k++ )
	  {
		  if(table1->player[k]->out!=1 && table1->player[k]->fold!=1) 
		  {
			  pthread_mutex_lock(&table1->player[k]->m);
			  table1->player[k]->controlFlg = 1;
			  pthread_cond_signal(&table1->player[k]->c);
			  pthread_mutex_unlock(&table1->player[k]->m);

			  waitOnCond(tabIndex);
			  
		  }
		  //printf("%d player has %ld\n",table1->player[k]->Id,table1->player[k]->money);
	  }
	  //table1->round++;

      winnerscore = -1;
      table1->winner = -1;
      for (j=0; j<NUM_PLAYERS; j++) 
	  {
		  if(table1->player[j]->flag!=1 && table1->player[j]->fold!=1)
		  {
			if (table1->player[j]->out==1)
				table1->player[j]->flag=1;
			
		 makeSevenCardHand(&(table.hand[j]), &community, &sevenCardHand);
		 handScore[j] = evaluateHand(&sevenCardHand);
		  }else makeSevenCardHand(&(table.hand[j]), &community, &sevenCardHand);
	 if (handScore[j] > winnerscore) {
            winnerscore = handScore[j];
	    winner = j;
         }
		  
		 		
		 
         //  Save all the hands, just for testing...
#if 0
	 printf("St Addr:%u End Addr %u Ref Addr %u\n",allHands,&allHands[NUM_PLAYERS*NUM_HANDS],&allHands[handcount]);
	 printf("Current hand count is :%d %d \n",handcount,NUM_PLAYERS*NUM_HANDS); 
	 allHands[handcount].sevenCardHand = sevenCardHand; 
	 allHands[handcount].score = handScore[j]; 
         handcount++; 
#endif
	 
		  
      }
	  if( table1->player[winner]->out==1)
	  {
		  table1->cou--;
		  table1->player[winner]->out=0;
		  table1->player[winner]->flag=0;
	  }

	//printf("The no of folds is %d nd no of outs r %d\n",table1.count1,table1.cou);
	  if(((table1->count1) + (table1->cou))>5)
		  {
		  //printf("The winner of hand %d is player %d and reminnig money is %ld and pot has %ld \n",i,table1->last_fold,table1->player[table1->last_fold]->money,table1->pot_money);
		  table1->player[table1->last_fold]->money+=table1->pot_money;
		  table1->pot_money=0;
		  //printf("Winnre money is %ld \n",table1->player[table1->last_fold]->money);
		  }
	  else
		  {//fprintf(stdout, "The winner of Hand %d is player %d and the remaining money is %ld nd pot has %ld \n", i, winner,table1->player[winner]->money,table1->pot_money);
	  table1->player[winner]->money += table1->pot_money;
	  table1->pot_money=0;
	  //fprintf(stdout," The winners money is %ld \n",table1->player[winner]->money);
	  } 
	  table1->round=0;
	  i++;
	  table1->count1=0;
	  frst=1;
	  t=0;
	  table1->cou=0;
	  for(cc=0;cc<NUM_PLAYERS;cc++)
	   {
		  if(table1->player[cc]->money!=0)
		   {
			  table1->player[cc]->out=0;
			  table1->player[cc]->flag=0;
			  //table1->cou++;
			  t++;
		   }
		   else table1->cou++;
	   }
	   if(t==5)
		   cp=1;
	 if(table1->cou==5)
	   {
			printf("************************\n");
	   }

	  /*
	  acquire mutex
	  check the no of players in free list
	  chk the available slots on the table
	  till the slots get emptied or the free list gets empty
	  add the players to the table 
	  */
	  //sleep(1);
   	  pthread_mutex_lock(&queue.m3);
	  pthread_mutex_lock(&table1->Mutex);
	  if(queue.index >=0 && table1->cou>0)
	   {
	     printf("Moving the winner to an new table\n");
		   for(k=0;k<table1->cou && queue.index >-1;k++)
		   {
		       c=freePos(table1);
		       if(queue.p[k])
		       {
			 pthread_mutex_lock(&table1->player[c]->m);
		         queue.p[k]->tableB=table1;
		         //printf("Adding new player at pos %d\n",c);
			 
		         table1->player[c]=queue.p[k];
		         queue.p[k] = 0;
		         table1->cou--;
		         table1->count1--;
		         table1->player[c]->out=0;
		         table1->player[c]->fold=0;
	                 table1->player[c]->flag=0;
			 pthread_mutex_unlock(&table1->player[c]->m);
		         queue.index--;
		       }
	               //printf("money status %d \n",table1->player[c]->money);
	               //printf("	money is added to the table %d \n",table1->TableId);
		   }
	      printf("player successfully moved to new table %d\n",table1->TableId);
	   }

	  pthread_mutex_unlock(&table1->Mutex);
	  pthread_mutex_unlock(&queue.m3);

   }
   
   printf("The table winner on table %d is %d \n",table1->TableId,winner);
   printf(" Winners money is %ld\n",table1->player[winner]->money);
	
  pthread_mutex_lock(&queue.m3);
  queue.index++;
  pthread_mutex_lock(&table1->player[winner]->m);
  queue.p[queue.index]=table1->player[winner];
  pthread_mutex_unlock(&table1->player[winner]->m);
  pthread_mutex_unlock(&queue.m3);
  
	
	sortAllHands(allHands, handcount);
   //showAllHands(allHands, handcount);
   //fprintf(stdout, "The winner of Hand %d is player %d\n", i, winner); 
   printf("Terminating Table thread::%u\n",pthread_self());
   pthread_exit( NULL);
	
	
}

int freePos (struct Table *table1)
{
	int i;
	for(i=0;i<NUM_PLAYERS;i++)
	{
	  if(table1->player[i])
	  {
	    if(table1->player[i]->out==1){
	      return i;
	    }
	  }
	}
}

int tableIndex[NUM_TABLES];

int main(int argc,char *argv[])
{
    pthread_t tabs[NUM_TABLES];
    int rc, t;
    pthread_mutex_init(&queue.m3,NULL);
    queue.index = -1;

#if 0

    for(t=0; t<atoi(argv[1]); t++)
    {
      tableIndex[t] = t;
      ControlThread(&tableIndex[t]);
    }

    while(1);

#endif

    for(t=0; t<atoi(argv[1]); t++)
    {
	  tableIndex[t] = t;
          printf("Creating thread %d\n", t);
          rc = pthread_create(&tabs[t], NULL, ControlThread,(void *)&tableIndex[t]);
	  if (rc)
	  {
	    printf("ERROR; return code from pthread_create() is %d\n", rc);
	    exit(-1);
         }
    }
    for(t=0;t<NUM_TABLES;t++)
    {
      pthread_join(tabs[t],NULL);
    }

  return 0;
}
