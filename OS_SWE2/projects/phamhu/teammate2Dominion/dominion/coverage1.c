Coverage numbers:
Line executed: 41.80% of 579
Branches executed: 47.06% of 425
Taken at least once: 42.86% of 425
Calls executed: 22.77% of 101.

Detailed report and implication:
1.	In function compare(), first if statement executed 1008 time, but never return value 1. This should be tested.
2.	In initializeGame(),  shuffle() never return -1. This means deckCount never becomes 0. This should be tested.
3.	In drawCard(),  deckCount <= 0 is not tested, which is important branch. 
4.	In getCost(), lack boundary test which should return -1 if wrong card number is passed in.
5.	In adventurerCard(), again deckCount <= 0 is not tested.
6.	In feastCard(), two branches:  supplyCount <= 0 and cost of a card larger than 9, are not tested. 
7.	In cutpurseCard(), the condition was not tested is that there is no copper in hand and reveal one card. 
8.	In discardCard(), the branch that if  handCount = 1 is not tested. 
9.	In gainCard(), if the supplyCount = 0 is not tested.
10.	The following cards are not coverer at all: playCard(), buyCard(), numHandCard(), handCard(), endTurn(), getWiner(), cardEffect(),ambassadorCard.
Summary: 
I think it is not too bad for this test suite considering coverage (-40%) for only testing 8 functions. To improve, some of boundary test should be included, such as deckCount < 0, supplyCount < 0. Also some branches need to be covered in feastCard(), but this probably need the fully implemented game. 
