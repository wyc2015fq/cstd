# SPI之从ServiceLoader联想到DriverManager的使用总结 - 零度的博客专栏 - CSDN博客
2016年11月22日 14:59:26[零度anngle](https://me.csdn.net/zmx729618)阅读数：601
# 前言
    这篇文章不是专门讲ServiceLoader的用法，这篇文章也不仅是讲DriverManager的用法。他们两个一个是java.util包里面的类，另外一个在jdbc里面应用的很多。从表面上看起来他们之间似乎没有多少的联系。实际上DriverManager对ServiceLoader的使用可以达到一种巧妙的效果。在这里我想探讨一下DriverManager使用到的一种设计思路以及对我们后续解决类似问题的指导。
# ServiceLoader
    ServiceLoader是jdk6里面引进的一个特性。在第一次碰到的时候还是有点不太理解。从官方的文档来说，它主要是用来装载一系列的service provider。而且ServiceLoader可以通过service provider的配置文件来装载指定的service provider。
    相信看完前面的这一段描述，依然还是会一头雾水。那么我们就结合一个具体的示例来讲讲吧。假定我们要定义一个接口MessageService，它的定义如下：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52998365#)[copy](http://blog.csdn.net/z69183787/article/details/52998365#)[print](http://blog.csdn.net/z69183787/article/details/52998365#)[?](http://blog.csdn.net/z69183787/article/details/52998365#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1961245/fork)
- package com.test.service;  
- 
- publicinterface MessageService {  
-     String getMessage();  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.test.service;
public interface MessageService {
    String getMessage();
}
```
    这是一个再简单不过的接口了。我们这个接口本质上定义了一个行为规范，后续要遵守这个规范的类都需要满足它，也就是说要实现这个接口。针对我们不同的需要，我们可能有不同的实现。比如说我们有如下两种实现：
RawMessage:
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52998365#)[copy](http://blog.csdn.net/z69183787/article/details/52998365#)[print](http://blog.csdn.net/z69183787/article/details/52998365#)[?](http://blog.csdn.net/z69183787/article/details/52998365#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1961245/fork)
- package com.test.raw;  
- import com.test.service.MessageService;  
- publicclass RawMessage implements MessageService {  
- public String getMessage() {  
- return"Raw message";  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.test.raw;
import com.test.service.MessageService;
public class RawMessage implements MessageService {
    public String getMessage() {
        return "Raw message";
    }
}
```
 FormattedMessage:
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52998365#)[copy](http://blog.csdn.net/z69183787/article/details/52998365#)[print](http://blog.csdn.net/z69183787/article/details/52998365#)[?](http://blog.csdn.net/z69183787/article/details/52998365#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1961245/fork)
- package com.test.format;  
- 
- import com.test.service.MessageService;  
- 
- publicclass FormattedMessage implements MessageService {  
- public String getMessage() {  
- return"Formatted message";  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.test.format;
import com.test.service.MessageService;
public class FormattedMessage implements MessageService {
    public String getMessage() {
        return "Formatted message";
    }
}
```
    针对前面这个接口来说，我们这里的两个实现可以说是两个service provider。因为按照前面这个接口的规范，我们有两个具体的不同实现。他们也许对应着不同的情形。
    OK，这里我们理解了service provider的意思。那么有了这些个接口和service provider，他们有什么用呢？他们和Service Loader有什么关系呢？我们继续往下讨论。
    既然前面提到Service Loader是用来装载这些service provider的，那就是说我们可以使用它来将这些具体实现的类引入进来使用。我们在原来代码的目录下面创建一个META-INF/services的目录，并创建一个com.test.service.MessageService的文件。这个文件名比较有意思，它是和我们前面定义的MessageService类的全名一样的。然后我们在这个文件里保存如下的信息：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52998365#)[copy](http://blog.csdn.net/z69183787/article/details/52998365#)[print](http://blog.csdn.net/z69183787/article/details/52998365#)[?](http://blog.csdn.net/z69183787/article/details/52998365#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1961245/fork)
- com.test.raw.RawMessage  
- com.test.format.FormattedMessage  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
com.test.raw.RawMessage
com.test.format.FormattedMessage
```
    整个项目的结构以及相关配置文件的布局如下图所示：
![](http://www.myexception.cn/img/2013/08/19/104530295.jpg)
     这里我们注意到，META-INF文件夹是放在src这个代码目录下。前面这部分的配置文件到底是做什么用的呢？他们为什么要命名成和类的全名一样？我们看一下使用他们的示例代码再来讨论。
我们在原来的项目中增加如下的代码：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52998365#)[copy](http://blog.csdn.net/z69183787/article/details/52998365#)[print](http://blog.csdn.net/z69183787/article/details/52998365#)[?](http://blog.csdn.net/z69183787/article/details/52998365#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1961245/fork)
- package com.test.messageconsumer;  
- 
- import java.util.ServiceLoader;  
- 
- import com.test.service.MessageService;  
- 
- publicclass MessageConsumer {  
- 
- publicstaticvoid main(String[] args) {  
-         ServiceLoader<MessageService> serviceLoader =   
-                 ServiceLoader.load(MessageService.class);  
- for(MessageService service : serviceLoader) {  
-             System.out.println(service.getMessage());  
-         }  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.test.messageconsumer;
import java.util.ServiceLoader;
import com.test.service.MessageService;
public class MessageConsumer {
	public static void main(String[] args) {
		ServiceLoader<MessageService> serviceLoader = 
				ServiceLoader.load(MessageService.class);
		for(MessageService service : serviceLoader) {
			System.out.println(service.getMessage());
		}
	}
}
```
    这部分代码使用我们前面定义的类，它通过ServiceeLoader的load方法来装载MessageService类型的对象。我们在一个循环里去调用装载的service里的getMessage方法。如果运行这个程序，我们会发现一个如下的结果：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52998365#)[copy](http://blog.csdn.net/z69183787/article/details/52998365#)[print](http://blog.csdn.net/z69183787/article/details/52998365#)[?](http://blog.csdn.net/z69183787/article/details/52998365#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1961245/fork)
- Raw message  
- Formatted message  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
Raw message
Formatted message
```
    现在如果我们从结果来反推那些代码的作用，至少可以看到一点，我们这里的ServiceLoader起作用了。它这个load方法虽然是load MessageService类型的class，实际上把MessageService的所有子类型都装载了进来并可以使用。那么java是怎么知道我们有这么两个子类呢？再想想我们前面定义的META-INF下面的配置文件，我们就可以猜出来了。没错，java通过查找META-INF目录下对应MessageService全名的文件，然后把文件里所有的项读取出来然后尝试去装载它们。我们可以尝试一下去验证他们，比如说我们把com.test.service.MessageService文件里后面的com.test.format.FormattedMessage这一行给删除了再去运行程序，我们会发现只有如下的输出了：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52998365#)[copy](http://blog.csdn.net/z69183787/article/details/52998365#)[print](http://blog.csdn.net/z69183787/article/details/52998365#)[?](http://blog.csdn.net/z69183787/article/details/52998365#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1961245/fork)
- Raw message  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
Raw message
```
    可见，java也不是通过施了什么魔法就找到我们定义的这些类，它是通过读取这个配置文件才知道的。我们现在回过头来看看前面的代码。我们定义了一个接口和它的两个实现，然后设置好配置文件后，一个使用他们的程序可以通过读取配置文件来初始化具体的实现。在使用他们的代码里，我们直接操作的是MessageService这个接口，并不是具体的实现。从这一点来说，很符合我们面向对象中针对接口和抽象编程的原则。而且这个时候如果对依赖注入比较熟悉的人似乎也嗅到了这么一丝的味道。确实，这里可以说是一种依赖注入的一个简单实现，我们通过配置文件可以提供一些特定的类给使用程序。当然，这里针对ServiceLoader还有一个特定的限制，就是我们提供的这些具体实现的类必须提供无参数的构造函数，否则ServiceLoader就会报错。
## ServiceLoader使用的思考
    目前来说，我们已经看到了ServiceLoader的一种简单使用场景。在前面这个示例中，我们对接口的实现是在同一个工程里面，如果我们需要使用他们的时候，完全没必要通过ServiceLoader再装载具体实现进来，我们完全可以通过一个ArrayList再将他们的具体实现一个个加进去就可以了。那么，什么时候用ServiceLoader比较合适呢？既然在同一个工程里我们jvm可以直接装载他们的实现，那么很可能就是我们要装载的实现不在同一个工程里，可能是需要我们动态添加的，这个时候，他们的引入不是编译时候加进来的，是在运行的时候加入的，我们不能像使用普通引入的静态类库那样来使用他们。所以这就是ServiceLoader的优点所在了。比如说我们有一组接口，有的实现是我们本地的，我们可以在使用代码里直接引入进来。而有的却是第三方实现的，他们可能会在运行的时候加入进来。那么我们事先是不清楚的，也就不可能定死了他们的实现是哪个具体的类名。在前面ServiceLoader的使用里，我不用把你具体的实现引用到代码里，而只是在配置文件里指定就可以了。这一点也是我们后面要讨论的DriverManager的一个重要的核心思想。
# DriverManager的应用和设计思路
    DriverManager是jdbc里管理和注册不同数据库driver的工具类。从它设计的初衷来看，和我们前面讨论的场景有相似之处。首先一个，针对一个数据库 可能会存在着不同的数据库驱动实现。我们在使用特定的驱动实现时不希望修改现有的代码才能达到目的，而希望通过一个简单的配置就可以达到效果。
    比如说，我们现在有一个数据库的驱动A，我们希望在程序里使用它而不修改代码。一种理想的选择就是我们将驱动A的信息加入到一个配置文件中，程序通过读取配置文件信息将A加载进来。而以后如果我们希望改用另外一个驱动B的时候，我们之需要将配置文件里的信息修改成驱动B的。我们肯定不希望在代码里写什么registerDriver(new A());之类的代码。尤其在有的情况下我们根本没有使用这些驱动的源代码。
## DriverManager的设计 
    下图是DriverManager相关的类图：
![](http://www.myexception.cn/img/2013/08/19/104530296.jpg)
    在实际的应用中，我们每个具体的驱动都需要实现这里的Driver接口。比如说[MySQL](http://lib.csdn.net/base/14) jdbc的驱动，它的方法声明如下：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52998365#)[copy](http://blog.csdn.net/z69183787/article/details/52998365#)[print](http://blog.csdn.net/z69183787/article/details/52998365#)[?](http://blog.csdn.net/z69183787/article/details/52998365#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1961245/fork)
- publicclass Driver extends NonRegisteringDriver implements java.sql.Driver  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
```java
public class Driver extends NonRegisteringDriver implements java.sql.Driver
```
    从使用的角度来说，我们一般是通过如下的语句来实现装载的：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52998365#)[copy](http://blog.csdn.net/z69183787/article/details/52998365#)[print](http://blog.csdn.net/z69183787/article/details/52998365#)[?](http://blog.csdn.net/z69183787/article/details/52998365#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1961245/fork)
- Class.forName(“com.mysql.jdbc.Driver”);  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
```java
Class.forName(“com.mysql.jdbc.Driver”);
```
    看到这一步的时候，大家估计还是会有点疑惑，这里只是用的Class.forName方法，似乎和DriverManager没关系啊。没错，这一步从表面上看没什么关系，实际上我们来看他们具体的行为。Class.forName主要是做了什么呢？它主要是要求JVM查找并装载指定的类。这样我们的类com.mysql.jdbc.Driver就被装载进来了。而且在类被装载进JVM的时候，它的静态方法就会被执行。我们来看com.mysql.jdbc.Driver的实现代码。在它的实现里有这么一段代码：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52998365#)[copy](http://blog.csdn.net/z69183787/article/details/52998365#)[print](http://blog.csdn.net/z69183787/article/details/52998365#)[?](http://blog.csdn.net/z69183787/article/details/52998365#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1961245/fork)
- static {  
- try {  
-         java.sql.DriverManager.registerDriver(new Driver());  
-     } catch (SQLException E) {  
- thrownew RuntimeException("Can't register driver!");  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
static {
	try {
		java.sql.DriverManager.registerDriver(new Driver());
	} catch (SQLException E) {
		throw new RuntimeException("Can't register driver!");
	}
}
```
    很明显，这里使用了DriverManager并将该类给注册上去了。所以，对于任何实现前面Driver接口的类，只要在他们被装载进JVM的时候注册DriverManager就可以实现被后续程序使用。在实际项目中，如果我们需要有更大的装载灵活性，可以把Class.forName里的参数放到配置文件里，需要装载其他Driver的时候，改一下文件再重启一下程序就可以了。
    我们前面讨论的这种场景主要针对某一个驱动的加载。在某些情况下，我们可能会这样想，如果我们一个项目里要同时用到多个数据库呢？他们每个数据库甚至用到的驱动都不同。那么我们该怎么办呢？很显然，我们可以考虑将这些所有可用的驱动先加载进来，然后再根据不同的需要来选择。DriverManager里面就有这么一个方法可以事先加载已有的一些驱动。我们来看看DriverManager的一部分代码：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52998365#)[copy](http://blog.csdn.net/z69183787/article/details/52998365#)[print](http://blog.csdn.net/z69183787/article/details/52998365#)[?](http://blog.csdn.net/z69183787/article/details/52998365#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1961245/fork)
- static {  
-         loadInitialDrivers();  
-         println("JDBC DriverManager initialized");  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
static {
        loadInitialDrivers();
        println("JDBC DriverManager initialized");
    }
```
    这部分代码是在DriverManager被加载进JVM执行的。而loadInitialDrivers()方法里有一个关键的实现部分如下：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52998365#)[copy](http://blog.csdn.net/z69183787/article/details/52998365#)[print](http://blog.csdn.net/z69183787/article/details/52998365#)[?](http://blog.csdn.net/z69183787/article/details/52998365#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1961245/fork)
- AccessController.doPrivileged(new PrivilegedAction<Void>() {  
- public Void run() {  
- 
-                 ServiceLoader<Driver> loadedDrivers = ServiceLoader.load(Driver.class);  
-                 Iterator driversIterator = loadedDrivers.iterator();  
- 
- try{  
- while(driversIterator.hasNext()) {  
-                         println(" Loading done by the java.util.ServiceLoader :  "+driversIterator.next());  
-                     }  
-                 } catch(Throwable t) {  
- // Do nothing
-                 }  
- returnnull;  
-             }  
-         });  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
AccessController.doPrivileged(new PrivilegedAction<Void>() {
            public Void run() {
                ServiceLoader<Driver> loadedDrivers = ServiceLoader.load(Driver.class);
                Iterator driversIterator = loadedDrivers.iterator();
                try{
                    while(driversIterator.hasNext()) {
                        println(" Loading done by the java.util.ServiceLoader :  "+driversIterator.next());
                    }
                } catch(Throwable t) {
                // Do nothing
                }
                return null;
            }
        });
```
    在这部分代码里，我们看到了一个熟悉的身影，没错，ServiceLoader。这部分代码将写到配置文件里的Driver都加载了进来。在DriverManager里面，有一个ArrayList，就专门用来保存注册好的Driver。它的定义如下：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52998365#)[copy](http://blog.csdn.net/z69183787/article/details/52998365#)[print](http://blog.csdn.net/z69183787/article/details/52998365#)[?](http://blog.csdn.net/z69183787/article/details/52998365#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1961245/fork)
- privatefinalstatic CopyOnWriteArrayList<DriverInfo> registeredDrivers = new CopyOnWriteArrayList<DriverInfo>();  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
```java
private final static CopyOnWriteArrayList<DriverInfo> registeredDrivers = new CopyOnWriteArrayList<DriverInfo>();
```
    这里使用了CopyOnWriteArrayList是因为考虑到在多线程使用的环境下为了保证线程安全。immutable的设计思路，你懂的。
    现在，我们针对DriverManager的初始化和装载过程做一个梳理。首先，DriverManager在一开始的时候会尝试装载配置文件里设定的Driver以及系统环境属性里设定的Driver。它里面也提供了registerDriver的方法。为了保证不和特定的具体Driver耦合，推荐的方式是采用Class.forName的方式来装载这些驱动。而需要在这些驱动的代码里一开始就调用registerDriver来加载自己。这样不管是显式还是隐式的加载驱动都保证这些驱动被保存到了DriverManager里的registeredDrivers列表里。
    我们再来看看对Driver的选择，这里用的Class.forName，它通过传入的String类型参数来确定具体的对象类型。虽然这里返回的只是一个Class对象。我们后面可以通过它的createInstance方法来创建具体的对象。这种手法也是很多工厂方法模式里惯用的手段。在这种方法里，我们通过创建好一系列的对象信息，然后根据传入的参数类型来创建具体的对象。但是返回的还是一个抽象类型。这样就完美的实现了和具体实现的解耦以及对象创建过程的封装。
    现在看来DriverManager实现所采用的思想和工厂模式有很多近似的地方。在我们一些具体问题的设计思路上也有可借鉴之处。下面针对项目中一个具体的问题来进行探讨。
# 一个具体问题的分析
    这是一个项目中碰到的具体的问题，问题的整体结构如下：
![](http://www.myexception.cn/img/2013/08/19/104530297.jpg)
     我们的RequestProcessor从消息队列里边接收消息。然后根据发送过来的消息来选择不同的具体processor，比如说如果消息指定的是list server，那么我们就需要用listServerRequestProcessor。这样，根据不同的请求消息，我们就可以提供不同的processor。从这个步骤来说，我们可能就会考虑到一个典型的工厂模式应用。从问题具体的案例来说，我们消息队列发送过来的消息类型总共有20多个。如果根据这些消息类型去创建不同的processor对象的话，总共也就大概20多种。
    一种我们典型的做法就是将选择processor对象的代码放到一个方法里，然后用一堆的if, else判断条件的语句，像如下所示：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52998365#)[copy](http://blog.csdn.net/z69183787/article/details/52998365#)[print](http://blog.csdn.net/z69183787/article/details/52998365#)[?](http://blog.csdn.net/z69183787/article/details/52998365#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1961245/fork)
- public RequestProcessor getProcessor(String messageType) {  
- if(messageType == "createInstance") {  
- returnnew CreateInstanceProcessor();  
-     } elseif(messageType == "deleteInstance") {  
- returnnew DeleteInstanceProcessor();  
-     } ...  
- //...
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
public RequestProcessor getProcessor(String messageType) {
    if(messageType == "createInstance") {
        return new CreateInstanceProcessor();
    } else if(messageType == "deleteInstance") {
        return new DeleteInstanceProcessor();
    } ...
//...
}
```
     这种做法不是不行，以后每次我们添加或者删除新类型的processor的时候还是很麻烦，需要来修改这边的代码。那么能不能不需要添加这么一堆的判断语句来实现选择对象的效果呢？这个时候，我们可以考虑借鉴DriverManager的思想了。
    首先一个我们可以将这些具体实现的request processor声明放到配置文件里，因为他们都继承自RequestProcessor，我们可以采用ServiceLoader把他们加载进来。下一个问题就是，我们该怎么来选择特定的request processor呢？我们可以学DriverManager里面getDriver的手法。在那里每个具体的Driver实现都提供了一个acceptsURL方法，这个方法判断是否符合该Driver的期望。比如说我给的url是com.mysql.jdbc.Driver，那么则只有mysql
 jdbc的Driver才能返回为true。我们在这里可以依葫芦画瓢的定义一个acceptMessage(String messageType)的方法。每个不同的实现根据不同的参数值来返回对象。那么我们前面那个方法就可以用如下的方式实现了。
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52998365#)[copy](http://blog.csdn.net/z69183787/article/details/52998365#)[print](http://blog.csdn.net/z69183787/article/details/52998365#)[?](http://blog.csdn.net/z69183787/article/details/52998365#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1961245/fork)
- public RequestProcessor getProcessor(String messageType) {  
- for(ServiceLoader service : serviceList) {  
- if(service.acceptMessage(messageType)) {  
- return service;  
-         }  
-     }  
- returnnull;  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
public RequestProcessor getProcessor(String messageType) {
    for(ServiceLoader service : serviceList) {
        if(service.acceptMessage(messageType)) {
            return service;
        }
    }
    return null;
}
```
     我们这里的serviceList定义可以放在类的静态代码中：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52998365#)[copy](http://blog.csdn.net/z69183787/article/details/52998365#)[print](http://blog.csdn.net/z69183787/article/details/52998365#)[?](http://blog.csdn.net/z69183787/article/details/52998365#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1961245/fork)
- ServiceLoader<MessageService> serviceList =   
-                 ServiceLoader.load(RequestProcessor.class);  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
```java
ServiceLoader<MessageService> serviceList = 
				ServiceLoader.load(RequestProcessor.class);
```
    这是一种选择对象的方式，当然，我们也可以把所有对象放到一个map里面，然后根据传入的参数去选择。
# 总结 
    ServiceLoader是一个可以实现动态加载具体实现类的机制，通过它可以实现和具体实现代码的解耦。也可以实现类似于IOC的效果。在jdbc的DriverManager里，就使用了ServiceLoader的特性。对它这种特性的灵活运用可以带来很多代码上实现的简化。它也可以作为一种工厂方法实现的参考。
# 参考材料
effective java
http://docs.oracle.com/javase/7/docs/api/java/util/ServiceLoader.html
http://my.oschina.net/hanzhankang/blog/109794
http://ludaojuan21.iteye.com/blog/243528
