# CppUnit测试框架在VC2008中应用 - 深之JohnChen的专栏 - CSDN博客

2010年07月19日 19:00:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2544标签：[测试																[框架																[单元测试																[mfc																[include																[junit](https://so.csdn.net/so/search/s.do?q=junit&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=单元测试&t=blog)](https://so.csdn.net/so/search/s.do?q=框架&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)
个人分类：[软件测试																[日记博文](https://blog.csdn.net/byxdaz/article/category/144576)](https://blog.csdn.net/byxdaz/article/category/506291)


测试驱动开发(TDD)是以测试作为开发过程的中心，它坚持，在编写实际代码之前，先写好基于产品代码的测试代码。开发过程的目标就是首先使测试能够通过，然后再优化设计结构。测试驱动开发式是极限编程的重要组成部分。XUnit，一个基于测试驱动开发的测试框架，它为我们在开发过程中使用测试驱动开发提供了一个方便的工具，使我们得以快速的进行单元测试。XUnit的成员有很多，如JUnit，PythonUnit等。今天给大家介绍的CppUnit即是XUnit家族中的一员，它是一个专门面向C++的测试框架。

用CppUint进行测试时，一般先以某个函数或者某个类作为测试单元，完成测试；在完成这些基本的测试后，再将引用到测试过的这些函数或者类的函数或者类，作为测试单元完成测试；继续下去，从而完成整个模块甚至是整个系统的测试，从而达到更多清除bug的目的。

**一、CppUnit****测试框架**
 CppUnit测试框架的源代码可以到 http://sourceforge.net/projects/cppunit/ 上下载。下载解压后，你将看到如下文件夹：
![](http://hi.csdn.net/attachment/201007/19/0_1279537024EWWs.gif)

主要的文件夹有：
- doc: CppUnit的说明文档。另外，代码的根目录，还有三个说明文档，分别是INSTALL，INSTALL-unix，INSTALL-WIN32.txt。 
- examples: CpppUnit提供的例子，也是对CppUnit自身的测试，通过它可以学习如何使用CppUnit测试框架进行开发。 
- include: CppUnit头文件。 
- src: CppUnit源代码目录。 

**二、基本原理**

CppUint进行测试的基本原理是：对于每一批测试，编写一个继承自TestCase的测试类，其中setUp和tearDown 两个虚函数分别用于进行测试数据的初始化和释放操作；对于每一项测试，分别定义一个void返回类型的无参数方法，检测测试过程，可以用以下几个宏：
|CPPUNIT_ASSERT(condition)|断言condition为真，否则失败|
|----|----|
|CPPUNIT_ASSERT_MESSAGE(message,condition)|当condition为假时失败, 并打印message|
|CPPUNIT_FAIL(message)|当前测试失败, 并打印message|
|CPPUNIT_ASSERT_EQUAL(expected, actual)|确信两者相等，否则失败|
|CPPUNIT_ASSERT_EQUAL_MESSAGE(message, expected, actual)|确信两者相等，否则失败，并同时打印message|
|CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, actual, delta)|当expected和actual之间差大于delta时失败|

其中，“失败”的含义就是程序运行出现异常，并且所有测试过程中止。

在完成一个或者多个测试类的编写后，需要用TestRunner类的实例来运行每个测试类的测试函数进行测试。TestRunner类型共有：TextUi::TestRunner，QtUi::TestRunner，MfcUi::TestRunner三个，都可以用来运行测试，其区别为：
|TextUi::TestRunner|默认将信息输出到控制台，也可以指定输出到std::ostringstream。|
|----|----|
|QtUi::TestRunner|用QT的GUI输出信息|
|MfcUi::TestRunner|用MFC的GUI输出信息|

整个测试过程需要另外附加代码，所以通常情况下，测试用例代码和被测试对象因在不同的项目中。应该在另一个项目（最好在不同的目录）中编写测试类，然后把被测试的对象包含在测试项目中。根据测试工程建立的类型，Consol可以选择TextUi::TestRunner，将结果输出至控制台显示，MFC的GUI可以使用MfcUi::TestRunner，使用GUI来控制并查看测试结果。测试工程与被测试对象所在工程的类型是可以不同的。

在使用VC作为开发工工具时，建议使用TextUi::TestRunner，并更改其Outputter实例，使用附加的TraceOutputter，将信息输出到信息输出栏，双击信息输出栏中一行，即可定位到出错位置。

**三、安装方式**

以VC使用为例，步骤如下：

1.获取原始包，比如cppunit-1.12.1.rar，解压缩到某个目录，比如E:/cppunit-1.12.1

2.将MsDevCallerListCtrl.cpp文件中的

#import"libid:80cc9f66-e7d8-4ddd-85b6-d9e6cd0e93e2"version("7.0") lcid("0") raw_interfaces_onlynamed_guids

改为：（VC2008，version("9.0")；VC2005，version("8.0")）

#import"libid:80cc9f66-e7d8-4ddd-85b6-d9e6cd0e93e2"version("9.0") lcid("0") raw_interfaces_onlynamed_guids

编译工程，编译完成之后会在lib目录下生成cppunit、TestRunner等lib/dll文件。

2.打开VC，菜单“工具”－> “选项”，在“选项”对话框的“项目和解决放方案”页中，包含文件include Files添加“E:/cppunit-1.12.1/include”，库文件Library Files添加“E:/cppunit-1.12.1/lib”。如图：

![](http://hi.csdn.net/attachment/201007/19/0_12795369288870.gif)

![](http://hi.csdn.net/attachment/201007/19/0_12795369431w1S.gif)

3.打开工程E:/cppunit-1.12.1/src/CppUnitLibraries.dsw，在该工程中，cppuint是静态库版本，cppunit_dll是动态库版本，一般只需要完成release版本编译，如果想要研究源代码，可以考虑编译debug版本，用于调试和跟踪代码。

4.如要使用MfcUi::TestRunner，编译dsplugin工程和TestRunner工程。dsplugin是一个VC的插件，TestRunner就是MfcUi::TestRunner的实现。

5、将CppUnitTestApp工程设为Active Project（Win32 Debug），编译后运行，则可以看到CppUnit的基于GUI方式进行单元测试TestRunner的界面。点击“Run”，将会看到如图所示界面：

![](http://hi.csdn.net/attachment/201007/19/0_1279536954dr54.gif)

点击“Browse”,我们还可以选择想要进行的单元测试，如图：

![](http://hi.csdn.net/attachment/201007/19/0_127953701705t0.gif)

**四、使用方法**

CppUnit最小的测试单位是TestCase，多个相关TestCase组成一个TestSuite。要添加测试代码最简单的方法就是利用CppUnit为我们提供的几个宏来进行（当然还有其他的手工加入方法，但均是殊途同归，大家可以查阅CppUnit头文件中的演示代码）。这几个宏是： 

CPPUNIT_TEST_SUITE() 开始创建一个TestSuite 

CPPUNIT_TEST() 添加TestCase 

CPPUNIT_TEST_SUITE_END() 结束创建TestSuite 

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION() 添加一个TestSuite到一个指定的TestFactoryRegistry工厂。（两个参数：第一个是TestSuite；第二个是唯一标识TestSuite名称字符串）

CppUnit::MfcUi::TestRunnerrunner;// MFC的GUI测试控制平台

CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry(TestSuite名称);//得到标识TestSuite名称字符串TestFactoryRegistry

runner.addTest(registry.makeTest());//添加Test

runner.run();//运行测试案例

///////////////////////////////////////////////////////////////////////实例：一个实现两个整数加减乘除运算的类测试

// Arithmetic.h 加减乘除运算类

#pragmaonce

classCArithmetic

{

public:

CArithmetic(void);

~CArithmetic(void);

public:

intAdd(intnNum1, intnNum2);

intSub(intnNum1,intnNum2);

intMul(intnNum1,intnNum2);

doubleDiv(intnNum1,intnNum2);

};

// Arithmetic.cpp

#include"Arithmetic.h"

CArithmetic::CArithmetic(void)

{

}

CArithmetic::~CArithmetic(void)

{

}

intCArithmetic::Add(intnNum1, intnNum2)

{

returnnNum1+nNum2;

}

intCArithmetic::Sub(intnNum1,intnNum2)

{

returnnNum1-nNum2;

}

intCArithmetic::Mul(intnNum1,intnNum2)

{

returnnNum1*nNum2;

}

doubleCArithmetic::Div(intnNum1,intnNum2)

{

returnnNum1/(nNum2*1.0);

}

在Project中加入一个类，取名CArithmeticTestCase，CArithmeticTestCase从CppUnit::TestCase继承，代码如下：

//ArithmeticTestCase.h

#pragmaonce

#include"cppunit/extensions/HelperMacros.h"

classCArithmeticCase :publicCppUnit::TestCase

{

CPPUNIT_TEST_SUITE(CArithmeticCase);

CPPUNIT_TEST(testAdd);

CPPUNIT_TEST(testSub);

CPPUNIT_TEST(testMul);

CPPUNIT_TEST(testDiv);

CPPUNIT_TEST_SUITE_END();

public:

CArithmeticCase(void);

~CArithmeticCase(void);

voidtestAdd();

voidtestSub();

voidtestMul();

voidtestDiv();

virtualvoidsetUp();

virtualvoidtearDown();

};

// ArithmeticTestCase.cpp

#include"StdAfx.h"

#include"ArithmeticTestCase.h"

#include"Plus.h"

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CArithmeticTestCase,"MyArithmeticTest");

CArithmeticTestCase::CArithmeticTestCase(void)

{

}

CArithmeticTestCase::~CArithmeticTestCase(void)

{

}

voidCArithmeticTestCase::testAdd()

{

CArithmeticplus;

intnResult = plus.Add(10, 20);

CPPUNIT_ASSERT_EQUAL(30, nResult);

}

voidCArithmeticTestCase::testSub()

{

CArithmeticplus;

intnResult = plus.Sub(10, 20);

CPPUNIT_ASSERT_EQUAL(-10, nResult);

}

voidCArithmeticTestCase::testMul()

{

CArithmeticplus;

intnResult = plus.Mul(10, 20);

CPPUNIT_ASSERT_EQUAL(200, nResult);

}

voidCArithmeticTestCase::testDiv()

{

CArithmeticplus;

doublefResult = plus.Div(10, 20);

CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, fResult,0.000001);

}

voidCArithmeticTestCase::setUp()

{

//初始化一些共用信息，比如全局变量，资源等。

TRACE("Enter CPlusTestCase::setUp()/n");

}

voidCArithmeticTestCase::tearDown()

{

//析构处理一些信息，比如释放资源

TRACE("Enter CPlusTest::tearDown()/n");

}

测试代码编写

#include<cppunit/extensions/TestFactoryRegistry.h>

#include<cppunit/ui/mfc/TestRunner.h>

#pragmacomment(lib, "cppunitd.lib")

#pragmacomment(lib, "testrunnerud.lib")//可以加载静态库，比动态库DEBUG版本，具体参考cppunit-1.12.1/lib下的文件。

CppUnit::MfcUi::TestRunnerrunner;

CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry("MyArithmeticTest");

runner.addTest(registry.makeTest());

runner.run();

代码：[http://download.csdn.net/source/2556833](http://download.csdn.net/source/2556833)


