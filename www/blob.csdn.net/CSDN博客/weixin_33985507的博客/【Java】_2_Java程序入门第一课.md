# 【Java】_2_Java程序入门第一课 - weixin_33985507的博客 - CSDN博客
2012年12月18日 21:53:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
　　首先说一下，为什么学习Java。主要是因为要用到这个东西，现场有一个设备，ABB的板型仪，里面用的是Java做的后台 ，利用Java服务器与
DSP芯片做数据采集分析功能，DSP进行高速数据采集，然后用Java做数据分析。所以需要想学习这个东西，来了解DSP和Java通讯，以及和Win32
平台通讯的一些相关内容。
【Java外壳类】
　　我感觉这个Java和C#有点类似，首先什么也不管，就需要定义一个类。我们首先来看这个类。
```
1 /*
 2  This program test the basic data type
 3  
 4  1、boolean
 5  2、byte
 6  3、short
 7  4、int
 8  5、long
 9  6、float
10  7、double
11  8、char
12 
13 */
14 
15 
16 
17 public class IntTest   //因为Java是纯面向对象的程序设计语言，所有的可执行代码均需要在类中，
18                        //因此这里必须定义一个类.
19                        //这里类作为一个加载“程序逻辑”的“容器”
20                       
21 {
22    public static void main(String[] args)  //Java中所有的函数都属于某个类，因此main方法需要一个”外壳类“
```
      我们看到这个东西，与C++还是不一样的。
【Java boolean类型】
　　在Java中提供了一种表示逻辑结果的数据类型：boolean
```
public class IntTest   //因为Java是纯面向对象的程序设计语言，所有的可执行代码均需要在类中，
                       //因此这里必须定义一个类.
                       //这里类作为一个加载“程序逻辑”的“容器”
                      
{
   public static void main(String[] args)  //Java中所有的函数都属于某个类，因此main方法需要一个”外壳类“
     {
        //首先测试boolean类型
        
        boolean YesOrNo=true;
        
        if(YesOrNo)
           System.out.println("Yes");
```
　　我们可以看到，这里布尔类型与C语言中的布尔类型非常类似，但是有一点不同的是，C语言中“非零即True，零等价于False”， 而在Java中不能这样认为，因为Java中不支持
布尔类型和其他类型间的转换。这一点必须注意。如下图所示：
![](https://pic002.cnblogs.com/images/2012/304449/2012121820363815.jpg)
【Java byte类型】
　　在Java中提供了一种单字节大小的数据类型：byte；这种数据类型，用来表示小数值时比较有效，节省内存空间；但是不利于CPU处理。
```
//测试byte类型
        byte OverFlow=(byte)300;  
             System.out.println(OverFlow); //因为在Java中byte表示的大小从 -128 ~ 127 共256个状态，同时
                                           //进行type cast时会进行截取，所以这里输出是 44
             OverFlow=300 & 0xFF;
             System.out.println(OverFlow); //可以看到在整型数据转换时，可以根据存储特点进行处理，与C语言类似
             System.out.println("The max value of byte is:" + 0x7F);
             System.out.println("The min value of byte is:" + ~0x7F); //可以看到这里有点特殊，在C语言中提供了宏来表示max和  
                                                                    //min值,在Java中有没有呢？
```
　　通过测试我们发现，byte的数值范围规定是： ~0x7F - 0x7F; 而且这里还可以像C语言中一样进行数值截取； 我不知道大家怎样看待
强制类型转换，我感觉用 “按位与” 更能体现计算机的计算机制。
     这里有一点值得探讨一下，为什么-256是~0xFF呢,而且~0x7F是-128？ 我目前感觉有点奇怪。上面的程序执行的结果如下所示：
![](https://images0.cnblogs.com/blog/304449/201212/20195741-48bd6fbd74594b80b953ca7d130b2c8a.jpg)
　　这里我知道Double类型提供了一些常量： Double.POSITIVE_INFINITY、Double.NEGATIVE_INFINITY、Double.NaN来表示最大值
最小值和无穷大，但是byte有没有提供这样的类库支持呢？
【Java Short类型】
    short类型与C语言中16位的有符号short类型一致，表示的值的范围：-327687 ~ 32767. 如下所示：
```
//测试short类型
        short shortTestVar=0x7FFF;
              System.out.println("The max value of short is:"+ shortTestVar);
              System.out.println("The min value of short is:"+ ~shortTestVar);
```
　　这段代码运行的结果如下图所示：
![](https://images0.cnblogs.com/blog/304449/201212/20194845-2dec8c471cf843059880ab99db73d968.jpg)
【Java int类型】
　 　 int类型与C语言中有符号int类型一致(32位的int型)，表示值的范围是： 0x80000000 ~0x7FFFFFFF（计算机内表示法）；
```
//测试int类型
        int intTestVar=0x80000000;
            System.out.println("The max value of int is:"+ ~intTestVar);
            System.out.println("The min value of int is:"+ intTestVar);
```
　　运行结果如下图所示：
![](https://images0.cnblogs.com/blog/304449/201212/20201149-763383bf42c84b0b8adb1ebba7bb8e7a.jpg)
【Java long类型】
　　long类型与C语言中long int类型一致， 表示的值范围是：0x8000 0000 0000 0000 ~ 0x7FFF FFFF FFFF FFFF（计算机内表示法）.
```
//测试long数据类型
        long longTestVar=0x8000000000000000L;
         System.out.println("The max value of long is:"+ ~longTestVar);
             System.out.println("The min value of long is:"+ longTestVar);
```
　　运行结果如下图所示：
![](https://images0.cnblogs.com/blog/304449/201212/20202222-ffb9fc9dd45e40d4b54c2261e288296b.jpg)
　　long数据类型有一点需要说明：那就是如果要表示一个long型的整数，则必须在数字后面加上一个“L”。
【Java整型数小结】
　　Java中没有无符号数的概念，所有的整形数据均是有符号数；这一点是与C/C++不一样的地方，需要注意。
　　默认情况下整型字面值是int类型的；这一点很重要，否则容易出错；如下图示：
![](https://images0.cnblogs.com/blog/304449/201212/20203048-012964a9ba74460baaeb4019bf6efa6c.jpg)
【Java double数据和float数据】
　　float数据占用4字节，而double数据占用8字节；同时float数据类型的有效位数是6~7位；double有效位是15位；在没有特别说明的情况下，浮点
数字面值是double型的；如果要表示float型的浮点数字面值，则需要在字面值后面加F，如： 3.1415926F。
