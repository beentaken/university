import java.io.*;
import java.util.*;

public class Relgen {

	private List<Level> levels;
	private List<Relation> relations;
	private List<User> users;

	public Relgen(String input, String output) {
		// create lists
		levels = new LinkedList<Level>();
		relations = new LinkedList<Relation>();
		users = new LinkedList<User>();

		try {
			// create file reader 
			FileInputStream fin = new FileInputStream(input);
			Scanner scanner = new Scanner(fin, "UTF-8");

			// read in file
			while (scanner.hasNextLine()) {
				String line = scanner.nextLine();
				String[] values = line.split(":");

				// create...
				if (values[0].equals("Levels")) { // levels

					for (int i = 1; i < values.length; i++) {
						levels.add(new Level(i-1, values[i]));
					}

				} else if (values[0].equals("Relation")) { // relation

					for (int i = 1; i < values.length; i+=2) {
						relations.add(new Relation(values[i], values[i+1]));
					}

				} else if (values[0].equals("User")) { // users

					for (int i = 1; i < values.length; i+=2) {
						users.add(new User(values[i], values[i+1]));
					}

				}

			}

			// create scripts
			String script = "";

			// create roles
			for (Level level : levels) {
				script += "CREATE ROLE role_" + level.name + ";\n";
			}

			script += "\n";

			List<String> views = new LinkedList<String>();

			// create views
			for (Level level: levels) {
				String subscript = "CREATE VIEW view_"+level.name + " \n";
				String rels = "";
				for (Relation relation : relations) {
					if (relation.level.equals(level.name)) {
						if (!rels.equals(""))
							rels += ", ";
						rels += relation.name;
					}
				}
				subscript += "AS (SELECT * FROM " + rels + ");\n";
				if (!rels.equals("")) {
					script += subscript;
					views.add(level.name);
				}
			}

			script += "\n";

			// grant roles access to views
			for (Level level : levels) {
				for (String view : views) {
					Level viewLevel = null;
					for (Level l2 : levels) {
						if (l2.name.equals(view)) {
							viewLevel = l2;
							break;
						}
					}

					if (viewLevel!=null) {
						String access = "";
						if (viewLevel.dominance < level.dominance) {// read
							access += "SELECT";
						} else if (viewLevel.dominance == level.dominance) {// read & write
							access += "SELECT, INSERT";
						} else if (viewLevel.dominance > level.dominance) { // write
							access += "INSERT";
						}

						if (!access.equals("")) {
							script += "GRANT " + access + " ON view_" + view + " TO role_" + level.name + ";\n";
						}
					}
				}
			}

			script += "\n";

			// assign roles to users
			for (User user : users) {
				Level ul = null;
				for (Level level : levels) {
					if (user.level.equals(level.name)) {
						ul = level;
					}
				}

				if (ul != null){ 
					script += "GRANT role_" + ul.name + " TO " + user.name + ";\n";
				}
			}

			// write script to output
			PrintWriter pw = null;
			try {
				FileWriter fout = new FileWriter(output, true);
				BufferedWriter bw = new BufferedWriter(fout);
				pw = new PrintWriter(bw);

				pw.write(script);
			} catch (IOException ex) {
				ex.printStackTrace();
			} finally {
				pw.close();  
			} 

			System.out.println("Please check " + output);


		} catch (IOException ex) {
			ex.printStackTrace();
		}
	}

	public static void main(String [] args) {

		if (args.length == 2) {

			new Relgen(args[0], args[1]);			

		} else { // else print usage
			System.out.println("usage: java Relgen <input> <output>");
		}

	}

	private class Level {
		public int dominance;
		public String name;

		public Level(int dominance, String name) {
			this.dominance = dominance;
			this.name = name;
		}
	}

	private class Relation {
		public String name, level;

		public Relation(String name, String level) {
			this.name = name;
			this.level = level;
		}
	}

	private class User {
		public String name, level;

		public User(String name, String level) {
			this.name = name;
			this.level = level;
		}
	}
}