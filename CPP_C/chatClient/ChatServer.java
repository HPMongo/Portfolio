/*
 * Author: Huy Pham
 * Created date: 02/06/2016
 * Program name: ChatServer.java
 * Description: this is a server chat program. The program is using TCP protocol to
 * listen to chat client program on a predefined port.
 *
 * Credits: the program is closely followed the same format as the Server Socket from
 * Oracle (http://docs.oracle.com/javase/tutorial/networking/sockets/clientServer.html)
 * 
 * compile the program using:
 * 	javac ChatServer.java
 *
 * run the program using:
 * 	java ChatServer <port_number>
 */ 

import java.net.*;
import java.io.*;
import java.util.Scanner;

public class ChatServer {
    public static void main(String[] args) throws IOException {
        
        if (args.length != 1) {
            System.err.println("Usage: java chatserver <port number>");
            System.exit(1);
        }
		
        int portNum = Integer.parseInt(args[0]);

        try ( 
            ServerSocket serverSocket = new ServerSocket(portNum);
            Socket clientSocket = serverSocket.accept();
            PrintWriter out =
                new PrintWriter(clientSocket.getOutputStream(), true);
            BufferedReader in = new BufferedReader(
                new InputStreamReader(clientSocket.getInputStream()));
        ) {
        
            String inputLine, outputLine;
            
            // Initiate conversation with client
   	    Scanner reader = new Scanner(System.in);
///	    out.println("server>Hi there!");
///	    outputLine = reader.nextLine();
///	    out.println(outputLine); 
	    System.out.println("Connected to client");
	    out.println("server>Hi there!");	 
   	    while ((inputLine = in.readLine()) != null) {
    	   // Display client' response
		System.out.println("client>" + inputLine);
	   // Generate reply
   		System.out.print("response>");
		outputLine = reader.nextLine();	
	   // Send response	
		out.println("server>" + outputLine);
                if (outputLine.equals("\\quit"))
		//    System.out.println("Good bye!");
                    break;
            }
        } catch (IOException e) {
            System.out.println("Exception caught when trying to listen on port "
                + portNum + " or listening for a connection");
            System.out.println(e.getMessage());
        }
    }
}
