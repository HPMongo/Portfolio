/* -----------------------------------------------------------------------
 * Unit test for feastCard() function
 * Include the following lines in your makefile:
 *
 * cardtest3: cardtest3.c dominion.o rngs.o
 *      gcc -o cardtest3 -g  cardtest3.c dominion.o rngs.o $(CFLAGS)
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


int checkFeastCard(int supplyPos, int p, struct gameState *post) {
  
  struct gameState pre;
  post->whoseTurn = p;
  memcpy (&pre, post, sizeof(struct gameState));

  int r;
  
  int handPos = rand() % pre.handCount[p];
  r = feastCard(supplyPos, post, handPos);
  if (supplyCount(supplyPos, &pre) <= 0){
    printf("None of that card left, sorry!(This shouldn't happen in the test) \n");
  } else if (getCost(supplyPos) > 9) {
    printf("That card is too expensive!(This should't happen in the test)\n");
  } else {
    gainCard(supplyPos, &pre, 0, p);
  }
  printf("After gaining a card tester discardCount: %d, gained cards: %d \n", pre.discardCount[p], pre.discard[p][pre.discardCount[p]-1]);
  printf("After gaining a card, testee discardCount: %d, gained cards: %d \n", post->discardCount[p], post->discard[p][post->handCount[p]-1]);
  
    
  assert (r == 0);
  discardCard(handPos, p, &pre, 0);
  if (memcmp(&pre, post, sizeof(struct gameState)) == 0) {
    printf("feastCard() function unit test passed for player %d\n", p);
  } else {
    printf("feastCard() function unit test failed for player %d \n", p);
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
  printf ("Testing feastCard().\n");
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

        //choose a supply position to gain a card
        int supplyPos;
        int isKingdomCard = 0;
        i = 0;
        while (isKingdomCard == 0)
        {
          supplyPos = rand() % 27;
          for (i = 0; i < 10; i++) 
          {
             if (k[i] == supplyPos || supplyPos < 7) { //make sure the card in the game
              isKingdomCard = 1;
              break;
            }
          }          
        } 
       
    checkFeastCard(supplyPos, p, &G);
  }
  return 0;
}