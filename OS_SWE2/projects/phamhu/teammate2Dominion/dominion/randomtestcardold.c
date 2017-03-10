#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>


int checkSmithyCard(int p, struct gameState *post);
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
   
  if (memcmp(&pre, post, sizeof(struct gameState)) == 0) {
    printf("Testing smithyCard() passed");
  } else {
    printf("Testing smithyCard() failed");
  }
  return 0;
}

int main () {

  srand(time(NULL));
 
  int i, n, p;

 int handRandom, deckRandom, discardRandom, playedRandom;
  struct gameState G;

  printf ("Testing smithyCard.\n");

  printf ("RANDOM TESTS.\n");

  SelectStream(2);
  PutSeed(3);

  for (n = 0; n < 2000; n++) {
    for (i = 0; i < sizeof(struct gameState); i++) {
      ((char*)&G)[i] = floor(Random() * 256);
    }
    p = floor(Random() * 2);
     handRandom = rand() % MAX_HAND + 1;
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
 
        discardRandom = rand () % MAX_DECK + 1;
        G.discardCount[p] = discardRandom;
        for (i = 0; i < discardRandom; i++) 
        {
            G.discard[p][i] = rand() % 27;
        }
        playedRandom = rand () % MAX_DECK;
        G.playedCardCount = playedRandom;
        for (i = 0; i < playedRandom; i++) 
        {
            G.playedCards[i] = rand() % 27;
        }
    checkSmithyCard(p, &G);
  }

  return 0;
}
