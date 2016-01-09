import org.zu.ardulink.Link;
import org.zu.ardulink.protocol.IProtocol;

public class SevenSegment {
	private Link link;
	private int[][] pins = new int[][]{
		{24, 23, 22, 25}, //Pins for MSD
		{28, 27, 26, 29},
		{32, 31, 30, 33} //Pins for LSD
	};
	private int[] decPins = new int[]{34, 35};
	private int powerPin = 13;
	private double value;
	
	public SevenSegment(){
		this(0.0);
	}
	public SevenSegment(double value){
		this.value = value;
	}
	
	public void connect(Link link){
		this.link = link;
		link.sendPowerPinSwitch(powerPin, IProtocol.HIGH);
		setNum(value);
	}
	
	public double getValue(){
		return value;
	}
	
	public void outDigit(int pin, int digit){
		for(int i=3;i>=0;--i){
			link.sendPowerPinSwitch(pins[pin][i], (digit%2==1)?IProtocol.HIGH:IProtocol.LOW);
			digit /= 2;
		}
	}
	public void setNum(double num){
		//System.out.println("Seven Segment: " + num);
		link.sendPowerPinSwitch(decPins[0], IProtocol.LOW);
		link.sendPowerPinSwitch(decPins[1], IProtocol.LOW);
		if(num < 0) num *= -1;
		if(num < 100){
			link.sendPowerPinSwitch(decPins[1], IProtocol.HIGH);
			num += 0.05;
			num *= 10;
		}
		for(int i=2;i>=0;--i){
			outDigit(i, (int)num%10);
			num /= 10;
		}
	}
}
