/* -----------------------------------------------------------------------
 * Testing adventurer card - using random tester.
 *
 * Include the following lines in your makefile:
 *
 * random1: randometestadventurer.c dominion.o rngs.o
 *      gcc -o randomtest1 -g  randomtest1.c dominion.o rngs.o $(CFLAGS)
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
    int p, player, round, i;
	int topCard;
	int treasureCount;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState oldGame;
	struct gameState newGame;
    int maxHandCount = 5;
    int failCount = 0;
    int passCount = 0;
	int testCount = 0;
	int MAXRUN = 100;
    printf ("<< TESTING - adventurer - using random tester  >>\n");
	
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
				memcpy(&oldGame, &newGame, sizeof(struct gameState));
				adventurerCard(player, &newGame);
				
				// check if the last two cards are treasure cards
				treasureCount = 0;
				for(i = 0; i < 2; i++)
				{
					topCard = newGame.hand[player][newGame.handCount[player] - (i + 1)];
					if(topCard == copper || topCard == silver || topCard == gold)
					{
						treasureCount++;				
					}
				} 
				if((newGame.handCount[player] == oldGame.handCount[player] + 2) && (newGame.deckCount[player] == oldGame.deckCount[player] - 2) && treasureCount == 2)
				{
					passCount++;
				}
				else
				{
					printf("Failed - \t player %d\n", player);
					printf("\t expected deckCount: %d - actual deckCount: %d\n", oldGame.deckCount[player] - 2, newGame.deckCount[player]);
					printf("\t expected handCount: %d - actual handCount: %d\n", oldGame.handCount[player] + 2, newGame.handCount[player]);
					printf("\t expected treasureCount for last two cards: %d - actual treasureCount: %d\n", 2, treasureCount);
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
