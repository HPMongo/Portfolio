/*************************************************************************
discardCard:
Prototype: 
int discardCard(int handPos, int currentPlayer, struct gameState *state, 
                int trashflag)

discardCard() is a function which is used to remove a single card from a
player's hand.  The card in question corresponds to a particular position
of the particular player's hand held in the struct gameState.  The function
checks that this card has not already been trashed, and if not, the card is
removed from the player's hand and added to the player's "played" pile. 
**************************************************************************/

/*************************************************************************
updateCoins:
Prototype: 
int updateCoins(int player, struct gameState *state, int bonus)

updateCoins determines the number of coins a player may play on his turn
by counting all of the treasure cards in a players hand on a particular 
turn, and then applying whatever coin bonus has been passed to the function. 
**************************************************************************/

/*************************************************************************
playAdventurer:
Prototype: 
int playAdventurer(struct gameState *state, int CurrentPlayer)

The playAdventurer() function implements the action of playing the adventurer
card.  This function receives the current state of the game and the current
player, and it draws cards from the players deck into his hand until 2 
additional treasure cards have been drawn.  All non-treasure cards that are
drawn during the play of the adventurer card are discarded at the end of 
the function. Treasure cards are retained for the buy phase. 
**************************************************************************/

/*************************************************************************
playSmithy
Prototype: 
int playSmithy(struct gameState *state, int CurrentPlayer, int handPos)

The playSmithy() function implements the action of playing the smithy
card.  This function receives the current state of the game and the current
player, and it draws 3 additional cards from the players deck into his hand.
**************************************************************************/



