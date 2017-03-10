//Program: refactor.c
//Author: Huy Pham
//Created date: 1/17/16
//Description: This is a documentation of refactoring for smithy, village, council_room, feast and 
//adventurer cards.

Errors were introduced in the following methods for testing purposes:

in smithyCard():
	increase 4 cards instead of 3
in villageCard():
	set trashcard switch to true (1) instead of (0) discardCard(handPos, currentPlayer, state, 1);
in councilRoomCard():
	set trashcard switch to true (1) instead of (0) discardCard(handPos, currentPlayer, state, 1);
in adventurerCard():
	set drawntreasure to 1 instead of 2 - while(drawntreasure<1)
