/***************************************************************************************
 ** Programe: Final Project - CS162 - Fall 2014
 ** Author: Huy Pham
 ** Date: 12/03/2014
 ** Description: This program will create a maze using linked list structure where the 
 ** player will have the opportunity to navigate.
 ** Input: Direction from user
 ** Output: Where the user is at, result of the game
 ***************************************************************************************/
#include<iostream>
#include<string>
#include<vector>
#include"function.h"

using std::cin;
using std::cout;
using std::endl;
using std::vector;

const int unsigned MAXSIZE = 7;

int main() {
 
   RoomNode *head, *tail, *currentRoom, *nextRoom;
   createMap(head,tail);
   currentRoom = head;
   string tempName, tempItem, tempHints;
   int tempKey;
   bool itemFound;
   char selection, removeSwitch;
   Bag tempBag;
   vector<Bag> bag;   

   displayWelcome();

   do
   {   
	itemFound = false;
	tempName = currentRoom->brPtr->getRoomName();
	cout << "\n * * * * * * * * * * * * * * *" << endl;
	cout << "You're current in the " << tempName << " room." << endl;
        tempKey = currentRoom->brPtr->itemInRoom.getItemKey();
	tempItem = currentRoom->brPtr->itemInRoom.getItemName();

	itemFound = itemCheck(bag, tempKey);
	if(!itemFound)
	{
	   cout << "There is a " << tempItem << " in this room." << endl;
	   tempHints = currentRoom->brPtr->itemInRoom.getItemHints();
	   cout << "Clue: " << tempHints << endl;
	   cout << "Would you like to take it?(y/n) ";
	   cin >> selection;
	   if(selection == 'y' || selection == 'Y')
	   {
		if(bag.size() < MAXSIZE)
		{
		   tempBag.itemName = tempItem;
		   tempBag.itemCode = tempKey;
		   bag.push_back(tempBag);
		   cout << "Item has been added!" << endl;
		}
		else
		{
		   cout << "Your bag has a maxium limit of " << MAXSIZE << " items "
			<< "and you've reached the limit." << endl;
		   cout << "If you want this item, then you must drop something else "
			<< "from your bag." << endl;
		   cout << "Do you want to do this?(y/n) ";
		   cin >> removeSwitch;
		   if(removeSwitch == 'Y' || removeSwitch == 'y')
		   {
			removeItem(bag);
		    	tempBag.itemName = tempItem;
			tempBag.itemCode = tempKey;
		        bag.push_back(tempBag);
			cout << "New item has been added to the bag!" << endl;
		   }
		   else
		   {
			cout << "Very well! Moving along!" << endl;
		   }
		}
	   }
	}
 	if(bag.size() > 0)		//display items in the bag
	{
	   cout << "\nCurrent items in the bag: " << endl;
	   for(int unsigned i = 0; i < bag.size(); ++i)
	   {
	   	cout << i + 1 << " - " <<  bag[i].itemName << endl;
 	   }
	}
	nextRoom = mainLogic(currentRoom, bag);
   	currentRoom = nextRoom;
   }while(nextRoom != tail);

   cout << "* * * * * * * * * * * * * *" << endl;
   cout << "   B   R   A   V   O   S" << endl;
   cout << "* * * * * * * * * * * * * *" << endl;
   cout << "Hard work and self-sacrifice pay off, my friend! You have found the treasure"
	<< "!!! Enjoy your new wealth! Don't forget to tell your boss that you won't be "
	<< "in the office on Monday!" << endl;
   cout << "Good-bye!" << endl;
   return 0;
}
