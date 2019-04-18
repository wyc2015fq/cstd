# Android测试系列-JUnit(一) - weixin_33985507的博客 - CSDN博客
2017年09月19日 10:26:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：15
#### 目录
- 简介
- Android下的JUnit
- 环境搭建
- 简单测试
#### 简介
JUnit 是一个 Java 编程语言的单元测试框架。JUnit 在测试驱动的开发方面有很重要的发展，是起源于 JUnit 的一个统称为 xUnit 的单元测试框架之一。
JUnit 促进了“先测试后编码”的理念，强调建立测试数据的一段代码，可以先测试，然后再应用。增加了程序员的产量和程序的稳定性。
> 
特点
- JUnit 是一个开放的资源框架，用于编写和运行测试。
- 提供注释来识别测试方法。
- 提供断言来测试预期结果。
- 提供测试运行来运行测试。
- JUnit 测试允许你编写代码更快，并能提高质量。
- JUnit 优雅简洁。没那么复杂，花费时间较少。
- JUnit 测试可以自动运行并且检查自身结果并提供即时反馈。所以也没有必要人工梳理测试结果的报告。
- JUnit 测试可以被组织为测试套件，包含测试用例，甚至其他的测试套件。
- JUnit 在一个条中显示进度。如果运行良好则是绿色；如果运行失败，则变成红色。
#### Android下的JUnit
因为Android代码的书写是基于Java语音，所以JUnit同样适用于Android开发中。
我们可以使用JUnit来测试一些方法和类，仅仅不牵扯到布局的部分，因此我们可以来测我们的逻辑代码和一些算法。关于布局的测试可以使用espresso框架，关于activity的测试可以使用Mockito框架。
#### 环境搭建
```
Android studio：2.3.3
 JDK：1.8
 Junit：4.12
```
我们在使用Android studio创建项目的时候，AS会默认帮我们创建相关的测试文件，并将JUnit框架自动导入。
```
文件目录：
       androidTest
       test
框架导入：
       testCompile 'junit:junit:4.12'
```
#### 简单测试
创建步骤：
1、创建一个新的包test用于测试使用
2、在test包下创建SimpleTest.java用于简单测试
3、在文件中写上加、减、乘、除四个方法
```
public class SimpleTest {
    public int plus(int a, int b) {
        return a + b;
    }
    public int less(int a, int b) {
        /*if (a >= b) {
            return a - b;
        } else {
            return b - a;
        }*/
        return a - b;
    }
    public double multiply(int a, int b) {
        return a * b;
    }
    public double except(int a, int b) {
        return a / b;
    }
}
```
4、直接右键选择go to—>test，会出现一个提示框，选择Create New Test…（第一次创建），之后会出现如下图：
![1115152-3afca32209e76d5a.png](https://upload-images.jianshu.io/upload_images/1115152-3afca32209e76d5a.png)
创建测试.png
5、如图选中各个方法，点击OK，然后会出现选择创建文件的位置，一般选择test文件夹。
6、在test文件夹下会出现一个SimpleTestTest.java文件，这个文件就是测试文件。
7、添加如下代码：
```
public class SimpleTestTest {
private SimpleTest simpleTest;
@Before
public void setUp() throws Exception {
    simpleTest = new SimpleTest();
}
@Test
public void plus() throws Exception {
    Assert.assertEquals(5, simpleTest.plus(2, 3), 0);
}
@Test
public void less() throws Exception {
    Assert.assertEquals(2, simpleTest.less(6, 4), 0);
}
@Test
public void multiply() throws Exception {
    Assert.assertEquals(6, simpleTest.multiply(2, 3), 0);
}
@Test
public void except() throws Exception {
    Assert.assertEquals(3, simpleTest.except(6, 2), 0);
}
}
```
8、选中SimpleTestTest.java文件，右键选择Run SimpleTestTest就可以运行了，运行成功后，会出现如下图片：
![1115152-d8e49b5c3214c12c.png](https://upload-images.jianshu.io/upload_images/1115152-d8e49b5c3214c12c.png)
运行成功.png
9、如果运行失败则会出现如下图片：
![1115152-545555aa65e4ac1b.png](https://upload-images.jianshu.io/upload_images/1115152-545555aa65e4ac1b.png)
运行失败.png
10、这就是JUnit的最进本的流程
#### 最后
接下来我们会都JUnit框架做进一步的了解，敬请期待。
[源码地址](https://link.jianshu.com?t=https://github.com/ZGaoFei/JUnitTest/tree/master)
