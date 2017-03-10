package edu.gatech.seclass.glm;
import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import edu.gatech.seclass.glm.MasterList;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;


public class ListHelper {

    public static final String MyPREFERENCES = "ListPrefs";
    public static final String InitialSetUp = "SetUpPrefs";

    public static void saveList(MasterList inList, Context context) {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
        SharedPreferences.Editor editor = prefs.edit();
        Gson gson = new Gson();
        String json = gson.toJson(inList);
        editor.putString(MyPREFERENCES, json);
        editor.commit();
    }

    public static MasterList getList(Context context) {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
        String jsonInString = prefs.getString(MyPREFERENCES, null);
        Gson gson = new Gson();
        MasterList outList = gson.fromJson(jsonInString, MasterList.class);
        return outList;
    }

    public static void saveSetUp(Context context) {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
        SharedPreferences.Editor editor = prefs.edit();
        String value = "SetUpCompleted";
        editor.putString(InitialSetUp, value);
        editor.commit();
    }

    public static String getSetUp(Context context) {
        SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(context);
        return prefs.getString(InitialSetUp, null);
    }

}
