
# TestNG 入门教程 - anlcy - 博客园






# [TestNG 入门教程](https://www.cnblogs.com/camilla/p/7761198.html)

# TestNG介绍
TestNG是Java中的一个测试框架， 类似于JUnit 和NUnit,   功能都差不多， 只是功能更加强大，使用也更方便
Java中已经有一个JUnit的测试框架了。  TestNG比JUnit功能强大的多。  测试人员一般用TestNG来写自动化测试。  开发人员一般用JUnit写单元测试。
官方网站:[http://testng.org/doc/index.html](http://testng.org/doc/index.html)

# 在Eclipse中在线安装TestNG
打开Eclipse   Help ->Install New Software ,   然后Add   "[http://beust.com/eclipse](http://beust.com/eclipse)"
![](https://images0.cnblogs.com/i/263119/201408/030942544628581.png)

# 在Eclipse中离线安装Testng
首先下载testng离线包:[http://pan.baidu.com/s/1hrEaX6k](http://pan.baidu.com/s/1hrEaX6k)
1. 将解压后的文件..\eclipse-testng离线包\features\org.testng.eclipse_6.9.9.201510270734　  文件夹　放到　　　　　　eclipse--》features目录下
2. 将解压后的文件..\eclipse-testng离线包\plugins\org.testng.eclipse_6.9.8.201510130443　 文件夹　 放到                 eclipse--》plugins目录下
3. 重启eclipse
4. 验证是否安装成功，file-->new-->other-->TestNg
![](https://images2015.cnblogs.com/blog/263119/201604/263119-20160408072122468-1248913039.png)
# TestNG最简单的测试
下面是TestNG的最简单的一个例子
![复制代码](https://common.cnblogs.com/images/copycode.gif)
packageTankLearn2.Learn;importorg.junit.AfterClass;importorg.junit.BeforeClass;importorg.testng.annotations.Test;publicclassTestNGLearn1 {
    @BeforeClasspublicvoidbeforeClass() {
        System.out.println("this is before class");
    }
    @TestpublicvoidTestNgLearn() {
        System.out.println("this is TestNG test case");
    }
    @AfterClasspublicvoidafterClass() {
        System.out.println("this is after class");
    }
}![复制代码](https://common.cnblogs.com/images/copycode.gif)

# TestNG的基本注解
|注解|描述|
|@BeforeSuite|注解的方法将只运行一次，运行所有测试前此套件中。|
|@AfterSuite|注解的方法将只运行一次此套件中的所有测试都运行之后。|
|@BeforeClass|注解的方法将只运行一次先行先试在当前类中的方法调用。|
|@AfterClass|注解的方法将只运行一次后已经运行在当前类中的所有测试方法。|
|@BeforeTest|注解的方法将被运行之前的任何测试方法属于内部类的 <test>标签的运行。|
|@AfterTest|注解的方法将被运行后，所有的测试方法，属于内部类的<test>标签的运行。|
|@BeforeGroups|组的列表，这种配置方法将之前运行。此方法是保证在运行属于任何这些组第一个测试方法，该方法被调用。|
|@AfterGroups|组的名单，这种配置方法后，将运行。此方法是保证运行后不久，最后的测试方法，该方法属于任何这些组被调用。|
|@BeforeMethod|注解的方法将每个测试方法之前运行。|
|@AfterMethod|被注释的方法将被运行后，每个测试方法。|
|@DataProvider|标志着一个方法，提供数据的一个测试方法。注解的方法必须返回一个Object[] []，其中每个对象[]的测试方法的参数列表中可以分配。
|该@Test 方法，希望从这个DataProvider的接收数据，需要使用一个dataProvider名称等于这个注解的名字。|
|@Factory|作为一个工厂，返回TestNG的测试类的对象将被用于标记的方法。该方法必须返回Object[]。|
|@Listeners|定义一个测试类的监听器。|
|@Parameters|介绍如何将参数传递给@Test方法。|
|@Test|标记一个类或方法作为测试的一部分。|

# TestNG中如何执行测试
第一种直接执行：右键要执行的方法，　　点Run As ->TestNG Test
![](https://images0.cnblogs.com/blog/263119/201410/050727158475617.png)

# 使用testtng.xml 文件执行 case
第二种:  通过testng.xml文件来执行. 把要执行的case, 放入testng.xml文件中。 右键点击testng.xml,   点Run As
testng.xml
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?><!DOCTYPE suite SYSTEM "http://testng.org/testng-1.0.dtd" >
<suite name="Suite1">
    <test name="test12">
        <classes>
            <class name="TankLearn2.Learn.TestNGLearn1" />
        </classes>
    </test>
</suite>![复制代码](https://common.cnblogs.com/images/copycode.gif)
![](https://images0.cnblogs.com/blog/263119/201410/050729215814295.png)

# TestNG按顺序执行Case
在testng.xml中，可以控制测试用例按顺序执行。  当preserve-order="true"是，可以保证节点下面的方法是按顺序执行的
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?><!DOCTYPE suite SYSTEM "http://testng.org/testng-1.0.dtd"><suitename="Suite1"><testname="test12"preserve-order="true"><classes><classname="TankLearn2.Learn.TestNGLearn1"><methods><includename="TestNgLearn3"/><includename="TestNgLearn1"/><includename="TestNgLearn2"/></methods></class></classes></test></suite>![复制代码](https://common.cnblogs.com/images/copycode.gif)


# TestNG异常测试
测试中，有时候我们期望某些代码抛出异常。
TestNG通过@Test(expectedExceptions)  来判断期待的异常， 并且判断Error Message
![复制代码](https://common.cnblogs.com/images/copycode.gif)
packageTankLearn2.Learn;importorg.testng.annotations.Test;publicclassExceptionTest {
    
    @Test(expectedExceptions = IllegalArgumentException.class, expectedExceptionsMessageRegExp="NullPoint")publicvoidtestException(){thrownew IllegalArgumentException("NullPoint");
    }
}![复制代码](https://common.cnblogs.com/images/copycode.gif)

# TestNG组测试
TestNG中可以把测试用例分组，这样可以按组来执行测试用例比如：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
packageTankLearn2.Learn;importorg.testng.annotations.Test;publicclassGroupTest {
    
    @Test(groups = {"systemtest"})publicvoidtestLogin(){
        System.out.println("this is test login");
    }
    
    @Test(groups = {"functiontest"})publicvoidtestOpenPage(){
        System.out.println("this is test Open Page");
    }
}![复制代码](https://common.cnblogs.com/images/copycode.gif)

然后在testng.xml中 按组执行测试用例
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?><!DOCTYPE suite SYSTEM "http://testng.org/testng-1.0.dtd"><suitename="Suite1"><testname="test1"><groups><run><includename="functiontest"/></run></groups></test></suite>![复制代码](https://common.cnblogs.com/images/copycode.gif)

# TestNG参数化测试
软件测试中，经常需要测试大量的数据集。 测试代码的逻辑完全一样，只是测试的参数不一样。  这样我们就需要一种 “传递测试参数的机制”。 避免写重复的测试代码
TestNG提供了2种传递参数的方式。
第一种: testng.xml 方式使代码和测试数据分离，方便维护
第二种：@DataProvider能够提供比较复杂的参数。 (也叫data-driven testing)
方法一：　通过testng.xml 传递参数给测试代码
![复制代码](https://common.cnblogs.com/images/copycode.gif)
packageTankLearn2.Learn;importorg.testng.annotations.Parameters;importorg.testng.annotations.Test;publicclassParameterizedTest1 {
    
    @Test
    @Parameters("test1")publicvoidParaTest(String test1){
        System.out.println("This is " +test1);
    }
}![复制代码](https://common.cnblogs.com/images/copycode.gif)
testng.xml
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?><!DOCTYPE suite SYSTEM "http://testng.org/testng-1.0.dtd"><suitename="Suite1"><parametername="test1"value="Tank"/><parametername="test1"value="Xiao"/><testname="test12"><classes><classname="TankLearn2.Learn.ParameterizedTest1"/></classes></test></suite>![复制代码](https://common.cnblogs.com/images/copycode.gif)

方式二:   通过DataProvider传递参数
![复制代码](https://common.cnblogs.com/images/copycode.gif)
packageTankLearn2.Learn;importorg.testng.annotations.DataProvider;importorg.testng.annotations.Test;publicclassDataProviderLearn {
    
    @DataProvider(name="user")publicObject[][] Users(){returnnewObject[][]{
                {"root","passowrd"},
                {"cnblogs.com", "tankxiao"},
                {"tank","xiao"}
        };
    }
    
    @Test(dataProvider="user")publicvoidverifyUser(String userName, String password){
        System.out.println("Username: "+ userName + " Password: "+password);
    }
}![复制代码](https://common.cnblogs.com/images/copycode.gif)

# TestNG忽略测试
有时候测试用例还没准备好， 可以给测试用例加上@Test(enable = false)，  来禁用此测试用例
![复制代码](https://common.cnblogs.com/images/copycode.gif)
packageTankLearn2.Learn;importorg.testng.annotations.Test;publicclassTesgNGIgnore {
    
    @Test(enabled =false)publicvoidtestIgnore(){
        System.out.println("This test case will ignore");
    }
}![复制代码](https://common.cnblogs.com/images/copycode.gif)

# TestNG 依赖测试
有时候，我们需要按顺序来调用测试用例，  那么测试用例之间就存在依赖关系。 TestNG支持测试用例之间的依赖
![复制代码](https://common.cnblogs.com/images/copycode.gif)
packageTankLearn2.Learn;importorg.testng.annotations.Test;publicclassDependsTest {
    
    @TestpublicvoidsetupEnv(){
        System.out.println("this is setup Env");
    }
    
    @Test(dependsOnMethods = {"setupEnv"})publicvoidtestMessage(){
        System.out.println("this is test message");
    }
}![复制代码](https://common.cnblogs.com/images/copycode.gif)

# TestNG测试结果报告
测试报告是测试非常重要的部分.
TestNG默认情况下，会生产两种类型的测试报告HTML的和XML的。 测试报告位于 "test-output" 目录下.
![](https://images0.cnblogs.com/blog/263119/201410/050741002222302.png)
当然我们也可以设置测试报告的内容级别.
verbose="2" 标识的就是记录的日志级别，共有0-10的级别，其中0表示无，10表示最详细
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?><!DOCTYPE suite SYSTEM "http://testng.org/testng-1.0.dtd"><suitename="Suite1"><testname="test12"verbose="2"><classes><classname="TankLearn2.Learn.TestNGLearn1"/></classes></test></suite>![复制代码](https://common.cnblogs.com/images/copycode.gif)






