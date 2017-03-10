/*
 * Author: Huy Pham
 * Created date: 02/02/2016
 * Document name: bug1.c
 * Description: discuss the unit test execution and bugs found during that process.
 *
 * The four functions that I tested are updateCoins(), drawCard(), buyCard() and isGameOver().
 *
 * For updateCoins(), 330 cases passed with no failure. 
 * 
 * For drawCard(), 220 cases passed with no failure.
 *
 * For buyCard(), 10 cases passed and 190 cases failed. The main reason for the high volume of
 * failures is because random cards were selected for each case while the number of coins was 
 * not updated for each tes case. As a result, more cases failed because there was not enough
 * money for the requested card.
 * 
 * For isGameOver(), 96 cases passed and 4 cases failed. Similar to buyCard() testing, a random
 * number determines when the supply cards count and province cards count should be decreased.
 * The four failed cases were because province cards were reduced to zero however the testing
 * loop continued. Better testing condition to reset the province card counts or stop the test
 * loop should eliminate the failed test cases. However, I decided to leave the test cases 
 * unchanged to exploited different limit of the program.
 * 
 * The four cards that I tested are smithy, adventurer, village and council room. 20 cases were
 * ran for each of these four cards.
 * 
 * For smithy card, all 20 cases failed. The reason that the cases failed is while the number 
 * of cards in the player's hand is added correctly (3), the number of cards drew from the deck
 * is higher (4). This is the same bug that was introduced earlier with the program.
 *
 * For adventurer card, all 20 cases failed. The cases failed because only one card is added to 
 * the player's hand instead of two. This is also a bug that was added to the program.
 *
 * For village card, all 20 cases failed. The card passed two of the three main tests for village
 * function (reduce deck by 1 hand and 2 to actions). However, the player's cards do not increase
 * after each iteration. This suggests that one card is discarded from the deck instead of being 
 * pushed back. This is a bug that was introduced with the program.
 *
 * For council room card, all 20 cases failed. In this case, the card also passed three of the four
 * tests (add 1 buy to the player, 1 card to other players and 4 cards drew from the deck). However,
 * similarly to village card, the number of cards added to player's hand was only 3. This also
 * suggested that one card is discard from the deck instead of being recycled in the game. This
 * is also a bug that was introduced with the program.
 *
 */
