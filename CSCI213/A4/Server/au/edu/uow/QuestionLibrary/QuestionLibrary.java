/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package au.edu.uow.QuestionLibrary;

import java.util.*;

import java.io.*;

/**
 * Creates a library 
 * @author Shannon Archer, 4220912, sja998
 */
public class QuestionLibrary {
    // properties
    // ==========================
    private static LinkedList<Question> questions = new LinkedList<Question>();
    
    // constructor
    // ==========================
    /**
     * Default constructor
     */
    public QuestionLibrary() {
        
    }
    
    // methods
    // ==========================
    /**
     * loads all questions from a question library file 
     * @param qFile The path to the question library file
     * @return True if read successful; false if unsuccessful
     */
    public static boolean buildLibrary ( String qFile ) {
        BufferedReader br = null;
        
        try {
            String line;
            
            br = new BufferedReader(new FileReader(qFile));
            
            for (int i = 0; (line = br.readLine()) != null; i++) {
                if (i < 2)
                    continue;
                
                // get type of question
                switch (line) {
                    case "<MQuestion>":
                        // create question
                        MultipleChoiceQuestion mquestion = new MultipleChoiceQuestion();
                        // get question
                        while ((line = br.readLine()) != null) {
                            if (line.equals("<question>")) {
                                while ((line = br.readLine()) != null) {
                                    // quit if end of question
                                    if (line.equals("</question>"))
                                        break;
                                    mquestion.addText(line);
                                }
                            } else if (line.equals("<answer>")) {
                                while ((line = br.readLine()) != null) {
                                    // quit if end of answer
                                    if (line.equals("</answer>"))
                                        break;
                                    mquestion.setAnswer(Integer.parseInt(line));
                                }
                            } else if (line.equals("<choices>")) {
                                while ((line = br.readLine()) != null) {
                                    // quit if end of choices
                                    if (line.equals("</choices>"))
                                        break;
                                    // add new choice to question
                                    mquestion.addChoice(line);
                                }
                            } else if (line.equals("</MQuestion>")) {
                                break;
                            }
                        }
                        questions.add(mquestion);
                        break;
                    case "<TFQuestion>":
                        // create question
                        TrueAndFalseQuestion tfquestion = new TrueAndFalseQuestion();
                        // get question
                        while ((line = br.readLine()) != null) {
                            if (line.equals("<question>")) {
                                while ((line = br.readLine()) != null) {
                                    // quit if end of question
                                    if (line.equals("</question>"))
                                        break;
                                    tfquestion.addText(line);
                                }
                            } else if (line.equals("<answer>")) {
                                while ((line = br.readLine()) != null) {
                                    // quit if end of answer
                                    if (line.equals("</answer>"))
                                        break;
                                    tfquestion.setAnswer(Boolean.parseBoolean(line));
                                }
                            } else if (line.equals("</TFQuestion>")) {
                                break;
                            }
                        }
                        questions.add(tfquestion);
                        break;
                }
            }
        } catch (IOException e) {
            System.out.println(e.toString());
            return false;
        }
        
        return true;
    }
    
    /**
     * makes a quiz with the specified number of questions from the question library
     * @param noOfQuestions The size of quiz to generate, can't be greater than library size
     * @return Random set of questions from question library
     */
    public static List<Question> makeQuiz ( int noOfQuestions ) {
        List<Question> questionSubList = new LinkedList<Question>();
        
        for (int i = 0; i < noOfQuestions && i < questions.size(); i++) {
            long j = (long)(Math.random() * questions.size());
            
            while (questionSubList.contains(questions.get((int)j))) {
                j = (long)(Math.random() * questions.size());
            }
            
            questionSubList.add(questions.get((int)j));
        }
        
       return questionSubList; 
    }
    
    // getters / setters
    // ==========================
    
}
