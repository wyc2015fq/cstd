# Mockito一个用于Java开发的伟大的模拟框架 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年04月20日 14:45:41[boonya](https://me.csdn.net/boonya)阅读数：5109









## 介绍

这个艺术将展示mock框架的一些基本概念，为什么我们应该使用它，并演绎一个在Java中应用Mockito的简单方法。


## **Mock**的概念

在软件开发世界之外，术语“`mock`”意味着模仿或模拟。因此，“`mock`”可以被认为是一种独立的，冒名顶生的，或者最常被称之为软件开发，一种`fake`。

假货经常被用作待测类的依赖关系。
|*条款和定义*|
|----|
|**依赖关系**- 依赖关系是指应用程序中的一个类别依赖于另一个类来执行其预期功能。依赖关系通常存储在依赖类中的实例变量中。|
||
|**被试课程**- 在进行单元测试时，术语“单元”通常是指一个单独的类，特别是测试写入的类。因此，被测试类是正在测试的应用程序类。|
为什么模拟？
当我们学习编程时，我们的对象通常是自包含的。任何你好的世界都没有依赖于外部类（System.out），也不会在学习语言的过程中写入许多其他类。然而，在现实世界中，软件具有依赖性。我们有依赖于数据访问对象（DAO）的服务和服务的操作类，列表继续。

![](http://3.bp.blogspot.com/_SAqQlE5FaX8/SdIaWQVdb5I/AAAAAAAAABI/o7ionJZdm2M/s400/what_is_mock_2.gif)

单元测试的想法是我们要测试我们的代码而不测试依赖。此测试允许您验证正在测试的代码是否工作，而不管其是否依赖。理论是，如果我写的代码按照设计工作，我的依赖关系按照设计工作，那么他们应该按照设计一起工作。下面的代码将是一个例子：

HideCopy Code
import java.util.ArrayList;
public class Counter {
     public Counter() {
     }
     public int count(ArrayList items) {
          int results = 0;
          for(Object curItem : items) {
               results ++;
          }
          return results;
     }
} 

我知道上面的例子就像你所得到的一样简单，但是它说明了一点。如果您想测试方法计数，您将在测试中写入，以解决计数方法的工作原理。*您不是试图测试ArrayList的工作原理，因为您假设它已经过测试并按照设计工作*。您唯一的目标是测试您对ArrayList的使用。

模拟对象背后的概念是我们想要创建一个取代真实对象的对象。这个模拟对象将期望使用某些参数调用某种方法，而在这种情况下，*它将返回一个预期的结果。*

## 什么是关键的Mock概念？

当谈到mock时，你只需要3件事情就需要担心;*扼杀，设定期望和验证。*某些单元测试场景不涉及任何这些情况，其他单独测试场景只涉及桩杆，而其他则涉及设置预期和验证。

### 存根

Stubbing是告诉你的假的过程，当它与它进行交互时如何表现。您通常可以存放公共属性（带有getter和/或setter的公共属性）和公共函数。

说到stubbing功能，你通常有很多选择。您可能希望返回特定值，抛出错误或调度事件。此外，您可能希望指示函数的行为方式取决于调用方式（即通过匹配传递给函数的参数的类型或值）。

如果这听起来像是很多的工作，它可以是，但它一般不是。许多mock框架的一个很大的特点是你不需要stub void函数。您也不必在执行测试过程中存储任何未被调用的函数或未查询的属性。

### 设定期望

假冒的主要特征之一就是能够告诉假冒测试运行时的期望。例如，您可能希望调用一个特定的函数3次。你可能期望它永远不会被调用。您可能希望它至少被调用两次，但不要超过5次。您可能希望使用特定类型的参数或特定值或上述任何组合来调用它。可能性是无止境的。

设定期望是告诉你假的你会发生什么的过程。记住，由于它是假的，实际上没有发生。但是，你被测试的班级并不聪明。从它的角度来看，它调用了这个函数，并期望它做了它应该做的任何事情。

对于什么是值得的，大多数嘲笑框架可以让您创建接口或公共类的模拟。您不限于仅仅模拟接口。

### 验证

设定期望和验证是并存的。在调用被测试类的函数之前，设置期望值。验证完成。所以，首先你设定了期望，然后验证你的期望是否得到满足。

从单元测试的角度来看，如果您的期望未得到满足，则单元测试失败。例如，如果您设置了ILoginService.login函数应该使用特定的用户名和密码一次调用的期望，但是在执行测试期间它并没有被调用，则该伪造不会被验证，并且测试失败。

## **Mock**的好处是什么？

![](http://www.soapui.org/images/stories/Mock/concept/tick_eviware.png)**您可以提前创建测试;****TDD**

这一个是更强大的好处之一。如果您创建一个模拟器，您可以在服务创建之前编写服务测试，使您能够在开发过程中将测试添加到自动化环境中。换句话说，Service Mocking可以让您使用测试驱动开发。

![](http://www.soapui.org/images/stories/Mock/concept/tick_eviware.png)**团队可以并行工作**

这与上述类似为不存在的代码创建测试。但是以前的一点是针对开发人员编写测试，这是针对测试团队的。当您没有任何测试时，团队如何开始创建测试？模拟它，并对模拟器进行测试！这意味着当服务准备好进行测试时，QA团队实际上可以准备一整套测试;我们没有停机的时候，一个队伍等待另一个完成。这使得嘲弄的财务论据特别强。

![](http://www.soapui.org/images/stories/Mock/concept/tick_eviware.png)**您可以创建概念或演示的证明。**

由于Mocks可以（成本最高）能够实现成本效益，因此可以使用Mocks来创建概念证明，线框架，或作为您正在考虑构建的应用程序的演示。这是非常强大的，为决定是否继续开发项目做出决定，但最重要的是为实际设计决策提供了良好的基础。

![](http://www.soapui.org/images/stories/Mock/concept/tick_eviware.png)**您可以编写测试资源不可访问**

这是不属于实际利益类别的好处之一，而是作为救生员。曾经想要测试或使用服务，只是被告知该服务是在防火墙后面，那个防火墙无法为您打开，或者您被授权使用该防火墙？当你这样做时，一个MockService放置在可访问的地方，包括在你本地的电脑上，是一个救命的人。

![](http://www.soapui.org/images/stories/Mock/concept/tick_eviware.png)**模拟器可以交付给客户**

有些情况下，您无法允许访问外部来源的测试系统（如合作伙伴或客户）的原因。这些原因可以是访问安全性，信息敏感性，或只是测试环境可能无法24/7访问的事实。在这些情况下您如何为您的合作伙伴或客户提供测试系统以开始开发或测试？一个简单的解决方案是从您的网络或客户自己的网络提供一个模拟。soapUI模拟是非常容易部署的，它可以在soapUI中运行，也可以作为.WAR文件导出，并放置在您选择的Java服务器中。

![](http://www.soapui.org/images/stories/Mock/concept/tick_eviware.png)**您可以隔离系统**

有时您想测试系统的一部分，而不会影响其他系统部件。这是因为其他系统会对测试数据增加噪音，使得从收集的数据中得出更好的结论更加困难。使用mocks，您可以删除所有的系统，嘲笑所有系统，除了您在测试中需要确定的一个系统。当进行嘲讽隔离时，这些嘲弄可以做得非常简单但可靠，快速，可预测。这给您一个测试环境，您已经删除了所有随机行为，具有可重复的模式，并可以很好地监控特定系统。

## Mockito框架

Mockito是根据MIT许可证发布的Java开源测试框架。
Mockito通过允许开发人员预先确定被测系统（SUT）的行为来区别于其他嘲笑框架。[4]对模拟对象的批评之一是测试代码与被测系统的紧密耦合。[5]由于Mockito试图通过消除预期的规范来消除预期运行验证模式[6]，所以耦合被减小或最小化。这个区别功能的结果是更简单的测试代码，应该更容易阅读和修改。

### 您可以验证互动：

HideCopy Code
// mock creation
List mockedList = mock(List.class);
// using mock object
mockedList.add("one");
mockedList.clear();
// selective and explicit vertification
verify(mockedList).add("one");
verify(mockedList).clear();   

### 或存根方法调用

HideCopy Code
// you can mock concrete class, not only interfaces
LinkedList mockedList = mock(LinkedList.class);
// stubbing - before execution
when(mockedList.get(0)).thenReturn("first");
// following prints "first"
System.out.println(mockedList.get(0));
// following prints "null" because get(999) was not stubbed.
System.out.println(mockedList.get(999)); 

## 使用Mockito的简单示例Java代码

![](https://lh6.googleusercontent.com/-u6x6JxA3mkE/UNvkYzhDQuI/AAAAAAAAAKc/-nqgpeuPwII/s653/p1.png)
没有Mock框架

![](https://lh3.googleusercontent.com/-zki3yUOzh24/UNvkS6GUABI/AAAAAAAAAKU/cPxOvC5fLtU/s660/P2.png)
用Mockito框架

### 步骤1：在Eclipse中创建一个Maven项目

定义`pom.xml`如下：

HideShrink![](https://www.codeproject.com/images/arrow-up-16.png)Copy Code
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0" 

         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"

         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <groupId>vn.com.phatbeo.ut.mockito.demo</groupId>
  <artifactId>demoMockito</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <packaging>jar</packaging>
  <name>demoMockito</name>
  <url>http://maven.apache.org</url>
  <properties>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
  </properties>
  <build>
    <sourceDirectory>src</sourceDirectory>
    <testSourceDirectory>test</testSourceDirectory>
    <plugins>
      <plugin>
        <groupId>org.apache.maven.plugins</groupId>
        <artifactId>maven-compiler-plugin</artifactId>
        <version>2.3.1</version>
        <configuration>
          <source>1.6</source>
          <target>1.6</target>
        </configuration>
      </plugin>
      </plugins>
  </build>
  
  <dependencies>
    <dependency>
      <groupId>junit</groupId>
      <artifactId>junit</artifactId>
      <version>4.8.1</version>
      <scope>test</scope>
    </dependency>
    <dependency>
      <groupId>org.mockito</groupId>
      <artifactId>mockito-all</artifactId>
      <version>1.8.5</version>
      <scope>test</scope>
    </dependency>
  </dependencies>
  
</project> 


### 步骤2：添加java源代码

类`Person.java`

``

HideCopy Code
 package vn.com.enclave.phatbeo.ut.mockito.demo;
/** 
 * @author Phat (Phillip) H. VU <vuhongphat@hotmail.com>
 *  
 */ 
public class Person
{
    private final Integer personID;
    private final String personName;
    public Person( Integer personID, String personName )
    {
        this.personID = personID;
        this.personName = personName;
    }
    public Integer getPersonID()
    {
        return personID;
    }
    public String getPersonName()
    {
        return personName;
    }
} 

接口`PersonDAO.java`

HideCopy Code
 package vn.com.enclave.phatbeo.ut.mockito.demo;
/**
 * @author Phat (Phillip) H. VU <vuhongphat@hotmail.com>
 * 
 */
public interface PersonDao
{
    public Person fetchPerson( Integer personID );
    public void update( Person person );
} 

类`PersonService.java`

HideShrink![](https://www.codeproject.com/images/arrow-up-16.png)Copy Code
package vn.com.enclave.phatbeo.ut.mockito.demo;
/**
 * @author Phat (Phillip) H. VU <vuhongphat@hotmail.com>
 * 
 */
public class PersonService
{
    private final PersonDao personDao;
    public PersonService( PersonDao personDao )
    {
        this.personDao = personDao;
    }
    public boolean update( Integer personId, String name )
    {
        Person person = personDao.fetchPerson( personId );
        if( person != null )
        {
            Person updatedPerson = new Person( person.getPersonID(), name );
            personDao.update( updatedPerson );
            return true;
        }
        else
        {
            return false;
        }
    }
} 

### 步骤3：添加单元测试类。

然后，跳转到`PersonService.java`类的单元测试用例

假设类`PersionServiceTest.java`如下所示：

HideShrink![](https://www.codeproject.com/images/arrow-up-16.png)Copy Code
 package vn.com.enclave.phatbeo.ut.mockito.demo.test;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.verifyZeroInteractions;
import static org.mockito.Mockito.when;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
/**
 * @author Phat (Phillip) H. VU <vuhongphat@hotmail.com>
 * 
 */
public class PersonServiceTest
{
    @Mock
    private PersonDao personDAO;
    private PersonService personService;
    @Before
    public void setUp()
        throws Exception
    {
        MockitoAnnotations.initMocks( this );
        personService = new PersonService( personDAO );
    }
    @Test
    public void shouldUpdatePersonName()
    {
        Person person = new Person( 1, "Phillip" );
        when( personDAO.fetchPerson( 1 ) ).thenReturn( person );
        boolean updated = personService.update( 1, "David" );
        assertTrue( updated );
        verify( personDAO ).fetchPerson( 1 );
        ArgumentCaptor<Person> personCaptor = ArgumentCaptor.forClass( Person.class );
        verify( personDAO ).update( personCaptor.capture() );
        Person updatedPerson = personCaptor.getValue();
        assertEquals( "David", updatedPerson.getPersonName() );
        // asserts that during the test, there are no other calls to the mock object.
        verifyNoMoreInteractions( personDAO );
    }
    @Test
    public void shouldNotUpdateIfPersonNotFound()
    {
        when( personDAO.fetchPerson( 1 ) ).thenReturn( null );
        boolean updated = personService.update( 1, "David" );
        assertFalse( updated );
        verify( personDAO ).fetchPerson( 1 );
        verifyZeroInteractions( personDAO );
        verifyNoMoreInteractions( personDAO );
    }
} 

## 兴趣点

+什么是Mock框架在概述。

+为什么我们在Java开发中使用Mockito进行测试。

## 参考文献

[http://java.dzone.com/articles/the-concept-mocking](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=http://java.dzone.com/articles/the-concept-mocking&usg=ALkJrhiB5w1znefvGxJt88EqnnGK1Ff6bA)

[http://en.wikipedia.org/wiki/Mockito](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=http://en.wikipedia.org/wiki/Mockito&usg=ALkJrhgeSCZuKtWLGmvZYKhN5Rj6fT4TrQ)

[http://code.google.com/p/mockito/](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=http://code.google.com/p/mockito/&usg=ALkJrhiXH-xo1BD9pKEm7jOm91y9EcsjnQ)

## 历史

2012年12月26日首发


## 执照


本文以及任何相关的源代码和文件均已获得[“代码项目开放许可证（CPOL）”许可.](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=http://www.codeproject.com/info/cpol10.aspx&usg=ALkJrhhWgQ1nOETxHDG6L5xi8d6gVwkTSQ)




