package edu.gatech.seclass.glm;

/**
 * This is an interface for the main class of the app.
 * It will control the modification of list objects.
 */
public interface MainActivityInterface {


    /*
     *
     */
    void viewAllLists();

    /*
     *
     */
    void selectList(MainActivity.MyCustomAdapter.ViewHolder holder);

    /*
     *
     */
    void createList(String listName);


    /*
     *
     */
    void deleteList(MainActivity.MyCustomAdapter.ViewHolder holder);

    /*
     *
     */
    void renameList(MainActivity.MyCustomAdapter.ViewHolder holder);



}
