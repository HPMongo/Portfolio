package edu.gatech.seclass.glm;

import android.content.Context;
import android.database.Cursor;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.ThemedSpinnerAdapter;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;
import android.util.Log;
import edu.gatech.seclass.glm.db.DBHelper;
import edu.gatech.seclass.glm.db.DBContract.Item;
import edu.gatech.seclass.glm.ListHelper.*;
import edu.gatech.seclass.glm.Items;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteConstraintException;
import edu.gatech.seclass.glm.InitialSetUp;
import java.util.ArrayList;


public class AddToDBActivity extends AppCompatActivity{
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.add_item_to_db);
    }
    //Main method to add the new item to the database
    public void handleAdd(View view) {
        EditText itemType = (EditText) findViewById(R.id.itemInType);
        EditText itemName = (EditText) findViewById(R.id.itemInName);
        EditText itemUnit = (EditText) findViewById(R.id.itemInUnit);
        if(itemName.getText().toString().isEmpty() || itemType.getText().toString().isEmpty() ||
                itemUnit.getText().toString().isEmpty()) {
            Context context = getApplicationContext();
            CharSequence text = "Item type, name and unit are required";
            int duration = Toast.LENGTH_SHORT;
            Toast toast = Toast.makeText(context, text, duration);
            toast.show();
        } else {
            DBHelper db = new DBHelper(getApplicationContext());
            try{
                db.getWritableDatabase();
                db.addItem(itemName.getText().toString(), itemType.getText().toString(), itemUnit.getText().toString());
                Context context = getApplicationContext();
                CharSequence text = "Item is added!";
                int duration = Toast.LENGTH_SHORT;
                Toast toast = Toast.makeText(context, text, duration);
                toast.show();            }
            catch(SQLiteConstraintException e) {
                //TO DO - still need to implement error handler
                System.out.println("Error from db - code: " + e);
            }
            finally {
                if(db != null) {
                    db.close();
                }
            }
        }
    }

    //Main method to query the database
    public void handleQuery(View view) {
        DBHelper db = new DBHelper(getApplicationContext());
        try{
            db.getWritableDatabase();
            ArrayList<Items> results = db.displayAllItems();
            Context context = getApplicationContext();
            String sqlResults = "";
            for (int i = 0; i < results.size(); i++) {
                sqlResults = sqlResults + results.get(i).getItemName() + " / "
                        + results.get(i).getItemType() + " / "
                        + results.get(i).getItemUnit() + " /// ";
            }
            //CharSequence text = results.toString();
            CharSequence text = sqlResults;
            int duration = Toast.LENGTH_SHORT;
            Toast toast = Toast.makeText(context, text, duration);
            toast.show();            }
        catch(SQLiteConstraintException e) {
            //TO DO - still need to implement error handler
            System.out.println("Error from db - code: " + e);
        }
        finally {
            if(db != null) {
                db.close();
            }
        }
    }

    //Main method to save the list
    public void handleSaveList(View view) {
        EditText itemName = (EditText) findViewById(R.id.searchItemName);
        if(itemName.getText().toString().isEmpty()) {
            Context context = getApplicationContext();
            CharSequence text = "Item name is required for saving";
            int duration = Toast.LENGTH_SHORT;
            Toast toast = Toast.makeText(context, text, duration);
            toast.show();
        } else {
            Context context = getApplicationContext();
            ListHelper lp = new ListHelper();
            InitialSetUp iSetUp = new InitialSetUp();
            String checkSetUp = lp.getSetUp(context);
            CharSequence text;
            if(checkSetUp != null && !checkSetUp.isEmpty() && checkSetUp.equals("SetUpCompleted")){
                text = "Set up has been completed";
            } else {
                text = "Running initial set up";
                iSetUp.runInitialSetUp(context);
            }
            int duration = Toast.LENGTH_SHORT;
            Toast toast = Toast.makeText(context, text, duration);
            toast.show();
        }
    }

    //Main method to retrieve the list
    public void handleRetrieveList(View view) {
            Context context = getApplicationContext();
            ListHelper lp = new ListHelper();
            MasterList mList = lp.getList(context);
            CharSequence text = "MasterList has " + mList.getListCount() + " list(s)";
            int duration = Toast.LENGTH_SHORT;
            Toast toast = Toast.makeText(context, text, duration);
            toast.show();
    }

    //Main method to query the database
    public void handleSearch(View view) {
        EditText itemName = (EditText) findViewById(R.id.searchItemName);
        if(itemName.getText().toString().isEmpty()) {
            Context context = getApplicationContext();
            CharSequence text = "Item name is required";
            int duration = Toast.LENGTH_SHORT;
            Toast toast = Toast.makeText(context, text, duration);
            toast.show();
        } else {
            DBHelper db = new DBHelper(getApplicationContext());
            try{
                db.getReadableDatabase();
                ArrayList<Items> results = db.searchItem(itemName.getText().toString());
                Context context = getApplicationContext();
                String sqlResults = "";
                for (int i = 0; i < results.size(); i++) {
                    sqlResults = sqlResults + results.get(i).getItemName() + " / "
                            + results.get(i).getItemType() + " / "
                            + results.get(i).getItemUnit() + " /// ";
                }
                //CharSequence text = results.toString();
                CharSequence text = sqlResults;
                int duration = Toast.LENGTH_SHORT;
                Toast toast = Toast.makeText(context, text, duration);
                toast.show();
            }
            catch(SQLiteConstraintException e) {
                //TO DO - still need to implement error handler
                System.out.println("Error from db - code: " + e);
            }
            finally {
                if(db != null) {
                    db.close();
                }
            }
        }
    }
}
