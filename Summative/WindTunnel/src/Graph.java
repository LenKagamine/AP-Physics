import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.geom.AffineTransform;
import java.util.ArrayList;

import javax.swing.JPanel;

public class Graph extends JPanel{
	private ArrayList<Double> x, y;
	private ArrayList<Color> colors;
	private double r = 10;
	private int xpanel = 960, ypanel = 960;
	private double xoffset = 50, yoffset = 50;
	private double xfull = 1200 - 2 * xoffset, yfull = 696 - 2 * yoffset;
	private double xsize = 10, ysize = 10;
	private double xnum = 10, ynum = 10;
	private String xname = "", yname = "";
	
	public Graph(){
		this(new ArrayList<Double>(), new ArrayList<Double>());
	}
	
	public Graph(ArrayList<Double> xs, ArrayList<Double> ys){
		x = xs;
		y = ys;
		colors = new ArrayList<Color>();
		setPreferredSize(new Dimension(xpanel, ypanel));
	}
	
	public void setxUnit(String xstr, double xmax){
		xname = xstr;
		xsize = xmax;
		repaint();
	}
	
	public void setyUnit(String ystr, double ymax){
		yname = ystr;
		ysize = ymax;
		repaint();
	}
	
	public void addPoint(double xpoint, double ypoint, Color c){
		x.add(xpoint);
		y.add(ypoint);
		colors.add(c);
		repaint();
	}
	
	public void setPoints(ArrayList<Double> xs, ArrayList<Double> ys){
		x = xs;
		y = ys;
		repaint();
	}
	
	public void clearPoints(){
		x = new ArrayList<Double>();
		y = new ArrayList<Double>();
		colors = new ArrayList<Color>();
		repaint();
	}
	
	public void paintComponent(Graphics g1){
		super.paintComponent(g1);
		Graphics2D g = (Graphics2D) g1;
		g.setColor(Color.white);
		g.fillRect(0, 0, getWidth(), getHeight());
		
		drawAxes(g);
		
		for(int i=0; i<x.size(); i++){
			System.out.println(x.get(i) + " " + y.get(i));
			drawPoint(g, x.get(i), y.get(i), colors.get(i));
		}
		drawPoint(g, 0, 0, Color.red);
	}
	
	private void drawAxes(Graphics2D g){
		g.setColor(Color.black);
		g.drawLine((int)xoffset, (int)(getHeight() - yoffset), (int)(getWidth() - xoffset), (int)(getHeight() - yoffset));
		g.drawLine((int)xoffset, (int)(getHeight() - yoffset), (int)xoffset, (int)yoffset);
		
		g.setFont(new Font("Arial", Font.PLAIN, 15));
		for(int i=1; i<=xnum; i++){
			g.drawLine((int)(xoffset + i * xfull / xnum), (int)(getHeight() - yoffset), (int)(xoffset + i * xfull / xnum), (int)(getHeight() - yoffset - 20));
			g.drawString("" + i * xsize / xnum, (int)(xoffset + i * xfull / xnum - 8), (int)(getHeight() - yoffset + 15));
		}
		
		g.setFont(new Font("Arial", Font.PLAIN, 20));
		g.drawString(xname, getWidth()/2 - 50, (int) (getHeight() - yoffset + 35));

		g.setFont(new Font("Arial", Font.PLAIN, 15));
		for(int i=1; i<=ynum; i++){
			g.drawLine((int)(xoffset), (int)(getHeight() - yoffset - i * yfull / ynum), (int)(xoffset + 20), (int)(getHeight() - yoffset - i * yfull / ynum));
			g.drawString("" + i * ysize / ynum, (int)(xoffset - 25), (int)(getHeight() - yoffset - i * yfull / ynum + 5));
		}

		g.setFont(new Font("Arial", Font.PLAIN, 20));
		AffineTransform orig = g.getTransform();
		g.rotate(-Math.PI/2);
		g.drawString(yname, -400, 20);
		g.setTransform(orig);
	}
	
	private void drawPoint(Graphics2D g, double xp, double yp, Color c){
		g.setColor(c);
		g.fillOval((int)(xp*xfull/xsize - r/2 + xoffset), (int)(getHeight() - yp*yfull/ysize - r/2 - yoffset), (int)r, (int)r);
	}
}
