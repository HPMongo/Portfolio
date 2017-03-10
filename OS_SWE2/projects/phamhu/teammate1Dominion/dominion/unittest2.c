/* -----------------------------------------------------------------------
 * Testing updateCoins() method - using testUpdateCoins.c as the base code.
 *
 * Include the following lines in your makefile:
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

int main() {
    int seed = rand() % 1000;
    int numPlayers = 2;
    int p;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState oldGame;
	struct gameState newGame;
    int failCount = 0;
    int passCount = 0;
	int maxPlay = 100;
	int player;

    printf ("<< TESTING unittest2 - drawCard() >>\n");
    
	for(p = 0; p < maxPlay; p++)
	{
		memset(&oldGame, 23, sizeof(struct gameState));		//clear the game state
		memset(&newGame, 23, sizeof(struct gameState));
		initializeGame(numPlayers, k, seed, &newGame);
		memcpy(&oldGame, &newGame, sizeof(struct gameState));
	
		for (player = 0; player < numPlayers; player++)
		{
			memcpy(&oldGame, &newGame, sizeof(struct gameState));
			if(drawCard(player, &newGame) == 0)
			{
				printf("Player - %d - Old stuff\n", player);
				printf("deckCount: %d handCount: %d\n", oldGame.deckCount[player], oldGame.handCount[player]);
				printf("New stuff\n");
				printf("deckCount: %d handCount: %d\n", newGame.deckCount[player], newGame.handCount[player]);
				if((newGame.deckCount[player] == oldGame.deckCount[player] - 1) && (newGame.handCount[player] == oldGame.handCount[player] + 1))
				{
					passCount++;
//					printf("Pass\n");
				}
				else
				{
					printf("Fail\n");
					failCount++;
				}	
			}	
			else
			{
				failCount++;	
			}
			memset(&oldGame, 23, sizeof(struct gameState));		//clear the game state		
		}
	}	
    printf("Number of cases passed: %d\n", passCount);
    printf("Number of cases failed: %d\n", failCount);

    return 0;
}
