# 图像处理之K-Means算法演示 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2013年04月20日 17:49:15[gloomyfish](https://me.csdn.net/jia20003)阅读数：22811
所属专栏：[Java数字图像处理与特效](https://blog.csdn.net/column/details/image-process.html)









一：数学原理

K-Means算法的作者是MacQueen， 基本的数学原理很容易理解，假设有一个像素

数据集P。我们要根据值不同将它分为两个基本的数据集合Cluster1, Cluster2，使

用K-Means算法大致如下：

假设两个Cluster的RGB值分别为112,225,244和23,34,99则像素集合中的像素点

a(222,212,234), b(198,205,229), c(25,77,52),d(34,55,101)计算每个像素点与这

两个cluster中心点的欧几里德距离，则像素点a, b属于前面一个cluster， c,d属于

后面一个cluster。然后在根据(222+198)/2, (212+205)/2, (234+52)/2更新cluster

的RGB值，对后一个cluster做同样处理。然后再计算每个像素点到cluster中心点

的欧几里德距离。最终值没有变化则得到分类Cluster点集合。

二：算法基本流程

![](https://img-my.csdn.net/uploads/201304/20/1366451358_1138.png)




**三：算法关键代码解析**

初始化cluster中心点代码如下：

```java
Random random = new Random();
for (int i = 0; i < numOfCluster; i++)
{
    int randomNumber1 = random.nextInt(width);
    int randomNumber2 = random.nextInt(height);
    index = randomNumber2 * width + randomNumber1;
    ClusterCenter cc = new ClusterCenter(randomNumber1, randomNumber2, inPixels[index]);
    cc.setcIndex(i);
    clusterCenterList.add(cc); 
}
```

初始化所有像素点代码如下：

```java
// create all cluster point
for (int row = 0; row < height; ++row)
{
    for (int col = 0; col < width; ++col)
    {
    	index = row * width + col;
    	int color = inPixels[index];
    	pointList.add(new ClusterPoint(row, col, color));

    }
}
```

计算两个像素点之间欧几里德距离的代码如下:

```java
// int pa = (p.getPixelColor() >> 24) & 0xff;
int pr = (p.getPixelColor() >> 16) & 0xff;
int pg = (p.getPixelColor() >> 8) & 0xff;
int pb = p.getPixelColor() & 0xff;
// int ca = (c.getPixelColor() >> 24) & 0xff;
int cr = (c.getPixelColor() >> 16) & 0xff;
int cg = (c.getPixelColor() >> 8) & 0xff;
int cb = c.getPixelColor() & 0xff;

return Math.sqrt(Math.pow((pr - cr), 2.0) + Math.pow((pg - cg), 2.0) + Math.pow((pb - cb), 2.0));
```
重新计算Cluster中心点RGB值的代码如下：


```java
private double[] reCalculateClusterCenters() {
	
	// clear the points now
	for(int i=0; i<clusterCenterList.size(); i++)
	{
		 clusterCenterList.get(i).setNumOfPoints(0);
	}
	
	// recalculate the sum and total of points for each cluster
	double[] redSums = new double[3];
	double[] greenSum = new double[3];
	double[] blueSum = new double[3];
	for(int i=0; i<pointList.size(); i++)
	{
		int cIndex = (int)pointList.get(i).getClusterIndex();
		clusterCenterList.get(cIndex).addPoints();
		int ta = (pointList.get(i).getPixelColor() >> 24) & 0xff;
        int tr = (pointList.get(i).getPixelColor() >> 16) & 0xff;
        int tg = (pointList.get(i).getPixelColor() >> 8) & 0xff;
        int tb = pointList.get(i).getPixelColor() & 0xff;
        ta = 255;
		redSums[cIndex] += tr;
		greenSum[cIndex] += tg;
		blueSum[cIndex] += tb;
	}
	
	double[] oldClusterCentersColors = new double[clusterCenterList.size()];
	for(int i=0; i<clusterCenterList.size(); i++)
	{
		double sum  = clusterCenterList.get(i).getNumOfPoints();
		int cIndex = clusterCenterList.get(i).getcIndex();
		int red = (int)(greenSum[cIndex]/sum);
		int green = (int)(greenSum[cIndex]/sum);
		int blue = (int)(blueSum[cIndex]/sum);
		System.out.println("red = " + red + " green = " + green + " blue = " + blue);
		int clusterColor = (255 << 24) | (red << 16) | (green << 8) | blue;
		clusterCenterList.get(i).setPixelColor(clusterColor);
		oldClusterCentersColors[i] = clusterColor;
	}
	
	return oldClusterCentersColors;
}
```

**四：运行效果**



![](https://img-my.csdn.net/uploads/201304/20/1366451587_7079.png)
**五：K-Means算法源代码**



```java
package com.gloomyfish.segmentation.kmeans;

import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import com.gloomyfish.filter.study.AbstractBufferedImageOp;
import com.gloomyfish.segmentation.fuzzycmeans.ClusterPoint;

public class KMeansProcessor extends AbstractBufferedImageOp {
	private List<ClusterCenter> clusterCenterList;
	private List<ClusterPoint> pointList;
	
	private int numOfCluster;
	
	public KMeansProcessor(int clusters)
	{
		this.numOfCluster = clusters;
		pointList = new ArrayList<ClusterPoint>();
		this.clusterCenterList = new ArrayList<ClusterCenter>();
	}

	@Override
	public BufferedImage filter(BufferedImage src, BufferedImage dest) {
		// initialization the pixel data
        int width = src.getWidth();
        int height = src.getHeight();
        int[] inPixels = new int[width*height];
        src.getRGB( 0, 0, width, height, inPixels, 0, width );
        int index = 0;
        
        //Create random points to use a the cluster center
		Random random = new Random();
		for (int i = 0; i < numOfCluster; i++)
		{
		    int randomNumber1 = random.nextInt(width);
		    int randomNumber2 = random.nextInt(height);
		    index = randomNumber2 * width + randomNumber1;
		    ClusterCenter cc = new ClusterCenter(randomNumber1, randomNumber2, inPixels[index]);
		    cc.setcIndex(i);
		    clusterCenterList.add(cc); 
		}
        
        // create all cluster point
        for (int row = 0; row < height; ++row)
        {
            for (int col = 0; col < width; ++col)
            {
            	index = row * width + col;
            	int color = inPixels[index];
            	pointList.add(new ClusterPoint(row, col, color));

            }
        }
        
        // initialize the clusters for each point
        double[] clusterDisValues = new double[clusterCenterList.size()];
        for(int i=0; i<pointList.size(); i++)
        {
        	for(int j=0; j<clusterCenterList.size(); j++)
        	{
        		clusterDisValues[j] = calculateEuclideanDistance(pointList.get(i), clusterCenterList.get(j));
        	}
        	pointList.get(i).setClusterIndex(getCloserCluster(clusterDisValues));
        }
        
        // calculate the old summary
        // assign the points to cluster center
        // calculate the new cluster center
        // computation the delta value
        // stop condition--
        double[] oldClusterCenterColors = reCalculateClusterCenters();
        while(true)
        {
        	stepClusters();
        	double[] newClusterCenterColors = reCalculateClusterCenters();
        	if(isStop(oldClusterCenterColors, newClusterCenterColors))
        	{        		
        		break;
        	} 
        	else
        	{
        		oldClusterCenterColors = newClusterCenterColors;
        	}
        }
        
        //update the result image
        dest = createCompatibleDestImage(src, null );
        index = 0;
        int[] outPixels = new int[width*height];       
        for (int j = 0; j < pointList.size(); j++)
        {
            for (int i = 0; i < clusterCenterList.size(); i++)
            {
                ClusterPoint p = this.pointList.get(j);
                if (clusterCenterList.get(i).getcIndex() == p.getClusterIndex())
                {
                	int row = (int)p.getX(); // row
                	int col = (int)p.getY(); // column
                	index = row * width + col;
                	outPixels[index] = clusterCenterList.get(i).getPixelColor();
                }
            }
        }
        
        // fill the pixel data
        setRGB( dest, 0, 0, width, height, outPixels );
		return dest;
	}
	
	private boolean isStop(double[] oldClusterCenterColors, double[] newClusterCenterColors) {
		for(int i=0; i<oldClusterCenterColors.length; i++)
		{
			System.out.println("cluster " + i + " old : " + oldClusterCenterColors[i] + ", new : " + newClusterCenterColors[i]);
			if(oldClusterCenterColors[i]  != newClusterCenterColors[i]) 
			{
				return false;
			}
		}
		System.out.println();
		return true;
	}

	/**
	 * update the cluster index by distance value
	 */
	private void stepClusters() 
	{
        // initialize the clusters for each point
        double[] clusterDisValues = new double[clusterCenterList.size()];
        for(int i=0; i<pointList.size(); i++)
        {
        	for(int j=0; j<clusterCenterList.size(); j++)
        	{
        		clusterDisValues[j] = calculateEuclideanDistance(pointList.get(i), clusterCenterList.get(j));
        	}
        	pointList.get(i).setClusterIndex(getCloserCluster(clusterDisValues));
        }
		
	}

	/**
	 * using cluster color of each point to update cluster center color
	 * 
	 * @return
	 */
	private double[] reCalculateClusterCenters() {
		
		// clear the points now
		for(int i=0; i<clusterCenterList.size(); i++)
		{
			 clusterCenterList.get(i).setNumOfPoints(0);
		}
		
		// recalculate the sum and total of points for each cluster
		double[] redSums = new double[3];
		double[] greenSum = new double[3];
		double[] blueSum = new double[3];
		for(int i=0; i<pointList.size(); i++)
		{
			int cIndex = (int)pointList.get(i).getClusterIndex();
			clusterCenterList.get(cIndex).addPoints();
    		int ta = (pointList.get(i).getPixelColor() >> 24) & 0xff;
            int tr = (pointList.get(i).getPixelColor() >> 16) & 0xff;
            int tg = (pointList.get(i).getPixelColor() >> 8) & 0xff;
            int tb = pointList.get(i).getPixelColor() & 0xff;
            ta = 255;
			redSums[cIndex] += tr;
			greenSum[cIndex] += tg;
			blueSum[cIndex] += tb;
		}
		
		double[] oldClusterCentersColors = new double[clusterCenterList.size()];
		for(int i=0; i<clusterCenterList.size(); i++)
		{
			double sum  = clusterCenterList.get(i).getNumOfPoints();
			int cIndex = clusterCenterList.get(i).getcIndex();
			int red = (int)(greenSum[cIndex]/sum);
			int green = (int)(greenSum[cIndex]/sum);
			int blue = (int)(blueSum[cIndex]/sum);
			System.out.println("red = " + red + " green = " + green + " blue = " + blue);
			int clusterColor = (255 << 24) | (red << 16) | (green << 8) | blue;
			clusterCenterList.get(i).setPixelColor(clusterColor);
			oldClusterCentersColors[i] = clusterColor;
		}
		
		return oldClusterCentersColors;
	}
	
	

	/**
	 * 
	 * @param clusterDisValues
	 * @return
	 */
	private double getCloserCluster(double[] clusterDisValues)
	{
		double min = clusterDisValues[0];
		int clusterIndex = 0;
		for(int i=0; i<clusterDisValues.length; i++)
		{
			if(min > clusterDisValues[i])
			{
				min = clusterDisValues[i];
				clusterIndex = i;
			}
		}
		return clusterIndex;
	}

	/**
	 * 
	 * @param point
	 * @param cluster
	 * @return distance value
	 */
	private double calculateEuclideanDistance(ClusterPoint p, ClusterCenter c) 
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

}
```
**转载请注明出自gloomyfish博客**




