/***************************************************************************
 ** unittest1.c ( test of shuffle() function) 
 ** Jeremy Fischman
 ** 
 ** This program is a unit test of the shuffle function.  It tests that the
 ** shuffle function complies with the following business rules: 
 **
 ** 1. The function accepts 1 player and a game state, and the discard deck
 ** of the chosen player is permuted. 
 **
 ** 2. Should result in the same number of total cards in the player's deck
 **    following shuffling.
 ** 
 ** 3. Should result in the same number of each type of card in the player's
 **    deck following shuffling as existed in the deck before shuffling
 ** 
 ** 4. The permutation after shuffling should look relatively random (i.e.
 **    the order of cards should be varied somewhat randomly).
 ** 
 ** 5. The other features of the state of the game should all be unchanged. 
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

int checkShuffle(int player, struct gameState *pre, FILE *filename); 





int main(){

  int n, i, j, p; 

  FILE *f= fopen("unittestresults.out", "w+");

  struct gameState *pre= malloc(sizeof(struct gameState));


  SelectStream(2);
  PutSeed(3);


  printf("\n Running tests of shuffle():\n");
  fprintf(f,"\n Running tests of shuffle():\n");

  for (n = 0; n < NUMTESTS; n ++){
    /* generate a random gamestate*/
    for (i=0; i <sizeof(struct gameState); i++){
      ((char*) pre)[i]= floor(Random() * 256);
    }
    p=floor(Random()*2);
    pre->deckCount[p]= floor(Random() * MAX_DECK);
    pre->discardCount[p] = floor(Random() * MAX_DECK);
    pre->handCount[p] = floor(Random() * MAX_HAND);

    /* Fill deck with a random selection of cards. */
    for (i = 0; i < MAX_PLAYERS; i++){
      for( j=0; j < MAX_DECK; j++){
	pre->deck[i][j]=floor(Random()*CARDTYPES);
      }
    }
    printf("\nTest run %i:", n);
    checkShuffle(p, pre, f);
  
 }

 
  fprintf(f,"Testing shuffle() complete. \n");
 
  printf("\nTesting shuffle() complete. \n");
  printf("\nFailed Tests, if any, in unittestresults.out\n");

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




/* This function counts the cards in a card deck and records number of each */
/* type of card in an array */
int countCards(int *deck, int *cardCount){

  int i; 
 
  for (i= 0; i < MAX_DECK; i++){
    cardCount[deck[i]]++;
  }
  return 0;
} 


int checkShuffle(int player, struct gameState *pre, FILE *f){

  int testFail = 0; 

  /* create two arrays to track the number of each cardtype in a deck */
  int *cardListPre = malloc(sizeof(int)*CARDTYPES);
  memset(cardListPre, 0, (sizeof(int)*CARDTYPES));
 int *cardListPost = malloc(sizeof(int)*CARDTYPES);
  memset(cardListPost, 0, (sizeof(int)*CARDTYPES));

  /* create a copy of the input gameState */
  struct gameState *post = malloc(sizeof(struct gameState));
  memcpy(post, pre, sizeof(struct gameState)); 

  /* create an array to track differences from pre to post and zeroize */
  int *differences = malloc(sizeof(int)*MEMBERS);
  memset(differences, 0, (sizeof(int)*MEMBERS));



  /* Business Rule #1: The function accepts 1 player and a game state... */
  shuffle(player, post); 


  compareGameState(pre, post, differences, MEMBERS); 


  /* Business rule #2: Should result in the same number of total cards in the
     player's deck following shuffling. */
  if (differences[DECKCOUNTMEMBER]) {
    printf("shuffle() fails business rule #2: deckCount changed");
    fprintf(f, "shuffle() fails business rule #2: deckCount changed");
    testFail = 1; 
  }


  /* Business rule #3: Should result in the same number of each type of card in     the deck following shuffling as existed before shuffling.*/
  countCards(pre->deck[player], cardListPre);
  countCards(post->deck[player], cardListPost);

  for (int i = 0; i < CARDTYPES; i++){
    if (cardListPre[i] != cardListPost[i]){
      printf("\nshuffle() fails business rule #3: card types changed\n");
      fprintf(f,"\nshuffle() fails business rule #3: card types changed\n");
      testFail = 1; 
      break;
    }
  }
   
 
/** 4. The permutation after shuffling should look relatively random (i.e.
 **    the order of cards should be varied somewhat randomly).  */

  /* if player's deck isn't different then it isn't shuffled*/
  /* if the difference code doesn't match the correct player then the*/
  /* wrong deck was shuffled. */
  if (   !(  ( (differences[12]== 1) && (player == 0))
	     || ((differences[12] == 10) && (player == 1))
	     || ((differences[12] == 100) && (player == 2))
	     || ((differences[12] == 1000) && (player == 3)))){

    testFail =1; 
    printf("shuffle fails business rule #4: player's deck not changed\n");
    printf("or wrong player's deck changed.\n");
    fprintf(f,"shuffle fails business rule #4: player's deck not changed\n");
    fprintf(f,"or wrong player's deck changed.\n");

  }


  /*5. The other features of the state of the game should all be unchanged. */
  for (int i =0; i < MEMBERS; i++){
    if ((differences[i]) && (i != 12)){
      testFail =1; 
      printf("shuffle fails business rule #5:");
      printf(" state param #%d fails with code %d\n", i, differences[i]); 
      fprintf(f,"shuffle fails business rule #5:");
      fprintf(f," state param #%d fails with code %d\n", i, differences[i]); 
    }
  }
  

  if(!testFail){
    printf("\tTests passed.");
  }


  free(post); 
  free(differences);
  return testFail;
} 
