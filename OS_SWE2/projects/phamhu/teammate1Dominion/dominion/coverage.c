/*
Total line coverage of all the tests for this Assignment was 23.83% of
dominion.c, which is a low number if the goal was to test the whole program, 
but is a reasonable number given that the assignment was only to test 8 units
or functions, rather than every function. 

More enlightenling was the tested line counts of the lines in the target
functions, since this better represents the thoroughness with which the tests
covered the TARGET functions: 

Shuffle: every line was tested, min # was 14 times, max 4411312. 
100% line coverage (for the target function). 

UpdateCoins: Every line was covered, min was 100 times, max 50438 times.
100% line coverage (for the target function). 


getCost: Every line except 2 were covered, approximately 3800 times per line,
other than the cost of a curse card, and the error input line.  This suggests
writing specific test cases to test the curse card and to try to test a card
with an undefined or improper number (i.e. card '600' for example). 
Approx. 93% line coverage of this function. 


isGameOver: Every line was covered, min 5/max 4940 times. 
100% line coverage (for the target function). 

playSmithy: Every line was covered, min 100, max 800 times. 
100% line coverage (for the target function). 

playAdventurer: Every line was covered, min 14, max 7936 times. 
100% line coverage (for the target function). 

Great Hall Card: Every line of this case in cardEffect was covered 100 times. 
100% line coverage of this function. 

playVillage: Every line of this function was covered 100 times.
100% line coverage. 


Conclusions:  The gcov data suggests that the target units were tested rather
thoroughly except for getCost, which has two edge cases that should probably
be tested for specifically.  Other than that, I would focus further testing on 
other units next, since the overall line coverage is still only ~23%. 
