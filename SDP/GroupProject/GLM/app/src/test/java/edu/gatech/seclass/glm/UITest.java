package edu.gatech.seclass.glm;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.robolectric.Robolectric;
import org.robolectric.RobolectricGradleTestRunner;
import org.robolectric.annotation.Config;
import android.os.Build;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Spinner;

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

@RunWith(RobolectricGradleTestRunner.class)
@Config(constants = BuildConfig.class, sdk = Build.VERSION_CODES.LOLLIPOP)
public class UITest {
    private MainActivity mainActivity;
    private AddItemActivity addItemActivity;
    private AddToDBActivity addToDBActivity;
    private ListActivity listActivity;
    private ListItemActivity listItemActivity;

    @Before
    public void setUp() throws Exception {
        // Another way to declare instance
        // mainActivity = Robolectric.buildActivity(MainActivity.class).create().start().resume().get();
        mainActivity = Robolectric.setupActivity(MainActivity.class);
        addItemActivity = Robolectric.setupActivity(AddItemActivity.class);
        addToDBActivity = Robolectric.setupActivity(AddToDBActivity.class);
        listActivity = Robolectric.setupActivity(ListActivity.class);
        listItemActivity = Robolectric.setupActivity(ListItemActivity.class);
    }

    // Check all required activities exist
    @Test
    public void testInitAllActivity() throws Exception {
        assertNotNull(mainActivity);
        assertNotNull(addItemActivity);
        assertNotNull(addToDBActivity);
        assertNotNull(listActivity);
        assertNotNull(listItemActivity);
    }

    // Check all required elements exist in MainActivity
    @Test
    public void lookUpMainActivity(){
        ListView lv = (ListView) mainActivity.findViewById(R.id.listView);
        EditText et = (EditText) mainActivity.findViewById(R.id.newListEditText);
        Button addNewList = (Button) mainActivity.findViewById(R.id.addNewListButton);

        assertNotNull("ListView could not be found in MainActivity", lv);
        assertNotNull("EditText could not be found in MainActivity", et);
        assertNotNull("Add new list button could not be found in MainActivity", addNewList);
    }

    // Check all required elements exist in ListActivity
    @Test
    public void lookUpListActivity(){
        ListView lv = (ListView) listActivity.findViewById(R.id.listView);
        Button addItem = (Button) listActivity.findViewById(R.id.addItemButton);
        Button uncheckItem = (Button) listActivity.findViewById(R.id.uncheckItemsButton);

        assertNotNull("ListView could not be found in ListActivity", lv);
        assertNotNull("Add item button could not be found in ListActivity", addItem);
        assertNotNull("Uncheck item button could not be found in ListActivity", uncheckItem);
    }

    // Check all required elements exist in ListItemActivity
    @Test
    public void lookUpListItemActivity(){
        TextView tv = (TextView) listItemActivity.findViewById(R.id.textView4);
        Spinner spinner = (Spinner) listItemActivity.findViewById(R.id.itemTypeSpinner);
        ListView lv = (ListView) listItemActivity.findViewById(R.id.itemListView);
        EditText et = (EditText) listItemActivity.findViewById(R.id.searchForItemByNameEditTExt);
        Button search = (Button) listItemActivity.findViewById(R.id.searchButton);
        //Button cancel = (Button) listItemActivity.findViewById(R.id.CancelSelectItemButton);
        Button addToDB = (Button) listItemActivity.findViewById(R.id.addToDBButton);

        assertNotNull("TextView could not be found in ListItemActivity", tv);
        assertNotNull("Spinner could not be found in ListItemActivity", spinner);
        assertNotNull("EditText could not be found in ListItemActivity", et);
        assertNotNull("Search button could not be found in ListItemActivity", search);
        //assertNotNull("Cancel Selected Item button could not be found in ListItemActivity", cancel);
        assertNotNull("Add to DB button could not be found in ListItemActivity", addToDB);
    }

    // Check all required elements exist in AddToDBActivity
    @Test
    public void lookUpAddToDBActivity(){
        TextView itemType = (TextView) addToDBActivity.findViewById(R.id.textView);
        TextView itemName = (TextView) addToDBActivity.findViewById(R.id.textView2);
        TextView itemUnit = (TextView) addToDBActivity.findViewById(R.id.textView3);
        TextView searchFor = (TextView) addToDBActivity.findViewById(R.id.textView4);
        EditText itemInType = (EditText) addToDBActivity.findViewById(R.id.itemInType);
        EditText itemInName = (EditText) addToDBActivity.findViewById(R.id.itemInName);
        EditText itemInUnit = (EditText) addToDBActivity.findViewById(R.id.itemInUnit);
        EditText searchItemName = (EditText) addToDBActivity.findViewById(R.id.searchItemName);
        EditText queryDisplay = (EditText) addToDBActivity.findViewById(R.id.queryDisplay);
        Button add = (Button) addToDBActivity.findViewById(R.id.button);
        Button query = (Button) addToDBActivity.findViewById(R.id.button2);
        Button search = (Button) addToDBActivity.findViewById(R.id.button4);
        Button retrieve = (Button) addToDBActivity.findViewById(R.id.button5);

        assertNotNull("Item type TextView could not be found in AddToDBActivity", itemType);
        assertNotNull("Item name TextView could not be found in AddToDBActivity", itemName);
        assertNotNull("Item unit TextView could not be found in AddToDBActivity", itemUnit);
        assertNotNull("Search for TextView could not be found in AddToDBActivity", searchFor);
        assertNotNull("Item in type EditText could not be found in AddToDBActivity", itemInType);
        assertNotNull("Item in name EditText could not be found in AddToDBActivity", itemInName);
        assertNotNull("Item in unit EditText could not be found in AddToDBActivity", itemInUnit);
        assertNotNull("Search item name EditText could not be found in AddToDBActivity", searchItemName);
        assertNotNull("Query display EditText could not be found in AddToDBActivity", queryDisplay);
        assertNotNull("Add button could not be found in AddToDBActivity", add);
        assertNotNull("Query Database button could not be found in AddToDBActivity", query);
        assertNotNull("Search Item button could not be found in AddToDBActivity", search);
        assertNotNull("Retrieve List button could not be found in AddToDBActivity", retrieve);
    }

    // Check all required elements exist in AddItemActivity
    @Test
    public void lookUpAddItemActivity(){
        TextView addItemToList = (TextView) addItemActivity.findViewById(R.id.textView5);
        EditText itemName = (EditText) addItemActivity.findViewById(R.id.itemNameEditText);
        EditText itemType = (EditText) addItemActivity.findViewById(R.id.itemTypeEditText);
        EditText itemQty = (EditText) addItemActivity.findViewById(R.id.itemQuantityEditText);
        EditText itemUnit = (EditText) addItemActivity.findViewById(R.id.itemUnitEditText);
        Button add = (Button) addItemActivity.findViewById(R.id.addItemToListButton);

        assertNotNull("Add Item to List TextView could not be found in AddItemActivity", addItemToList);
        assertNotNull("Item Name EditText could not be found in AddItemActivity", itemName);
        assertNotNull("Item Type EditText could not be found in AddItemActivity", itemType);
        assertNotNull("Item Quantity EditText could not be found in AddItemActivity", itemQty);
        assertNotNull("Item Unit EditText could not be found in AddItemActivity", itemUnit);
        assertNotNull("Add Item to List button could not be found in AddItemActivity", add);
    }
}
