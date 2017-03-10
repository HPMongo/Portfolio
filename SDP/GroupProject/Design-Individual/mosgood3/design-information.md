GroceryListManager Design Information
Mark Osgood

Requirements and their >> Implementation:

1. A grocery list consists of items the users want to buy at a grocery store. The application must allow users to add items to a list, delete items from a list, and change the quantity of items in the list (e.g., change from one to two pounds of apples).
>> The class "List" was created to realize the grocery list. It has the attribute "ListName". Associated with the "List" class is the "ListEntry" class. Grocery Lists are composed of "ListEntry's". Each instance of a ListEntry has the attribute "Qty" to record the quantity of that particular item and "CheckedOff" to record the checked or not-checked off state of the item. The ListEntry class has the operations "Add", "Delete" and "SetQty" to allow the adding, deleting and updating the desired quantity of each item in the list. 

2. The application must contain a database (DB) of items and corresponding item types.
>> A class called "ItemDataBase" was created to realize this. Each item has the attributes "ItemName" and "ItemType". To facilitate adding and deleting entries to the database the "ItemDataBase" class has the operations "AddToDB" and "DeleteFromDB".

3. Users must be able to add items to a list by picking them from a hierarchical list, where the first level is the item type (e.g., cereal), and the second level is the name of the actual item (e.g., shredded wheat). After adding an item, users must be able to specify a quantity for that item.
>> The bottom or initially only entry in an empty list will be named "Add Item". Clicking on this will bring up a window with three entry boxes labelled "Item Type", "Item Name" and "Qty". If the user clicks on the box labeled "Type" this will bring up pick list showing all available types of items. To populate the types picklist the "ItemDataBase" class has an operation called "ListAllItemTypes" that will returns a list of all item types currently in the database. If the use selects a type of item first then when they click on the "Item Name" entry box they are presented with a pick list of all items that are of this type. To facilitate populating this picklist the "ItemDatabase" class has an operation called "ListAllItemsOfType" that returns a list of all items with the selected type. To facilitate adding a new item if the one the user desires is not in the drop down list the bottom of the pick list will always be populated with the entry "Add New Item" and this will use the "ItemDataBase" operation "AddToDB".

4. Users must also be able to specify an item by typing its name. In this case, the application must look in its DB for items with similar names and ask the users, for each of them, whether that is the item they intended to add. If a match cannot be found, the application must ask the user to select a type for the item and then save the new item, together with its type, in its DB.
>> To add an item to a list the user will push a button labelled "Add Item" in the GUI. This brings up a window with three entry boxes labelled Item "Item Type", "Item Name" and "Qty". In the description of implementing requirement 3 it was described how an item is added to the list when the user first selects the item type and then item name. If the user types directly in the "Item Name" entry box the operation "SearchItemName" in the "ItemDataBase" class will be used to return a list of items with similar names and the user can click on one of these entries to add it to the list.

5. Lists must be saved automatically and immediately after they are modified.
>> After any item is added or deleted from the list "Save" operation in the List class is executed.

6. Users must be able to check off items in a list (without deleting them).
>> The "ListEntry" class has a "CheckedOff" attribute that records whether a particular list item has been checked off or not. The item is not deleted from the list when "CheckedOff" is set to true. It is simple marked as such in the GUI.

7. Users must also be able to clear all the check-off marks in a list at once.
>> To facilitate un-checking all items in a given list the "List" class has a "UnCheckAll" operation that will set the "CheckedOff" attribute for each entry in the list to false.

8. Check-off marks for a list are persistent and must also be saved immediately.
>> Each time an item is checked or un-checked the "Save" operation in the List class is executed.

9. The application must present the items in a list grouped by type, so as to allow users to shop for a specific type of products at once (i.e., without having to go back and forth between aisles).
>> This is principally a function of the GUI. The bottom or initially only entry in an empty list will be named "Add Item". As users add items to a list as described in requirements 3 and 4 implementation the List will grow but items will be shown in a sorted order first by type and then by item name alphabetically within the type. This will be shown in an indented heading style with the "ItemType" providing the headings. 

10. The application must support multiple lists at a time (e.g., “weekly grocery list”, “monthly farmer’s market list”). Therefore, the application must provide the users with the ability to create, (re)name, select, and delete lists.
>> To realize this the "List" class has the corresponding operations of "Create", "Rename", "Select", and "Delete". Top level class "ListManager" has the operation "Lists" that returns a list of all existing lists.  

11. The User Interface (UI) must be intuitive and responsive.
>> Not considered because the intuitiveness and responsiveness is not a function of the Class level design.
