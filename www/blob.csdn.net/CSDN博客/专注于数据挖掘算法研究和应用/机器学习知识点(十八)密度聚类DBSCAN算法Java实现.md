# 机器学习知识点(十八)密度聚类DBSCAN算法Java实现 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月20日 11:41:28[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3318
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)









为更好理解聚类算法，从网上找现成代码来理解，发现了一个Java自身的ML库，链接：http://java-ml.sourceforge.net/

有兴趣可以下载来看看源码，理解基础ML算法。对于DBSCAN算法，从网上找到一个Java实现的，主要是用来理解其算法过程。参考代码如下：

1、Point类，数据对象



```java
package sk.cluster;

public class Point {
    private double x;//坐标x轴
    private double y;//坐标y轴
    private boolean isVisit;//是佛访问标记
    private int cluster;//所属簇类
    private boolean isNoised;//是否是噪音数据

    public Point(double x,double y) {
        this.x = x;
        this.y = y;
        this.isVisit = false;
        this.cluster = 0;
        this.isNoised = false;
    }

    public double getDistance(Point point) {//计算两点间距离
        return Math.sqrt((x-point.x)*(x-point.x)+(y-point.y)*(y-point.y));
    }

    public void setX(double x) {
        this.x = x;
    }

    public double getX() {
        return x;
    }

    public void setY(double y) {
        this.y = y;
    }

    public double getY() {
        return y;
    }

    public void setVisit(boolean isVisit) {
        this.isVisit = isVisit;
    }

    public boolean getVisit() {
        return isVisit;
    }

    public int getCluster() {
        return cluster;
    }

    public void setNoised(boolean isNoised) {
        this.isNoised = isNoised;
    }

    public void setCluster(int cluster) {
        this.cluster = cluster;
    }

    public boolean getNoised() {
        return this.isNoised;
    }

    @Override
    public String toString() {
        return x+" "+y+" "+cluster+" "+(isNoised?1:0);
    }

}
```


2、Data类，数据集



```java
package sk.cluster;

import java.io.*;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.ArrayList;
import java.util.Random;


public class Data {
    private static DecimalFormat df=(DecimalFormat) NumberFormat.getInstance();

    //随机生成数据
    public static ArrayList<Point> generateSinData(int size) {
        ArrayList<Point> points = new ArrayList<Point>(size);
        Random rd = new Random(size);
        for (int i=0;i<size/2;i++) {
            double x = format(Math.PI / (size / 2) * (i + 1));
            double y = format(Math.sin(x)) ;
            points.add(new Point(x,y));
        }
        for (int i=0;i<size/2;i++) {
            double x = format(1.5 + Math.PI / (size/2) * (i+1));
            double y = format(Math.cos(x));
            points.add(new Point(x,y));
        }
        return points;
    }

    //输入指定数据
    public static ArrayList<Point> generateSpecialData() {
        ArrayList<Point> points = new ArrayList<Point>();
        points.add(new Point(2,2));
        points.add(new Point(3,1));
        points.add(new Point(3,4));
        points.add(new Point(3,14));
        points.add(new Point(5,3));
        points.add(new Point(8,3));
        points.add(new Point(8,6));
        points.add(new Point(9,8));
        points.add(new Point(10,4));
        points.add(new Point(10,7));
        points.add(new Point(10,10));
        points.add(new Point(10,14));
        points.add(new Point(11,13));
        points.add(new Point(12,7));
        points.add(new Point(12,15));
        points.add(new Point(14,7));
        points.add(new Point(14,9));
        points.add(new Point(14,15));
        points.add(new Point(15,8));
        return points;
    }
    
    //获取文件数据
    public static ArrayList<Point> getData(String sourcePath) {
        ArrayList<Point> points = new ArrayList<Point>();
        File fileIn = new File(sourcePath);
        try {
            BufferedReader br = new BufferedReader(new FileReader(fileIn));
            String line = null;
            line = br.readLine();
            while (line != null) {
                Double x = Double.parseDouble(line.split(",")[3]);
                Double y = Double.parseDouble(line.split(",")[4]);
                points.add(new Point(x, y));
                line = br.readLine();
            }
            br.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return points;
    }

    //输出到文件
    public static void writeData(ArrayList<Point> points,String path) {
        try {
            BufferedWriter bw = new BufferedWriter(new FileWriter(path));
            for (Point point:points) {
                bw.write(point.toString()+"\n");
            }
            bw.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static double format(double x) {
        return Double.valueOf(df.format(x));
    }

}
```


3、DBSCAN类，实现DBSCAN算法



```java
package sk.cluster;

import java.util.ArrayList;


public class DBScan {
    private double radius;
    private int minPts;

    public DBScan(double radius,int minPts) {
        this.radius = radius;//领域半径参数
        this.minPts = minPts;//领域密度值，该领域内有多少个样本
    }

    public void process(ArrayList<Point> points) {
        int size = points.size();
        int idx = 0;
        int cluster = 1;
        while (idx<size) {//样本总数
            Point p = points.get(idx++);
            //choose an unvisited point
            if (!p.getVisit()) {
                p.setVisit(true);//set visited
                ArrayList<Point> adjacentPoints = getAdjacentPoints(p, points);//计算两点距离，看是否在领域内
                //set the point which adjacent points less than minPts noised
                if (adjacentPoints != null && adjacentPoints.size() < minPts) {
                    p.setNoised(true);//噪音数据
                } else {//建立该点作为领域核心对象
                    p.setCluster(cluster);
                    for (int i = 0; i < adjacentPoints.size(); i++) {
                        Point adjacentPoint = adjacentPoints.get(i);//领域内的样本
                        //only check unvisited point, cause only unvisited have the chance to add new adjacent points
                        if (!adjacentPoint.getVisit()) {
                            adjacentPoint.setVisit(true);
                            ArrayList<Point> adjacentAdjacentPoints = getAdjacentPoints(adjacentPoint, points);
                            //add point which adjacent points not less than minPts noised
                            if (adjacentAdjacentPoints != null && adjacentAdjacentPoints.size() >= minPts) {
                                //adjacentPoints.addAll(adjacentAdjacentPoints);
                                for (Point pp : adjacentAdjacentPoints){
                                    if (!adjacentPoints.contains(pp)){
                                        adjacentPoints.add(pp);
                                    }
                                }
                            }
                        }
                        //add point which doest not belong to any cluster
                        if (adjacentPoint.getCluster() == 0) {
                            adjacentPoint.setCluster(cluster);
                            //set point which marked noised before non-noised
                            if (adjacentPoint.getNoised()) {
                                adjacentPoint.setNoised(false);
                            }
                        }
                    }
                    cluster++;
                }
            }
            if (idx%1000==0) {
                System.out.println(idx);
            }
        }
    }

    private ArrayList<Point> getAdjacentPoints(Point centerPoint,ArrayList<Point> points) {
        ArrayList<Point> adjacentPoints = new ArrayList<Point>();
        for (Point p:points) {
            //include centerPoint itself
            double distance = centerPoint.getDistance(p);
            if (distance<=radius) {
                adjacentPoints.add(p);
            }
        }
        return adjacentPoints;
    }

}
/*
##DBScan算法流程图

算法：DBScan，基于密度的聚类算法
输入：
   D：一个包含n个数据的数据集
   r：半径参数
   minPts：领域密度阈值
输出：基于密度的聚类集合
标记D中所有的点为unvisted
for each p in D
	if p.visit = unvisted
		找出与点p距离不大于r的所有点集合N
		If N.size() < minPts
		   标记点p为噪声点
		Else
		   for each p' in N
		       If p'.visit == unvisted
		          找出与点p距离不大于r的所有点集合N'
		            If N'.size()>=minPts
		                将集合N'加入集合N中去
		            End if
				Else
			        If p'未被聚到某个簇
			            将p'聚到当前簇
			            If p'被标记为噪声点
			               将p'取消标记为噪声点
				        End If
			         End If
			     End If
			 End for
		 End if
	 End if
End for
*/
```


4、client测试类



```java
package sk.cluster;

import java.util.ArrayList;

public class Client {


    public static void main(String[] args) {
        ArrayList<Point> points = Data.generateSinData(200);//随机生成200个point
        DBScan dbScan = new DBScan(0.6,4);//r：领域半径参数 ，minPts领域密度阈值，密度值
        //ArrayList<Point> points = Data.generateSpecialData();
        //ArrayList<Point> points = Data.getData("D:\\tmp\\testData.txt");
        //DBScan dbScan = new DBScan(0.1,1000);
        dbScan.process(points);
        for (Point p:points) {
            System.out.println(p);
        }
        Data.writeData(points,"D:\\tmp\\data.txt");
    }


}
```





