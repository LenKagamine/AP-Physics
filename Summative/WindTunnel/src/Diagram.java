import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class Diagram extends JPanel{
	private int numImages;
	private CardLayout cardLayout;
	private BufferedImage[] images;
	private JLabel[] airfoils;
	private String[] names;

	private Arrow[] arrows; //lift 1, drag 1, lift 2, drag 2
	
	public Diagram(String[] airfoilNames){
		names = airfoilNames;
		numImages = airfoilNames.length;
		cardLayout = new CardLayout();
		setLayout(cardLayout);
		images = new BufferedImage[numImages];
		airfoils = new JLabel[numImages];
		try{
			for(int i=0; i<numImages; i++){
				images[i] = ImageIO.read(getClass().getResourceAsStream("airfoil" + (i+1) + ".png"));
				airfoils[i] = new JLabel(new ImageIcon(images[i]));
				add(airfoils[i], names[i]);
			}
		} catch(IOException e){
			e.printStackTrace();
		}
		
		arrows = new Arrow[4];
		arrows[0] = new Arrow(200, 148, 50, Arrow.UP, Color.blue, "Lift 1");
		arrows[1] = new Arrow(210, 148, 50, Arrow.RIGHT, Color.red, "Drag 1");
		arrows[2] = new Arrow(900, 148, 50, Arrow.UP, Color.blue, "Lift 2");
		arrows[3] = new Arrow(910, 148, 50, Arrow.RIGHT, Color.red, "Drag 2");
	}
	
	public void setArrows(double[] magn){
		for(int i=0;i<4;i++){
			arrows[i].setValue(magn[i] * 10);
		}
		repaint();
	}
	
	public void setImage(int choice){
		cardLayout.show(this, names[choice]);
	}
	
	@Override
	public void paint(Graphics g){
		super.paint(g);
		for(int i=0; i<4; i++) arrows[i].draw(g);
	}
}
