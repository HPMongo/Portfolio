/* -----------------------------------------------------------------------
 * Testing buyCard() method - using testUpdateCoins.c as the base code.
 *
 * Include the following lines in your makefile:
 *
 * unittest3: unittest3.c dominion.o rngs.o
 *      gcc -o unittest3 -g  unittest3.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

int main() {
    int seed = rand() %1000;
    int numPlayers = 2;
    int p;
    int randPos;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState oldGame;
	struct gameState newGame;
    int maxHandCount = 10;
    int failCount = 0;
    int passCount = 0;
	int player, round;
	
    printf ("<< TESTING unittest3 - buyCard() >>\n");

	for(p = 0; p < maxHandCount; p++)
	{
		memset(&oldGame, 23, sizeof(struct gameState));         //clear the game state
		memset(&newGame, 23, sizeof(struct gameState));         //clear the game state
		initializeGame(numPlayers, k, seed, &newGame);
		for(round = 0; round < 10; round++)
		{
			for(player = 0; player < numPlayers; player++)
			{
				memcpy(&oldGame, &newGame, sizeof(struct gameState));
				randPos = rand() % 26;		
				if(buyCard(randPos, &newGame) == 0)
				{
//					printf("Player %d - oldhand\n", player);
//					printf("numBuy: %d supply: %d coin: %d\n", oldGame.numBuys, oldGame.supplyCount[randPos], oldGame.coins);
//					printf(" - new hand\n");
//					printf("numBuy: %d supply: %d coin: %d\n", newGame.numBuys, newGame.supplyCount[randPos], newGame.coins);		
					if(oldGame.supplyCount[randPos] == newGame.supplyCount[randPos] + 1)
					{
						passCount++;
					}
					else
					{
						failCount++;
					}
				}
				else
				{
					failCount++;
				}	
			}
		}
	}
    printf("Number of cases passed: %d\n", passCount);
    printf("Number of cases failed: %d\n", failCount);

    return 0;
}
