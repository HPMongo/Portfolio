package edu.gatech.seclass.glm;

import android.content.Context;
import android.content.Intent;
import android.database.sqlite.SQLiteConstraintException;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import java.util.ArrayList;

import java.util.Arrays;

import edu.gatech.seclass.glm.db.DBHelper;
import edu.gatech.seclass.glm.List;
import edu.gatech.seclass.glm.MasterList;
import edu.gatech.seclass.glm.ListHelper;

public class AddItemActivity extends AppCompatActivity {

    private String listName;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_item);

        // Allow transition from previous activity screen
        Intent intent = getIntent();
        //String inList = intent.getStringExtra("listSelected");
        //List listSelected = new List(inList);

        //Get arguments from the prior screen
        String inText = intent.getStringExtra("listSelected");

        //String inText = "newList;kiwi;fruit;each";
        String[] stringArray = inText.split(";");

        //Get edit text field name
        EditText itemName = (EditText) findViewById(R.id.itemNameEditText);
        EditText itemType = (EditText) findViewById(R.id.itemTypeEditText);
        EditText itemUnit = (EditText) findViewById(R.id.itemUnitEditText);
        EditText itemQty = (EditText) findViewById(R.id.itemQuantityEditText);

        //Set listName
        listName = stringArray[0];

        // Populate edit texts using prior screen selection if the inputs are not
        // the default values "-"
        if(!stringArray[1].equals("-")){
            itemName.setText(stringArray[1]);
        }

        if(!stringArray[2].equals("-")){
            itemType.setText(stringArray[2]);
        }

        if(!stringArray[3].equals("-")){
            itemUnit.setText(stringArray[3]);
        }

        /*
        itemName.setText(stringArray[1]);
        itemType.setText(stringArray[2]);
        itemUnit.setText(stringArray[3]);
        */

        //itemQty.setText("0.0");

    }

    //OnClick callback method for Add Item to List Button
    public void onClickAddItemToList(View view) {

        //Retrieve list object from sharedpreferences
        Context context = getApplicationContext();
        ListHelper lp = new ListHelper();
        MasterList mList = lp.getList(context);

        //Search for input list
        int listIndex = mList.findListIndex(listName);

        if(listIndex < 0) {
            CharSequence text = "O oh, it looks like you have found a bug! We'll be working this bug shortly.";
            int duration = Toast.LENGTH_SHORT;
            Toast toast = Toast.makeText(context, text, duration);
            toast.show();
            return;
        }


        //List listSelected = new List(inList);

        // Populate edit texts from prior selection
        EditText itemName = (EditText) findViewById(R.id.itemNameEditText);
        EditText itemType = (EditText) findViewById(R.id.itemTypeEditText);
        EditText itemUnit = (EditText) findViewById(R.id.itemUnitEditText);
        EditText itemQty = (EditText) findViewById(R.id.itemQuantityEditText);

        //TODO: Insert code to Add Item to DB based on Arguments in edit boxes

        if(itemName.getText().toString().isEmpty() || itemType.getText().toString().isEmpty() ||
                itemUnit.getText().toString().isEmpty() || itemQty.getText().toString().isEmpty()) {
            CharSequence text = "Item type, name, quantity and unit are required";
            int duration = Toast.LENGTH_SHORT;
            Toast toast = Toast.makeText(context, text, duration);
            toast.show();
        } else {
            DBHelper db = new DBHelper(getApplicationContext());

            //Convert to variables
            String inName = itemName.getText().toString();
            String inType = itemType.getText().toString();
            String inUnit = itemUnit.getText().toString();
            String strQty = itemQty.getText().toString();
            double inQty = 0.0;
            //Convert string to double
            try
            {
                inQty = Double.parseDouble(strQty);
            }
            catch (NumberFormatException e)
            {
                inQty = 0.0;
            }

            //Add item to database
            try{
                db.getWritableDatabase();
                //Adding item to the database
                db.addItem(inName, inType, inUnit);
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

            //Search to see if the item already exists
            int itemIndex = mList.getList(listIndex).findItemIndex(inName);

            //Item exists
            if(itemIndex >= 0) {
                //Display a message
                CharSequence text = "Item already exists on the list! Please update the item accordingly.";
                int duration = Toast.LENGTH_SHORT;
                Toast toast = Toast.makeText(context, text, duration);
                toast.show();
            } else {
                //Item does not exist
                //Adding item to the current list
                Items nItem = new Items(inName,inType, inUnit, inQty, false);

                //listSelected.addItemToList(nItem);
                mList.addItemToList(listIndex, nItem);

                //Context context = getApplicationContext();

                //Save list back to shared preferences
                lp.saveList(mList, context);

                //Display a message
                CharSequence text = "Item is added!";
                int duration = Toast.LENGTH_SHORT;
                Toast toast = Toast.makeText(context, text, duration);
                toast.show();
            }

            // Go back to ListActivity
            goToListActivity();
        }

    }

    // Go back to ListActivity
    public void goToListActivity(){
        Intent intent = new Intent(AddItemActivity.this, ListActivity.class);
        intent.putExtra("listSelected", listName);
        startActivity(intent);
    }

}
