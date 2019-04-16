# 图像处理之Fuzzy C Means的聚合算法 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年04月14日 16:27:41[gloomyfish](https://me.csdn.net/jia20003)阅读数：14256
所属专栏：[Java数字图像处理与特效](https://blog.csdn.net/column/details/image-process.html)









Fuzzy C-Means聚合算法在图像分割(segmentation)和图像视觉处理中常常被用到聚合算法之

一本文是完全基于JAVA语言实现Fuzzy C-Means聚合算法，并可以运用到图像处理中实现简

单的对象提取。




**一：数学原理**

在解释数学原理之前，请先看看这个链接算是热身吧

[http://home.deib.polimi.it/matteucc/Clustering/tutorial_html/cmeans.html](http://home.deib.polimi.it/matteucc/Clustering/tutorial_html/cmeans.html)。

看不懂没关系。我的解释足够详细，小学毕业都可以学会，本人就是小学毕业。

Fuzzy C-means算法主要是比较RGB空间的每个像素值与Cluster中的每个中心点值，最终给

每个像素指派一个值(0~1之间)说明该像素更接近于哪里Cluster的中心点，模糊规则是该像

素对所有cluster的值之和为1。简单的举例：假设图像中有三个聚类cluster1，cluster2，cluster3，

像素A对三个聚类的值分别为a1, a2, a3, 根据模糊规则a1 + a2 + a3 = 1。更进一步，如果a1

最大，则该像素比较接近于Cluster1。计算总的对象值**J**：

![](https://img-my.csdn.net/uploads/201304/14/1365927277_5974.png)


**二：算法流程**

初始输入参数:

a.      指定的聚类个数numberOfClusters,

b.      指定的最大循环次数maxIteration

c.      指定的最小终止循环差值deltaValue

大致流程如下：

1.      初始化所有像素点值与随机选取每个Cluster的中心点，初始化每个像素点P[i]对应

Cluster的模糊值p[i][k]并计算cluster index。

2.      计算对象值J

3.      计算每个Cluster的颜色值，产生新的图像像素

4.      计算每个像素的对应每个cluster的模糊值，更新每个像素的Cluster Index

5.      再次计算对象值J，并与第二步的对象值相减，如果差值小于deltaValue或者达到最大

循环数，停止计算输出结果图像，否则继续2 ～ 4

**三：关键代码解析**

欧几里德距离计算方法如下：



```java
private double calculateEuclideanDistance(ClusterPoint p, ClusterCentroid c) 
{
	// int pa = (p.getPixelColor() >> 24) & 0xff;
    int pr = (p.getPixelColor() >> 16) & 0xff;
    int pg = (p.getPixelColor() >> 8) & 0xff;
    int pb = p.getPixelColor() & 0xff;
    // int ca = (c.getPixelColor() >> 24) & 0xff;
    int cr = (c.getPixelColor() >> 16) & 0xff;
    int cg = (c.getPixelColor() >> 8) & 0xff;
    int cb = c.getPixelColor() & 0xff;
    
    return Math.sqrt(Math.pow((pr - cr), 2.0) + Math.pow((pg - cg), 2.0) + Math.pow((pb - cb), 2.0));
}
```



计算每个像素与每个Cluster的Fuzzy数值的代码如下：



```java
public void stepFuzzy()
{
    for (int c = 0; c < this.clusters.size(); c++)
    {
        for (int h = 0; h < this.points.size(); h++)
        {

            double top;
            top = calculateEuclideanDistance(this.points.get(h), this.clusters.get(c));
            if (top < 1.0) top = Eps;

            // sumTerms is the sum of distances from this data point to all clusters.
            double sumTerms = 0.0;

            for (int ck = 0; ck < this.clusters.size(); ck++)
            {
                sumTerms += top / calculateEuclideanDistance(this.points.get(h), this.clusters.get(ck));

            }
            // Then the membership value can be calculated as...
            fuzzyForPixels[h][c] = (double)(1.0 / Math.pow(sumTerms, (2 / (this.fuzzy - 1)))); 
        }
    };


    this.recalculateClusterMembershipValues();
}
```



计算并更新每个像素的Cluster index的代码如下：



```java
private void recalculateClusterMembershipValues() 
{

    for (int i = 0; i < this.points.size(); i++)
   {
       double max = 0.0;
       double min = 0.0;
       double sum = 0.0;
       double newmax = 0;
       ClusterPoint p = this.points.get(i);
       //Normalize the entries
       for (int j = 0; j < this.clusters.size(); j++)
       {
           max = fuzzyForPixels[i][j] > max ? fuzzyForPixels[i][j] : max;
           min = fuzzyForPixels[i][j] < min ? fuzzyForPixels[i][j] : min;
       }
       //Sets the values to the normalized values between 0 and 1
       for (int j = 0; j < this.clusters.size(); j++)
       {
    	   fuzzyForPixels[i][j] = (fuzzyForPixels[i][j] - min) / (max - min);
           sum += fuzzyForPixels[i][j];
       }
       //Makes it so that the sum of all values is 1 
       for (int j = 0; j < this.clusters.size(); j++)
       {
    	   fuzzyForPixels[i][j] = fuzzyForPixels[i][j] / sum;
           if (Double.isNaN(fuzzyForPixels[i][j]))
           {
        	   fuzzyForPixels[i][j] = 0.0;
           }
           newmax = fuzzyForPixels[i][j] > newmax ? fuzzyForPixels[i][j] : newmax;
       }
       // ClusterIndex is used to store the strongest membership value to a cluster, used for defuzzification
        p.setClusterIndex(newmax);
     };
}
```



四：运行效果

![](https://img-my.csdn.net/uploads/201304/14/1365927873_9248.png)


五：算法源代码

**FuzzyCMeansProcessor - 算法类**




```java
package com.gloomyfish.segmentation.fuzzycmeans;

import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import com.gloomyfish.filter.study.AbstractBufferedImageOp;

public class FuzzyCMeansProcessor extends AbstractBufferedImageOp {
	
	private List<ClusterPoint> points;
	private List<ClusterCentroid> clusters;
	private BufferedImage originalImage;
	private BufferedImage processedImage;
	private double Eps = Math.pow(10, -5);

    private double[][] fuzzyForPixels;
    
    // Gets or sets objective function
    private double numObj;
    
    public void setObj(double j) {
    	this.numObj = j;
    }
    
    public double getObj() {
    	return this.numObj;
    }
	
	private float fuzzy; // default is 2
	private int numCluster; // number of clusters in image
	
	public BufferedImage getResultImage()
	{
		return this.processedImage;
	}
	
	public FuzzyCMeansProcessor(/*List<ClusterPoint> points, List<ClusterCentroid> clusters, */float fuzzy, BufferedImage myImage, int numCluster)
	{
        points = new ArrayList<ClusterPoint>();

        int width = myImage.getWidth();
        int height = myImage.getHeight();
        int index = 0;
        int[] inPixels = new int[width*height];
        myImage.getRGB( 0, 0, width, height, inPixels, 0, width );
        for (int row = 0; row < myImage.getHeight(); ++row)
        {
            for (int col = 0; col < myImage.getWidth(); ++col)
            {
            	index = row * width + col;
            	int color = inPixels[index];
                points.add(new ClusterPoint(row, col, color));

            }
        }



        clusters = new ArrayList<ClusterCentroid>();
       
        //Create random points to use a the cluster centroids
        Random random = new Random();
        for (int i = 0; i < numCluster; i++)
        {
            int randomNumber1 = random.nextInt(width);
            int randomNumber2 = random.nextInt(height);
            index = randomNumber2 * width + randomNumber1;
            clusters.add(new ClusterCentroid(randomNumber1, randomNumber2, inPixels[index])); 
        }
        
        this.originalImage = myImage;
        this.fuzzy = fuzzy;
        this.numCluster = numCluster;
        
        double diff;

        // Iterate through all points to create initial U matrix
        fuzzyForPixels = new double[this.points.size()][this.clusters.size()];
        for (int i = 0; i < this.points.size(); i++)
        {
            ClusterPoint p = points.get(i);
            double sum = 0.0;

            for (int j = 0; j < this.clusters.size(); j++)
            {
                ClusterCentroid c = this.clusters.get(j);
                diff = Math.sqrt(Math.pow(calculateEuclideanDistance(p, c), 2.0));
                fuzzyForPixels[i][j] = (diff == 0) ? Eps : diff;
                sum += fuzzyForPixels[i][j];
            }
         }
        
        // re-calculate the membership value for one point of all clusters, and make suer it's sum of value is 1
        recalculateClusterMembershipValues();
        
	}
	
    public void calculateClusterCentroids()
    {
        for (int j = 0; j < this.clusters.size(); j++)
        {
            ClusterCentroid clusterCentroid = this.clusters.get(j);
            
            double l = 0.0;
            clusterCentroid.setRedSum(0);
            clusterCentroid.setBlueSum(0);
            clusterCentroid.setGreenSum(0);
            clusterCentroid.setMemberShipSum(0);
            double redSum = 0;
            double greenSum = 0;
            double blueSum = 0;
            double memebershipSum = 0;
            double pixelCount = 1;

            for (int i = 0; i < this.points.size(); i++)
            {
            
                ClusterPoint p = this.points.get(i);
                l = Math.pow(fuzzyForPixels[i][j], this.fuzzy);
        		int ta = (p.getPixelColor() >> 24) & 0xff;
                int tr = (p.getPixelColor() >> 16) & 0xff;
                int tg = (p.getPixelColor() >> 8) & 0xff;
                int tb = p.getPixelColor() & 0xff;
                redSum += l * tr;
                greenSum += l * tg;
                blueSum += l * tb;
                memebershipSum += l;

                if (fuzzyForPixels[i][j] == p.getClusterIndex())
                {
                	pixelCount += 1;
                }
            }
            
            int clusterColor = (255 << 24) | ((int)(redSum / memebershipSum) << 16) | ((int)(greenSum / memebershipSum) << 8) | (int)(blueSum / memebershipSum);
            clusterCentroid.setPixelColor(clusterColor);
         }

        //update the original image
        // Bitmap tempImage = new Bitmap(myImageWidth, myImageHeight, PixelFormat.Format32bppRgb);
        BufferedImage tempImage = createCompatibleDestImage( originalImage, null );
        int width = tempImage.getWidth();
        int height = tempImage.getHeight();
        int index = 0;
        int[] outPixels = new int[width*height];
        
        for (int j = 0; j < this.points.size(); j++)
        {
            for (int i = 0; i < this.clusters.size(); i++)
            {
                ClusterPoint p = this.points.get(j);
                if (fuzzyForPixels[j][i] == p.getClusterIndex())
                {
                	int row = (int)p.getX(); // row
                	int col = (int)p.getY(); // column
                	index = row * width + col;
                	outPixels[index] = this.clusters.get(i).getPixelColor();
                }
            }
        }
        
        // fill the pixel data
        setRGB( tempImage, 0, 0, width, height, outPixels );
        processedImage = tempImage;
    }
    
    /// <summary>
    /// Perform one step of the algorithm
    /// </summary>
    public void stepFuzzy()
    {
        for (int c = 0; c < this.clusters.size(); c++)
        {
            for (int h = 0; h < this.points.size(); h++)
            {

                double top;
                top = calculateEuclideanDistance(this.points.get(h), this.clusters.get(c));
                if (top < 1.0) top = Eps;

                // sumTerms is the sum of distances from this data point to all clusters.
                double sumTerms = 0.0;

                for (int ck = 0; ck < this.clusters.size(); ck++)
                {
                    sumTerms += top / calculateEuclideanDistance(this.points.get(h), this.clusters.get(ck));

                }
                // Then the membership value can be calculated as...
                fuzzyForPixels[h][c] = (double)(1.0 / Math.pow(sumTerms, (2 / (this.fuzzy - 1)))); 
            }
        };


        this.recalculateClusterMembershipValues();
    }
	
    public double calculateObjectiveFunction()
    {
        double Jk = 0.0;

        for (int i = 0; i < this.points.size();i++)
        {
            for (int j = 0; j < this.clusters.size(); j++)
            {
                Jk += Math.pow(fuzzyForPixels[i][j], this.fuzzy) * Math.pow(this.calculateEuclideanDistance(points.get(i), clusters.get(j)), 2);
            }
        }
        return Jk;
    }
    
	
	private void recalculateClusterMembershipValues() 
	{
	
	    for (int i = 0; i < this.points.size(); i++)
	   {
	       double max = 0.0;
	       double min = 0.0;
	       double sum = 0.0;
	       double newmax = 0;
	       ClusterPoint p = this.points.get(i);
	       //Normalize the entries
	       for (int j = 0; j < this.clusters.size(); j++)
	       {
	           max = fuzzyForPixels[i][j] > max ? fuzzyForPixels[i][j] : max;
	           min = fuzzyForPixels[i][j] < min ? fuzzyForPixels[i][j] : min;
	       }
	       //Sets the values to the normalized values between 0 and 1
	       for (int j = 0; j < this.clusters.size(); j++)
	       {
	    	   fuzzyForPixels[i][j] = (fuzzyForPixels[i][j] - min) / (max - min);
	           sum += fuzzyForPixels[i][j];
	       }
	       //Makes it so that the sum of all values is 1 
	       for (int j = 0; j < this.clusters.size(); j++)
	       {
	    	   fuzzyForPixels[i][j] = fuzzyForPixels[i][j] / sum;
	           if (Double.isNaN(fuzzyForPixels[i][j]))
	           {
	        	   fuzzyForPixels[i][j] = 0.0;
	           }
	           newmax = fuzzyForPixels[i][j] > newmax ? fuzzyForPixels[i][j] : newmax;
	       }
	       // ClusterIndex is used to store the strongest membership value to a cluster, used for defuzzification
	        p.setClusterIndex(newmax);
	     };
	}

	private double calculateEuclideanDistance(ClusterPoint p, ClusterCentroid c) 
	{
		// int pa = (p.getPixelColor() >> 24) & 0xff;
	    int pr = (p.getPixelColor() >> 16) & 0xff;
	    int pg = (p.getPixelColor() >> 8) & 0xff;
	    int pb = p.getPixelColor() & 0xff;
	    // int ca = (c.getPixelColor() >> 24) & 0xff;
	    int cr = (c.getPixelColor() >> 16) & 0xff;
	    int cg = (c.getPixelColor() >> 8) & 0xff;
	    int cb = c.getPixelColor() & 0xff;
	    
	    return Math.sqrt(Math.pow((pr - cr), 2.0) + Math.pow((pg - cg), 2.0) + Math.pow((pb - cb), 2.0));
	}

	@Override
	public BufferedImage filter(BufferedImage src, BufferedImage dest) {
		return processedImage;
	}

}
```
**ClusterPoint- 存储图像像素点对象**




```java
package com.gloomyfish.segmentation.fuzzycmeans;

public class ClusterPoint {
	private double x;
	private double y;
	private int pixelColor;
	private int originalPixelColor;
	private double clusterIndex;
	
    public ClusterPoint(double x, double y, int col)
	{
		this.x = x;
        this.y = y;
        this.pixelColor = col;
        this.originalPixelColor = col;
        this.clusterIndex = -1;
	}
    
	public double getX() {
		return x;
	}

	public void setX(double x) {
		this.x = x;
	}

	public double getY() {
		return y;
	}

	public void setY(double y) {
		this.y = y;
	}

	public int getPixelColor() {
		return pixelColor;
	}

	public void setPixelColor(int pixelColor) {
		this.pixelColor = pixelColor;
	}

	public int getOriginalPixelColor() {
		return originalPixelColor;
	}

	public void setOriginalPixelColor(int originalPixelColor) {
		this.originalPixelColor = originalPixelColor;
	}

	public double getClusterIndex() {
		return clusterIndex;
	}

	public void setClusterIndex(double clusterIndex) {
		this.clusterIndex = clusterIndex;
	}

}
```
**ClusterCentroid - 存储Cluster信息对象**




```java
package com.gloomyfish.segmentation.fuzzycmeans;

public class ClusterCentroid {

	private double x;
	private double y;
	private int pixelColor;
	private double redSum;
	private double greenSum;
	private double blueSum;
	private double memberShipSum;
	private int originalPixelColor;
	
    public ClusterCentroid(double x, double y, int color)
    {
    	this.x = x;
    	this.y = y;
    	this.originalPixelColor = color;
    	this.pixelColor = color;
    }
    
	public double getX() {
		return x;
	}

	public void setX(double x) {
		this.x = x;
	}

	public double getY() {
		return y;
	}

	public void setY(double y) {
		this.y = y;
	}

	public int getPixelColor() {
		return pixelColor;
	}

	public void setPixelColor(int pixelColor) {
		this.pixelColor = pixelColor;
	}

	public double getRedSum() {
		return redSum;
	}

	public void setRedSum(double redSum) {
		this.redSum = redSum;
	}

	public double getGreenSum() {
		return greenSum;
	}

	public void setGreenSum(double greenSum) {
		this.greenSum = greenSum;
	}

	public double getBlueSum() {
		return blueSum;
	}

	public void setBlueSum(double blueSum) {
		this.blueSum = blueSum;
	}

	public double getMemberShipSum() {
		return memberShipSum;
	}

	public void setMemberShipSum(double memberShipSum) {
		this.memberShipSum = memberShipSum;
	}

	public int getOriginalPixelColor() {
		return originalPixelColor;
	}

	public void setOriginalPixelColor(int originalPixelColor) {
		this.originalPixelColor = originalPixelColor;
	}

}
```



**算法调用：**

****

```java
int numClusters = 2; // (int)numericUpDown2.Value;
  int maxIterations = 20; //(int)numericUpDown3.Value;
  double accuracy = 0.00001; // (double)numericUpDown4.Value;
  FuzzyCMeansProcessor alg = new FuzzyCMeansProcessor(numClusters, sourceImage, numClusters);
  int k = 0;
  do
  {
      k++;
      alg.setObj(alg.calculateObjectiveFunction());
      alg.calculateClusterCentroids();
      alg.stepFuzzy();
      double Jnew = alg.calculateObjectiveFunction();
      System.out.println("Run method accuracy of delta value = " + Math.abs(alg.getObj() - Jnew));
      if (Math.abs(alg.getObj() - Jnew) < accuracy) break;
  }
  while (maxIterations > k);
  resultImage = alg.getResultImage();
  this.repaint();
}
```

**六：Fuzzy C-means不足之处**

需要提供额外的参数，不能自动识别Cluster，运行时间比较长。

**博客从本月开始更新，请关注！！谢谢谢！**



