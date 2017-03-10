/*
 * Author: Huy Pham
 * Created date: 03/01/2016
 * Program name: FtClient.java
 * Description: This is a client file transfer program. The program is using TCP protocol
 * to connect to a file transfer server.
 *
 * Credits: 
 * (1) - The program format is closely followed the same format as the Client Socket example from
 * Oracle (http://docs.oracle.com/javase/tutorial/networking/sockets/clientServer.html)
 * (2) - File writer logic from
 * http://www.mkyong.com/java/how-to-write-to-file-in-java-bufferedwriter-example/
 *
 * Compile the program using:
 * 	javac FtClient.java
 *
 * Run the program using:
 * 	java FtClient [host] [send_port_number] [-l] [receive_port_number]
 * 	java FtClient [host] [send_port_number] [-g] [filename] [receive_port_number]
 */ 

import java.net.*;
import java.io.*;
import java.util.Scanner;

public class FtClient{
    public static void main(String[] args) throws IOException {
 
		String localHost;
		String destinationHost;
		String cmd;
		String fileName = "foo";
		int inPortNum = -1;
		int outPortNum = -1;
		int cmdType = -1;
		// validate number of arguments
		if (args.length < 4 || args.length > 5) 
		{
    		System.err.println("Usage: java ftclient [host] [send_port number] [-l|-g] [filename] [receive_port_number]");
       		System.exit(1);
    	}
		// get local and destination hosts	
		localHost = InetAddress.getLocalHost().getHostName();
		destinationHost = args[0];
		inPortNum = Integer.parseInt(args[1]);
		cmd = args[2];
		// validate command input
		if (cmd.equals("-l") && args.length == 4)
		{
			cmdType = 1;
			outPortNum = Integer.parseInt(args[3]);
		}
		else if (cmd.equals("-g") && args.length == 5)
		{		
			cmdType = 2;
			fileName = args[3];
			outPortNum = Integer.parseInt(args[4]);		
		}
		else
		{
			System.out.println("Invalid input. Valid usages are:");
    		System.err.println("  java ftclient [host] [send_port number] [-l] [receive_port_number]");
     		System.err.println("  java ftclient [host] [send_port number] [-g] [filename] [receive_port_number]");
			System.exit(1);	
		}
		try (
			Socket ftSocket = new Socket(destinationHost, inPortNum);
            PrintWriter out = new PrintWriter(ftSocket.getOutputStream(), true);
            BufferedReader in = new BufferedReader(
                new InputStreamReader(ftSocket.getInputStream()));
        ) {
            BufferedReader stdIn =
                new BufferedReader(new InputStreamReader(System.in));
	        String fromServer = "";
            String fromUser = "";
		//	send request
			fromUser = cmdType + "|" + localHost + "|" + outPortNum + "|" + fileName + "|";
			out.println(fromUser);
		//	format response by reading 
			int count;
			char [] buffer = new char[8192];
			StringBuilder strBuilder = new StringBuilder();
			System.out.println("Downloading the file... ");
			while ((count = in.read(buffer)) > 0)
			{
		///		System.out.println("Reading " + count + " bytes from socket");
				for (int i = 0; i < buffer.length; i++)
				{
					strBuilder.append(buffer[i]);
				}
		///		System.out.println("Current file's length: " + strBuilder.length());
			}
		// convert string builder to string
			fromServer = strBuilder.toString();
		// 	parse result using delimiter	
			String delims = "[|]";
			String[] tokens = fromServer.split(delims);
		// response from request	
			if(cmdType == 1)		// list contents request
			{
				System.out.println("Receiving directory structure from " + destinationHost + ":" + outPortNum);
				for (int i = 0; i < tokens.length; i++)
				{
					System.out.println(tokens[i]);
				}
			}
			else					// get file request
			{
				int rc = Integer.parseInt(tokens[0]);
				if(rc == 1)	//success
				{
					System.out.println("Receiving '" + fileName + "' from " + destinationHost + ":" + outPortNum);
					// (2) //
					File outFile = new File(fileName);
					
					if (outFile.exists())		//file exists
					{
						System.out.println("'" + fileName + "' already exists. Do you want to replace the old file?(Y/N)");
						Scanner reader = new Scanner(System.in);
						String response = reader.nextLine();
						if(response.equals("Y") || response.equals("y"))	// process with replace old file
						{
							outFile.createNewFile();
							FileWriter fw = new FileWriter(outFile.getAbsoluteFile());
							BufferedWriter bw = new BufferedWriter(fw);
							for (int i = 1; i < tokens.length; i++)
							{
								bw.write(tokens[i]);	
							}	
							bw.close();
							System.out.println("File transfer complete.");
						}
						else												// cancel transfer
						{
							System.out.println("File transfer cancel.");
						}	
					}
					else						//file does not exist
					{
						outFile.createNewFile();
						FileWriter fw = new FileWriter(outFile.getAbsoluteFile());
						BufferedWriter bw = new BufferedWriter(fw);
						for (int i = 1; i < tokens.length; i++)
						{
							bw.write(tokens[i]);	
						}	
						bw.close();
						System.out.println("File transfer complete.");
					}
/*
					FileWriter fw = new FileWriter(outFile.getAbsoluteFile());
					BufferedWriter bw = new BufferedWriter(fw);
					for (int i = 1; i < tokens.length; i++)
					{
						bw.write(tokens[i]);	
					}	
					bw.close();
					System.out.println("File transfer complete.");
*/
				}
				else		// file not found
				{
					System.out.println(destinationHost + ":" + outPortNum + " says " + tokens[1]);
				}
			}
        } catch (UnknownHostException e) {
            System.err.println("Don't know about host " + destinationHost);
            System.exit(1);
        } catch (IOException e) {
            System.err.println("Couldn't get I/O for the connection to " +
                destinationHost + " port " + inPortNum);
            System.exit(1);
        }
	}
}
