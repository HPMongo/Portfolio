package edu.gatech.seclass.glm;
import edu.gatech.seclass.glm.List;
import java.util.ArrayList;


public class MasterList {

    private ArrayList<List> currentList = new ArrayList<List>();

    public MasterList(){
        super();
    }

    public void addToMasterList(List newList) {
        currentList.add(newList);
    }

    public void removeFromMasterList(List inList) {
        currentList.remove(inList);
    }

    public void renameFromMasterList(int inList, String newName){
        currentList.get(inList).setName(newName);
    }

    public int getListCount(){
        return currentList.size();
    }

    public ArrayList<List> getMasterList() {return currentList;}

    public String getListName(int inList){
        return currentList.get(inList).getName();
    }

    public List getList(int listIdx) {
        return currentList.get(listIdx);
    }

    public void addItemToList(int listIdx, Items newItem){
        currentList.get(listIdx).addItemToList(newItem);
    }

    public void removeItemFromList(int listIdx, Items oldItem){
        currentList.get(listIdx).removeItemFromList(oldItem);
    }

    public int findListIndex (String listName){
        int listIdx = -1;
        for (int i = 0; i < currentList.size(); i++){
            if(currentList.get(i).getName().equals(listName)){
                listIdx = i;
                return listIdx;
            }
        }
        return listIdx;
    }
}
