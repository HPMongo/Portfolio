/* -----------------------------------------------------------------------
 * Testing isGameOver() method - using testUpdateCoins.c as the base code.
 *
 * Include the following lines in your makefile:
 *
 * unittest1: unittest4.c dominion.o rngs.o
 *      gcc -o unittest4 -g  unittest4.c dominion.o rngs.o $(CFLAGS)
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
    int p, round;
    int randPro, randSup;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState oldGame;
	struct gameState newGame;
    int maxHandCount = 10;
    int failCount = 0;
    int passCount = 0;

    printf ("<< TESTING unittest4 - isGameOver() >>\n");
	
	for(p = 0; p < maxHandCount; p++)
	{
		memset(&oldGame, 23, sizeof(struct gameState));         //clear the game state
		memset(&newGame, 23, sizeof(struct gameState));         //clear the game state
		initializeGame(numPlayers, k, seed, &newGame);
		for(round = 0; round < 10; round++)
		{
			randPro = rand() % 2;
			randSup = rand() % 26;
			memcpy(&oldGame, &newGame, sizeof(struct gameState));
			if(randPro == 1)
			{
				if(newGame.supplyCount[province] > 0)
					newGame.supplyCount[province]--;
			}		
			if( newGame.supplyCount[randSup] > 0)
			{
				newGame.supplyCount[randSup]--;
			}

			if(isGameOver(&newGame) == 0)
			{
/*
				printf("Game is not over\n");
				printf("Old province count: %d Last supply: %d\n", oldGame.supplyCount[province], oldGame.supplyCount[randSup]);
				printf("New province count: %d Last supply: %d\n", newGame.supplyCount[province], newGame.supplyCount[randSup]);
*/
				if((newGame.supplyCount[province] < 1 && newGame.supplyCount[province] == oldGame.supplyCount[province] - 1) || (newGame.supplyCount[randSup] < 1 && newGame.supplyCount[randSup] == oldGame.supplyCount[randSup] - 1))
				{
					failCount++;
//					printf("Fail\n");
				}
				else
				{
					passCount++;
//					printf("Pass\n");
				}
			}
			else
			{
/*
				printf("Game is over\n");	
				printf("Old province count: %d Last supply: %d\n", oldGame.supplyCount[province], oldGame.supplyCount[randSup]);
				printf("New province count: %d Last supply: %d\n", newGame.supplyCount[province], newGame.supplyCount[randSup]);
*/
				if((newGame.supplyCount[province] < 1 && newGame.supplyCount[province] == oldGame.supplyCount[province] - 1) || (newGame.supplyCount[randSup] < 1 && newGame.supplyCount[randSup] == oldGame.supplyCount[randSup] - 1))
				{
					passCount++;
//					printf("Pass\n");
				}
				else
				{
					failCount++;
//					printf("Fail\n");
				}
	
			}		
		}		
	}
    printf("Number of cases passed: %d\n", passCount);
    printf("Number of cases failed: %d\n", failCount);

    return 0;
}
