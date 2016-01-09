import org.zu.ardulink.Link;
import org.zu.ardulink.event.AnalogReadChangeEvent;
import org.zu.ardulink.event.AnalogReadChangeListener;

public class Anemometer implements AnalogReadChangeListener{
	private int pin;
	private double speed;
	private double offset;
	private boolean calibrated = false;
	private double sensitivity = 1.0;
	private boolean updated = false;
	
	public Anemometer(int pin){
		this.pin = pin;
		speed = 0;
	}
	
	public void connect(Link link){
		link.addAnalogReadChangeListener(this);
	}
	
	public boolean isUpdated(){
		return updated;
	}
	
	public double getSpeed(){
		updated = false;
		return speed;
	}
	
	public double getScaledSpeed(){
		updated = false;
		return speed;
	}
	
	public int getPinListening(){
		return pin;
	}

	public void calibrate(){
		calibrated = false;
	}
	
	public void stateChanged(AnalogReadChangeEvent e){
		//int pin = e.getPin();
		int value = e.getValue();
		if(!calibrated){
			//calculate offset
			calibrated = true;
			offset = value - 240;
		}
		//calculate speed from value
		double shiftValue = value - offset;
		double voltage = 5.0 * shiftValue / 1023.0;
		double pressure = voltage / sensitivity;
		if(pressure < 0) pressure = 0;
		double num = Math.sqrt(2.0 * pressure / 1.2);
		speed = 5515.570538*Math.sqrt(num) - 2165.047384*num - 3494.745014;
		if(speed < 0.2) speed = 0;
		updated = true;
		//System.out.println("Wind speed: " + value + " " + speed);
	}
}