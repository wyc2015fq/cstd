# Java实现算法导论中凸包问题Jarvis步进法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年12月07日 10:00:41[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3093
所属专栏：[算法导论专栏](https://blog.csdn.net/column/details/16283.html)









对于凸包的理解，参考http://www.cnblogs.com/Booble/archive/2011/02/28/1967179.html，说的还是比较深入浅出。

凸包问题的Jarvis步进法，其算法流程：

1.找横坐标最小的点（有一样的话纵坐标更小的）

2.从这点开始卷包裹  找最靠近外侧的点（通过叉积比较）

3.找到的点为起点退出


代码参考如下：



```java
package cn.ansj;

import static java.lang.Math.abs;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

public class JarvisMarch {
	private int count;

    public int getCount() {
        return count;
    }

    public void setCount(int count) {
        this.count = count;
    }

    private static int MAX_ANGLE = 4;
    private double currentMinAngle = 0;
    private List<Point> hullPointList;
    private List<Integer> indexList;
    private PointFactory pf;
    private Point[] ps;

    public Point[] getPs() {
        return ps;
    }

    private int firstIndex;

    public int getFirstIndex() {
        return firstIndex;
    }

    public JarvisMarch() {
        this(10);
    }

    public JarvisMarch(int count) {
        pf = PointFactory.getInstance(count);
        initialize();
    }

    public JarvisMarch(int[] x, int[] y) {
        pf = PointFactory.getInstance(x, y);
        initialize();
    }

    private void initialize() {
        hullPointList = new LinkedList<Point>();
        indexList = new LinkedList<Integer>();
        firstIndex = pf.getFirstIndex();
        ps = pf.getPoints();
        addToHull(firstIndex);
    }

    private void addToHull(int index) {
        indexList.add(index);
        hullPointList.add(ps[index]);
    }

    public int calculateHull() {
        for (int i = getNextIndex(firstIndex); i != firstIndex; i = getNextIndex(i)) {
            addToHull(i);
        }
        showHullPoints();
        return 0;
    }

    private void showHullPoints() {
        Iterator<Point> itPoint = hullPointList.iterator();
        Iterator<Integer> itIndex = indexList.iterator();
        Point p;
        int i;
        int index = 0;
        System.out.println("The hull points is: -> ");
        while (itPoint.hasNext()) {
            i = itIndex.next();
            p = itPoint.next();
            System.out.print(i + ":(" + p.getX() + "," + p.getY() + ")  ");
            index++;
            if (index % 10 == 0)
                System.out.println();
        }
        System.out.println();
        System.out.println("****************************************************************");
        System.out.println("The count of all hull points is " + index);
    }

    public int getNextIndex(int currentIndex) {
        double minAngle = MAX_ANGLE;
        double pseudoAngle;
        int minIndex = 0;
        for (int i = 0; i < ps.length; i++) {
            if (i != currentIndex) {
                pseudoAngle = getPseudoAngle(ps[i].getX() - ps[currentIndex].getX(), 
                                             ps[i].getY() - ps[currentIndex].getY());
                if (pseudoAngle >= currentMinAngle && pseudoAngle < minAngle) {
                    minAngle = pseudoAngle;
                    minIndex = i;
                } else if (pseudoAngle == minAngle){
                        if((abs(ps[i].getX() - ps[currentIndex].getX()) > 
                            abs(ps[minIndex].getX() - ps[currentIndex].getX()))
                            || (abs(ps[i].getY() - ps[currentIndex].getY()) > 
                            abs(ps[minIndex].getY() - ps[currentIndex].getY()))){
                            minIndex = i;
                        }
                }
            }

        }
        currentMinAngle = minAngle;
        return minIndex;
    }

    public double getPseudoAngle(double dx, double dy) {//计算极角
        if (dx > 0 && dy >= 0)
            return dy / (dx + dy);
        if (dx <= 0 && dy > 0)
            return 1 + (abs(dx) / (abs(dx) + dy));
        if (dx < 0 && dy <= 0)
            return 2 + (dy / (dx + dy));
        if (dx >= 0 && dy < 0)
            return 3 + (dx / (dx + abs(dy)));
        throw new Error("Impossible");
    }


    public static void main(String[] args) {
        long start = System.currentTimeMillis();
        JarvisMarch j = new JarvisMarch(100000);
        Point[] points = j.getPs();
        int firstIndex = j.getFirstIndex();
        
        System.out.println("the first point is: " + firstIndex + ":(" + 
                    points[firstIndex].getX() + "," + points[firstIndex].getY() + ")");      
        System.out.println("*****************************************************************");
        j.calculateHull();
        System.out.println("The total running time is " + (System.currentTimeMillis() - start) + " millis.");
    }
}
```

```java
package cn.ansj;

public class PointFactory {
/**
 * 单例模式，大批量产生Point，也可以手动产生Point
 */
    private Point[] points = null;
    private int newIndex;
    private int firstIndex = 0;

    public Point[] getPoints() {
        return points;
    }

    public int getFirstIndex() {
        return firstIndex;
    }

    public static PointFactory getInstance() {
        return new PointFactory();
    }

    public static PointFactory getInstance(int count) {
        return new PointFactory(count);
    }
    
    public static PointFactory getInstance(int[] x, int[] y) {
        return new PointFactory(x, y);
    }

    private PointFactory() {
        this(10);
    }

    private PointFactory(int count) {
        points = new Point[count];
        for (int i = 0; i < count; i++) {
            points[i] = new Point();
            newIndex = i;
            validatePoints();
        }
        firstIndex = getFirstPoint();
    }

    public PointFactory(int[] x, int[] y) {
        points = new Point[y.length];
        for (int i = 0; i < y.length; i++) {
            points[i] = new Point(x[i], y[i]);
        }
        firstIndex = getFirstPoint();
    }

    private void validatePoints() {
        for(int i = 0; i < newIndex; i++) {
                if(points[i].equals(points[newIndex])) {
                    points[newIndex] = new Point();
                    validatePoints();
                }
            }
    }

    public int getFirstPoint() {
        int minIndex = 0;
        for (int i = 1; i < points.length; i++) {
            if (points[i].getY() < points[minIndex].getY()) {
                minIndex = i;
            } else if ((points[i].getY() == points[minIndex].getY())
                    && (points[i].getX() < points[minIndex].getX())) {
                minIndex = i;
            }
        }
        return minIndex;
    }

}
```

```java
package cn.ansj;

public class Point {
    
//  定义点的x，y坐标，之所以是int类型，是为了日后可以在计算机屏幕上进行可视化。
  private int x;
  private int y;

//  x,y的get方法
  public int getX() {
      return x;
  }

  public int getY() {
      return y;
  }
//  定义点到屏幕边缘的距离
  private static double PADDING = 20;
//  点在屏幕中的范围
  private static double POINT_RANGE = (800 - PADDING * 2);

//  默认构造方法，产生随机点
  public Point() {
      this.x = (int) ((Math.random() * POINT_RANGE) + PADDING);
      this.y = (int) ((Math.random() * POINT_RANGE) + PADDING);
  }
  
//  带参构造方法，可以实现手动输入固定点    
  public Point(int x, int y) {
      this.x = x;
      this.y = y;
  }

//  覆写hashCode()和equals()方法，实现比较和Hash
  @Override
  public int hashCode() {
      final int prime = 31;
      int result = 1;
      result = prime * result + x;
      result = prime * result + y;
      return result;
  }

  @Override
  public boolean equals(Object obj) {
      Point other = (Point) obj;
      if ((x == other.x) && (y == other.y))
          return true;
      
      return false;
  }
  
  
}
```


执行结果如下：



```
the first point is: 72886:(52,20)
*****************************************************************
The hull points is: -> 
72886:(52,20)  37943:(778,20)  81531:(779,21)  34317:(779,779)  69049:(20,779)  14579:(20,22)  80398:(22,21)  
****************************************************************
The count of all hull points is 7
The total running time is 38019 millis.
```







