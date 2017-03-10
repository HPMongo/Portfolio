package edu.gatech.seclass.glm;

import android.content.Context;
import android.content.Intent;
import android.database.sqlite.SQLiteConstraintException;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.Spinner;

import java.util.ArrayList;
import java.util.Arrays;

import edu.gatech.seclass.glm.db.DBHelper;

public class ListItemActivity extends AppCompatActivity {

    String listSelected;

    // Create alias for adapter
    MyCustomAdapter adapter = null;

    // Initialize the array list that will contain our lists
    ArrayList<Items> listOfItems = new ArrayList<>();

    // Initialize the array list that will contain our list of types
    ArrayList<String> listOfTypes = new ArrayList<>();

    // Initialize spinner
    Spinner spinner;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_list_item);

        // Allow transition from main activity screen
        Intent intent = getIntent();
        listSelected = intent.getStringExtra("listSelected");

        // Find all lists and display them
        viewAllItems();

        /*
        // Throw a toast to give confirmation of the list chosen
        Context context = getApplicationContext();
        CharSequence text = "List selected: ";
        int duration = Toast.LENGTH_SHORT;
        Toast toast = Toast.makeText(context, text + listSelected, duration);
        toast.show();
        */

        //Populate Spinner
        spinner = (Spinner) findViewById(R.id.itemTypeSpinner);

        // "Listen" for clicks in our spinner list
        spinnerItemSelected();

        // Load the spinner with current existing item types
        loadSpinnerData();
    }

    // "Listen" for an item selection in our spinner
    public void spinnerItemSelected() {

        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                String itemTypeSelected = parent.getItemAtPosition(position).toString();

                if (itemTypeSelected != "*") {
                    // Filter out items of different types from that selected in the spinner
                    onClickSearchItemByTypeButton(itemTypeSelected);

                    // Repopulate listview with only items of the item type selected from spinner
                    viewAllItems();
                }
            }

            public void onNothingSelected(AdapterView<?> parent) {
                // Do nothing
            }
        });
    }

    //OnClick callback method for Search Button by Type
    public void onClickSearchItemByTypeButton(String inType) {

        DBHelper db = new DBHelper(getApplicationContext());
        try {
            db.getReadableDatabase();
            //Clean out the listOfItems
            listOfItems.clear();
            //Populate sql result back to listOfItems
            listOfItems = db.searchItemByType(inType);
            //Check result
            if (listOfItems.size() < 1) {
                Context context = getApplicationContext();
                CharSequence text = "No item matches that type";
                int duration = Toast.LENGTH_SHORT;
                Toast toast = Toast.makeText(context, text, duration);
                toast.show();
            }
        } catch (SQLiteConstraintException e) {
            //TO DO - still need to implement error handler
            System.out.println("Error from db - code: " + e);
        } finally {
            if (db != null) {
                db.close();
            }
        }

    }

    //TODO: Fix the below list populating code.
    // Find all pre-existing items in the selected list and return them
    //@Override
    public void viewAllItems() {
        // Create an adapter which will convert each list into a view item
        adapter = new ListItemActivity.MyCustomAdapter(this, R.layout.list_item_rows, listOfItems);

        // Put each item inside our list view by mapping the adapter to our list view
        ListView listView = (ListView) findViewById(R.id.itemListView);
        listView.setAdapter(adapter);
    }

    public class MyCustomAdapter extends ArrayAdapter<Items> {

        private ArrayList<Items> listOfItems;

        private MyCustomAdapter(Context context, int textViewResourceId,
                                ArrayList<Items> listOfItems) {
            super(context, textViewResourceId, listOfItems);
            this.listOfItems = new ArrayList<>();
            this.listOfItems.addAll(listOfItems);
        }

        public class ViewHolder {
            TextView type;
            TextView unit;
            TextView name;
        }

        public View getView(int position, View convertView, ViewGroup parent) {

            ListItemActivity.MyCustomAdapter.ViewHolder holder;
            //Log.v("ConvertView", String.valueOf(position));

            if (convertView == null) {
                LayoutInflater vi = (LayoutInflater) getSystemService(
                        Context.LAYOUT_INFLATER_SERVICE);
                convertView = vi.inflate(R.layout.list_item_rows, null);

                holder = new ListItemActivity.MyCustomAdapter.ViewHolder();
                holder.type = (TextView) convertView.findViewById(R.id.itemTypeTextView);
                holder.unit = (TextView) convertView.findViewById(R.id.unitTextView);
                holder.name = (TextView) convertView.findViewById(R.id.itemNameTextView);
                convertView.setTag(holder);

            } else {
                holder = (ListItemActivity.MyCustomAdapter.ViewHolder) convertView.getTag();
            }

            Items item = listOfItems.get(position);
            holder.type.setText(item.getItemType());
            holder.type.setTag(item);
            holder.name.setText(item.getItemName());
            holder.name.setTag(item);
            holder.unit.setText(item.getItemUnit());
            holder.unit.setTag(item);

            // Listen if an item in our listView was clicked
            onClickAddToDBButton(holder);

            return convertView;
        }
    }

    // Find all item types in database and populate them in our spinner
    public void loadSpinnerData() {

        DBHelper db = new DBHelper(getApplicationContext());
        ArrayList<String> temp = new ArrayList<>(Arrays.asList("*"));
        ArrayList<String> listTypes = new ArrayList<>();
        listTypes.addAll(temp);
        listTypes.addAll(db.displayAllTypes());

        ArrayAdapter<String> dataAdapter = new ArrayAdapter<>
                (this, R.layout.support_simple_spinner_dropdown_item, listTypes);

        spinner.setAdapter(dataAdapter);

    }

    //OnClick callback method for Search Button
    public void onClickSearchButton(View view) {
        EditText itemName = (EditText) findViewById(R.id.searchForItemByNameEditTExt);
        if (itemName.getText().toString().isEmpty()) {
            Context context = getApplicationContext();
            CharSequence text = "Item name is required";
            int duration = Toast.LENGTH_SHORT;
            Toast toast = Toast.makeText(context, text, duration);
            toast.show();
        } else {
            DBHelper db = new DBHelper(getApplicationContext());
            try {
                db.getReadableDatabase();
                //Clean out the listOfItems
                listOfItems.clear();
                //Populate sql result back to listOfItems
                listOfItems = db.searchItem(itemName.getText().toString());
                //Check result
                if (listOfItems.size() < 1) {
                    Context context = getApplicationContext();
                    CharSequence text = "No item matches that name";
                    int duration = Toast.LENGTH_SHORT;
                    Toast toast = Toast.makeText(context, text, duration);
                    toast.show();
                } else {
                    // Find all lists and display them
                    viewAllItems();
                }
            } catch (SQLiteConstraintException e) {
                //TO DO - still need to implement error handler
                System.out.println("Error from db - code: " + e);
            } finally {
                if (db != null) {
                    db.close();
                }
            }
        }
    }


    //OnClick callback method for Display Button
    public void onClickDisplayAllButton(View view) {
        DBHelper db = new DBHelper(getApplicationContext());
        try {
            db.getReadableDatabase();
            //Clean out the listOfItems
            listOfItems.clear();
            //Populate sql result back to listOfItems
            listOfItems = db.displayAllItems();
            //Check result
            if (listOfItems.size() < 1) {
                Context context = getApplicationContext();
                CharSequence text = "It looks like we don't have any item, yet. Please add a new item to our database.";
                int duration = Toast.LENGTH_SHORT;
                Toast toast = Toast.makeText(context, text, duration);
                toast.show();
            } else {
                // Find all lists and display them
                viewAllItems();
            }
        } catch (SQLiteConstraintException e) {
            //TO DO - still need to implement error handler
            System.out.println("Error from db - code: " + e);
        } finally {
            if (db != null) {
                db.close();
            }
        }
    }

    //OnClick callback method for Display All Types
    public void onClickDisplayAllTypesButton(View view) {
        DBHelper db = new DBHelper(getApplicationContext());
        try {
            db.getReadableDatabase();
            //Clean out the listOfItems
            listOfTypes.clear();
            //Populate sql result back to listOfItems
            listOfTypes = db.displayAllTypes();
            //Check result
            if (listOfTypes.size() < 1) {
                Context context = getApplicationContext();
                CharSequence text = "It looks like we don't have any type, yet. Please add a new item to our database.";
                int duration = Toast.LENGTH_SHORT;
                Toast toast = Toast.makeText(context, text, duration);
                toast.show();
            }
        } catch (SQLiteConstraintException e) {
            //TO DO - still need to implement error handler
            System.out.println("Error from db - code: " + e);
        } finally {
            if (db != null) {
                db.close();
            }
        }
    }
    //OnClick callback method for Add to DB Button
    public void onClickAddToDBButton(View view)
    {
        switch (view.getId()){

            case R.id.addToDBButton:

                addItemToDB();

                break;
        }
    }

    //OnClick callback method for Add to DB Button
    public void addItemToDB() {

        //Jump to AddItemActivity and pass current listSelected with intent
        Intent intent = new Intent(ListItemActivity.this, AddItemActivity.class);
        intent.putExtra("listSelected", listSelected + ";-;-;-");
        startActivity(intent);
    }

    //OnClick callback method for Add to DB Button
    public void onClickAddToDBButton(MyCustomAdapter.ViewHolder holder) {

        // "Listens" for clicks on our list of item names
        holder.name.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                TextView tv = (TextView) v;

                Items item = (Items) tv.getTag();
                String itemName = item.getItemName();
                String itemType = item.getItemType();
                String itemUnit = item.getItemUnit();

                itemName = (itemName == null) ? "-" : itemName;
                itemType = (itemType == null) ? "-" : itemType;
                itemUnit = (itemUnit == null) ? "-" : itemUnit;

                //Jump to AddItemActivity and pass current listSelected with intent
                Intent intent = new Intent(ListItemActivity.this, AddItemActivity.class);
                intent.putExtra("listSelected", listSelected + ";" + itemName + ";" + itemType + ";" + itemUnit);
                startActivity(intent);

            }
        });

        // "Listens" for clicks on our list of item types
        holder.type.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                TextView tv = (TextView) v;

                Items item = (Items) tv.getTag();
                String itemName = item.getItemName();
                String itemType = item.getItemType();
                String itemUnit = item.getItemUnit();

                itemName = (itemName == null) ? "-" : itemName;
                itemType = (itemType == null) ? "-" : itemType;
                itemUnit = (itemUnit == null) ? "-" : itemUnit;

                //Jump to AddItemActivity and pass current listSelected with intent
                Intent intent = new Intent(ListItemActivity.this, AddItemActivity.class);
                intent.putExtra("listSelected", listSelected + ";" + itemName + ";" + itemType + ";" + itemUnit);
                startActivity(intent);

            }
        });

        // "Listens" for clicks on our list of item units
        holder.unit.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                TextView tv = (TextView) v;

                Items item = (Items) tv.getTag();
                String itemName = item.getItemName();
                String itemType = item.getItemType();
                String itemUnit = item.getItemUnit();

                itemName = (itemName == null) ? "-" : itemName;
                itemType = (itemType == null) ? "-" : itemType;
                itemUnit = (itemUnit == null) ? "-" : itemUnit;

                //Jump to AddItemActivity and pass current listSelected with intent
                Intent intent = new Intent(ListItemActivity.this, AddItemActivity.class);
                intent.putExtra("listSelected", listSelected + ";" + itemName + ";" + itemType + ";" + itemUnit);
                startActivity(intent);

            }
        });
    }
}
