/* -----------------------------------------------------------------------
 * Testing council room card - using random tester.
 *
 * Include the following lines in your makefile:
 *
 * random2: randometestcard.c dominion.o rngs.o
 *      gcc -o randomtest2 -g  randomtestcard.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

int main() {
    int seed;
	int numPlayers;
    int p, player, round;
	int otherPlayers;
    int otherDrawCount;
	int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState oldGame;
	struct gameState newGame;
    int maxHandCount = 5;
    int failCount = 0;
    int passCount = 0;
	int testCount = 0;
	int MAXRUN = 100;
    printf ("<< TESTING - council room card - using random tester  >>\n");
	
	for(testCount = 0; testCount < MAXRUN; testCount++)
	{	
	//generate random seed
	seed = rand() % 100000;
	// generate random number of players    		
	numPlayers = rand() % 3 + 2;
	printf("Test #%d - seed:%d \n", testCount, seed);
 	
	for(p = 0; p < maxHandCount; p++)
	{
		memset(&oldGame, 23, sizeof(struct gameState));         //clear the game state
		memset(&newGame, 23, sizeof(struct gameState));         //clear the game state
		initializeGame(numPlayers, k, seed, &newGame);
		for(round = 0; round < 2; round++)
		{
			for(player = 0; player < numPlayers; player++)
			{
				otherDrawCount = 0;
				memcpy(&oldGame, &newGame, sizeof(struct gameState));
				//councilRoomCard(council_room, player, &newGame);
				playCard(council_room, -1, -1, -1, &newGame);

				// count number of players whom drew a card
				for (otherPlayers = 0; otherPlayers < numPlayers; otherPlayers++)
				{
					if(otherPlayers != player)
					{
						if(newGame.handCount[otherPlayers] == oldGame.handCount[otherPlayers] + 1)
						{
							otherDrawCount++;
						}
					}
				}

				if((newGame.numBuys == oldGame.numBuys + 1) &&
					(newGame.handCount[player] == oldGame.handCount[player] + 4) &&
					(otherDrawCount == numPlayers - 1) &&
					(newGame.deckCount[player] == oldGame.deckCount[player] - 4)	
				  )
				{
					passCount++;
				}
				else
				{
					printf("Failed - \t player %d\n", player);
					printf("\t expected buy: %d - actual buy: %d\n", oldGame.numBuys + 1, newGame.numBuys);
					printf("\t expected handCount: %d - actual handCount: %d\n", oldGame.handCount[player] + 4, newGame.handCount[player]);
					printf("\t expected deckCount: %d - actual deckCount: %d\n", oldGame.deckCount[player] - 4, newGame.deckCount[player]);	
//					printf("\t expected playedCardCount: %d - actual playedCardCount: %d\n", oldGame.playedCardCount, newGame.playedCardCount);
					printf("\t expected number of other players whom drew a card: %d - actual count: %d\n", numPlayers - 1, otherDrawCount);
					failCount++;
				}

			}	
		}		
	}
	}
	printf("\n < < < < RESULTS > > > >\n\n");
    printf("\t Number of cases passed: %d\n", passCount);
    printf("\t Number of cases failed: %d\n\n", failCount);

    return 0;
}
