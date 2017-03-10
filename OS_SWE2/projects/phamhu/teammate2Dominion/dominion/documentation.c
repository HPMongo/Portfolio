Adventure card: draw cards from deck, to obtain two treasure cards putting into hand. First while loop is for getting two treasure cards. In this loop, first step is to check if deck is empty. If yes, then shuffle discard and put it into deck. Then use drawCard() to get new card. If it is treasure card, increment drawtreasure count, otherwise, put the card in tempDeck[] and remove the card from hand. Second while is for putting the tempDeck[] card back to discardDeck[].

Smithy card: draws three cards using a loop and discard the Smithy card.

discardCard() method: discard the card that has been played. If the card is not trashed, put it in played pile first. Then, invalidate the played card by setting the handpos to -1. To remove the card from hand, if it is the last card or only one card in hand, simply reduce handcount by 1. Otherwise, using last card to replace played card and invalidate last card and reduce handcount by 1. 

updateCoin() method: reset the coin count to 0. Loop cards in player hands, add coins together based on how many copper, silver or gold cards player has. Return the combined coin count with bonus coin. 
