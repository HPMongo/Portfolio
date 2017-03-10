/***************************************************************************
 ** cardtest4.c ( test of playVillage function) 
 ** Jeremy Fischman
 ** 
 ** This program is a unit test of the playVillage  function to ensure it 
 ** complies with the following business rules: 
 **
 ** 1. The playVillage card should accept a game state, a player, and a hand
 **    position.
 **
 ** 2. The next card in the player's deck should be moved to the hand. 
 **
 ** 3. The number of player actions should increase by 2. 
 ** 
 ** 4. The deck count should decrease by 1. 
 **
 ** 5. The hand count should remain the same (since the village card is 
 **    discarded, but one card is added).
 **
 ** 6. The play village card should be discarded.  
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

int checkPlayVillage(struct gameState *pre, int player, int handPosition, 
		     FILE *f);


int main(){

  int n, i, j, p; 
  FILE *f= fopen("unittestresults.out", "a+");
  int randomDeckCount=0;
  int randomHandCount=0;
  int randomDiscardCount=0;

  struct gameState *pre= malloc(sizeof(struct gameState));


  SelectStream(2);
  PutSeed(3);


  printf("\n Running tests of the playVillage function:\n");
  fprintf(f,"\n Running tests of the playVillage function:\n");


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


    /* pick a random player */
    p=floor(Random()*2);
      
    /* set the count of played cards to 0 */
    pre->playedCardCount =0;

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

    /* Rewrite the selected player's hand to include village card at the random*/
    /* position in his hand */
    int randomHandPosition = floor(Random()* 10 +5 ); 
    pre->hand[p][randomHandPosition] = village;

    printf("\nTest run %i:", n);
    checkPlayVillage(pre, p,randomHandPosition, f);
  
 }



 
  printf("\nTesting playVillage complete.  \n");
  printf("Failed test results, if any, in unittestresults.out\n");
  fprintf(f,"Testing playVillage complete.  \n");

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




int checkPlayVillage(struct gameState *pre, int player, int handPosition, 
		     FILE *f){

  int testFail = 0; 
  int actionCount; 
  int nextCard = 0; 
  int preDeckCounter = pre->deckCount[player];


  /* create a copy of the input gameState-- 'post' */
  struct gameState *post = malloc(sizeof(struct gameState));
  memcpy(post, pre, sizeof(struct gameState)); 

  /* create an array to track differences from pre to post and zeroize it */
  int *differences = malloc(sizeof(int)*MEMBERS);
  memset(differences, 0, (sizeof(int)*MEMBERS));
 


  /************************************************************************/
  /*                              BEGIN TESTING                           */
  /************************************************************************/

  /* 1. The playVillage card should accept a game state, a player, and a hand */
  /*    position.  */

  playVillage(post, player, handPosition);

  /* 2. The next card in the player's deck should be moved to the hand. */   
  nextCard= pre->deck[player][preDeckCounter -1];
  if (nextCard != post->hand[player][handPosition]){
    printf("Business rule #2 fails: next deck card not delivered to hand\n");
    fprintf(f,"Business rule #2 fails: next deck card not delivered to hand\n");
    testFail=1;
  }


  /* 3. The number of player actions should increase by 2. */
  actionCount = pre->numActions+2;
  if (actionCount != post->numActions){
    printf("Business rule #3 fails: number of actions not increased by 3\n");
    fprintf(f,"Business rule #3 fails: number of actions not increased by 3\n");
    testFail=1;
  }


  
  /* 4. The deck count should decrease by 1. */
  if( (preDeckCounter-1) != post->deckCount[player]){
    printf("Business rule #4 fails: improper deckCount\n");
    fprintf(f,"Business rule #4 fails: improper deckCount\n");
    testFail=1;    
  }


  /* 5. The hand count should remain the same (since the village card is */
  /*     discarded, but one card is added). */
  if (pre->handCount[player] != post->handCount[player]){
    printf("Business rule #5 fails: handCount changed\n");
    fprintf(f,"Business rule #5 fails: handCount changed\n");
    testFail=1;    
  }



  /* 6. The play village card should be discarded.  */
  if(post->playedCards[post->playedCardCount-1] != village){
    printf("Business rule #6 fails: improper discard\n");
    fprintf(f,"Business rule #6 fails: improper discard\n");
    testFail=1;    
  }

  /* 7. The other features of the state of the game should all be unchanged.*/
  
  compareGameState(pre, post, differences, MEMBERS); 
  for (int i =0; i < MEMBERS; i++){
    /* First clause checks all of the members that shouldn't change at all */
      if( (i == 7) ||(i ==16) ||(i==17) ){
         /* parameter 7, 16, and 17 are supposed to change (numActions) */
      }
      else if ( (differences[i]) && (i < 10) && (i!=7)){
      testFail =1; 
      printf("playVillage() fails business rule #7:");
      printf(" state param #%d fails\n with code %d", i, differences[i]); 
      fprintf(f,"playVillage() fails business rule #7:");
      fprintf(f," state param #%d fails\n with code %d", i, differences[i]);

      /* Next 4 clauses ensure only the chosen player's data has changed*/ 
    } else if ((differences[i] ==1) && (player !=0)){
      testFail =1; 
      printf("playVillage() fails business rule #7:");
      printf(" state param #%d fails\n with code %d", i, differences[i]); 
      fprintf(f,"playVillage() fails business rule #7:");
      fprintf(f," state param #%d fails\n with code %d", i, differences[i]); 
    } else if ((differences[i] ==10) && (player !=1)){
      testFail =1; 
      printf("playVillage() fails business rule #7:");
      printf(" state param #%d fails\n with code %d", i, differences[i]); 
      fprintf(f,"playVillage() fails business rule #7:");
      fprintf(f," state param #%d fails\n with code %d", i, differences[i]); 
    } else if ((differences[i] ==100) && (player !=2)){
      testFail =1; 
      printf("playVillage() fails business rule #7:");
      printf(" state param #%d fails\n with code %d", i, differences[i]); 
      fprintf(f,"playVillage() fails business rule #7:");
      fprintf(f," state param #%d fails\n with code %d", i, differences[i]); 
    } else if ((differences[i] ==1000) && (player !=3)){
      testFail =1; 
      printf("playVillage() fails business rule #7:");
      printf(" state param #%d fails\n with code %d", i, differences[i]); 
      fprintf(f,"playVillage() fails business rule #7:");
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
