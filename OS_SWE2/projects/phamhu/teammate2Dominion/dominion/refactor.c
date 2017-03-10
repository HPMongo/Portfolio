Changes to the code:
Move out the code for the following cards from cardEffect into separate functions: 
1.	smithyCard
2.	adventurerCard
3.	feastCard
4.	ambassadorCard
5.	cutpurseCard
These functions are called inside of cardEffect function. The codes for implementing these card are  the same, the parameters passed from cardEffect are passed into called functions. 
Introduced bugs:
1.	smithyCard: 
Change statement: for (i = 0; i < 3; i++) to: for (i = 0; i < =3; i++) drawntreasure=3.
2.	adventurerCard: Change statement: while(drawntreasure<2) to: while(drawntreasure<4)
3.	feastCard: Missing statement x = 1 before while loop
4.	ambassadorCard: Change statement: if (choice2 > 2 || choice2 < 0) to: if (choice2 > 2 && choice2 < 0)
