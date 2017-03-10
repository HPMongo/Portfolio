package edu.gatech.seclass.glm;


public class Items {
    private String itemName;
    private String itemType;
    private String itemUnit;
    private double itemQty;
    private boolean checkOff;

    public Items(String iName, String iType, String iUnit, double iQty, boolean inCheckOff){
        itemName = iName;
        itemType = iType;
        itemUnit = iUnit;
        itemQty = iQty;
        checkOff = inCheckOff;
    }

    // Constructor
    public Items()
    {
        itemName = null;
        itemType = null;
        itemUnit = null;
        itemQty = 0.0;
        checkOff = false;
    }

    //Setter methods
    public void setItemName(String inName) {
        this.itemName = inName;
    }

    public void setItemType(String inType) {
        this.itemType = inType;
    }

    public void setItemUnit(String inUnit) {
        this.itemUnit = inUnit;
    }

    public void setItemQty(double inQty) {
        this.itemQty = inQty;
    }

    public void setCheckOff(boolean inValue){
        this.checkOff = inValue;
    }

    //Getter methods
    public String getItemName(){
        return this.itemName;
    }

    public String getItemType(){
        return this.itemType;
    }

    public String getItemUnit(){
        return this.itemUnit;
    }

    public double getItemQty() {
        return this.itemQty;
    }

    public boolean getCheckOff(){
        return this.checkOff;
    }
}

