/*
 * Author: Huy Pham
 * Created date: 02/14/2016
 * Program name: randomhistory.c
 * Description: discuss the testing process and the results for testing adventurer card and council room card.
 * 
 *
 * Adventurer card test result:
 * Number of cases passed: 0
 * Number of cases failed: 3030
 *
 * Coverage in 'dominion.c'
 * Lines executed:19.48% of 580
 * Branches executed:19.18% of 417
 * Taken at least once:16.31% of 417
 * Calls executed:8.42% of 95
 * 
 * Functions that were not executed:
 * isGameOver()
 * numHandCards()
 * handCards()
 * supplyCount()
 * fullDeckCount()
 * whoseTurn()
 * endTurn()
 * scoreFor()
 * getWinners()
 * getCost()
 * discardCard()
 * newGame()
 * kingdomCards()
 * playCard()
 * buyCard()
 *
 * Council room card test result:
 * Number of cases passed: 0
 * Number of cases failed: 3030
 * 
 * Coverage in 'dominion.c'
 * Lines executed:22.07% of 580
 * Branches executed:20.14% of 417
 * Taken at least once:17.51% of 417
 * Calls executed:11.58% of 95
 * 
 * Functions that were not executed:
 * newGame()
 * kingdomCards()
 * playCard()
 * buyCard()
 * numHandCards()
 * handCard()
 * supplycount()
 * fullDeckCount()
 * whoseTurn()
 * endTurn()
 * isGameOver()
 * scoreFor()
 * getWinners()
 * getCost()
 * cardEffect()
 * gainCard()
 *
 * Testing process:
 * For adventurer card, I initially set up 40 random test cases and ran the program through. All 40 cases failed
 * and the bug was identified as allowing the player to draw only one treasure card instead of two. This bug was
 * introduced as part of earlier assignment. While the test cases were able to find the bug, I was interested to
 * see if the coverages could be improved (18.28% lines executed, 18.71% branches executed, 15.11% taken at least
 * once and 8.42% calls executed). As a result, I implemented a few different changes and observe the results.
 *
 * The first change was using playCard() function instead of calling adventurerCard() function directly. My thought
 * was that playCard() would be able to cover more branch, hence, it could allow the program to discover more bugs.
 * The result was surprising as the coverages were decreased (17.59% lines executed, 17.75% branches executed, 
 * 13.91% take at least once and 9.47% calls executed). The only coverage increase was calls executed. One way
 * that would explain this is the playCard() function was able to streamline some process while increased number
 * of executions slightly. However, since the code coverage benefit was not apparent, I decided to retract the 
 * changes.
 *
 * The second change that I implemented was adding a random number of players (2 - 4) and set the test cycle to
 * 100. This increased the total number of test cases to 3030, all of which failed because of the bug in the 
 * function.  However, this change improved overall coverages (19.48% lines executed, 19.18% branches executed, 
 * 16.31% taken at least once and 8.42% calls executed). I fell that adding the random number of players had 
 * improved testing coverage.
 *
 * The third change that I implemented was increasing the test cycle to 1000. This increase the total number of
 * test cases to 30300 and all was able to identify the bug in the function. However, the code coverage was 
 * identical to previous testing (19.48% lines executed, 19.18% branches executed, 16.31% taken at least
 * once and 8.42% calls executed). Felling that this did not make the test program better, I removed the change
 * and set the test cycle back to 100.
 *
 * For council room card, I initially set up 100 random test cases. All 100 cases failed and the bug was identified 
 * initially as only allowing the player to draw three additional cards instead of two. However, further 
 * investigation showed that the deck count was decreasing while it should remain the same. The bug was pinpointed
 * as discarding the card instead of putting it back to the deck which was introducted previously. The initial
 * coverages were 20.86% lines executed, 19.66% branches executed, 16.31% taken at least once and 11.58% calls
 * executed. I decided to implemented a few different changes and observe the results.
 *
 * The first change was adding additional criteria for pass/fail check. While this might not improve coverage
 * directly, I fell that it would help identify different errors with future testing. After the changes, the  
 * result for coverage was identical with initial testing which was what I had expected. 
 *
 * For the second change, I added a random number of players (2 - 4) variable. This increased the total code
 * coverage to  and set the test cycle to 21.90% lines executed, 20.14% branches executed, 17.27% taken at least
 * and 11.58% calls executed. I fell that adding the random number of players had improved testing coverage just
 * as it did with adventurer card testing.
 *
 * The third change that I implemented was increasing the test cycle to 100. This increase the total number of
 * test cases to 3030 and all was able to identify the bug in the function. The code coverage was increased slightly 
 * 22.07% lines executed, 20.14% branches executed, 17.51% taken at least once and 11.58% calls executed. I believe
 * the additional test cycles improved the overall testing. 
 *
 * Summary:
 * Overall, I feel that the two test cases were adequate for identifying bugs in the adventurer and council room
 * cards. Additional changes helped improve the test cases by allowing more code coverage without impacting the
 * main functionality of the programs (identify bugs). I also notice that adding random factors (such as number of
 * players and number of test runs) allows the test suit to explore different options and path in the program. This,
 * in turn, provides better chances for the programs to identify bugs that might not have been caught using 
 * traditional testing method. 
 */


