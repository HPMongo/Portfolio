/***************************************************************************
 ** unittest3.c ( test of getCost() function) 
 ** Jeremy Fischman
 ** 
 ** This program is a unit test of the getCost function.  It tests that the
 ** function complies with the following business rules: 
 **
 ** 1. The function accepts an integer representing a card
 **
 ** 2. getCost should return the correct integer cost of the card.
 **
 ** 3. The other features of the state of the game should all be unchanged. 
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
#define NUMTESTS 100   /* Number of times to run the tests */


/* function prototypes */

int compareGameState(struct gameState *old, struct gameState *new, 
		     int *diffArray, int length);

int checkGetCost(int player, struct gameState *pre, FILE *f); 





int main(){

  int n, i, j, p; 
  FILE *f= fopen("unittestresults.out", "a+");

 
  struct gameState pre;


  SelectStream(2);
  PutSeed(3);




  printf("\n Running tests of getCost():\n");
  fprintf(f,"\n Running tests of getCost():\n");

  for (n = 0; n < NUMTESTS; n ++){
    /* generate a random gamestate*/
    for (i=0; i <sizeof(struct gameState); i++){
      ((char*)&pre)[i]= floor(Random() * 256);
    }
    p=floor(Random()*2);
    pre.deckCount[p]= floor(Random() * MAX_DECK);
    pre.discardCount[p] = floor(Random() * MAX_DECK);
    pre.handCount[p] =floor(Random() * MAX_HAND);


    /* Fill hands with random cards*/
    for (i = 0; i < MAX_PLAYERS; i++){
      for( j=0; j < MAX_HAND; j++){
	pre.hand[i][j]=floor(Random()*CARDTYPES);
      }
    }
    printf("\nTest run %i:", n);

    
    checkGetCost(p, &pre, f);
  
 }

 
  printf("\nTesting getCost() complete.  \n");
  printf("Failed test results, if any, in unittestresults.out\n");

  fprintf(f,"\nTesting getCost() complete.  \n");
  fclose(f);

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



int checkGetCost(int player, struct gameState *pre, FILE *f){

  int testFail = 0; 
  int randomInt = 0; 
  int randomCard = 0;
  int programCost = 0;
  int myCost = 0;
  int costArray[CARDTYPES]={0,2,5,8,0,3,6,6,5,4,4,5,4,4,3,4,3,5,3,
			    5,3,4,2,5,4,4,4};

  /* create a copy of the input gameState */
  struct gameState *post = malloc(sizeof(struct gameState));
  memcpy(post, pre, sizeof(struct gameState)); 

  /* create an array to track differences from pre to post and zeroize */
  int *differences = malloc(sizeof(int)*MEMBERS);
  memset(differences, 0, (sizeof(int)*MEMBERS));


  /* For each random game state, I'll run 1000 tests of getCost*/
  for (int i = 0; i < 1000; i++){

    randomInt = floor(Random()* MAX_HAND);
    randomCard = pre->hand[player][randomInt];

    /* Business Rule #1: The function an integer representing the card */
    programCost = getCost(randomCard); 
  
    /* 2. getCost should return the appropriate integer cost for each card*/
    myCost = costArray[randomCard];

    if(programCost != myCost){
      printf("getCost fails business rule #2: incorrect cost returned.\n");
      printf("%d == %d\n", myCost, programCost);
   
      fprintf(f,"getCost fails business rule #2: incorrect cost returned.\n");
      fprintf(f,"%d == %d\n", myCost, programCost);
      testFail = 1; 
    }
  }


  /* After 1000 tests of getCost, I'll make sure game state is unchanged. */
  /*3. The other features of the state of the game should all be unchanged. */

  compareGameState(pre, post, differences, MEMBERS); 

  for (int i =0; i < MEMBERS; i++){
    if ((differences[i]) && (i != 8)){
      testFail =1; 
      printf("getCost fails business rule #3:");
      printf(" state param #%d fails\n with code %d", i, differences[i]); 
      fprintf(f,"getCost fails business rule #3:");
      fprintf(f," state param #%d fails\n with code %d", i, differences[i]); 
    }
  }
  

  if(!testFail){
    printf("\tTests passed.");
  }


  free(post); 
  free(differences);
  return testFail;
} 
