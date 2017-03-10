/*
 * Author: Huy Pham
 * Created date: 02/02/2016
 * Document name: coverage1.c
 * Description: discuss the test coverage for the initial test suit.
 *
 * Here's the result of the initial test suit:
 *
 * File 'dominion.c'
 * Lines executed:34.14% of 580
 * Branches executed:33.57% of 417
 * Taken at least once:25.66% of 417
 * Calls executed:22.11% of 95
 * dominion.c:creating 'dominion.c.gcov'
 *
 * Functions that were not executed:
 * playedCard()
 * numHandCards()
 * handCard()
 * whoseTurn()
 * endTurn()
 * scoreFor()
 * getWinners()
 * feastCard()
 * cardEffect()
 * kingdomCards()
 *
 * Observation:
 * The overall coverage was relatively low for the test suit (1/3 of the statements and branches, 1/4 of the 
 * branch was taken at least once and 1/5 of all the calls were executed). However, the test suit does what it
 * supposes to which is identifing different bugs in the program. 
 *
 * The reason that the test suit was able to find bugs even with low testing coverage is because the separate 
 * test cases (card tests and unit tests) were developed to focus on the  functionalities of either a 
 * function or a card's effect. As a result, since the testing was highly focused on the specific functionality, 
 * other functions that are not related to the test were not executed. This is the reason why many other functions 
 * such as playedCard(), getWinner(), etc were not covered with this testing.
 * 
 * There are rooms for improving the test suit such as including more testing functions, increase random testing 
 * and complete game simulation. While the goal of doing this is not to have testing coverage at 100%, 
 * the additional coverage could potentially expose more bugs in the program that might not be caught with the current
 * testing suit.
 */
