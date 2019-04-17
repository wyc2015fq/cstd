# 【JAVA_GUI】单文件 JAVA_GUI 矩阵链动态规划算法 - CD's Coding - CSDN博客





2014年11月11日 21:57:03[糖果天王](https://me.csdn.net/okcd00)阅读数：568标签：[单文件																[JAVA_GUI																[矩阵链																[动态规划算法](https://so.csdn.net/so/search/s.do?q=动态规划算法&t=blog)
个人分类：[开发备忘																[作业](https://blog.csdn.net/okcd00/article/category/1727537)](https://blog.csdn.net/okcd00/article/category/2611293)





老师可萌可萌的~ 看下面的项目实验吐槽~

就是编界面挺恼火的……QtCreator的界面打死我都不想再碰第二次……以前的Bug制造出强大的心灵阴影

所以……废话不多说……START

## 项目要求

![](https://img-blog.csdn.net/20141111214530125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## 【重要部分代码说明】



```java
for(int i = 1; i <= n; i++)  
            {  
                g.setFont(fontLetter);  
                g.drawString("A", matrixXStart, YStart + di);  
                matrixXStart = matrixXStart + 12;  
                g.setFont(fontNumber);  
                g.drawString(String.valueOf(i), matrixXStart, YStart + di);  
                matrixXStart = matrixXStart + 10;  
                g.setFont(fontLetter);  
                g.drawString(": " + p[i - 1] + "*" + p[i] + " ", matrixXStart, YStart + di);  
                matrixXStart = matrixXStart + di * 3/2;  
            }
```
仔细看这里，所使用的是drawString(str,xpos,ypos),三个参数分别为字符串，x坐标和y坐标。



有了这个是不是就跟在PPT里写字符串一样了呢~





```java
for(int i = 1; i <= n; i++)  
        {  
            m[i][i] = 0;  
            int x = polygonArrayM[i][i].xpoints[0];  
            int y = polygonArrayM[i][i].ypoints[0];  
              
            setColorAndFill(g, Color.GREEN, polygonArrayM[i][i]);  
            setColorAndDrawStr(g, Color.BLACK, m[i][i], x, y, 10, 5);  
            sleepAndPossiblePause(interval);  
        }
```
setColorAndFill，如同字面意思，把【Graphics g】，填充为某种颜色，第三个参数是polygon，就是我要画的是哪一个方块。



setColorAndDrawStr则为填充颜色后把字符串（这里的m[i][i]是longlong型的）也扔上去。

以上两个为自写函数，但只是合并调用减少代码长度罢了，如下：



```java
private void setColorAndFill(Graphics g, Color c, Polygon p)    
    {    
        g.setColor(c);    
        g.fillPolygon(p);    
        g.setColor(Color.BLACK);    
        g.drawPolygon(p);    
    }    
        
    private void setColorAndDrawStr(Graphics g, Color c, Object value, int x, int y, int xDeviation, int yDeviation)    
    {    
        g.setColor(c);    
        g.drawString(String.valueOf(value), x + xDeviation, y + yDeviation);    
    }
```











```java
//睡眠并可能暂停  
    private void sleepAndPossiblePause(int interval)  
    {  
        try  
        {  
            Thread.sleep(interval);  
            synchronized(so)  
            {  
                while(so.status.equals("1"))  
                {  
                    so.wait();  
                }  
            }  
        }  
        catch (InterruptedException e)  
        {  
            e.printStackTrace();  
        }  
    }
```
这个很方便的暂停函数是借鉴来的，十分的方便，代码倒是相当短啦~



不过用在小游戏什么的里面作为pause使用应该也相当不错的。




## 【源码】





```java
package pack;

import java.awt.Color;  
import java.awt.Container;  
import java.awt.EventQueue;  
import java.awt.Font;  
import java.awt.Frame;  
import java.awt.Graphics;  
import java.awt.Polygon;  
import java.awt.event.ActionEvent;  
import java.awt.event.ActionListener;  
  
import java.util.Scanner;

import javax.swing.JButton;  
import javax.swing.JComboBox;  
import javax.swing.JFrame;  
import javax.swing.JLabel;  
import javax.swing.JOptionPane;  
import javax.swing.JPanel;  
import javax.swing.SwingUtilities;  
import javax.swing.WindowConstants;  
      
@SuppressWarnings("serial")
public class MatrixChainGUI extends JFrame  
{  
    int n;// Num of Rec  
    int di;//Length of dia_ratio  
    int XMStart;//M_Matrix left.xpos  
    int YStart;//Matrix left.pos
    int xSStart;//S_Matrix left.xpos  
    int rstStrXStart;//Answer_xpos
    Polygon[][] polygonArrayM;//m_Array  
    Polygon[][] polygonArrayS;//S_Array  
    int[] p;
    JPanel charPanel;  
    @SuppressWarnings("rawtypes")
	JComboBox change;  
    JButton calculate = new JButton("开始/暂停");  
    Container pane;  
    DrawPolygon d;//drawP
  //Rst_ WriteAnswer
    DrawCalculateRst dr;  
    Thread algThread;
  //Resume and Pause
    PauseOrResume pOR;  
    Thread pORThread;  
    SynObj so = new SynObj();//用以同步  
    int interval; //speed
    boolean awake = false;//init 
    Font fontLetter = new Font("letter", 0, 18);
    Font fontNumber = new Font("number", 0, 10);  
    Font fontBracket = new Font("number", 0, 17);  
    @SuppressWarnings({ "unchecked", "rawtypes" })
	public MatrixChainGUI(int[] p, int di)  
    {  
        super();  
        n = p.length - 1;  
        if(n <= 2)  
        {  
            JOptionPane.showMessageDialog(MatrixChainGUI.this,   
                  "矩阵数目最少为3", "", JOptionPane.ERROR_MESSAGE);  
            System.exit(0);  
        }  
        this.p = p;  
        this.di = di;  
        XMStart = di/2;  
        YStart = di * (n+2)/2;  
        xSStart = XMStart + n*di;  
        rstStrXStart = XMStart;  
        polygonArrayM = new Polygon[n+1][n+1];  
        polygonArrayS = new Polygon[n+1][n+1];  
          
        pane = this.getContentPane();  
        d = new DrawPolygon();  
        d.setIgnoreRepaint(true);  
        charPanel = new JPanel();  
        String str[] = { "1", "2", "3" };  
        change = new JComboBox(str);  
        charPanel.add(new JLabel("速度："));  
        charPanel.add(change);  
        charPanel.add(calculate);  
        pane.add("South", charPanel);  
        pane.add("Center", d);  
          
        calculate.addActionListener(new ActionListener()  
        {  
            public void actionPerformed(ActionEvent arg0)   
            {  
                if(so.status.equals("0"))  
                {  
                    pOR = new PauseOrResume();  
                    pORThread = new Thread(pOR);  
                    pORThread.start();//pause of resume Thread
                      
                    if(change.getSelectedItem().toString().equals("1"))  
                    {  
                        interval = 1000;  
                    }  
                    else if(change.getSelectedItem().toString().equals("2"))  
                    {  
                        interval = 600;  
                    }  
                    else  
                    {  
                        interval = 200;  
                    }  
                    dr = new DrawCalculateRst();  
                    pane.add(dr);  
                    algThread = new Thread(dr);  
                    algThread.start();//Calculate Thread
                }  
                //judge status
                else if(so.status.equals("2") || so.status.equals("1"))  
                {  
                    awake = true;  
                }  
            }  
        });  
    }  
      
    public class DrawPolygon extends JPanel  
    {  
        public void paintComponent(Graphics g)  
        {  
            super.paintComponents(g);  
            //PAINT Matrix_M
            int outCount = 1;  
            int q = 0;//纵坐标偏移单位  
            while(outCount < n + 1)  
            {  
                int inCount = 1;  
                int p = 0;//横坐标偏移单位  
                int j = outCount, i = 1;  
                while(inCount < n + 2 - outCount)  
                {  
                    int X[] = {XMStart+di*p+di/2*q,XMStart+di/2+di*p+di/2*q,XMStart+di+di*p+di/2*q,XMStart+di/2+di*p+di/2*q};  
                    int Y[] = {YStart-di/2*q,YStart+di/2-di/2*q,YStart-di/2*q,YStart-di/2-di/2*q};  
                    polygonArrayM[i][j] = new Polygon(X,Y,4);  
                    g.drawPolygon(polygonArrayM[i][j]);  
                    p++; i++; j++; inCount++;  
                }  
                q++; outCount++;  
            }  
              
            g.drawString("Matrix M_ij\n(m)", XMStart + di * n/2 - 25 , YStart - di * (n + 1)/2);  
              
            //Paint No.
            for(int i = 1; i <= n; i++)  
            {  
                int xl = polygonArrayM[1][i].xpoints[0];  
                int yl = polygonArrayM[1][i].ypoints[0];  
                int xr = polygonArrayM[i][n].xpoints[0];  
                int yr = polygonArrayM[i][n].ypoints[0];  
                  
                setColorAndDrawStr(g, Color.BLACK, i, xl, yl, 0, -9*di/25);  
                setColorAndDrawStr(g, Color.BLACK, i, xr, yr, 4*di/5, -9*di/25);  
            }  
              
            //Paint Axis
            int midv=n/2;
            int xj = polygonArrayM[1][midv].xpoints[0] -75;  
            int yj = polygonArrayM[1][midv].ypoints[0] +25;  
            int xi = polygonArrayM[midv+1][n].xpoints[0] ;  
            int yi = polygonArrayM[midv+1][n].ypoints[0] + 25;  
              
            setColorAndDrawStr(g, Color.BLACK, "Matrix M_ij\n(j)", xj, yj, 0, -di);  
            setColorAndDrawStr(g, Color.BLACK, "Matrix M_ij\n(i)", xi, yi, di, -di);  
              
            //Paint Matrix_S  
            int outCount1 = 2;  
            int q1 = 1;//纵坐标偏移单位  
            while(outCount1 < n + 1)  
            {  
                int inCount = 1;  
                int p = 1;//横坐标偏移单位  
                int j = outCount1, i = 1;  
                while(inCount < n + 2 - outCount1)  
                {  
                    int X[] = {xSStart+di*p+di/2*q1,xSStart+di/2+di*p+di/2*q1,xSStart+di+di*p+di/2*q1,xSStart+di/2+di*p+di/2*q1};  
                    int Y[] = {YStart-di/2*q1,YStart+di/2-di/2*q1,YStart-di/2*q1,YStart-di/2-di/2*q1};  
                    polygonArrayS[i][j] = new Polygon(X,Y,4);  
                    g.drawPolygon(polygonArrayS[i][j]);  
                    p++; i++; j++; inCount++;  
                }  
                q1++; outCount1++;  
            }  
              
            g.drawString("Value Matrix of S", xSStart + di * (n + 2)/2 - 50, YStart - di * (n + 1)/2);  
              
            //绘制s值编号  
            for(int i = 2; i <= n; i++)  
            {  
                int xl = polygonArrayS[1][i].xpoints[0];  
                int yl = polygonArrayS[1][i].ypoints[0];  
                  
                setColorAndDrawStr(g, Color.BLACK, i, xl, yl, 0, -9*di/25);  
            }  
            for(int i = 1; i <= n - 1; i++)  
            {  
  
                int xr = polygonArrayS[i][n].xpoints[0];  
                int yr = polygonArrayS[i][n].ypoints[0];  
                  
                setColorAndDrawStr(g, Color.BLACK, i, xr, yr, 4*di/5, -9*di/25);  
            }  
              
            int matrixXStart = XMStart;  
            g.setFont(fontLetter);  
        		g.drawString("[	CD's HINT	]: \t       [ Green ]	   Already Calculated ", matrixXStart, YStart + di*9/3);
        		g.drawString("[	IOTClass1	]: \t       [ Yellow]	   Now is Calculating ", matrixXStart, YStart + di*10/3);
        		g.drawString("[	20125209	]: \t        [ Pink ]	   Calculating Related", matrixXStart, YStart + di*11/3);
  
            for(int i = 1; i <= n; i++)  
            {  
                g.setFont(fontLetter);  
                g.drawString("A", matrixXStart, YStart + di);  
                matrixXStart = matrixXStart + 12;  
                g.setFont(fontNumber);  
                g.drawString(String.valueOf(i), matrixXStart, YStart + di);  
                matrixXStart = matrixXStart + 10;  
                g.setFont(fontLetter);  
                g.drawString(": " + p[i - 1] + "*" + p[i] + " ", matrixXStart, YStart + di);  
                matrixXStart = matrixXStart + di * 3/2;  
            }  
     
        }  
    }  
      
    public class SynObj  
    {  
        String status = "0";  
    }  
      
    public class DrawCalculateRst extends JPanel implements Runnable  
    {  
        public void run()   
        {  
            synchronized(so)  
            {  
                so.status = "2";  
            }  
            matrixChainMul(d.getGraphics());  
        }  
    }  
      
    public class PauseOrResume implements Runnable  
    {  
  
        public void run()   
        {  
            while(true)  
            {  
                while(!awake)  
                {  
                    try   
                    {  
                        Thread.sleep(50);  
                    }   
                    catch (InterruptedException e)   
                    {  
                        e.printStackTrace();  
                    }  
                }  
                // Runtime Set
                if(so.status.equals("1"))  
                {  
                    if(change.getSelectedItem().toString().equals("1"))  
                    {  
                        interval = 1000; 
                    }  
                    else if(change.getSelectedItem().toString().equals("2"))  
                    {  
                        interval = 600;  
                    }  
                    else  
                    {  
                        interval = 200;  
                    }  
                    synchronized(so)  
                    {  
                        so.status = "2";  
                        so.notifyAll();  
                    }  
                }  
                else if(so.status.equals("2"))  
                {  
                    so.status = "1";  
                }  
                awake = false;  
            }  
        }  
          
    }  
      
    //Main Algorithm Here 
    public void matrixChainMul(Graphics g)   
    {  
        long[][] m = new long[n + 1][n + 1];  
        int[][] s = new int[n][n + 1];  
          
        for(int i = 1; i <= n; i++)  
        {  
            m[i][i] = 0;  
            int x = polygonArrayM[i][i].xpoints[0];  
            int y = polygonArrayM[i][i].ypoints[0];  
              
            setColorAndFill(g, Color.GREEN, polygonArrayM[i][i]);  
            setColorAndDrawStr(g, Color.BLACK, m[i][i], x, y, 10, 5);  
            sleepAndPossiblePause(interval);  
        }  
          
        for(int l = 2; l <= n; l++)  
        {  
            for(int i = 1; i <= n - l + 1; i++)  
            {  
                int j = i + l - 1;  
                m[i][j] =  Integer.MAX_VALUE;  
                int x = polygonArrayM[i][j].xpoints[0];  
                int y = polygonArrayM[i][j].ypoints[0];  
                int xs = polygonArrayS[i][j].xpoints[0];  
                int ys = polygonArrayS[i][j].ypoints[0];  
                setColorAndFill(g, Color.YELLOW, polygonArrayM[i][j]);  
                setColorAndFill(g, Color.YELLOW, polygonArrayS[i][j]);  
                  
                sleepAndPossiblePause(interval);  
                  
                for(int k = i; k <= j - 1; k++)  
                {  
                    int x1 = polygonArrayM[i][k].xpoints[0];  
                    int y1 = polygonArrayM[i][k].ypoints[0];  
                    int x2 = polygonArrayM[k+1][j].xpoints[0];  
                    int y2 = polygonArrayM[k+1][j].ypoints[0];  
                    setColorAndFill(g, Color.PINK, polygonArrayM[i][k]);  
                    setColorAndDrawStr(g, Color.BLACK, m[i][k], x1, y1, 10, 5);  
                    setColorAndFill(g, Color.PINK, polygonArrayM[k+1][j]);  
                    setColorAndDrawStr(g, Color.BLACK, m[k+1][j], x2, y2, 10, 5);  
                    long q = m[i][k] + m[k+1][j] + p[i-1]*p[k]*p[j];  
                    if(q < m[i][j])  
                    {  
                        m[i][j] = q;  
                        s[i][j] = k;  
                    }  
                      
                    sleepAndPossiblePause(interval);  
                      
                    setColorAndFill(g, Color.GREEN, polygonArrayM[i][k]);  
                    setColorAndDrawStr(g, Color.BLACK, m[i][k], x1, y1, 10, 5);  
                    setColorAndFill(g, Color.GREEN, polygonArrayM[k+1][j]);  
                    setColorAndDrawStr(g, Color.BLACK, m[k+1][j], x2, y2, 10, 5);  
                }  
                setColorAndFill(g, Color.GREEN, polygonArrayM[i][j]);  
                setColorAndDrawStr(g, Color.BLACK, m[i][j], x, y, 10, 5);  
                setColorAndFill(g, Color.GREEN, polygonArrayS[i][j]);  
                setColorAndDrawStr(g, Color.BLACK, s[i][j], xs, ys, 10, 5);  
                  
                sleepAndPossiblePause(interval);  
            }  
        }  
        
        g.drawString("最优的加括号的序列： ", rstStrXStart, YStart + di * 2);  
        rstStrXStart = rstStrXStart + 120;  
        printRstSequence(s, 1, n, g);  
    }  
      
    //Paint Answer
    public void printRstSequence(int[][] s, int i, int j, Graphics g)  
    {  

        if(i == j)   
        {  
            g.setFont(fontLetter);  
            g.drawString("A", rstStrXStart, YStart + di * 2);  
            rstStrXStart = rstStrXStart + di/4;  
            g.setFont(fontNumber);  
            g.drawString(String.valueOf(i), rstStrXStart, YStart + di * 2);  
            rstStrXStart = rstStrXStart + di/4;  
            System.out.print("A" + i + " ");  
        }  
        else  
        { 
            g.setFont(fontBracket);  
            g.drawString("(", rstStrXStart, YStart + di * 2);  
            rstStrXStart = rstStrXStart + di/4;  
            System.out.print("(");  
            printRstSequence(s, i, s[i][j], g);  
            printRstSequence(s, s[i][j] + 1, j, g);  
            g.setFont(fontBracket);  
            g.drawString(")", rstStrXStart, YStart + di * 2);  
            rstStrXStart = rstStrXStart + di/4;  
            System.out.print(")");  
        }  
    }  
      
    //Set Sleep(Pause) Status
    private void sleepAndPossiblePause(int interval)  
    {  
        try  
        {  
            Thread.sleep(interval);  
            synchronized(so)  
            {  
                while(so.status.equals("1"))  
                {  
                    so.wait();  
                }  
            }  
        }  
        catch (InterruptedException e)  
        {  
            e.printStackTrace();  
        }  
    }  
  
    private void setColorAndFill(Graphics g, Color c, Polygon p)  
    {  
        g.setColor(c);  
        g.fillPolygon(p);  
        g.setColor(Color.BLACK);  
        g.drawPolygon(p);  
    }  
      
    private void setColorAndDrawStr(Graphics g, Color c, Object value, int x, int y, int xDeviation, int yDeviation)  
    {  
        g.setColor(c);  
        g.drawString(String.valueOf(value), x + xDeviation, y + yDeviation);  
    }  
      
    private static class FrameShower implements Runnable   
    {  
            
        private final Frame frame;  
          
        FrameShower(Frame frame)   
        {  
          this.frame = frame;  
        }  
          
        public void run()   
        {  
         frame.setVisible(true);  
        }  
          
    }     
      
    public static void main(String[] args)  
    {  
        SwingUtilities.invokeLater(new Runnable()  
        {  
            public void run()   
            {  
            	Scanner in=new Scanner(System.in);
                //矩阵行列数组  
            	//int[] p = {27, 33, 20, 18, 15, 18, 25};  
            	System.out.println("Please tell me how many matrix are there?(Recommend 6~15)");
                int vnum=in.nextInt();
                int [] p=new int[vnum+1];
                System.out.println("Please input values of these Matrix Size:");
            	System.out.printf("(Hint: %d matrix need %d numbers)\n",vnum,vnum+1);
                for(int i=0;i<p.length;i++)
                {
                	int tmp=in.nextInt();
                	if(tmp<=0){System.out.println("Invalid Input!");break;}
                	p[i]=tmp;
                }
                int di = 50;  
                int n = p.length - 1;  
                MatrixChainGUI c = new MatrixChainGUI(p, di);  
                c.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);  
                c.pack();  
                c.setSize(di * (2 * n + 2), di * (n+2)/2 + 280);  
                c.setResizable(false);  
                c.setTitle("MatrixChain_DP By CD_20125209");  
                EventQueue.invokeLater(new FrameShower(c));  
                System.out.println("单文件JAVA_GUI矩阵链动态规划算法演示结束，友情提示：这个可以打高分^_^");
                System.out.println("作者：陈点 物联网1班 20125209");
            }  
        });  
    }  
}
```


## 【运行演示】

![](https://img-blog.csdn.net/20141111220035359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20141111220051312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20141111220046373?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20141111220423557?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







## 【源码超·中文注释完全版】 





```java
package pack;

import java.awt.Color;  
import java.awt.Container;  
import java.awt.EventQueue;  
import java.awt.Font;  
import java.awt.Frame;  
import java.awt.Graphics;  
import java.awt.Polygon;  
import java.awt.event.ActionEvent;  
import java.awt.event.ActionListener;  
  
import java.util.Scanner;

import javax.swing.JButton;  
import javax.swing.JComboBox;  
import javax.swing.JFrame;  
import javax.swing.JLabel;  
import javax.swing.JOptionPane;  
import javax.swing.JPanel;  
import javax.swing.SwingUtilities;  
import javax.swing.WindowConstants;  
      
@SuppressWarnings("serial")
public class MatrixChainGUI extends JFrame  
{  
    int n;//矩阵个数  
    int di;//正方形斜对角线长  
    int XMStart;//m值菱形数组最左边的x坐标  
    int YStart;//菱形数组最左边的y坐标  
    int xSStart;//s值菱形数组最左边的x坐标  
    int rstStrXStart;//最优加括号矩阵链起始横坐标  
    Polygon[][] polygonArrayM;//m值菱形数组  
    Polygon[][] polygonArrayS;//s值菱形数组  
    int[] p;  
    JPanel charPanel;  
    @SuppressWarnings("rawtypes")
	JComboBox change;  
    JButton calculate = new JButton("开始/暂停");  
    Container pane;  
    DrawPolygon d;//画表格  
    DrawCalculateRst dr;//计算并向屏幕写结果的任务  
    Thread algThread;//计算向屏幕写结果的线程  
    PauseOrResume pOR;//暂停、恢复的任务  
    Thread pORThread;//暂停、恢复的线程  
    SynObj so = new SynObj();//用以同步  
    int interval;//计算速度  
    boolean awake = false;//暂停、恢复计算线程初始化为睡眠状态  
    Font fontLetter = new Font("letter", 0, 18);//字母字体  
    Font fontNumber = new Font("number", 0, 10);//角标字体  
    Font fontBracket = new Font("number", 0, 17);//括号字体  
    @SuppressWarnings({ "unchecked", "rawtypes" })
	public MatrixChainGUI(int[] p, int di)  
    {  
        super();  
        n = p.length - 1;  
        if(n <= 2)  
        {  
            JOptionPane.showMessageDialog(MatrixChainGUI.this,   
                  "矩阵数目最少为3", "", JOptionPane.ERROR_MESSAGE);  
            System.exit(0);  
        }  
        this.p = p;  
        this.di = di;  
        XMStart = di/2;//m值菱形数组最左边的x坐标  
        YStart = di * (n+2)/2;//菱形数组最左边的y坐标  
        xSStart = XMStart + n*di;//s值菱形数组最左边的x坐标  
        rstStrXStart = XMStart;//最优加括号矩阵链起始横坐标  
        polygonArrayM = new Polygon[n+1][n+1];//m值菱形数组  
        polygonArrayS = new Polygon[n+1][n+1];//s值菱形数组  
          
        pane = this.getContentPane();  
        d = new DrawPolygon();  
        d.setIgnoreRepaint(true);  
        charPanel = new JPanel();  
        String str[] = { "1", "2", "3" };  
        change = new JComboBox(str);  
        charPanel.add(new JLabel("速度："));  
        charPanel.add(change);  
        charPanel.add(calculate);  
        pane.add("South", charPanel);  
        pane.add("Center", d);  
          
        calculate.addActionListener(new ActionListener()  
        {  
            public void actionPerformed(ActionEvent arg0)   
            {  
                if(so.status.equals("0"))  
                {  
                    pOR = new PauseOrResume();  
                    pORThread = new Thread(pOR);  
                    pORThread.start();//启动暂停或恢复计算的线程  
                      
                    if(change.getSelectedItem().toString().equals("1"))  
                    {  
                        interval = 1000;  
                    }  
                    else if(change.getSelectedItem().toString().equals("2"))  
                    {  
                        interval = 600;  
                    }  
                    else  
                    {  
                        interval = 200;  
                    }  
                    dr = new DrawCalculateRst();  
                    pane.add(dr);  
                    algThread = new Thread(dr);  
                    algThread.start();//启动开始计算的线程  
                }  
                //若正在运行，则进入暂停状态;若已经暂停，则恢复  
                else if(so.status.equals("2") || so.status.equals("1"))  
                {  
                    awake = true;  
                }  
            }  
        });  
    }  
      
    public class DrawPolygon extends JPanel  
    {  
        public void paintComponent(Graphics g)  
        {  
            super.paintComponents(g);  
              
            //绘制m值表格  
            int outCount = 1;  
            int q = 0;//纵坐标偏移单位  
            while(outCount < n + 1)  
            {  
                int inCount = 1;  
                int p = 0;//横坐标偏移单位  
                int j = outCount, i = 1;  
                while(inCount < n + 2 - outCount)  
                {  
                    int X[] = {XMStart+di*p+di/2*q,XMStart+di/2+di*p+di/2*q,XMStart+di+di*p+di/2*q,XMStart+di/2+di*p+di/2*q};  
                    int Y[] = {YStart-di/2*q,YStart+di/2-di/2*q,YStart-di/2*q,YStart-di/2-di/2*q};  
                    polygonArrayM[i][j] = new Polygon(X,Y,4);  
                    g.drawPolygon(polygonArrayM[i][j]);  
                    p++;  
                    i++;  
                    j++;  
                    inCount++;  
                }  
                q++;  
                outCount++;  
            }  
              
            g.drawString("Matrix M_ij\n(m)", XMStart + di * n/2 - 25 , YStart - di * (n + 1)/2);  
              
            //绘制m值编号  
            for(int i = 1; i <= n; i++)  
            {  
                int xl = polygonArrayM[1][i].xpoints[0];  
                int yl = polygonArrayM[1][i].ypoints[0];  
                int xr = polygonArrayM[i][n].xpoints[0];  
                int yr = polygonArrayM[i][n].ypoints[0];  
                  
                setColorAndDrawStr(g, Color.BLACK, i, xl, yl, 0, -9*di/25);  
                setColorAndDrawStr(g, Color.BLACK, i, xr, yr, 4*di/5, -9*di/25);  
            }  
              
            //绘制i，j字母  
            int midv=n/2;
            int xj = polygonArrayM[1][midv].xpoints[0] -75;  
            int yj = polygonArrayM[1][midv].ypoints[0] +25;  
            int xi = polygonArrayM[midv+1][n].xpoints[0] ;  
            int yi = polygonArrayM[midv+1][n].ypoints[0] + 25;  
              
            setColorAndDrawStr(g, Color.BLACK, "Matrix M_ij\n(j)", xj, yj, 0, -di);  
            setColorAndDrawStr(g, Color.BLACK, "Matrix M_ij\n(i)", xi, yi, di, -di);  
              
            //绘制s值表格  
            int outCount1 = 2;  
            int q1 = 1;//纵坐标偏移单位  
            while(outCount1 < n + 1)  
            {  
                int inCount = 1;  
                int p = 1;//横坐标偏移单位  
                int j = outCount1, i = 1;  
                while(inCount < n + 2 - outCount1)  
                {  
                    int X[] = {xSStart+di*p+di/2*q1,xSStart+di/2+di*p+di/2*q1,xSStart+di+di*p+di/2*q1,xSStart+di/2+di*p+di/2*q1};  
                    int Y[] = {YStart-di/2*q1,YStart+di/2-di/2*q1,YStart-di/2*q1,YStart-di/2-di/2*q1};  
                    polygonArrayS[i][j] = new Polygon(X,Y,4);  
                    g.drawPolygon(polygonArrayS[i][j]);  
                    p++;  
                    i++;  
                    j++;  
                    inCount++;  
                }  
                q1++;  
                outCount1++;  
            }  
              
            g.drawString("Value Matrix of S", xSStart + di * (n + 2)/2 - 50, YStart - di * (n + 1)/2);  
              
            //绘制s值编号  
            for(int i = 2; i <= n; i++)  
            {  
                int xl = polygonArrayS[1][i].xpoints[0];  
                int yl = polygonArrayS[1][i].ypoints[0];  
                  
                setColorAndDrawStr(g, Color.BLACK, i, xl, yl, 0, -9*di/25);  
            }  
            for(int i = 1; i <= n - 1; i++)  
            {  
  
                int xr = polygonArrayS[i][n].xpoints[0];  
                int yr = polygonArrayS[i][n].ypoints[0];  
                  
                setColorAndDrawStr(g, Color.BLACK, i, xr, yr, 4*di/5, -9*di/25);  
            }  
              
            int matrixXStart = XMStart;  
            g.setFont(fontLetter);  
        		g.drawString("[	CD's HINT	]: \t       [ Green ]	   Already Calculated ", matrixXStart, YStart + di*9/3);
        		g.drawString("[	IOTClass1	]: \t       [ Yellow]	   Now is Calculating ", matrixXStart, YStart + di*10/3);
        		g.drawString("[	20125209	]: \t        [ Blue ]	   Calculating Related", matrixXStart, YStart + di*11/3);
  
            for(int i = 1; i <= n; i++)  
            {  
                g.setFont(fontLetter);  
                g.drawString("A", matrixXStart, YStart + di);  
                matrixXStart = matrixXStart + 12;  
                g.setFont(fontNumber);  
                g.drawString(String.valueOf(i), matrixXStart, YStart + di);  
                matrixXStart = matrixXStart + 10;  
                g.setFont(fontLetter);  
                g.drawString(": " + p[i - 1] + "*" + p[i] + " ", matrixXStart, YStart + di);  
                matrixXStart = matrixXStart + di * 3/2;  
            }  
     
        }  
    }  
      
    public class SynObj  
    {  
        String status = "0";  
    }  
      
    public class DrawCalculateRst extends JPanel implements Runnable  
    {  
        public void run()   
        {  
            synchronized(so)  
            {  
                so.status = "2";  
            }  
            matrixChainMul(d.getGraphics());  
        }  
    }  
      
    public class PauseOrResume implements Runnable  
    {  
  
        public void run()   
        {  
            while(true)  
            {  
                while(!awake)  
                {  
                    try   
                    {  
                        Thread.sleep(50);  
                    }   
                    catch (InterruptedException e)   
                    {  
                        e.printStackTrace();  
                    }  
                }  
                if(so.status.equals("1"))  
                {  
                    if(change.getSelectedItem().toString().equals("1"))  
                    {  
                        interval = 1000;  
                    }  
                    else if(change.getSelectedItem().toString().equals("2"))  
                    {  
                        interval = 600;  
                    }  
                    else  
                    {  
                        interval = 200;  
                    }  
                    synchronized(so)  
                    {  
                        so.status = "2";  
                        so.notifyAll();  
                    }  
                }  
                else if(so.status.equals("2"))  
                {  
                    so.status = "1";  
                }  
                awake = false;  
            }  
        }  
          
    }  
      
    //矩阵链乘法最优算序寻找算法  
    public void matrixChainMul(Graphics g)   
    {  
        long[][] m = new long[n + 1][n + 1];  
        int[][] s = new int[n][n + 1];  
          
        for(int i = 1; i <= n; i++)  
        {  
            m[i][i] = 0;  
            int x = polygonArrayM[i][i].xpoints[0];  
            int y = polygonArrayM[i][i].ypoints[0];  
              
            setColorAndFill(g, Color.GREEN, polygonArrayM[i][i]);  
            setColorAndDrawStr(g, Color.BLACK, m[i][i], x, y, 10, 5);  
            sleepAndPossiblePause(interval);  
        }  
          
        for(int l = 2; l <= n; l++)  
        {  
            for(int i = 1; i <= n - l + 1; i++)  
            {  
                int j = i + l - 1;  
                m[i][j] =  Integer.MAX_VALUE;  
                int x = polygonArrayM[i][j].xpoints[0];  
                int y = polygonArrayM[i][j].ypoints[0];  
                int xs = polygonArrayS[i][j].xpoints[0];  
                int ys = polygonArrayS[i][j].ypoints[0];  
                setColorAndFill(g, Color.YELLOW, polygonArrayM[i][j]);  
                setColorAndFill(g, Color.YELLOW, polygonArrayS[i][j]);  
                  
                sleepAndPossiblePause(interval);  
                  
                for(int k = i; k <= j - 1; k++)  
                {  
                    int x1 = polygonArrayM[i][k].xpoints[0];  
                    int y1 = polygonArrayM[i][k].ypoints[0];  
                    int x2 = polygonArrayM[k+1][j].xpoints[0];  
                    int y2 = polygonArrayM[k+1][j].ypoints[0];  
                    setColorAndFill(g, Color.BLUE, polygonArrayM[i][k]);  
                    setColorAndDrawStr(g, Color.BLACK, m[i][k], x1, y1, 10, 5);  
                    setColorAndFill(g, Color.BLUE, polygonArrayM[k+1][j]);  
                    setColorAndDrawStr(g, Color.BLACK, m[k+1][j], x2, y2, 10, 5);  
                    long q = m[i][k] + m[k+1][j] + p[i-1]*p[k]*p[j];  
                    if(q < m[i][j])  
                    {  
                        m[i][j] = q;  
                        s[i][j] = k;  
                    }  
                      
                    sleepAndPossiblePause(interval);  
                      
                    setColorAndFill(g, Color.GREEN, polygonArrayM[i][k]);  
                    setColorAndDrawStr(g, Color.BLACK, m[i][k], x1, y1, 10, 5);  
                    setColorAndFill(g, Color.GREEN, polygonArrayM[k+1][j]);  
                    setColorAndDrawStr(g, Color.BLACK, m[k+1][j], x2, y2, 10, 5);  
                }  
                setColorAndFill(g, Color.GREEN, polygonArrayM[i][j]);  
                setColorAndDrawStr(g, Color.BLACK, m[i][j], x, y, 10, 5);  
                setColorAndFill(g, Color.GREEN, polygonArrayS[i][j]);  
                setColorAndDrawStr(g, Color.BLACK, s[i][j], xs, ys, 10, 5);  
                  
                sleepAndPossiblePause(interval);  
            }  
        }  
          
          
        g.drawString("最优的加括号的序列： ", rstStrXStart, YStart + di * 2);  
        rstStrXStart = rstStrXStart + 120;  
        printRstSequence(s, 1, n, g);  
    }  
      
    //打印加括号顺序  
    public void printRstSequence(int[][] s, int i, int j, Graphics g)  
    {  

        if(i == j)   
        {  
            g.setFont(fontLetter);  
            g.drawString("A", rstStrXStart, YStart + di * 2);  
            rstStrXStart = rstStrXStart + di/4;  
            g.setFont(fontNumber);  
            g.drawString(String.valueOf(i), rstStrXStart, YStart + di * 2);  
            rstStrXStart = rstStrXStart + di/4;  
            System.out.print("A" + i + " ");  
        }  
        else  
        { 
            g.setFont(fontBracket);  
            g.drawString("(", rstStrXStart, YStart + di * 2);  
            rstStrXStart = rstStrXStart + di/4;  
            System.out.print("(");  
            printRstSequence(s, i, s[i][j], g);  
            printRstSequence(s, s[i][j] + 1, j, g);  
            g.setFont(fontBracket);  
            g.drawString(")", rstStrXStart, YStart + di * 2);  
            rstStrXStart = rstStrXStart + di/4;  
            System.out.print(")");  
        }  
    }  
      
    //睡眠并可能暂停  
    private void sleepAndPossiblePause(int interval)  
    {  
        try  
        {  
            Thread.sleep(interval);  
            synchronized(so)  
            {  
                while(so.status.equals("1"))  
                {  
                    so.wait();  
                }  
            }  
        }  
        catch (InterruptedException e)  
        {  
            e.printStackTrace();  
        }  
    }  
  
    private void setColorAndFill(Graphics g, Color c, Polygon p)  
    {  
        g.setColor(c);  
        g.fillPolygon(p);  
        g.setColor(Color.BLACK);  
        g.drawPolygon(p);  
    }  
      
    private void setColorAndDrawStr(Graphics g, Color c, Object value, int x, int y, int xDeviation, int yDeviation)  
    {  
        g.setColor(c);  
        g.drawString(String.valueOf(value), x + xDeviation, y + yDeviation);  
    }  
      
    private static class FrameShower implements Runnable   
    {  
            
        private final Frame frame;  
          
        FrameShower(Frame frame)   
        {  
          this.frame = frame;  
        }  
          
        public void run()   
        {  
         frame.setVisible(true);  
        }  
          
    }     
      
    public static void main(String[] args)  
    {  
        SwingUtilities.invokeLater(new Runnable()  
        {  
            public void run()   
            {  
            	Scanner in=new Scanner(System.in);
                //矩阵行列数组  
            	//int[] p = {27, 33, 20, 18, 15, 18, 25};  
            	System.out.println("Please tell me how many matrix are there?(Recommend 6~15)");
                int vnum=in.nextInt();
                int [] p=new int[vnum+1];
                System.out.println("Please input values of these Matrix Size:");
            	System.out.printf("(Hint: %d matrix need %d numbers)\n",vnum,vnum+1);
                for(int i=0;i<p.length;i++)
                {
                	int tmp=in.nextInt();
                	if(tmp<=0){System.out.println("Invalid Input!");break;}
                	p[i]=tmp;
                }
                int di = 50;  
                int n = p.length - 1;  
                MatrixChainGUI c = new MatrixChainGUI(p, di);  
                c.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);  
                c.pack();  
                c.setSize(di * (2 * n + 2), di * (n+2)/2 + 280);  
                c.setResizable(false);  
                c.setTitle("MatrixChain_DP By CD_20125209");  
                EventQueue.invokeLater(new FrameShower(c));  
                System.out.println("单文件JAVA_GUI矩阵链动态规划算法演示结束，友情提示：这个可以打高分^_^");
                System.out.println("作者：陈点 物联网1班 20125209");
            }  
        });  
    }  
}
```










==== 部分非原创说明 ====

（以下提及的辅助函数中注释大多为xglv2013的函数自带注释）

此版本依照 software8_co的 Swing动态演示中的

· “线程”概念 （线程函数摘自网络教程）

· 三速切换控制 （此函数摘自xglv）](https://so.csdn.net/so/search/s.do?q=矩阵链&t=blog)](https://so.csdn.net/so/search/s.do?q=JAVA_GUI&t=blog)](https://so.csdn.net/so/search/s.do?q=单文件&t=blog)




