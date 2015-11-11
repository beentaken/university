/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package au.edu.uow.UserInterface;

import java.util.*;
import java.io.*;
import javax.swing.*;
import javax.swing.event.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.border.*;
import au.edu.uow.QuestionLibrary.*;
import java.net.*;
//import Client.*;

/**
 * User Interface for quiz taking
 * @author Shannon Archer, 4220912, sja998
 */
public class UserInterface extends JFrame implements WindowListener, 
                                                    WindowFocusListener,
                                                    WindowStateListener {
    // properties
    // ==========================
    private Student student;
   // private JavaQuizClient client;
    private Properties config;
    private final int width, height, numQuestions;
    private int currentChoice, questionNo = -1;
    private Question currentQuestion;

    private Socket socket;
    private String hostname;
    private int port;

    private JLabel statusLbl;

    // constructor
    // ==========================
    /**
     * default constructors
     */
    public UserInterface (String hostname, int port) {

        this.hostname = hostname;
        this.port = port;

        // get properties
        config = new Properties();

        try {
            // get properties for the system
            FileInputStream fin = new FileInputStream("quiz.conf");
            config.load(fin);
            fin.close();

        } catch (Exception ex) {

            System.err.println("Error @JavaQuizGUI(  ): " + ex);

        }


        // settings
        width = Integer.parseInt(config.getProperty("window.width"));
        height = Integer.parseInt(config.getProperty("window.height"));
        numQuestions = Integer.parseInt(config.getProperty("quiz.numQuestions"));

        //quizfile = config.getProperty("quiz.filename");

        try {
            String lookandfeel = config.getProperty("window.lookandfeel");
            UIManager.setLookAndFeel(lookandfeel);
        } catch (Exception ex) {

        }

        // create view
        this.setTitle("Java Quiz GUI");
        this.setSize(width, height);
        this.setLocationRelativeTo(null);
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
         
        // init student
        create();   
        
        this.setVisible(true);

    }


    public void create() {
        final UserInterface that = this;

        this.setLayout(new BorderLayout());

        // create toolbar with options
        JMenuBar menuBar = new JMenuBar();

        JMenu menu = new JMenu("Connection");
        menu.setMnemonic(KeyEvent.VK_C);
        menuBar.add(menu);

        final JMenuItem connectMI = new JMenuItem("Connect");
        connectMI.setMnemonic(KeyEvent.VK_E);
        menu.add(connectMI);

        final JMenuItem disconnectMI = new JMenuItem("Disconnect");
        disconnectMI.setMnemonic(KeyEvent.VK_D);
        disconnectMI.setEnabled(false);
        menu.add(disconnectMI);

        menu.addSeparator();

        final JMenuItem setServerMI = new JMenuItem("Set Server");
        setServerMI.setMnemonic(KeyEvent.VK_S);
        menu.add(setServerMI);

        menu.addSeparator();

        final JMenuItem exitMI = new JMenuItem("Exit");
        exitMI.setMnemonic(KeyEvent.VK_X);
        menu.add(exitMI);

        final JMenu helpMI = new JMenu("Help");
        helpMI.setMnemonic(KeyEvent.VK_H);
        menuBar.add(helpMI);

        // create pane for content
        final JPanel contentPnl = new JPanel();
        contentPnl.setBackground(new Color(43, 136, 134));
        contentPnl.setLayout(new GridBagLayout());

        GridBagConstraints c = new GridBagConstraints();

        // create splash screen
        JLabel titleLbl = new JLabel("Java Quiz Client");
        titleLbl.setForeground(Color.white);
        titleLbl.setFont(titleLbl.getFont().deriveFont(32.0f));

        c.gridy = 0;

        contentPnl.add(titleLbl, c);

        JLabel creditLbl = new JLabel("created by Shannon Archer");
        creditLbl.setForeground(Color.black);
        creditLbl.setFont(titleLbl.getFont().deriveFont(20.0f));

        c.gridy = 1;

        contentPnl.add(creditLbl, c);

        creditLbl = new JLabel("for");
        creditLbl.setForeground(Color.black);
        creditLbl.setFont(titleLbl.getFont().deriveFont(20.0f));

        c.gridy = 2;

        contentPnl.add(creditLbl, c);

        creditLbl = new JLabel("CSCI213 Assignment 4");
        creditLbl.setForeground(Color.black);
        creditLbl.setFont(titleLbl.getFont().deriveFont(20.0f));

        c.gridy = 3;

        contentPnl.add(creditLbl, c);


        // add register panel
        final JPanel registerPnl = new JPanel();
        registerPnl.setForeground(Color.white);
        registerPnl.setPreferredSize(new Dimension(this.getWidth(), 50));
        registerPnl.setLayout(new GridBagLayout());

        JLabel nameLbl = new JLabel("Your Name: ");
        registerPnl.add(nameLbl);

        final JTextField nameTxt = new JTextField();
        nameTxt.setPreferredSize(new Dimension(120, 25));
        registerPnl.add(nameTxt);

        // add status panel
        JPanel statusPnl = new JPanel();
        statusPnl.setPreferredSize(new Dimension(this.getWidth(), 30));
        statusPnl.setLayout(new BoxLayout(statusPnl, BoxLayout.X_AXIS));
        statusPnl.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createLineBorder(Color.black, 1, true), new EmptyBorder(4, 2, 4, 2)));

        statusLbl = new JLabel("Connect to the server first.");
        statusLbl.setHorizontalAlignment(SwingConstants.LEFT);
        statusPnl.add(statusLbl);

        // footer panel
        final JPanel footerPnl = new JPanel(new BorderLayout());
        footerPnl.add(registerPnl, BorderLayout.PAGE_START);
        footerPnl.add(statusPnl, BorderLayout.PAGE_END);

        // add in events for it
        connectMI.addActionListener(new ActionListener() {
            // 
            public void actionPerformed(ActionEvent e) {
                if (nameTxt.getText().length() > 0) {
                    if (that.connect(nameTxt.getText())) {
                        connectMI.setEnabled(false);
                        disconnectMI.setEnabled(true);
                        setServerMI.setEnabled(false);
                        nameTxt.setEnabled(false);

                        // display first question
                        statusLbl.setText("Connected to " + hostname + ":" + port);
                        displayQuestion(contentPnl, footerPnl, registerPnl, next());
                    } else {
                        JOptionPane.showMessageDialog(that, "Sorry, can't connect to server at this time");
                    }
                } else {
                    JOptionPane.showMessageDialog(that, "Please enter your name first");
                }
            }
        });

        disconnectMI.addActionListener(new ActionListener() {
            // 
            public void actionPerformed(ActionEvent e) {
                that.disconnect();
                connectMI.setEnabled(true);
                disconnectMI.setEnabled(false);
                setServerMI.setEnabled(true);
                nameTxt.setEnabled(true);

                statusLbl.setText("Disconnected");
            }
        });

        setServerMI.addActionListener(new ActionListener() {
            // 
            public void actionPerformed(ActionEvent e) {
                String serverPort = JOptionPane.showInputDialog(that, "Server:Port", hostname + ":" + port);                

                if (serverPort != null && serverPort.length() > 0) {
                    String[] settings = serverPort.split(":");

                    // set ports
                    hostname = settings[0];
                    port = Integer.parseInt(settings[1]);
                }
            }
        });

        exitMI.addActionListener(new ActionListener() {
            // 
            public void actionPerformed(ActionEvent e) {
                that.exit();
            }
        });

        helpMI.addMenuListener(new MenuListener() {
            // 
            public void menuSelected(MenuEvent e) {
                JOptionPane.showMessageDialog(that, "Java Quiz Client Ver 1.0 based on Java Sockets by Shannon Archer");
            }

            //
            public void menuDeselected(MenuEvent e) {

            }

            //
            public void menuCanceled(MenuEvent e) {

            }
        });

        this.add(menuBar, BorderLayout.PAGE_START);
        this.add(footerPnl, BorderLayout.PAGE_END);
        this.add(contentPnl, BorderLayout.CENTER);
    }

    public void windowClosing (WindowEvent e) {
        disconnect();
    }

    public void windowClosed (WindowEvent e) {

    }

    public void windowOpened (WindowEvent e) {

    }

    public void windowIconified(WindowEvent e) {

    }

    public void windowDeiconified(WindowEvent e) {

    }

    public void windowActivated(WindowEvent e) {

    }

    public void windowDeactivated(WindowEvent e) {

    }

    public void windowGainedFocus(WindowEvent e) {

    }

    public void windowLostFocus(WindowEvent e) {

    }

    public void windowStateChanged(WindowEvent e) {

    }






    public boolean connect (String name) {
        
        try {
            socket = new Socket(hostname, port);

            // send register command
            String command = "REGISTER " + name;

            OutputStream os = socket.getOutputStream();
            ObjectOutputStream out = new ObjectOutputStream(os);
            out.writeObject(command);

            // get OK message
            InputStream is = socket.getInputStream();
            ObjectInputStream in = new ObjectInputStream(is);

            String response = (String)in.readObject();
            if (response.indexOf("OK") != -1) {
                student = new Student();
                student.setName(name);
                return true;
            }
        } catch (Exception ex) {

        }

        return false;
    }

    public void disconnect () { 
        if (socket != null) {
            try {
                // send bye message
                String command = "BYE";

                OutputStream os = socket.getOutputStream();
                ObjectOutputStream out = new ObjectOutputStream(os);
                out.writeObject(command);
             
                // close socket
                socket.close();

                statusLbl.setText("Disconnected");
            } catch (Exception ex) {

            }
        }
    }

    public void setServer (String details) {

    }

    public void exit () {
        System.exit(0);
    }

    public void submit (int answer) {
        boolean correct = currentQuestion.compareAnswer(currentChoice + 1);
        student.recordScore(correct);
    }

    public Question next() {
        if (questionNo < 4) {
            try {
                // send register command
                String command = "GET QUESTION";

                OutputStream os = socket.getOutputStream();
                ObjectOutputStream out = new ObjectOutputStream(os);
                out.writeObject(command);

                // get question message
                InputStream is = socket.getInputStream();
                ObjectInputStream ois = new ObjectInputStream(is);

                Question question;
                question = (Question)ois.readObject();

                currentQuestion = question;

                return question;            

            } catch (Exception ex) {
                System.out.println("ERROR WHEN GETTING QUESTION: " + ex);
            }
        }
            return null;
    }

    public void displayQuestion(final JPanel panel, final JPanel footer, final JPanel next, Question q) {
        final UserInterface that = this;

        questionNo++;
        if (questionNo < 5 && q != null) {
            panel.removeAll();
            panel.updateUI();
            panel.setLayout(new GridLayout(2, 1));

            // build panel
            JTextPane questiontext = new JTextPane();      
            questiontext.setMargin(new Insets(20, width / 2 - 200, 0, 0));
            questiontext.setEditable(false);
            panel.add(questiontext); 

            // add question text
            String question = "";
            for (String s : q.getQuestion()) {
                question += s + '\n';
            }

            questiontext.setText(question);
            
            JPanel answers = new JPanel();
            answers.setLayout(new BoxLayout(answers, BoxLayout.Y_AXIS));  
            answers.setBorder(new EmptyBorder(20, width / 2 - 200, 0, 0));     
            panel.add(answers);

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

            JButton nextBtn;
            if (questionNo == 4)
                nextBtn = new JButton("Get Marks");
            else
                nextBtn = new JButton("Next");
            next.removeAll();
            next.updateUI();
            next.setLayout(new GridBagLayout());
            next.add(nextBtn);

            nextBtn.addActionListener(new ActionListener() {
                public void actionPerformed(ActionEvent e) {
                    that.submit(currentChoice);
                    that.displayQuestion(panel, footer, next, that.next());
                }
            });
        } else {
            finishQuiz(panel, footer, next);
        }
    }

    public void finishQuiz (JPanel panel, JPanel footerPanel, JPanel btnPanel) {

        footerPanel.remove(btnPanel);
        footerPanel.updateUI();

        panel.removeAll();
        panel.updateUI();
        panel.setBackground(new Color(43, 136, 134));
        panel.setLayout(new GridBagLayout());

        GridBagConstraints c = new GridBagConstraints();

        // create splash screen
        JLabel titleLbl = new JLabel("Final Score");
        titleLbl.setForeground(Color.white);
        titleLbl.setFont(titleLbl.getFont().deriveFont(32.0f));

        c.gridy = 0;

        panel.add(titleLbl, c);

        JLabel creditLbl = new JLabel(student.getScore() + " out of 5");
        creditLbl.setForeground(Color.black);
        creditLbl.setFont(titleLbl.getFont().deriveFont(20.0f));

        c.gridy = 1;

        panel.add(creditLbl, c);

        this.disconnect();
    }
}
