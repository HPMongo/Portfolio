/* -----------------------------------------------------------------------
 * Unit test for adventurerCard() function
 * Include the following lines in your makefile:
 *
 * cardtest1: cardtest1.c dominion.o rngs.o
 *      gcc -o cardtest1 -g  cardtest1..c dominion.o rngs.o $(CFLAGS)
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


int checkAdventurerCard(int p, struct gameState *post) {
  printf("testing player %d \n", p);
  struct gameState pre;
  post->whoseTurn = p;
  memcpy (&pre, post, sizeof(struct gameState));


  int r, i, cardDrawn;
  int z = 0;
  int temphand[MAX_HAND];
  int handPos = rand() % + pre.handCount[p];
  r = adventurerCard(post, handPos);
  int drawntreasure = 0;

  while(drawntreasure < 2)
  {
   
    if (pre.deckCount[p] < 1) {
    
      for(i = 0; i < pre.discardCount[p]; i++){
        pre.deck[p][i] = pre.discard[p][i];//Move to deck
        pre.deckCount[p]++;
        pre.discard[p][i] = -1;
        pre.discardCount[p]--;
      }
      shuffle(p, &pre);
    }
    drawCard(p, &pre);
    cardDrawn = pre.hand[p][pre.handCount[p]-1];//top card of hand is most recently drawn card.
    if (cardDrawn == copper || cardDrawn == silver || cardDrawn == gold)
      drawntreasure++;
    else {
      temphand[z]=cardDrawn;
      pre.handCount[p]--; //remove the top card (the most recently drawn one).
      z++;
    }
  }
    printf("After drawing two cards, tester handCount: %d, drawn cards: %d, %d \n", pre.handCount[p], pre.hand[p][pre.handCount[p]-1], pre.hand[p][pre.handCount[p]-2]);
    printf("After drawing two cards, testee handCount: %d, drawn cards: %d, %d \n", post->handCount[p], post->hand[p][post->handCount[p]-1], post->hand[p][post->handCount[p]-2]);
      while(z-1>=0) {
        pre.discard[p][pre.discardCount[p]++] = temphand[z-1]; // discard all cards in play that have been drawn
        z -= 1;
      }
  assert (r == 0);
  discardCard(handPos, p, &pre, 0);
  if (memcmp(&pre, post, sizeof(struct gameState)) == 0)
  {
    printf("Testing adventurerCard() passed for player: %d \n", p);
  } else {
    printf("Testing adventurerCard() failed for player: %d \n",p);
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
  printf ("Testing adventurerCard().\n");

 

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

    checkAdventurerCard(p, &G);


  }
   
  return 0;

}