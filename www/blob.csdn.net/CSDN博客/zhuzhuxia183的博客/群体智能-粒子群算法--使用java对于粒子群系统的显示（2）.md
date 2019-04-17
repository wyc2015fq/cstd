# 群体智能-粒子群算法--使用java对于粒子群系统的显示（2） - zhuzhuxia183的博客 - CSDN博客





2017年07月31日 20:52:16[喵小姐的邻居小小朱](https://me.csdn.net/zhuzhuxia183)阅读数：641








大家好，可爱的小编今天来补充剩下的关于粒子群算法的想法。

呃呃，今天呢？小编主要谈的是关于如何编写这个的java实现代码。其实，关于代码实现呢？这个小编觉得，其实有时候你的编程思维很重要，可能要是一开始你的对于你的代码实现的整个的结构不是很清晰，其实想要比较好的展现这个效果还是蛮难的。但是，小编今天想分享自己写这类程序的相关套路，大家互相学习学习哈。

先来简单的理下思绪，想想我们的目的是什么？我们怎么样才算是验证了这个算法呢？我想，这个算法的，是想验证    通过局部最优，全局最优，以及不断的迭代，更新，最终我们能够无尽的逼近我们的最优解。好了吧，虽然所理论上，对于最优解的维度的话，其实是不加以限制的，但是这里的话，鉴于笔者的能力，暂时的话，就只是讨论2维层面的吧（3D效果之前没有接触过，哈哈）。朴素点来说，我们的目的，就是验证一群均匀分布在一片二维平面上的蜜蜂（速度是随机的），然后一旦有目标出现，可以通过这个粒子群算法，迅速的定位在目标位置周围。

> 
然后我们有了思路，就可以来想想，我们该怎么办，要怎么show呢？不过在此之前，其实大家应该准备几张比较精美的图片吧，作为背景嘛，然后准备一张小动物的小图片，其实gif的图片更好，省得你要去找一组图来进行拼接。然后呢？我想先总结下这种程序的相关特征，以方便大家使用套路啊。特征不外乎：1.我们需要不间断的显示我们的图片 2.然后随着鼠标点击事件，或者键盘事件，图片会有相应的移动现象 3.以上的整个画面中图片的切换需要表现的十分的流畅。   好吧，看到这里，是不是感觉，有点和我们小时候玩的小霸王游戏机上面的游戏很相似呢？哈哈，是不是很激动，小编第一次了解到的时候，情绪那叫一个激动啊。反正，就是说只要会了这个，相关的游戏也可以完成啦。啦啦啦。




我对于这类程序的分析：1.首先我们得要有一个Jframe窗体吧，不然，你怎么show呢？好，看代码

```java
package TryTest43;

import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;

import javax.swing.JFrame;

public class MainJFrame extends JFrame{

	public Graphics g;
	public Image img;
	public static int JFrame_Width;
	public static int JFrame_Height;
	public MainJFrame(){
		init();
		
	}
	
	public void init(){
		this.setSize(Toolkit.getDefaultToolkit().getScreenSize());
		this.setTitle("蜜蜂寻觅演示");
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setLocationRelativeTo(null);
		this.setVisible(true);
		Graphics g1=this.getGraphics();
		img=this.createImage(this.getWidth(),this.getHeight());

		g=img.getGraphics();
		this.addMouseListener(new MyMouseListener(g,g1,img));

		
		
		
	}
	
	public static void main(String[] args){
		MainJFrame jf=new MainJFrame();
		
		
	}

}
```




其实，挺简单对吧，就是新建一个JFrame的继承类，然后加下监听器，监听我们的鼠标的操作嘛。然后就是对于鼠标监听器中相关代码。


```java
package TryTest43;

import java.awt.Graphics;
import java.awt.Image;

import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;



public class MyMouseListener implements MouseListener {
	public Graphics g;
	public Graphics g1;
	public Image img;
	public TDimension dimen;

	
	public static int flag=1;
	public NewThread td;
	public MyMouseListener(Graphics g,Graphics g1,Image img){
		this.g=g;
		this.g1=g1;
		this.img=img;

		dimen=new TDimension(g, g1);
		
		td=new NewThread(dimen);
		td.start();
		
	}
	
	public void mouseClicked(MouseEvent e) {
		// TODO Auto-generated method stub

		if(e.getButton()==MouseEvent.BUTTON1){
			MyPoint Target=new MyPoint();
			Target.x=e.getX();
			Target.y=e.getY();
			
//			System.out.println(td);
			
			td.setTarget(Target);
		}
		if(e.getButton()==MouseEvent.BUTTON3){
//			System.out.println("rrrrrrrrrrrrrrrrr");
			td.isTone=true;
			td.setTone(e.getPoint());
			dimen.flag=true;
		}
		
	
	}

	public void mousePressed(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	public void mouseReleased(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	public void mouseEntered(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

	public void mouseExited(MouseEvent e) {
		// TODO Auto-generated method stub
		
	}

}
```


这里主要注意一点，就是说，我们在创建监听的同时，启动了线程，注意，这里线程一开始就启动了，从始至终我们就只有一个线程在跑。同时我也制定了下规则嘛，就是左击某个点时，模拟是那个地方有花朵，模拟蜜蜂的目标，然后使用 td.setTarget();这个函数就是我们设定目标的函数。如果右击的话，会有石头砸过去，模拟干扰物，然后蜜蜂就会暂时混乱并且忘掉目标物。同时，我们多次的左击右击，来验证此算法。（额，至于其中的Tdimension类的话，一会说哈，别急。）






接下来就是谈谈那个线程类。



```java
package TryTest43;

import java.awt.Graphics;

import java.awt.Point;
import java.awt.Toolkit;
import java.awt.image.BufferedImage;

public class NewThread extends Thread {
	public TDimension dimen;
//	public boolean Finished=true;
	public boolean isTone=false;
	public static int TimeNum=0;
	public NewThread(TDimension dimen){
		this.dimen=dimen;
		dimen.initial();
	}
	
	public void setTarget(MyPoint myP){
		dimen.Target=myP;
		dimen.finished=false;
		TimeNum=0;
//		Finished=false;
		
	}
	
	public void setTone(Point p){
		dimen.TonePos=p;
		isTone=true;
		TimeNum=0;
	}
	
	
	public void run(){
		while(true){
//			BufferedImage img=new BufferedImage(Toolkit.getDefaultToolkit().getScreenSize().width
//					, Toolkit.getDefaultToolkit().getScreenSize().height, BufferedImage.TYPE_INT_RGB);
//			Graphics g=img.getGraphics();
//			System.out.println("执行了?");
			
				if(!isTone&&!dimen.finished){
//					System.out.println("执行了！");
					dimen.search();
//					Finished=true;
				}else if(isTone&&TimeNum<50){
					//这里我们必须默认是完成了
					dimen.finished=true;
//					System.out.println("开始变乱！");
						TimeNum++;
						if(TimeNum==50){
							isTone=false;
						}
						dimen.run();
				}else{
					dimen.show();
				}
			
//			g1.drawImage(img, 0, 0, null);
		}
	}
}
```


主要是注意几个变量 1.boolean isTone是指有没有石头，一开始是默认没有的（如果认为一开始有，你可以冷静下再想想，哈哈）2.TimeNum是指我们如果丢了石头，然后蜜蜂开始混乱，其真实场景是，肯定刚开始的时候，速度很混乱嘛，也很大，然后呢？应该在各自的位置以较小速度晃荡对吧。所以的话，这其实就是两种状态，我们知道肯定前面一个状态的改变时间点便是一个自发的过程嘛，我们就自己设定下，因为线程是每次休眠一定的时间，我们通过TimeNum来进行控制，即从0-15作为一次周期。



其中主要逻辑在2个if 判断上面，就是说         1.如果没有石头，并且没有达到稳定状态时，我们就使蜜蜂处于寻觅状态。2.如果有石头，计数器并没有超过15个的话，处于混乱状态。  3.如果以上两种情况都没有满足，就是说处于没有石头，且完成状态， 就模拟稳定状态。







好了，下面就是我们主要算法类了。



```java
package TryTest43;

import java.awt.Color;



import java.awt.Graphics;
import java.awt.Image;
import java.awt.Point;
import java.awt.Toolkit;

import java.awt.image.BufferedImage;
import java.util.Random;

import javax.swing.ImageIcon;
import javax.tools.Tool;

public class TDimension {
	
	private final int step=100000;//循环遍历1000次
	private final int PosNum=10;//粒子数目
	private final double w=0.9;//惯性权重
	
	public static int time=0;
	
	private final double c1=2;//局部权重参数
	private final double c2=2;//全局权重参数
	
	public static boolean flag=false;
	public static Point TonePos=new Point();
	private int WIDTH;
	private int HEIGHT;
	
	private Image image;
	private Image BackImg;
	private Image ToneImg;
	private MyPoint g_best=new MyPoint();//全局最优解
	private MyPoint p_best[]=new MyPoint[PosNum];//粒子本身历史最优解
	private MyPoint p_v[]=new MyPoint[PosNum];//粒子的速度
	private MyPoint p_Pos[]=new MyPoint[PosNum];//粒子的位置
	
	public Graphics g1;
	public  static MyPoint Target;
	
	
	//一开始的时候，我们默认为是没有目标，即是已经完成了
	public  boolean finished=true;
	public Graphics g;
//	public Image img;
	
	private Random random=new Random();
	
	
	
	public TDimension(Graphics g,Graphics g1){
//		this.g=g;
		this.g1=g1;
		
		
		image=new ImageIcon("蜜蜂.gif").getImage();
		BackImg=new ImageIcon("BackGround.png").getImage();
		ToneImg=new ImageIcon("石头.png").getImage();
		WIDTH=Toolkit.getDefaultToolkit().getScreenSize().width;
		HEIGHT=Toolkit.getDefaultToolkit().getScreenSize().height;
	}
	
	public void initial(){

		//创建缓冲对象
		BufferedImage img=new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_RGB);
		g=img.getGraphics();
		
		g.drawImage(BackImg, 0,0,WIDTH,HEIGHT, 0, 0
				, BackImg.getWidth(null),BackImg.getHeight(null),null);
		for(int i=0;i<PosNum;i++){
			
			p_Pos[i]=new MyPoint(random.nextDouble()*WIDTH,random.nextDouble()*HEIGHT);
			p_v[i]=p_best[i]=new MyPoint(random.nextDouble()*10,random.nextDouble()*10);
			
			g.drawImage(image, (int)p_Pos[i].x,(int)p_Pos[i].y, (int)p_Pos[i].x+40,(int)p_Pos[i].y+40, 0, 0
					, image.getWidth(null),image.getHeight(null),null);
			
		}
		g1.drawImage(img, 0, 0, null);
	}
		

	public double Function(MyPoint now){
		
		double yy=Math.pow((now.x-Target.x), 2)+Math.pow((now.y-Target.y), 2);
		
		double distance=Math.pow((yy), 0.5);
		return distance;
	}
	
	
	public void search(){
		
		
		for(int i=0;i<PosNum;i++){
			if(Function(g_best)>Function(p_best[i])){
				g_best=p_best[i];
			}
			
		}	
			BufferedImage img=new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_RGB);
			g=img.getGraphics();
			for(int i=0;i<PosNum;i++){   //更新

				
				
				p_v[i].x=(w*p_v[i].x+c1*random.nextDouble()*(p_best[i].x-p_Pos[i].x)
						   +c2*random.nextDouble()*(g_best.x-p_Pos[i].x));
				p_v[i].y=w*p_v[i].y+c1*random.nextDouble()*(p_best[i].y-p_Pos[i].y)
						   +c2*random.nextDouble()*(g_best.y-p_Pos[i].y);
				
				if(p_v[i].x>10)
					p_v[i].x=10;
				if(p_v[i].x<-10)
					p_v[i].x=-10;
				if(p_v[i].y>10)
					p_v[i].y=10;
				if(p_v[i].y<-10)
					p_v[i].y=-10;
				
				if((p_v[i].x+p_Pos[i].x)>0&&(p_v[i].x+p_Pos[i].x)<WIDTH){
						
						p_Pos[i].x+=p_v[i].x;
						
				}
				if((p_v[i].y+p_Pos[i].y)>0&&(p_v[i].y+p_Pos[i].y)<HEIGHT){
					p_Pos[i].y+=p_v[i].y;
				}

//				p_Pos[i].y=p_Pos[i].y+p_v[i].y;
				if(Function(p_Pos[i])<Function(g_best)){
					g_best=p_Pos[i];
				}
				if(Function(p_best[i])>Function(p_Pos[i])){
					p_best[i]=p_Pos[i];
				}
				

				
			}

			
			
			
			//画背景
			g.drawImage(BackImg, 0,0,WIDTH
					,HEIGHT, 0, 0
					, BackImg.getWidth(null),BackImg.getHeight(null),null);
//			//判断是否有石头
//			if(flag&&time<10){
//				
//				g.drawImage(ToneImg,(int)TonePos.x-20, (int)TonePos.y-20
//						,(int)TonePos.x+20, (int)TonePos.y+20,0,0,
//						ToneImg.getWidth(null),ToneImg.getHeight(null),
//						null);
//						
//				time++;
//				if(time==9){
//					time=0;
//					flag=false;
//				}
//				
//			}
//			画我们的蜜蜂
			for(int i=0;i<PosNum;i++){
				g.drawImage(image, (int)p_Pos[i].x,(int)p_Pos[i].y, (int)p_Pos[i].x+40,(int)p_Pos[i].y+40, 0, 0
						, image.getWidth(null),image.getHeight(null),null);
			}
			
			double MaxDis=-200;
			for(int i=0;i<PosNum;i++){
				if(MaxDis<Function(p_Pos[i]))
					MaxDis=Function(p_Pos[i]);
			}
			
			if(MaxDis<20){
				finished=true;
			}
			
			try {
				Thread.sleep(40);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			g1.drawImage(img, 0, 0, null);
			
	
	}
	
	public void show(){
		
			for(int i=0;i<PosNum;i++){
				p_v[i].x=(new Random()).nextInt()%10;
				p_v[i].y=(new Random()).nextInt()%10;
				if(p_v[i].x>10)
					p_v[i].x=10;
				if(p_v[i].x<-10)
					p_v[i].x=-10;
				if(p_v[i].y>10)
					p_v[i].y=10;
				if(p_v[i].y<-10)
					p_v[i].y=-10;
				
				if((p_v[i].x+p_Pos[i].x)>0&&(p_v[i].x+p_Pos[i].x)<WIDTH){
						
						p_Pos[i].x+=p_v[i].x;
						
				}
				if((p_v[i].y+p_Pos[i].y)>0&&(p_v[i].y+p_Pos[i].y)<HEIGHT){
					p_Pos[i].y+=p_v[i].y;
				}
			}
			BufferedImage img=new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_RGB);
			g=img.getGraphics();
			//画背景
			g.drawImage(BackImg, 0,0,WIDTH
					,HEIGHT, 0, 0
					, BackImg.getWidth(null),BackImg.getHeight(null),null);

			
			//画蜜蜂
			for(int i=0;i<PosNum;i++){
				g.drawImage(image, (int)p_Pos[i].x,(int)p_Pos[i].y, (int)p_Pos[i].x+40,(int)p_Pos[i].y+40, 0, 0
						, image.getWidth(null),image.getHeight(null),null);

			}
			try {
				Thread.sleep(40);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			g1.drawImage(img, 0, 0, null);
		
	}
	
	public void run(){
		for(int i=0;i<PosNum/2;i++){
			p_v[i].x=(new Random()).nextInt()%80;
			p_v[i].y=(new Random()).nextInt()%80;
			if(p_v[i].x>-10&&p_v[i].x<-10)
				p_v[i].x*=2;
			if(p_v[i].y>-10&&p_v[i].y<-10)
				p_v[i].y*=2;
//			if(p_v[i].y>80)
//				p_v[i].y=80;
//			if(p_v[i].y<-80)
//				p_v[i].y=-80;
			
			if((p_v[i].x+p_Pos[i].x)>0&&(p_v[i].x+p_Pos[i].x)<WIDTH){
					
					p_Pos[i].x+=p_v[i].x;
					
			}
			if((p_v[i].y+p_Pos[i].y)>0&&(p_v[i].y+p_Pos[i].y)<HEIGHT){
				p_Pos[i].y+=p_v[i].y;
			}
		}
		for(int i=PosNum/2;i<PosNum;i++){
			p_v[i].x=(new Random()).nextInt()%30+50;
			p_v[i].y=(new Random()).nextInt()%30+50;
			if(p_v[i].x>80)
				p_v[i].x=80;
			if(p_v[i].x<-80)
				p_v[i].x=-80;
			if(p_v[i].y>80)
				p_v[i].y=80;
			if(p_v[i].y<-80)
				p_v[i].y=-80;
			
			if((p_v[i].x+p_Pos[i].x)>0&&(p_v[i].x+p_Pos[i].x)<WIDTH){
					
					p_Pos[i].x+=p_v[i].x;
					
			}
			if((p_v[i].y+p_Pos[i].y)>0&&(p_v[i].y+p_Pos[i].y)<HEIGHT){
				p_Pos[i].y+=p_v[i].y;
			}
		}
		BufferedImage img=new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_RGB);
		g=img.getGraphics();
		//画背景
		g.drawImage(BackImg, 0,0,WIDTH
				,HEIGHT, 0, 0
				, BackImg.getWidth(null),BackImg.getHeight(null),null);

		g.drawImage(ToneImg,(int)TonePos.x-40, (int)TonePos.y-40
				,(int)TonePos.x+40, (int)TonePos.y+40,0,0,
				ToneImg.getWidth(null),ToneImg.getHeight(null),
				null);
		//画蜜蜂
		for(int i=0;i<PosNum;i++){
			g.drawImage(image, (int)p_Pos[i].x,(int)p_Pos[i].y, (int)p_Pos[i].x+40,(int)p_Pos[i].y+40, 0, 0
					, image.getWidth(null),image.getHeight(null),null);

		}
		try {
			Thread.sleep(40);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		g1.drawImage(img, 0, 0, null);
	}
//	public static void main(String[] args){
//		TDimension td=new TDimension();
//		td.Target=new MyPoint(700.0d, 70.0d);
//		td.initial();
//		td.search();
//	}
	
}
```


别急，虽然上面还是有些代码的，但是事实上，全部都在套公式的，好吧，注意几点。



1.首先我们的速度是直接由公式得到的。

p_v[i].x=(w*p_v[i].x+c1*random.nextDouble()*(p_best[i].x-p_Pos[i].x)

                           +c2*random.nextDouble()*(g_best.x-p_Pos[i].x));

 p_v[i].y=w*p_v[i].y+c1*random.nextDouble()*(p_best[i].y-p_Pos[i].y)

                           +c2*random.nextDouble()*(g_best.y-p_Pos[i].y);

恶心吧，但是重点不是上面的，我是想说。


                if(p_v[i].x>10)

                    p_v[i].x=10;

                if(p_v[i].x<-10)

                    p_v[i].x=-10;

                if(p_v[i].y>10)

                    p_v[i].y=10;

                if(p_v[i].y<-10)

                    p_v[i].y=-10;


以上算出来的速度可能很大，就等于说如果每次如果调整的很大的话，极易出现一种情况，就是一会太左，一会太右，或者说刚才太小了，一会又太大了，所以我们可以按照精度和速度两方面来平衡下，这样效果会比较好些，这里我选的是20，因为我的整个画面区域为 1000多*1000多（没有记住。啊哈）

然后呢？大家基本上都会有这样的常识，就是 现在位置  s(t)=s( t - t' )+ v * t' ，额，这里直接默认 t'=1的啊，反正大家只要理解这个道理就可以啦。其实乘以多少都无所谓，反正就是一个速度嘛，我们其实就是看方向对不对，方向由什么决定的，其实在二维平面上就是由x/y嘛。

当然还有一点，就是我们怎么判断是否已经达到目标了呢？我是这样认为的：遍历蜜蜂当中所有的距离（距离目标），当最远距离都不超过我们给定的一个值时，我们认为基本上已经完成寻觅了嘛。

好了。以上就是我对于代码的解释啦。




但是好像忘了讲一件事了。就是关于双缓冲的时候，就是说呢？我们如果每隔30ms然后直接在JFrame上面绘制的话，就是直接传一个Jframe过去，或者是JFrame的画笔的话，这样应该会闪屏的吧，所以我们得考虑使用双缓冲。什么意思呢？我们先在一张新的画布上面画好我们想画的所有东西，画好后，直接打包把那个画布画上去嘛。额，其实就这么简单。我提醒一点：在使用双缓冲时，一般是说在一个地方进行绘制，就是不会在多个地方进行绘制，否则干嘛还要打包，打包就是说得打包干净嘛。有时候，想想，其实也蛮搞笑的话，如果用不好的话，就好比是
 一大堆的图片，一个一个画容易闪屏，现在如果把每一个都放进新的一个袋子里，然后再去画，反而会更累赘，因为除了那么多的图片，画的次数没少，还多了好多的袋子嘛。所以我们使用这个双缓冲就好比使用袋子，一次性就把他们全部打包好。







ok，今天就讲到这里，好吧，以上就是全部的代码，如果你觉得麻烦，可以点击下载。（ps免积分）哈哈。哈哈，备注下，链接
[点击打开链接](http://download.csdn.net/my/uploads) 到代码出，我先说明清楚，我是使用Eclipse，然后的话，我是直接取了一个包，你们可以新建一个工程，建一个包，然后Ctrl+v进去即可，然后的话，把图片放在src同一级的目录下，我说明清楚了哈。不过，有疑惑，留言哈，我看到尽可能解答吧。哈哈。晚安。
































