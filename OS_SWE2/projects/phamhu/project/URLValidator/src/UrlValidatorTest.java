/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


import junit.framework.TestCase;
import java.util.Random;


/**
 * Performs Validation Test for url validations.
 *
 * @version $Revision: 1128446 $ $Date: 2011-05-27 13:29:27 -0700 (Fri, 27 May 2011) $
 */
public class UrlValidatorTest extends TestCase {

   private boolean printStatus = false;
   private boolean printIndex = false;//print index that indicates current scheme,host,port,path, query test were using.

   public UrlValidatorTest(String testName) {
      super(testName);
   }

   
   
   public void testManualTest()
   {
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   System.out.println("\n--------------------------");
	   System.out.println("Perform manual testing - http");
	   System.out.println("--------------------------\n");
	   String [] inURL = {"http://www.amazon.com"
			   			 ,"something.com"
			   			 ,"https://www.w3.org/Addressing/URL/url-spec.txt"
			   			 ,"http://www.autotrader.com/cars-for-sale/showcase.xhtml?zip=90210&Log=0&searchRadius=25"
			   			 ,"http://www.autotrader.com/cars-for-sale/Ames+IA-50014?endYear=2017"
			   			 ,"ftp://ftp.funet.fi/pub/standards/RFC/rfc959.txt"
			   			 ,"mailto:someone@example.com?Subject=Hello%20again"
			   			 ,"mailto:someone@example.com"
			   			 ,"file://alpha.hut.fi/u/lai/tik/tik76002/public_html/lerman.files/chaps"
			   			 ,"file://localhost/etc/fstab"
			   			 ,"ftp://public.ftp-servers.example.com/mydirectory/myfile.txt"
			   			 ,"http://example.com/cgi-bin/feed.cgi?feed=news&frm=rss"
			   			 ,"htt://sppace . com"
			   			 ,"ftps://oregonstate.edu:80"}; 
	   boolean [] inValid = {true
	   			 ,false
	   			 ,true
	   			 ,true
	   			 ,true
	   			 ,true
	   			 ,true
	   			 ,true
	   			 ,true
	   			 ,true
	   			 ,true
	   			 ,true
	   			 ,false
	   			 ,false}; 
	   boolean testResult;
	   for (int i =  0; i < inURL.length; i++) {
		   testResult = urlVal.isValid(inURL[i]);
		   if(testResult != inValid[i]) {
			   System.out.println(inURL[i] + " --- test failed");
		   }
	   }
   }
   
   /*
    * Performs Validation Test for http.
    * http URL syntax: "http:" "//" host [ ":" port ] [ abs_path [ "?" query ]]
    */
   public void testYourFirstPartition()
   {
	   UrlValidator testValidator = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   System.out.println("\n--------------------------");
	   System.out.println("Testing first partition - http");
	   System.out.println("--------------------------\n");
	   /* test inputs */
	   String [] scheme = {"http://", "https://", "httpe://"};
	   boolean[] schemeValid = {true, true, false};
	   String [] host = {"amazon.com", "ebay.com", "google.com", "baidu.com", "cars.net", "oregonstate.edu", "foo.com", "bar.com", ".", "..", "./"};
	   boolean [] hostValid = {true, true, true, true, true, true, true, true, false, false, false};
	   String [] port = {":80", ":90", ":102", ":240", " "};
	   boolean [] portValid = {true, true, true, true, false};
	   String [] path = {"/index", "/home", "/directory", "/pages", "", "/ spaces"};
	   boolean [] pathValid = {true, true, true, true, true, false};
	   String [] query = {"?new=1", "?rand='Y'", "?location=90210&color=orange", "", "? space = space"};
	   boolean [] queryValid = {true, true, true, true, false};
	   
	   boolean myTestValidity;
	   
	   for(int i = 0; i < 50; i++) {
		   String[] myURLSet = generateTestURL(scheme, schemeValid, host, hostValid, port, portValid, path, pathValid, query, queryValid);
		   myTestValidity = false;
		   if (myURLSet[0] == "valid") {
			   myTestValidity = true; 
		   }
		   
		   try{
			   assertTrue(myTestValidity==testValidator.isValid(myURLSet[1]));
			   /* 
			  	System.out.print("\""+myURLSet[1]+"\" test passed: ");
				System.out.print(testValidator.isValid(myURLSet[1])+"\n");
			   */
		   } catch(Error e) {
		   		System.out.print("\""+myURLSet[1]+"\" test failed: \n");
		   		System.out.print("URLValidator returned: ");
		   		System.out.print(testValidator.isValid(myURLSet[1])+"\n\n" ); 			   
		   }
	   }
   }
   
   /*
    * Performs Validation Test for ftp.
    * ftp URL syntax: ftp://[user[:password]@]host[:port]/url-path
    * 
    */
   public void testYourSecondPartition()
   {	   
	   UrlValidator testValidator = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   System.out.println("\n--------------------------");
	   System.out.println("Testing second partition - ftp");
	   System.out.println("--------------------------\n");
	   /* test inputs */
	   String [] scheme = {"ftp://", "ftps://", "ftpe://"};
	   boolean[] schemeValid = {true, false, false};
	   String [] userNamePW = {"jon:snow@", "tyrion:lannister@", "color:orange@", "username:password@", "", "onlyname@", " space : pw@"};
	   boolean [] userNamePWValid= {true, true, true, true, true, true, false};
	   String [] host = {"amazon.com", "ebay.com", "google.com", "baidu.com", "cars.net", "oregonstate.edu", "foo.com", "bar.com", ".", "..", "./"};
	   boolean [] hostValid = {true, true, true, true, true, true, true, true, false, false, false};
	   String [] port = {":80", ":90", ":102", ":240", " "};
	   boolean [] portValid = {true, true, true, true, false};
	   String [] path = {"/index", "/home", "/directory", "/pages", "", "/ spaces"};
	   boolean [] pathValid = {true, true, true, true, true, false};

	   
	   boolean myTestValidity;
	   
	   for(int i = 0; i < 50; i++) {
		   String[] myURLSet = generateTestURL(scheme, schemeValid, userNamePW, userNamePWValid, host, hostValid, port, portValid, path, pathValid);
		   myTestValidity = false;
		   if (myURLSet[0] == "valid") {
			   myTestValidity = true; 
		   }
		   
		   try{
			   assertTrue(myTestValidity==testValidator.isValid(myURLSet[1]));
			   /* 
			  	System.out.print("\""+myURLSet[1]+"\" test passed: ");
				System.out.print(testValidator.isValid(myURLSet[1])+"\n");
			   */
		   } catch(Error e) {
		   		System.out.print("\""+myURLSet[1]+"\" test failed: \n");
		   		System.out.print("URLValidator returned: ");
		   		System.out.print(testValidator.isValid(myURLSet[1])+"\n\n" ); 			   
		   }
	   }
   }
   
   /*
    * Performs validation test using predefined test values.
    */
   public void testIsValid()
   {
	   System.out.println("\n--------------------------");
	   System.out.println("Testing testIsValid()");
	   System.out.println("--------------------------\n");
	   UrlValidator testValidator = new UrlValidator();
	   boolean myTestValidity; 
	   /* test inputs */
 	   String[] schemes = {"http://", "https://", "ftp://", "bad://", "http:////"};
 	   boolean[] schemeValid ={ true, true, true, false, false};
 	    	    
 	   String[] domains = {"www.google.com", "cnn.com", "stuff.au", "bad#domain"};
 	   boolean []domainValid = {true, true, true, false};
 	   
 	   
 	   String[] ports = {":80",":8000", "", ":-1"};
 	   boolean[] portValid={true, true, true, false};
 	   
 	   String[] paths ={"","/test1", "/$23", "/test/", "/..", "/#",
 			   "/test1/file", "/$23/file", "/test/file1", "/../file", "/#/file"};
 	   boolean[] pathValid ={true, true, true, true, false, true,
 			   true, true, true, false, true};
 	   
 	   String[] queries = {"", "?view=true", "?id=12345" };
 	   boolean[] queryValid ={true, true, true};
 	   
	   for(int i = 0;i<100;i++)
	   {
		   String[] myURLSet = generateTestURL(schemes, schemeValid, domains, domainValid, ports, portValid, paths, pathValid, queries, queryValid);
		   myTestValidity = false;
		   if (myURLSet[0] == "valid") {
			   myTestValidity = true; 
		   }		   
		   try{
			   assertTrue(myTestValidity==testValidator.isValid(myURLSet[1]));
			   /* 
			  	System.out.print("\""+myURLSet[1]+"\" test passed: ");
				System.out.print(testValidator.isValid(myURLSet[1])+"\n");
			   */
		   } catch(Error e) {
		   		System.out.print("\""+myURLSet[1]+"\" test failed: \n");
		   		System.out.print("URLValidator returned: ");
		   		System.out.print(testValidator.isValid(myURLSet[1])+"\n\n" ); 			   
		   }
	   }
   }
   
   public void testAnyOtherUnitTest()
   {
	   
   }
   /**
    * Create set of tests by taking the testUrlXXX arrays and
    * running through all possible permutations of their combinations.
    *
    * @param testObjects Used to create a url.
    */
   

   /**
    * Returns an array of 2 strings.  The 0th element contains the test
    * URL, and the 2nd contains either "valid" or "invalid."  The URL's
    * are constructed from 5 parts, 
    * @param n
    * @return
    */
   	public String[] generateTestURL(String [] schemes, boolean[] schemeValid, String[] input1, boolean[] input1Valid, String[] input2, boolean[] input2Valid, String[] input3, boolean[] input3Valid, String[] input4, boolean[] input4Valid){
   		
   	/* Listings of all of the test schemes, domains, paths, ports, and queries */
    /* and their separate validity values.*/
 	   String[] returnString = new String[2];
	   
 	   int randScheme = new Random().nextInt(schemes.length);
	   int rand1 = new Random().nextInt(input1.length);
	   int rand2 = new Random().nextInt(input2.length);
	   int rand3 = new Random().nextInt(input3.length);
	   int rand4 = new Random().nextInt(input4.length);
	   	   	   
	   boolean URLValid=true;
	   StringBuilder testURL = new StringBuilder();
	   String URL = ""; 
	   
	   testURL = testURL.append(schemes[randScheme]);
	   URLValid = URLValid && schemeValid[randScheme];

	   testURL = testURL.append(input1[rand1]);
	   URLValid = URLValid && input1Valid[rand1];

	   testURL = testURL.append(input2[rand2]);
	   URLValid = URLValid && input2Valid[rand2];

	   testURL = testURL.append(input3[rand3]);
	   URLValid = URLValid && input3Valid[rand3];

	   testURL = testURL.append(input4[rand4]);
	   URLValid = URLValid && input4Valid[rand4];
	   
	   if (URLValid){
		   returnString[0] = "valid";
	   } else {
		   returnString[0] = "invalid";
	   }
	   
	   URL = testURL.toString();
	   returnString[1] = URL; 
	   return returnString;
   	}
}
