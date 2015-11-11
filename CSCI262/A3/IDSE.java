import java.util.*;
import java.io.*;

public class IDSE 
{	
	// variables
	int n_events = 0;
	float threshold = 0;

	String[] events = new String[20];
	float[] weights = new float[20];

	float[] eventsAverage = new float[20];
	float[] eventsStdDev = new float[20];

	List<float[]> testEvents;

	/**
	 * Run program
	 */
	public IDSE(String [] args) {

		if (args.length == 3) { // proper usage
			String eventsFile = args[0], baseDataFile = args[1], testEventsFile = args[2];

			// 1. / 2.
			// read in events			
			readEvents(eventsFile);

			// read in base data and calculate metrics
			readBaseData(baseDataFile);

			// print first step and second
			printBaseStats();

			// 3.
			// read in test events
			testEvents = new LinkedList<float[]>();
			readTestEvents(testEventsFile);

			// process all events for intrusion
			// print findings
			processAllEventsForIntrusion();

			// add some padding to the bottom
			System.out.print("\n\n");

		}
		else { // print usage
			System.out.print("usage: java IDSE <events.txt> <base-data.txt> <test-events.txt>");
		}
	}

	/** 
	 * 1. Read Data from Base-Data.txt
	 *	  into baseData while calculating 
	 *    averages and std deviations for 
	 *    each event
	 */
	public void readBaseData(String baseDataFile) { 
		try {
			File file = new File(baseDataFile);
			Scanner scanner = new Scanner(file);

			List<float[]> baseData = new LinkedList<float[]>();
			// read in base data
			while (scanner.hasNextLine()) {
				float[] data = new float[n_events];
				String line = scanner.nextLine();

				String[] arData = line.split(":");

				for (int i = 0; i < arData.length && i < n_events; i++) {
					data[i] = Float.parseFloat(arData[i]);
				}

				baseData.add(data);
			}

			// calculate averages
			float[] eventTotals = new float[n_events];
			for (int i = 0; i < baseData.size(); i++) {
				for (int j = 0; j < baseData.get(i).length; j++) {
					eventTotals[j]+=baseData.get(i)[j];
				}
			}

			for (int i = 0; i < eventTotals.length; i++) {
				eventsAverage[i] = eventTotals[i]/baseData.size();
			}

			// calculate stddev
			for (int i = 0; i < eventTotals.length; i++) {
				float eventVariance = 0;

				for (int j = 0; j < baseData.size(); j++) {
					float diff = baseData.get(j)[i] - eventsAverage[i];
					eventVariance += (diff * diff);
				}

				eventVariance /= baseData.size();
				eventsStdDev[i] = (float)Math.sqrt(eventVariance);	
			}

		} catch (FileNotFoundException ex) { ex.printStackTrace(); }				
	}

	/**
	 * 1. Read events and weights from Events.txt
	 *    Calculate threshold for intrusion
	 */
	public void readEvents(String eventsFile) {
		try {
			File file = new File(eventsFile);
			Scanner scanner = new Scanner(file);

			n_events = Integer.parseInt(scanner.nextLine());
			int counter = 0;
			while (scanner.hasNextLine()) {
				String line = scanner.nextLine();

				String [] arEvents = line.split(":");

				for (int i = 0; i < arEvents.length; i+=2) {
					if (arEvents[i].length() > 0) {
						events[counter] = arEvents[i];
						weights[counter] = Float.parseFloat(arEvents[i+1]);
						threshold += weights[counter];

						counter++;
					}
				}
			}

			threshold*=2;
		} 
		catch (FileNotFoundException ex) { ex.printStackTrace(); } 
	}

	/** 
	 * 1/2. Print stats calculated from events
	 *      and basedata
	 */
	public void printBaseStats() {
		// print header
		System.out.printf("\n\n%-25s", "Event");
		System.out.printf("%10s", "Average");
		System.out.printf("%10s", "Stdev");
		System.out.printf("%10s", "Weight");	
		System.out.print("\n\n");	

		// print each events stats
		for (int i = 0; i < n_events; i++) {
			System.out.printf("%-25s", events[i]);
			System.out.printf("%10.2f", eventsAverage[i]);
			System.out.printf("%10.2f", eventsStdDev[i]);
			System.out.printf("%10.2f", weights[i]);
			System.out.print("\n");
		}

		// print threshold
		System.out.printf("\n\n%-20s", "Threshold");
		System.out.printf("%.2f\n\n", threshold);
	}

	/**
	 * 3. Read in the data for the events to test
	 */
	public void readTestEvents(String testEventsFile) {

		try {
			File file = new File(testEventsFile);
			Scanner scanner = new Scanner(file);

			while (scanner.hasNextLine()) {

				float[] testEvent = new float[n_events];
				String line = scanner.nextLine();

				String[] arData = line.split(":");
				for (int i = 0; i < arData.length; i++) {
					if (arData[i].length() > 0) {
						testEvent[i] = Float.parseFloat(arData[i]);
					}
				}

				testEvents.add(testEvent);
			}
		} catch (FileNotFoundException ex) {
			ex.printStackTrace();

		}
	}

	/**
	 * 3. Loop through all test event data and run
	 *    processEventsForIntrusion
	 */
	public void processAllEventsForIntrusion() {
		for (int i = 0; i < testEvents.size(); i++) {
			// calculate distance
			float distance = getDistanceForLine(testEvents.get(i));

			// calculate if there is an intrusion
			boolean intrusion = (distance > threshold);

			// print findings
			printTestEvent(testEvents.get(i), distance, intrusion, (i+1));
		}
	}

	/**
	 * 3. Go through test event data and calculate distance
	 */
	public float getDistanceForLine(float[] events) {
		float distance = 0;

		// calculate distance based off events
		for (int i = 0; i < events.length; i++) {
			float eventVal = events[i];
			int n_stddev = (int)(Math.abs(eventVal - eventsAverage[i]) / eventsStdDev[i]);

			distance += n_stddev * weights[i];
		}

		return distance;
	}

	/**
	 * 3. Print results of testing a line from test events
	 */
	public void printTestEvent(float[] events, float distance, boolean intrusion, int index) {
		System.out.printf("Line %-2s -- ", index);
		String eventAsStr = "";
		for (int i = 0; i < events.length; i++) {
			eventAsStr += ((int)events[i]) + ":";
		}
		System.out.printf("%-25s", eventAsStr);
		System.out.printf("Distance: %-15.2f", distance);
		System.out.printf("Alarm: %s \n", (intrusion ? "Yes Yes Yes" : "No"));
	}

	/**
	 * Main Function
	 */
	public static void main(String [] args) 
	{
		new IDSE(args);
	}
}
