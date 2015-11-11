
import java.net.InetAddress;
import java.net.Socket;
import java.util.Properties;
import java.io.*;

import au.edu.uow.Networking.*;
import au.edu.uow.UserInterface.*;

/**
 * @author Shannon Archer, 4220912, sja998
 */
public class JavaQuizClient {
    
    Properties props;
    UserInterface iface;
    Socket socket;
  
    public static void main (String[] args) {
        new JavaQuizClient(args);
    }  


    public JavaQuizClient(String[] args) {

            // connect to server
            run(args);

    }

    public void run(String[] args) {
        props = new Properties();

        try {
            // get properties for the system
            FileInputStream fin = new FileInputStream("quiz.conf");
            props.load(fin);
            fin.close();

            int port = Integer.parseInt( props.getProperty("server.port") );
            String hostname = InetAddress.getLocalHost().getHostName();    

            if (args.length > 0) {
                String[] settings = args[0].split(":");
                if (settings.length > 1) {
                    hostname = settings[0];
                    port = Integer.parseInt(settings[1]);
                }
                else 
                    hostname = args[0];
            } 

            iface = new UserInterface(hostname, port);

        } catch (Exception ex) {
            System.out.println(ex);
        }
        
    }



}