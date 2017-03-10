package edu.gatech.seclass.glm.db;
import android.provider.BaseColumns;

/**
 * Created by HPS on 10/12/16.
 */

public class DBContract {
    public static final class Item implements BaseColumns {

        public static final String TABLE_NAME = "items";
        public static final String COLUMN_ITEM_NAME = "item_name";
        public static final String COLUMN_ITEM_TYPE = "item_type";
        public static final String COLUMN_ITEM_UNIT = "item_unit";
    }
}
