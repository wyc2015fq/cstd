# JUnit 单元测试 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [咸菜](http://www.jobbole.com/members/Pickle) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
# JUnit 单元测试解析
## 1.首先看看什么是 JUnit：
JUnit官网对JUnit的简单解释：
JUnit is a simple framework to write repeatable tests. It is an instance of the xUnit architecture for unit testing frameworks.
JUnit是一个简单的可复用的测试框架，是xUnit测试框架的一个子集。
#### xUnit 又是什么呢？
xUnit是一套基于测试驱动开发的测试框架。
xUnit包括：PythonUnit、CppUnit等等。JUnit应该算是xUnit家族最成功的一个了。
## 2.为什么要使用 JUnit
使用JUnit能够帮助我们减少在开发过程中的错误，把Bug扼杀在萌芽之中，有利于代码的后期维护和检查。做好了单元测试可以缩短开发周期，提高代码质量。这样我们就可以把更多的时间用到我们应该干的事情上来而不是去解决项目后期发现的越来越多的令人头疼的问题。
**测试不是用来证明你是对的，而是用来证明你没有错。**
## 3.下载安装 JUnit
JUnit官网：[http://junit.org/](http://junit.org/)
首先访问JUnit官网，找到welcome模块：
![](http://jbcdn2.b0.upaiyun.com/2016/08/d24761c654865983add77a14bd9ed5f0.png)
然后点击下载相应版本的JUnit就OK了。
## 4.使用 JUnit（这里使用的 JUnit4，IDE 为 MyEclipse）
### 4.1首先创建一个使用 JUnit 进行单元测试的 Java 项目：
准守一定的规则会使我们的项目和代码看起来更加的和谐。
一般来说规则是这样的：
a. 在Java项目（这里为JUnitTest）中创建一个名为test的source folder。
b. 在test目录中创建和src目录下相同的包名，在其中存放对应的测试类。
c. 不要忘了引入JUnit及其依赖jar包。
等项目完成之后把test测试源码文件删除即可，不影响其他的代码。
项目的结构目录看起来应该是这样的：
![](http://jbcdn2.b0.upaiyun.com/2016/08/be11e64ed8be3ab011f45daf84af37b7.png)
这样我们就可以来进行测试代码的编写了。
### 4.2一个简单的例子
a.首先在src/util包下创建Calculator.java，编写一个简单的加法运算器。

```
package util;
/**
 *被测试的类
 *@author wxisme
 *@ time 2015-9-2 上午9:52:24
 */
public class Calculator {
    
    public int add(int a, int b) {
        return a + b;
    }
}
```
b. 在test/util包下创建CalculatorTest.java作为Calculator的测试类。

```
package util;
import junit.framework.Assert;
import org.junit.Before;
import org.junit.Test;
import util.Calculator;
/**
 *
 *@author wxisme
 *@ time 2015-9-1 下午8:44:15
 */
public class CalculatorTest {
    
    private static Calculator calculator;
    
    @BeforeClass
    public static void BuildCalculator() {
        calculator = new Calculator();
    }
    
    @ Test
    public void testAdd() {
        
        Assert.assertEquals(8, calculator.add(3, 5));
        
    }
}
```
上面的程序JUnit4提供的注解和方法。
首先看@Test注解，见名知意，是标注testAdd()方法为一个测试方法。
Assert.assertEquals(8, calculator.add(3, 5));方法是JUnit中Assert类提供的断言方法。能够判断我们的预期结果和程序的输出结果是否是一致的。
我们来测试一下：选中testAdd()方法,右键选择run as JUnit test：
![](http://jbcdn2.b0.upaiyun.com/2016/08/03b57bf276ecc7f2202a230e554fcff9.png)
上面程序运行的结果应该是这样的：
![](http://jbcdn2.b0.upaiyun.com/2016/08/b51e98186328e1adbc3ed626d1968333.png)
看到有绿色的条，并且Error和Failure的值都为零，说明我们的测试通过。程序运行结果与我们所期望的是一致的。
那如果把testAdd()方法中的断言改成：

```
Assert.assertEquals(8, calculator.add(3, 5));
```
运行结果会是怎样的呢？
应该是这样的：
![](http://images2015.cnblogs.com/blog/735119/201509/735119-20150902172433778-771760756.png)
从红色圈起来的地方我们可以得知，测试没有通过，并且错误在于，我们期望的值为7，但是程序的输出结果为8.
c. 通过上面的例子我们可以有一下总结：
- 测试方法必须使用@Test修饰
- 测试方法必须使用public void修饰
- 新建test源代码目录来存放测试代码
- 测试类的包名应该和被测试类的报名保持一致
- 测试单元中的每个方法必须可以独立测试，测试方法之间不能有任何依赖
- 测试类一般使用Test作为后缀
- 测试方法一般使用test作为前缀
### 4.3系统的来看 JUnit 中注解和某些方法的用法
JUnit4中提供的常用注解大致包括：
**@BeforeClass**
**@AfterClass**
**@Before**
**@After**
**@Test**
**@Ignore**
**@RunWith**
我们通过一个例子来了解他们的用法：
编写一个MethodTest来测试注解的用法。

```
package util;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
/**
 *
 *@author wxisme
 *@ time 2015-9-2 上午11:28:29
 */
public class MethodTest {
    @BeforeClass   //用static修饰
    public static void setUpBeforeClass() throws Exception {
        System.out.println("beforeClass...");
    }
    @AfterClass        //用static修饰
    public static void tearDownAfterClass() throws Exception {
        System.out.println("afterClass...");
    }
    @Before
    public void setUp() throws Exception {
        System.out.println("before...");
    }
    @After
    public void tearDown() throws Exception {
        System.out.println("after...");
    }
    @ Test
    public void test1() {
        System.out.println("test1...");
//        fail("Not yet implemented");
    }
    
    @ Test
    public void test2() {
        System.out.println("test2...");
//        fail("Not yet implemented");
    }
}
```
测序的运行后再Console的中输出如下：

```
beforeClass...
before...
test1...
after...
before...
test2...
after...
afterClass...
```
其中我们可以总结出这几个注解的执行顺序和作用：
@Test将一个普通的方法修饰为一个测试方法
@BeforeClass会在所有方法运行前执行，static修饰
@AfterClass会在所有方法运行结束后执行，static修饰
@Before会在每个测试方法运行前执行一次
@After会在每个测试方法运行后被执行一次
这就是为什么在第一个例子中我会这么干了（所有的测试方法只使用一个Calculator就行了）：

```
private Calculator calculator;
    
    @BeforeClass
    public static void BuildCalculator() {
        calculator = new Calculator();
    }
```
@Test注解还可以带有参数。像这样：
**@Test(expected=xx.class)**
** @Test(timeout=毫秒)**
timeout参数很容易理解，就是指定程序运行的超时时间，单位为毫秒。
我们来看一下expected参数。
看一个例子：
在第一个例子里面加一个Exception异常属性，在testAdd()方法中抛出。

```
package util;
import junit.framework.Assert;
import org.junit.Before;
import org.junit.Test;
import util.Calculator;
/**
 *
 *@author wxisme
 *@ time 2015-9-1 下午8:44:15
 */
public class CalculatorTest {
    
    private static Calculator calculator;
    private static Exception ex;
    
    @BeforeClass
    public static void BuildCalculator() {
        calculator = new Calculator();
        ex = new Exception("手动抛出的异常！");
    }
    
    @ Test
    public void testAdd() throws Exception {
        
        Assert.assertEquals(8, calculator.add(3, 5));
        throw ex;
    }
}
```
结果会是这样的：
![](http://jbcdn2.b0.upaiyun.com/2016/08/95c44faf27ed1cb66eb7d69ffda061e9.png)
发现测试没有通过，并且抛出了一个异常，使我们手动抛出的异常。
那么把上面的程序总@Test注解加上这样的参数：
@Test(expected=Exception.class)
再看结果：
![](http://jbcdn2.b0.upaiyun.com/2016/08/9a9b91942882aa687db27211f8ea84e3.png)
发现测试通过了。
是不是突然就明白expected参数的作用了呢？
其他几个注解的功能可以类比。
再来看@Ignore和@RunWith
@Ignore见名知意，作用是让测试运行器忽略其修饰的测试方法。@Ignore所修饰的方法不会执行。
自行测试。
@RunWith可以用来更改运行测试器org.junit.runner.Runner。
下面的两个例子都是通过@RunWith注解来实现的。
1）测试套件试想如果项目中有100个测试类需要测试，你会怎么做呢？每个类运行一次？运行100次？
JUnit当然不会只允许有这一个愚蠢的方法。答案就是测试套件。
测试套件用来组织批量运行测试类。看一个例子。
首先来创建3个测试类模拟需要批量运行的测试类群。

```
public class DemoTest1 {
    @ Test
    public void test() {
        System.out.println("DemoTest1...");
    }
}
public class DemoTest2 {
    @ Test
    public void test() {
        System.out.println("DemoTest2...");
    }
}
public class DemoTest3 {
    @ Test
    public void test() {
        System.out.println("DemoTest3...");
    }
}
```
然后创建一个名为SuiteTest的测试类，并加上@RunWith注解和@Suite.SuiteClasses注解。（该类中没有其他测试方法）

```
package util;
import org.junit.runner.RunWith;
import org.junit.runners.Suite;
/**
 *测试套件
 *@author wxisme
 *@ time 2015-9-2 下午7:37:56
 */
@RunWith(Suite.class)
@Suite.SuiteClasses({DemoTest1.class,DemoTest2.class,DemoTest3.class})
public class SuiteTest {
}
```
测试程序总首先用@RunWith来修改测试运行器。
然后使用@Suite.SuiteClasses来指定要批量执行的测试类（数组的形式）
程序的运行结果应该是这样的（通过测试并且在Console中输出）：
![](http://jbcdn2.b0.upaiyun.com/2016/08/0bf084a23bfd54910da9ee7904da69ea.png)
可见我们的批量运行生效了。
#### 2）JUnit 参数化设置
再试想一个场景：如果一个测试方法中需要测试100组数据是否和期望值一致，你会怎么做呢？手动copy+change100次？
JUnit也不会只允许使用这么愚蠢的方法。这就用到了JUnit的参数化设置。
看一个例子：
a.首先创建一个名为ParameterTest的测试类，并用RunWith注解来改变测试运行器。
b.声明变量来存放预期值和结果值。
c.声明一个返回值为Collection的公共静态方法，并使用 @Parameters进行修饰。
d.为测试类声明一个带有参数的公共构造方法，并在其中为之声明变量赋值。
代码应该是这样的：

```
package util;
import java.util.Arrays;
import java.util.Collection;
import junit.framework.Assert;
import org.junit.BeforeClass;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.junit.runners.Parameterized.Parameters;
/**
 *JUnit参数化设置
 *@author wxisme
 *@ time 2015-9-2 下午8:03:47
 */
@RunWith(Parameterized.class)
public class ParameterTest {
    
    private static Calculator calculator;
    
    @BeforeClass
    public static void setUpBeforeClass() {
        calculator = new Calculator();
    }
    
    private int expected;
    private int input1;
    private int input2;
    
    @Parameters
    public static Collection<Object[]> setParameters() {
        Object[][] objs = {{8,3,5},{5,3,2}};
        
        return Arrays.asList(objs);
    }
    
    public ParameterTest(int expected, int input1, int input2) {
        this.expected = expected;
        this.input1 = input1;
        this.input2 = input2;
    }
    
    @ Test
    public void testParameters() {
        Assert.assertEquals(expected, calculator.add(input1, input2));
    }
}
```
然后我们运行这个测试类（要在类的级别上运行，如果在方法上运行就会有初始化错误）。
结果应该是这样的：
![](http://jbcdn2.b0.upaiyun.com/2016/08/02d9cc622fb6fd824fa676c6ef1743cb.png)
运行结果显示两组测试数据均测试通过。
到此JUnit的基本用法就介绍完了，关于JUnit的其他用法以及断言的API请参考官方提供的document（如果你不想自己找的话，在评论区留下邮箱，我会发给你的哦）。
# 5.总结
JUnit带给我们的不仅是开发效率、代码质量的提高，更是一种思想的提高，现在都在讲测试驱动开发、回归质量大概就是这种思想。
JUnit使用起来不仅简单方便，其源码更是短小精悍，Erich Gamma 是著名的 GoF 之一，在JUnit中设计模式的使用堪称经典，有优良的扩展性和可重用性。值得细细品味。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2016/09/c67bd37238d8422176f282e5b98a80a9.png)![](http://jbcdn2.b0.upaiyun.com/2016/09/40382ba66fc52885f43198654438713e.jpg)
