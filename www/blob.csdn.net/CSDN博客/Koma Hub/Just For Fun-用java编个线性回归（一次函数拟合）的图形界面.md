# Just For Fun-用java编个线性回归（一次函数拟合）的图形界面 - Koma Hub - CSDN博客
2018年04月25日 13:33:01[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：188
个人分类：[Java																[DeepLearning																[GUI																[Math																[Algorithm](https://blog.csdn.net/Rong_Toa/article/category/7221428)](https://blog.csdn.net/Rong_Toa/article/category/7610063)](https://blog.csdn.net/Rong_Toa/article/category/7517387)](https://blog.csdn.net/Rong_Toa/article/category/7517386)](https://blog.csdn.net/Rong_Toa/article/category/7156200)
先看效果：
![](https://img-blog.csdn.net/2018042513271089)
给的点和解得的方程是：
```
float x[] = {10,8,9,6,7,5,3,4,2,1,0,-1,-2,-4};
float y[] = {12,9,10,5,6,7,4,3,0,0,0,-3,-5,-8};
D:\test\java>javac Main.java
D:\test\java>java Main
squared error: 0.9610176
y = 1.328994*x -1.6994082
```
感觉还可以吧！来个最线性的试试：
线性拟合结果图：
![](https://img-blog.csdn.net/20180425133041578)
我采用的参数如下：
```
float x[] = {-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9};
float y[] = {-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9};
D:\test\java>javac Main.java
D:\test\java>java Main
squared error: 1.0
y = 1.0*x 0.0
```
下面贴上源代码，仅供参考，互相学习交流。
```java
/**
 * Linear Regression
 *    solution:y[] = a*x[] + b
 *
 *        2018.04: rongtao
 */
import javax.swing.*;
import java.awt.*;
/**
 *  JFrame : main window
 *
 */
class myJFrame extends JFrame
{
    private Toolkit toolkit; 
    
    myJFrame(String title, int width, int height)
    {
        this.setTitle(title);
        this.setSize(width, height);
        this.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        
        toolkit = getToolkit();  
        Dimension size = toolkit.getScreenSize();  
        setLocation((size.width/2 - getWidth())/2, (size.height - getWidth())/2); 
    }
}
/**
 *  JPanel: for drawing
 *
 */
class myJPanel extends JPanel
{
    private int lineColor, lineWidth;
    private int pointColor, pointWidth;
    private int width, height, numpoint;
    private float x[], y[], minx=99999, miny=99999, maxx=0, maxy=0;
    private float slope, intercept;
    private float radiox, radioy;
    private int bndr = 20;
    
    myJPanel(int lc, int lw, float x[], float y[], int pc, int pw, 
             float slope, float intercept)/*xielv, jiejv*/
    {
        this.lineColor = lc;
        this.lineWidth = lw;
        this.pointColor = pc;
        this.pointWidth = pw;
        this.x = x;
        this.y = y;
        this.slope = slope;
        this.intercept = intercept;
        this.numpoint = x.length<y.length?x.length:y.length;
        getArea();
    }
    /**
     *  painting
     *
     */
    public void paintComponent(Graphics g) 
    {
        super.paintComponent(g);
        
        Graphics2D g2d = (Graphics2D) g;  
  
        Dimension size = getSize();  
        Insets insets = getInsets(); 
        
        /* linear regression line */
        g2d.setColor(getLineColor(lineColor));  
        g2d.setStroke(getLineWidth(lineWidth)); 
        g2d.drawLine((int)((x[0]-minx)*1.0f/(maxx-minx)*(getWidth()-bndr*2))+bndr,
                     getHeight()-bndr-
                        (int)((x[0]*slope+intercept-miny)*1.0f
                                /(maxy-miny)*(getHeight()-bndr*2)),
                     (int)((x[numpoint-1]-minx)*1.0f/(maxx-minx)*(getWidth()-bndr*2))+bndr,
                     getHeight()-bndr-
                        (int)((x[numpoint-1]*slope+intercept-miny)*1.0f
                                /(maxy-miny)*(getHeight()-bndr*2))
                     ); 
        
        /* Axis */
        g2d.setColor(getLineColor(4));  
        g2d.setStroke(getLineWidth(1)); 
        /* Vertical axis */
        g2d.drawLine((int)((0-minx)*1.0f/(maxx-minx)*(getWidth()-bndr*2))+bndr,
                     getHeight()-bndr- (int)((miny-miny)*1.0f/(maxy-miny)*(getHeight()-bndr*2)),
                     (int)((0-minx)*1.0f/(maxx-minx)*(getWidth()-bndr*2))+bndr,
                     getHeight()-bndr- (int)((maxy-miny)*1.0f/(maxy-miny)*(getHeight()-bndr*2))
                     );
        /* Horizontal axis */
        g2d.drawLine((int)((0)*1.0f/(maxx-minx)*(getWidth()-bndr*2))+bndr,
                     getHeight()-bndr- (int)((0-miny)*1.0f/(maxy-miny)*(getHeight()-bndr*2)),
                     (int)((maxx-minx)*1.0f/(maxx-minx)*(getWidth()-bndr*2))+bndr,
                     getHeight()-bndr- (int)((0-miny)*1.0f/(maxy-miny)*(getHeight()-bndr*2))
                     );
        
        /* point */
        g2d.setColor(getLineColor(pointColor));  
        g2d.setStroke(getLineWidth(pointWidth));
        for(int i = 0; i < numpoint; i++)
        {
            g2d.drawLine((int)((x[i]-minx)*1.0f/(maxx-minx)*(getWidth()-bndr*2))+bndr,
                         getHeight()-bndr-(int)((y[i]-miny)*1.0f/(maxy-miny)*(getHeight()-bndr*2)),
                         (int)((x[i]-minx)*1.0f/(maxx-minx)*(getWidth()-bndr*2))+bndr,
                         getHeight()-bndr-(int)((y[i]-miny)*1.0f/(maxy-miny)*(getHeight()-bndr*2)));
        }
        
    }
    /* get raypath line width */  
    public BasicStroke getLineWidth(int dim)
    {  
        BasicStroke tmp = new BasicStroke(dim);  
        return tmp;  
    }  
    /* get raypath line color */  
    public Color getLineColor(int dim)
    { 
        if(dim==1)return Color.RED;  
        else if(dim==2)return Color.BLUE;  
        else if(dim==3)return Color.GREEN;  
        else if(dim==4)return Color.BLACK;  
        else if(dim==5)return Color.GRAY;  
        else if(dim==6)return Color.YELLOW;  
        else if(dim==7)return Color.PINK;  
        else if(dim==8)return Color.CYAN;  
        else if(dim==9)return Color.MAGENTA;  
        else if(dim==10)return Color.ORANGE;  
        else return Color.BLACK;  
    } 
    /* get drawing area */
    void getArea()
    {
        for(int i=0;i<numpoint;i++)
        {
            if(minx > x[i])
                minx = x[i];
            if(miny > y[i])
                miny = y[i];
            if(maxx < x[i])
                maxx = x[i];
            if(maxy < y[i])
                maxy = y[i];
        }
    }
}
/**
 *  linear regression calculation
 *
 */
class linearRegression
{
    private float x[], y[];
    private int numpoint=0;
    linearRegression(float x[], float y[])
    {
        this.x = x;
        this.y = y;
        this.numpoint = x.length<y.length?x.length:y.length;
    }
    float[] normalEquation()
    {
        /*line slope, line intercept, squared error*/
        float solution[] = new float[3];
        float SumX=0,SumY=0,SumXY=0,SumXX=0,SumYY=0,SumXAndSumY,SumXAndSumX,SumYAndSumY;
        
        for(int i=0;i< numpoint;i++) 
        {
            SumX=SumX+x[i];      
            SumY=SumY+y[i];   
            SumXY=SumXY+x[i]*y[i];
            SumXX=SumXX+x[i]*x[i];
            SumYY=SumYY+y[i]*y[i]; 
        }
        
        SumXAndSumY=SumX*SumY;  
        SumXAndSumX=SumX*SumX;   
        SumYAndSumY=SumY*SumY;   
        
        if((numpoint*SumXX-SumXAndSumX)>0)
        {
            /* line slope */
            solution[0] = (numpoint*SumXY-SumXAndSumY)/(numpoint*SumXX-SumXAndSumX);  
            /* line intercept */
            solution[1] = (SumY-solution[0]*SumX)/numpoint;  //intercept
        }
        
        if((numpoint*SumXX-SumXAndSumX)*(numpoint*SumYY-SumYAndSumY)>0)
            /* squared error */
            solution[2]=(numpoint*SumXY-SumXAndSumY)*(numpoint*SumXY-SumXAndSumY)
                    /((numpoint*SumXX-SumXAndSumX)*(numpoint*SumYY-SumYAndSumY));//R^2
        
        System.out.println("squared error: "+solution[2]);
        return solution;
    }
    
}
/**
 *  Main class
 * 
 */
public class Main
{
    static public void main(String[]args)
    {
        float x[] = {-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9};
        float y[] = {-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6,7,8,9};
        
        myJFrame jf = new myJFrame("Linear Regression",200,200);
        
        /*line slope, line intercept, squared error*/
        float solution[] = new linearRegression(x,y).normalEquation();
        
        /* lineColor, lineWidth, X, Y, pointColor, pointWidth, slope, intercept*/
        jf.add(new myJPanel(3,2,x,y,2,6,solution[0],solution[1]));
        
        System.out.println("y = "+solution[0]+"*x "+solution[1]);
        
        jf.setVisible(true);
    }
}
```
