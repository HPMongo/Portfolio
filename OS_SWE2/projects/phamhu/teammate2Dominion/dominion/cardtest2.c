/* -----------------------------------------------------------------------
 * Testing adventurer card - using testUpdateCoins.c as the base code.
 *
 * Include the following lines in your makefile:
 *
 * unittest1: cardtest2.c dominion.o rngs.o
 *      gcc -o cardtest2 -g  cardtest2.c dominion.o rngs.o $(CFLAGS)
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
    int p, player, round;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState oldGame;
	struct gameState newGame;
    int maxHandCount = 5;
    int failCount = 0;
    int passCount = 0;

    printf ("<< TESTING cardtest2 - adventurer  >>\n");
	
	for(p = 0; p < maxHandCount; p++)
	{
		memset(&oldGame, 23, sizeof(struct gameState));         //clear the game state
		memset(&newGame, 23, sizeof(struct gameState));         //clear the game state
		initializeGame(numPlayers, k, seed, &newGame);
		for(round = 0; round < 2; round++)
		{
			for(player = 0; player < numPlayers; player++)
			{
				memcpy(&oldGame, &newGame, sizeof(struct gameState));
				adventurerCard(&newGame, adventurer);

 				printf("Old stuff - player %d\n", player);
				printf("deckCount: %d handCount: %d\n", oldGame.deckCount[player], oldGame.handCount[player]);
				printf("New stuff\n");				
				printf("deckCount: %d handCount: %d\n", newGame.deckCount[player], newGame.handCount[player]);

				if((newGame.handCount[player] == oldGame.handCount[player] + 2) && (newGame.deckCount[player] == oldGame.deckCount[player] - 2))
				{
					passCount++;
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
