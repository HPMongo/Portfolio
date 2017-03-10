package edu.gatech.seclass.glm;
import edu.gatech.seclass.glm.Items;

import java.util.ArrayList;

public class List {

    private String name = null;
    private ArrayList<Items> currentList = new ArrayList<Items>();
    //boolean selected = false;

    public List(String name){
        super();
        this.name = name;
    }

    public String getName() {return name;}

    public void setName(String name) {this.name = name;}

    public void addItemToList(Items newItem) {
        currentList.add(newItem);
    }

    public void removeItemFromList(Items inItem) {
        currentList.remove(inItem);
    }

    public ArrayList<Items> getItemsList() {return currentList;}

    public void removeAllCheckOffs() {
        for(int i = 0; i < currentList.size(); i++) {
            currentList.get(i).setCheckOff(false);
        }
    }

    public void addCheckOffItemFromList(int itemIdx) {
        currentList.get(itemIdx).setCheckOff(true);
    }

    public void removeItemCheckOffFromList(int itemIdx){
        currentList.get(itemIdx).setCheckOff(false);
    }

    public void updateItemQtyFromList (int itemIdx, double inQty){
        currentList.get(itemIdx).setItemQty(inQty);
    }

    public int findItemIndex (String itemName){
        int itemIdx = -1;
        for (int i = 0; i < currentList.size(); i++){
            if(currentList.get(i).getItemName().equals(itemName)){
                itemIdx = i;
                return itemIdx;
            }
        }
        return itemIdx;
    }
}
