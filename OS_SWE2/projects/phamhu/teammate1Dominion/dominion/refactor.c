/*
The following is a list of the cards that I refactored, and the subtle bugs I
introduced into the code: 

adventurer: I refactored this code into a function called playAdventurer()
    bugs: In the first while loop, I exchanged 'while (drawntreasure < 2)' for
    'while(drawntreasure <= 2)'  This should cause the adventurer card to draw
    cards until 3 treasures are added, rather than just 2. 

smithy: I refactored this code into a function called playSmithy(). 
    bugs:  on the discardCard function call in this function, I replaced the
    0 with a 1.  This is the 'trashflag' so it should discard the smithy card
    to the trash pile rather than back to the player's hand. 

council room: I refactored this code into a function called playCouncilRoom().
    bugs: under the comment about each other player drawing a card, I removed
    an 'if' line. Now, it now longer tests if( i != currentPlayer). Instead, it
    will give an additional card to all the players, including the current 
    player

village:  I refactored this code into a function called playVillage(). 
    bugs: In the discardCard function, I used the same bug as I put in 
    playSmithy, above (I set the trashflag). This will cause the village card
    to be trashed instead of discarded to the player's pile. 

feast: I refactored this code into a function called playFeast().
    bugs:  NONE (that I know of!). 
