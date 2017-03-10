package edu.gatech.seclass.replace;

import javax.print.DocFlavor;

import java.io.*;
import java.io.IOException;
import java.nio.file.*;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;

import edu.gatech.seclass.replace.ReplacePair;
public class Main {

    public static void main(String[] args) {
        //The following array will be used to identify the input arguments
        //as well as the position for each argument
        //[-b, -f, -l, -i, starting replaced instance, starting replacing instance, number of file(s), starting position]
        int[] replaceInput = new int[] {-1, -1, -1, -1, -1, -1, -1, -1};
        ArrayList<ReplacePair> replaceList = new ArrayList<>();
        if(!validateInput(args, replaceInput, replaceList)){
            usage();
        } else {
            //Evaluate input options
            //Check if -b is provided
            if(replaceInput[0] != -1) {
                createBackUpFiles(args, replaceInput);
            }
            //process replace if the (first) replaced word is not empty
            if(args[replaceInput[4]] != "") {
                processReplace(args, replaceInput, replaceList);
            } else {
                usage();
            }
        }
    }

    /*
        This method will process the replace using the provided arguments
    */
    public static void processReplace(String[] args, int[] replaceInput, ArrayList<ReplacePair> replaceList) {
        boolean replaceFirstOccurrence = false;
        boolean replaceLastOccurrence = false;
        boolean replaceAll = false;
        boolean ignoreCase = false;

        //set option switches
        if(replaceInput[1] == -1 && replaceInput[2] == -1){
            replaceAll = true;
        } else {
            if(replaceInput[1] != -1){
                replaceFirstOccurrence = true;
            }
            if(replaceInput[2] != -1){
                replaceLastOccurrence = true;
            }
        }
        if(replaceInput[3] != -1) {
            ignoreCase = true;
        }

        //processing replace on the input file(s)
        int startingFile = replaceInput[7];
        for (int i = 0; i < replaceInput[6]; i++){
            //replacing each pair of words
            for (int j = 0; j < replaceList.size(); j++) {
                replaceString(args[startingFile], replaceList.get(j).getLeft(), replaceList.get(j).getRight(), replaceAll, replaceFirstOccurrence, replaceLastOccurrence, ignoreCase);
            }
            startingFile++;
        }
    }

    /*
        This method will replace a string with another string in a given input file using different replacement options
        (i.e. replace the first occurrence, replace the last occurrence, replace all occurrences and case sensitive)
     */
    public static void replaceString(String inputFile, String replacedString, String replacingString,
                                     boolean replaceAll, boolean replaceFirstOccurrence, boolean replaceLastOccurrence, boolean ignoreCase){
        try {
            String oldContent = getFileContent(inputFile);
            if(oldContent != null) {
                String oldContentCaseIgnored = null;
                String replacedStringCaseIgnored = null;
                String newContent = null;
                FileWriter fileWriter = new FileWriter(inputFile);

                if (replaceAll) {
                    if (ignoreCase) {
                        //replace string and ignore the case
                        newContent = oldContent.replaceAll("(?i)" + replacedString, replacingString);
                    } else {
                        //replace string with case sensitive
                        newContent = oldContent.replace(replacedString, replacingString);
                    }
                } else {
                    if (replaceFirstOccurrence) {
                        if (ignoreCase) {
                            //replace 1st occurrence and ignore the case
                            newContent = oldContent.replaceFirst("(?i)" + replacedString, replacingString);
                        } else {
                            //replace 1st occurrence with case sensitive
                            newContent = oldContent.replaceFirst(replacedString, replacingString);
                        }
                        oldContent = newContent;
                    }
                    if (replaceLastOccurrence) {
                        int startingIndex = -1;
                        if (ignoreCase) {
                            //convert old file and replaced string to lower case when case ignore is selected
                            oldContentCaseIgnored = oldContent.toLowerCase();
                            replacedStringCaseIgnored = replacedString.toLowerCase();
                            //search for the last index of the replaced string
                            startingIndex = oldContentCaseIgnored.lastIndexOf(replacedStringCaseIgnored);
                            //replace the last occurrence and ignore the case
                            if (startingIndex != -1) {
                                //check to see if there are still content at the end of the existing string
                                if(startingIndex + replacedString.length() < oldContent.length()) {
                                    //there are content at the end of the string, copy it to the new string
                                    newContent = oldContent.substring(0, startingIndex) + replacingString + oldContent.substring(startingIndex + replacedString.length());
                                } else {
                                    //there are no other content at the end of the string
                                    newContent = oldContent.substring(0, startingIndex) + replacingString;
                                }
                            }
                        } else {
                            //search for the last index of the replaced string
                            startingIndex = oldContent.lastIndexOf(replacedString);
                            //replace the last occurrence with case sensitive
                            if (startingIndex != -1) {
                                //check to see if there are still content at the end of the existing string
                                if (startingIndex + replacedString.length() < oldContent.length()) {
                                    //there are contents at the end of the string, copy it to the new string
                                    newContent = oldContent.substring(0, startingIndex) + replacingString + oldContent.substring(startingIndex + replacedString.length());
                                } else {
                                    //there are no other content at the end of the string
                                    newContent = oldContent.substring(0, startingIndex) + replacingString;
                                }
                            }
                        }
                    }
                }
                fileWriter.write(newContent);
                fileWriter.close();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /*
        This method will return the file content
     */
    public static String getFileContent(String filename) {
        Charset charset = StandardCharsets.UTF_8;
        String content = null;
        Path p = Paths.get(filename);
        try {
            content = new String(Files.readAllBytes(p), charset);
        } catch (NoSuchFileException noF){
            String inputFile = p.getFileName().toString();
            System.err.println("File " + inputFile + " not found");
        } catch (IOException e) {
            e.printStackTrace();
        }
        return content;
    }

    /*
        This method will create backup for the provided input files
     */
    public static void createBackUpFiles(String[] args, int[] replaceInput) {
        int startingPosition = replaceInput[7];
        int fileCounts = replaceInput[6];
        Path source;
        Path destination;
        for(int i = 0; i < fileCounts; i++) {
            source = Paths.get(args[startingPosition]);
            destination = Paths.get(args[startingPosition] + ".bck");
            try {
                Files.copy(source, destination);
            }catch (FileAlreadyExistsException fe){
                String inputFile = source.getFileName().toString();
                System.err.println("Not performing replace for " + inputFile + ": Backup file already exists");
            }catch (IOException e) {
                e.printStackTrace();
            }
            startingPosition++;
        }
    }

    /*
        This method will validate the input args and return true if the inputs are
        valid and false if the inputs are invalid
     */
    public static boolean validateInput(String[] args, int[] replaceInput, ArrayList<ReplacePair> replaceList) {
        int varCount = 0;
        boolean result;
        ArrayList<Integer> list = new ArrayList<>();
        for (int i = 0; i < args.length; i++) {
            if (args[i] == "--") {
                varCount++;
                list.add(i);
            }
        }
        if(varCount < 1){
            //not enough input provided
            result = false;
        } else if (varCount == 1){
            result = validateRegular(args, replaceInput, list, replaceList);
        } else {
            result = validateOverride(args, replaceInput, list, replaceList);
        }
        return result;
    }

    /*
     *  This method will validate the input when no override is provided (i.e. only one '--' was specified)
     */
    public static boolean validateRegular(String[] args, int[] replaceInput, ArrayList<Integer> list, ArrayList<ReplacePair> replaceList) {
        boolean firstFile = true;
        boolean invalidInput = false;
        boolean replacedWordFound = false;
        boolean replacingWordFound = false;
        boolean leftFilled = false;
        boolean endOfOption = false;

        String left = null;
        String right = null;
        int elementCount = 0;
        int leftCount = 0;
        int rightCount = 0;
        int fileCount = 0;
        //assign beginning position
        int startFilePos = list.get(0);
        //evaluate replace option(s)
        if(startFilePos == 0) {
            //no input was provided
            invalidInput = true;
        } else {
            for (int i = 0; i < startFilePos; i++) {
                if(!endOfOption) {
                    switch (args[i]) {
                        case "-b":
                            replaceInput[0] = i;
                            break;
                        case "-f":
                            replaceInput[1] = i;
                            break;
                        case "-l":
                            replaceInput[2] = i;
                            break;
                        case "-i":
                            replaceInput[3] = i;
                            break;
                        default:
                            //Populate the replace and replacing words
                            //Set the endOfOption to true
                            endOfOption = true;
                            //left element (replaced word) hasn't been filled
                            if (!leftFilled) {
                                //assign left string to current args
                                left = args[i];
                                //increase element count
                                elementCount++;
                                leftCount++;
                                //set left element filled to true
                                leftFilled = true;
                                //set the first replaced occurrence
                                if (!replacedWordFound) {
                                    //initial
                                    replaceInput[4] = i;
                                    replacedWordFound = true;
                                }
                            } //fill the right element (replacing word)
                            else {
                                //assign right string to current args
                                right = args[i];
                                //increase element count
                                elementCount++;
                                rightCount++;
                                //set left element filled to false - get ready for the next iteration
                                leftFilled = false;
                                //set first replacing occurrence
                                if (!replacingWordFound) {
                                    //initial
                                    replaceInput[5] = i;
                                    replacingWordFound = true;
                                }
                            }
                            //element count is exactly 2 (completed pair), add the pair to the replaceList
                            if (elementCount == 2) {
                                ReplacePair rp = new ReplacePair(left, right);
                                replaceList.add(rp);
                                //reset the element count
                                elementCount = 0;
                            }
                    }
                } else {
                    //Populate the replace and replacing words
                    //left element (replaced word) hasn't been filled
                    if (!leftFilled) {
                        //assign left string to current args
                        left = args[i];
                        //increase element count
                        elementCount++;
                        leftCount++;
                        //set left element filled to true
                        leftFilled = true;
                        //set the first replaced occurrence
                        if (!replacedWordFound) {
                            //initial
                            replaceInput[4] = i;
                            replacedWordFound = true;
                        }
                    } //fill the right element (replacing word)
                    else {
                        //assign right string to current args
                        right = args[i];
                        //increase element count
                        elementCount++;
                        rightCount++;
                        //set left element filled to false - get ready for the next iteration
                        leftFilled = false;
                        //set first replacing occurrence
                        if (!replacingWordFound) {
                            //initial
                            replaceInput[5] = i;
                            replacingWordFound = true;
                        }
                    }
                    //element count is exactly 2 (completed pair), add the pair to the replaceList
                    if (elementCount == 2) {
                        ReplacePair rp = new ReplacePair(left, right);
                        replaceList.add(rp);
                        //reset the element count
                        elementCount = 0;
                    }
                }
            }
        }
        //validate that replaced and replacing words are available
        if(!replacedWordFound || !replacingWordFound) {
            invalidInput = true;
        }
        //validate that the replaced word and replacing word counts are the same
        if(leftCount != rightCount){
            invalidInput = true;
        }
        /*** This has been obsoleted as part of D3 requirement (mainTest23) - '--' is no longer a requirement
         * for '-*' option.
        //verify that the input arguments are in the correct order (i.e. replaced and replacing words should always
        //come after the replace options
        if(replaceInput[4] < replaceInput[0] || replaceInput[4] < replaceInput[1] ||
           replaceInput[4] < replaceInput[2] || replaceInput[4] < replaceInput[3] ||
           replaceInput[5] < replaceInput[0] || replaceInput[5] < replaceInput[1] ||
           replaceInput[5] < replaceInput[2] || replaceInput[5] < replaceInput[3] ){
            invalidInput = true;
        }
        */
        //evaluate input file(s)
        if (!invalidInput) {
            for (int i = startFilePos + 1; i < args.length; i++) {
                if (firstFile) {
                    replaceInput[7] = i;
                    firstFile = false;
                    fileCount++;
                } else {
                    fileCount++;
                }
            }
            if(fileCount == 0){
                invalidInput = true;
            } else {
                //Assign the total number of input files to index 6
                replaceInput[6] = fileCount;
            }
        }

        if(invalidInput) {
            return false;
        } else {
            return true;
        }
    }

    /*
     *  This method will validate the input when override is provided (i.e. two '--' were specified)
     */
    public static boolean validateOverride(String[] args, int[] replaceInput, ArrayList<Integer> list, ArrayList<ReplacePair> replaceList) {
        boolean firstFile = true;
        boolean invalidInput = false;
        boolean replacedWordFound = false;
        boolean replacingWordFound = false;
        boolean leftFilled = false;
        String left = null;
        String right = null;
        int leftCount = 0;
        int rightCount = 0;
        int fileCount = 0;
        int elementCount = 0;
        //assign starting argument position
        int startOverridePos = list.get(0);
        //assign the file beginning position
        int startFilePos = list.get(list.size() - 1);

        //evaluate replace option(s)
        if(startOverridePos > 0){
            for (int i = 0; i < startOverridePos; i++) {
                System.out.println(i + " - " + args[i]);
                switch (args[i]) {
                    case "-b":
                        replaceInput[0] = i;
                        break;
                    case "-f":
                        replaceInput[1] = i;
                        break;
                    case "-l":
                        replaceInput[2] = i;
                        break;
                    case "-i":
                        replaceInput[3] = i;
                        break;
                    default:
                        invalidInput = true;
                        break;
                }
            }
        }
        //populate replaced and replacing word
        if (!invalidInput) {
            //set position for replaced and replacing words
            for (int i = startOverridePos + 1; i < startFilePos; i++) {
                if (!leftFilled) {
                    //assign left string to current args
                    left = args[i];
                    //set left element filled to true
                    leftFilled = true;
                    //set the first replaced occurrence
                    if (!replacedWordFound) {
                        //initial
                        replaceInput[4] = i;
                        replacedWordFound = true;
                    }
                    //increase element count
                    elementCount++;
                    leftCount++;
                } //fill the right element (replacing word)
                else {
                    //assign right string to current args
                    right = args[i];
                    //set left element filled to false - get ready for the next iteration
                    leftFilled = false;
                    //set first replacing occurrence
                    if (!replacingWordFound) {
                        //initial
                        replaceInput[5] = i;
                        replacingWordFound = true;
                    }
                    //increase element count
                    elementCount++;
                    rightCount++;
                }
                //element count is exactly 2 (completed pair), add the pair to the replaceList
                if (elementCount == 2) {
                    ReplacePair rp = new ReplacePair(left, right);
                    replaceList.add(rp);
                    //reset the element count
                    elementCount = 0;
                }
            }
        }
        //validate that the replaced and replacing words are found and the pairs match
        if (!replacedWordFound || !replacingWordFound || (leftCount != rightCount)){
            invalidInput = true;
        }
        //evaluate input file(s)
        if (!invalidInput) {
            for (int i = startFilePos + 1; i < args.length; i++) {
                if (firstFile) {
                    replaceInput[7] = i;
                    firstFile = false;
                    fileCount++;
                } else {
                    fileCount++;
                }
            }
            if(fileCount == 0){
                invalidInput = true;
            } else {
                //Assign the total number of input files to index 6
                replaceInput[6] = fileCount;
            }
        }

        if(invalidInput) {
            return false;
        } else {
            return true;
        }
    }

    private static void usage() {
        System.err.println("Usage: Replace [-b] [-f] [-l] [-i] <from> <to> -- " + "<filename> [<filename>]*" );
    }

}