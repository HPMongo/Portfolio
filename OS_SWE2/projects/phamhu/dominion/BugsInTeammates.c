//Program: BugsInTeammates
//Author: Huy Pham
//Created date: 02/27/2016
//Description: Describe bugs in teammates' code

Here are the bugs that I found after executing the test suit:

Teammate 1 (Jeremy Fischman)
	smithy - hand count is correct but the deck count decreases one more than it should. It's possible that the card is trashed instead of putting back to the deck.
	adventurer - the hand count is one more than the normal hand count (3 instead of 2). 
	village - deck count is one less than it should be. It's possible that the card is being trashed instead of putting back to the deck.
	council_room - hand count is one more than the normal (5 instead of 4).

Teammate 2 (Xisheng Wang)
	smithy - hand count is correct but deck count also decrease one more than it should.
	adventurer - hand drawn is one more than it should (3 instead of 2) while deck count is decreased by 4 instead of 2
	village - action doesn't increase by 2 and player doesn't draw addtional card (should be 1)
	council_room - buy doesn't increase by 1 and players don't draw additional cards (should be 4 for the current player and 1 for everyone else)


<Code coverage>
--Teammate 1
For 4 cards and 4 functions test -
Lines executed:35.37% of 574
Branches executed:34.22% of 415
Taken at least once:26.51% of 415
Calls executed:23.16% of 95

For random test card -
Lines executed:22.47% of 574
Branches executed:19.76% of 415
Taken at least once:17.11% of 415
Calls executed:11.58% of 95

For random adventurer test - 
Lines executed:22.82% of 574
Branches executed:20.24% of 415
Taken at least once:18.07% of 415
Calls executed:10.53% of 95

--Teammate 2
For 4 cards and 4 functions test - 
Lines executed:35.74% of 596
Branches executed:35.76% of 425
Taken at least once:26.35% of 425
Calls executed:21.78% of 101

For random test card -
Lines executed:18.29% of 596
Branches executed:17.88% of 425
Taken at least once:14.82% of 425
Calls executed:8.91% of 101
dominion.c:creating 'dominion.c.gcov'

For random adventurer test -
Lines executed:25.34% of 596
Branches executed:22.12% of 425
Taken at least once:19.53% of 425
Calls executed:11.88% of 101
