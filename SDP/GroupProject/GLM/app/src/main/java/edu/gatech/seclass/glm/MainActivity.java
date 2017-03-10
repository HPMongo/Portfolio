package edu.gatech.seclass.glm;

import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import java.util.ArrayList;


public class MainActivity extends AppCompatActivity implements MainActivityInterface {

    // Create alias for adapter
    MyCustomAdapter adapter = null;

    // Initialize the array list that will contain our lists and the master list object
    ArrayList<List> listOfLists = new ArrayList<>();
    MasterList mList;
    ListHelper lp;
    EditText newListName;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        newListName = (EditText) findViewById(R.id.newListEditText);

        //Test data
        //List list1 = new List("foo");
        //listOfLists.add(list1);
        //List list2 = new List("bar");
        //listOfLists.add(list2);

        Context context = getApplicationContext();
        lp = new ListHelper();
        InitialSetUp iSetUp = new InitialSetUp();
        String checkSetUp = lp.getSetUp(context);
        CharSequence text;
        if(checkSetUp != null && !checkSetUp.isEmpty() && checkSetUp.equals("SetUpCompleted")){
            text = "Set up has been completed";
        } else {
            text = "Running initial set up";
            iSetUp.runInitialSetUp(context);
        }
        //int duration = Toast.LENGTH_SHORT;
        //Toast toast = Toast.makeText(context, text, duration);
        //toast.show();

        mList = lp.getList(context);

        // Convert our Master List object into Array List to use with the adapter
        listOfLists = mList.getMasterList();

        // Find all lists and display them
        viewAllLists();
    }

    // Find all pre-existing lists and return them
    @Override
    public void viewAllLists()
    {
        ListView listView = (ListView) findViewById(R.id.listView);

        // Create an adapter which will convert each list into a view item
        adapter = new MyCustomAdapter(this, R.layout.list_rows, listOfLists);

        // Put each item inside our list view by mapping the adapter to our list view
        listView.setAdapter(adapter);
    }

    // Code modified from that found at the following URL:
    // http://www.mysamplecode.com/2012/07/android-listview-checkbox-example.html
    public class MyCustomAdapter extends ArrayAdapter<List> {

        private ArrayList<List> listOfLists;

        private MyCustomAdapter(Context context, int textViewResourceId,
                               ArrayList<List> listOfLists) {
            super(context, textViewResourceId, listOfLists);
            this.listOfLists = new ArrayList<>();
            this.listOfLists.addAll(listOfLists);
        }

        public class ViewHolder {
            TextView name;
            CheckBox trashCan;
        }

        public View getView(int position, View convertView, ViewGroup parent) {

            ViewHolder holder;
            //Log.v("ConvertView", String.valueOf(position));

            if (convertView == null) {
                LayoutInflater vi = (LayoutInflater) getSystemService(
                        Context.LAYOUT_INFLATER_SERVICE);
                convertView = vi.inflate(R.layout.list_rows, null);

                holder = new ViewHolder();
                holder.name = (TextView) convertView.findViewById(R.id.textView);
                holder.trashCan = (CheckBox) convertView.findViewById(R.id.deleteButton);
                convertView.setTag(holder);
            } else {
                holder = (ViewHolder) convertView.getTag();
            }

            List list = listOfLists.get(position);
            holder.name.setText(list.getName());
            holder.name.setTag(list);
            holder.trashCan.setChecked(false);
            holder.trashCan.setTag(list);

            // This will check if any lists are clicked
            selectList(holder);

            // This will check if a list is long clicked and allow the name
            // to be changed.
            renameList(holder);

            // This will check if a trash can was clicked and should be removed
            deleteList(holder);

            return convertView;
        }
    }


    // Select the list the user specifies
    @Override
    public void selectList(MyCustomAdapter.ViewHolder holder)
    {
        // "Listens" for clicks on our list of lists
        holder.name.setOnClickListener( new View.OnClickListener() {
            public void onClick(View v) {
                TextView tv = (TextView) v;

                List list = (List) tv.getTag();
                String listName = list.getName();

                // Throw a toast to give confirmation of the list chosen
                /*Context context = getApplicationContext();
                CharSequence text = "List selected: ";
                int duration = Toast.LENGTH_SHORT;
                Toast toast = Toast.makeText(context, text + listName, duration);
                toast.show();*/

                // Jump to our list view and all items in it
                Intent intent = new Intent(MainActivity.this, ListActivity.class);
                intent.putExtra("listSelected", listName);
                startActivity(intent);
            }
        });
    }

    // Rename the specified list with that given by the user
    @Override
    public void renameList(MainActivity.MyCustomAdapter.ViewHolder holder)
    {
        // "Listens" for long clicks on our list of lists
        holder.name.setOnLongClickListener( new View.OnLongClickListener() {
            public boolean onLongClick(View v) {
                TextView tv = (TextView) v;

                List list = (List) tv.getTag();
                //String oldListName = list.getName();

                String strNewListName = newListName.getText().toString();
                // Note assumption is that new list is case sensitive
                // i.e. GroceryList is different from groceryList
                if (strNewListName.length() > 0
                        && !(listExists(listOfLists, strNewListName))){

                    // Rename with test data
                    // list.setName(strNewListName);

                    // Throw a toast to the user for confirmation
                    /*Context context = getApplicationContext();
                    CharSequence text = "List " + oldListName + " renamed to " + strNewListName;
                    int duration = Toast.LENGTH_SHORT;
                    Toast toast = Toast.makeText(context, text, duration);
                    toast.show();*/

                    // Rename list with user provided name
                    int listIndex = mList.findListIndex(list.getName());
                    mList.renameFromMasterList(listIndex, strNewListName);
                    Context context = getApplicationContext();
                    lp.saveList(mList,context);

                    // Re-populate the list view
                    viewAllLists();
                }
                else {
                    Context context = getApplicationContext();
                    CharSequence text = "List already exists/invalid list name specified!";
                    int duration = Toast.LENGTH_SHORT;
                    Toast toast = Toast.makeText(context, text, duration);
                    toast.show();
                }
                return false;
            }
        });
    }

    public void onButtonClick(View view)
    {
        // Initialize variable if user put text in text box
        String strNewListName = newListName.getText().toString();

        switch (view.getId()){

            case R.id.addNewListButton:

                createList(strNewListName);

            break;
        }
    }

    // Create a new list named by the user
    @Override
    public void createList(String strNewListName)
    {
        // Note assumption is that new list is case sensitive
        // i.e. GroceryList is different from groceryList
        if (strNewListName.length() > 0
                && !(listExists(listOfLists, strNewListName))){

            List list = new List(strNewListName);
            //listOfLists.add(list);

            // Add list
            mList.addToMasterList(list);
            Context context = getApplicationContext();
            lp.saveList(mList,context);

            // Re-populate the list view
            viewAllLists();
        }
        else {
            Context context = getApplicationContext();
            CharSequence text = "List already exists/invalid list name specified!";
            int duration = Toast.LENGTH_SHORT;
            Toast toast = Toast.makeText(context, text, duration);
            toast.show();
        }
    }

    // Delete the list the user specifies
    @Override
    public void deleteList(MyCustomAdapter.ViewHolder holder)
    {
        holder.trashCan.setOnClickListener( new View.OnClickListener() {
            public void onClick(View v) {
                CheckBox cb = (CheckBox) v;

                if (cb.isChecked()) {
                    TextView tv = (TextView) v;
                    List list = (List) tv.getTag();
                    String listName = list.getName();

                    //listOfLists.remove(list);

                    // Remove list
                    mList.removeFromMasterList(list);
                    Context context = getApplicationContext();
                    lp.saveList(mList,context);

                    // Re-populate the list view
                    viewAllLists();

                    /*Toast.makeText(getApplicationContext(),
                            "List deleted: " + listName,
                            Toast.LENGTH_SHORT).show();*/
                }
            }
        });
    }

    // Checks if a list the user wants to create already exists
    public boolean listExists(ArrayList<List> list, String newListName) {
        for (List item : list){
            if (item.getName().equals(newListName)) return true;
        }

        return false;
    }

}
