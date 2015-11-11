package au.edu.uow.Networking;

import java.net.*;
import java.security.*;
import java.util.*;
import java.io.*;

import au.edu.uow.QuestionLibrary.*;


/**
 * @author Shannon Archer, 4220912, sja998
 */
public class JavaClientHandler implements Runnable {

	private Socket server;
	private List<Question> quiz;
	private int currentQuestion;
	private String name;
 	/**
 	 * default constructor
 	 * @param server The socket to handle communication through
 	 * @param bs     The main server
 	 */
	public JavaClientHandler (Socket server) {
		System.out.println("Connection made");
		this.server = server;

		quiz = QuestionLibrary.makeQuiz(5);
		currentQuestion = -1;
	}

	/**
	 * Run thread to handle client
	 */
	public void run() {

		String line = "", input = "";

		try {

			String request = "";
			while (request.indexOf("BYE") == -1) {

				InputStream is = server.getInputStream();
				ObjectInputStream ois = new ObjectInputStream(is);	

				request = (String)ois.readObject();

				// figure out what type of request it is
				if (request.indexOf("GET QUESTION") != -1) {
					// get appropriate response
					currentQuestion++;
					Question response = null;
					if (currentQuestion < quiz.size()) {
						response = quiz.get(currentQuestion);
					}

					// send response
					OutputStream os = server.getOutputStream();
					ObjectOutputStream oos = new ObjectOutputStream(os);
					oos.writeObject(response);

				} else if (request.indexOf("REGISTER") != -1) {
					// get appropriate response
					String[] command = request.split("\\s+");
					this.name = command[1];
					System.out.println(this.name + " registered.");
					String response = "OK";

					// send response
					OutputStream os = server.getOutputStream();
					ObjectOutputStream oos = new ObjectOutputStream(os);
					oos.writeObject(response);
				}

			}

			System.out.println(this.name + " disconnected.");
			server.close();

		} catch (Exception ex) {
			System.out.println("Exception @JavaClientHandler.run(): " + ex);
		}

	}

}