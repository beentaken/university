package au.edu.uow.QuestionDB;
import au.edu.uow.QuestionLibrary.*;

import java.util.*;
import java.io.*;
import java.sql.*;

/**
 *       
 * @author Shannon Archer
 * @version v.1.0
 *
 */
public class MyQuestionDB implements QuestionDB {

	private Connection connection;

	/**
	 * Default constructor that creates database and inits 
	 * table for questions
	 */
	public MyQuestionDB () {
		try {
			// get properties from file
			Properties props = new Properties();

			FileInputStream in = new FileInputStream("database.properties");	
			props.load(in);
			in.close();

			// get drivers
			String drivers = props.getProperty("jdbc.drivers");
			if (drivers != null) 
				System.setProperty("jdbc.drivers", drivers);

			String url = props.getProperty("jdbc.url");	

			// create db
			connection = DriverManager.getConnection(url);

			String createTableSql = 
			"CREATE TABLE Questions (Q_ID INT NOT NULL GENERATED ALWAYS AS " +
						"IDENTITY (START WITH 1, INCREMENT BY 1), " +
						"question CLOB, " +
						"choices CLOB, " +
						"answer INT, " +
						"CONSTRAINT primary_key PRIMARY KEY (Q_ID)) ";
			
			Statement statement = connection.createStatement();
			statement.execute(createTableSql);

			// close statement object
			if (statement != null)
				statement.close();
			
		} catch (Exception ex) {

			System.out.println(ex);

		}
	}
	
	/**
	 * This method creates and populates a table in the database 
	 *   storing the questions from the question file. This method 
	 *   should use the addQuestion method to insert question content 
	 *   into the database
	 * @param questionFile The file name of the question file
	 * @return True if the database is successfully populated
	 * @see #addQuestion(Question)
	 */
	public boolean buildQuestionDB(String questionFile) {
		BufferedReader br = null;
          
        // begin read
		try {
		    String line;
		      
		    br = new BufferedReader(new FileReader(questionFile));
		      
		    for (int i = 0; (line = br.readLine()) != null; i++) {
		        if (i < 2)
		            continue;
		          
		        // get type of question
		        if (line.equals("<MQuestion>")) {
				LinkedList<String> mquestion = new LinkedList<String>();
				LinkedList<String> mchoices  = new LinkedList<String>();
				int manswer = 0;

		                // get question
		                while ((line = br.readLine()) != null) {
		                    if (line.equals("<question>")) {
		                        while ((line = br.readLine()) != null) {
		                            // quit if end of question
		                            if (line.equals("</question>"))
		                                break;
		                            mquestion.add(line);
		                        }
		                    } else if (line.equals("<answer>")) {
		                        while ((line = br.readLine()) != null) {
		                            // quit if end of answer
		                            if (line.equals("</answer>"))
		                                break;
		                            manswer = Integer.parseInt(line);
		                        }
		                    } else if (line.equals("<choices>")) {
		                        while ((line = br.readLine()) != null) {
		                            // quit if end of choices
		                            if (line.equals("</choices>"))
		                                break;
		                            // add new choice to question
		                            mchoices.add(line);
		                        }
		                    } else if (line.equals("</MQuestion>")) {
		                        break;
		                    }
		                }
		                addQuestion(new MultipleChoiceQuestion(mquestion, mchoices, manswer));
			} else if (line.equals("<TFQuestion>")) {
			        // create question
				LinkedList<String> tfquestion = new LinkedList<String>();
				String tfanswer = "";

			        // get question
			        while ((line = br.readLine()) != null) {
			            if (line.equals("<question>")) {
			                while ((line = br.readLine()) != null) {
			                    // quit if end of question
			                    if (line.equals("</question>"))
			                        break;
			                    tfquestion.add(line);
			                }
			            } else if (line.equals("<answer>")) {
			                while ((line = br.readLine()) != null) {
			                    // quit if end of answer
			                    if (line.equals("</answer>"))
			                        break;
			                    tfanswer = line;
			                }
			            } else if (line.equals("</TFQuestion>")) {
			                break;
			            }
			        }
			        addQuestion(new TrueAndFalseQuestion(tfquestion, tfanswer));
				}
		    }
		} catch (IOException e) {

		    System.out.println(e.toString());
		    return false;

		}

		return true;
	}
	
	/**
	 * This method returns the total number of questions in the database
	 * @return The total number of questions in the question database
	 * @see #buildQuestionDB
	 */
	public int getTotalNumberOfQuestions() {

		// build query
		String query = "SELECT COUNT(*) FROM Questions";

		// perform query
		Statement statement;

		try {

			statement = connection.createStatement();
			ResultSet results = statement.executeQuery(query);

			results.next();

			// return count
			return results.getInt(1);

		} catch (Exception ex) {

			System.out.println("Exception @getTotalNumberOfQuestions: " + ex);

		}

		return 0;
	}

	/**
	 * This method returns the question from the database at the given position
	 * @param questionIndex The index of the question in the database
	 * @return The question object
	 */
	public Question getQuestion(int questionIndex) {

		// 
		String query = "SELECT * FROM Questions WHERE Q_ID = " + questionIndex;

		Statement statement;

		try {
			
			statement = connection.createStatement();

			ResultSet results = statement.executeQuery(query);

			// get first result
			while (results.next()) {

				List<String> qtext;
				List<String> choices;
				int answer = 0;

				String[] qstr = results.getString("question").split("#ENDLINE");
				String[] cstr = results.getString("choices").split("#ENDLINE");

				qtext    = Arrays.asList(qstr);
				choices  = Arrays.asList(cstr);

				answer   = results.getInt("answer");

				// return true and false question if question has 2 choices
				if (choices.size() == 2) {
					return new TrueAndFalseQuestion(qtext, (answer == 0 ? "true" : "false"));
				} 
				// return multiple choice question by default
				else {
					return new MultipleChoiceQuestion(qtext, choices, answer);
				}

			}

			if (statement != null)
				statement.close();

		} catch (Exception ex) {

			System.out.println("Exception: @makeQuiz(int)\n" + ex);
			return null;

		}

		return null;
	}
	
	/**
	 * This method adds a question to the database
	 * @param question The question object to be added to the database
	 * @return True if the operation is successful
	 */
	public boolean addQuestion(Question question) {

		// begin query construction
		String query = "INSERT INTO Questions (question, choices, answer) VALUES ('";

		// add question lines delimited by #ENDLINE
		List<String> questionText = question.getQuestion();

		for (int i = 0; i < questionText.size(); i++) {
			query += questionText.get(i) + "#ENDLINE";
		} 

		// seperate questions from choices
		query += "', '";

		// add choices delimited by #ENDLINE
		List<String> choices = question.getChoices();
		for (int i = 0; i < choices.size(); i++) {
			query += choices.get(i) + "#ENDLINE";
		}

		// add answer
		query += "', " + question.getAnswer() + ")";

		// commit to database
		Statement statement;

		try {

			statement = connection.createStatement();
			statement.executeUpdate(query);
			
			if (statement != null)
				statement.close();

		} catch (Exception ex) {

			System.out.println("Exception: @addQuestion(Question)\n" + ex);
			return false;

		}

		return true;
	}
		
	/**
	 * This method removes the created table from the database
	 * @return True if the operation is successful
	 * @see #buildQuestionDB(String)
	 */
	public boolean cleanUpDB() {

		// build query
		String query = "DROP TABLE Questions";

		// perform query
		Statement statement;

		try {

			statement = connection.createStatement();
			statement.executeUpdate(query);

			if (statement != null) 
				statement.close();

		} catch (Exception ex) {

			System.out.println("Exception: @cleanUpDB()\n" + ex);
			return false;

		}

		return true;
	}

	/**
	 * This method makes a quiz from the question database 
	 *   with the number of questions as specified. This method
	 *   should use the getQuestion method to retrieval question
	 *   content from the database
	 * @param noOfQuestions - the number of questions in a quiz
	 * @return Quiz questions in a list
	 * @see #getQuestion(int)
	 */
	public List<Question> makeQuiz(int noOfQuestions) {

		// list to store questions in
		List<Question> questions = new LinkedList<Question>();

		// get all questions
		String query = "SELECT * FROM Questions";

		

		// grab random questions from database
		List<Question> questionSubList = new LinkedList<Question>();
		List<Integer>  ids = new LinkedList<Integer>();

		int numQuestions = getTotalNumberOfQuestions();

		for (int i = 0; i < noOfQuestions && i < numQuestions; i++) {
            long j = (long)(Math.random() * numQuestions);

            Question question = getQuestion((int)j);
            
            while ((ids.contains((int)j) || question == null) && questionSubList.size() < numQuestions) {
                j = (long)(Math.random() * numQuestions);
                question = getQuestion((int)j);
            }            

            if (question != null) {
            	questionSubList.add(question);
            	ids.add((int)j);
            }
        }
        
        return questionSubList;
	}
	
}
