import java.awt.Color;
import java.awt.Font;
import java.awt.GridLayout;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSpinner;
import javax.swing.SpinnerNumberModel;

public class Readout extends JPanel{
	private double value;
	private JLabel nameLabel, valueLabel;
	private String unit;
	
	private boolean edit;
	private JSpinner angle;
	
	public Readout(String name, double num, String str, boolean editable){
		setLayout(new GridLayout(0, 1));
		setBorder(BorderFactory.createLineBorder(Color.black));

		nameLabel = new JLabel("<html><b><font size='7' color='red'>" + name + "</font></b></html>");
		add(nameLabel);
		
		value = num;
		unit = str;
		edit = editable;
		if(edit){
			JPanel spinnerPanel = new JPanel();
			spinnerPanel.setLayout(new BoxLayout(spinnerPanel, BoxLayout.LINE_AXIS));
			add(spinnerPanel);
			angle = new JSpinner(new SpinnerNumberModel(0, -30, 30, 1));
			angle.setFont(new Font("Arial", Font.PLAIN, 30));
			spinnerPanel.add(angle);
			valueLabel = new JLabel("<html><font size='7' color='red'>" + unit + "</font></html>");
			spinnerPanel.add(valueLabel);
		}
		else{
			valueLabel = new JLabel("<html><font size='7' color='red'>" + value + " " + unit + "</font></html>");
			add(valueLabel);
		}
	}
	
	public double getAngle(){
		//if(edit) return (double) angle.getValue();
		return 0;
	}
	public void setValue(double num){
		value = Math.round(num * 100) / 100.0;
		if(edit){
			angle.setValue(num);
		}
		else{
			valueLabel.setText("<html><font size='7' color='red'>" + value + " " + unit + "</font></html>");
		}
	}
}
