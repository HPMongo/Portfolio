#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>


int checkAdventurerCard(int p, struct gameState *post);
int checkAdventurerCard(int p, struct gameState *post) {
  post->whoseTurn = p;
  struct gameState pre;
  memcpy (&pre, post, sizeof(struct gameState));


  int r, i, cardDrawn;
  int z = 0;
  int temphand[MAX_HAND];
  int handPos = rand() % pre.handCount[p];
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
      while(z-1>=0) {
        pre.discard[p][pre.discardCount[p]++] = temphand[z-1]; // discard all cards in play that have been drawn
        z -= 1;
      }
  assert (r == 0);
  discardCard(handPos, p, &pre, 0);
  if (memcmp(&pre, post, sizeof(struct gameState)) == 0)
  {
    printf("Testing adventurerCard() passed");
  } else {
    printf("Testing adventurerCard() failed");
  }
  return 0;
}

int main () {

  srand(time(NULL));
 
  int i, n, p;

 int handRandom, deckRandom, discardRandom, playedRandom;
  struct gameState G;

  printf ("Testing adverturerCard.\n");

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
    checkAdventurerCard(p, &G);
  }

  return 0;
}
