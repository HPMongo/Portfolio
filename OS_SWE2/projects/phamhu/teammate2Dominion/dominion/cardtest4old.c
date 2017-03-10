/* -----------------------------------------------------------------------
 * Unit test for cutPurseCard() function
 * Include the following lines in your makefile:
 *
 * cardtest4: cardtest4.c dominion.o rngs.o
 *      gcc -o cardtest4 -g  cardtest4.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>

int checkCutpurseCard(int p, struct gameState *post) {
  
  struct gameState pre;
  post->whoseTurn = p;
  memcpy (&pre, post, sizeof(struct gameState));

  int i, j, r;
  
  int handPos = rand() % pre.handCount[p];
  r = cutpurseCard(post, handPos);

  updateCoins(p, &pre, 2);
   printf("After gaining 2 coins, tester coinCount: %d\n", pre.coins);
   printf("After gaining 2 coins, testee coinCount: %d\n", post->coins);
  for (i = 0; i < pre.numPlayers; i++)
     {
       if (i != p)
       {
         for (j = 0; j < pre.handCount[i]; j++)
          {
            if (pre.hand[i][j] == copper)
            {
              discardCard(j, i, &pre, 0);
              break;
            }
            if (j == pre.handCount[i])
            {
              int revealedCard = rand () % pre.handCount[i];
              printf("Player %d reveals card number %d\n", i, pre.hand[i][revealedCard]);          
              break;
            }   
          }          
        }        
       }        
    
  assert (r == 0);
  discardCard(handPos, p, &pre, 0);
  if (memcmp(&pre, post, sizeof(struct gameState)) == 0) {
    printf("cutpurseCard() function unit test passed for player %d!\n", p);
  } else {
    printf("cutpurseCard() function unit test failed for player %d\n", p);
  }
  return 0;
}

int main () 
{
  srand(time(NULL));
  int i, p;
  int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
  struct gameState G;
  int numPlayers = 2;

  int seed = rand() % + 9999;
  int handRandom, deckRandom, discardRandom;
  printf ("Testing cutPurseCard().\n");

  for (p = 0; p < numPlayers; p++) 
  {
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    assert(initializeGame(numPlayers, k, seed, &G) == 0);

        handRandom = rand() % MAX_HAND;
        G.handCount[p] = handRandom;
        for (i = 0; i < handRandom; i++) 
        {
            G.hand[p][i] = rand() % 27;
        }
        
        deckRandom = rand() % MAX_DECK;
        G.deckCount[p] = deckRandom;
        for (i = 0; i < deckRandom; i++) 
        {
            G.deck[p][i] = rand() % 27;
        }
 
        discardRandom = rand () % MAX_DECK;
        G.discardCount[p] = discardRandom;
        for (i = 0; i < discardRandom; i++) 
        {
            G.discard[p][i] = rand() % 27;
        }
    checkCutpurseCard(p, &G);
  }
   
  return 0;

}