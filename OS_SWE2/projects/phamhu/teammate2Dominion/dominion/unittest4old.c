/* -----------------------------------------------------------------------
 * Unit test for gainCard() function
 * Include the following lines in your makefile:
 *
 * unittest4: unittest4.c dominion.o rngs.o
 *      gcc -o unittest4 -g  unittest4.c dominion.o rngs.o $(CFLAGS)
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


int checkGainCard(int supplyPos, int p, struct gameState *post) {
  srand(time(NULL));
  struct gameState pre;
  memcpy (&pre, post, sizeof(struct gameState));

  int j;
  
  int toTag = 0;   
 
  for (j = 0; j < 100; j++) 
  {
    toTag = rand() % 3;

    if (toTag == 1) {
        pre.deck[p][pre.deckCount[p]] = supplyPos;
        pre.deckCount[p]++;
      } else if (toTag == 2) {
        pre.hand[p][pre.handCount[p]] = supplyPos;
        pre.handCount[p]++;
      } else {
        pre.discard[p][pre.discardCount[p]] = supplyPos;
        pre.discardCount[p]++;
      }
    pre.supplyCount[supplyPos]--;
    assert (gainCard (supplyPos, post, toTag, p) == 0);
    assert(memcmp(&pre, post, sizeof(struct gameState)) == 0);
  }
  //printf ("Testing gainCard() function on player %d passed!", p);
  return 0;
}

int main () {

  int i, n, p;

  struct gameState G;

  printf ("Testing gainCard() \n");

  SelectStream(2);
  PutSeed(3);

  for (n = 0; n < 2000; n++) {
    for (i = 0; i < sizeof(struct gameState); i++) {
      ((char*)&G)[i] = floor(Random() * 256);
    }
  p = floor(Random() * 2);

  int randDeckCount = floor(Random() * MAX_DECK);
  G.deckCount[p] = randDeckCount;
  for (i = 0; i < randDeckCount; i++) 
    {
      G.deck[p][i] = rand() % 27;
    }

    int randHandCount = floor(Random() * MAX_HAND);
    G.handCount[p] = randHandCount;
    for (i = 0; i < randHandCount; i++) 
    {
      G.hand[p][i] = rand() % 27;
    }

    int randDiscardCount = floor(Random() * MAX_DECK);
    G.discardCount[p] = randDiscardCount;
    for (i = 0; i < randDiscardCount; i++) 
    {
      G.discard[p][i] = rand() % 27;
    }

    int supplyPos = rand() % 27;
    G.supplyCount[supplyPos] = 1000;     
    
    checkGainCard(supplyPos, p, &G);
  }
  printf("Testing gainCard() function passed! \n");
  return 0;

}