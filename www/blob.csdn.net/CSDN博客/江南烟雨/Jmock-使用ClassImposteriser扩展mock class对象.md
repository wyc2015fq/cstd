# Jmock-使用ClassImposteriser扩展mock class对象 - 江南烟雨 - CSDN博客
2014年09月24日 21:41:54[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：4026
由于Jmock使用了java的标准反射机制，jmock框架默认配置只能mock
 interface对象，而不能mock class对象。但是使用CGLIB 2.1以及Objenesis库，采用ClassImposteriser扩展不仅能够mock
 interface对象，而且能够mock class对象。
**ClassImposteriser扩展使用方法介绍及实例说明**
ClassImposteriser mock实例对象的时候，不会调用被mock的class的构造函数，因此拥有具有多个参数或者需要调用重载方法的构造函数的类可以被安全的mock，然而，它不能mockfinal
 class或有final方法的class。如果要mock final class，需要首先使用JDave库来在类被加载进JVM之前去掉class的final特性。
下面看看如何具体使用ClassImposteriser来mock class。
- 将以下三个jar包添加到class path
- jmock-legacy-2.6.1.jar
- cglib-nodep-2.1_3.jar
- objenesis-1.0.jar
- 在代码中，创建mock上下文的时候，设置ClassImposteriser。
```java
import org.jmock.Mockery;
import org.jmock.Expectations;
import org.jmock.integration.junit4.JUnit4Mockery;
import org.jmock.lib.legacy.ClassImposteriser;
 
@RunWith(JMock.class)
public class ConcreteClassTest {
    private Mockery context = new JUnit4Mockery() {{
       setImposteriser(ClassImposteriser.INSTANCE);
    }};
   
    ...
}
```
- 然后就能够按照通常的方法来mock class对象了。
```java
private ExterfaceImpl    target;
    private final static Mockery         mockContext                  = new JUnit4Mockery() {
                                                                         {
                                                                             setImposteriser(ClassImposteriser.INSTANCE);
                                                                         }
                                                                     };
 
    private Helper helper = null;
 
    //    static {
    //       mockery.setImposteriser(ClassImposteriser.INSTANCE);
    //    }
 
    @Before
    public void setUp() throwsException {
        target = new ExterfaceImpl();
        helper =mockContext.mock(Helper.class);
 
        target.setHelper(helper);
 
    }
```
产生的相关问题
但是在使用ClassImposteriser来mock class的时候，发现产生了和mock interface不同的新问题。就是，当在一个java文件中写多个单元测试用例（标@Test注解的函数），会报错，而且是第一个测试用例能够通过，后面的都无法通过。
```
@Test
    public voidtestGetExterfaceInvokeByPartnerId() {
        return;
        Object result =target.getById(null, null, null);
               Assert.assertNull(result);
 
    }
 
    @Test
    public void testGetExterfaceInvokeByPartnerId1() {
        return;
        Object result =target.getById(null, null, null);
               Assert.assertNull(result);
 
    }
```
具体错误信息如下：
java.lang.IllegalArgumentException:a mock with name extHelper already exists
atorg.jmock.Mockery.mock(Mockery.java:128)
atcom.alipay.mapitool.facade.exterface.impl.ExterfaceInvokeFacadeImplUnitTest.setUp(ExterfaceInvokeFacadeImplUnitTest.java:54)
atsun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
atsun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:39)
atsun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:25)
atjava.lang.reflect.Method.invoke(Method.java:597)
atorg.junit.internal.runners.MethodRoadie.runBefores(MethodRoadie.java:122)
atorg.junit.internal.runners.MethodRoadie.runBeforesThenTestThenAfters(MethodRoadie.java:86)
atorg.junit.internal.runners.MethodRoadie.runTest(MethodRoadie.java:77)
atorg.junit.internal.runners.MethodRoadie.run(MethodRoadie.java:42)
atorg.junit.internal.runners.JUnit4ClassRunner.invokeTestMethod(JUnit4ClassRunner.java:88)
atorg.junit.internal.runners.JUnit4ClassRunner.runMethods(JUnit4ClassRunner.java:51)
atorg.junit.internal.runners.JUnit4ClassRunner$1.run(JUnit4ClassRunner.java:44)
atorg.junit.internal.runners.ClassRoadie.runUnprotected(ClassRoadie.java:27)
atorg.junit.internal.runners.ClassRoadie.runProtected(ClassRoadie.java:37)
atorg.junit.internal.runners.JUnit4ClassRunner.run(JUnit4ClassRunner.java:42)
atorg.eclipse.jdt.internal.junit4.runner.JUnit4TestReference.run(JUnit4TestReference.java:50)
atorg.eclipse.jdt.internal.junit.runner.TestExecution.run(TestExecution.java:38)
atorg.eclipse.jdt.internal.junit.runner.RemoteTestRunner.runTests(RemoteTestRunner.java:467)
atorg.eclipse.jdt.internal.junit.runner.RemoteTestRunner.runTests(RemoteTestRunner.java:683)
atorg.eclipse.jdt.internal.junit.runner.RemoteTestRunner.run(RemoteTestRunner.java:390)
atorg.eclipse.jdt.internal.junit.runner.RemoteTestRunner.main(RemoteTestRunner.java:197)
错误信息显示，已经有一个该名字的mock对象存在了。这是因为，每运行一个测试用例，都会调用setup方法一次。我们需要每次给mock对象取不同的名字才行。这样做就可以了。
**参考资料：**
[http://jmock.org/mocking-classes.html](http://jmock.org/mocking-classes.html)
[http://osdir.com/ml/java.jmock.user/2008-11/msg00049.html](http://osdir.com/ml/java.jmock.user/2008-11/msg00049.html)
