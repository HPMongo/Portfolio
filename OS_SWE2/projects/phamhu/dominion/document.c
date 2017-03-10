//Program: document.c
//Author: Huy Pham
//Created date: 1/17/16
//Description: This is a documentation of how smithy card, adventurer card, discardCard() method
//and updateCoins() methods work.

smithy card:
	call drawCard() three times using currentPlayer and state as inputs
	discard() current card
	return 0

adventurer card:
	perform draw 2 treasure cards
		call shuffle() if deck is empty
		drawCard()
		if cardDrawn is copper or silver or gold
			increase treasure card count
		else
			put cardDrawn in tempHand
			remove top card from user's deck
			increase tempHand count
	discard all cards in the tempHand
	
discardCard():
	inputs:
		hand position
		current player
		game state
		trash flag
	outputs:
		return 0
	logic:
		if card is not trashed, add it back to played filel
		if the hand position equals to the last card in hand
			reduce handCount by 1
		else if handCount equals 1
			reduce handCount by 1
		else	
			replace discarded card with the last card in the hand
			set last card to -1
			reduce handCount by 1
		return 0
		
updateCoins():
	inputs:
		player
		game state
		bonus
	outputs:
		return 0
	logic:
		reset coin count to 0
		add coins for each treasure card in player's hand
			for copper, add 1 coin to coin count; 
			for silver, add 2 coins to coin count; 
			for gold, add 3 coins to coin count;
		add bonus coins to coin count 

