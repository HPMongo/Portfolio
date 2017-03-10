/* -----------------------------------------------------------------------
 * Unit test to test getCost() function.
 * Include the following lines in your makefile:
 *
 * unittest1: unittest1.c dominion.o rngs.o
 *      gcc -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

int getRealCost(int i);
//Return the cost of card i

int main() {

  srand(time(NULL));
  int i, p, randomCard;
  int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
  struct gameState G;
  int numPlayers = 2;


  int seed = rand() % + 9999;
  int handRandom, deckRandom;
  printf ("Testing getCost().\n");

  for (p = 0; p < numPlayers; p++) 
  {
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    assert(initializeGame(numPlayers, k, seed, &G) == 0);
        printf("Testing player %d hand card cost\n", p );
        handRandom = rand() % MAX_HAND;
        G.handCount[p] = handRandom;
        for (i = 0; i < handRandom; i++) 
        {
            randomCard = rand() % 27;
            G.hand[p][i] = randomCard;
            assert(getCost(G.hand[p][i]) == getRealCost(randomCard));
        }
        printf("Testing player %d deck card cost\n", p );
        deckRandom = rand() % MAX_DECK;
        G.deckCount[p] = deckRandom;
        for (i = 0; i < deckRandom; i++) 
        {
            randomCard = rand() % 27;
            G.deck[p][i] = randomCard;
            assert(getCost(G.deck[p][i]) == getRealCost(randomCard));
        }
 
  }
  printf("All test passed for getCost() \n");
  return 0;
}

int getRealCost(int i)
{
    if (i == curse) {
            return 0;
        } else if (i == estate) {
            return 2;
        } else if (i == duchy) {
            return 5;
        } else if (i == province) {
            return 8;
        } else if (i == copper) {
            return 0;
        } else if (i == silver) {
            return 3;
        } else if (i == gold) {
            return 6;
        } else if (i == adventurer) {
            return 6;
        } else if (i == council_room) {
            return 5;
        } else if (i == feast) {
            return 4;
        } else if (i == gardens) {
            return 4;
        } else if (i == mine) {
            return 5;
        } else if (i == remodel) {
            return 4;
        } else if (i == smithy) {
            return 4;
        } else if (i == village) {
            return 3;
        } else if (i == baron) {
            return 4;
        } else if (i == great_hall) {
            return 3;
        } else if (i == minion) {
            return 5;
        } else if (i == steward) {
            return 3;
        } else if (i == tribute) {
            return 5;
        } else if (i == ambassador) {
            return 3;
        } else if (i == cutpurse) {
            return 4;
        } else if (i == embargo) {
            return 2;
        } else if (i == outpost) {
            return 5;
        } else if (i == salvager) {
            return 4;
        } else if (i == sea_hag) {
            return 4;
        } else if (i == treasure_map) {
            return 4;
        } else {
            return -1;
        }
}

    
        
