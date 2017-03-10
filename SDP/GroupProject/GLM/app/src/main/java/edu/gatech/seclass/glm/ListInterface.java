package edu.gatech.seclass.glm;

import android.widget.ListView;

import java.util.ArrayList;

/**
 * This is an interface for the List class
 */

public interface ListInterface {

    void sort(ArrayList<Items> listOfItems);
    void addItem();
    void clearAllCheckOffs(ListView listView);
    void deleteItem(ListActivity.MyCustomAdapter.ViewHolder holder);
    void viewAllItems();

}
