# Test Plan

*This is the template for your test plan. The parts in italics are concise explanations of what should go in the corresponding sections and should not appear in the final document.*

**Author**: \<Team64\>

__Please leave a brief comment like below when you add or edit so that everyone can track the update.__

- 1st version of test plan document(TestPlan_1st) has been pushed on Aug. 6. 2016 by Kihoon Ahn
- 2nd version of test plan document(TestPlan_2nd)is updated by Kihoon Ahn

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

We will commonly use Android Studio 2.2.0 to develop and JUnit to test. 
Also, we tried to use Espresso for testing, but we couldn't resolve some issues (build issue and bug linked http://stackoverflow.com/questions/26150522/espresso-withtext-in-textview-doesnt-match-selected-view), so we decided to use Robolectric (http://robolectric.org/getting-started/) for testing. To run test, API level is 21-22, Lollipop because Robolectric doesn't support the higher level (API 23~). Since each team member has a different machine, Mac, PC and Linux, consequentially, it has an advantage of testing our code on the different machine.

## 2 Test Cases

*This section should be the core of this document. You should provide a table of test cases, one per row. For each test case, the table should provide its purpose, the steps necessary to perform the test, the expected result, the actual result (to be filled later), pass/fail information (to be filled later), and any additional information you think is relevant.*

- Unit Testing

DBHelper.java

|             Purpose              |                    Expected Result                |           Actual Result        | Pass / Fail |
|----------------------------------|---------------------------------------------------|--------------------------------|-------------|
|Check DB is correctly created     |DB should be opened                                |DB is correctly opened          |     Pass    |
|Check DB columns                  |Needed columns should be existed in DB table       |Columns correctly exist         |     Pass    |
|Test adding item into DB          |An item should be added into DB                    |Item is correctly added         |     Pass    |
|Test searching item in DB         |a new added item should be searched from DB        |Item is correctly searched      |     Pass    |
|Test displaying all items in DB   |testDisplayAllItems method should return all items |All items are correctly returned|     Pass    |

- Integration Testing: will be updated later

- System Testing

|             Action               |                       Expected Result             |           Actual Result        | Pass / Fail |
|----------------------------------|---------------------------------------------------|--------------------------------|-------------|
|Install the app (APK file)        |The app is installed                               | The app is correctly installed |     Pass    |
|Launch the app                    |The app main GUI appears                           | Main GUI is correctly appeared |     Pass    |
|Press the Add List button         |A new list is added                                | A new list is correctly added  |     Pass    |
|Press the Add Items button        |A new item is added                                | A new item is correctly added  |     Pass    |
|Press the View Items button       |Viewing all items in DB                            | Can view all items in DB       |     Pass    |

- Regression Testing: will be updated later
