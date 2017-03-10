/*
BUG 1- discovered by unittest4.c
Found 1 bug in isGameOver:  If one of the empty piles is sea_hag or 
treasure_map, and there are 2 other empty piles elsewhere in the supply, 
this doesn't generate a 'Game Over'.  Problem is in the following (LINE 403): 

  for (i = 0; i < 25; i++)
        should be
  for (i = 0; i < 27 ; i++)

BUG 2 - discovered by cardtest1.c
Found bug in playSmithy(): smithy card not in the discard pile after completion
of the function. 

BUG 3 - discovered by cardtest1.c
Found bug in playSmithy(): discard count not increased by 1 after playSmithy().
This is probably related to Bug #2 above. 

BUG 4- discovered by cardtest2. 
Found bug in playAdventuerer: Temp hand not discarded properly (i.e., an 
improper number of cards were discarded, either too many or too few). 

BUG 5- discovered by cardtest2. 
Found bug in playAdventurer: player hand count not increased by 2 (i.e. an
improper number of treasure cards were added to the player's hand). This could
also cause BUG #4 and BUG #6. 

BUG 6- discovered by cardtest2:
Found bug in playAdventurer: player deck count improperly decreased (i.e., an 
improper number of cards, either too few or too many, were removed from the 
player's deck).  This could also be caused by the same bug that causes BUG #5.

BUG 7- discovered by cardtest3:
Found bug during great hall card test: Suspect location is in discard().
When the random position of the great hall card is the last card in the hand,
the card gained into the hand from the great hall card is immediately discarded.
The great hall card is overwritten, and the cards in the hand are faulty: (a -1
instead of the next card in the deck). 

BUG 8 - discovered by cardtest4: 
Found bug during playVillage: Discarded village card not placed in played cards 
after play.  This is probably a separate bug from BUG #7, because it occurs every
test.  

BUG 9 - discovered by cardtest4: 
Found bug in playVillage: Suspect the problem is the same problem as in BUG 7, likely
location is discard().  When the random position of the village card is the last card
in the hand, the next card placed into the deck is overwritten (a -1 instead of the
next card).  
