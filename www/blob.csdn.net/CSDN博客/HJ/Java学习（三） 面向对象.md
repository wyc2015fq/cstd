# Java学习（三） 面向对象 - HJ - CSDN博客
2018年02月07日 15:28:24[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：111
个人分类：[Java](https://blog.csdn.net/feizaoSYUACM/article/category/7349915)
所属专栏：[Java学习](https://blog.csdn.net/column/details/19570.html)
一、面向对象概述
面向对象（OO），是一种以事物为中心的编程思想。面向对象程序设计（OOP），是一种程序开发的方法。它将对象作为程序的基本单元，将程序和数据封装其中，以提高软件的重用性、灵活性和扩展性。
对象（object）代表现实世界中可以明确标识的一个实体。每个对象都有自己独特的状态标识和行为对象的属性。
对象的行为，是由方法定义，调用对象的一个方法，其实就是给对象发消息，要求对象完成一个动作。
面向对象的特征：封装、继承、多态
在开发的过程其实就是不断的创建对象，使用对象，指挥对象做事情；
设计的过程其实就是在管理和维护对象之间关系；
二、类与对象
1.类的描述
例：定义一个汽车类，并生产出汽车，有颜色、轮胎个数，有运行的功能。
**public class** Car** {    String** color**;**// 成员变量
**        int** num**;** // 成员变量
    // 成员函数
**    void** run**() {        System.**out**.**println**(**color** +** "的车，轮胎数："** +** num** +** "个，跑起来了"**);    }}**
2.对象的创建
创建Car对象
①使用new关键词，就像new数组一样
②Car car = new Car();
**class** CarDemo** {    public static void** main**(String[]** args**) {**
        // java 中创建对象，使用new关键字。在堆内存中开辟了空间。产生了一个实体。
        Car c** = new** Car**();**
        //为了方便使用生产的汽车，就起了一个名字。
        //那么c是什么类型,是Car类型。叫做类类型变量。
        //注意 c是持有的引用，新生产的汽车并没有直接赋值给c，持有的只是一个引用。c就想电视遥控器一样。
        c**.**run**();**  //使用对象的功能。
**    }}**
3.对象成员的调用
主要是调用成员变量和成员方法
**public class** CarDemo** {    public static void** main**(String[]** args**) {**
        Car c** = new** Car**();**
        //对象名.成员变量名将返回成员变量中存储的数值
**        int** num**=**c**.**num**;         System.**out**.**println**(**num**);**
        
        //对象名.成员变量名,也可以给成员变量赋值
        c**.**num** =** 4**;**
        c**.**color** =** "black"**;**
        
        //对象名.成员方法();
        c**.**run**();    }}**
三、局部变量与成员变量比较
成员变量：定义在类中的变量
局部变量：定义在方法中的变量
区别：
1.应用范围
①成员变量在整个类内都有效；
②局部变量只在其声明的方法内有效；
2.生命周期
①成员变量属于对象，随着对象的创建而创建，随着对象的消失而消失；
②局部变量使用完马上释放空间；
3.存储位置
    ①成员变量属于对象，它存储在堆内，堆内的实体，当没有引用指向其时，才垃圾回收清理；
    ②局部变量存在栈内存中，当不再使用时，马上就会被释放；
4.初始值
①成员变量它存储在堆中，如果没有赋初值，它有默认值；
②局部变量如果要想使用必须手动初始化；
四、内存分析
分析一：
//汽车
**class** Car** {**
    //汽车应该具备的属性
**    int** num**;**
    //汽车具备的颜色
**    String** color**;**
    //汽车跑的行为
**    public void** run**(){        System.**out**.**println**(**num**+**"轮子的汽车跑起来啦"**);    }}public class** CarDemo**{        public static void** main**(String[]** args**)    {**    //创建实体，并且给该实体起一个名字
        Car c** = new** Car**();**
        c**.**color** =** "red"**;**
        c**.**num** =** 4**;**
        c**.**run**();**//指挥车进行运行。调用格式：对象.对象成员
**            }}**
![](https://img-blog.csdn.net/20180207152718213?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
分析二：
**public static void** main**(String[]** args**){**    //创建实体，并且给该实体起一个名字
    Car c** = new** Car**();**
    Car c1** = new** Car**();**
    c**.**color** =** "red"**;**
    c1**.**num** =** 4**;    System.**out**.**println**(**c1**.**color**);**
    c**.**run**();**//指挥车进行运行。调用格式：对象.对象成员    
**}**
![](https://img-blog.csdn.net/20180207152736080?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
分析三：
**public static void** main**(String[]** args**){**    //创建实体，并且给该实体起一个名字
    Car c** = new** Car**();**
    Car c1** =** c**;**
    c**.**color** =** "red"**;**
    c1**.**num** =** 4**;**
    c1**.**color** =** "green"**;    System.**out**.**println**(**c1**.**color**);**
    c**.**run**();**//指挥车进行运行。调用格式：对象.对象成员    
**}**
![](https://img-blog.csdn.net/20180207152759423?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
