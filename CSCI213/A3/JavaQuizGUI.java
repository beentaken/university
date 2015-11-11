/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author Shannon Archer, 4220912, sja998
 */
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.border.*;

import java.util.*;
import java.io.*;

import au.edu.uow.QuestionLibrary.*;
import au.edu.uow.UserInterfaceGUI.*;

public class JavaQuizGUI extends JFrame {
    
    public static void main (String[] args) {
        new JavaQuizGUI();
    }    
    
    // view
    private JButton scoreButton, registerButton, nextButton;
    private JTextField nametext;
    private JToolBar toolbar;
    private JTextPane questiontext;    
    private JPanel main, answers, bottom;
    
    // model
    private Student student;    
    private java.util.List<Question> quiz;
    
    // settings
    private Properties config;
    private final int width, height, numQuestions;
    private int currentQuestion, currentChoice;
    private final String quizfile;
    
    public JavaQuizGUI() {
    	// get properties
    	config = new Properties();

    	try {
			// get properties for the system
			FileInputStream fin = new FileInputStream("JavaQuizGUI.conf");
			config.load(fin);
			fin.close();

		} catch (Exception ex) {

			System.err.println("Error @JavaQuizGUI(  ): " + ex);

		}


		// settings
		width = Integer.parseInt(config.getProperty("window.width"));
		height = Integer.parseInt(config.getProperty("window.height"));
		numQuestions = Integer.parseInt(config.getProperty("quiz.numQuestions"));

		quizfile = config.getProperty("quiz.filename");

		try {
			String lookandfeel = config.getProperty("window.lookandfeel");
			UIManager.setLookAndFeel(lookandfeel);
		} catch (Exception ex) {

		}

        // create quiz data
        QuestionLibrary.buildLibrary(quizfile);
                
        // create view
        this.setTitle("Java Quiz GUI");
        this.setSize(width, height);
        this.setLocationRelativeTo(null);
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        
        generateRegistration();        
        
        this.setVisible(true);
    }
    
    private void generateRegistration() {
        final JFrame that = this;
        
        JPanel content = new JPanel(new BorderLayout());
        
        JPanel centerPanel = new JPanel(new FlowLayout());
        
        JLabel label = new JLabel("Your Name:");
        centerPanel.add(label);
        
        nametext = new JTextField(20);
        nametext.addKeyListener(new KeyAdapter() {
            @Override
           public void keyReleased(KeyEvent e) {
               //
               if (e.getKeyCode() == KeyEvent.VK_ENTER) {
                   if (((JTextField)e.getSource()).getText().length() <= 0)
                        JOptionPane.showMessageDialog(that, "Name needs to be longer than 0 characters.");
                   else
                       register(((JTextField)e.getSource()).getText());
               }
           } 
            
           @Override
           public void keyTyped(KeyEvent e) {
               //
           }
           
           @Override
           public void keyPressed(KeyEvent e) {
               
           }
        });
        centerPanel.add(nametext);
        
        registerButton = new JButton("Register");
        
        registerButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (nametext.getText().length() <= 0)
                    JOptionPane.showMessageDialog(that, "Register your name first.");
                else {
                    // register name
                    if (nametext.getText().length() <= 0)
                        JOptionPane.showMessageDialog(that, "Name needs to be longer than 0 characters.");
                   else
                       register(nametext.getText());
                }
            }
        });
        centerPanel.add(registerButton);
        
        content.add(centerPanel, BorderLayout.CENTER);
        
        JPanel leftPanel = new JPanel(new FlowLayout());
        
        scoreButton = new JButton("Score");
        // add click handler
        scoreButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                JOptionPane.showMessageDialog(that, "Register your name first.");
            }
        });
        leftPanel.add(scoreButton);
        
        JButton button = new JButton("Exit");
        // add click handler
        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent evt) {
                System.exit(0);
            }
        });
        leftPanel.add(button);
        
        content.add(leftPanel, BorderLayout.LINE_START);
        
        toolbar = new JToolBar();
        toolbar.add(content);
        
        main = new JPanel(new GridBagLayout());
        GridBagConstraints c = new GridBagConstraints();
        
        label = new JLabel("Java Quiz");
        label.setForeground(Color.BLUE);
        label.setFont(new Font(label.getFont().getName(), Font.BOLD, 24));
        
        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridx = 0;
        c.gridy = 0;
        c.insets = new Insets(10, 0, 0, 0);
        
        main.add(label, c);
        
        label = new JLabel("created by");
        
        c.gridx = 0;
        c.gridy = 1;
        c.insets = new Insets(0, 20, 0, 0);
        
        main.add(label, c);
        
        label = new JLabel("Shannon Archer");
        
        c.gridx = 0;
        c.gridy = 2;
        c.insets = new Insets(0, 5, 0, 0);
        
        main.add(label, c);
        
        this.getContentPane().add(toolbar, BorderLayout.PAGE_START);
        this.getContentPane().add(main, BorderLayout.CENTER);
    }
    
    private void register (final String name) {     
        final JFrame that = this;
        
        // disable old controls
        nametext.setEnabled(false);
        registerButton.setEnabled(false);
        
        // register name
        student = new Student();
        student.setName(name);

        // create quiz
        quiz = QuestionLibrary.makeQuiz(numQuestions);
        currentQuestion = -1;
        
        // add listener to score button
        for (ActionListener al : scoreButton.getActionListeners()) {
            scoreButton.removeActionListener(al);
        }
        scoreButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                // show dialog with score
                JOptionPane.showMessageDialog(that, "Current score of " + student.getName() + " is " + student.getScore());
            }
        });
        
        // create quiz gui
        this.remove(main);
        main.removeAll();
        main = new JPanel(new GridLayout(2,1));        
        
        questiontext = new JTextPane();      
        questiontext.setMargin(new Insets(20, width / 2 - 200, 0, 0));
        questiontext.setEditable(false);
        main.add(questiontext); 
        
        answers = new JPanel();
        answers.setLayout(new BoxLayout(answers, BoxLayout.Y_AXIS));  
        answers.setBorder(new EmptyBorder(20, width / 2 - 200, 0, 0));     
        main.add(answers);
                
        this.add(main, BorderLayout.CENTER);
                
        bottom = new JPanel(new GridBagLayout());

        GridBagConstraints c = new GridBagConstraints();

        c.gridx = 0;
        c.gridy = 0;
        c.insets = new Insets(0, 0, 20, 0);

        nextButton = new JButton("Next");   
        nextButton.addActionListener(new ActionListener() {
        	@Override
        	public void actionPerformed(ActionEvent e) {
        		submitQuestion();
        		nextQuestion();
        	}
        });
        bottom.add(nextButton, c);

        this.add(bottom, BorderLayout.PAGE_END);
        
        this.setVisible(true);

        nextQuestion();
    }

    private void submitQuestion() {
    	if (currentQuestion < quiz.size()) {
	    	boolean correct = quiz.get(currentQuestion).compareAnswer(currentChoice + 1);
	    	student.recordScore(correct);
	    }
    }

    private void nextQuestion () {
       	currentQuestion++;
    	if (currentQuestion < quiz.size()) {
    		if (currentQuestion == (quiz.size() - 1)) {
    			nextButton.setText("Get Marks");
    		} 

	    	Question q = quiz.get(currentQuestion);

	    	// add question text
	    	String question = "";
	    	for (String s : q.getQuestion()) {
	    		question += s + '\n';
	    	}

	    	questiontext.setText(question);

	    	// add choices
	    	answers.removeAll();
	    	answers.updateUI();

	    	ButtonGroup group = new ButtonGroup();
	    	for (int i = 0; i < q.getChoices().size(); i++) {
	    		JRadioButton rb = new JRadioButton(q.getChoices().get(i));
	    		if (i == 0)
	    			rb.setSelected(true);
	    		rb.setActionCommand(i + "");

	    		rb.addActionListener(new ActionListener() {
	    			@Override
	    			public void actionPerformed(ActionEvent e) {
	    				currentChoice = Integer.parseInt(e.getActionCommand());
	    			}
	    		});

	    		group.add(rb);
	    		answers.add(rb);
	    	}
	    } else {
	    	finishQuiz();
	    }
    }

    private void finishQuiz() {
    	// remove everything
    	toolbar.setVisible(false);
    	main.removeAll();
    	main.setLayout(new GridBagLayout());
    	main.updateUI();

    	// change next button to exit
        for (ActionListener al : nextButton.getActionListeners()) {
            nextButton.removeActionListener(al);
        }
        nextButton.setText("Exit");
        nextButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                // show dialog with score
                System.exit(0);
            }
        });

    	// add label
        GridBagConstraints c = new GridBagConstraints();

        c.fill = GridBagConstraints.HORIZONTAL;
        c.gridx = 0;
        c.gridy = 0;

    	JLabel label = new JLabel("Result of " + student.getName() + ": " + student.getScore() + " out of " + numQuestions);
    	main.add(label, c);
    	main.setVisible(true);

    }
    
}
