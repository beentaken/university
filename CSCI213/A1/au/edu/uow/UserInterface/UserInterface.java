/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package au.edu.uow.UserInterface;

import java.util.*;
import java.io.*;
import au.edu.uow.QuestionLibrary.*;

/**
 * User Interface for quiz taking
 * @author Shannon Archer, 4220912, sja998
 */
public class UserInterface {
    // properties
    // ==========================
    private Student student;
    
    // constructor
    // ==========================
    /**
     * default constructors
     */
    public UserInterface () {
        // init student
    }
    
    // methods
    // ==========================
    /**
     * Begin quiz process
     * @param quiz The questions belonging to quiz
     * @param student The student taking the quiz
     */
    public void startQuiz ( List<Question> quiz, Student student ) {
        try {
            BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            
            for (int i = 0; i < quiz.size(); i++) {
                System.out.println("\nQuestion No " + (i+1) + ":\n");
                
                List<String> questionText = quiz.get(i).getQuestion();
                for (int j = 0; j < questionText.size(); j++) {
                    System.out.println(questionText.get(j));
                }
                
                System.out.println("\nAnswer choices:");
                List<String> choices = quiz.get(i).getChoices();
                for (int j = 0; j < choices.size(); j++) {
                    System.out.println((j+1) + ": " + choices.get(j));
                }
                
                System.out.print("\nChoose your answer: ");
                int answer = Integer.parseInt(br.readLine());
                
                student.recordScore(quiz.get(i).compareAnswer(answer));
            }
            
            
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    
    /**
     * Display student marks
     * @param student  The student that will have their marks displayed
     */
    public void showStudentMarks ( Student student ) {
        System.out.println("Results of " + student.getName() + ": " + student.getScore() + " out of " + student.getTotalAnswered());
    }
    
    // getters / setters
    // ==========================
    /**
     * "Login" student
     * @return Instance of student
     */
    public Student getStudent () { 
        if (student == null) {
            try {
                // init student
                BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

                System.out.print("You name: ");

                String name = br.readLine();
                student = new Student(name);
                
            } catch (IOException e) {
                e.printStackTrace();
                return null;
            }
        }
        return student; 
    }
}
