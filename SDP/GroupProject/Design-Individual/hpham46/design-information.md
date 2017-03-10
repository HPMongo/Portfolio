Design considerations.
1.	A grocery list consists of items the users want to buy at a grocery store. The application must allow users to add items to a list, delete items from a list, and change the quantity of items in the list (e.g., change from one to two pounds of apples).
	->The GroceryList class will allow the users to create list, rename an existing list, copy an existing list, delete a list or view all available lists. These actions will be done via various functions in the class (such as createList(), renameList(), etc).
2.	The application must contain a database (DB) of items and corresponding item types.
	->A simple database will be created to store the item names and the corresponding types. Database helper functions will also be built to assist querying the database as well as adding new item to the database.
3.	Users must be able to add items to a list by picking them from a hierarchical list, where the first level is the item type (e.g., cereal), and the second level is the name of the actual item (e.g., shredded wheat). After adding an item, users must be able to specify a quantity for that item.
	->Through the database helper function (querying the database), the user will be able to view the list of available items. The actual application will have several scenes to allow the user add the item in a sequential order.
4.	Users must also be able to specify an item by typing its name. In this case, the application must look in its DB for items with similar names and ask the users, for each of them, whether that is the item they intended to add. If a match cannot be found, the application must ask the user to select a type for the item and then save the new item, together with its type, in its DB.
	->The database search function will allow the user to search for a specific item. The add function will allow the user to add the new item to the database if it does not exist.
5.	Lists must be saved automatically and immediately after they are modified.
	->This is not being considered in the UML design as it will be handled with the implementation.
6.	Users must be able to check off items in a list (without deleting them).
	->updateStatus() function will allow the user to check an item off of the list.
7.	Users must also be able to clear all the check-off marks in a list at once.
	->clearAllStatus() function will allow the user to clear all check-off marks on the list.
8.	Check-off marks for a list are persistent and must also be saved immediately.
	->This will be handled at the time of implementation.
9.	The application must present the items in a list grouped by type, so as to allow users to shop for a specific type of products at once (i.e., without having to go back and forth between aisles).
	->This will be handled at the time of the implementation (i.e. UI design stage).
10.	The application must support multiple lists at a time (e.g., “weekly grocery list”, “monthly farmer’s market list”). Therefore, the application must provide the users with the ability to create, (re)name, select, and delete lists.
	->Various functions within the GroceryList class should allow the user to perform multiple functions on multiple lists.
11.	The User Interface (UI) must be intuitive and responsive.
	->This will be handled at the time of UI design.
