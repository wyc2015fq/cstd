# Java游戏：FlappyBird源代码 - Koma Hub - CSDN博客
2017年09月27日 22:21:16[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：1026
```java
/*
2017/7/23
https://www.youtube.com/watch?v=DO2zW365CEg
*/
import java.awt.Graphics;
//import java.util.Timer;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseListener;
import java.awt.event.MouseEvent;
import java.awt.event.KeyListener;
import java.awt.event.KeyEvent;
import java.awt.Rectangle;
import java.awt.*;
import java.util.*;
import javax.swing.JFrame;
import javax.swing.Timer;
import javax.swing.*;
import javax.swing.JPanel;
class Renderer extends JPanel
{
	
	private static final long serialVersionUID = 1L;
	protected void paintComponent(Graphics g)
	{
		super.paintComponent(g);
		FlappyBird.flappyBird.repaint(g);
	}
}
public class FlappyBird implements ActionListener, MouseListener, KeyListener
{
	public static FlappyBird flappyBird;
	public final int WIDTH = 900, HEIGHT = 800;
	public Renderer renderer;
	public Rectangle bird;
	public ArrayList<Rectangle> columns; 
	public int ticks, yMotion, score;
	public boolean gameOver, started;
	public Random rand;
	public FlappyBird()
	{
		JFrame jframe = new JFrame();
		Timer timer = new Timer(20,this);
		renderer = new Renderer();
		rand = new Random();
		jframe.add(renderer);
		jframe.setTitle("Flappy Bird");
		jframe.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		jframe.setSize(WIDTH,HEIGHT);
		jframe.addMouseListener(this);
		jframe.addKeyListener(this);
		jframe.setResizable(false);
		jframe.setVisible(true);
		bird = new Rectangle(WIDTH / 2 - 10, HEIGHT / 2 - 10, 20, 20);
		columns = new ArrayList<Rectangle>();
		addColumn(true);
		addColumn(true);
		addColumn(true);
		addColumn(true);
		timer.start();
	}
	public void addColumn(boolean start)
	{
		int space = 300;
		int width = 100;
		int height = 50 + rand.nextInt(300);
		
		if(start)
		{
			columns.add(new Rectangle(WIDTH + width + columns.size() * 300, HEIGHT - height - 120, width, height));
			columns.add(new Rectangle(WIDTH + width + (columns.size()-1)*300, 0, width, HEIGHT - height - space));
		}
		else
		{
			columns.add(new Rectangle(columns.get(columns.size() - 1).x + 600, HEIGHT - height - 120, width, height));
			columns.add(new Rectangle(columns.get(columns.size() - 1).x , 0, width, HEIGHT - height - space));
		}
		
	}
	public void paintColumn(Graphics g, Rectangle column)
	{
		g.setColor(Color.green.darker());
		g.fillRect(column.x, column.y, column.width, column.height);
	}
	public void jump()
	{
		if (gameOver)
		{
			bird = new Rectangle(WIDTH / 2 - 10, HEIGHT / 2 - 10, 20, 20);
			columns.clear();
			yMotion = 0;
			score = 0;
			addColumn(true);
			addColumn(true);
			addColumn(true);
			addColumn(true);
			gameOver = false;
		}
		if(!started)
		{
			started = true;
		}
		else if(!gameOver)
		{
			if(yMotion > 0)
			{
				yMotion = 0;
			}
		
			yMotion -= 10;
		}
	}
	public void actionPerformed(ActionEvent e)
	{
		int speed = 10;
		ticks++;
		if(started )
		{
			for( int i = 0; i < columns.size(); i++)
			{
				Rectangle column = columns.get(i);
				column.x -= speed;
			}
			if(ticks % 2 ==0 && yMotion < 15)
			{
				yMotion += 2;
			}
			for (int i = 0; i < columns.size(); i++)
			{
				Rectangle column = columns.get(i);
				if (column.x + column.width < 0)
				{
					columns.remove(column);
					if(column.y ==0)
					{
						addColumn(false);
					}
				}
			}
			bird.y += yMotion;
			for(Rectangle column : columns)
			{
				if(bird.x + bird.width / 2 > column.x + column.width / 2 - 5 
				&& bird.x + bird.width / 2 < column.x + column.width / 2 + 5
				&& column.y == 0)
				{
					score++;
				}	
				if(column.intersects(bird))
				{
					gameOver = true;
					if(bird.x <= column.x)
					{
						bird.x = column.x - bird.width;
					}
					else
					{
						if(column.y != 0)
						{
							bird.y = column.y - bird.height;
						}
						else if(bird.y < column.height)
						{
							bird.y = column.height;
						}
					}
				}	
			}
			if(bird.y > HEIGHT - 120 || bird.y < 0 )
			{
				gameOver = true;
			}
			if(bird.y + yMotion >= HEIGHT -120)//(gameOver)
			{
				bird.y = HEIGHT -120 - bird.height;
			}
		}
		renderer.repaint();
	}
	public void repaint(Graphics g)
	{
		//System.out.println("hello");
		g.setColor(Color.cyan);
		g.fillRect(0,0,WIDTH,HEIGHT);
		g.setColor(Color.orange);
		g.fillRect(0, HEIGHT - 120, WIDTH, 150);
		g.setColor(Color.green);
		g.fillRect(0, HEIGHT - 120, WIDTH, 20);
		g.setColor(Color.red);
		g.fillRect(bird.x, bird.y, bird.width, bird.height);
		for ( Rectangle column : columns )
		{
			paintColumn(g,column);
		}
		
		g.setColor(Color.white);
		g.setFont(new Font("Arial",1,70));
		if(!started)
		{
			g.drawString("Click to start!",90,HEIGHT / 2-50);
		}
		if(gameOver)
		{
			g.drawString("Game Over! You suck!",40,HEIGHT / 2-50);
		}
		if(!gameOver && started)
		{
			g.drawString(String.valueOf(score), WIDTH / 2, 100);
		}
	}
	public static void main(String[]args)
	{
		flappyBird = new FlappyBird();
	}
	public void mouseClicked(MouseEvent e)
	{
		jump();
	}
	public void mousePressed(MouseEvent e){}
	public void mouseReleased(MouseEvent e){}
	public void mouseEntered(MouseEvent e){}
	public void mouseExited(MouseEvent e){}
	public void keyPressed(KeyEvent e){}
	public void keyTyped(KeyEvent e){}
	public void keyReleased(KeyEvent e)
	{
		if(e.getKeyCode() == KeyEvent.VK_SPACE)
		{
			jump();
		}
	}
}
```
![](https://img-blog.csdn.net/20170927222239922?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170927222244815?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
