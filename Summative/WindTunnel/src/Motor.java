import org.zu.ardulink.Link;

public class Motor{
	public static final int minPower = 1200, maxPower = 1600;
	private Link link;
	private int pin;
	private int value;
	
	public Motor(int pin){
		this.pin = pin;
	}
	
	public void connect(Link link){
		this.link = link;
	}
	
	public void setValue(int value){
    	if(value < minPower) value = minPower;
    	if(value > maxPower) value = maxPower;
    	link.sendCustomMessage("pesc/" + pin + "/" + value);
    }
	
	public int getPin(){
		return pin;
	}
	
	public int getValue(){
		return value;
	}
}
