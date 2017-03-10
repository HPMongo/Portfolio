# Test Plan

*This is the template for your test plan. The parts in italics are concise explanations of what should go in the corresponding sections and should not appear in the final document.*

**Author**: \<Team64\>

__Please leave a brief comment like below when you add or edit so that everyone can track the update.__

- 1st version of test plan document(TestPlan_1st) has been pushed on Aug. 6. 2016 by Kihoon Ahn
- 2nd version of test plan document(TestPlan_2nd)is updated by Kihoon Ahn
- Final version of test plan document (TestPlan) is being updated.

## 1 Testing Strategy

### 1.1 Overall strategy

*This section should provide details about your unit-, integration-, system-, and regression-testing strategies. In particular, it should discuss which activities you will perform as part of your testing process, and who will perform such activities.*

- Unit testing – Each test will be separately executed, and some situational modules should be coded before running the tests. For example, storing an item into a list should be preceded before checking if the item is correctly stored in the proper list. The use case will provide what test cases would be needed for the unit testing. Before writing the test cases, we will organize 1) what situations would be occurred, 2) what actions would be applied to each situation.

- Integration testing – After the unit testing, the integration testing will be executed. For the integration testing, the created modules for the unit testing should be combined one by one depending on a specific purpose and its coverage. When the modules are integrated, we will check the combinational behavior, and confirm if the requirements are correctly implemented or not. 

- System testing – After the integration testing is completed, the system testing will be performed. The purpose of the system testing is to check if all requirements have been fulfilled, and if there is an unfulfilled part, then provide the objective evidence. For doing the system testing, 1) write all the requirements and expectations, 2) prepare a test scenario. Focus on, interfaces, functionalities, performance, interaction between user and system, installability, documentation, and usability. 

- Regression testing – Finally, the regression testing will be performed to check if the existing features are totally free from any code changes such as adding new code. We will prioritize the test cases depending on impact, critical, and frequently used functionalities, and after that the testing will be executed under the conditions: no code and database changes during the regression test phase. 

Each testing is performed by all members of Team64, and the order of testing is 1) Unit testing, 2) Integration testing, 3) System testing, and 4) Regression testing.

Since this is the 1st draft of test plan, we will list only system tests in this document, and the other testing will be accordingly updated depending on the progress of this project.

### 1.2 Test Selection

*Here you should discuss how you are going to select your test cases, that is, which black-box and/or white-box techniques you will use. If you plan to use different techniques at different testing levels (e.g., unit and system), you should clarify that.*

- White-Box test(Unit testing): 
For doing the unit testing, the auditor must know the inner structure of logics in the program, and need to create some modules based on the logical structure. It means that the tester should be auditing through the program to execute the test, so in this case, the White-Box test is suitable because the White-Box test is performed in the same context (testing internal structures).

- Black-Box test(Integration, System, and Regression): 
To implement this program, we focused on the sequential progress based on the provided software requirements. The main point of the black-box test is cause and effect. The reason why we choose the black-box test technique is that the sequential progress and 'cause and effect' have a common points, simply focusing on input and outcome. Also, this project should be finished in a short time, so in this situation, the black-box test would be suitable technique. In the Black-Box testing, it is not required to look through the code, but rather according to the provided requirements, the auditor needs to set a input and expect what would be effected by the cause to do testing.


### 1.3 Adequacy Criterion

*Define how you are going to assess the quality of your test cases. Typically, this involves some form of functional or structural coverage. If you plan to use different techniques at different testing levels (e.g., unit and system), you should clarify that.*

- Unit testing (White-Box test): 
Grocery List Manager is a relatively simple program. In this program, there would be no complex path between modules, and it would have a simple data structure. Therefore, the statement and condition coverage will be simply used to perform unit tests. As the statement coverage testing, we should check if every module in the program is executed at least one time. For example, there is a ‘createList’ method in the GroceryList class, and the method should have a ‘listName’ parameter of String type to be executed. Since we know the overall structure, we can try to execute the ‘createList’ method with a parameter using an instance of the GroceryList class. This testing will be assessed by the execution result of each test case. Also, as the condition coverage testing, we can compare an item in database after the progress of storing the item into a list. This testing will be assessed by the result of match or mismatch.

- Integration, System, and Regression Testing (Black-Box test): 
Testing will be performed according to the written test cases, and the test cases will be written based on the provided requirements. Each test case has a brief description of specific cause, expected effect, and actual result. Pass or Failure result is the standard of how we assess the test cases.

### 1.4 Bug Tracking

*Describe how bugs and enhancement requests will be tracked.*

GitHub has issue tracking feature, so we will simply use the feature for bug tracking. 

### 1.5 Technology

*Describe any testing technology you intend to use or build (e.g., JUnit, Selenium).*

We will commonly use Android Studio 2.2.0 to develop and JUnit to test. Also, we tried to use Espresso for testing, but we couldn't resolve some issues (build issue and bug linked http://stackoverflow.com/questions/26150522/espresso-withtext-in-textview-doesnt-match-selected-view), so we decided to use Robolectric (http://robolectric.org/getting-started/) for testing. To run test, API level is 21-22, Lollipop because Robolectric doesn't support the higher level (API 23~). Since each team member has a different machine, Mac, PC and Linux, consequentially, it has an advantage of testing our code on the different machine.

## 2 Test Cases

*This section should be the core of this document. You should provide a table of test cases, one per row. For each test case, the table should provide its purpose, the steps necessary to perform the test, the expected result, the actual result (to be filled later), pass/fail information (to be filled later), and any additional information you think is relevant.*

- Unit Testing

UITest.java (Purpose: check that all required elements are prepared)

| # |                         Purpose                     |         Expected Result            | Actual Result| Pass / Fail |
|---|-----------------------------------------------------|------------------------------------|--------------|-------------|
| 1 |Check all required activities exist                  |assertNotNull(Each Activity) -> True|     True     |     Pass    |
| 2 |Check all required elements exist in MainActivity    |assertNotNull(Each Element) -> True |     True     |     Pass    |
| 3 |Check all required elements exist in ListActivity    |assertNotNull(Each Element) -> True |     True     |     Pass    |
| 4 |Check all required elements exist in ListItemActivity|assertNotNull(Each Element) -> True |     True     |     Pass    |
| 5 |Check all required elements exist in AddToDBActivity |assertNotNull(Each Element) -> True |     True     |     Pass    |
| 6 |Check all required elements exist in AddItemActivity |assertNotNull(Each Element) -> True |     True     |     Pass    |

'#1: MainActivity, AddItemActivity, AddToDBActivity, ListActivity, ListItemActivity

'#2: 1 ListView, 1 EditText, 1 Button

'#3: 1 ListView, 2 Button

'#4: 1 TextView, 1 Spinner, 1 ListView, 1 EditText, 2 Button

'#5: 4 TextView, 5 EditText, 5 Button

'#6: 1 TextView, 4 EditText, 2 Button

DBHelper.java (Purpose: test that helper is ready to interact with DB)

|             Purpose              |                    Expected Result                |           Actual Result        | Pass / Fail |
|----------------------------------|---------------------------------------------------|--------------------------------|-------------|
|Check DB is correctly created     |DB should be opened                                |DB is correctly opened          |     Pass    |
|Check DB columns                  |Needed columns should be existed in DB table       |Columns correctly exist         |     Pass    |
|Test adding item into DB          |An item should be added into DB                    |Item is correctly added         |     Pass    |
|Test searching item in DB         |a new added item should be searched from DB        |Item is correctly searched      |     Pass    |
|Test displaying all items in DB   |testDisplayAllItems method should return all items |All items are correctly returned|     Pass    |

UnitTest.java (Purpose: check that modules in each activity work correctly)

- MainActivity

| # |     Test Case     |                      Expected Result                 |Actual Result| Pass / Fail |
|---|-------------------|------------------------------------------------------|-------------|-------------|
| 1 |testViewAllLists() |assertEquals() -> True                                |     True    |    Pass     |
| 2 |testCreateList()   |assertTrue(), assertEquals() -> True                  |     True    |    Pass     |
| 3 |testDeleteList()   |                                                      |             |             |
| 4 |testRenameList()   |                                                      |             |             |
| 5 |testSelectList()   |                                                      |             |             |

'#1: Compare the sizes of the Master List(listOfLists) and ListView list's actual size after create a list

'#2: 1) check that the created list exists in the Master List((listOfLists)) 2) same with #1's comparison way

'#3:

'#4: 

'#5: 
 
- ListActivity

| # |        Test Case       |                      Expected Result                 |Actual Result| Pass / Fail |
|---|------------------------|------------------------------------------------------|-------------|-------------|
| 1 |testViewAllItems()      |assertEquals() -> True                                |     True    |    Pass     |
| 2 |testClearAllCheckOffs() |assertEquals() -> True                                |     True    |    Pass     |
| 3 |testAddItem()           |                                                      |             |             |
| 4 |testDeleteItem()        |                                                      |             |             |

'#1: Compare the sizes of the listOfItems and ListViw list's actual size

'#2: Perform clearAllCheckOffs method, and after that, check all checkboxs if they are checkoffed

'#3:

'#4: 

- AddToDBActivity
 
| # |        Test Case       |        Expected Result                 |    Actual Result   | Pass / Fail |
|---|------------------------|----------------------------------------|--------------------|-------------|
| 1 |testHandleAdd()         |assertEquals() -> True                  |        True        |    Pass     |
| 2 |testHandleQuery()       |assertEquals() -> True                  |        True        |    Pass     |
| 3 |testHandleSaveList()    |assertEquals() -> True                  |        True        |    Pass     |
| 4 |testHandleRetrieveList()|assertEquals() -> True                  |        True        |    Pass     |
| 5 |testHandleSearch()      |assertEquals() -> True                  |        True        |    Pass     |

'#1: 1) set only item type, then click add button -> Toast message "Item type, name and unit are required" 2) set item type and item name, then click add button -> Toast message "Item type, name and unit are required" 3) set all fileds, then click add button -> Toast message "Item is added!"

'#2: 1) Manually create the result of query 2) Perform Query button 3) Compare the result of 1) and 2)

'#3: 1) Perform click without input item name -> Toast message "Item name is required for saving" 1) Perform click after input item name -> Toast message "Running initial set up"

'#4: Create text including the Master List's size. After that click Retrieve button, and compare the created text and Toast message

'#5: 1) Try to search without input item name -> Toast message "Item name is required" 2) Manually create a result of query, and after that compare the created result and the performed result.

- AddItemActivity
 
| # |             Test Case          |        Expected Result                 |    Actual Result   | Pass / Fail |
|---|--------------------------------|----------------------------------------|--------------------|-------------|
| 1 |testOnClickAddItemToList()      |assertEquals() -> True                  |         Bug        | Fail to test|
| 2 |testOnClickCancelAddItemToList()|assertNotNull() -> True                 |         True       |     Pass    |
| 3 |testOnClickCheckDatabase()      |assertNotNull() -> True                 |         True       |     Pass    |

'#1: 1) Perform click without input ItemQty, then Toast Message "Item type, name, quantity and unit are required" 2) All Item Name, Type, Unit, and Qty are set, after that perform click, then Toast Message "Item is added!"

'#2: Check the intent from AddItemActivity to ListItemActivity

'#3: Check the intent from AddItemActivity to AddToDBActivity

- ListItemActivity
 
| # |               Test Case            |        Expected Result                 |    Actual Result   | Pass / Fail |
|---|------------------------------------|----------------------------------------|--------------------|-------------|
| 1 |testViewAllItemsInListItemActivity()|assertFalse() -> True                   |        True        |     Pass    |
| 2 |testOnClickSearchButton()           |assertEquals() -> True                  |        True        |     Pass    |
| 3 |testOnClickAddToDBButton()          |assertNotNull() -> True                 |        True        |     Pass    |

'#1: Perform viewAllItems method, after that check the status of adapter of ListItemActivity

'#2: Perform search button click, after that compare the Toast message

'#3: Check the intent from ListItemActivity to AddItemActivity


- Integration Testing

- System Testing

|                    Action                         |                  Expected Result             |           Actual Result                 | Pass / Fail |
|---------------------------------------------------|----------------------------------------------|-------------------------------------------|-------------|
|Install the app (APK file)                         |The app is installed                          |The app is correctly installed        |     Pass    |
|Launch the app                                     |The app main GUI appears                      |Main GUI is correctly appeared         |     Pass    |
|Press the + button on main screen                  |A new list is added into below                |A new list is showed below                  |     Pass    |
|Press trashcan button                              |A selected list is removed                    |The selected list is removed          |     Pass    | 
|Select a list on main screen                       |2nd screen appears, shows items               |2nd screen is correctly appeared         |     Pass    |
|Long click on a list with a new name               |Selected list's name is changed               |Selected list's name is correctly changed|     Pass    |
|Click the checkbox                                 |Checkmarks on/off                             |Checkmarks correctly on/off           |     Pass    |
|Click UNCHECK ITEMS button on 2nd screen           |All checkmarks off                            |All checkmarks correctly off              |     Pass    |
|Press trashcan button next to item                 |A selected item is removed                    |The selected item is removed          |     Pass    |
|Click ADD ITEM button on 2nd screen                |3rd screen appears, Select Item to Add to List|3rd screen is correctly appeared         |     Pass    |
|Click SEARCH button on 3rd screen                  |Show a search result                          |Wrong message appeared                     |     Fail    |
|Click ADD NEW ITEM TO DATABASE button on 3rd screen|4th screen appears, Add Item to List          |4rd screen is correctly appeared         |     Pass    |
|Click Cancel button on 4th screen                  |Go back to the 4th screen                     |Correctly go back to the 
4th screen       |     Pass    |
|Click ADD ITEM TO LIST button on 4th screen        |Show the result of add                        |Wrong Message                              |     Fail    |
|Click CHECK DATABASE button on 4th screen          |5th screen appears, db handler                |5th screen is correctly appeared         |     Pass    |
|Click ADD button on 5th screen                     |Show the result of add                        |Showed the result correctly        |     Pass    |
|Click QUERY DATABASE button on 5th screen          |Show the result of query                      |Showed the result correctly        |     Pass    |
|Click SEARCH ITEM button on 5th screen             |Show the result of search                     |Showed the result correctly        |     Pass    |
|Click RETRIEVE LIST button on 5th screen           |Show the result of retrieve                   |Showed the result correctly        |     Pass    |
|Click Back button on the device                    |Go back to the previous screen                |Correctly go back to the previous screen  |     Pass    |
|Click Home button on the device                    |Shows home screen of the device               |Correctly shows home  of the device       |     Pass    |
|Click Menu button on the device                    |Minimize the app, if click X, then terminate  |Correctly minimized, and can be terminated|     Pass    |



- Regression Testing: will be updated later
