package edu.gatech.seclass.glm;

import android.os.Build;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.CheckBox;
import android.view.View;
import android.view.ViewGroup;
import android.view.LayoutInflater;
import android.content.Context;
import android.content.Intent;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.robolectric.Robolectric;
import org.robolectric.shadows.ShadowLooper;
import org.robolectric.shadows.ShadowToast;
import org.robolectric.RobolectricGradleTestRunner;
import org.robolectric.annotation.Config;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertThat;
import static org.junit.Assert.fail;

import android.widget.ArrayAdapter;
import java.util.ArrayList;

import edu.gatech.seclass.glm.db.DBHelper;

@RunWith(RobolectricGradleTestRunner.class)
@Config(constants = BuildConfig.class, sdk = Build.VERSION_CODES.LOLLIPOP)
public class UnitTest extends AppCompatActivity{
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

    public class MyCustomAdapter extends ArrayAdapter<List> {

        private ArrayList<List> masterList;

        public MyCustomAdapter(Context context, int textViewResourceId,
                               ArrayList<List> listOfLists) {
            super(context, textViewResourceId, listOfLists);
            this.masterList = new ArrayList<>();
            this.masterList.addAll(listOfLists);
        }

        public class ViewHolder {
            TextView name;
            CheckBox trashCan;
        }
    }

    // Test ViewAllLists method in MainActivity comparing list size
    @Test
    public void testViewAllLists() {
        mainActivity.createList("AddTestList");
        ListView listView = (ListView) mainActivity.findViewById(R.id.listView);
        assertEquals(mainActivity.listOfLists.size(), listView.getAdapter().getCount());
    }

    // Test createList method in MainActivity
    @Test
    public void testCreateList() {
        int length = 0;

        mainActivity.createList("CreateList");
        assertTrue(mainActivity.listExists(mainActivity.listOfLists, "CreateList"));

        length = mainActivity.listOfLists.size();

        mainActivity.createList("CreateList");
        assertEquals(length, mainActivity.listOfLists.size());
    }

    /*
        //TODO: need to implement delete/rename/select list
        // Test deleteList method
        @Test
        public void testDeleteList(){
            mainActivity.createList("WillBeDeleted");
            Context context = mainActivity.getApplicationContext();
            MyCustomAdapter adapter = new MyCustomAdapter(context, R.id.textView, mainActivity.listOfLists);
            assertFalse(mainActivity.listExists(mainActivity.listOfLists, "CreateList"));
        }
    */
    // Test ViewAllItems method in ListActivity comparing list size
    @Test
    public void testViewAllItems() {
        ListView listView = (ListView) listActivity.findViewById(R.id.listView);
        assertEquals(listActivity.listOfItems.size(), listView.getAdapter().getCount());
    }


    // Test clearAllCheckOffs method in ListActivity
    @Test
    public void testClearAllCheckOffs() {
        ListView listView = (ListView) listActivity.findViewById(R.id.listView);
        listActivity.clearAllCheckOffs(listView);

        for (int i = 0; i < listView.getCount(); i++) {
            View v = listView.getChildAt(i);
            CheckBox checkBox = (CheckBox) v.findViewById(R.id.checkBox);
            assertEquals(checkBox.isChecked(), false);
        }
    }

    // Test addItem method in ListActivity
    @Test
    public void testAddItem() {
        fail("Not yet implemented");
    }

    // Test deleteItem method in ListActivity
    @Test
    public void testDeleteItem() {
        fail("Not yet implemented");
    }

    // Test handleAdd method in AddToDBActivity
    @Test
    public void testHandleAdd() {
        EditText itemType = (EditText) addToDBActivity.findViewById(R.id.itemInType);
        EditText itemName = (EditText) addToDBActivity.findViewById(R.id.itemInName);
        EditText itemUnit = (EditText) addToDBActivity.findViewById(R.id.itemInUnit);

        itemType.setText("itemType");
        addToDBActivity.findViewById(R.id.button).performClick();
        ShadowLooper.idleMainLooper(10);
        assertEquals(ShadowToast.getTextOfLatestToast().toString(), "Item type, name and unit are required");

        itemType.setText("itemType");
        itemName.setText("itemName");
        addToDBActivity.findViewById(R.id.button).performClick();
        ShadowLooper.idleMainLooper(10);
        assertEquals(ShadowToast.getTextOfLatestToast().toString(), "Item type, name and unit are required");

        itemType.setText("itemType");
        itemName.setText("itemName");
        itemUnit.setText("itemUnit");
        addToDBActivity.findViewById(R.id.button).performClick();
        ShadowLooper.idleMainLooper(10);
        assertEquals(ShadowToast.getTextOfLatestToast().toString(), "Item is added!");
    }

    // Test handleQuery method in AddToDBActivity
    @Test
    public void testHandleQuery() {
        DBHelper db = new DBHelper(addToDBActivity.getApplicationContext());

        db.getWritableDatabase();
        ArrayList<Items> results = db.displayAllItems();
        Context context = addToDBActivity.getApplicationContext();
        String sqlResults = "";

        for (int i = 0; i < results.size(); i++) {
            sqlResults = sqlResults + results.get(i).getItemName() + " / "
                    + results.get(i).getItemType() + " / "
                    + results.get(i).getItemUnit() + " /// ";
        }

        addToDBActivity.findViewById(R.id.button2).performClick();
        ShadowLooper.idleMainLooper(10);
        assertEquals(ShadowToast.getTextOfLatestToast().toString(), sqlResults);
    }

    /*
        // Test handleSaveList method in AddToDBActivity
        @Test
        public void testHandleSaveList() {
            EditText itemName = (EditText) addToDBActivity.findViewById(R.id.searchItemName);

            addToDBActivity.findViewById(R.id.button3).performClick();
            ShadowLooper.idleMainLooper(10);
            assertEquals(ShadowToast.getTextOfLatestToast().toString(), "Item name is required for saving");

            itemName.setText("newItemName");
            addToDBActivity.findViewById(R.id.button3).performClick();
            ShadowLooper.idleMainLooper(10);
            assertEquals(ShadowToast.getTextOfLatestToast().toString(), "Running initial set up");
        }
    */
    // Test handleRetrieveList method in AddToDBActivity
    @Test
    public void testHandleRetrieveList() {
        Context context = addItemActivity.getApplicationContext();
        ListHelper lp = new ListHelper();
        MasterList mList = lp.getList(context);
        CharSequence text = "";
        if (mList.getListCount() >= 0) {
            text = "MasterList has " + mList.getListCount() + " list(s)";
        }
        addToDBActivity.findViewById(R.id.button5).performClick();
        ShadowLooper.idleMainLooper(10);
        assertEquals(ShadowToast.getTextOfLatestToast().toString(), text);
    }

    // Test handleSearch method in AddToDBActivity
    @Test
    public void testHandleSearch() {
        EditText searchItemName = (EditText) addToDBActivity.findViewById(R.id.searchItemName);
        EditText itemType = (EditText) addToDBActivity.findViewById(R.id.itemInType);
        EditText itemName = (EditText) addToDBActivity.findViewById(R.id.itemInName);
        EditText itemUnit = (EditText) addToDBActivity.findViewById(R.id.itemInUnit);

        itemType.setText("itemType");
        itemName.setText("SearchByItemName");
        itemUnit.setText("itemUnit");
        addToDBActivity.findViewById(R.id.button).performClick();

        addToDBActivity.findViewById(R.id.button4).performClick();
        ShadowLooper.idleMainLooper(10);
        assertEquals(ShadowToast.getTextOfLatestToast().toString(), "Item name is required");

        DBHelper db = new DBHelper(addToDBActivity.getApplicationContext());
        db.getReadableDatabase();
        ArrayList<Items> results = db.searchItem(itemName.getText().toString());
        String sqlResults = "";
        for (int i = 0; i < results.size(); i++) {
            sqlResults = sqlResults + results.get(i).getItemName() + " / "
                    + results.get(i).getItemType() + " / "
                    + results.get(i).getItemUnit() + " /// ";
        }
        searchItemName.setText("SearchByItemName");
        addToDBActivity.findViewById(R.id.button4).performClick();
        ShadowLooper.idleMainLooper(10);
        assertEquals(ShadowToast.getTextOfLatestToast().toString(), sqlResults);
    }

    // Test onClickAddItemToList method in AddItemActivity
    @Test
    public void testOnClickAddItemToList() {
        EditText itemName = (EditText) addItemActivity.findViewById(R.id.itemNameEditText);
        EditText itemType = (EditText) addItemActivity.findViewById(R.id.itemTypeEditText);
        EditText itemUnit = (EditText) addItemActivity.findViewById(R.id.itemUnitEditText);
        EditText itemQty = (EditText) addItemActivity.findViewById(R.id.itemQuantityEditText);

        itemName.setText("setItemName");
        itemType.setText("setItemType");
        itemUnit.setText("setItemUnit");
        addItemActivity.findViewById(R.id.addItemToListButton).performClick();
        ShadowLooper.idleMainLooper(10);
        assertEquals(ShadowToast.getTextOfLatestToast().toString(), "Item type, name, quantity and unit are required");

        itemQty.setText("setItemQty");
        addItemActivity.findViewById(R.id.addItemToListButton).performClick();
        ShadowLooper.idleMainLooper(10);
        assertEquals(ShadowToast.getTextOfLatestToast().toString(), "Item is added!");
    }

    // Test onClickCancelAddItemToList method in AddItemActivity
    @Test
    public void testOnClickCancelAddItemToList() {
        Intent intent = new Intent(addItemActivity, ListItemActivity.class);
        assertNotNull(intent.getComponent());
    }

    // Test onClickCheckDatabase method in AddItemActivity
    @Test
    public void testOnClickCheckDatabase() {
        Intent intent = new Intent(addItemActivity, AddToDBActivity.class);
        assertNotNull(intent.getComponent());
    }

    // Test viewAllItems method in ListItemActivity
    @Test
    public void testViewAllItemsInListItemActivity() {
        listItemActivity.viewAllItems();
        assertFalse(listItemActivity.adapter.isEmpty());
    }

        // Test onClickSearchButton method in ListItemActivity
    @Test
    public void testOnClickSearchButton() {
        listItemActivity.findViewById(R.id.searchButton).performClick();
        ShadowLooper.idleMainLooper(10);
        assertEquals(ShadowToast.getTextOfLatestToast().toString(), "pressed Search Button");
    }

    // Test onClickAddToDBButton method in ListItemActivity
    @Test
    public void testOnClickAddToDBButton() {
        Intent intent = new Intent(listItemActivity, AddItemActivity.class);
        assertNotNull(intent.getComponent());
    }
}

