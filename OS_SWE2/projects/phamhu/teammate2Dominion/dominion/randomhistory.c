randomtestadventurer.c : 
Coverage:  
On dominion.c:
Lines executed:13.76% of 596
Branches executed:9.41% of 425
Taken at least once:8.24% of 425
Calls executed:6.93% of 101
On adventurerCard() function:  called 2000,  returned 100% blocks executed 100%.
I first generated randomly game states. Then randomly assigned the player number to 1 to 2, deckCount, handCount, discardCount and playedCount within range of game MAX. The purpose is to let game variables have meaning setup. The game state was copied to a new state holder to run as tester and adventurerCard() function was called to get testee state. Two states were compared to see if the random test is passed. The test iterated 2000 times. All tests were failed because of the bug introduced from assignment 2.  The coverage on dominion.c is only 13.75% since it only tested one function. However, the test covered 100% of testing function. 
Randomtestcard.c: (smithyCard())
On dominion.c 
Lines executed:11.05% of 579
Branches executed:6.59% of 425
Taken at least once:5.41% of 425
Calls executed:5.94% of 101
On smithyCard() function:  called 2000,  returned 100% blocks executed 100%.
Random game setup was same as randomtestadventurer.c. All tests were failed because of the bug introduced from assignment 2.  The coverage on dominion.c is only 11.05% since it only tested one function. However, the test covered 100% of testing function.
