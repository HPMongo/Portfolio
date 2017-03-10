/***************************************************************************
 ** randomtestadventurer.c ( test of adventurer functions) 
 ** Jeremy Fischman
 ** 
 ** This program is a unit test of the playAdventurer() function including a
 ** random tester.  It tests that playAdventurer() complies with the following
 ** business rules: 
 **
 ** 1. The function accepts a game state, and a player
 **
 ** 2. Should result in the cards drawn from the deck until 2 treasures are
 **    found.
 **
 ** 3. The two treasures should be added to the hand
 ** 
 ** 4. Other cards should be discarded & discardCount increase by total - 2.
 **
 ** 5. handCount should increase by 2. 
 **
 ** 6. deckCount should decrease by the total number drawn.  
 ** 
 ** 7. The other features of the state of the game should all be unchanged. 
 **
 **************************************************************************/

#include "dominion_helpers.h"
#include "dominion.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "rngs.h"

#define MEMBERS 18     /* Number of member variables in a gameState (gS) */
#define DECKCOUNTMEMBER 13  /* The deckCount is 14th member of gS, index 13*/
#define CARDTYPES treasure_map+1  /* Number of different card types in game */
#define NUMTESTS 100000   /* Number of times to run the tests */


/* function prototypes */

int compareGameState(struct gameState *old, struct gameState *new, 
		     int *diffArray, int length);

int countCards(int *deck, int *cardCount);

int checkAdventurer(struct gameState *pre, int player, 
		    int *array); 


int main(){

  int n, i, j, p;
  int testsFailed = 0;

  /* record the rules which fail tests*/  
  int *rulesTest = malloc(sizeof(int)*7);
  for (i = 0; i <7; i++){
    rulesTest[i]=0; 
  }

  float percentage; 
  int randomDeckCount=0;
  int randomHandCount=0;
  int randomDiscardCount=0;


  struct gameState *pre= malloc(sizeof(struct gameState));


  SelectStream(2);
  PutSeed(3);


  printf("\n Running tests of playAdventurer():\n");


  for (n = 0; n < NUMTESTS; n ++){
    /* generate a random gamestate*/

    int k[10]= {adventurer, gardens, embargo, village, minion, mine, cutpurse,
		sea_hag, tribute, smithy};

    /* initialize a game state */
    initializeGame(2, k, 2, pre);

    /* setting certain key features to random, but within specs. */

    /* first, make sure total number of cards is below MAX_DECK */
    /* and select some random cards for the deck, discard, and hand*/
    do{
      randomDeckCount = floor(Random()* MAX_DECK); 
      randomHandCount = floor(Random()* 10 +5 ); /*random hand of 5-15 */
      randomDiscardCount= floor(Random()*MAX_DECK);
    } while ((randomDeckCount+randomHandCount+randomDiscardCount)>MAX_DECK); 


    /* pick a random player */
    p=floor(Random()*2);

    /* then set his card counts, and fill his deck, discard, and hand */
    pre->deckCount[p]= randomDeckCount;
    pre->discardCount[p] = randomDiscardCount;
    pre->handCount[p] = randomHandCount;

    /* Fill deck with a random selection of cards. */
    for (i = 0; i < MAX_PLAYERS; i++){
      for( j=0; j < MAX_DECK; j++){
          pre->deck[i][j]=floor(Random()*CARDTYPES); 
	  pre->discard[i][j] = floor(Random()*CARDTYPES);
	  pre->hand[i][j]= floor(Random()*CARDTYPES);
      }
    }
    testsFailed += checkAdventurer(pre, p, rulesTest);
 }



  percentage = (float)rulesTest[i]/NUMTESTS *100;
  printf("\nTesting playAdventurer() complete.\n");
  printf("%d of %d test cases failed. ", testsFailed, NUMTESTS);
  printf("(%f%%)\n\n", percentage);

  for (i = 0; i < 7 ; i++){
    percentage = (float)rulesTest[i]/NUMTESTS *100; 
    printf("Test criteria %d failed %d", i+1, rulesTest[i]); 
    printf(" times out of %d tests (%f%%).\n",NUMTESTS, percentage);
  }
  return 0;
}



/* This function compares 2 struct gameStates, and returns 0 if they are  */
/* identical, and 1 otherwise.  Also, it fills an Array with a 1 for each */
/* member of gameState 'new' that differs from gameState 'old' */
int compareGameState(struct gameState *old, struct gameState *new, 
		     int *diffArray, int length){

  int i, j, k;
  
  struct gameState *difference= malloc(sizeof(struct gameState)); 

 
  /* generating the difference between gameState old and gameState new using */
  /* bitwise XOR */
  char *charPtr1 =(char *)old; 
  char *charPtr2 =(char *) new; 
  char *charPtr3 =(char *) difference;  
  for (i = 0; i < sizeof(struct gameState); i++){

    /*generating a struct gamestate called differences that is the XOR of */
    /*old and new */
    charPtr3[i]= charPtr1[i]^charPtr2[i];
  }
 


  /* checking which members of new were changed, member by member */
  if (difference->numPlayers)    {diffArray[0]=1;}

  /* need to check every type of card for cardSupply and embargoTokens*/
  for (i = 0; i < CARDTYPES; i++){
    if (difference->supplyCount[i])   {diffArray[1] = 1;}
    if (difference->embargoTokens[i]) {diffArray[2] = 1;}
  }

  if (difference->outpostPlayed)  {diffArray[3]=1; }
  if (difference->outpostTurn)    {diffArray[4]=1; }
  if (difference->whoseTurn)      {diffArray[5]=1;}
  if (difference->phase)          {diffArray[6]=1;  }
  if (difference->numActions)     {diffArray[7]=1;  }
  if (difference->coins)          {diffArray[8]=1;} 
  if (difference->numBuys)        {diffArray[9]=1;}
 
  /*Checking all of the game states that depend on a particular player */
  for (j = 0; j < MAX_PLAYERS; j++){
 
    /*checking the player's hands */
    for (k = 0; k < MAX_HAND; k++){
      /*If a player's hand has changed, record a 1 in that digit */	
      if (difference->hand[j][k])   {diffArray[10] += (int) pow(10, j);}
    }


    /*If a player's hand Count has changed, record a 1 in that digit */
    if(difference->handCount[j])    {diffArray[11] += (int) pow(10, j);}


    /*checking all of the decks */
    for(int k = 0; k < MAX_DECK; k++){

      /* If any card in player j's deck has changed record player number */
      /* player 1 = 1, player 2=10, player 3= 100, etc.- then break out  */
      /* to examine the next player's deck.  */
      if(difference->deck[j][k])    {
	diffArray[12] += (int) pow(10, j);
	break;
      }

    }

    /*checking all of the discards */
    for(int k = 0; k < MAX_DECK; k++){

      /* If any card in player j's discard changed record player number  */
      /* player 1 = 1, player 2=10, player 3= 100, etc.- then break out  */
      /* to examine the next player's deck.  */
      if(difference->discard[j][k]) {
	diffArray[14] += (int) pow(10, j);
	break;
      }

    }

  
    if(difference->deckCount[j])    {diffArray[13] += (int)pow(10, j); }
    if(difference->discardCount[j]) {diffArray[15] += (int)pow(10, j); }
  }

  for (i = 0; i < MAX_DECK; i++){
    if (difference->playedCards[i]) {diffArray[16] = 1;}
  }

  if (difference->playedCardCount)  {diffArray[17] = 1;}

  free(difference); 
  return 0; 
}




int checkAdventurer(struct gameState *pre, int player, int *array){

  int testFail = 0; 
  int tempHand[MAX_HAND];
  int tempCount = 0;
  int treasureCount = 0;

  int preDeckCounter = pre->deckCount[player];
  int preHandCounter = pre->handCount[player];
  int preDiscardCounter = pre->discardCount[player];



  /* create a copy of the input gameState-- 'post' */
  struct gameState *post = malloc(sizeof(struct gameState));
  memcpy(post, pre, sizeof(struct gameState)); 

  /* create an array to track differences from pre to post and zeroize it */
  int *differences = malloc(sizeof(int)*MEMBERS);
  memset(differences, 0, (sizeof(int)*MEMBERS));



  /************************************************************************/
  /*                              BEGIN TESTING                           */
  /************************************************************************/


  /* 1. The function accepts a game state, and a player*/
  playAdventurer(post,player); 


  /* 2. Should result in the cards drawn from the deck until 2 treasures are*/
  /*    found.*/
  while (treasureCount < 2){
    tempHand[tempCount]= pre->deck[player][preDeckCounter -(tempCount+1)];
    if ( (tempHand[tempCount] == copper)
	 || (tempHand[tempCount]== silver)
	 || (tempHand[tempCount] == gold) ){
      treasureCount ++; 
    }

    tempCount++;
  }


  /* 3. The two treasures should be added to the hand */
  if ( (  (  post->hand[player][preHandCounter] != copper) 
	  &&(post->hand[player][preHandCounter] != silver)
	  &&(post->hand[player][preHandCounter] != gold))

       || ( (  post->hand[player][preHandCounter+1] != copper)
	    &&(post->hand[player][preHandCounter +1] != silver)
	    &&(post->hand[player][preHandCounter +1] != gold))){
    //printf("Business rule #3 fails: 2 treasures not delivered to hand\n");
    //fprintf(f,"Business rule #3 fails: 2 treasures not delivered to hand\n");
    testFail=1;
    array[3]++; 
  }



  /* 4. The other cards should be discarded, discardCount increase by total-2*/
  if(post->discardCount[player]>=preDiscardCounter+tempCount-2){
    //printf("Business rule #4 fails: temp hand not discarded properly\n");
    //fprintf(f,"Business rule #4 fails: temp hand not discarded properly\n");
    testFail =1;
    array[4]++;  
  }
 


  /* 5. handCount should increase by 2. */
  if(post->discardCount[player]>=preHandCounter+2){
    //printf("Business rule #5 fails: player hand count not increased by 2\n");
    //fprintf(f,"Business rule #5 fails: player hand count not");
    //fprintf(f," increased by 2\n");
    testFail =1;
    array[5]++; 
  }



  /* 6. deckCount should decrease by the total number drawn.  */
  if(post->deckCount[player] != (preDeckCounter - tempCount)){
    //printf("Business rule #6 fails: player deck count improperly decreased\n");
    //fprintf(f,"Business rule #6 fails: player deck count improperly decreased");
    testFail =1;
    array[6]++;  
  }



  /* 7. The other features of the state of the game should all be unchanged.*/
  
  compareGameState(pre, post, differences, MEMBERS); 
  for (int i =0; i < MEMBERS; i++){
    /* First clause checks all of the members that shouldn't change at all */
    if ( (differences[i]) && (i < 10) ){
      testFail =1;
      array[7]++;  
      //printf("playAdventurer() fails business rule #7:");
      //printf(" state param #%d fails\n with code %d", i, differences[i]); 
      //fprintf(f,"playAdventurer() fails business rule #7:");
      //fprintf(f," state param #%d fails\n with code %d", i, differences[i]);

      /* Next 4 clauses ensure only the chosen player's data has changed*/ 
    } else if ((differences[i] ==1) && (player !=0)){
      testFail =1;
      array[7]++; 
      //printf("playAdventurer() fails business rule #7:");
      //printf(" state param #%d fails\n with code %d", i, differences[i]); 
      //fprintf(f,"playAdventurer() fails business rule #7:");
      //fprintf(f," state param #%d fails\n with code %d", i, differences[i]); 
    } else if ((differences[i] ==10) && (player !=1)){
      testFail =1;
      array[7]++; 
      //printf("playAdventurer() fails business rule #7:");
      //printf(" state param #%d fails\n with code %d", i, differences[i]); 
      //fprintf(f,"playAdventurer() fails business rule #7:");
      //fprintf(f," state param #%d fails\n with code %d", i, differences[i]); 
    } else if ((differences[i] ==100) && (player !=2)){
      testFail =1;
      array[7]++; 
      //printf("playAdventurer() fails business rule #7:");
      //printf(" state param #%d fails\n with code %d", i, differences[i]); 
      //fprintf(f,"playAdventurer() fails business rule #7:");
      //fprintf(f," state param #%d fails\n with code %d", i, differences[i]); 
    } else if ((differences[i] ==1000) && (player !=3)){
      testFail =1;
      array[7]++;
      //printf("playAdventurer() fails business rule #7:");
      //printf(" state param #%d fails\n with code %d", i, differences[i]); 
      //fprintf(f,"playAdventurer() fails business rule #7:");
      //fprintf(f," state param #%d fails\n with code %d", i, differences[i]); 
    }
  }

  if(!testFail){
    //printf("\tTests passed.\n");
  }


  free(post); 
  free(differences);
  return testFail;
} 
