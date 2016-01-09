import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;

public class Arrow {
	public static final int UP = 0, RIGHT = 1;
	private double x1, y1, x2, y2;
	private Color color;
	private String name;
	private int angle;
	
	private int[] x, y;
	
	public Arrow(int x, int y, double magn, int angle, Color c, String str){
		x1 = x2 = x;
		y1 = y2 = y;
		this.x = new int[7];
		this.y = new int[7];
		this.angle = angle;
		color = c;
		name = str;
		setValue(magn);
	}
	
	public void setValue(double magn){
		if(angle == UP){
			y2 = y1 - magn;
			x[0] = x[6] = (int)(x1 - 5);
			x[1] = x[2] = (int)(x1 + 5);
			x[3] = (int)(x1 + 12);
			x[4] = (int)(x1);
			x[5] = (int)(x1 - 12);
			
			y[0] = y[1] = (int)(y1);
			y[2] = y[3] = y[5] = y[6] = (int)(y2);
			y[4] = (int)(y2 - magn/2);
		}
		else{
			x2 = x1 + magn;
			x[0] = x[1] = (int)(x1);
			x[2] = x[3] = x[5] = x[6] = (int)(x2);
			x[4] = (int)(x2 + magn/2);
			
			y[0] = y[6] = (int)(y1 + 5);
			y[1] = y[2] = (int)(y1 - 5);
			y[3] = (int)(y1 + 12);
			y[4] = (int)(y1);
			y[5] = (int)(y1 - 12);
		}
	}
	
	public void draw(Graphics g1){
		Graphics2D g = (Graphics2D) g1;
		g.setColor(color);
		g.fillPolygon(x, y, 7);
		g.setFont(new Font("Arial", Font.PLAIN, 20));
		if(name.substring(0, 4).equals("Lift"))	g.drawString(name, (int)(x1 + 20), (int)(y1 - 30));
		else g.drawString(name, (int)(x1 + 20), (int)(y1 + 30));
	}
}
