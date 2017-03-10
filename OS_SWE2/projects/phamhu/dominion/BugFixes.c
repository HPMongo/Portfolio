//Program: BugFixes.c
//Author: Huy Pham
//Created date: 02/27/2016
//Description: Describe the process of identifying and fixing the bugs in dominion.c program

The following bugs that were introduced with earlier version of the program through code refactoring are now fixed:
in smithyCard():
	correct card increase from 4 to 3
in villageCard():
	set trashcard switch back to false (0) (discardCard(handPos, currentPlayer, state, 0));
in councilRoomCard():
	set trashcard switch back to false (0) (discardCard(handPos, currentPlayer, state, 0));
in adventurerCard():
	set drawntreasure back to 2 instead of 1


Bug fixes from teammate 1's bug report:
smithyCard():
	gain 4 cards instead of 3 - fixed with code refactor
adventurerCard():
	draw 1 card instead of 2 - fixed with code refactor


Bug fixes from teammate 2's bug report:
smithyCard():
	gain 4 cards instead of 3 - fixed with code refactor
villageCard():
	improper discard - fixed with code refactor
adventurerCard():
	incorrect cards count - fixed with code refactor
