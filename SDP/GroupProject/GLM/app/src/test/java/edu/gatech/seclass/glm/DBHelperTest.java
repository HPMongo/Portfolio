package edu.gatech.seclass.glm;

import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.robolectric.RobolectricGradleTestRunner;
import org.robolectric.RuntimeEnvironment;
import org.robolectric.annotation.Config;

import static org.junit.Assert.*;
import static org.hamcrest.Matchers.hasItemInArray;

import edu.gatech.seclass.glm.db.DBHelper;
import edu.gatech.seclass.glm.db.DBContract.Item;

import org.junit.runners.MethodSorters;
import org.junit.FixMethodOrder;

import java.util.ArrayList;

@RunWith(RobolectricGradleTestRunner.class)
@Config(constants = BuildConfig.class, sdk = 21, manifest = "/src/main/AndroidManifest.xml")
@FixMethodOrder(MethodSorters.NAME_ASCENDING)
public class DBHelperTest {

    private Context context;
    private DBHelper helper;
    private SQLiteDatabase db;

    @Before
    public void setup(){
        context = RuntimeEnvironment.application;
        helper = new DBHelper(context);
        db = helper.getReadableDatabase();
    }

    @After
    public void cleanup(){
        db.close();
    }

    // Test DB is correctly created
    @Test
    public void testDBCreated(){
        DBHelper helper = new DBHelper(context);
        SQLiteDatabase db = helper.getWritableDatabase();
        assertTrue(db.isOpen());
        db.close();
    }

    // Test columns in DB
    @Test
    public void testDBCols() {
        Cursor cur = db.query(Item.TABLE_NAME, null, null, null, null, null, null);
        assertNotNull( cur );

        String[] cols = cur.getColumnNames();
        assertThat(Item.COLUMN_ITEM_NAME, cols, hasItemInArray(Item.COLUMN_ITEM_NAME));
        assertThat(Item.COLUMN_ITEM_TYPE, cols, hasItemInArray(Item.COLUMN_ITEM_TYPE));
        assertThat(Item.COLUMN_ITEM_UNIT, cols, hasItemInArray(Item.COLUMN_ITEM_UNIT));

        cur.close();
    }

    // Test addItem method
    @Test
    public void testAddItem(){
        assertTrue(helper.addItem("item1","type1","unit1"));
    }

    // Test searchItem method
    @Test
    public void testSearchItem(){
        helper.addItem("SearchedItem","type1","unit1");
        helper.addItem("itemForSearch2","type2","unit2");
        helper.addItem("itemForSearch3","type3","unit3");

        ArrayList<Items> array_list = new ArrayList<Items>();
        Items record = null;
        record = new Items();
        record.setItemName("SearchedItem");
        record.setItemType("type1");
        record.setItemUnit("unit1");
        array_list.add(record);

        assertEquals(array_list, helper.searchItem("SearchedItem"));
    }

    @Test
    public void testDisplayAllItems(){
        helper.addItem("testForDisplayAllItems1","type1","unit1");
        helper.addItem("testForDisplayAllItems2","type2","unit2");
        helper.addItem("testForDisplayAllItems3","type3","unit3");

        ArrayList<Items> array_list = new ArrayList<Items>();
        Items record = null;
        Cursor res = null;

        SQLiteDatabase db = helper.getReadableDatabase();
        res =  db.rawQuery( "select * from items order by " + Item.COLUMN_ITEM_TYPE + " asc, "
                + Item.COLUMN_ITEM_NAME + " asc;", null );
        res.moveToFirst();

        while(!res.isAfterLast()){
            record = new Items();
            record.setItemName(res.getString(res.getColumnIndex(Item.COLUMN_ITEM_NAME)));
            record.setItemType(res.getString(res.getColumnIndex(Item.COLUMN_ITEM_TYPE)));
            record.setItemUnit(res.getString(res.getColumnIndex(Item.COLUMN_ITEM_UNIT)));
            array_list.add(record);
            res.moveToNext();
        }

        assertEquals(array_list, helper.displayAllItems());
    }
}