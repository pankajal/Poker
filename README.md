In this poker game simulation I used 10 Control Threads (Table Threads, The number of table threads I am taking from standard Input) 
and created a set os 6 player threads for each ControlThread. Then when a winner is announced for a table I am shifting that 
winner (player) thread with the whole money won to another table on which the remaining players are playing. I am doing this till 
I have one table left with the ultimate Tournament Winner who has won the whole money lost in the game by other lost threads.

I am printing the table winner with the table index on which he has won and the money he won
Then i am printing the table on which the winner is shifting to.

Then the last print statement on the console is giving the tournament winner with the whole money.


In the standard functions I changed the evaluateHand function to check for the royalflush as well as straightflush.

Note: I am taking the number of tables to be run by the commandLine. The maximum number would not go beyond 10 as NUM_TABLES i declared as 10.

To run my code you need to give 

./play_poker 10

I have not including the Makefile..

