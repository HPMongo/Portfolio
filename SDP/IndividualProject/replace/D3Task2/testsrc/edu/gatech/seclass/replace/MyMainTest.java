package edu.gatech.seclass.replace;

import org.junit.After;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TemporaryFolder;

import java.io.*;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;

import static org.junit.Assert.*;

/**
 * Created by Huy on 11/11/2016.
 */
public class MyMainTest {
    //The following code was copied from the MainTest.java to fulfill the
    //initial testing requirement
    private ByteArrayOutputStream outStream;
    private ByteArrayOutputStream errStream;
    private PrintStream outOrig;
    private PrintStream errOrig;
    private Charset charset = StandardCharsets.UTF_8;

    @Rule
    public TemporaryFolder temporaryFolder = new TemporaryFolder();

    @Before
    public void setUp() throws Exception {
        outStream = new ByteArrayOutputStream();
        PrintStream out = new PrintStream(outStream);
        errStream = new ByteArrayOutputStream();
        PrintStream err = new PrintStream(errStream);
        outOrig = System.out;
        errOrig = System.err;
        System.setOut(out);
        System.setErr(err);
    }

    @After
    public void tearDown() throws Exception {
        System.setOut(outOrig);
        System.setErr(errOrig);
    }

    // Some utilities

    private File createTmpFile() throws IOException {
        File tmpfile = temporaryFolder.newFile();
        tmpfile.deleteOnExit();
        return tmpfile;
    }

    private File createInputFile1() throws Exception {
        File file1 =  createTmpFile();
        FileWriter fileWriter = new FileWriter(file1);

        fileWriter.write("Howdy Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!");

        fileWriter.close();
        return file1;
    }

    private File createInputFile2() throws Exception {
        File file1 =  createTmpFile();
        FileWriter fileWriter = new FileWriter(file1);

        fileWriter.write("Howdy Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"howdy Bill\" twice");

        fileWriter.close();
        return file1;
    }

    private File createInputFile3() throws Exception {
        File file1 =  createTmpFile();
        FileWriter fileWriter = new FileWriter(file1);

        fileWriter.write("Howdy Bill, have you learned your abc and 123?\n" +
                "It is important to know your abc and 123," +
                "so you should study it\n" +
                "and then repeat with me: abc and 123");

        fileWriter.close();
        return file1;
    }

    private File createInputFile4() throws Exception {
        File file1 =  createTmpFile();
        FileWriter fileWriter = new FileWriter(file1);

        fileWriter.write("");

        fileWriter.close();
        return file1;
    }

    private File createInputFile5() throws Exception {
        File file = createTmpFile();
        FileWriter fileWriter = new FileWriter(file);

        fileWriter.write("First line: not replaced\n" +
                "Second line: not replaced\n" +
                "Third line: not replaced\n" +
                "Last line: not replaced\n");

        fileWriter.close();
        return file;
    }

    private File createInputFile6() throws Exception {
        File file = createTmpFile();
        FileWriter fileWriter = new FileWriter(file);

        fileWriter.write("ababab");

        fileWriter.close();
        return file;
    }

    private File createInputFile7() throws Exception {
        File file = createTmpFile();
        FileWriter fileWriter = new FileWriter(file);

        fileWriter.write("The goal here is to replace string \"-i\" with\n" +
                "string \"-f\". Since we may also want to do multiple replacements,\n" +
                "we will repeat the two strings here: -i and -f");

        fileWriter.close();
        return file;
    }

    private File createInputFile8() throws Exception {
        File file = createTmpFile();
        FileWriter fileWriter = new FileWriter(file);

        fileWriter.write("Let's have some numbers in the file: 12345678");

        fileWriter.close();
        return file;
    }

    private File createInputFile9() throws Exception {
        File file = createTmpFile();
        FileWriter fileWriter = new FileWriter(file);

        fileWriter.write("-- -- -- --");

        fileWriter.close();
        return file;
    }

    private File createInputFile10() throws Exception {
        File file = createTmpFile();
        FileWriter fileWriter = new FileWriter(file);

        fileWriter.write("-------");

        fileWriter.close();
        return file;
    }

    private File createInputFile11() throws Exception {
        File file = createTmpFile();
        FileWriter fileWriter = new FileWriter(file);

        fileWriter.write("-b -f -B -F");

        fileWriter.close();
        return file;
    }

    private String getFileContent(String filename) {
        String content = null;
        try {
            content = new String(Files.readAllBytes(Paths.get(filename)), charset);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return content;
    }

    // Actual test cases

    @Test
    public void mainTest1() throws Exception {
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();
        File inputFile3 = createInputFile3();

        String args[] = {"-i", "Howdy", "Hello", "--", inputFile1.getPath(), inputFile2.getPath(), inputFile3.getPath()};
        Main.main(args);

        String expected1 = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String expected2 = "Hello Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"Hello Bill\" twice";
        String expected3 = "Hello Bill, have you learned your abc and 123?\n" +
                "It is important to know your abc and 123," +
                "so you should study it\n" +
                "and then repeat with me: abc and 123";

        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        String actual3 = getFileContent(inputFile3.getPath());

        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
        assertEquals("The files differ!", expected3, actual3);

        assertFalse(Files.exists(Paths.get(inputFile1.getPath() + ".bck")));
        assertFalse(Files.exists(Paths.get(inputFile2.getPath() + ".bck")));
        assertFalse(Files.exists(Paths.get(inputFile3.getPath() + ".bck")));
    }

    @Test
    public void mainTest2() throws Exception {
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-b", "-f", "Bill", "William", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);

        String expected1 = "Howdy William,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String expected2 = "Howdy William,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"howdy Bill\" twice";

        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());

        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
        assertTrue(Files.exists(Paths.get(inputFile1.getPath() + ".bck")));
        assertTrue(Files.exists(Paths.get(inputFile2.getPath() + ".bck")));
    }

    @Test
    public void mainTest3() throws Exception {
        File inputFile = createInputFile3();

        String args[] = {"-f", "-l", "abc", "ABC", "--", inputFile.getPath()};
        Main.main(args);

        String expected = "Howdy Bill, have you learned your ABC and 123?\n" +
                "It is important to know your abc and 123," +
                "so you should study it\n" +
                "and then repeat with me: ABC and 123";

        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
        assertFalse(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    @Test
    public void mainTest4() throws Exception {
        File inputFile = createInputFile3();

        String args[] = {"123", "<numbers removed>", "--", inputFile.getPath()};
        Main.main(args);

        String expected = "Howdy Bill, have you learned your abc and <numbers removed>?\n" +
                "It is important to know your abc and <numbers removed>," +
                "so you should study it\n" +
                "and then repeat with me: abc and <numbers removed>";

        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
        assertFalse(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    @Test
    public void mainTest5() throws Exception {
        File inputFile = createInputFile2();

        String args1[] = {"-b", "--", "-a", "1", "--", inputFile.getPath()};
        Main.main(args1);
        String args2[] = {"--", "-b", "2", "--", inputFile.getPath()};
        Main.main(args2);

        String expected = "Howdy Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "1) Item 1\n" +
                "2) Item 2\n" +
                "...\n" +
                "and says \"howdy Bill\" twice";

        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    @Test
    public void mainTest6() {
        String args[] = {"blah",};
        Main.main(args);
        assertEquals("Usage: Replace [-b] [-f] [-l] [-i] <from> <to> -- <filename> [<filename>]*", errStream.toString().trim());
    }

    /*
    /The following test cases are generated either manually or via test frame
    */

    //Purpose: Testing when the file is not present
    //Implementation of test frame #1
    @Test
    public void mainTest01() throws Exception{
        String args[] = {"Howdy", "Hello", "--"};
        Main.main(args);
        assertEquals("Usage: Replace [-b] [-f] [-l] [-i] <from> <to> -- <filename> [<filename>]*", errStream.toString().trim());
    }

    //Purpose: Testing when the file is empty
    //Implementation of test frame #2
    @Test
    public void mainTest02() throws Exception{
        File inputFile = createInputFile4();

        String args[] = {"Howdy", "Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
    }

    //Purpose: Testing when the file is available and the replace word is available
    //Implementation of test frame #3
    @Test
    public void mainTest03() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"Howdy", "Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
    }

    //Purpose: Testing when the replaced word is missing
    //Implementation of test frame #4
    @Test
    public void mainTest04() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"Hello", "--", inputFile.getPath()};
        Main.main(args);
        assertEquals("Usage: Replace [-b] [-f] [-l] [-i] <from> <to> -- <filename> [<filename>]*", errStream.toString().trim());
    }

    //Purpose: Testing when the replacing word is missing
    //Implementation of test frame #5
    @Test
    public void mainTest05() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"Howdy", "--", inputFile.getPath()};
        Main.main(args);
        assertEquals("Usage: Replace [-b] [-f] [-l] [-i] <from> <to> -- <filename> [<filename>]*", errStream.toString().trim());
    }

    //Purpose: Testing when the replaced word is not available in the file
    //Implementation of test frame #6
    @Test
    public void mainTest06() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"Hola","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Howdy Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
    }

    //Purpose: Testing when all options are available and the pattern is in the beginning of the file
    //Implementation of test frame #7
    @Test
    public void mainTest07() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-b","-f","-l","-i", "This","NewThis", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Howdy Bill,\n" +
                "NewNewThis is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    //Purpose: Testing when all options (minus case sensitive) are available and the pattern is in the beginning of the file
    //Implementation of test frame #8
    @Test
    public void mainTest08() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-b","-f","-l", "Howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));

    }

    //Purpose: Testing when all options (minus replace last) are available and the pattern is in the beginning of the file
    //Implementation of test frame #9
    @Test
    public void mainTest09() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-b","-f","-i", "Howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    //Purpose: Testing when -f, -b are available and the pattern is in the beginning of the file
    //Implementation of test frame #10
    @Test
    public void mainTest10() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-b","-f", "Howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    //Purpose: Testing when -b, -l, -i are available and the pattern is in the beginning of the file
    //Implementation of test frame #11
    @Test
    public void mainTest11() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-b","-l","-i","Howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Howdy Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    //Purpose: Testing when -b, -l are available and the pattern is in the beginning of the file
    //Implementation of test frame #12
    @Test
    public void mainTest12() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-b","-l","Howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    //Purpose: Testing when -b, -i are available and the pattern is in the beginning of the file
    //Implementation of test frame #13
    @Test
    public void mainTest13() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-b","-i","Howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    //Purpose: Testing when -b is available and the pattern is in the beginning of the file
    //Implementation of test frame #14
    @Test
    public void mainTest14() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-b","Howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    //Purpose: Testing when  -f, -l, -i are available and the pattern is in the beginning of the file
    //Implementation of test frame #15
    @Test
    public void mainTest15() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-f","-l","-i","Howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);

    }

    //Purpose: Testing when  -f, -l are available and the pattern is in the beginning of the file
    //Implementation of test frame #16
    @Test
    public void mainTest16() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-f","-l","Howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
    }

    //Purpose: Testing when  -f, -i are available and the pattern is in the beginning of the file
    //Implementation of test frame #17
    @Test
    public void mainTest17() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-f","-i","Howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
    }

    //Purpose: Testing when  -f is available and the pattern is in the beginning of the file
    //Implementation of test frame #18
    @Test
    public void mainTest18() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-f","Howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
    }

    //Purpose: Testing when  -l, -i are available and the pattern is in the beginning of the file
    //Implementation of test frame #19
    @Test
    public void mainTest19() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-l","-i","Howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Howdy Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
    }

    //Purpose: Testing when  -l is available and the pattern is in the beginning of the file
    //Implementation of test frame #20
    @Test
    public void mainTest20() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-l","Howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
    }

    //Purpose: Testing when  -i is available and the pattern is in the beginning of the file
    //Implementation of test frame #21
    @Test
    public void mainTest21() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-i","Howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
    }

    //Purpose: Testing when no option is available and the pattern is in the beginning of the file
    //Implementation of test frame #22
    @Test
    public void mainTest22() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"Howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
    }

    //Purpose: Testing when -b, -f, -l, -i are available and the pattern is in the ending of the file
    //Implementation of test frame #23
    @Test
    public void mainTest23() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-b","-f","-l","-i","howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    //Purpose: Testing when -b, -f, -l are available and the pattern is in the ending of the file
    //Implementation of test frame #24
    @Test
    public void mainTest24() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-b","-f","-l","howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Howdy Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    //Purpose: Testing when -b, -f, -i are available and the pattern is in the ending of the file
    //Implementation of test frame #25
    @Test
    public void mainTest25() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-b","-f","-i","howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    //Purpose: Testing when -b, -f are available and the pattern is in the ending of the file
    //Implementation of test frame #26
    @Test
    public void mainTest26() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-b","-f","howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Howdy Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    //Purpose: Testing when -b, -l, -i are available and the pattern is in the ending of the file
    //Implementation of test frame #27
    @Test
    public void mainTest27() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-b","-l","-i","howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Howdy Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    //Purpose: Testing when -b, -l are available and the pattern is in the ending of the file
    //Implementation of test frame #28
    @Test
    public void mainTest28() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-b","-l","howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Howdy Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    //Purpose: Testing when -b, -i are available and the pattern is in the ending of the file
    //Implementation of test frame #29
    @Test
    public void mainTest29() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-b","-i","howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    //Purpose: Testing when -b is available and the pattern is in the ending of the file
    //Implementation of test frame #30
    @Test
    public void mainTest30() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-b","howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Howdy Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    //Purpose: Testing when -f, -l, -i are available and the pattern is in the ending of the file
    //Implementation of test frame #31
    @Test
    public void mainTest31() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-f","-l", "-i", "howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
    }

    //Purpose: Testing when -f, -l are available and the pattern is in the ending of the file
    //Implementation of test frame #32
    @Test
    public void mainTest32() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-f", "-l", "howdy", "Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Howdy Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
    }

    //Purpose: Testing when -f, -i are available and the pattern is in the ending of the file
    //Implementation of test frame #33
    @Test
    public void mainTest33() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-f","-i", "howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
    }

    //Purpose: Testing when -f is available and the pattern is in the ending of the file
    //Implementation of test frame #34
    @Test
    public void mainTest34() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-f", "howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Howdy Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
    }

    //Purpose: Testing when -l, -i are available and the pattern is in the ending of the file
    //Implementation of test frame #35
    @Test
    public void mainTest35() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-l", "-i", "howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Howdy Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
    }

    //Purpose: Testing when -l is available and the pattern is in the ending of the file
    //Implementation of test frame #36
    @Test
    public void mainTest36() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-l", "howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Howdy Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
    }

    //Purpose: Testing when -i is available and the pattern is in the ending of the file
    //Implementation of test frame #37
    @Test
    public void mainTest37() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"-i", "howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
    }

    //Purpose: Testing when no option is available and the pattern is in the ending of the file
    //Implementation of test frame #38
    @Test
    public void mainTest38() throws Exception{
        File inputFile = createInputFile1();

        String args[] = {"howdy","Hello", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Howdy Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String actual = getFileContent(inputFile.getPath());

        assertEquals("The files differ!", expected, actual);
    }

    //Purpose: Testing when no option is available and the pattern is not available in the files
    //Implementation of test frame #39
    @Test
    public void mainTest39() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"Hola","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Howdy Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String expected2 = "Howdy Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"howdy Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
    }

    //Purpose: Testing when -b, -f, -l, -i are available and the pattern is in the beginning of the files
    //Implementation of test frame #40
    @Test
    public void mainTest40() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-b","-f","-l","-i","Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String expected2 = "Hello Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"Hello Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
        assertTrue(Files.exists(Paths.get(inputFile1.getPath() + ".bck")));
        assertTrue(Files.exists(Paths.get(inputFile2.getPath() + ".bck")));
    }

    //Purpose: Testing when -b, -f, -l are available and the pattern is in the beginning of the files
    //Implementation of test frame #41
    @Test
    public void mainTest41() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-b","-f","-l","Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String expected2 = "Hello Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"howdy Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
        assertTrue(Files.exists(Paths.get(inputFile1.getPath() + ".bck")));
        assertTrue(Files.exists(Paths.get(inputFile2.getPath() + ".bck")));
    }

    //Purpose: Testing when -b, -f, -i are available and the pattern is in the beginning of the files
    //Implementation of test frame #42
    @Test
    public void mainTest42() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-b","-f","-i","Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String expected2 = "Hello Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"howdy Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
        assertTrue(Files.exists(Paths.get(inputFile1.getPath() + ".bck")));
        assertTrue(Files.exists(Paths.get(inputFile2.getPath() + ".bck")));
    }

    //Purpose: Testing when -b, -f are available and the pattern is in the beginning of the files
    //Implementation of test frame #43
    @Test
    public void mainTest43() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-b","-f","Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String expected2 = "Hello Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"howdy Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
        assertTrue(Files.exists(Paths.get(inputFile1.getPath() + ".bck")));
        assertTrue(Files.exists(Paths.get(inputFile2.getPath() + ".bck")));
    }

    //Purpose: Testing when -b, -l, -i are available and the pattern is in the beginning of the files
    //Implementation of test frame #44
    @Test
    public void mainTest44() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-b","-l", "-i", "Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Howdy Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String expected2 = "Howdy Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"Hello Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
        assertTrue(Files.exists(Paths.get(inputFile1.getPath() + ".bck")));
        assertTrue(Files.exists(Paths.get(inputFile2.getPath() + ".bck")));
    }

    //Purpose: Testing when -b, -l are available and the pattern is in the beginning of the files
    //Implementation of test frame #45
    @Test
    public void mainTest45() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-b","-l", "Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String expected2 = "Hello Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"howdy Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
        assertTrue(Files.exists(Paths.get(inputFile1.getPath() + ".bck")));
        assertTrue(Files.exists(Paths.get(inputFile2.getPath() + ".bck")));
    }

    //Purpose: Testing when -b, -i are available and the pattern is in the beginning of the files
    //Implementation of test frame #46
    @Test
    public void mainTest46() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-b","-i", "Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String expected2 = "Hello Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"Hello Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
        assertTrue(Files.exists(Paths.get(inputFile1.getPath() + ".bck")));
        assertTrue(Files.exists(Paths.get(inputFile2.getPath() + ".bck")));
    }

    //Purpose: Testing when -b is available and the pattern is in the beginning of the files
    //Implementation of test frame #47
    @Test
    public void mainTest47() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-b", "Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String expected2 = "Hello Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"howdy Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
        assertTrue(Files.exists(Paths.get(inputFile1.getPath() + ".bck")));
        assertTrue(Files.exists(Paths.get(inputFile2.getPath() + ".bck")));
    }

    //Purpose: Testing when -f, -l, -i are available and the pattern is in the beginning of the files
    //Implementation of test frame #48
    @Test
    public void mainTest48() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-f","-l","-i", "Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String expected2 = "Hello Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"Hello Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
    }

    //Purpose: Testing when -f, -l are available and the pattern is in the beginning of the files
    //Implementation of test frame #49
    @Test
    public void mainTest49() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-f","-l", "Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String expected2 = "Hello Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"howdy Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
    }

    //Purpose: Testing when -f, -i are available and the pattern is in the beginning of the files
    //Implementation of test frame #50
    @Test
    public void mainTest50() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-f","-i", "Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String expected2 = "Hello Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"howdy Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
    }

    //Purpose: Testing when -f is available and the pattern is in the beginning of the files
    //Implementation of test frame #51
    @Test
    public void mainTest51() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-f", "Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String expected2 = "Hello Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"howdy Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
    }

    //Purpose: Testing when -l, -i are available and the pattern is in the beginning of the files
    //Implementation of test frame #52
    @Test
    public void mainTest52() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-l","-i", "Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Howdy Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String expected2 = "Howdy Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"Hello Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
    }

    //Purpose: Testing when -l is available and the pattern is in the beginning of the files
    //Implementation of test frame #53
    @Test
    public void mainTest53() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-l", "Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String expected2 = "Hello Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"howdy Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
    }

    //Purpose: Testing when -i is available and the pattern is in the beginning of the files
    //Implementation of test frame #54
    @Test
    public void mainTest54() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-i", "Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String expected2 = "Hello Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"Hello Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
    }

    //Purpose: Testing when no option is available and the pattern is in the beginning of the files
    //Implementation of test frame #55
    @Test
    public void mainTest55() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String expected2 = "Hello Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"howdy Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
    }

    //Purpose: Testing when -b, -f, -l, -i are available and the pattern is in the end of the files
    //Implementation of test frame #56
    @Test
    public void mainTest56() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-b","-f","-l","-i","Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String expected2 = "Hello Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"Hello Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
        assertTrue(Files.exists(Paths.get(inputFile1.getPath() + ".bck")));
        assertTrue(Files.exists(Paths.get(inputFile2.getPath() + ".bck")));
    }

    //Purpose: Testing when -b, -f, -l are available and the pattern is in the end of the files
    //Implementation of test frame #57
    @Test
    public void mainTest57() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-b","-f","-l","Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String expected2 = "Hello Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"howdy Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
        assertTrue(Files.exists(Paths.get(inputFile1.getPath() + ".bck")));
        assertTrue(Files.exists(Paths.get(inputFile2.getPath() + ".bck")));
    }

    //Purpose: Testing when -b, -f, -i are available and the pattern is in the end of the files
    //Implementation of test frame #58
    @Test
    public void mainTest58() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-b","-f","-i","Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String expected2 = "Hello Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"howdy Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
        assertTrue(Files.exists(Paths.get(inputFile1.getPath() + ".bck")));
        assertTrue(Files.exists(Paths.get(inputFile2.getPath() + ".bck")));
    }

    //Purpose: Testing when -b, -f are available and the pattern is in the end of the files
    //Implementation of test frame #59
    @Test
    public void mainTest59() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-b", "-f", "Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Hello Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"howdy bill\" again!";
        String expected2 = "Hello Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"howdy Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
        assertTrue(Files.exists(Paths.get(inputFile1.getPath() + ".bck")));
        assertTrue(Files.exists(Paths.get(inputFile2.getPath() + ".bck")));
    }

    //Purpose: Testing when -b, -l, -i are available and the pattern is in the end of the files
    //Implementation of test frame #60
    @Test
    public void mainTest60() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-b", "-l", "-i", "Howdy","Hello", "--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Howdy Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hello bill\" again!";
        String expected2 = "Howdy Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"Hello Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
        assertTrue(Files.exists(Paths.get(inputFile1.getPath() + ".bck")));
        assertTrue(Files.exists(Paths.get(inputFile2.getPath() + ".bck")));
    }

    //Purpose: Testing double replaces
    //Additional test case for D3T2
    @Test
    public void mainTest61() throws Exception{
        File inputFile1 = createInputFile1();
        File inputFile2 = createInputFile2();

        String args[] = {"-i", "-b", "Howdy", "Hello", "Hello", "Hola","--", inputFile1.getPath(), inputFile2.getPath()};
        Main.main(args);
        String expected1 = "Hola Bill,\n" +
                "This is a test file for the replace utility\n" +
                "Let's make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And let's say \"Hola bill\" again!";
        String expected2 = "Hola Bill,\n" +
                "This is another test file for the replace utility\n" +
                "that contains a list:\n" +
                "-a) Item 1\n" +
                "-b) Item 2\n" +
                "...\n" +
                "and says \"Hola Bill\" twice";
        String actual1 = getFileContent(inputFile1.getPath());
        String actual2 = getFileContent(inputFile2.getPath());
        assertEquals("The files differ!", expected1, actual1);
        assertEquals("The files differ!", expected2, actual2);
        assertTrue(Files.exists(Paths.get(inputFile1.getPath() + ".bck")));
        assertTrue(Files.exists(Paths.get(inputFile2.getPath() + ".bck")));
    }

    //Purpose: Testing double replaces
    //Additional test case for D3T2
    @Test
    public void mainTest62() throws Exception {
        File inputFile = createInputFile7();

        String args[] = {"The", "THE", "-i", "-f", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "THE goal here is to replace string \"-f\" with\n" +
                "string \"-f\". Since we may also want to do multiple replacements,\n" +
                "we will repeat the two strings here: -f and -f";
        String actual = getFileContent(inputFile.getPath());
        assertEquals("The files differ!", expected, actual);
        assertFalse(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
        assertEquals("", errStream.toString().trim());
    }

    //Purpose: Testing for file that doesn't exist
    //Additional test case for D3T2
    @Test
    public void mainTest63() throws Exception {
        String inputFile = "doesnotexist.txt";

        String args[] = {"-b", "Hello", "Goodbye", "--", inputFile};
        Main.main(args);
        assertFalse(Files.exists(Paths.get(inputFile + ".bck")));
        assertEquals("File doesnotexist.txt not found", errStream.toString().trim());
    }

    //Purpose: Testing delimited replace
    //Additional test case for D3T2
    @Test
    public void mainTest64() throws Exception {
        File inputFile = createInputFile10();

        String args[] = {"--", "--", "xx", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "xxxxxx-";
        String actual = getFileContent(inputFile.getPath());
        assertEquals("The files differ!", expected, actual);
        assertEquals("", errStream.toString().trim());
    }

    //Purpose: Testing edge cases for D3T2
    //Adapted from MainTestAddon #30
    @Test
    public void mainTest65() throws Exception {
        File inputFile = createInputFile8();
        String args[] = {"", "Hey", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Let's have some numbers in the file: 12345678";
        String actual = getFileContent(inputFile.getPath());
        assertEquals("The files differ!", expected, actual);
        assertEquals("Usage: Replace [-b] [-f] [-l] [-i] <from> <to> -- <filename> [<filename>]*", errStream.toString().trim());
    }

    //Purpose: Testing edge cases for D3T2
    //Adapted from MainTestAddon #54
    @Test
    public void mainTest66() throws Exception {
        File inputFile1 = temporaryFolder.newFile("_tmpfile");
        File inputFile2 = temporaryFolder.newFile("_tmpfile.bck");
        inputFile1.deleteOnExit();
        inputFile2.deleteOnExit();
        FileWriter fileWriter1 = new FileWriter(inputFile1);
        FileWriter fileWriter2 = new FileWriter(inputFile2);
        fileWriter1.write("Content of file1");
        fileWriter2.write("Content of file2");
        fileWriter1.close();
        fileWriter2.close();

        String args1[] = {"-b", "Content of ", "This is ", "--", inputFile1.getPath()};
        Main.main(args1);

        String expected1 = "Content of file1";
        String actual1 = getFileContent(inputFile1.getPath());

        assertTrue(Files.exists(Paths.get(inputFile1.getPath() + ".bck")));
        assertEquals("Not performing replace for " + inputFile1.getName() + ": Backup file already exists", errStream.toString().trim());
    }

    //Purpose: Testing edge cases for D3T2
    //Adapted from MainTestAddon #57
    @Test
    public void mainTest67() throws Exception {
        File inputFile = createInputFile1();
        String args[] = {"-i", "let's", "Let us", "let us", "we will", "--", inputFile
                .getPath()};
        Main.main(args);
        String expected = "Howdy Bill,\n" +
                "This is a test file for the replace utility\n" +
                "we will make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And we will say \"howdy bill\" again!";
        String actual = getFileContent(inputFile.getPath());
        assertEquals("The files differ!", expected, actual);
        assertFalse(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
        assertEquals("", errStream.toString().trim());
    }

    //Purpose: Testing edge cases for D3T2
    //Adapted from MainTestAddon #34
    @Test
    public void mainTest68() throws Exception {
        File inputFile = createInputFile8();
        String args[] = {"678", "91011", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Let's have some numbers in the file: 1234591011";
        String actual = getFileContent(inputFile.getPath());
        assertEquals("The files differ!", expected, actual);
    }

    //Purpose: Testing edge cases for D3T2
    //Adapted from MainTestAddon #57
    @Test
    public void mainTest69() throws Exception {
        File inputFile = createInputFile1();
        String args[] = {"-i", "-b", "let's", "Let us", "let us", "we will", "--", inputFile
                .getPath()};
        Main.main(args);
        String expected = "Howdy Bill,\n" +
                "This is a test file for the replace utility\n" +
                "we will make sure it has at least a few lines\n" +
                "so that we can create some interesting test cases...\n" +
                "And we will say \"howdy bill\" again!";
        String actual = getFileContent(inputFile.getPath());
        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
        assertEquals("", errStream.toString().trim());
    }

    //Purpose: Testing edge cases for D3T2
    //Adapted from MainTestAddon #34 (Bug found)
    //Type d: found a bug when converting a string to '--' then converting '--' back to a string.
    //This could be because '--' is not treated as a string for replacement when it is not a delimiter.
    @Test
    public void mainTest70() throws Exception {
        File inputFile = createInputFile8();
        String args[] = {"-b", "12345678", "--", "--", "abc", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Let's have some numbers in the file: abc";
        String actual = getFileContent(inputFile.getPath());
        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    //Purpose: Testing edge cases for D3T2
    //Testing different invalid replace options
    @Test
    public void mainTest71() throws Exception {
        File inputFile = createInputFile8();
        String args[] = {"-a", "-b", "-c", "-d", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "Let's have some numbers in the file: 12345678";
        String actual = getFileContent(inputFile.getPath());
        assertEquals("The files differ!", expected, actual);
    }

    //Purpose: Testing edge cases for D3T2
    //Testing multiple replaces with case insensitive and different replace positions
    //Type d: found a bug when converting a string to '--' then converting '--' back to a string.
    //Either '-' or '--' are not considered as a string when it is not using as delimiter.
    @Test
    public void mainTest72() throws Exception {
        File inputFile = createInputFile9();
        String args[] = {"-b", "-i", "-f", "-l", "--", "ABC", "abc", "xyz", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "xyz -- -- xyz";
        String actual = getFileContent(inputFile.getPath());
        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    //Purpose: Testing edge cases for D3T2
    //Testing multiple replaces with case insensitive and different replace positions
    @Test
    public void mainTest73() throws Exception {
        File inputFile = createInputFile11();
        String args[] = {"-b", "-i", "-f", "B", "BEE", "-B", "-B2", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "-B2EE -f -B -F";
        String actual = getFileContent(inputFile.getPath());
        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    //Purpose: Testing edge cases for D3T2
    //Testing multiple replaces with case insensitive and different replace positions
    //Type d: found a bug when multiple replaces are performing at the same time. This could be
    //because the '-' is not handling appropriately when it is not one of the options.
    @Test
    public void mainTest74() throws Exception {
        File inputFile = createInputFile11();
        String args[] = {"-B", "-BFF", "F", "eff", "f", "ph", "--", inputFile.getPath()};
        Main.main(args);
        String expected = "-b -ph -Bpheph -ephph";
        String actual = getFileContent(inputFile.getPath());
        assertEquals("The files differ!", expected, actual);
        assertTrue(Files.exists(Paths.get(inputFile.getPath() + ".bck")));
    }

    //Purpose: Testing edge cases for D3T2
    //Testing multiple replaces with case insensitive and different replace positions
    //Type d: found a bug when multiple replaces are performing at the same time and option is used.
    //This could be because the ' ' is not treated as a regular string.
    @Test
    public void mainTest75() throws Exception {
        File inputFile = createInputFile6();
        String args[] = {"-l", "ab", " ", " ", "x y z", inputFile.getPath()};
        Main.main(args);
        String expected = "ababx y z";
        String actual = getFileContent(inputFile.getPath());
        assertEquals("The files differ!", expected, actual);
    }
}
