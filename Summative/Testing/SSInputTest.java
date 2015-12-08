//Testing of Seven Segment Display
//Manual number input

import java.util.List;
import java.util.Scanner;

import org.zu.ardulink.Link;
import org.zu.ardulink.event.DigitalReadChangeEvent;
import org.zu.ardulink.event.DigitalReadChangeListener;
import org.zu.ardulink.protocol.IProtocol;

public class SSInputTest {
	private static Link link;
	static int[][] pins = new int[][]{
		{22, 23, 24, 25}, //Pins for MSD
		{26, 27, 28, 29},
		{30, 31, 32, 33} //Pins for LSD
	};
	static int[] decPins = new int[]{34, 35};
	public static void outDigit(int pin, int digit){
		for(int i=3;i>=0;--i){
			link.sendPowerPinSwitch(pins[pin][i], (digit%2==1)?IProtocol.HIGH:IProtocol.LOW);
			System.out.println("Pin " + pins[pin][i] + ": on");
			digit /= 2;
		}
	}
	public static void outNum(double num){
		link.sendPowerPinSwitch(decPins[0], IProtocol.LOW);
		link.sendPowerPinSwitch(decPins[1], IProtocol.LOW);
		if(num < 0) num *= -1;
		if(num < 10){
			link.sendPowerPinSwitch(decPins[0], IProtocol.HIGH);
			num *= 100;
		}
		else if(num < 100){
			link.sendPowerPinSwitch(decPins[1], IProtocol.HIGH);
			num *= 10;
		}
		for(int i=2;i>=0;--i){
			outDigit(i, (int)num%10);
			num /= 10;
		}
	}
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		try {
			link = Link.getDefaultInstance();

			List<String> portList = link.getPortList();
			if(portList != null && portList.size() > 0) {
				String port = portList.get(0);
				System.out.println("Connecting on port: " + port);
				boolean connected = link.connect(port);
				System.out.println("Connected:" + connected);
				Thread.sleep(2000); //Wait for Arduino to load
				double input = 0;
				while(input*100 != -314){
					input = in.nextDouble();
					outNum(input);
				}
				link.disconnect();
			}
			else {
				System.out.println("No port found!");
			}
		}
		catch(Exception e) {
			e.printStackTrace();
		}
		finally{
			in.close();
		}
	}
}
