/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package au.edu.uow.UserInterface;

/**
 * Records student data related to quiz
 * @author Shannon Archer, 4220912, sja998
 */
public class Student {
    // properties
    // ==========================
    private String name;
    private int    score;
    private int    totalAnswered;
    
    // constructor
    // ==========================
    /**
     * default constructor
     */
    public Student () {
        this("");
    }
    
    /**
     * create student with name
     * @param name  The name of the student
     */
    public Student (String name) {
        this(name, 0);
    }
    
    /**
     * create student with name and starting score
     * @param name The name of the student
     * @param score  The starting score of the student
     */
    public Student (String name, int score) {
        this.name = name;
        this.score = score;
        
        this.totalAnswered = 0;
    }
    
    // methods
    // ==========================
    /**
     * Increment score when question answered correctly
     * @param isCorrect True if question answered correctly; false if answered incorrectly
     */
    public void recordScore (boolean isCorrect) {
        if (isCorrect) 
            score++;
        totalAnswered++;
    }
    
    // getters / setters
    // ==========================
    public String getName() { return name; }
    public void   setName(String value) { name = value; }
    
    public int    getScore() { return score; }
    
    public int    getTotalAnswered() { return totalAnswered; }
}
