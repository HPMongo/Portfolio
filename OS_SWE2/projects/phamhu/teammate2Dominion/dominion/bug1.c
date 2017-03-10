unittest1:  test getCost()
Results: no bugs found.
unittest2:  test isGameOver()
Results: no bugs found.
unittest3:   test scoreFor()
Results: the function calls fullDeckCount() which implemented incorrectly. This caused the function did not pass the test.
unittest4:  test gainCard()
Results:  no bugs found
cardtest1:  test AdventurerCard() 
Results:  in the original version of dominion.c , there are two bugs: 1) did not implement shuffle discard and add to deck if deck card is empty. 2) did not discard the handcard after playing.  In my version, I fixed these two bugs and introduced new one.  The test can detect this bug. (increase the draw cards number from 2 to 4.
Cardtest2:  test smithyCard()
Results:  find the bug introduce from me. (draw four cards instead of three).
Cardtest3:  test feastCard()
Results: in the original version of dominion.c, there are few problems: 1) unnecessary to save the hand to temphand and reset after the play. 2) unnecessary updateCoins(). 3) It is wrong to compare the coins in the hand with the cost of gained card. It should compare 9 with cost of gained card to decide if the player can get the card. In this game, playing feast card can gain any card. In my version, I fixed these problems and introduce a bug. The test can detect the bug. (lack of initiate statement for while loop counter)

Cardtest4:  test cutpurseCard()
Results: no bugs found.
