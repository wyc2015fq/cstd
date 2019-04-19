# JUnit - 三少GG - CSDN博客
2012年10月25日 01:51:00[三少GG](https://me.csdn.net/scut1135)阅读数：904
个人分类：[重构Java](https://blog.csdn.net/scut1135/article/category/621771)
**1、概述**
　　Junit测试是程序员测试，即所谓白盒测试，因为程序员知道被测试的软件如何（How）完成功能和完成什么样（What）的功能。
　　Junit本质上是一套框架，即开发者制定了一套条条框框，遵循这此条条框框要求编写测试代码，如继承某个类，实现某个接口，就可以用Junit进行自动测试了。
　　由于Junit相对独立于所编写的代码，可以测试代码的编写可以先于实现代码的编写，XP 中推崇的 test first design的实现有了现成的手段：用Junit写测试代码，写实现代码，运行测试，测试失败，修改实现代码，再运行测试，直到测试成功。以后对代码的修改和优化，运行测试成功，则修改成功。
　　Java 下的 team 开发，采用 cvs(版本控制) + ant(项目管理) + junit(集成测试) 的模式时，通过对ant的配置，可以很简单地实现测试自动化。
　　对不同性质的被测对象，如Class，Jsp，Servlet，Ejb等，Junit有不同的使用技巧，以后慢慢地分别讲叙。以下以Class测试为例讲解，除非特殊说明。
**2、下载安装**
- 去[Junit主页](http://www.infomall.cn/cgi-bin/mallgate/20040505/http://www.junit.org/index.htm)下载最新版本3.8.1程序包junit-3.8.1.zip
- 用winzip或unzip将junit-3.8.1.zip解压缩到某一目录名为$JUNITHOME
- 将junit.jar和$JUNITHOME/junit加入到CLASSPATH中，加入后者只因为测试例程在那个目录下。
- 注意不要将junit.jar放在jdk的extension目录下
- 运行命令,结果如右图。
java junit.swingui.TestRunner junit.samples.AllTests
![](http://www.uml.org.cn/j2ee/images/001.gif)
**3、Junit架构**
　　下面以Money这个类为例进行说明。
public class Money {
    private int fAmount;//余额
    private String fCurrency;//货币类型
    public Money(int amount, String currency) {
        fAmount= amount;
        fCurrency= currency;
    }
    public int amount() {
        return fAmount;
    }
    public String currency() {
        return fCurrency;
    }
    public Money add(Money m) {//加钱
        return new Money(amount()+m.amount(), currency());
    }
    public boolean equals(Object anObject) {//判断钱数是否相等
        if (anObject instanceof Money) {
            Money aMoney= (Money)anObject;
            return aMoney.currency().equals(currency())
                && amount() == aMoney.amount();
        }
        return false;
    }    
}
　　Junit本身是围绕着两个设计模式来设计的：命令模式和集成模式.
- 命令模式
　　利用TestCase定义一个子类，在这个子类中生成一个被测试的对象，编写代码检测某个 方法被调用后对象的状态与预期的状态是否一致，进而断言程序代码有没有bug。
　　当这个子类要测试不只一个方法的实现代码时，可以先建立测试基础，让这些测试在同一个基础上运行，一方面可以减少每个测试的初始化，而且可以测试这些不同方法之间的联系。
　　例如，我们要测试Money的Add方法，可以如下:
public class MoneyTest extends TestCase { //TestCase的子类
    public void testAdd() { //把测试代码放在testAdd中
        Money m12CHF= new Money(12, "CHF");  //本行和下一行进行一些初始化
        Money m14CHF= new Money(14, "CHF");        
        Money expected= new Money(26, "CHF");//预期的结果
        Money result= m12CHF.add(m14CHF);    //运行被测试的方法
        Assert.assertTrue(expected.equals(result));     //判断运行结果是否与预期的相同
    }
}
　　如果测试一下equals方法，用类似的代码，如下：
public class MoneyTest extends TestCase { //TestCase的子类
    public void testEquals() { //把测试代码放在testEquals中
        Money m12CHF= new Money(12, "CHF"); //本行和下一行进行一些初始化
        Money m14CHF= new Money(14, "CHF");
        Assert.assertTrue(!m12CHF.equals(null));//进行不同情况的测试
        Assert.assertEquals(m12CHF, m12CHF);
        Assert.assertEquals(m12CHF, new Money(12, "CHF")); // (1)
        Assert.assertTrue(!m12CHF.equals(m14CHF));
    }
}
　　当要同时进行测试Add和equals方法时，可以将它们的各自的初始化工作，合并到一起进行，形成测试基础,用setUp初始化，用tearDown清除。如下：
public class MoneyTest extends TestCase {//TestCase的子类
    private Money f12CHF;//提取公用的对象
    private Money f14CHF;   
    protected void setUp() {//初始化公用对象
        f12CHF= new Money(12, "CHF");
        f14CHF= new Money(14, "CHF");
    }
    public void testEquals() {//测试equals方法的正确性
        Assert.assertTrue(!f12CHF.equals(null));
        Assert.assertEquals(f12CHF, f12CHF);
        Assert.assertEquals(f12CHF, new Money(12, "CHF"));
        Assert.assertTrue(!f12CHF.equals(f14CHF));
    }
    public void testSimpleAdd() {//测试add方法的正确性
        Money expected= new Money(26, "CHF");
        Money result= f12CHF.add(f14CHF);
        Assert.assertTrue(expected.equals(result));
    }
}
　　将以上三个中的任一个TestCase子类代码保存到名为MoneyTest.java的文件里，并在文件首行增加
import junit.framework.*;
，都是可以运行的。关于Junit运行的问题很有意思，下面单独说明。
　　上面为解释概念“测试基础(fixture)”，引入了两个对两个方法的测试。命令模式与集成模式的本质区别是，前者一次只运行一个测试。
- 集成模式
　　利用TestSuite可以将一个TestCase子类中所有test***()方法包含进来一起运行，还可将TestSuite子类也包含进来，从而行成了一种等级关系。可以把TestSuite视为一个容器，可以盛放TestCase中的test***()方法，它自己也可以嵌套。这种体系架构，非常类似于现实中程序一步步开发一步步集成的现况。
　　对上面的例子，有代码如下：
public class MoneyTest extends TestCase {//TestCase的子类
    ....
    public static Test suite() {//静态Test
        TestSuite suite= new TestSuite();//生成一个TestSuite
        suite.addTest(new MoneyTest("testEquals")); //加入测试方法
        suite.addTest(new MoneyTest("testSimpleAdd"));
        return suite;
    }
}
　　从Junit2.0开始，有列简捷的方法:
public class MoneyTest extends TestCase {//TestCase的子类
    ....
    public static Test suite() {静态Test
        return new TestSuite(MoneyTest.class); //以类为参数
    }
}
　　TestSuite见嵌套的例子，在后面应用案例中有。
**4、测试代码的运行**
　　先说最常用的集成模式。
　　测试代码写好以后，可以相应的类中写main方法，用java命令直接运行；也可以不写main方法，用Junit提供的运行器运行。Junit提供了textui,awtui和swingui三种运行器。
　　以前面第2步中的AllTests运行为例，可有四种：
java junit.textui.TestRunner junit.samples.AllTests
java junit.awtui.TestRunner junit.samples.AllTests
java junit.swingui.TestRunner junit.samples.AllTests
java junit.samples.AllTests
　　main方法中一般也都是简单地用Runner调用suite()，当没有main时，TestRunner自己以运行的类为参数生成了一个TestSuite.
　　对于命令模式的运行，有两种方法。
- 静态方法
TestCase test= new MoneyTest("simple add") {
public void runTest() {
testSimpleAdd();
}
};
- 动态方法
TestCase test= new MoneyTest("testSimpleAdd");
　　我试了一下，~~好象有问题，哪位朋友成功了，请指点我一下。~~确实可以。
import junit.framework.*;
public class MoneyTest extends TestCase {//TestCase的子类
    private Money f12CHF;//提取公用的对象
    private Money f14CHF;   
    public MoneyTest(String name){
        super(name);
    }
    protected void setUp() {//初始化公用对象
        f12CHF= new Money(12, "CHF");
        f14CHF= new Money(14, "CHF");
    }
    public void testEquals() {//测试equals方法的正确性
        Assert.assertTrue(!f12CHF.equals(null));
        Assert.assertEquals(f12CHF, f12CHF);
        Assert.assertEquals(f12CHF, new Money(12, "CHF"));
        Assert.assertTrue(!f12CHF.equals(f14CHF));
    }
    public void testAdd() {//测试add方法的正确性
        Money expected= new Money(26, "CHF");
        Money result= f12CHF.add(f14CHF);
        Assert.assertTrue(expected.equals(result));
    }
//    public static void main(String[] args) {
//        TestCase test=new MoneyTest("simple add") {
//                public void runTest() {
//                    testAdd();
//                }
//            };
//        junit.textui.TestRunner.run(test);
//    }
    public static void main(String[] args) {
        TestCase test=new MoneyTest("testAdd");
        junit.textui.TestRunner.run(test);
    }
}
再给一个静态方法用集成测试的例子：
public static Test suite() {
    TestSuite suite= new TestSuite();
    suite.addTest(
        new testCar("getWheels") {
            protected void runTest() { testGetWheels(); }
        }
    );
    suite.addTest(
        new testCar("getSeats") {
            protected void runTest() { testGetSeats(); }
        }
    );
    return suite;
}
**单元测试基础**
当今软件测试十分盛行时，本人通过项目实践和个人亲身体会浅谈单元测试，本人一直坚持“用代码说话的原则”，同时也希望个人能给出宝贵意见，共同探讨、共同进步，为中国软件事业有更大的发展共同奋斗！
最早我们项目组开发的项目时，写代码都是从底层一直写到表现层到jsp，然后开发人员在web层调试页面，近乎98％都会报一大堆exception,然后再在代码中加断点一步一步查到底哪一层代码出现问题……,比较好点做法就是在各个类中加上main方法测试，但总体很不理想，给web层开发人员的调试和质量控制人员带来繁重的工作压力；使用单元测试后，针对每一个方法都做严格的把关，大大减少调试的时间；同时质量控制人员返回过来的bug少了近60％，现在对于开发人员写测试用例非常熟练，并且本人根据实际情况对测试用例做了点小小改动（这部分主要在后面代码中详述），带来很好的效果！
**单元测试到底给实际开发带来什么好处那？**
（1）           （1）首先对于开发人员来说大大减少调试工作的时间，同时也规范了对于代码安全管理（我们知道那些方法是可以调用的）；
（2）           （2） 对于整个项目来说，有了完整的测试，保证项目最后交付测试有了可靠依据；
（3）           （3）对于测试人员大大减少bug的反馈；
（4）           （4）对于项目经理整个项目达到很好的可控；
（5）          （5）最主要的完整的单元测试给后期维护人员带来很大的便捷！
单元测试好处可能还有很多，但本人只能理解和感悟这么多,希望观者补充！
**单元测试配置：**
       我将使用eclipse＋myEclopse给大家介绍关于JUNIT的环境的简单配置；右键点击项目选择“属性”，在弹出窗口中到环境变量中添加junit.jar包，这样下一步我们就可以进行单元测试了；
![](http://www.51testing.com/ddimg/uploadimg/20061009/1105300.JPG)
**使用eclipse快速开发test Case：**
如下图：右键选择你要测试的类，在新建中点击“JUnit 测试用例”，
![](http://www.51testing.com/ddimg/uploadimg/20061009/1105301.JPG)
弹出对话框，配置测试名称和根目录，添加注释等，再点击“下一步”到下图：
![](http://www.51testing.com/ddimg/uploadimg/20061009/1105302.JPG)
      选择你要测试类中的方法，点击完成！便生成测试类的基本框架，如下代码，我们以对一个DAO类测试为例：
/*
  * Copyright reserved 2005 by XXXXCo. Ltd.
  * Author：Fu wei    Date：2006-9-4
  */
import junit.framework.TestCase;
/**
  * @author Fu wei
  */
publicclass OrgTypeDAOTest extends TestCase {
/**
      * @param arg0
      */
public OrgTypeDAOTest(String arg0) {
super(arg0);
     }
/* 
      * @see junit.framework.TestCase#setUp()
      */
protectedvoid setUp() throws Exception {
super.setUp();
     }
/* 
      * @see junit.framework.TestCase#tearDown()
      */
protectedvoid tearDown() throws Exception {
super.tearDown();
     }
/**
      *  主函数
      * @param args
      */
publicstaticvoid main(String[] args){
         TestRunner.run(OrgTypeDAOTest .class);
     }
/**
      * {@link OrgTypeDAO#getOrgTypeList()} 的测试方法。
      */
publicfinalvoid testGetOrgTypeList() {
         fail("尚未实现"); // TODO
    }
/**
      * {@link OrgTypeDAO#insertOrgTypeInfo(com.zhjy.mltx.vo.OrgTypeVO)} 的测试方法。
      */
publicfinalvoid testInsertOrgTypeInfo() {
         fail("尚未实现"); // TODO
    }
/**
      * {@link OrgTypeDAO#deleteOrgTypeInfo(java.lang.String)} 的测试方法。
      */
publicfinalvoid testDeleteOrgTypeInfo() {
         fail("尚未实现"); // TODO
    }
/**
      * {@link OrgTypeDAO#updateOrgTypeInfo(com.zhjy.mltx.vo.OrgTypeVO)} 的测试方法。
      */
publicfinalvoid testUpdateOrgTypeInfo() {
         fail("尚未实现"); // TODO
    }
/**
      * {@link OrgTypeDAO#getOrgTypeInfoById(java.lang.String)} 的测试方法。
      */
publicfinalvoid testGetOrgTypeInfoById() {
         fail("尚未实现"); // TODO
    }
/**
      * {@link OrgTypeDAO#isRepeatOrgTypeInfo(java.lang.String)} 的测试方法。
      */
publicfinalvoid testIsRepeatOrgTypeInfoString() {
         fail("尚未实现"); // TODO
    }
/**
      * {@link OrgTypeDAO#isRepeatOrgTypeInfo(com.zhjy.mltx.vo.OrgTypeVO)} 的测试方法。
      */
publicfinalvoid testIsRepeatOrgTypeInfoOrgTypeVO() {
         fail("尚未实现"); // TODO
    }
/**
      * {@link OrgTypeDAO#getFlatOrgIdByName(java.lang.String)} 的测试方法。
      */
publicfinalvoid testGetFlatOrgIdByName() {
         fail("尚未实现"); // TODO
    }
 }
**JUnit****单元测试一共要注意一下几点：**
（1）import junit.framework.TestCase 和junit.textui.TestRunner；
（2）继承junit.framework.TestCase ；
（3）自行添加一个main方法中调用TestRunner.run(测试类名.class);
（4）有一个调用super(String)的构造函数；
以上都是JUnit必有的特征，除以上外，我们发现有许多以test开头的方法，而这些方法正是我们要测试的方法，Junti测试其实采用的是断言的方式，只要我们在所有test开头中的方法对数据添加断言方法，同时提供很多断言的方法，
assertSame("失败提示信息","期望数据","测试数据")断言获取数据是否与所期望的相同
当我们写完所有方法策略后，JUnit测试如下图：
在方法页面中点击右键在“调试方式”或“运行方式”中点击“**JUnit 测试**”，就运行测试类！
![](http://www.51testing.com/ddimg/uploadimg/20061009/1105303.JPG)
**在执行测试类时，执行的大概过程**：
（1）先执行构造方法public OrgTypeDAOTest(String arg0) ；
（2）再执行初始化数据方法protected void setUp() ；
（3）再执行以test开头的测试方法；
（4）最后执行protected void tearDown()方法清理对象；
如果测试失败或者错误，将会显示一个红色的亮条；如果测试通过将显示绿色亮条；如下图
![](http://www.51testing.com/ddimg/uploadimg/20061009/1105304.JPG)
