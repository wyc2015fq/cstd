# Java学习笔记：this使用总结 - alw2009的博客 - CSDN博客
2016年09月19日 12:30:19[traveler_zero](https://me.csdn.net/alw2009)阅读数：114
1
```java
public class Point {
    public int x = 0;
    public int y = 0;
        
    //constructor
    public Point(int x, int y) {
        this.x = x;
        this.y = y;
    }
}
```
2.
public class Rectangle {
    private int x, y;
    private int width, height;
        
    public Rectangle() {
        **this(0, 0, 1, 1);**
    }
    public Rectangle(int width, int height) {
        **this(0, 0, width, height);**
    }
    public Rectangle(int x, int y, int width, int height) {
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
    }
    ...
}
