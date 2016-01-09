import org.zu.ardulink.Link;
import org.zu.ardulink.RawDataListener;

public class LoadCell implements RawDataListener{
	private Link link;
	private double values[];
	private boolean updated = false;
	
	double[] areaL = new double[]{0.004, 0.004, 0.00327, 0.4, 0.502};
	double[] areaD = new double[]{0.0002, 0.0002, 0.0002, 0.4, 0.502};
	double[] length = new double[]{0.04, 0.04, 0.04, 0, 0.02};
	double[] areaSL = new double[]{13.65504, 13.65504, 14.1, 1, 1};
	double[] areaSD = new double[]{0.227612448, 0.227612448, 0.227612448, 1, 1};
	double[] lengthScaled = new double[]{2.1336, 2.1336, 2.1336,1, 1};
	
	public LoadCell(){
		values = new double[]{0, 0, 0, 0, 0};
	}
	
	public void connect(Link link){
		this.link = link;
		link.addRawDataListener(this);
	}
	
	public void tare(){
		link.sendCustomMessage("tare");
	}
	
	public void windTare(){
		link.sendCustomMessage("wind");
	}
	
	public boolean isUpdated(){
		return updated;
	}
	
	public void requestValue(){
		link.sendCustomMessage("load");
	}
	
	public double[] getValue(){
		updated = false;
		double[] copyValues = values;
		values[0] = 0.006330 * (copyValues[0] - 0.0211*copyValues[1]) * 1000; //lift 1
		values[1] = 0.00594 * (copyValues[1] - 0.0134*copyValues[0]) * 1000;  //drag 1
		values[2] = 0.006293 * (copyValues[2] + 0.0206*copyValues[3]) * 1000; //lift 2
		values[3] = 0.005645 * (copyValues[3] - 0.0151*copyValues[2]) * 1000; //drag 2
		values[4] = 0.49 * copyValues[4]; //speed 
		return values;
	}
	
	public double[] getScaledValue(int airfoilNum){
		double[] temp = getValue();
		if(airfoilNum == 4){ //Cylinder
			double Cl = 0, Cd = 0.47;
			double Re = temp[4] * length[airfoilNum] / 1.36E-4;
			double speedS = 1.36E-4 * Re / lengthScaled[airfoilNum];
			double liftS = 1.36E-4 * Cl * areaSL[airfoilNum];
			double dragS = 1.225 * speedS * speedS * Cd * areaSD[airfoilNum];
			return new double[]{liftS, dragS, liftS, dragS, speedS};
		}
		double Cl1 = temp[0] / (1.36E-4 * areaL[airfoilNum]);
		double Cl2 = temp[2] / (1.36E-4 * areaL[airfoilNum]);
		double Cd1 = 2*temp[1] / (1.225 * temp[4] * temp[4] * areaD[airfoilNum]);
		double Cd2 = 2*temp[3] / (1.225 * temp[4] * temp[4] * areaD[airfoilNum]);
		double Re = temp[4] * length[airfoilNum] / 1.36E-4;
		double speedS = 1.36E-4 * Re / lengthScaled[airfoilNum];
		double liftS1 = 1.36E-4 * Cl1 * areaSL[airfoilNum];
		double liftS2 = 1.36E-4 * Cl2 * areaSL[airfoilNum];
		double dragS1 = 1.225 * speedS * speedS * Cd1 * areaSD[airfoilNum];
		double dragS2 = 1.225 * speedS * speedS * Cd2 * areaSD[airfoilNum];
		return new double[]{liftS1, dragS1, liftS2, dragS2, speedS};
	}
	
	public double getLiftCoeff(int airfoilNum){
		if(airfoilNum == 4) return 0;
		return 0.5 * ((values[0] / (1.36E-4 * areaL[airfoilNum])) + (values[2] / (1.36E-4 * areaL[airfoilNum])));
	}
	
	public double getDragCoeff(int airfoilNum){
		if(airfoilNum == 4) return 0.47;
		return 0.5 * ((2*values[1] / (1.225 * values[4] * values[4] * areaD[airfoilNum])) +
							(2*values[3] / (1.225 * values[4] * values[4] * areaD[airfoilNum])));
	}

	@Override
	public void parseInput(String id, int numBytes, int[] message){ //  alp://load/[num]/[value]
		StringBuilder build = new StringBuilder(numBytes + 1);
		for (int i = 0; i < numBytes; i++) {
			build.append((char) message[i]);
		}
		String inputString = build.toString();
		if(inputString.substring(6, 10).equals("load")){
			int sepPos = inputString.indexOf("/", 11);
			int inputNum = Integer.parseInt(inputString.substring(11, sepPos));
			double inputValue = Double.parseDouble(inputString.substring(sepPos + 1));
			
			values[inputNum] = inputValue;
			updated = true;
			//System.out.println("Raw: " + inputString);
			//System.out.println("Num: " + inputNum + ", Value: " + inputValue);
		}
	}
}
