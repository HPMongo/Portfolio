/***************************************************************************
 ** cardtest1.c ( test of smithy functions) 
 ** Jeremy Fischman
 ** 
 ** This program is a unit test of the playSmithy() function.  It tests that
 ** playSmithy() complies with the following business rules: 
 **
 ** 1. The function accepts 1 player, a game state, and a hand position
 **
 ** 2. Should result in the next 3 cards from the player's deck moving to 
 **    his hand. 
 **
 ** 3. The smithy card should be removed from the player's hand, and go to
 **    the discard deck. 
 ** 
 ** 4. deckCount should decrease by 3.
 **
 ** 5. handCount should increase by 2 (3 more cards, but smithy discarded). 
 **
 ** 6. discardCount should increase by 1 (for smithy card). 
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
#define NUMTESTS 100   /* Number of times to run the tests */


/* function prototypes */

int compareGameState(struct gameState *old, struct gameState *new, 
		     int *diffArray, int length);

int countCards(int *deck, int *cardCount);

int checkSmithy(int player, struct gameState *pre, int position, 
		FILE *filename); 


int main(){

  int n, i, j, p; 
  FILE *f= fopen("unittestresults.out", "a+");
  int randomDeckCount=0;
  int randomHandCount=0;
  int randomDiscardCount=0;


  struct gameState *pre= malloc(sizeof(struct gameState));


  SelectStream(2);
  PutSeed(3);


  printf("\n Running tests of smithy():\n");
  fprintf(f,"\n Running tests of smithy():\n");


  for (n = 0; n < NUMTESTS; n ++){
    /* generate a random gamestate*/
    for (i=0; i <sizeof(struct gameState); i++){
      ((char*) pre)[i]= floor(Random() * 256);
    }

    /* setting certain key features to random, but within specs. */

    /* first, make sure total number of cards is below MAX_DECK */
    /* and select some random cards for the deck, discard, and hand*/
    do{
      randomDeckCount = floor(Random()* MAX_DECK); 
      randomHandCount = floor(Random()* 10 +5 ); /*random hand of 5-15 */
      randomDiscardCount= floor(Random()*MAX_DECK);
    } while ((randomDeckCount+randomHandCount+randomDiscardCount)>MAX_DECK); 

    /* pick one card in the hand to be the Smithy*/
    int randomSmithyPos = floor(Random()*randomHandCount); 

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

    /* Rewrite the selected player's hand to include smithy at the random*/
    /* position in his hand */
    pre->hand[p][randomSmithyPos]= smithy;

    printf("\nTest run %i:", n);
    checkSmithy(p, pre, randomSmithyPos, f);
  
 }



 
  printf("\nTesting playSmithy() complete.  \n");
  printf("Failed test results, if any, in unittestresults.out\n");
  fprintf(f,"Testing playSmithy() complete.  \n");

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




int checkSmithy(int player, struct gameState *pre, int position, FILE *f){

  int testFail = 0; 
  int preCards[3];
  int preDeckCounter;
  int preHandCounter;
  int preDiscardCounter;



  /* create a copy of the input gameState-- 'post' */
  struct gameState *post = malloc(sizeof(struct gameState));
  memcpy(post, pre, sizeof(struct gameState)); 

  /* create an array to track differences from pre to post and zeroize it */
  int *differences = malloc(sizeof(int)*MEMBERS);
  memset(differences, 0, (sizeof(int)*MEMBERS));



  /************************************************************************/
  /*                              BEGIN TESTING                           */
  /************************************************************************/



  /* 1. The function accepts 1 player, a game state, and a hand position */
  playSmithy(post,player, position); 




  /* 2. Should result in the next 3 cards from the player's deck moving to */
  /*    his hand. */

  preDeckCounter=pre->deckCount[player];
  preHandCounter=pre->handCount[player];
  preDiscardCounter=pre->discardCount[player];

  /* Check the 2 cards added to the end of the hand*/
  for (int i = 0; i < 2; i++){
    preCards[i] = pre->deck[player][preDeckCounter-(1+i)];
    //preDeckCounter--;
    if (post->hand[player][preHandCounter+i] != preCards[i]){
      printf("\nBusiness rule #2 fails: Next 3 cards from the deck not moved to the hand.\n");
      fprintf(f,"\nBusiness rule #2 fails: Next 3 cards from the deck not moved to the hand.\n");
    }
  }

  /* 3. The smithy card should be removed from the player's hand, and go to the discard deck.*/ 

  /* check that the 3rd card was moved to the hand in the same position as the discarded Smithy*/
  /* (This also finishes test of business rule 2.*/ 
  if (post->hand[player][position] !=pre->deck[player][preDeckCounter-3]){
    printf("\nBusiness rule #2 fails: Next 3 cards from the deck not moved to the hand.\n");
    fprintf(f,"\nBusiness rule #2 fails: Next 3 cards from the deck not moved to the hand.\n");
  }

  /* Check that the most recent discard is Smithy*/    
  if (post->discard[player][preDiscardCounter+1] != smithy){
    printf("Business rule #3 fails: smithy not in the discard pile");
    fprintf(f,"Business rule #3 fails: smithy not in the discard pile");
  }

  /* 4. deckCount should decrease by 3.*/
  if(post->deckCount[player] != preDeckCounter -3){
    printf("\nBusiness rule #4 fails: Deck count not decreased by 3\n ");
    fprintf(f,"\nBusiness rule #4 fails: Deck count not decreased by 3\n ");
  }

  /* 5. handCount should increase by 2 (3 more cards, but smithy discarded).*/
  if(post->handCount[player] != preHandCounter +2){
    printf("\nBusiness rule #5 fails: Hand count not increased by 2\n ");
    fprintf(f,"\nBusiness rule #5 fails: Deck count not increased by 2\n ");
  }


  /* 6. discardCount should increase by 1 (for smithy card). */
  if(post->discardCount[player] != preDiscardCounter + 1){
    printf("\nBusiness rule #6 fails: discard count not increased by 1\n ");
    fprintf(f,"\nBusiness rule #6 fails: discard count not increased by 2\n ");
  }


  /* 7. The other features of the state of the game should all be unchanged.*/
  
  compareGameState(pre, post, differences, MEMBERS); 
  for (int i =0; i < MEMBERS; i++){
    /* First clause checks all of the members that shouldn't change at all */
    if ( (differences[i]) && (i < 10) ){
      testFail =1; 
      printf("playSmithy() fails business rule #7:");
      printf(" state param #%d fails\n with code %d", i, differences[i]); 
      fprintf(f,"playSmithy() fails business rule #7:");
      fprintf(f," state param #%d fails\n with code %d", i, differences[i]);

      /* Next 4 clauses ensure only the chosen player's data has changed*/ 
    } else if ((differences[i] ==1) && (player !=0)){
      testFail =1; 
      printf("playSmithy() fails business rule #7:");
      printf(" state param #%d fails\n with code %d", i, differences[i]); 
      fprintf(f,"playSmithy() fails business rule #7:");
      fprintf(f," state param #%d fails\n with code %d", i, differences[i]); 
    } else if ((differences[i] ==10) && (player !=1)){
      testFail =1; 
      printf("playSmithy() fails business rule #7:");
      printf(" state param #%d fails\n with code %d", i, differences[i]); 
      fprintf(f,"playSmithy() fails business rule #7:");
      fprintf(f," state param #%d fails\n with code %d", i, differences[i]); 
    } else if ((differences[i] ==100) && (player !=2)){
      testFail =1; 
      printf("playSmithy() fails business rule #7:");
      printf(" state param #%d fails\n with code %d", i, differences[i]); 
      fprintf(f,"playSmithy() fails business rule #7:");
      fprintf(f," state param #%d fails\n with code %d", i, differences[i]); 
    } else if ((differences[i] ==1000) && (player !=3)){
      testFail =1; 
      printf("playSmithy() fails business rule #7:");
      printf(" state param #%d fails\n with code %d", i, differences[i]); 
      fprintf(f,"playSmithy() fails business rule #7:");
      fprintf(f," state param #%d fails\n with code %d", i, differences[i]); 
    }
  }

  if(!testFail){
    printf("\tTests passed.\n");
  }


  free(post); 
  free(differences);
  return testFail;
} 
