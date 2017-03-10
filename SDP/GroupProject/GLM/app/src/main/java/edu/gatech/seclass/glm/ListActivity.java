package edu.gatech.seclass.glm;

import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

public class ListActivity extends AppCompatActivity implements ListInterface{

    // Create alias for adapter
    ListActivity.MyCustomAdapter adapter = null;

    // Initialize the array list that will contain our lists
    ArrayList<Items> listOfItems = new ArrayList<>();
    MasterList mList;
    ListHelper lp;
    List currentList;
    String listSelected;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_list);

        // Allow transition from main activity screen
        Intent intent = getIntent();
        listSelected = intent.getStringExtra("listSelected");

        // Throw a toast to give confirmation of the list chosen
        //Context context = getApplicationContext();
        //CharSequence text = "List selected: ";
        //int duration = Toast.LENGTH_SHORT;
        //Toast toast = Toast.makeText(context, listSelected, duration);
        //toast.show();

        //ListView listView = (ListView) findViewById(R.id.listView);

        // Test Data
        //Items item0 = new Items("Donut", "Sweets", "each", 12.0, true);
        //listOfItems.add(item0);
        //Items item4 = new Items("Carrots", "Vegetables", "each", 6.0, true);
        //listOfItems.add(item4);
        //Items item1 = new Items("Milk", "Dairy", "gallon", 1.0, false);
        //listOfItems.add(item1);
        //Items item3 = new Items("Bread", "Grain", "loaf", 2.0, false);
        //listOfItems.add(item3);
        //Items item2 = new Items("Cheese", "Dairy", "pat", 1.0, true);
        //listOfItems.add(item2);

        // Convert our Master List object into ArrayList of our items to use with the adapter
        Context context = getApplicationContext();
        mList = lp.getList(context);
        int listIndex = mList.findListIndex(listSelected);
        currentList = mList.getList(listIndex);
        listOfItems = currentList.getItemsList();

        // Find all lists and display them
        viewAllItems();

    }

    // Find all pre-existing items in the selected list and return them
    @Override
    public void viewAllItems()
    {
        // Sort items by their type
        sort(listOfItems);

        // Create an adapter which will convert each list into a view item
        adapter = new ListActivity.MyCustomAdapter(this, R.layout.item_rows, listOfItems);

        // Put each item inside our list view by mapping the adapter to our list view
        ListView listView = (ListView) findViewById(R.id.listView);
        listView.setAdapter(adapter);
    }

    // Sort our items by their type to make shopping easier for the customer
    @Override
    public void sort(ArrayList<Items> listOfItems){
        Collections.sort(listOfItems, new Comparator<Items>(){
            public int compare(Items item1, Items item2) {
                return item1.getItemType().compareToIgnoreCase(item2.getItemType());
            }
        });
    }

    // Adapter that will take each individual item per row and translated into our list view
    // Code modified from that found at the following URL:
    // http://www.mysamplecode.com/2012/07/android-listview-checkbox-example.html
    public class MyCustomAdapter extends ArrayAdapter<Items> {

        private ArrayList<Items> listOfItems;

        private MyCustomAdapter(Context context, int textViewResourceId,
                                ArrayList<Items> listOfItems) {
            super(context, textViewResourceId, listOfItems);
            this.listOfItems = new ArrayList<>();
            this.listOfItems.addAll(listOfItems);
        }

        public class ViewHolder {
            CheckBox checkMark;
            EditText qty;
            TextView unit;
            TextView name;
            TextView type;
            CheckBox trashCan;
        }

        public View getView(int position, View convertView, ViewGroup parent) {

            ListActivity.MyCustomAdapter.ViewHolder holder;
            //Log.v("ConvertView", String.valueOf(position));

            if (convertView == null) {
                LayoutInflater vi = (LayoutInflater) getSystemService(
                        Context.LAYOUT_INFLATER_SERVICE);
                convertView = vi.inflate(R.layout.item_rows, null);

                holder = new ListActivity.MyCustomAdapter.ViewHolder();
                holder.checkMark = (CheckBox) convertView.findViewById(R.id.checkBox);
                holder.qty = (EditText) convertView.findViewById(R.id.qtyEditText);
                holder.unit = (TextView) convertView.findViewById(R.id.unitTextView);
                holder.name = (TextView) convertView.findViewById(R.id.itemNameTextView);
                holder.type = (TextView) convertView.findViewById(R.id.itemTypeTextView);
                holder.trashCan = (CheckBox) convertView.findViewById(R.id.deleteButton);
                convertView.setTag(holder);

            } else {
                holder = (ListActivity.MyCustomAdapter.ViewHolder) convertView.getTag();
            }

            Items item = listOfItems.get(position);
            holder.checkMark.setChecked(item.getCheckOff());
            holder.checkMark.setTag(item);
            holder.qty.setText(String.valueOf(item.getItemQty()));
            holder.qty.setTag(item);
            holder.unit.setText(item.getItemUnit());
            holder.unit.setTag(item);
            holder.name.setText(item.getItemName());
            holder.name.setTag(item);
            holder.type.setText(item.getItemType());
            holder.type.setTag(item);
            holder.trashCan.setChecked(false);
            holder.trashCan.setTag(item);


            // This will check if a trash can was clicked and should be removed
            deleteItem(holder);

            // This will check if a check mark for an item is changed
            checkMarkChange(holder);

            // This will check if an item's quantity was changed
            itemQtyChange(holder, convertView);

            return convertView;
        }
    }

    public void onButtonClick(View view)
    {
        // Initialize variable if user put text in text box
        //String strNewListName = newListName.getText().toString();

        ListView listView = (ListView) findViewById(R.id.listView);

        switch (view.getId()){

            case R.id.addItemButton:

                addItem();

            break;

            case R.id.uncheckItemsButton:

                clearAllCheckOffs(listView);

            break;
        }
    }

    @Override
    public void addItem() {
        // Jump to our list view and all items in it
        //TODO: For testing purpose, the add activity will go directly to the AddItemActivity
        // Once testing is done, this needs to change back to ListItemActivity



        Intent intent = new Intent(ListActivity.this, ListItemActivity.class);
        intent.putExtra("listSelected", listSelected);
        startActivity(intent);

    }

    // Remove all check marks from every item in the list
    @Override
    public void clearAllCheckOffs(ListView listView)
    {
        currentList.removeAllCheckOffs();
        Context context = getApplicationContext();
        lp.saveList(mList,context);
        // Re-populate the list view
        viewAllItems();

        /*Toast.makeText(getApplicationContext(),
                "All check marks cleared!",
                Toast.LENGTH_SHORT).show();*/

        /*for (int i = 0; i < listView.getCount(); i++) {
            View v = listView.getChildAt(i);

            CheckBox checkBox = (CheckBox) v.findViewById(R.id.checkBox);
            checkBox.setChecked(false);
        }*/
    }

    // Delete the item the user specifies
    @Override
    public void deleteItem(ListActivity.MyCustomAdapter.ViewHolder holder)
    {
        holder.trashCan.setOnClickListener( new View.OnClickListener() {
            public void onClick(View v) {
                CheckBox cb = (CheckBox) v;

                if (cb.isChecked()) {
                    TextView tv = (TextView) v;
                    Items item = (Items) tv.getTag();
                    //String itemName = item.getItemName();

                    //listOfItems.remove(items);

                    // Rename list with user provided name;
                    currentList.removeItemFromList(item);
                    Context context = getApplicationContext();
                    lp.saveList(mList,context);

                    // Re-populate the list view
                    viewAllItems();

                    // Send out confirmation to user of item delete
                    /*Toast.makeText(getApplicationContext(),
                            "Item deleted: " + itemName,
                            Toast.LENGTH_SHORT).show();*/
                }
            }
        });
    }

    // Save the JSON whenever a check mark changes
    public void itemQtyChange(ListActivity.MyCustomAdapter.ViewHolder holder, View convertView) {
        //Attaching the TextWatcher to our item quantity edit text
        holder.qty.addTextChangedListener(new MyTextWatcher(convertView));
    }

    // TextWatcher adapted from:
    // http://www.mysamplecode.com/2012/12/android-attach-textwatcher-to-multiple-edittext.html
    public class MyTextWatcher implements TextWatcher {

         private View view;

         private MyTextWatcher(View view) {
             this.view = view;
         }

         public void beforeTextChanged(CharSequence s, int start, int count, int after) {
             //do nothing
         }

         public void onTextChanged(CharSequence s, int start, int before, int count) {
             //do nothing
         }

         public void afterTextChanged(Editable s) {
            /*
             TextView tv = (TextView) view;
             Items item = (Items) tv.getTag();
             int itemIndex = currentList.findItemIndex(item.getItemName());
             double inQty = Double.parseDouble(tv.getText().toString());

             currentList.updateItemQtyFromList(itemIndex, inQty);

             Context context = getApplicationContext();
             lp.saveList(mList, context);
            */
         }
    }

    // Save the JSON whenever a check mark changes
    public void checkMarkChange(ListActivity.MyCustomAdapter.ViewHolder holder) {
        holder.checkMark.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                CheckBox cb = (CheckBox) v;

                TextView tv = (TextView) v;
                Items item = (Items) tv.getTag();
                int itemIndex = currentList.findItemIndex(item.getItemName());

                if (cb.isChecked()) {

                    currentList.addCheckOffItemFromList(itemIndex);

                }
                else {

                    currentList.removeItemCheckOffFromList(itemIndex);

                }

                Context context = getApplicationContext();
                lp.saveList(mList, context);

                // Re-populate the list view
                viewAllItems();

            }
        });
    }
}
