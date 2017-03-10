/* -----------------------------------------------------------------------
 * Testing updateCoins() method - using testUpdateCoins.c as the base code.
 *
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

int main() {
    int i;
    int seed = rand() % 1000;
    int numPlayer = 2;
    int maxBonus = 10;
    int p, r, handCount;
    int bonus;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G;
    int maxHandCount = 5;
    int failCount = 0;
    int passCount = 0;
    // arrays of all coppers, silvers, and golds
    int coppers[MAX_HAND];
    int silvers[MAX_HAND];
    int golds[MAX_HAND];
    for (i = 0; i < MAX_HAND; i++)
    {
        coppers[i] = copper;
        silvers[i] = silver;
        golds[i] = gold;
    }

    printf ("<< TESTING unittest1 - updateCoins() >>\n");
    for (p = 0; p < numPlayer; p++)
    {
        for (handCount = 1; handCount <= maxHandCount; handCount++)
        {
            for (bonus = 0; bonus <= maxBonus; bonus++)
            {
//              printf("Test player %d with %d treasure card(s) and %d bonus.\n", p, handCount, bonus);
                memset(&G, 23, sizeof(struct gameState));   // clear the game state
                r = initializeGame(numPlayer, k, seed, &G); // initialize a new game
                G.handCount[p] = handCount;                 // set the number of cards on hand
                memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper
                updateCoins(p, &G, bonus);
//            	printf("G.coins = %d, expected = %d\n", G.coins, handCount * 1 + bonus);

                if (G.coins == handCount * 1 + bonus) // check if the number of coins is correct
		{
//			printf("Copper coins passed\n");
			passCount++;
		}
		else
		{
//			printf("Copper coins failed\n");
			failCount++;
		}
                memcpy(G.hand[p], silvers, sizeof(int) * handCount); // set all the cards to silver
                updateCoins(p, &G, bonus);
//              printf("G.coins = %d, expected = %d\n", G.coins, handCount * 2 + bonus);
 
               	if (G.coins == handCount * 2 + bonus) // check if the number of coins is correct
		{
//			printf("Silver coins passed\n");
			passCount++;
		}
		else
		{
//			printf("Silver coins failed\n");
			failCount++;
		}

                memcpy(G.hand[p], golds, sizeof(int) * handCount); // set all the cards to gold
                updateCoins(p, &G, bonus);
//              printf("G.coins = %d, expected = %d\n", G.coins, handCount * 3 + bonus);
                if (G.coins == handCount * 3 + bonus) // check if the number of coins is correct
		{
//			printf("Gold coins passed\n");
			passCount++;
		}
		else
		{
//			printf("Gold coins failed\n");
			failCount++;
		}		
            }
        }
    }

    printf("Number of cases passed: %d\n", passCount);
    printf("Number of cases failed: %d\n", failCount);

    return 0;
}
