#include <iostream>
#include <string>
#include "function.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

//This function will set the item name
void Items::setItemName(string inName)
{
   itemName = inName;
}
//This function will set the item key
void Items::setItemKey(int inKey)
{
   itemKey = inKey;
}

//This function will set the item hints
void Items::setItemHints(string inHints)
{
   itemHints = inHints;
}

//This function will get the item name
string Items::getItemName()
{
   return itemName;
}

//This function will get the item hints
string Items::getItemHints()
{
   return itemHints;
}
//This function will get the item key
int Items::getItemKey()
{
   return itemKey;
}

//Display info for Base Room
void BaseRoom::display()
{
   cout << "RoomName: " << roomName << endl;
   cout << "RoomType: " << roomType << endl;
   cout << "Restriction: " << restriction << endl;
   cout << "R-code: " << restrictionCode << endl;
   cout << "Special item is: " << itemInRoom.getItemName() << endl;
   cout << "Item code: " << itemInRoom.getItemKey() << endl;
   cout << "Key require: " << keyRequire << endl;
   cout << "RoomKey is: " << roomKey << endl;
}

string BaseRoom::getRoomName()
{
   return roomName;
}

int BaseRoom::getRestrictionCode()
{
   return restrictionCode;
}

void BaseRoom::setKeyRequire(bool inRequire) 
{
   keyRequire = inRequire;
}

bool BaseRoom::getKeyRequire()
{
   return keyRequire;
}

void BaseRoom::setRoomKey(int inKey)
{
   roomKey = inKey;
}

int BaseRoom::getRoomKey()
{
   return roomKey;
}

//Constructor for Regular Room
RegRoom::RegRoom(string inName)
{ 
   roomName = inName;
   roomType = "Regular";
   restriction = false;
   restrictionCode = 0;
}
//This function will assign an item to Regular Room
void RegRoom::setItem(string inName, string inHints, int inCode)
{
   itemInRoom.setItemName(inName);
   itemInRoom.setItemHints(inHints);
   itemInRoom.setItemKey(inCode);   
}

//Constructor for Cold Room
ColdRoom::ColdRoom(string inName)
{ 
   roomName = inName;
   roomType = "Cold";
   restriction = true;
   restrictionCode = 1;
}
//This function will assign an item to Cold Room
void ColdRoom::setItem(string inName, string inHints, int inCode)
{
   itemInRoom.setItemName(inName);
   itemInRoom.setItemHints(inHints);
   itemInRoom.setItemKey(inCode);   
}

//Constructor for Dark Room
DarkRoom::DarkRoom(string inName)
{ 
   roomName = inName;
   roomType = "Dark";
   restriction = true;
   restrictionCode = 2;
}
//This function will assign an item to Dark Room
void DarkRoom::setItem(string inName, string inHints, int inCode)
{
   itemInRoom.setItemName(inName);
   itemInRoom.setItemHints(inHints);
   itemInRoom.setItemKey(inCode);   
}

void displayRoom(RoomNode * n)
{
   cout << "NorthNext: " << n->NorthNext << endl;
   cout << "SouthNext: " << n->SouthNext << endl;
   cout << "EastNext: " << n->EastNext << endl;
   cout << "WestNext: " << n->WestNext << endl << endl;
}


RoomNode::RoomNode(BaseRoom* n)
{
   brPtr = n;
   NorthNext = NULL;
   SouthNext = NULL;
   EastNext = NULL;
   WestNext = NULL;
} 

void addDoor(RoomNode *n, RoomNode*p, int side)
{
   switch(side)
   {
  	case 1: 	//Add door on North entrance
   	   n->NorthNext = p;
	   p->SouthNext = n; 
  	   break;
    	case 2: 	//Add door on South entrance
   	   n->SouthNext = p;
 	   p->NorthNext = n;
   	   break;
    	case 3: 	//Add door on East entrance
   	   n->EastNext = p;
 	   p->WestNext = n;
   	   break;
    	case 4: 	//Add door on West entrance
   	   n->WestNext = p;
	   p->EastNext = n;
   	   break;
     	default:
	   cout << "Bambb! You have entered the Matrix!!\n"; 
   }
}

RoomNode * mainLogic(RoomNode *n, vector<Bag> inVec)
{
   int selection = 0, attemp = 0;
   bool validPath = false, restricted = false;
   RoomNode *nextNode, *tempXNode;
   string tempName;
   tempName = n->brPtr->getRoomName();
//   cout << "\n>>You're in the " << tempName << ".\n";
   
   while(!validPath)
   {
	restricted = false;
   	cout << "\nHere's the possible direction:\n"
	     << "1 - North\n"
	     << "2 - South\n"
	     << "3 - East\n"
	     << "4 - West\n"
	     << "Where do you want to go? ";
  	cin >> selection;
  	while(!cin || selection < 1 || selection > 4)
  	{
	   cin.clear();
	   cin.ignore(1000,'\n');
	   cout << "That's not a valid option. Please reselect your option (1-4): ";
   	   cin >> selection;
  	}

  	switch (selection)
  	{
	   case 1:
	       	if (n->NorthNext == NULL)
	       	{
	          cout << " That's a wall!\n";
	       	}
 	       	else
	       	{ 
	 	  tempXNode = n->NorthNext;
		  restricted = restrictionCheck(tempXNode, inVec);
	          if(!restricted)
		  {
			nextNode =  n->NorthNext;
	          	validPath = true;
		  }
	       	}
		break;
 	   case 2:
	       	if (n->SouthNext == NULL)
	        {
		  cout << " That's a wall!\n";
	       	}
		else
	        {
	 	  tempXNode = n->SouthNext;
		  restricted = restrictionCheck(tempXNode, inVec);	  
		  if(!restricted)
		  {
		  	nextNode =  n->SouthNext;
	          	validPath = true;
		  }
		}       
		break;
	   case 3:
	       	if (n->EastNext == NULL)
	       	{
		   cout << " That's a wall!\n";
	       	}
		else
	        {
	 	  tempXNode = n->EastNext;
		  restricted = restrictionCheck(tempXNode, inVec);
		  if(!restricted) 
		  {
			nextNode =  n->EastNext;
	          	validPath = true;
	       	  }
		}
		break;
	   case 4:
	       	if (n->WestNext == NULL)
	        {
		  cout << " That's a wall!\n";
	       	}
		else
		{
	 	  tempXNode = n->WestNext;
		  restricted = restrictionCheck(tempXNode, inVec);
	          if(!restricted) 
		  {
			nextNode = n->WestNext;
	          	validPath = true;	
		  }      
	 	}
		break;
	   default:
	       cout << "Seriously! How did you get passed this?\n";
	       break;
  	}
   	++attemp;
	if(!validPath && attemp > 6)	//User doesn't have the right equipment
	{				//allow option to go back 
	   cout << "It doesn't look like you have the right equipment. How about go "
		<< "back and repick your item, eh? " << endl;
	   nextNode = n;		//return current node
	   validPath = true;
	}  
   }
   return nextNode;
}
//
//This function will perform a restriction check. First, check to see if the room has any
//restriction. Second, check to see if the room requires any special key. If there is a
//restriction and the bag contains the require item, then return false. Otherwise, return
//true - the room is restricted.
//
bool restrictionCheck(RoomNode* nextRoomNode, vector<Bag> inVec)
{
  string tempName;
  int tempRest, tempKey;
  bool tempReq, roomReq = false, itemReq = false;

  tempName = nextRoomNode->brPtr->getRoomName();
  tempRest = nextRoomNode->brPtr->getRestrictionCode();
  tempKey = nextRoomNode->brPtr->getRoomKey();
  tempReq = nextRoomNode->brPtr->getKeyRequire();
  
  if(tempRest == 1)
  {
	roomReq = itemCheck(inVec, 1); 	//Check for jacket in the bag
	if(!roomReq)			//Item not found
	{	
	   cout << "To go to the next room, you will need a jacket!" << endl;
	   return true;			//Impose restriction
  	}
	else
	{
	   if(tempReq)			//Require key
	   {
		itemReq = itemCheck(inVec, tempKey);
		if(!itemReq)		//Item not found
		{
		   displayRequirement(tempKey);
		   return true;		//Impose restriction
 		}
		else
		{
		   return false;	//No restriction
		}		
	   }
	   else				//Don't require key
	   {
		return false;		//No restriction
	   }   
	}
  }
  else if(tempRest == 2)
  {
	roomReq = itemCheck(inVec, 2);	//Check for candle in the bag
  	if(!roomReq)			//Item not found
	{
	   cout << "To go to the next room, you will need a candle!" << endl;
	   return true;			//Impose restriction
    	}
	else
	{
	   if(tempReq)			//Require key
	   {
		itemReq = itemCheck(inVec, tempKey);
		if(!itemReq)		//Item not found
		{
		   displayRequirement(tempKey);
		   return true;		//Impose restriction
 		}
		else
		{
		   return false;	//No restriction
		}		
	   }
	   else				//Don't require key
	   {
		return false;		//No restriction
	   }   
	}
  }
  else					//No room restriction
  {
   	if(tempReq)			//Require key
	{
	   itemReq = itemCheck(inVec, tempKey);
	   if(!itemReq)			//Item not found
	   {
	        displayRequirement(tempKey);
		return true;		//Impose restriction
 	   }
	   else
	   {
		return false;		//No restriction
	   }		
	}
	else				//Don't require key
	{
	   return false;		//No restriction
	}   
  }
}

void displayRequirement(int inR)
{
  switch(inR)
  {
	case 4:
	   cout << "To go to this room, you will need a map!" << endl;
	   break; 
	case 7:
   	   cout << "To go to this room, you will need a wooden key!" << endl;
 	   break;
	case 8:
	   cout << "To go to this room, you will need a golden key!" << endl;
	   break;
	case 9:
	   cout << "To go to this room, you will need a golden candle!" << endl;
	   break; 	
	default:
	   cout << "Interesting! It doesn't seem that you need anything for " << inR 
		<< "." << endl;
	   break;
  }
}
//
//This function will display the welcome message as well as the rules of the game
//
void displayWelcome()
{
   cout << "\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n";
   cout << "*    W E L C O M E   T O   T R E A S U R E    H U N T           *\n";
   cout << "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n";
   cout << "The object of the game is to navigate through maze and collect \n";
   cout << "items along the way. Some items will help you with your journey,\n";
   cout << "some won't. You can carry a maximum of 7 items total. So, choose wisely.\n";
   cout << "                    GOOD LUCK!!\n\n";

}

//
//This function will check to see if an item already exists in the user's bag. If it is,
//return true. Otherwise, return false.
//
bool itemCheck(vector<Bag> inVec, int inItem)
{
   bool itemFound = false;
   if(inVec.size() > 0)
   {
	for(int unsigned i = 0; i < inVec.size(); ++i)
	{
	   if(inVec[i].itemCode == inItem)
	   {
		itemFound = true;
	   }
	}
   }

   if(itemFound)
   {
//	cout << "Found item" << endl;
	return true;
   }
   else
   {
//	cout << "Item not found" << endl;
	return false;
   }
}

//
//This function will remove an item from the user's bag
//
void removeItem(vector<Bag>& inVec)
{
   int unsigned itemSelect = 0;
   cout << "Here are the items in the bag: " << endl;
   for(int unsigned i = 0; i < inVec.size(); ++i)
   {
	cout << i + 1 << " - " << inVec[i].itemName << endl;
   }
   cout << "Which item do you want to remove? ";
   cin >> itemSelect;
   while ((!cin) || itemSelect < 1 || itemSelect > inVec.size())
   {
	cin.clear();
	cin.ignore(1000,'\n');
	cout << "You have entered an invalid selection. Please enter a valid number: ";
	cin >> itemSelect;
   }
//Remove item
   inVec.erase(inVec.begin() + (itemSelect - 1));
   cout << "Item has been removed!" << endl;
}

//
//This function will create the initial map layout for all the room and assign special
//item for each room.
void createMap(RoomNode* &inHead, RoomNode* &inTail)
{
//Create pointer to objects
   BaseRoom* bsr1 = new RegRoom("First");
   BaseRoom* bsr2 = new ColdRoom("Second");
   BaseRoom* bsr3 = new RegRoom("Third");
   BaseRoom* bsr4 = new DarkRoom("Fourth");
   BaseRoom* bsr5 = new DarkRoom("Fifth");
   BaseRoom* bsr6 = new ColdRoom("Sixth");
   BaseRoom* bsr7 = new ColdRoom("Seventh");
   BaseRoom* bsr8 = new DarkRoom("Eighth");
   BaseRoom* bsr9 = new RegRoom("Ninth");
   BaseRoom* bsr10 = new RegRoom("Tenth");
   BaseRoom* bsr11 = new ColdRoom("Eleventh");
   BaseRoom* bsr12 = new DarkRoom("Twelfth");
//Create room nodes
   RoomNode* r1 = new RoomNode(bsr1);
   RoomNode* r2 = new RoomNode(bsr2);
   RoomNode* r3 = new RoomNode(bsr3);
   RoomNode* r4 = new RoomNode(bsr4);
   RoomNode* r5 = new RoomNode(bsr5);
   RoomNode* r6 = new RoomNode(bsr6);
   RoomNode* r7 = new RoomNode(bsr7);
   RoomNode* r8 = new RoomNode(bsr8);
   RoomNode* r9 = new RoomNode(bsr9);
   RoomNode* r10 = new RoomNode(bsr10);
   RoomNode* r11 = new RoomNode(bsr11);
   RoomNode* r12 = new RoomNode(bsr12);
//Connect rooms
   addDoor(r1,r2,3);
   addDoor(r2,r3,3);
   addDoor(r3,r6,2);
   addDoor(r6,r5,4);
   addDoor(r5,r4,4);
   addDoor(r4,r7,2);
   addDoor(r5,r8,2);
   addDoor(r6,r9,2);
   addDoor(r7,r8,3);
   addDoor(r8,r9,3);
   addDoor(r7,r10,2);
   addDoor(r10,r11,3);
   addDoor(r11,r12,3);   
//Add items to each room
   bsr1->setItem("Jacket","It's cold around here. You will need this item for the duration of the trip!",1);
   bsr1->setKeyRequire(false);
//   bsr1->display();
//   displayRoom(r1);

   bsr2->setItem("Candle","Hold me and NEVER let go!", 2);
   bsr2->setKeyRequire(false);
//   bsr2->display();
//   displayRoom(r2);

   bsr3->setItem("Bowl of ice cream","The next item you should seek is a map. In a bright room, it's not!", 3);
   bsr3->setKeyRequire(false);
//   bsr3->display();
//   displayRoom(r3);

   bsr4->setItem("Map", "It's always good to know where you're going. Take me you must! Next clue - to go over, one must go around",4);
   bsr4->setKeyRequire(false); 
//   bsr4->display();
//   displayRoom(r4);

   bsr5->setItem("12 packs of beer", "Beer is cold but won't get you very far!", 5);
   bsr5->setKeyRequire(false);
//   bsr5->display();
//   displayRoom(r5);

   bsr6->setItem("Hot pizza", "Hungry is temporary; wealthy will last much longer!", 6);
   bsr6->setKeyRequire(false); 
//   bsr6->display();
//   displayRoom(r6);

   bsr7->setItem("Wooden key", "The key word is 'key'. Nuff say!", 7);
   bsr7->setKeyRequire(true);
   bsr7->setRoomKey(4);	//Need map
//   bsr7->display();
//   displayRoom(r7);

   bsr8->setItem("Golden key", "Anything with gold is a must have! Next clue - now that you have the key, can you find the treasure?", 8);
   bsr8->setKeyRequire(true);
   bsr8->setRoomKey(9); //Need golden candle
//   bsr8->display();
//   displayRoom(r8);

   bsr9->setItem("Golden candle", "A golden candle? Need I say more?", 9);
   bsr9->setKeyRequire(true);
   bsr9->setRoomKey(7); //Need wooden key
//   bsr9->display();
//   displayRoom(r9);

   bsr10->setItem("Bowl of fruit", "Fruit is good but treasure is better!", 10);
   bsr10->setKeyRequire(false);
//   bsr10->display();
//   displayRoom(r10);

   bsr11->setItem("Bucket of booze", "Early celebration won't get you no where!", 11);
   bsr11->setKeyRequire(false);
//   bsr11->display();
//   displayRoom(r11);

   bsr12->setItem("TREASURE!!", "You found me! Now, call you boss and let them know that you won't be back to work!", 12);
   bsr12->setKeyRequire(true);
   bsr12->setRoomKey(8); //Need golden key
//   bsr12->display();
//   displayRoom(r12);

   inHead = r1;
   inTail = r12;  
}
