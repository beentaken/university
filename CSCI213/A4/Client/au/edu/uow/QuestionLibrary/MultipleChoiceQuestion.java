/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package au.edu.uow.QuestionLibrary;

import java.io.Serializable;
import java.util.*;

/**
 * A multiple choice question for a quiz
 * @author Shannon Archer, 4220912, sja998
 */
public class MultipleChoiceQuestion extends Question {
    // properties
    // ==========================
    protected List<String> text;
    protected int          answer;
    
    protected List<String> choices;
    
    // constructor
    // ==========================
    /**
     * Default constructor
     */
    public MultipleChoiceQuestion () {
        text = new LinkedList<String>();
        choices = new LinkedList<String>();
    }
        
    // methods
    // ==========================
    /**
     * This method returns the question text.
     * @return The question text in a list
    */
    @Override
    public List<String> getQuestion() {
        return text;
    }

   /**
    * This method returns the multiple choices. 
    * @return The list of choices
    */
   @Override
   public List<String> getChoices() {
       return choices;
   }

   /**
    * This method compares the student's answer to the standard answer. 
    * @param ans The student's answer
    * @return True for the correct answer; false for incorrect answers.
    */
   @Override
   public boolean compareAnswer(int ans) {
       return (answer == ans);
   }
   
   /**
    * Add a choice to the question
    * @param text  The text for the choice
    */
   public void addChoice (String text) {
       this.choices.add(text);
   }
   
    // getters / setters
    // ==========================
    public void   addText(String value) { text.add(value); }
    
    public int    getAnswer() { return answer; }
    public void   setAnswer(int value)  { answer = value; }   
}
