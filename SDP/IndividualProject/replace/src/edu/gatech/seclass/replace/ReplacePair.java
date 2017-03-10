package edu.gatech.seclass.replace;

/**
 * Created by Huy on 11/30/2016.
 * This class will be used to store replaced and replacing words as a pair.
 */
public class ReplacePair {
    private String left;
    private String right;

    public ReplacePair(String left, String right) {
        this.left = left;
        this.right = right;
    }

    public String getLeft(){
        return left;
    }

    public String getRight(){
        return right;
    }
}
