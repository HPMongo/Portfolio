/*************************************************************************
 * Huy Pham
 * CS 161 - Summer 2014
 ************************************************************************
 * Final project
 * This program will create a one person black jack game. The player can
 * continue to play the game as long as there is money in the account.
 * The dealer will draw if the card is less than 17.
 ************************************************************************
 *Input: user's input on each hand
 *Output: display of individual cards and result of the game
 *************************************************************************
 *References:
 *Ref#1: "Bubble Sort Random Array - C++ Forum." Bubble Sort Random Array-
 *            C++ Forum. N.p., n.d. Web. 28 Aug. 2014.
 *************************************************************************/
//------------------------------------
// Req #26: awesome game
//------------------------------------
#include<iostream>
#include<cstdlib>
#include<ctime>
#include<cstring>
using namespace std;

const int possibleNum = 52;
//------------------------------------
// Req #21: struct
//------------------------------------
struct IndCard
{
	int cardID;
	int cardOrder;
	int cardValue;
	int alternateValue;
	char faceValue[4];
};
//------------------------------------
// Req #22: class
//------------------------------------
class Hands
{
public:
	Hands();//default constructor
	void setCards(int cardsArray[], int value, int currentPos);
	void setValue(IndCard *dPtr);	//set value for the current hands
	int getValue();			//get value of the current hands
	void copyNSort(IndCard *dPtr);	//copy the regular cards array and sort it before storing it
    //to the cards sorted array
	void setStat();		//set status of current hands: 1 - in play; 2 - blackjack; 3 - busted
	int getStat();		//return status of current hands
	void displayCards(IndCard *dPtr);
	void displayCards(IndCard *dPtr, int cardHide);
    void resetHands();
private:
	int cardsArray[13];
	int cardsArrSorted[13];
	int handsValue;
	int lastPos;
	int playStat;
};

class Payout
{
public:
	Payout();//default constructor
	void setPayFactor(int pStat, int dStat, int pVal, int dVal);
	int getPayFactor();
private:
	int payFactor;
};

class Bet
{
public:
    Bet();//default constructor
    void setBetAmt(int totalCash);
    int getBetAmt();
private:
    int betAmt;
};

class Purse
{
public:
	Purse();//default constructor
	void setCashAmt(int amount);
	int getCashAmt();
private:
	int totalCash;
};

int genRand(int arr[], int size);
//This function will generate a random number that is not in the given array
//Precondition: array and size
//Postcondition: unique number

void getCards(int cardsArr[], int numCard, int &currentPos);
//This function will generate a unique number of cards based on the request
//from the calling function.
//Precondition: array, number of cards to generate and current position of
//the array element
//Postcondition: array populated with the number of cards requested and the
//current position of the next empty array element

void resetGame(int cardsArr[], int arrSize, int* sPosPtr, Hands* pHPtr, Hands* dHPtr);
//This function will reset everything to get ready for a new game.

bool initialPlay(int cardsArr[], int* sPosPtr, Hands* pHPtr,Hands* dHPtr,  IndCard* dPtr);
//This function will handle the intial set up for both player's and dealer's first hand.

bool playerTurn(int cardsArr[], int* sPosPtr, Hands* pHPtr, IndCard* dPtr, bool* ddPtr, bool* ddAPtr);
//This function will handle all the logic for the player

bool dealerTurn(int cardsArr[], int* sPosPtr, Hands* dHPtr, IndCard* dPtr);
//This function will handle all the logic for the dealer. The dealer needs to draw
//cards as long as the hand is less than 17.

void displayOption(int optNum);
//This function will diplay the available options to the user

int getOption(int optNum);
//This function will get and validate the option that the user selects

void initialNewDeck(IndCard nDeck[], int size);
//This function will initialize the value for the entire deck of cards

int getBV(int currentValue, int currentHand, int altHand);
//This function will evaluate the best value based on a current value and
//a given card

void trueUpResult(Payout* payPtr, Bet* betPtr, Purse* totalPtr, bool* ddPtr);
//This funciton will calculate the win and lose amount and update that toward the user's
//total cash amount

int encryption (int inValue, char input);
//This is either a super awesome secret encryption to encrypt and decrypt sensitive value
//or just a filler to get some requirement point.

int validateInput(char **argv);
//This function will validate the user's inputs and store those into
//a person struct


int main(int argc, char* argv[])
{
	srand(time(0));
    
	const int arraySize = 26;
    //------------------------------------
    // Req #19: dynamic array
    //------------------------------------
	int *cardsDealt = new int [arraySize];
	int sPos, encryptedValue = 0, startingCash = 0;           // decryptedValue = 0; //not being used currently
	int *sPosPtr;
	sPosPtr = &sPos;
	//play condition for the game, player and dealer
	bool gameCont, playerCont, dealerCont;
	bool doubleDown, ddAllow;        //double down and double down check indicators
	bool *ddPtr, *ddAPtr;
	ddPtr = &doubleDown;
    	ddAPtr = &ddAllow;
    	char playAgain = 'y', encryptNow = 'E'; // decryptNow = 'D'; //not being used currently
    
    //------------------------------------
    // Req #18: multi-dimensional array
    //------------------------------------
   	int custComp[2][1];             //this is use to keep track of customer's hand plays and total amount bet for comp calculation
    
	IndCard newDeck[possibleNum];	//declare newDeck struct
    //------------------------------------
    // Req #24: pointer to struct
    //------------------------------------
    //------------------------------------
    // Req #23: pointer to (struct) array
    //------------------------------------
	IndCard *deckPtr;               //declare pointer to IndCard struct
	deckPtr = newDeck;
	
    	Hands playerHands, dealerHands; //declare player and dealer hands objects
    //------------------------------------
    // Req #25: pointer to object
    //------------------------------------
	Hands *pHPtr, *dHPtr;           //declare pointers to Hands objects
	pHPtr = &playerHands;
	dHPtr = &dealerHands;

 	Payout mFactor;                 //
    	Payout* payPtr;
    	payPtr = &mFactor;
	//------------------------------------
    // Req #22: class object
    //------------------------------------
    	Bet userBet;
    	Bet* betPtr;
    	betPtr = &userBet;
    
    	Purse kaching;                  //
    	Purse* kPtr;
    	kPtr = &kaching;
    
    //initial a new deck of cards and its values for the game
	initialNewDeck(newDeck, possibleNum);
    
    if(argc == 2) // user provides the amount of money they want to start with
    {
	startingCash = validateInput(argv);    
    }
    else
    {
        startingCash = 1000;        //assign default amount
    }
    
    //assign cash for the player
    kaching.setCashAmt(startingCash);
    
    custComp[0][0] = 0;
    custComp[1][0] = 0;

    cout << "\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *" << endl;   
    cout << "* * * *  Welcome to Awesome BlackJack! Where everyone can be a winner   * * * *" << endl;
    cout << "* * The cards will be displayed as face value plus suit initial, C - Club,  * *" << endl;
    cout << "* *         D - Diamond, H - Heart and S - Spade                            * *" << endl;
    cout << "* *          (i.e. Queen of Diamond will be QD, 10 of Spade will be 10S)    * * " << endl;
    cout << "* * * *                         GOOD LUCK !!!                           * * * * " << endl;

    do
    {
    playerCont = true;
    dealerCont = true;
    doubleDown = false;
    ddAllow = true;
    sPos = 0;
    //------------------------------------
    // Req #01: output
    //------------------------------------
    cout << "\nHere is how much you curently have in the account: $ " << kaching.getCashAmt() << endl;
    
    //take user bet
    //------------------------------------
    // Req #14: pass by value
    //------------------------------------
    userBet.setBetAmt(kaching.getCashAmt());
    
    cout << "\nYou have placed a bet of $ " <<userBet.getBetAmt() << endl;
    
    if((userBet.getBetAmt() * 2) > kaching.getCashAmt())    //if the 2x user bet is more than account value,
    {                                                       //don't allow double double option
        ddAllow = false;
    }
    
    resetGame(cardsDealt, arraySize, sPosPtr, pHPtr, dHPtr);
        
    gameCont = initialPlay(cardsDealt, sPosPtr, pHPtr, dHPtr, deckPtr);
    
	//This is use to evaluate final hands and determine pay factor
	if(gameCont)	//no one has 21
    	{
		//player goes first until the turn is over
    	//------------------------------------
		// Req #07: loop
		//------------------------------------
		do
		{
			playerCont = playerTurn(cardsDealt, sPosPtr, pHPtr, deckPtr, ddPtr, ddAPtr);
		} while (playerCont);
        
		//evaluate player's hand
		if(playerHands.getStat() < 3) // not over 21
		{
			//go over dealer logic
			do
			{
				dealerCont = dealerTurn(cardsDealt, sPosPtr, dHPtr, deckPtr);
			} while (dealerCont);
		}
	}

    mFactor.setPayFactor(playerHands.getStat(),dealerHands.getStat(),playerHands.getValue(),dealerHands.getValue());

    //Display the hands
    cout << "\nHere are your final hands: " << endl;
    playerHands.displayCards(deckPtr);
    cout << "\nHere are the dealer final hands: " << endl;
    dealerHands.displayCards(deckPtr);

    //Calculate the result
    trueUpResult(payPtr, betPtr, kPtr, ddPtr);
    
    cout << "\nCurrent balance: $" << kaching.getCashAmt() << endl;
    
    custComp [0][0]++;
    custComp [1][0] += userBet.getBetAmt();
/*Use for debug cards drew
	for (int j = 0; j < sPos; j++)
	{
		cout << endl << "cardsDealt[" << j << "]: " << cardsDealt[j] << endl;
		cout << "cardFace: "  << newDeck[cardsDealt[j]].faceValue << endl;
		cout << "cardValue: " << newDeck[cardsDealt[j]].cardValue << endl;
		cout << "cardAltValue: " << newDeck[cardsDealt[j]].alternateValue << endl;
	}
*/
 
    if(kaching.getCashAmt() == 0)
    {
        playAgain = 'n'; // player doesn't have any money left
    }
    else
    {
        cout << "\nDo you want to continue to play? (y/n) ";
        cin >> playAgain;
    }
        
    }while (playAgain == 'y' || playAgain == 'Y');

    cout << "\nThank you for playing Awesome BlackJack!" << endl << endl;
    cout << "You have played " << custComp[0][0] << " times with an average starting bet of $ " << custComp[1][0]/custComp[0][0]
         << " per hand." << endl;
    
//encrypt the money left in the player's account for outside transfer
    encryptedValue = encryption(kaching.getCashAmt(),encryptNow);
    
//decryption is not needed in this program but it could come in handy in case the player needed to wire some money to the account
/*
    decryptedValue = encryption(encryptedValue, decryptNow);
*/
  
    //delete dynamic array
    delete []cardsDealt;
    
	return 0;
}


int encryption (int inValue, char input)
{
    int holding = 0, storedValue = 0, decryptedValue = 0;

    const int encryptionKey = 161;
    
    if(input == 'E')
    {
        //------------------------------------
        // Req #06: bitwise operator
        //------------------------------------
        holding = inValue ^ encryptionKey;
        storedValue = ~holding;
        
        return storedValue;
    }
    else
    {
        holding = ~inValue;
        decryptedValue = holding ^ encryptionKey;
        
        return decryptedValue;
    }
}

int validateInput(char **argv)
{
    	int inputAmt;

    	inputAmt = atoi(argv[1]);
	
    	if(inputAmt < 0 || (!isdigit(argv[1][0])))
    	{
		cout << "It looks like you have entered an invalid starting amount. Your account will be assigned a default amount of $1000." << endl;
        	inputAmt = 1000;
    	}
    
    return inputAmt;
}
bool initialPlay(int cardsArr[],int* sPosPtr, Hands* pHPtr, Hands* dHPtr,  IndCard* dPtr)
{
	bool contPlay = true;//set condition for the initial play
    
	getCards(cardsArr, 2, *sPosPtr);
	pHPtr->setCards(cardsArr, 2, *sPosPtr);
	pHPtr->copyNSort(dPtr);
	getCards(cardsArr, 2, *sPosPtr);
	dHPtr->setCards(cardsArr, 2, *sPosPtr);
	dHPtr->copyNSort(dPtr);
    
    //display face value of the hand
    cout << "\nHere are your cards: " << endl;
	pHPtr->displayCards(dPtr);
    cout << "\nHere are the dealer's cards: " << endl;
	dHPtr->displayCards(dPtr, 1);	//hide dealer cards for the intial play
    
    //set value of the hand
	pHPtr->setValue(dPtr);
	dHPtr->setValue(dPtr);
    //set status of current hand
	pHPtr->setStat();
	dHPtr->setStat();
    
/*
    //get current status of the hand
	cout << "current status of player's: " << pHPtr->getStat() << endl;
	cout << "current status of dealers': " << dHPtr->getStat() << endl;
*/
	if(pHPtr->getStat() == 2 || dHPtr->getStat() == 2) //either hand is a blackjack
	{
		contPlay = false;                       //stop the game
	}
	
	return contPlay;
}

void resetGame(int cardsArr[], int arrSize, int* sPosPtr, Hands* pHPtr, Hands* dHPtr)
{
    for (int i = 0; i < arrSize; i++)
    {
        cardsArr[i] = -1;
    }
    
    sPosPtr = 0;
    
    pHPtr->resetHands();
    dHPtr->resetHands();
}

bool playerTurn(int cardsArr[], int* sPosPtr, Hands* pHPtr, IndCard* dPtr, bool* ddPtr, bool* ddAPtr)
{
	int optionSelect = 0;
	const int firstMove = 3, subMove = 2; 	//options available for initial and subsequence plays
	bool contPlay = true;                   //set condition for user's play
    cout << "\n\n***Your turn***" << endl;

	if(*sPosPtr <= 4 && (*ddAPtr))	//first play & double down allow
	{
		displayOption(firstMove);
		optionSelect = getOption(firstMove);
	}
	else                            //subsequence play
	{
		displayOption(subMove);
		optionSelect = getOption(subMove);
	}
	   
	if(optionSelect == 1)		///Stay
	{
		cout << "\nVery well! The value of your hand is: " << pHPtr->getValue() << endl;
///		cout << "The status is: " << pHPtr->getStat() << endl;
		contPlay = false;				//stop the turn
	}
	
	if(optionSelect == 2)		///Draw
	{
		getCards(cardsArr, 1, *sPosPtr);		//draw a card
		pHPtr->setCards(cardsArr, 1, *sPosPtr);	//put the card in user's hands
		pHPtr->copyNSort(dPtr);                 //copy and sort cards
        cout << "\nHere are your cards: " << endl;
		pHPtr->displayCards(dPtr);              //display current hands
		pHPtr->setValue(dPtr);                  //set the current value of the hands
		pHPtr->setStat();                       //set the current status of the hands
		cout << "\nThe value of the hand is: " << pHPtr->getValue() << endl;
///		cout << "The status is: " << pHPtr->getStat() << endl;
		if(pHPtr->getStat() == 2 || pHPtr->getStat() == 3)//player hits 21 or busted
		{
			contPlay = false;                   //stop the turn
		}
	}
    
	if(optionSelect == 3)		///Double down
	{
		getCards(cardsArr, 1, *sPosPtr);		//draw a card
		pHPtr->setCards(cardsArr, 1, *sPosPtr);		//put the card in user's hands
		pHPtr->copyNSort(dPtr);				//copy and sort cards
        cout << "\nHere are your cards: " << endl;
		pHPtr->displayCards(dPtr);			//display current hands
		pHPtr->setValue(dPtr);				//set the current value of the hands
		pHPtr->setStat();				//set the current status of the hands
		cout << "\nAfter draw one card and double down, the value of your hand is: " << pHPtr->getValue() << endl;
///		cout << "The status is: " << pHPtr->getStat() << endl;
		*ddPtr = true;					//set double down switch to true
		contPlay = false;				//stop the turn since this is a double down
	}
    
	return contPlay;
}

bool dealerTurn(int cardsArr[], int* sPosPtr, Hands* dHPtr, IndCard* dPtr)
{
    cout << "\n**Dealer's turn**" << endl;
	bool contPlay = true;			//set condition for dealer's play
	
	if(dHPtr->getValue() < 17)		///Dealer's hand is less than 17, need to draw
	{
		getCards(cardsArr, 1, *sPosPtr);            //draw a card
		dHPtr->setCards(cardsArr, 1, *sPosPtr);		//put the card in user's hands
		dHPtr->copyNSort(dPtr);                     //copy and sort cards
		dHPtr->displayCards(dPtr);                  //display current hands
		dHPtr->setValue(dPtr);                      //set the current value of the hands
		dHPtr->setStat();                           //set the current status of the hands
		cout << "\nThe value of the hand is: " << dHPtr->getValue() << endl;
//		cout << "The status is: " << dHPtr->getStat() << endl;
		if(dHPtr->getStat() == 2 || dHPtr->getStat() == 3)//player hits 21 or busted
		{
			contPlay = false;			//stop the turn
		}
	}
	else					//dealer is 17 or more
	{
		contPlay = false;
	}
	
	return contPlay;
}

void displayOption(int optNum)
{
	cout << "\nHere are the available options:\n" << endl;
	switch (optNum)
	{
		case 1:
			cout << " 1 - Stay" << endl;
			break;
		case 2:
			cout << " 1 - Stay" << endl;
			cout << " 2 - Draw" << endl;
			break;
		case 3:
			cout << " 1 - Stay" << endl;
			cout << " 2 - Draw" << endl;
			cout << " 3 - Double down" << endl;
			break;
		default:
			cout << " Invalid input for option!!!" << endl;
	}
}

int getOption(int optNum)
{
	int input = 0;
	cout << "\nWhat would you like to do? ";
	if (optNum == 3)
	{
		cin >> input;
        //------------------------------------
		// Req #09: logic error check - verify that the user enters a valid input
		//------------------------------------
		while(!cin || (input < 1 || input > 3))
		{
			cin.clear();
			cin.ignore(1000,'\n');
			cout << "You have entered an invalid option. Please select your move (1 - 3): ";
			cin >> input;
		}
	}
	else if (optNum == 2)
	{
		cin >> input;
		while(!cin || (input < 1 || input > 2))
		{
			cin.clear();
			cin.ignore(1000,'\n');
			cout << "You have entered an invalid option. Please select your move (1 - 2): ";
			cin >> input;
		}
	}
	else
	{
		cout << "Hey, double check your logic!! " << endl;
	}
    
	return input;
}
//------------------------------------
// Req #11: function
//------------------------------------
void trueUpResult(Payout* payPtr, Bet* betPtr, Purse* totalPtr,bool* ddPtr)
{
    int multiplier = 1;
    
    if((*ddPtr))  //the player won and double down is true
    {
        multiplier  = 2;
    }
    
    cout << endl << endl;   //create white lines
    
    switch (payPtr->getPayFactor())
    {
        case -1:
            cout << "It looks like you have lost $ " << betPtr->getBetAmt() * multiplier << " on this hand!" << endl;
            break;
        case 0:
            cout << "It looks like we have a tie." << endl;
            break;
        case 1:
            cout << "Congrats! You won $ " << betPtr->getBetAmt() * multiplier << "!" << endl;
            break;
        default:
            cout << "C'on, man! You need to double check the results for Pay Factor!!" << endl;
            break;
    }
    //calculate the new cash value as bet amount * multipler * pay factor
    totalPtr->setCashAmt(betPtr->getBetAmt() * multiplier * (payPtr->getPayFactor()));
}

Purse::Purse() : totalCash(0)//default constructor
{/*Intentionally blank*/}


Bet::Bet() : betAmt(0)//default constructor
{/*Intentionally blank*/}

Hands::Hands()//default constructor
{
	for (int i = 0; i < 13; i++)
	{
		cardsArray[i] = -1;
		cardsArrSorted[i] = -1;
	}
	lastPos = 0;
	handsValue = 0;
	playStat = 0;
}

Payout::Payout() : payFactor (0)//default constructor
{/*Intentionally blank*/}

void Payout::setPayFactor(int pStat, int dStat, int pVal, int dVal)
{
	//check for busted condition first
	if(pStat == 3)			//player lost
	{
		payFactor = -1;		//lose bet amount
		return;
	}
	if(dStat == 3)			//dealer lost
	{
		payFactor = 1;		//win bet amount
		return;
	}
	//check for blackjack or 21
	if(pStat == 2 && dStat != 2)	//player got 21
	{
		payFactor = 1; 		//win bet amount
		return;
	}
	if(dStat == 2 && pStat != 2) 	//dealer got 21
	{
		payFactor = -1;		//lose bet amount
		return;
	}
	if(pStat == 2 && dStat == 2)	//both tie at 21
	{
		payFactor = 0;		//money stays the same
		return;
	}
	//check for under 21
	if(pStat == 1 && dStat == 1)
	{
		if(pVal > dVal)		//player won
		{
			payFactor = 1;	//win bet amount
			return;
		}
		else if(pVal < dVal)	//dealer won
		{
			payFactor = -1;	//lose bet amount
			return;
		}
		else			//tie
		{
			payFactor = 0;	//money stays the same
			return;
		}
	}
}
//------------------------------------
// Req #12: function decomposition
//------------------------------------
int Payout::getPayFactor()
{
	return payFactor;
}

void Bet::setBetAmt(int totalCash)
{
    int betInput = 0;
    cout << endl << "How much do you want to bet? ";
    //------------------------------------
    // Req #02: input
    //------------------------------------
    cin >> betInput;
    while(!cin || betInput < 0 || betInput > totalCash)
    {
        cin.clear();
        cin.ignore(1000,'\n');
        cout << "You have enter an invalid amount. Please enter a bet between 1 and " << totalCash << ": ";
        cin >> betInput;
    }
    betAmt = betInput;
}

int Bet::getBetAmt()
{
    return betAmt;
}

void Purse::setCashAmt(int amount)
{
    totalCash += amount;
}

int Purse::getCashAmt()
{
    return totalCash;
}

void Hands::setCards(int cardArray[], int value, int currentPos)
{
	int getPos = currentPos - value;
	for(int i = 0; i < value; i++)
	{
		cardsArray[lastPos] = cardArray[getPos];
		getPos++;
		lastPos++;
	}
}

void Hands::copyNSort(IndCard* dPtr)
{
	int temp = 0;
    
	for(int s = 0; s < lastPos; s++)
	{
		cardsArrSorted[s] = cardsArray[s];
	}
    //------------------------------------
    // Req #13: scope. 's' was to be used in place of 'lastPos' for the following statements.
    // However, since 's' is declared only in the previous statement, its scope is only
    // in that statement; therefore, 's' can't be referred to in any other place.
    //------------------------------------
	//The following bubble sort logic was copied from <http://www.cplusplus.com/forum/general/97463/> (Ref#1)
	for(int t = 0; t < lastPos; t++)
	{
		for(int v = 0; v < lastPos - 1; v++)
		{
			if(dPtr[cardsArrSorted[v]].cardOrder < dPtr[cardsArrSorted[v + 1]].cardOrder)
			{
				temp = cardsArrSorted[v];
				cardsArrSorted[v] = cardsArrSorted[v + 1];
				cardsArrSorted[v + 1] = temp;
			}
		}
	}
//	cout << "\nIn copyNSort - at the end of the function***" << endl;
/*
 //------------------------------------
 // Req #10: debug - this is used to validate the input sorted values
 //------------------------------------
	for(int i = 0; i < lastPos; i++)
	{
		cout << "<" << i << ">: " << cardsArrSorted[i] << endl;
	}
*/
}

void Hands::setValue(IndCard* dPtr)
{
	handsValue = 0;	//reset value
	for (int d = 0; d < lastPos; d++)
	{
		handsValue = getBV(handsValue, dPtr[cardsArrSorted[d]].cardValue, dPtr[cardsArrSorted[d]].alternateValue);
///		cout << "\n***current value of hands is: " << handsValue << endl;
	}
}

int Hands::getValue()
{
    return handsValue;
}

void Hands::displayCards(IndCard* dPtr)
{
//	cout << "\nHere are the current cards: " << endl;
	for (int a = 0; a < lastPos; a++)
	{
		cout << dPtr[cardsArray[a]].faceValue << " ";
	}
    cout << endl; //blank line
}
//------------------------------------
// Req #15: function overloading
//------------------------------------
void Hands::displayCards(IndCard* dPtr, int cardHide)
{
	int cardCount = 0;

	for (int a = 0; a < lastPos; a++)
	{
		if(cardCount >= cardHide)
		{
			cout << dPtr[cardsArray[a]].faceValue << " ";
		}
		else
		{
			cout << "*** ";
		}
		cardCount++;
	}
}

void Hands::setStat()
{
	if(handsValue < 21) 	 //still in play
	{
		playStat = 1;
	}
	else if(handsValue == 21)//top dawgg
	{
		playStat = 2;
	}
	else                     //busted
	{
		playStat = 3;
	}
}

int Hands::getStat()
{
	return playStat;
}

void Hands::resetHands()
{
    for (int i = 0; i < 13; i++)
	{
		cardsArray[i] = -1;
		cardsArrSorted[i] = -1;
	}
	lastPos = 0;
	handsValue = 0;
	playStat = 0;
}

//------------------------------------
// Req #17: recursive
//------------------------------------
int getBV(int currentValue, int currentHand, int altHand)
{
	if(altHand < 0)	//no alternate value
	{
		return currentValue + currentHand;
	}
	else            //has alternate value
	{	//if alt hand is over 21, pick current hand
		if(getBV(currentValue, altHand, -1) > 21)
		{
			return currentValue + currentHand;
		}
		else //alt hand is not over 21, compare the two
		{
			if(getBV(currentValue, altHand, -1) > getBV(currentValue, currentHand, -1))
			{
				return currentValue + altHand;
			}
			else
			{
				return currentValue + currentHand;
			}
		}
	}
}
//------------------------------------
// Req #11: function
//------------------------------------
void initialNewDeck(IndCard nDeck[], int size)
{
	//------------------------------------
    // Req #16: string and c-string
    //------------------------------------
	string suit = "CDHS";//stands for Club, Diamond, Heart and Spade
	char oneChar;
	int order = 1, cOrder = 1, suitCount = 1, currentSuit = 0;
	const int altVal = 11;
	//------------------------------------
	// Req #07: loop
	//------------------------------------
	for(int i = 0; i < size; i++)
	{
		char preFix[4] = "   ";
        
		//assign card id
		nDeck[i].cardID = i;
        
		//assign order for each card
		//------------------------------------
		// Req #04: conditional
		//------------------------------------
		if(cOrder <= 13)
		{
			nDeck[i].cardOrder = cOrder;
			cOrder++;
		}
		else	//new suit, restart the value
		{
			cOrder = 1;
			nDeck[i].cardOrder = cOrder;
			cOrder++;
		}
        
		//assign play value for each card
		if(order < 10)
		{
			nDeck[i].cardValue = order;
			order++;
		}
		else if(order >= 10 && order <= 13)
		{
			nDeck[i].cardValue = 10; //card has value of 10 for 10 to King
			order++;
		}
		else	//new suit, restart the value
		{
			order = 1;
			nDeck[i].cardValue = order;
			order++;
		}
        
		//assign alternate play value for Aces
		//------------------------------------
		// Req #05: logical operator
		//------------------------------------
		if(nDeck[i].cardID == 0 ||
		   nDeck[i].cardID == 13 ||
		   nDeck[i].cardID == 26 ||
           nDeck[i].cardID == 39)
		{
			nDeck[i].alternateValue = altVal;
		}
		else
		{
			nDeck[i].alternateValue = -1;
		}
        
		//assign display value
		//the first two characters are the face value
		switch(nDeck[i].cardOrder)
		{
			case 1:
				strncpy(preFix," A",2);
				break;
			case 10:
				strncpy(preFix,"10",2);
				break;
			case 11:
				strncpy(preFix," J",2);
				break;
			case 12:
				strncpy(preFix," Q",2);
				break;
			case 13:
				strncpy(preFix," K",2);
				break;
			default:
                //------------------------------------
                // Req #03: type casting
                //------------------------------------
				oneChar = (char)((int)'0' + nDeck[i].cardOrder);
				preFix[0] = ' ';
				preFix[1] = oneChar;
		}
 		//the third character represents the suit
		preFix[2] = suit[currentSuit];
		if (suitCount < 13)
		{
            //			cout << "suit is: " << suit[currentSuit] << endl;
			suitCount++;
		}
		else
		{
			++currentSuit;
            //			cout<< "Current suit is: " << suit[currentSuit] << endl;
			suitCount = 1;
		}
        //		cout << "current face: " << preFix << endl;
		strcpy(nDeck[i].faceValue, preFix);
        
	}
}

void getCards(int cardsArr[], int numCard, int &currentPos)
{
///	cout << endl << "current pos b4: " << currentPos << endl;
	for(int a = 0; a < numCard; a++)
	{
		cardsArr[currentPos] = genRand(cardsArr, currentPos);
		currentPos++;
	}
///	cout << "current pos after: " << currentPos << endl;
}

int genRand(int arr[], int size)
{
	srand(time(0));
	int randNum = -1, numMatch;
    
	do
	{
		//------------------------------------
		// Req #08: random number
		//------------------------------------ 
		randNum = rand() % possibleNum;
		numMatch = 0;
		for(int i = 0; i < size; i++)
		{
			if(randNum == arr[i])
				++numMatch;
		}
	}while(numMatch > 0);
    
	return randNum;
}
