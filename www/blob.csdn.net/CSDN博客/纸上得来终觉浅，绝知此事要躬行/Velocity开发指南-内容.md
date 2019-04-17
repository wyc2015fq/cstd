# Velocity开发指南-内容 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年10月10日 14:22:37[boonya](https://me.csdn.net/boonya)阅读数：1020








本文采用谷歌翻译，原文地址：[http://velocity.apache.org/engine/devel/developer-guide.html](http://velocity.apache.org/engine/devel/developer-guide.html)


# 介绍

Velocity是一个基于Java的模板引擎，一个简单而功能强大的开发工具，让您轻松地创建和渲染文档格式和显示数据。在本指南中，我们希望能够给开发使用Velocity的基础知识的概述。

## 使用Velocity构建Web应用程序


Velocity通常用于构建web应用程序。为了在web应用程序使用Velocity你需要一个servlet或基于servlet的框架。上手最简单的方法是在速度工具子项目的VelocityViewServlet。您还可以使用任意数量的第三方框架或使用本文件中描述的技术建立自己的servlet。

我们建议您阅读这篇文章的[Web应用程序的入门](http://velocity.apache.org/engine/devel/webapps.html)有关各种选项的详细信息。

## 下载Velocity

您可以从Apache主速度下载站点下载[Velocity引擎](http://velocity.apache.org/download.cgi#engine)或[Velocity](http://velocity.apache.org/download.cgi#tools)[工具](http://velocity.apache.org/download.cgi#tools)的最新版本的版本。对Velocity本身，源代码包含在二进制下载。

如果您想要下载最新的源代码，你可以通过颠覆（SVN）源代码控制系统这样做。

从源代码[构建](http://velocity.apache.org/engine/devel/build.html)Velocity指令可以生成文档中找到。

## 依赖

对于编译或运行时，Velocity需要1.7或更新版本的Java运行时环境的版本。

Velocity还取决于通用功能的几个包：

- [**Apache Commons Collections**](https://commons.apache.org/proper/commons-collections/) - required for compilation only.
- [**Apache Commons Lang**](http://commons.apache.org/proper/commons-lang/) - required for compilation only.
- [**SLF4J API**](http://www.slf4j.org) - required for compilation and at runtim


在运行时，速度将需要一个SLF4J结合存在于该路径。通常情况下，其中一个：

- [AVSL](http://software.clapper.org/avsl/)
- [SLF4J JDK14 Adapter](http://www.slf4j.org/api/org/slf4j/impl/JDK14LoggerAdapter.html)
- [SLF4J Log4J Adapter](http://www.slf4j.org/api/org/slf4j/impl/Log4jLoggerAdapter.html)
- [SLF4J Simple Logger](http://www.slf4j.org/api/org/slf4j/impl/SimpleLogger.html)
- [SLF4J Android](http://www.slf4j.org/android/)
- [LOGBack](http://logback.qos.ch/)
- [WebApp SLF4J Logger](https://github.com/arkanovicz/webapp-slf4j-logger)




# 资源

这里提供给程序员相当多的资源和示例，我们建议你看一下我们的例子中，文档甚至源代码。一些不错的来源是：



- The user and developer community: join us via the [mail-lists](http://velocity.apache.org/contact.html). Mail list archives are available from that page, too.
- The [Velocity Wiki](http://wiki.apache.org/velocity/) contains articles, sample code, and other community-written content.
- [Frequently Asked Questions (FAQ)](http://wiki.apache.org/velocity/VelocityFAQ) : please visit this page to read the latest FAQ and to contribute your own answers.
- source code: `src/java/...`: all the source code to the Velocity project
- application example 1: `examples/app_example1`: a simple example showing how to use Velocity in an application program.
- application example 2: `examples/app_example2`: a simple example showing how to use Velocity in an application program using the Velocity application utility class.
- XML example: `examples/xmlapp_example`: a simple example showing how to use JDOM to read and access XML document data from within a Velocity template. It also includes a demonstration of a recursive Velocimacro that walks the document tree.
- event example: `examples/event_example`: An example that demonstrates the use of the event handling API.
- Anakia application: `examples/anakia`: example application showing how to use Velocity for creating stylesheet renderings of xml data
- documentation: `docs`: all the generated documentation for the Velocity project in html
- API documentation: `docs/api`: the generated Javadoc documentation for the Velocity project
- templates: `test/templates`: a large collection of template examples in our testbed directory, these are a great source of useage examples of VTL, the Velocity Template Language
- context example: `examples/context_example`: two examples showing how the Velocity context can be extended. For advanced users.
以上所有目录的引用都是相对于分配根目录。



# Velocity如何工作

## 基本模式


当一个应用程序或一个servlet使用Velocity（或任何地方，实际上），你一般会做到以下几点：

**    1、初始化Velocity。这适用于对速度两者的使用模式，辛格尔顿以及作为“独立的运行时实例”（看更多的关于这个下文），你只这样做一次。2、创建上下文对象（在这就是后来更多）。3、您的数据对象添加到上下文。    4、 选择一个模板。    5、“合并”模板和数据产生输出中。**

在代码中，通过org.apache.velocity.app.Velocity类使用Singleton模式，这看起来如下：



```java
import java.io.StringWriter;
import org.apache.velocity.VelocityContext;
import org.apache.velocity.Template;
import org.apache.velocity.app.Velocity;
import org.apache.velocity.exception.ResourceNotFoundException;
import org.apache.velocity.exception.ParseErrorException;
import org.apache.velocity.exception.MethodInvocationException;

Velocity.init();

VelocityContext context = new VelocityContext();

context.put( "name", new String("Velocity") );

Template template = null;

try
{
  template = Velocity.getTemplate("mytemplate.vm");
}
catch( ResourceNotFoundException rnfe )
{
  // couldn't find the template
}
catch( ParseErrorException pee )
{
  // syntax error: problem parsing the template
}
catch( MethodInvocationException mie )
{
  // something invoked in the template
  // threw an exception
}
catch( Exception e )
{}

StringWriter sw = new StringWriter();

template.merge( context, sw );
```
这是基本的模式。这是非常简单的，不是吗？这通常是当你使用Velocity呈现模板会发生什么。你可能不会编写代码完全一样
 - 我们提供了一些工具来帮助使其更容易。但是，不管如何使用Velocity上面的顺序发生了什么任何明示或幕后。


# 单例还是非单例..

开发人员可以使用Velocity引擎，单例模式和单独的实例模型两个选项。相同的核心速度编码用于这两种方法，其中设置，使速度更轻松地集成到Java应用程序。


## 单例模型

这是传统的模式，其中只有一个在JVM（或Web应用程序，这取决于）Velocity引擎的实例是受所有共享。这是非常方便的，因为它允许局部配置和资源的共享。例如，这是在Servlet2.2+兼容的Web应用程序的每个Web应用程序都可以有自己速度的情况下使用非常合适的模型，允许该Web应用程序的servlet来共享资源，如模板，记录仪等。单是通过org.apache.velocity.app.Velocity类accessable，并和使用示例：




```java
import org.apache.velocity.app.Velocity;
import org.apache.velocity.Template;

...

/*
 *  Configure the engine
 */

Velocity.setProperty(
    Velocity.RUNTIME_LOG_NAME, "mylog");

/*
 *  now initialize the engine
 */

Velocity.init();

...

Template t = Velocity.getTemplate("foo.vm");
```




## 单独的实例


新的1.2版本中，单独的实例，可以创建，配置和你在同一个JVM希望使用Velocity的多个实例（或Web应用程序）。当你想支持不同的配置，如模板目录，这是有用的，记录器等在同一应用程序。要使用单独的实例，使用org.apache.velocity.app.VelocityEngine类。一个例子，它平行于上述单的例子，是这样的：




```java
import org.apache.velocity.app.VelocityEngine;
import org.apache.velocity.Template;

...

/*
 *  create a new instance of the engine
 */

VelocityEngine ve = new VelocityEngine();

/*
 *  configure the engine.  In this case, we are using
 *  a specific logger name
 */

ve.setProperty(
    VelocityEngine.RUNTIME_LOG_NAME, "mylog");

/*
 *  initialize the engine
 */

ve.init();

...

Template t = ve.getTemplate("foo.vm");
```

正如你所看到的，这是非常简单明了。除了一些简单的语法变化，使用Velocity作为一个单身或作为单独的实例，无需更改应用程序或模板的高级结构。

作为一个程序员，类你，如果使用单模式，或者如果org.apache.velocity.app.VelocityEngine使用非单模式应使用交互与速度内部是org.apache.velocity.app.Velocity类（“单独的实例'）。

任何时候都不能申请使用内部运行时，RuntimeConstants，RuntimeSingleton或RuntimeInstance类的org.apache.velocity.runtime包，因为这些仅供内部使用，可能会随时间而改变。正如上面提到的，你应该使用类位于org.apache.velocity.app包，是速度和VelocityEngine类。如果有任何缺失或从这些课程中需要的话，不要犹豫，提出修改意见
 - 这些类是用于应用程序开发人员。


# Context上下文

## 基础

“上下文”的概念是Velocity的核心，并为左右移动的数据的容器的系统的部件之间的常用技术。这个想法是，所述上下文是在Java层之间的数据的“载体”（或者编程）和模板层（或设计者）。你作为程序员将聚集各类对象，无论您的应用需要，并将其放置在上下文。设计者，这些对象，以及它们的方法和属性，将通过被称为引用模板元素成为accessable。一般来说，你会与设计师合作，以确定应用程序的数据需求。从某种意义上说，这将成为一个“API”为你生产为设计师在模板中访问的数据集。因此，在发展过程中的这个阶段是值得投入一定的时间和仔细的分析。

虽然Velocity可以让你创建自己的上下文类，支持特殊需求和技术（如直接访问LDAP服务器，例如上下文），提供了一个名为VelocityContext良好的基础实现类为你的发行版的一部分。

VelocityContext适用于所有通用的需求，我们强烈建议您使用它。只有在特殊和先进的情况下，你会需要扩展或创建自己的上下文实现。

使用VelocityContext是用普通的Java Hashtable类一样简单。而接口包含其他有用的方法，你将使用两种主要方法是：



```java
public Object put(String key, Object value);
public Object get(String key);
```

请注意它像一个Hashtable，值必须从衍生的java.lang.Object，并且不能为空。基本类型，如整数或浮点数必须包装在适当的包装类。

这的确是所有有基本上下文操作。欲了解更多信息，请参阅包含在发布的API文档。


## #foreach()支持为迭代对象

作为一个程序员，你必须给对象预留巨大的自由度给上下文。但与大多数自由，这其中必有一点点的责任，所以明白Velocity支持什么，以及可能出现的任何问题。Velocity支持serveral的类型适合在VTL
 #foreach（）指令使用集合类型。+ `Object []`定期对象数组，不需要过多的在这里说。Velocity将包裹在内部数组中，提供了一个Iterator接口的类，但不应该关心你是作为程序员或模板作者。更令人感兴趣的是一个事实，即速度不会允许模板作者对待数组作为固定长度的列表。这意味着他们可能会调用size()、`isEmpty()``和get(int)`方法一样，两个数组和标准的java.util.List实例在空属性，而不涉及对自身的差异。+
 java.util.Collection的速度将使用iterator（）方法来获得一个Iterator在循环使用，因此，如果您是在对象上实现Collection接口，请确保iterator（）返回工作迭代器。接口+的java.util.Map这里，速度取决于值（）方法得到一个Collection接口，其上迭代器（）被调用来检索循环迭代器。+小心使用java.util.Iterator的用途：这是目前唯一支持暂时
 - 关注的问题是Iterator的“非resettablity”。如果是'裸'迭代放入上下文，并且在一个以上的#foreach（），随后#foreach（）使用的第一后块将失败，因为迭代不重置。+小心使用的java.util.Enumeration
 USE：像java.util.Iterator的，这是目前只支持临时 - 关心的问题是枚举的“非resettablity”。如果是'裸“枚举放入上下文，并且在一个以上的#foreach（），随后#foreach（）使用的第一后块将失败，因为枚举不重置。+任何公共类从不返回null公众的Iterator
 iterator（）方法。作为最后的手段，速度会寻找一个iterator（）方法。这提供了极大的灵活性和java.util.Iterable接口自动支持。


在迭代器(Iterator)和枚举(Enumeration)的情况下，建议它们被放置在上下文仅当不能避免它，应该让速度找到相应的可重复使用的迭代接口时是充分的，可能的。

这里有（通过JDBC大型数据集，例如）使用java.util.Iterator的接口直接充分的理由，但如果能够避免它，它可能会更好使用别的东西。通过“直接”，我们是认真做类似：




```java
Vector v = new Vector();
v.addElement("Hello");
v.addElement("There");

context.put("words", v.iterator() );
```

其中迭代器本身被置于上下文中。相反，如果你简单实现：





```java
context.put("words", v );
```

那么所有将被罚款：Velocity会弄清楚，矢量实现Collection（通过列表），因此会发现iterator（）方法，并使用每个它需要时间来获得一个“fresh”迭代器供其使用。只需一个简单的迭代（高于第一片断...），一旦速度已经以#foreach（）中使用它，速度没有得到一个新的要使用的下一个#foreach（）它是在使用的方式。结果是没有从任何后续#foreach（）输出块使用该引用。

上面这并不意味着给了遍历集合的速度印象的东西，需要很大的良苦用心。相反，情况正好相反，一般。当你将一个Iterator到上下文只是要小心。


## 支持“静态类”

并非所有的类都是实例化的。像java.lang.Math中的类不提供任何公共构造函数，但可能包含有用的静态方法。为了从模板访问这些静态方法，可以在类本身只需添加到上下文：



```java
context.put("Math", Math.class);
```

这将允许您调用java.lang.Math中的任何公共静态方法在模板中的$Math参考。


## Context上下文链

Velocity的上下文设计的一个创新功能是上下文链的概念。也有时被称为Context包装，这种先进的功能，可以将独立的上下文在一起的方式，使得它看起来像一个“连续的”上下文模板连接。

这是最好的一个例子来说明：



```java
VelocityContext context1 = new VelocityContext();

context1.put("name","Velocity");
context1.put("project", "Jakarta");
context1.put("duplicate", "I am in context1");

VelocityContext context2 = new VelocityContext( context1 );

context2.put("lang", "Java" );
context2.put("duplicate", "I am in context2");

template.merge( context2, writer );
```

在上面的代码中，我们建立了context2，使其链CONTEXT1。这意味着，在模板中，可以访问任何放入任两个VelocityContext物体的物品，只要没有用于添加对象的键的重复。如果是这样的话，因为它是上述用于键'重复'中，存储在最近的上下文链中的对象将可用。在以上这个例子中，对象返回将是字符串“我在context2”。

需要注意的是这种重复或“覆盖”，一个上下文项的不以任何方式损害或改变覆盖对象。所以在上面的例子中，字符串“我在CONTEXT1”，是活得很好，通过context1.get仍然accessable（“重复”）。但在上述的例子中，模板中的参考'$重复'的值将是“我在context2'和模板具有到包串”我在CONTEXT1'无访问。

还需要注意的是你有，当你依靠模板将信息添加到一个背景下，你以后会渲染后检查要小心。上下文通过#SET在模板中的变化（）语句只会影响外部环境。因此，请确保您不要丢弃外层的背景下，预计从模板中的数据已经被放置到内层。

此功能有许多用途，最常见的迄今正在提供分层的数据访问和工具集。

正如前面提到的，速度方面的机制还扩展，但超出了本指南的当前范围。如果你有兴趣，请参阅类包装org.apache.velocity.context看到所提供的上下文是如何放在一起。进一步的，也有在实施例/
 context_example目录其显示替代的实现，包括[一个愚蠢]一个使用一个数据库作为后备存储的几个例子中的分布。

请注意，这些例子是不支持的，而且有仅用于演示/教育目的。



## 对象创建的模板

这里有其中的Java代码必须处理在模板运行时创建的对象的两种常见情况：




当模板作者所说放入由Java代码的上下文对象的方法。

```
#set($myarr = ["a","b","c"] )
$foo.bar( $myarr )
```

当模板添加对象的上下文中，合并过程完成后，Java代码可以访问这些对象。

```
#set($myarr = ["a","b","c"] )
#set( $foo = 1 )
#set( $bar = "bar")
```
**这些情况处理是非常简单的，这里有几个事项需要知道：**


     1、放置在上下文或传递给方法时VTLRangeOperator[1..10]和ObjectArray[“一”，“B”]是java.util.ArrayList中的对象。因此，你的方法，旨在接受模板创建的阵列中，应考虑到这一点写的。

2、VTL地图引用意料之中存储的java.util.Map。

3、十进制数将是双打，或在上下文BigDecimals的，整数会为Integer，Long，或BigIntegers和字符串会，当然，串。

4、从2.0版本开始，Velocity将转换所有主要的Java标准类型（布尔，数字和字符串）之间的方法参数。如果你想恢复到速度1.x的行为，在这里提供的转化仅限于隐Java的转换，可以设置属性runtime.conversion.handler无法比拟的。



## 其他上下文问题

一个由VelocityContext（或从AbstractContext派生的任何上下文）提供的功能是节点特定自省缓存。通常情况下，你作为一个开发人员不需要使用VelocityContext作为上下文时担心这一点。但是，目前，你必须了解此功能的一个已知的使用模式。

**该VelocityContext会堆积intropection有关语法节点的信息在一个模板，因为它访问这些节点。所以，在下面的情况：**

    1、您正在使用同一个VelocityContext对象遍历相同的模板。
2、模板缓存关闭。
3、请您从getTemplate（）每次迭代模板。


这可能是你的VelocityContext会出现“泄漏”内存（它真的只是收集更多的内省信息。）什么情况是，它积累的模板节点内省信息它所访问的每个模板，并作为模板缓存是关闭的，它似乎它每次访问一个新的模板VelocityContext。因此，它收集更多的内省信息和增长。强烈建议你做一个或多个以下：

    1、为每个游览新VelocityContext上下贯通的模板渲染过程。这将防止内省缓存数据的积累。对于那些你想，因为它与数据或对象填充重用VelocityContext的情况下，你可以简单地包裹在另一填充VelocityContext，而“外”一会积累内省的信息，您将只需放弃。防爆。VelocityContext
 useThis =new VelocityContext（populatedVC）;这工作，因为外部环境将存储反省缓存数据，并从内部环境的任何请求的数据（因为它是空的。）不过要小心
 - 如果您的模板位置数据转换成的背景和它的预期，将在被使用随后的迭代，你需要做其他的修补程序之一，因为任何模板#SET（）语句将被存储在最外面的环境。在上下文中查看[链接](http://velocity.apache.org/engine/devel/developer-guide.html#context-chaining)以获取更多信息的讨论。

2、打开模板缓存。这将防止重新分析在每次迭代，产生的所述VelocityContext能够不仅避免加重内省缓存信息，但能够使用它产生的性能改进的模板。


    3、重复使用的循环迭代的持续时间的模板对象。然后，你将不会强迫速度，如果缓存被关闭，重读又一遍地重新分析相同的模板，所以VelocityContext不会收集每一次新的内省信息。




......待续！




