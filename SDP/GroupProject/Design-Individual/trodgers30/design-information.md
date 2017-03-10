# Author: Tyler Rodgers
# Title: design-information.md
# Purpose: Concise description of how the proposed design achieves the specified requirements.

1. Methods (with appropriate names) are encased in the GroceryListManager class to modify the items on each list. Also the list class is specified as an aggregation of the items class as a list can contain many items.
2. I added class items with attributes itemType and itemName. 
3. I added methods to the GroceryListManager class for adding items to a list and changing the quantity. The hierarchical list will be managed through the items class as it is representative of the database, which will be sorted to specifications.
4. Added a search method to GroceryListManager class to search the items class by name. I also added a verify method that will prompt the user to see to check if the items returned match what they were searching for. The selectItemType method will prompt users to proved an item type with no match could be found. 
5. I included a method in the GroceryListManager to save each list whenever it is modified. The class also keeps track of whether recent changes have been saved through a boolean parameter.
6. I have included a check mark method within the GroceryListManager class that takes the list name and item name as 2of 3 of the input parameters.
7. I have included a clear check mark method within the GroceryListManager class that takes the list name as one of the input parameters.
8. I included a method in the GroceryListManager to save each list whenever it is modified. The class also keeps track of whether recent changes have been saved through a boolean parameter.
9. I have included a sort method within the GroceryListManager class to sort the lists by item type.
10. Appropriately named methods have been added to the GroceryListManager. Also all classes take the list name into consideration so that there is independence amongst the lists. Alse depict the n:1 relationship.
11. Not considered because it does not effect the design directly.