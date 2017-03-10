/* -----------------------------------------------------------------------
 * Testing village card - using testUpdateCoins.c as the base code.
 *
 * Include the following lines in your makefile:
 *
 * unittest1: cardtest3.c dominion.o rngs.o
 *      gcc -o cardtest3 -g  cardtest3.c dominion.o rngs.o $(CFLAGS)
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
    printf ("<< TESTING cardtest3 - village  >>\n");
	
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
				villageCard(village, player, &newGame);
/*
 				printf("Old stuff - player %d\n", player);
				printf("deckCount: %d handCount: %d ", oldGame.deckCount[player], oldGame.handCount[player]);
				printf(" playedCardCount: %d", oldGame.playedCardCount);
				printf(" discardCount: %d action: %d\n", oldGame.discardCount[player], oldGame.numActions);
				printf("New stuff\n");				
				printf("deckCount: %d handCount: %d ", newGame.deckCount[player], newGame.handCount[player]);
				printf(" playedCardCount: %d", newGame.playedCardCount);
				printf(" discardCount: %d action: %d\n", newGame.discardCount[player], newGame.numActions);
*/
				if((newGame.numActions == oldGame.numActions + 2) && 
					((newGame.deckCount[player] + newGame.playedCardCount == oldGame.deckCount[player] + oldGame.playedCardCount))
				  )
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
