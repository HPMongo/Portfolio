/* -----------------------------------------------------------------------
 * Testing councilRoom card - using testUpdateCoins.c as the base code.
 *
 * Include the following lines in your makefile:
 *
 * unittest1: cardtest4.c dominion.o rngs.o
 *      gcc -o cardtest4 -g  cardtest4.c dominion.o rngs.o $(CFLAGS)
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
    int p, player, otherPlayers, round;
	int othersDrawCount;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState oldGame;
	struct gameState newGame;
    int maxHandCount = 5;
    int failCount = 0;
    int passCount = 0;
    printf ("<< TESTING cardtest4 - councilRoom  >>\n");
	
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
				playCouncilRoom(&newGame, player, council_room);
				othersDrawCount = 0;

 				printf("Player %d\n", player);
				printf("Old - deckCount: %d handCount: %d ", oldGame.deckCount[player], oldGame.handCount[player]);
				printf(" playedCardCount: %d", oldGame.playedCardCount);
				printf(" discardCount: %d numBuys: %d\n", oldGame.discardCount[player], oldGame.numBuys);
				printf("New - deckCount: %d handCount: %d ", newGame.deckCount[player], newGame.handCount[player]);
				printf(" playedCardCount: %d", newGame.playedCardCount);
				printf(" discardCount: %d numBuys: %d\n", newGame.discardCount[player], newGame.numBuys);

				// tally the number of other players whom drew a card		
				for (otherPlayers = 0; otherPlayers < numPlayers; otherPlayers++)
				{
					if(otherPlayers != player)
					{
						if(newGame.handCount[otherPlayers] == oldGame.handCount[otherPlayers] + 1)
						{
							othersDrawCount++;
						}
					}
				}
				if((newGame.numBuys == oldGame.numBuys + 1) && 						//new buy should increase by 1
				   (newGame.handCount[player] == oldGame.handCount[player] + 4) &&	//player's hand should increase by 4
				   (othersDrawCount == numPlayers + 1))								//other players should add a card
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
