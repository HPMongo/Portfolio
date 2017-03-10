package edu.gatech.seclass.glm;
import android.support.v7.app.AppCompatActivity;
import edu.gatech.seclass.glm.MasterList;
import edu.gatech.seclass.glm.db.DBHelper;
import android.content.Context;


/**
 * Created by HPS on 10/21/16.
 */

public class InitialSetUp extends AppCompatActivity {

    public static void runInitialSetUp(Context context){

        //Initialize objects
        MasterList mList = new MasterList();
        ListHelper lp = new ListHelper();
        DBHelper db = new DBHelper(context);

        //Add initial values to database
        db.getWritableDatabase();
        db.addItem("apple", "fruit", "each");
        db.addItem("banana", "fruit", "each");
        db.addItem("Swiss cheese", "dairy", "oz");
        db.addItem("American cheese", "dairy", "oz");
        db.addItem("Coke", "beverage", "can");
        db.addItem("Water", "beverage", "bottle");
        db.addItem("Donuts", "snacks", "dozen");
        db.addItem("Cookie", "snacks", "each");

        //Save MasterList and initial set up to sharedpreferences
        lp.saveList(mList,context);
        lp.saveSetUp(context);
    }
}
