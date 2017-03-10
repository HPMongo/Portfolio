#ifndef FUNCTION_H
#define FUNCTION_H

#include<iostream>
#include<string>
#include<vector>

using std::string;
using std::vector;

/*
 *ITEMS CLASS
 */
class Items
{
protected:
   string itemName;
   string itemHints;
   int itemKey;
public:
   void setItemName(string);
   void setItemKey(int);
   void setItemHints(string);
   string getItemName();
   string getItemHints();
   int getItemKey();
};

/*
 * BASEROOM CLASS
 */
class BaseRoom
{
public:
   string getRoomName();
   int getRestrictionCode();
   virtual void setItem(string, string, int) = 0;
   void setKeyRequire(bool);
   bool getKeyRequire();
   void setRoomKey(int);
   int getRoomKey();
   void display();
   Items itemInRoom;
protected:
   string roomName;
   string roomType;
   bool restriction;
   int restrictionCode;
   bool keyRequire;
   int roomKey;
   friend struct RoomNode;
};

/*
 * REGULAR ROOM CLASS
 */
class RegRoom : public BaseRoom
{
public:
   RegRoom(string);
   virtual void setItem(string, string, int);
};

/*
 * DARK ROOM CLASS
 */
class DarkRoom : public BaseRoom
{
public:
   DarkRoom(string);
   virtual void setItem(string, string, int);
};

/*
 * COLD ROOM CLASS
 */
class ColdRoom : public BaseRoom
{
public:
   ColdRoom(string);
   virtual void setItem(string, string, int);
};


/*
 * ROOMNODE STRUCT
 */
struct RoomNode
{ 
   RoomNode(BaseRoom* bRoom);
   BaseRoom* brPtr;
   RoomNode *NorthNext;
   RoomNode *SouthNext;
   RoomNode *EastNext;
   RoomNode *WestNext;
};

/*
 * BAG STRUCT
 */
struct Bag
{
   int itemCode;
   string itemName;
};
//
//This function will display the different information in the Room Node - use for 
//debugging
//
void displayRoom(RoomNode*);

//
//This function will connect the room together via different doors
//
void addDoor(RoomNode*, RoomNode*, int);

//
//This function will create the layout of the room and assign secret item to each room
//
void createMap(RoomNode*&, RoomNode*&);

//
//This is the main logic to allow the user to move from room to room
//
RoomNode *  mainLogic(RoomNode*, vector<Bag>);

//
//This function will perform an item check against a list of items. It will return true
//if the item is found and false otherwise.
//
bool itemCheck(vector<Bag>, int);

//
//This function will remove an item from the list of items
//
void removeItem(vector<Bag>&);

//
//This function will validate the different restrictions for each room (room restriction,
//item restriction). Return false if no restriction is needed and true otherwise.
//
bool restrictionCheck(RoomNode*, vector<Bag>);

//
//This function will display the require item to enter the room
//
void displayRequirement(int);

//
//This function will display the welcome message the the rules of the game.
//
void displayWelcome();

#endif
