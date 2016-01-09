import java.awt.Color;
import java.awt.EventQueue;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.ArrayList;
import java.util.Hashtable;
import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.JRadioButton;
import javax.swing.JSlider;
import javax.swing.Timer;
import javax.swing.UIManager;
import javax.swing.UIManager.LookAndFeelInfo;
import javax.swing.border.EmptyBorder;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import org.zu.ardulink.Link;
import org.zu.ardulink.gui.ConnectionStatus;

public class Main extends JFrame implements ActionListener, ChangeListener{
    private JPanel contentPane;
    private Link link;
    //private Anemometer anemo;
    private LoadCell loadCells;
    private Motor motor;
    private SevenSegment ss;
    private boolean connected = false;
    private boolean recording = false;
    
    private JPanel leftPanel;
    //Connection to Arduino
    private JButton connect;
    private ConnectionStatus status;
    //Controlling ESC / Motor
    private JSlider slider;
    private int majorSpacing = 100;
    private JProgressBar percentPower;
    //Sensors Panel
    private JButton speedCalib;
    private JButton forceCalib;
    private JButton record;
    private JButton clearGraph;
    //Display
    private JLabel sendToDisplay;
    private ButtonGroup switchSpeed;
    private JRadioButton switchTrue, switchScaled;
    private JComboBox<String> sendValues;
    private JLabel xLabel, yLabel;
    private JComboBox<String> xCombo, yCombo; //lift1, drag1, lift2, drag2, speed
    private String[] displayNames = new String[]{"Lift 1", "Lift 2", "Drag 1", "Drag 2", "Speed", "Lift Coeff", "Drag Coeff"};
    //Airfoils
    private JComboBox<String> airfoils;
    private String[] airfoilNames = new String[]{"NACA 2412", "S1223 RTL", "Cessna 172", "Triangle", "Cylinder"};
    //Readouts
    private Readout[] readouts;
    
    private JPanel centerPanel;
    //Diagram
    private Diagram diagram;
    //Graph
    private Graph graph;
    private ArrayList<double[]> data; //lift1, drag1, lift2, drag2, speed
    private double[] recentData = new double[]{0, 0, 0, 0, 0, 0, 0, 0};  //speed, lift1, drag1, lift2, drag2, angle, Cl, Cd;
    private double[] graphMaxes = new double[]{10, 10, 10, 10, 5, 3, 1};
    
    private Timer timer;
    
    private JLabel liftRatio, dragRatio;
    
    public static void main(String[] args) {
        EventQueue.invokeLater(new Runnable() {
            public void run() {
                try {
                    try {
                        for (LookAndFeelInfo info : UIManager.getInstalledLookAndFeels()) {
                            if ("Nimbus".equals(info.getName())) {
                                UIManager.setLookAndFeel(info.getClassName());
                                break;
                            }
                        }
                    } catch(Exception e){
                    	System.out.println("Defaulting Look and Feel");
                    }
                    Main frame = new Main();
                    frame.setVisible(true);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
    }

    public Main(){
        setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
        setExtendedState(getExtendedState() | JFrame.MAXIMIZED_BOTH);
        setTitle("1315-MH Wind Tunnel GUI");
        addWindowListener(new WindowAdapter(){
        	@Override
        	public void windowClosing(WindowEvent e){
        		if(connected) disconnect();
        		System.exit(0);
        	};
        });
        
        contentPane = new JPanel();
        contentPane.setBorder(new EmptyBorder(17, 17, 17, 17));
        contentPane.setLayout(new GridBagLayout());
        setContentPane(contentPane);
        
        GridBagConstraints c = new GridBagConstraints();
        c.gridx = c.gridy = 0;
        c.gridwidth = c.gridheight = 1;
        c.weightx = c.weighty = 1;
        c.fill = GridBagConstraints.BOTH;
        c.anchor = GridBagConstraints.FIRST_LINE_END;
        leftPanel = new JPanel();
        leftPanel.setLayout(new BoxLayout(leftPanel, BoxLayout.PAGE_AXIS));
        contentPane.add(leftPanel, c);
        addLeftComponents();
        
        c.gridx = 1;
        c.weightx = 0.1;
        c.fill = GridBagConstraints.VERTICAL;
        centerPanel = new JPanel();
        centerPanel.setLayout(new BoxLayout(centerPanel, BoxLayout.PAGE_AXIS));
        contentPane.add(centerPanel, c);
        addCenterComponents();
        pack();
        
        link = Link.getDefaultInstance();
        //anemo = new Anemometer(2);
        loadCells = new LoadCell(); //Lift 1, Drag 1, Lift 2, Drag 2, Speed
        motor = new Motor(12);
        ss = new SevenSegment();
        
        timer = new Timer(250, this);
        timer.setActionCommand("timer");
		
        graph.setxUnit("Lift 1 (mN)", 2);
        graph.setyUnit("Lift 2 (mN)", 2);
        
        data = new ArrayList<double[]>();
    }
    
    public void addLeftComponents(){
    	/** ConnectionPanel:
    	 * 		Button connect: connecting button
    	 * 		ConnectionStatus status: display connection status
    	 */
        JPanel connectPanel = new JPanel(new GridLayout(0, 1));
        connectPanel.setBorder(BorderFactory.createTitledBorder("Connection Panel"));
        leftPanel.add(connectPanel);
        connect = new JButton("Connect");
        connect.setFont(new Font("Arial", Font.PLAIN, 20));
        connect.setActionCommand("connect");
        connect.addActionListener(this);
        connectPanel.add(connect);
        
        status = new ConnectionStatus();
        connectPanel.add(status);
        
        /** Motor Control
         * 		JSlider slider: slider control for motor
         * 		Hashtable<Integer, JLabel> labelTable: tick labels for slider
         * 		JProgressBar percentPower: percent display
         */
        JPanel motorPanel = new JPanel(new GridLayout(0, 1));
        motorPanel.setBorder(BorderFactory.createTitledBorder("Motor Panel"));
        leftPanel.add(motorPanel);
        slider = new JSlider(JSlider.HORIZONTAL, Motor.minPower, Motor.maxPower, Motor.minPower);
        slider.setMajorTickSpacing(majorSpacing);
        slider.setMinorTickSpacing(majorSpacing / 5);
        slider.setPaintTicks(true);
        slider.setEnabled(false);
        
        Hashtable<Integer, JLabel> labelTable = new Hashtable<Integer, JLabel>();
        for(int i=Motor.minPower; i<=Motor.maxPower; i+=majorSpacing){
        	labelTable.put(i, new JLabel(""+i));
        }
        slider.setLabelTable(labelTable);
        slider.setPaintLabels(true);
        slider.addChangeListener(this);
        motorPanel.add(slider);
        
        percentPower = new JProgressBar(JProgressBar.HORIZONTAL, Motor.minPower, Motor.maxPower);
        percentPower.setValue(Motor.minPower);
        percentPower.setStringPainted(true);
        percentPower.setFont(new Font("Arial", Font.PLAIN, 15));
        motorPanel.add(percentPower);
        
        /** Sensors Control
         * 		JButton speedCalib: calibrate the wind sensor
         * 		JButton forceCalib: calibrate the force sensors
         * 		JButton record: start/stop recording readings
         * 		JButton clearGraph: clear all points on the graph
         */
        JPanel sensorPanel = new JPanel(new GridLayout(1, 0));
        leftPanel.add(sensorPanel);
        sensorPanel.setBorder(BorderFactory.createTitledBorder("Sensors Panel"));
        speedCalib = new JButton("Reset Speed");
        speedCalib.setFont(new Font("Arial", Font.PLAIN, 20));
        speedCalib.setActionCommand("resetSpeed");
        speedCalib.addActionListener(this);
        sensorPanel.add(speedCalib);
        
        forceCalib = new JButton("Reset Forces");
        forceCalib.setFont(new Font("Arial", Font.PLAIN, 20));
        forceCalib.setActionCommand("resetForce");
        forceCalib.addActionListener(this);
        sensorPanel.add(forceCalib);
        
        record = new JButton("Record");
        record.setFont(new Font("Arial", Font.PLAIN, 20));
        record.setActionCommand("record");
        record.addActionListener(this);
        sensorPanel.add(record);
        
        clearGraph = new JButton("Clear");
        clearGraph.setFont(new Font("Arial", Font.PLAIN, 20));
        clearGraph.setActionCommand("clearGraph");
        clearGraph.addActionListener(this);
        sensorPanel.add(clearGraph);
        
        /** Display Panel
         * 		JPanel radioPanel: holds True/Scaled radio buttons
         * 			JLabel sendToDisplay: "Sensor Values"
         * 			JRadioButton switchTrue: radio button for true values
         * 			JRadioButton switchScaled: radio button for scaled values
         * 			ButtonGroup switchSpeed: radio group that holds radio buttons
         * 		JComboBox<String> sendValues: choose which variable to send to display
         */
        JPanel displayPanel = new JPanel(new GridLayout(1, 0));
        leftPanel.add(displayPanel);
        displayPanel.setBorder(BorderFactory.createTitledBorder("Display Panel"));
        JPanel radioPanel = new JPanel(new GridLayout(0, 1));
        displayPanel.add(radioPanel);
        
        sendToDisplay = new JLabel("Sensor Values");
        sendToDisplay.setFont(new Font("Arial", Font.PLAIN, 20));
        radioPanel.add(sendToDisplay);
        
        switchTrue = new JRadioButton("True");
        switchTrue.setFont(new Font("Arial", Font.PLAIN, 15)); 
        switchTrue.setSelected(true);
        radioPanel.add(switchTrue);
        
        switchScaled = new JRadioButton("Scaled");
        switchScaled.setFont(new Font("Arial", Font.PLAIN, 15));
        radioPanel.add(switchScaled);
        
        switchSpeed = new ButtonGroup();
        switchSpeed.add(switchTrue);
        switchSpeed.add(switchScaled);
        
        JPanel displayPanel2 = new JPanel(new GridLayout(0, 1));
        displayPanel.add(displayPanel2);
        JLabel toDisplay = new JLabel("To Display");
        toDisplay.setFont(new Font("Arial", Font.PLAIN, 20));
        displayPanel2.add(toDisplay);
        sendValues = new JComboBox<String>(displayNames);
        sendValues.setFont(new Font("Arial", Font.PLAIN, 20));
        displayPanel2.add(sendValues);
        
        /** Airfoil Panel
         * 		JComboBox<String> airfoils: choose airfoil diagram
         */
    	JPanel airfoilPanel = new JPanel(new GridLayout(1, 0));
        leftPanel.add(airfoilPanel);
        airfoilPanel.setBorder(BorderFactory.createTitledBorder("Airfoil Panel"));
        airfoils = new JComboBox<String>(airfoilNames);
        airfoils.setFont(new Font("Arial", Font.PLAIN, 20));
        airfoils.setActionCommand("airfoilCombo");
        airfoils.addActionListener(this);
        airfoilPanel.add(airfoils);
        
        liftRatio = new JLabel("LRatio: 1");
        airfoilPanel.add(liftRatio);
        
        dragRatio = new JLabel("DRatio: 1");
        airfoilPanel.add(dragRatio);
        
        /** Graph Panel
         * 		JLabel xLabel: "x variable"
         * 		JLabel yLabel: "y variable"
         * 		JComboBox<String> xCombo: select x variable to graph
         * 		JComboBox<String> yCombo: select y variable to graph
         */
        JPanel graphPanel = new JPanel(new GridLayout(2, 2));
        leftPanel.add(graphPanel);
        graphPanel.setBorder(BorderFactory.createTitledBorder("Graph Panel"));
        xLabel = new JLabel("x Axis");
        xLabel.setFont(new Font("Arial", Font.PLAIN, 20));
        graphPanel.add(xLabel);
        
        yLabel = new JLabel("y Axis");
        yLabel.setFont(new Font("Arial", Font.PLAIN, 20));
        graphPanel.add(yLabel);
        
        xCombo = new JComboBox<String>(displayNames);
        xCombo.setFont(new Font("Arial", Font.PLAIN, 20));
        xCombo.setActionCommand("xCombo");
        xCombo.addActionListener(this);
        graphPanel.add(xCombo);
        
        yCombo = new JComboBox<String>(displayNames);
        yCombo.setFont(new Font("Arial", Font.PLAIN, 20));
        yCombo.setSelectedIndex(1);
        yCombo.setActionCommand("yCombo");
        yCombo.addActionListener(this);
        graphPanel.add(yCombo);
        
        /** Readout Panel
         * 		Readout[] readouts: list of readouts
         * 		String[] readoutNames: names of readouts
         * 		String[] readoutUnits: units of readouts
         */
        JPanel readoutPanel = new JPanel();
        readoutPanel.setLayout(new GridLayout(3, 2));
        leftPanel.add(readoutPanel);
        readoutPanel.setBorder(BorderFactory.createTitledBorder("Readout Panel"));
        String[] readoutNames = new String[]{"Lift 1", "Lift 2", "Drag 1", "Drag 2", "Speed", "Angle"};
        String[] readoutUnits = new String[]{"mN", "mN", "mN", "mN", "ms<font size='6'><sup>-1</sup></font>", "<font size='6'><sup>o</sup></font>"};
        readouts = new Readout[6];
        for(int i=0; i<readoutNames.length; i++){
        	readouts[i] = new Readout(readoutNames[i], 0.0, readoutUnits[i], i==5);
        	readoutPanel.add(readouts[i]);
        }
    }
    
    public void addCenterComponents(){
    	//Diagram
    	diagram = new Diagram(airfoilNames);
    	diagram.setBorder(BorderFactory.createTitledBorder("Forces Diagram"));
    	centerPanel.add(diagram);
    	
    	//Graph
    	JPanel graphPanel = new JPanel(new GridLayout(0, 1));
    	graphPanel.setBorder(BorderFactory.createTitledBorder("Graph"));	
    	centerPanel.add(graphPanel);
    	graph = new Graph();
    	graphPanel.add(graph);
    }
    
    @Override
    public void actionPerformed(ActionEvent e){
    	if(e.getActionCommand().equals("connect")){ //Connect to Arduino
    		java.util.List<String> portList = link.getPortList();
    		if(portList != null && portList.size() > 0){ /////////////////////Change to loop through all ports
    			String port = portList.get(0);
    			connected = link.connect(port);
    			System.out.println("Connecting on port: " + port);
    			if(connected){
    				try{
    					Thread.sleep(2000);
    				} catch(Exception ex){
    					ex.printStackTrace();
    				}
    				connect.setText("Disconnect");
    				connect.setActionCommand("disconnect");
    				System.out.println("Connected");
    				
    				motor.connect(link);
    				motor.setValue(Motor.minPower);
    				slider.setEnabled(true);
    				
    				//anemo.connect(link);
    				
    				loadCells.connect(link);
    				loadCells.tare();
    		    	loadCells.requestValue();
    		    	
    				ss.connect(link);
    				
    				timer.start();
    			}
    			else System.out.println("Connect failure");
    		}
    		else System.out.println("No port found");
    	}
    	else if(e.getActionCommand().equals("disconnect")){ //Disconnect
    		connect.setText("Connect");
    		connect.setActionCommand("connect");
    		disconnect();
    	}
    	else if(e.getActionCommand().equals("airfoilCombo")){ //Select airfoil diagram
    		diagram.setImage(airfoils.getSelectedIndex());
    	}
    	else if(e.getActionCommand().equals("timer")) loop(); //Timer loop
    	else if(e.getActionCommand().equals("resetSpeed")){ //Calibrate wind speed (load cell #5)
    		//anemo.calibrate();
    		loadCells.windTare();
    	}
    	else if(e.getActionCommand().equals("resetForce")){ //Calibrate force sensors
    		loadCells.tare();
    	}
    	else if(e.getActionCommand().equals("record")){ //Start recording data points for graph
    		recording = true;
    		record.setText("Stop Record");
    		record.setActionCommand("stopRecord");
    	}
    	else if(e.getActionCommand().equals("stopRecord")){ //Stop recording
    		recording = false;
    		record.setText("Record");
    		record.setActionCommand("record");
    	}
    	else if(e.getActionCommand().equals("clearGraph")){ //Clear the graph
    		graph.clearPoints();
    	}
    	else if(e.getActionCommand().equals("xCombo")){ //Select x variable of graph
    		String xUnit = (String) xCombo.getSelectedItem();
    		graph.setxUnit(convertUnit(xUnit), graphMaxes[xCombo.getSelectedIndex()]);
    	}
    	else if(e.getActionCommand().equals("yCombo")){ //Select y variable of graph
    		String yUnit = (String) yCombo.getSelectedItem();
    		graph.setyUnit(convertUnit(yUnit), graphMaxes[yCombo.getSelectedIndex()]);
    	}
    }
    
    public String convertUnit(String unit){ //Add units after variable name
    	if(unit.length() == 6) return unit + " (mN)"; //Forces
    	if(unit.length() == 5) return unit + " (ms^-1)"; //Wind speed
    	return unit;
    }
    
    public void disconnect(){ //Disconnect
		slider.setValue(Motor.minPower);
		slider.setEnabled(false);
		motor.setValue(Motor.minPower);
		
		ss.setNum(0.0);
		
		diagram.setArrows(new double[]{0, 0, 0, 0});
		
		for(int i=0;i<6;i++) readouts[i].setValue(0);
		connected = false;
    }
    
    public void loop(){
    	if(!connected){
    		link.disconnect();
    		connect.setText("Connect");
    		connect.setActionCommand("connect");
        	connected = false;
    		System.out.println("Disconnected");
    		timer.stop();
    		return;
    	}
    	
    	boolean trueValue = switchTrue.isSelected();
    	/*if(anemo.isUpdated()){
    		if(trueValue){
    			double spd = anemo.getSpeed();
    			readouts[4].setValue(spd);
    			ss.setNum(spd);
    			recentData[0] = spd;
    		}
    		else{
    			double spd = anemo.getScaledSpeed();
    			readouts[4].setValue(spd);
    			ss.setNum(spd);
    		}
    	}*/
    	if(loadCells.isUpdated()){
    		double[] forces;
    		if(trueValue) forces = loadCells.getValue();
    		else forces = loadCells.getScaledValue(airfoils.getSelectedIndex());
    		readouts[0].setValue(forces[0]); //lift 1
    		readouts[1].setValue(forces[2]); //lift 2
    		readouts[2].setValue(forces[1]); //drag 1
    		readouts[3].setValue(forces[3]); //drag 2
    		readouts[4].setValue(forces[4]); //speed
    		
    		diagram.setArrows(forces);
    		recentData[0] = forces[0];
    		recentData[1] = forces[2];
    		recentData[2] = forces[1];
    		recentData[3] = forces[3];
    		recentData[4] = forces[4];
    	}
    	loadCells.requestValue();
    	
    	recentData[5] = readouts[5].getAngle(); //angle of attack
    	recentData[6] = loadCells.getLiftCoeff(airfoils.getSelectedIndex()); //lift coefficient
    	recentData[7] = loadCells.getDragCoeff(airfoils.getSelectedIndex()); //drag coefficient
    	
    	if(recentData[1] == 0) liftRatio.setText("LRatio: N/A");
    	else liftRatio.setText("LRatio: " + recentData[0]/recentData[1]);
    	if(recentData[3] == 0) dragRatio.setText("DRatio: N/A");
    	else dragRatio.setText("DRatio: " + recentData[2]/recentData[3]);
    	
    	ss.setNum(recentData[sendValues.getSelectedIndex()]);
    	if(recording){
    		//data.add(recentData);  //lift 1, lift 2, drag 1, drag 2, speed
    		graph.addPoint(recentData[xCombo.getSelectedIndex()], recentData[yCombo.getSelectedIndex()], Color.blue);
    	}
    }

    @Override
    public void stateChanged(ChangeEvent e){
		JSlider slider = (JSlider) e.getSource();
		int value = slider.getValue();
		percentPower.setValue(value);
		motor.setValue(value);
    }
}