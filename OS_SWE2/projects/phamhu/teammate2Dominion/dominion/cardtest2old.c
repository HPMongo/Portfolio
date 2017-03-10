/* -----------------------------------------------------------------------
 * Unit test for smithyCard() function
 * Include the following lines in your makefile:
 *
 * cardtest2: cardtest2.c dominion.o rngs.o
 *      gcc -o cardtest2 -g  cardtest2.c dominion.o rngs.o $(CFLAGS)
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


int checkSmithyCard(int p, struct gameState *post) {
  
  struct gameState pre;
  post->whoseTurn = p;
  memcpy (&pre, post, sizeof(struct gameState));


  int r;
  
  int handPos = rand() % pre.handCount[p];
  r = smithyCard(post, handPos);
  int drawNum = 0;

  while(drawNum < 3)
  {
    drawCard(p, &pre);
    drawNum++;  
  }
   
  assert (r == 0);
  discardCard(handPos, p, &pre, 0);
   printf("After drawing three cards, tester handCount: %d, drawn cards: %d, %d, %d \n", pre.handCount[p], pre.hand[p][pre.handCount[p]-1], pre.hand[p][pre.handCount[p]-2], pre.hand[p][pre.handCount[p]-3]);
    printf("After drawing three cards, testee handCount: %d, drawn cards: %d, %d, %d \n", post->handCount[p], post->hand[p][post->handCount[p]-1], post->hand[p][post->handCount[p]-2], post->hand[p][post->handCount[p]-3]);
    
  if (memcmp(&pre, post, sizeof(struct gameState)) == 0) {
    printf("Testing smithyCard() passed for player: %d\n", p);
  } else {
    printf("Testing smithyCard() failed for player: %d\n", p);
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
  printf ("Testing smithyCard().\n");

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

    checkSmithyCard(p, &G);


  }
  

  return 0;

}