package edu.gatech.seclass.glm.db;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Hashtable;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.DatabaseUtils;
import android.database.sqlite.SQLiteOpenHelper;
import android.database.sqlite.SQLiteDatabase;
import edu.gatech.seclass.glm.db.DBContract.Item;
import edu.gatech.seclass.glm.Items;


public class DBHelper extends SQLiteOpenHelper {

    public static final String DATABASE_NAME = "item.db";
    private static final int DATABASE_VERSION = 2;

    public DBHelper(Context context)
    {
        super(context, DATABASE_NAME , null, 1);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        final String SQL_CREATE_ITEM_TABLE = "CREATE TABLE " + Item.TABLE_NAME + " (" +
                Item.COLUMN_ITEM_NAME + " TEXT NOT NULL, " +
                Item.COLUMN_ITEM_TYPE + " TEXT NOT NULL, " +
                Item.COLUMN_ITEM_UNIT + " TEXT NOT NULL, " +
                "PRIMARY KEY (" + Item.COLUMN_ITEM_NAME + ", " + Item.COLUMN_ITEM_TYPE + "));";

        db.execSQL(SQL_CREATE_ITEM_TABLE);
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL("DROP TABLE IF EXISTS" + Item.TABLE_NAME);
        onCreate(db);
    }

    public void onDowngrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        onUpgrade(db, oldVersion, newVersion);
    }

    public boolean addItem (String inName, String inType, String inUnit)
    {
        SQLiteDatabase db = this.getWritableDatabase();
        ContentValues contentValues = new ContentValues();
        contentValues.put(Item.COLUMN_ITEM_NAME, inName);
        contentValues.put(Item.COLUMN_ITEM_TYPE, inType);
        contentValues.put(Item.COLUMN_ITEM_UNIT, inUnit);
        db.insertOrThrow(Item.TABLE_NAME, null, contentValues);
        return true;
    }

    public ArrayList<Items> displayAllItems()
    {
        ArrayList<Items> array_list = new ArrayList<Items>();
        Items record = null;
        Cursor res = null;

        SQLiteDatabase db = this.getReadableDatabase();
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
        return array_list;
    }

    public ArrayList<String> displayAllTypes()
    {
        ArrayList<String> array_list = new ArrayList<String>();
        Items record = null;
        Cursor res = null;

        SQLiteDatabase db = this.getReadableDatabase();
        res =  db.rawQuery( "select distinct " + Item.COLUMN_ITEM_TYPE + " from items order by "
                + Item.COLUMN_ITEM_TYPE + " asc;", null );
        res.moveToFirst();

        while(!res.isAfterLast()){
            array_list.add(res.getString(res.getColumnIndex(Item.COLUMN_ITEM_TYPE)));
            res.moveToNext();
        }
        return array_list;
    }

    public ArrayList<Items> searchItem(String inName){
            ArrayList<Items> array_list = new ArrayList<Items>();
            Items record = null;
            Cursor res = null;
            SQLiteDatabase db = this.getReadableDatabase();

            res =  db.rawQuery( "select * from items where item_name like '%" + inName + "%' order by "
                    + Item.COLUMN_ITEM_TYPE + " asc, " + Item.COLUMN_ITEM_NAME + " asc;", null );
            res.moveToFirst();

            while(!res.isAfterLast()){
                //         array_list.add(res.getString(res.getColumnIndex(Item.COLUMN_ITEM_NAME)));
                record = new Items();
                record.setItemName(res.getString(res.getColumnIndex(Item.COLUMN_ITEM_NAME)));
                record.setItemType(res.getString(res.getColumnIndex(Item.COLUMN_ITEM_TYPE)));
                record.setItemUnit(res.getString(res.getColumnIndex(Item.COLUMN_ITEM_UNIT)));
                array_list.add(record);
                res.moveToNext();
            }
            return array_list;
    }

    public ArrayList<Items> searchItemByType(String inType){
        ArrayList<Items> array_list = new ArrayList<Items>();
        Items record = null;
        Cursor res = null;
        SQLiteDatabase db = this.getReadableDatabase();

        res = db.rawQuery("select * from items where item_type like '%" + inType + "%' order by "
                + Item.COLUMN_ITEM_TYPE + " asc, " + Item.COLUMN_ITEM_NAME + " asc;", null);
        res.moveToFirst();

        while (!res.isAfterLast()) {
            record = new Items();
            record.setItemName(res.getString(res.getColumnIndex(Item.COLUMN_ITEM_NAME)));
            record.setItemType(res.getString(res.getColumnIndex(Item.COLUMN_ITEM_TYPE)));
            record.setItemUnit(res.getString(res.getColumnIndex(Item.COLUMN_ITEM_UNIT)));
            array_list.add(record);
            res.moveToNext();
        }
        return array_list;
    }
}