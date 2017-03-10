/* -----------------------------------------------------------------------
 * Unit test for isGameOver():
 *
 * unittest2: unittest2.c dominion.o rngs.o
 *      gcc -o unittest2 -g  unittest2.c dominion.o rngs.o $(CFLAGS)
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

int main() {
    srand(time(NULL));
    int numPlayer = 2;
    int seed = rand() % 9999;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    
    struct gameState G;
    printf ("TESTING isGameOver():\n");
    assert(initializeGame(numPlayer, k, seed, &G) == 0); 

   
    G.supplyCount[province] = 0;
   
    assert(isGameOver(&G) == 1);
    printf("Testing succefully on game over due to run out of province tested\n");
    G.supplyCount[province] = 8;
    int i, r1, r2, r3;
    for (i = 0; i < 2; i++) 
    {
        r1 = rand() % 27;
        r2 = rand() % 27;
        r3 = rand() % 27;
        while (r1 == r2 || r1 == r3 || r2 == r3) 
        {
            r1 = rand() % 27;
            r2 = rand() % 27;
            r3 = rand() % 27;
        }  
        G.supplyCount[r1] = 0;
        G.supplyCount[r2] = 0;
        G.supplyCount[r3] = 0;
        if (isGameOver(&G) == 1) {
            printf("Tesing isGameOver() is passed on empty of three supplyCount\n");
        } else {
            printf("Testing isGameOver() failed on empty of three supplyCount\n");
        }

    }

    printf("isGameOver() function tests passed!\n");

    return 0;
}
