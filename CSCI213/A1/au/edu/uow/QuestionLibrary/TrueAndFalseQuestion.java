/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package au.edu.uow.QuestionLibrary;

import java.util.*;

/**
 * True and false question for quiz
 * @author Shannon Archer, 4220912, sja998
 */
public class TrueAndFalseQuestion implements Question {
    // properties
    // ==========================
    protected List<String>  text;
    protected Boolean       answer;
    
    // constructor
    // ==========================
    /**
     * default constructor
     */
    public TrueAndFalseQuestion () {
        text = new LinkedList<String>();
    }
        
    // methods
    // ==========================
    /**
     * This method returns the question text.
     * @return The question text as a list
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
       List<String> choices = new LinkedList();
       
       choices.add("True");
       choices.add("False");
       
       return choices;
   }

   /**
    * This method compares the student's answer to the standard answer. 
    * @param ans The student's answer
    * @return True for the correct answer; false for incorrect answers.
    */
   @Override
   public boolean compareAnswer(int ans) {
       return ((answer ? 1 : 2) == ans);
   }
    
    
    // getters / setters
    // ==========================
    public void    addText(String value) { text.add(value); }
    
    public Boolean getAnswer() { return answer; }
    public void    setAnswer(Boolean value) { answer = value; } 
    
}
