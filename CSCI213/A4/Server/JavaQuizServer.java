import java.util.*;
import java.io.*;
import java.sql.*;

import java.net.*;
import java.security.*;

import au.edu.uow.Networking.*;
import au.edu.uow.QuestionLibrary.*;

/**
 * Main server class for a4
 * @author Shannon Archer, 4220912, sja998
 */
public class JavaQuizServer {
       
    public static void main (String[] args) {
        new JavaQuizServer(args);
    } 

	private Properties properties;
	private int port = 4444;

	/**
	 * Server Default Constructor
	 * Connects database and begins threaded server
	 */
	public JavaQuizServer (String[] args) {
		properties = new Properties();

		try {
			// get properties for the system
			FileInputStream fin = new FileInputStream("quiz.conf");
			properties.load(fin);
			fin.close();

		} catch (Exception ex) {

			System.out.println("Error @Connect(  ): " + ex);

		}

		if (args.length == 1)
			port = Integer.parseInt(args[0]);

		// run server
		run();
	}

	/*
	 * Run loop to listen for connections
	 */
	public void run () {

		int maxConnections = 0, i = 0;

		QuestionLibrary.buildLibrary("questions.xml");
                   
		try {

			ServerSocket listener = new ServerSocket(port);
			Socket server;

			while ((i++ < maxConnections) || (maxConnections == 0)) {

				JavaClientHandler sHandler;

				server = listener.accept();
				sHandler = new JavaClientHandler(server);

				Thread t = new Thread(sHandler);
				t.start();

			}

		} catch (Exception ex) {

			System.out.println("Error @BaconServer.Run(): " + ex);

		}

	}
}