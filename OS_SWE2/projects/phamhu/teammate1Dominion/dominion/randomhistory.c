/*
This week I implemented a random tester for the adventure card and the smithy
card. In my previous week's work, I had already implemented random testing on 
both of those cards, so this week's work did not  result in substantive 
improvements in the test specifications and correctness of the tests, because 
the things being tested did not change, only the number of test cases.  

The one substantive change that did occur was that I decided to use the game
initialization function to initialize the gameState, whereas last week I kept
most items random.  I did this for two reasons.  First, I kept experiencing a
segfault several thousand test cases into my testing of the adventurer card 
which I was unable to diagnose.  I suspect that it's either a bug in the 
adventurer code, or it's caused by my passing a gamestate to the function which
has a randomized (garbage) value in one of the required variables. Secondly, I
thought that using initializeGame() would also allow me to run some more lines
of the dominion.c code in my test. Using initializeGame() allowed me to solve
the problem and get additional coverage over some more lines in dominion.c. 


So the overall coverage of the 8 tests I wrote last week was about 23.83% line
coverage, and the overall coverage of the 10 tests (last week's 8 plus this 
week's 2 tests) was 34.72%.  Every line of playAdventurer() and playSmithy() 
were already covered last week, and they remain covered now. No new bugs were
discovered, but there is one line of code that I discovered that I'm 
suspicious of.  That line would prompt me to write another manual test for
the playAdventurer card.    
