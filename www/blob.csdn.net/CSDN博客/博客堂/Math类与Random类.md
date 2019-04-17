# Math类与Random类 - 博客堂 - CSDN博客





2017年02月27日 17:21:57[最小森林](https://me.csdn.net/u012052268)阅读数：257标签：[random																[math](https://so.csdn.net/so/search/s.do?q=math&t=blog)](https://so.csdn.net/so/search/s.do?q=random&t=blog)
个人分类：[java](https://blog.csdn.net/u012052268/article/category/3058811)










- [Math类](#math类)- [Math类概述](#math类概述)
- [成员变量](#成员变量)
- [成员方法](#成员方法)
- [应用](#应用)

- [Random类](#random类)- [构造方法](#构造方法)
- [成员方法](#成员方法-1)





# Math类

## Math类概述

用于数学运算的类。掌握常用方法即可。

## 成员变量

注意 定义的方式（静态 常量 ）

public static **final** double PI

public static **final** double E

```
System.out.println("PI:" + Math.PI);

System.out.println("E:" + Math.E);

System.out.println("--------------");
```

## 成员方法

public static int abs(int a)：绝对值

public static double ceil(double a):向上取整

public static double floor(double a):向下取整

public static int max(int a,int b):最大值 (min自学)

public static double pow(double a,double b):a的b次幂

public static double random():随机数 [0.0,1.0)

public static int round(float a) 四舍五入(参数为double的自学)

public static double sqrt(double a):正平方根

```
// public static int abs(int a)：绝对值
        System.out.println("abs:" + Math.abs(10));
        System.out.println("abs:" + Math.abs(-10));
        System.out.println("--------------");

        // public static double ceil(double a):向上取整
        System.out.println("ceil:" + Math.ceil(12.34));
        System.out.println("ceil:" + Math.ceil(12.56));
        System.out.println("--------------");

        // public static double floor(double a):向下取整
        System.out.println("floor:" + Math.floor(12.34));
        System.out.println("floor:" + Math.floor(12.56));
        System.out.println("--------------");

        // public static int max(int a,int b):最大值
        System.out.println("max:" + Math.max(12, 23));
        // 需求：我要获取三个数据中的最大值
        // 方法的嵌套调用
        System.out.println("max:" + Math.max(Math.max(12, 23), 18));
        // 需求：我要获取四个数据中的最大值
        System.out.println("max:"
                + Math.max(Math.max(12, 78), Math.max(34, 56)));
        System.out.println("--------------");

        // public static double pow(double a,double b):a的b次幂
        System.out.println("pow:" + Math.pow(2, 3));
        System.out.println("--------------");

        // public static double random():随机数 [0.0,1.0)
        System.out.println("random:" + Math.random());
        // 获取一个1-100之间的随机数
        System.out.println("random:" + ((int) (Math.random() * 100) + 1));
        System.out.println("--------------");

        // public static int round(float a) 四舍五入(参数为double的自学)
        System.out.println("round:" + Math.round(12.34f));
        System.out.println("round:" + Math.round(12.56f));
        System.out.println("--------------");

        //public static double sqrt(double a):正平方根
        System.out.println("sqrt:"+Math.sqrt(4));
```

## 应用

> 
请设计一个方法，可以实现获取任意范围内的随机数。


```java
public class MathDemo {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.println("请输入开始数：");
        int start = sc.nextInt();
        System.out.println("请输入结束数：");
        int end = sc.nextInt();

        for (int x = 0; x < 100; x++) {
            // 调用功能
            int num = getRandom(start, end);
            // 输出结果
            System.out.println(num);
        }
    }

    /*
     * 写一个功能 两个明确： 返回值类型：int 参数列表：int start,int end
     */
    public static int getRandom(int start, int end) {
        // 回想我们讲过的1-100之间的随机数
        // int number = (int) (Math.random() * 100) + 1;
        // int number = (int) (Math.random() * end) + start;
        // 发现有问题了，怎么办呢?
        int number = (int) (Math.random() * (end - start + 1)) + start;
        return number;
    }
}
```

# Random类

Random:产生随机数的类。上面用Math类有些麻烦，用专门的类，更方便

## 构造方法

```
public Random():    没有给种子，用的是默认种子，是当前时间的毫秒值
public Random(long seed):   给出指定的种子

给定种子后，每次得到的随机数是相同的。
```

## 成员方法

```
public int nextInt()：返回的是int范围内的随机数

public int nextInt(int n):返回的是[0,n)范围的内随机数
```

实验代码

```
public class RandomDemo {
    public static void main(String[] args) {
        // 创建对象
        // Random r = new Random();
        Random r = new Random(1111);

        for (int x = 0; x < 10; x++) {
            // int num = r.nextInt();
            int num = r.nextInt(100) + 1;
            System.out.println(num);
        }
    }
}
```




