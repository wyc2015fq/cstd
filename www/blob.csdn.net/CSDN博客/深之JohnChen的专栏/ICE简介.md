# ICE简介 - 深之JohnChen的专栏 - CSDN博客

2019年04月07日 13:28:33[byxdaz](https://me.csdn.net/byxdaz)阅读数：162


一、ICE简介

1、ICE是什么？

 ICE是ZEROC的开源通信协议产品，它的全称是：The Internet Communications Engine，翻译为中文是互联网通信引擎，是一个面向对象的中间件，使我们能够以最小的代价构建分布式应用程序。ICE使我们专注于应用逻辑的开发，它来处理所有底层的网络接口编程，这样我们就不用去考虑这样的细节：打开网络连接、网络数据传输的序列化与反序列化、连接失败的尝试次数等。

官网：[https://zeroc.com/](https://zeroc.com/)

文档：[https://doc.zeroc.com/ice/3.6/](https://doc.zeroc.com/ice/3.6/)

2、为什么会有ICE？ 

 ICE是分布式应用的一种比较好的解决方案，虽然现在也有一些比较流行的分布式应用解决方案，如微软的.NET（以及原来的DCOM）、CORBA及WEB SERVICE等，但是这些面向对象的中间件都存在一些不足：

 .NET是微软产品，只面向WINDOWS系统，而实际的情况是在当前的网络环境下，不同的计算机会运行不同的系统，如LINUX上面就不可能使用.NET；

 CORBA虽然在统一标准方面做了很多的工作，但是不同的供应商实现之间还是缺乏互操作性，并且目前还没有一家供应商可以针对所有的异种环境提供所有的实现支持，且CORBA的实现比较复杂，学习及实施的成本都会比较高；

 WEB SERVICE最要命的缺点就是他的性能问题，对于要求比较高的行业是很少会考虑WEB SERVICE的。

 ICE的产生就是源于.NET、CORBA及WEB SERVICE这些中间件的不足，它可以支持不同的系统，如WINDOWS、LINUX等，也可以支持在多种开发语言上使用，如C++、C、JAVA、RUBY、PYTHON、VB等，服务端可以是上面提到的任何一种语言实现的，客户端也可以根据自己的实际情况选择不同的语言实现，如服务端采用C语言实现，而客户端采用JAVA语言实现，底层的通讯逻辑通过ICE的封装实现，我们只需要关注业务逻辑。

3、ICE是如何工作的？

Ice 是一种面向对象的中间件平台，这意味着 Ice为构建面向对象的客户－服务器应用提供了工具、API和库支持。要与Ice持有的对象进行通信，客户端必须持有这个对象的代理（与CORBA的引用是相同的意思），这里的代理指的是这个对象的实例，ICE在运行时会定位到这个对象，然后寻找或激活它，再把In参数传给远程对象，再通过Out参数获取返回结果。

这里提到的代理又分为直接代理和间接代理，直接代理其内部保存有某个对象的标识，以及它的服务器的运行地址；间接代理指的是其内部保存有某个对象的标识，以及对象适配器名（objectadaptername），间接代理没有包含寻址信息，为了正确地定位服务器，客户端在运行时会使用代理内部的对象适配器名，将其传给某个定位器服务，比如IcePack服务，然后，定位器会把适配器名当作关键字，在含有服务器地址的表中进行查找，把当前的服务器地址返回给客户，客户端run time现在知道了怎样联系服务器，就会像平常一样分派 （dispatch）客户请求。

ICE可以保证在任何的网络环境或者操作系统下，成功的调用只有一次，它在运行时会尽力的定位到远程服务器，在连接失败的情况下会做尝试性重复性连接，确实连不上的情况会给用户以提示。

客户端在调用服务端的方法时，可以采取同步或异步的方式实现，同步调用就相当于调用自己本地的方法一样，其它行为会被阻塞；异步调用是非常有用的调用方式，如服务端需要准备的数据来自于其它异步接口，这个时候客户端就不需要等待，待服务端数据准备充份后，以消息的方式通知客户端，服务端就可以去干其它的事情了，而客户端也可以到服务端获取数据了。

Ice应用组成：

Ice对象——Ice对象是在本地或远地的地址空间中、能响应客户请求的实体。先由Slice定义，然后编译成特定语言版本；每个Ice对象有多个接口，支持特定操作，客户通过调用它的接口来发出请求；每个对象有唯一的对象标识。

Servants——服务器端编写的类实例，体现Ice对象行为。

代理(Proxies)——代理是Ice对象在客户端的代表，客户调用代理上的操作来获得服务端上Ice对象提供的服务。

通信器(Ice::Communicator)——通信时，服务器和客户建立一个通信器，为通信进程分配和管理资源，是双方通信的专用线路。

对象适配器(object adapter)——专用于服务器端的Ice API，把Ice run time与服务端编写的Servant类连接在一起。每个适配器绑定一个端口。

Ice应用的结构如下：

![](https://img-blog.csdnimg.cn/20190407132208425.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9jaGVuemhlbmd5aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

Slice

Slice (Specification Language for Ice, Ice规范语言)，用于对象接口定义与实现相分离的基础性抽象机制，可以把定义好的Ice对象映射成特定语言。具体规范参考https://blog.csdn.net/kaede999/article/details/39337475
Slice开发过程见下图：
相同开发环境：

![](https://img-blog.csdnimg.cn/20190407134407731.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9jaGVuemhlbmd5aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

 代理是根据SLICE定义的ice文件实现，它提供了一个向下调用的接口，提供了数据的序列化与反序化。

 ICE的核心部份，提供了客户端与服务端的网络连接等核心通信功能，以及其它的网络通信功能的实现及可能的问题的处理，让我们在编写应用代码的时候不必要去关注这一块，而专注于应用功能的实现。

4、ICE调用模式

 ICE采用的网络协议有TCP、UDP以及SSL三 种，不同于WebService，ICE在调用模式上有好几种选择方案，并且每种方案正对不同的网络协议的特性做了相应的选择。

ICE采用的网络协议有TCP、UDP以及SSL三种，不同于WebService，ICE在调用模式上有好几种选择方案，并且每种方案正对不同的网络协议的特性做了相应的选择。

Oneway(单向调用)

       客户端只需将调用注册到本地传输缓冲区（Local Transport Buffers）后就立即返回，并不对调用结果负责。

Twoway（双向调用）

       最通用的模式，同步方法调用模式，只能用TCP或SSL协议。

Datagram（数据报）

       类似于Oneway调用，不同的是 Datagram调用只能采用UDP协议而且只能调用无返回值和无输出参数的方法。

BatchOneway（批量单向调用）

       先将调用存在调用缓冲区里面，到达一定限额后自动批量发送所有请求（也可手动刷除缓冲区）。

BatchDatagram（批量数据报）

       与上类似。

       不同的调用模式其实对应着不同的业务，对于大部分的有返回值的或需要实时响应的方法，我们可能都采用twoway方式调用，对于一些无需返回值或者不依赖返回值的业务，我们可以用Oneway或者BatchOneway方式，例如消息通知。剩下的Datagram和BatchDatagram方式，一般用在无返回值且不做可靠性检查的业务上，例如日志。

 不同的调用模式其实对应着不动的业务，对于大部分的有返回值的或需要实时响应的方法，我们可能都采用Twoway方式调用，对于一些无需返回值或 者不依赖返回值的业务，我们可以用Oneway或者BatchOneway方式，例如消息通知；剩下的Datagram和BatchDatagram方式 一般用在无返回值且不做可靠性检查的业务上，例如日志。

二、ICE框架的安装与配置

Windows下ICE框架的安装与配置

1，下载安装：

下载是在官网下载的，但是官网点击链接没有响应，想了个办法，调出下载页面的源代码，从中找出下载链接，部分下载链接如下：

3.7.1版本：[https://zeroc.com/download/Ice/3.7/Ice-3.7.1.msi](https://zeroc.com/download/Ice/3.7/Ice-3.7.1.msi)

3.6.4版本：[https://zeroc.com/download/Ice/3.6/Ice-3.6.4.msi](https://zeroc.com/download/Ice/3.7/Ice-3.7.1.msi)

2，配置：

本文安装路径：D:\ZeroC\ICE3.6.4

在我的电脑中配置环境变量，path路径中进行添加：

D:\ZeroC\ICE3.6.4\lib\vc140

D:\ZeroC\ICE3.6.4\bin

若为Win7的系统，需要用英文分号“;”连接。

上面的环境变量事C++语言的，如果是java语言，配置环境变量如下：

主要有环境变量需要设置：path、classpath、ICE_HOME

例如：

path:D:\Ice\bin;

classpath:D:\Ice\lib\Ice.jar;D:\Ice\lib\Freeze.jar;D:\Ice\lib\db.jar;

ICE_HOME:D:\Ice

3，检验

配置完成后，在cmd命令提示符中输入slice2cpp -v，可以看见相应版本号。

![https://img-blog.csdn.net/201805221446035](https://img-blog.csdn.net/201805221446035)

表明已经配置成功。

Linux下安装ICE
[https://blog.csdn.net/flamezyg/article/details/44174905](https://blog.csdn.net/flamezyg/article/details/44174905)

# **三、Ice属性与配置**

Ice使用了一种配置机制，允许我们控制自己的Ice应用在运行时的许多行为，比如最大消息尺寸、线程数，是否产生网络跟踪消息。这种机制不仅能用于配置Ice，还可以用它来给自己的应用提供配置参数。

**1****、属性**

Ice及其各子系统是通过属性（property）来配置的。一个属性就是一个名-值对（name-value）。例如： Ice.UDP.SendSize = 65535;

保留的前缀：Ice , IceBox , IcePack , IcePatch , IceSSL , IceStorm , Freeze及Glacier。我们不能用这些前缀起头的属性来配置自己的应用。

**2****、配置文件**

属性通常在配置文件中设置。配置文件含有一些名-值对，每一对都在单独的行上。空行及完全有空白字符组成的行会被忽略。#字符后面的字符串是注释，直到当前行行尾。

**3****、配置文件示例：**

# Example config file for Ice

Ice.MessageSizeMax = 2048  # Larger message size is 2MB

Ice.Trace.NetWork = 3      # Highest level of tracing for network

Ice.Trace.Protocol =        # Disable protocol tracing


如果把空置赋给属性，就会清除该属性。

对于C++，Ice会在我们创建通信器时读取配置文件的内容，配置文件名由ICE_CONFIG环境变量的内容来决定。例如：

$ ICE_CONFIG = /opt/Ice/default_config

$ export ICE_CONFIG

$ ./server

这使得服务器从配置文件/opt/Ice/default_config中读取它的属性设置。

**4.      ****在命令行中设置属性**

除了在配置文件中设置属性。我们还可以在命令行上设置属性，例如：

$ ./server –Ice.UDP.SendSize=65535 –IceSSL.Trace.Security=2

任何以—起头，并且后跟某个保留前缀的命令行选项，都会在创建通信器时被读取，并转换成属性设置。命令行中的属性设置会覆盖配置文件中的设置。

  为方便起见，任何没有明确设置的属性都会被设置为值1.

例如：./server –Ice.Trace.Protocol等价于 ./server –Ice.Trace.protocol=1(只适合命令行设置)

**5.      Ice.Config****属性**

对于Ice run time而言，Ice.Config属性有着特殊含义：它确定用于读取属性设置的配置文件的路径名。例如：

 $ ./server –Ice.Config=/usr/local/filesystem/config

 这使得配置设置从/usr/local/filesystem/comfig配置文件中被读出。

**6.      ****命令行解析与初始化**

如果我们使用了Ice::Application助手类，run方法收到的参数也是清理过的参数向量。

**7.      Ice.ProgramName****属性**

在C++中,initialize把Ice.ProgramName属性设成当前程序的名字(argv[0])。Ice把程序名用于日志信息。

**8.      ****在程序中使用属性**

Ice属性机制不仅可用于配置Ice,还可以用于自己的应用程序的配置机制。例如我们引入一个属性，控制我们的文件系统应用的最大文件尺寸:

# Configuration file for file system application

Filesystem.MaxFileSize = 1024  #Max file size in kB

Ice run time像存储其他任何属性一样存储FileSystem.MaxFileSize属性，并且让我们通过Properties接口访问它。

要在自己的程序里访问属性值，需要调用getProperties，获取通信器的各个属性：

module Ice {

localinterface Properties; // Forward declaration

localinterface Communicator {

PropertiesgetProperties();

// ...

};

};

Properties接口提供了用于读写属性设置的方法：

module Ice {

local dictionary<string,string> PropertyDict;

local interface Properties {

stringgetProperty(string key);

stringgetPropertyWithDefault(string key, string value);

intgetPropertyAsInt(string key);

intgetPropertyAsIntWithDefault(string key, int value);

PropertyDictgetPropertiesForPrefix(string prefix);

voidsetProperty(string key, string value);

StringSeqgetCommandLineOptions();

StringSeqparseCommandLineOptions(string prefix,

StringSeqoptions);

StringSeqparseIceCommandLineOptions(StringSeq options);

voidload(string file);

Propertiesclone();

};

};

     8.1）读取属性

getProperty：这个操作返回指定属性的值。

getPropertyWithDefault：如果该属性没有设置，操作返回自己提供的缺省值。

getPropertyAsInt：把指定属性的值作为整数返回。

getPropertyAsIntWithDefault：若属性没有设置，或包含的是不能解析成整数的串，返回自己所提供的缺省值。

getPropertyForPrefix：把以指定前缀起头的所有属性，作为PropertyDict类型的词典返回。


例如：

// ...

Ice::CommunicatorPtric;

// ...

ic =Ice::initialize(argc, argv);

// Getthe maximum file size.

//

Ice::PropertiesPtr props =ic->getProperties();

Ice::IntmaxSize

=props->getPropertyAsIntWithDefault("Filesystem.MaxFileSize",

1024);

// ...


假定我们创建了一个配置文件，用于设置Filesystem.MaxFileSize属性（并相应地设置了ICE_CONFIG变量或—Ice.Config选项），我们的应用将会获得所配置的属性值。

  8.2）设置属性

      setProperty 操作把某个属性设成指定的值（只要把属性设成空串，你就可以清除它）。只有在你调用initialize 之前，这个操作才有用。如果不管配置文件中的设置是什么，你都想要确保某个属性被设成特定的值， getDefaultProperties 会很有用。例如：

  setProperty可将属性设置为指定值。由于通讯器在调用Ice:initialize(argc,argv);时就会读取配置文件，之后不再重新读配置。因此本函数应该在初始化之前调用，初始化之后设置属性，在重启应用程序前是不起作用的。

Ice::PropertiesPtr props = Ice::createProperties(argc, argv);

props->setProperty("Ice.Trace.Network","0");

props->setProperty("Ice.Trace.Protocol","0");

// ...

Ice::InitializationData id;

id.properties = props;

Ice::CommunicatorPtr ic = Ice::initialize(id);

// ...

  8.3）解析属性

            Properties接口提供了三个用于转换和解析属性的操作：getCommandLineOptions，parseCommandLineOptions，parseIceCommandLineOptions。

  8.4）实用操作

           properties接口提供了两个实用操作：clone--这个操作制作一个现有属性集的副本。

                                                                              load-- 这个操作的参数是一个配置文件的路径名，他会根据这个文件初始化属性集。

  8.5）处理多个属性集

      有时在程序中可能使用了多个通信器，因而需要使用多个属性集。为了能都使用独立的属性集，Ice提供了用于创建新属性集的实用函数：

 // Create a property set for the first communcator.

//

Ice::PropertiesPtr props1 = createProperties();

// Make sure that network tracing is off.

//

props1->setProperty("Ice.Trace.Network", "0");

// Initialize a communicator with this property set.

//

Ice::CommunicatorPtr ic1

= Ice::initializeWithProperties(argc, argv, props1);

四、同步调用

默认情况下，使用由ICE的请求调用模型也就是远程同步调用：一个函数操作调用行为像本地函数调用一样，就是客户端在调用期间线程被挂起，而在调用完成之后恢复。

5.1 定义接口文件

```
#ifndef demo_idl
#define demo_idl
 
module Demo
{
    interface Hello
    {
        string sayHello(int num, out string strMsg);
    };
};
 
#endif
```

        利用Slice定义接口/类方法时和其他编程语言很相似，需要一个返回值和若干个参数，但是需要注意的是Slice不支持引用方式的参数传递，参数要么为输入参数，要么为输出参数，不同时为in和out。作为out参数的时候，不管客户端对out参数的初始赋值是什么，在服务端都取不到该值(参数strMsg在服务端始终为空)，但是服务端可以对该参数进行赋值，再传递给客户端。还要注意的是：out参数一定是放在所有输入参数后面，不允许交叉使用。

5.2 服务端实现代码

// HelloImplement.h

```
#pragma once

#include "demo.h"
#include <string>

using namespace std;

class HelloImplement : public Demo::Hello
{
public:
	HelloImplement();
	~HelloImplement();

	virtual std::string sayHello(Ice::Int, std::string&, const Ice::Current& = ::Ice::Current());
};
```

// HelloImplement.cpp

```
#include "stdafx.h"
#include "HelloImplement.h"

HelloImplement::HelloImplement()
{
}

HelloImplement::~HelloImplement()
{
}

std::string HelloImplement::sayHello(int nNum, std::string& strMsg, const Ice::Current& current)
{
	strMsg = "output message";
	cout << "Hello World!" << endl;
	return strMsg;
}
```

//main

```
#include "stdafx.h"
#include <Ice/Ice.h>
#include <Ice/Application.h>
#include <Ice//Service.h>
#include "demo.h"
#include "HelloImplement.h"

int main(int argc,char * argv[])
{
	Ice::CommunicatorPtr ic = Ice::initialize(argc, argv);
	Ice::ObjectAdapterPtr adapter = ic->createObjectAdapterWithEndpoints("HelloAdapter", "default -p 10001");
	Demo::HelloPtr hello = new HelloImplement();
	adapter->add(hello, ic->stringToIdentity("Hello"));
	adapter->activate();
	ic->waitForShutdown();
	ic->destroy();
    return 0;
}
```

windows c++使用ice库需要包含库路径和引用lib文件

包含ice库的include和lib路径

debug版本引用iced.lib iceutild.lib

release版本引用ice.lib iceutil.lib

5.3 客户端实现代码

```
#include "stdafx.h"
#include <Ice/Ice.h>
#include "demo.h"
#include <iostream>

using namespace Demo;

int main(int argc, char * argv[])
{
	Ice::CommunicatorPtr ic = Ice::initialize(argc,argv);
	try {
	Ice::ObjectPrx base = ic->stringToProxy("Hello:default -p 10001");
	HelloPrx hello = HelloPrx::checkedCast(base);
	std::string strTemp = "abc";
	hello->sayHello(1, strTemp);
	std::cout << strTemp << std::endl;
	}
	catch (Ice::ConnectFailedException & ex)
	{
	std::cout << ex.what() << std::endl;
	}
	catch (Ice::Exception & ex)
	{
	std::cout << ex.what() << std::endl;
	}
	ic->destroy();
	return 0;
}

/*
int main(int argc, char * argv[])
{
	//设置属性
	Ice::PropertiesPtr props = Ice::createProperties();
	props->setProperty("Ice.Trace.Network", "0");
	props->setProperty("Ice.Trace.Protocol", "0");
	props->setProperty("Ice.MessageSizeMax", "2048");
	Ice::InitializationData id;
	id.properties = props;
	Ice::CommunicatorPtr ic = Ice::initialize(id);
	try {
		Ice::ObjectPrx base = ic->stringToProxy("Hello:default -p 10001");
		//Ice::ObjectPrx base = ic->stringToProxy("Hello:default -p 10001")->ice_timeout(5000);//超时代理
		HelloPrx hello = HelloPrx::checkedCast(base);
		std::string strTemp = "abc";
		hello->sayHello(1, strTemp);
		std::cout << strTemp << std::endl;
	}
	catch (Ice::TimeoutException & ex) {
		std::cout << "invocation timed out" << std::endl;
	}
	catch (Ice::ConnectFailedException & ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (Ice::Exception & ex)
	{
		std::cout << ex.what() << std::endl;
	}
	ic->destroy();
    return 0;
}
*/
```

五、异步调用

ICE具备异步方法调用的API函数（AMI-- Asynchronous Method Invocation）。然而，这个API相当冗长而且使用不容易，应用不灵活。随着ICE3.4发布之后，ZeroC引入了一个新的API的异步方法调用不再遭受这些问题了，为开发人员提供了更多的选择，他们如何可以构建自己的代码。新特性的API适合可用于C++，Java，和Python。

       ICE异步有两种形式，一种是客户端的AMI形式，一种是服务器端的AMD（Asynchronous Method Dispatch），在3.4版本之后AMI已经不再显示指定时，slice就会生成callback类，在客户端直接用say()是同步的，begin_say( …)就是异步的。

         3.4版本之前异步调用方法参考[https://www.cnblogs.com/tangr206/archive/2011/11/21/2256906.html](https://www.cnblogs.com/tangr206/archive/2011/11/21/2256906.html)

         本文介绍3.4版本之后的异步调用方法。

6.1 定义接口文件

```
#ifndef demo_idl
#define demo_idl
 
module Demo
{
    interface Hello
    {
        string sayHello(int num, out string strMsg);
    };
};
 
#endif
```

6.2 服务端实现代码

// HelloImplement.h

```
#pragma once

#include "demo.h"
#include <string>

using namespace std;

class HelloImplement : public Demo::Hello
{
public:
	HelloImplement();
	~HelloImplement();

	virtual std::string sayHello(Ice::Int, std::string&, const Ice::Current& = ::Ice::Current());
};
```

// HelloImplement.cpp

```
#include "stdafx.h"
#include "HelloImplement.h"

HelloImplement::HelloImplement()
{
}

HelloImplement::~HelloImplement()
{
}

std::string HelloImplement::sayHello(int nNum, std::string& strMsg, const Ice::Current& current)
{
	strMsg = "output message";
	cout << "Hello World!" << endl;
	return strMsg;
}
```

//main

```
#include "stdafx.h"
#include <Ice/Ice.h>
#include <Ice/Application.h>
#include <Ice//Service.h>
#include "demo.h"
#include "HelloImplement.h"

int main(int argc,char * argv[])
{
	Ice::CommunicatorPtr ic = Ice::initialize(argc, argv);
	Ice::ObjectAdapterPtr adapter = ic->createObjectAdapterWithEndpoints("HelloAdapter", "default -p 10001");
	Demo::HelloPtr hello = new HelloImplement();
	adapter->add(hello, ic->stringToIdentity("Hello"));
	adapter->activate();
	ic->waitForShutdown();
	ic->destroy();
    return 0;
}
```

6.3 客户端实现代码

```
#include "stdafx.h"
#include <Ice/Ice.h>
#include "demo.h"
#include <iostream>

using namespace Demo;

int main(int argc, char * argv[])
{
	Ice::CommunicatorPtr ic = Ice::initialize(argc, argv);
	try {
		Ice::ObjectPrx base = ic->stringToProxy("Hello:default -p 10001");
		HelloPrx hello = HelloPrx::checkedCast(base);
		std::string strTemp = "";
		::Ice::AsyncResultPtr helloAsyncResultPtr = hello->begin_sayHello(1);
		while (true)
		{
			if (helloAsyncResultPtr == NULL)
			{
				break;
			}
			if (helloAsyncResultPtr->isCompleted())
			{
				hello->end_sayHello(strTemp, helloAsyncResultPtr);
				break;
			}
			else
			{
				Sleep(500);
			}
		}
		std::cout << strTemp << std::endl;
	}
	catch (Ice::ConnectFailedException & ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (Ice::Exception & ex)
	{
		std::cout << ex.what() << std::endl;
	}
	ic->destroy();
	return 0;
}
```

六、其他组件介绍

6.1  icestorm 消息发布订阅

IceStorm是一个高效的用于ICE应用的发布/订阅服务，IceStorm有几个比较重要的概念：

²  消息：IceStorm的消息和普通的消息队列中间件中描述的消息有点区别，IceStorm 的消息是强类型的，由对某个Slice 操作的调用代表：操作名标识消息的类型，操作参数定义消息内容。要发布消息，可以按普通的方式调用某个IceStorm 代理上的操作。与此类似，订阅者会像收到常规的向上调用（upcall）一样收到消息。所以IceStorm 的消息递送使用的是“推”模式

²  主题：应用要通过订阅某个主题（topic）来表明自己有兴趣接收某些消息。IceStorm 服务器能够支持任意数量的主题，这些主题是动态创建的，通过唯一的名字来区分。每个主题都可以有多个发布者和订阅者。   

²  持久模式：IceStorm 拥有一个数据库，里面维护的是关于其主题和链接的信息。但是，通过IceStorm发送的消息不会被持久地存储，而是会在递送给主题目前的订阅者集之后，马上被丢弃。如果在把消息递送给某个订阅者的过程中发生错误， IceStorm 不会为该订阅者进行消息排队。

²  订阅者出错：因为IceStorm 消息是采用单向语义递送的， IceStorm 只能检测到连接或超时错误。如果在把消息递送给订阅者的过程中， IceStorm 遇到这样的错误，该订阅者就会立刻被解除与该消息对应的主题的订阅。当然用户在使用过程中也可以通过设定QOS参数来改善这个问题，比如重试次数（retryCount），但是对于ObjectNotExistException 或者 NotRegisteredException之类的硬错误，Ice运行时不会重试，而是仍然直接解除订阅关系。

IceStorm支持两个主要的QOS参数reliability 和 retryCount，reliability的取值分别为ordered 和空值，取ordered时，发布者发布的消息会保证按顺序递送给订阅者。

从IceStorm提供的功能来看，对于不需要进行消息持久存储转发的应用来说很适合，但是由于在订阅者出错后立即解除订阅关系，不是由订阅者主动解除，这个在应用中需要特别注意是否符合实际应用。

IceStorm被实现为IceBox服务，所以在部署IceStorm应用时需要启动IceBox服务。

ICE发布者／订阅者的一个最简单例子

[https://www.cnblogs.com/leaf-w/p/3786387.html](https://www.cnblogs.com/leaf-w/p/3786387.html)

6.2动态服务管理（IceBox）

IceBox 用于动态加载用户服务并对他们进行集中管理，可以通过iceboxadmin管理工具对IceBox中的服务进行远程管理，通过IceBox用户服务可以被开发成可以动态加载的动态库组件.

使用IceBox的服务组件需要继承IceBox::Service类，并实现start（）、stop()方法，并在实现类中提供服务进入点函数,一般为create()函数，在这函数中创建服务实现类的对象并返回。例如：

extern "C"

{

ICE_DECLSPEC_EXPORT IceBox::Service*

create(Ice::CommunicatorPtr communicator)

{

    return new HelloServiceI;

}

}

    对于Ice3.3.0以上版本，iceboxadmin提供了启动、停止服务及停止IceBox 服务器的命令管理工具和应用程序接口，管理工具命令如下：

iceboxadmin [options] [command...]

commands:

start SERVICE Start a service.

stop SERVICE Stop a service.

shutdown Shutdown the server.

6.3  IceGrid分布式网络服务

IceGrid用于支持分布式网络服务应用，一个IceGrid域由一个注册表（Registry）和任何数目的节点(Node)构成。注册表（Registry）和节点(Node)一起合作管理一些信息以及包含一些应用（Application）的服务进程。每项应用（Application）被指定在特定节点上的服务。这个注册表（Registry）持久记录了这些信息，而节点（Node）负责启动和监测其指定的服务器进程。对于一个典型的配置，一个节点（Node）运行在一台计算机(称之为Ice服务器主机)。注册表（Registry）并不消耗很多处理器时间，所以它常常是和一个节点（Node）运行在同一台计算机上的，注册表（Registry）还可以和一个节点（Node）可以运行在同一进程中.如果需要容错，注册表（Registry）还可以用主从式的设计支持复制（Replication）。

注册表（Registry）的主要责任，是解决作为Ice定位服务的间接代理问题，当客户端第一次尝试使用一种间接代理，客户端Ice run time首先连接注册表（registry），注册表将间接代理的符号信息转化为直接代理的endpoint，然后客户端和直接代理建立一个连接。通过适配器复制，同名适配器可以分布在多个节点上，间接代理可以映射到多个节点上的直接代理，在运行时由注册表服务根据负载均衡自动选择一个直接代理给客户端。

使用间接代理时，客户端可以用以下方式直接获取服务对象代理：

MyProxy=theObject@theAdapter   //  对象@适配器

更简单一点的话可以用以下方式

MyProxy=theObject   //  对象

6.3.1 分布式部署

在部署IceGrid分布式服务时，需要启动注册表服务（icegridregistry），并配置注册表服务地址端口、通信协议和注册信息保存的目录地址（ICE的注册信息保存为BerkeleyDB的数据库文件）：

IceGrid.Registry.Client.Endpoints=tcp -p 4061

IceGrid.Registry.Data=/opt/ripper/registry

在服务器节点中和客户端都需要配置注册表服务的地址端口和通信协议：

Ice.Default.Locator=IceGrid/Locator:tcp -h 172.0.0.1 -p 4061

然后分别启动注册表服务(icegridregistry)和节点服务(icegridnode).

ICE提供了部署工具icegridadmin, 这个icegridadmin工具也需要定义Ice.Default.Locator属性.

接下需要编写应用部署文件，应用部署文件以XML方式保存。以下为支持适配器复制的应用配置文件，使用了服务模板：

//定义适配器复制组

将在客户端中使用。

type="::Ripper::MP3EncoderFactory"/>

//定义服务器模板

default="/opt/ripper/bin/server"/>

exe="${exepath}"

activation="on-demand">

replica-group="EncoderAdapters"

endpoints="tcp"/>



index="1"/>


index="2"/>


然后在客户端可以用以下方式获取对象代理：

Ice::ObjectPrx obj = communicator->stringToProxy("EncoderFactory");

6.3.2 负载均衡

ICE平台内嵌负载均衡功能，对于分布大多个节点上的应用服务提供多种负载均衡方案，只需要通过XML配置文件即可完成负载均衡配置。配置项包括Type (负载均衡类型)、Sampling interval（负载信息收集间隙）、Number of replicas（返回给客户端的适配器个数）。

负载均衡类型有以下4种方式：

²  Random (随机方式):注册中心随机选择一个适配器给客户端，不检查适配器的负载。

²  Adaptive(适配方式)：注册中心从所有适配器中选择一个负载最轻的适配器给客户端，Sampling interval参数只有在该类型的负载均衡中有效，这个参数指定节点定期向注册中心报告本地系统负载信息（system load information）;

²  Round Robin(最近最少使用):注册中心从对应的适配器组中选择一个最近最少使用的适配器给客户。

²  Ordered（顺序方式）：注册中心根据适配器的优先级，从高到低顺序选择一个适配器给客户端。

配置示例：

   //配置为适配方式

type="::Ripper::MP3EncoderFactory"/>

6.3.3 注册中心集群

前两节中描述的是属于用户应用的分布部署，分布式部署一个很重要的支撑是ICE的注册中心，所有客户端都向注册中心查询服务代理的真实端点，从而建立通信连接，在这里注册中心又成了一个单点服务，为了避免注册中心成为应用的瓶颈，提高系统的可靠性，ICE3.3.0以上版本提供了注册中心集群功能。

ICE注册中心集群通过主从式的注册中心复制来实现，一个集群中有一个主注册中心，若干个副注册中心，主从的区别通过IceGrid.Registry.ReplicaName属性配置来实现，主注册中心的名称为 Master，其他的名字可以任意取。启动时先启动主注册中心，再启动其他注册中心，通过主注册中心更新的信息都将同步给副注册中心，各副注册中心之间不通信。如果主注册中心失效，需要从其他副注册中心提拨一个成为主注册中心，但是从3.3版本的说明文档中来看，如果需要把某个副注册中心提拨成为主注册中心需要重新启动相应进程并修改IceGrid.Registry.ReplicaName 属性值为Master，或者删除该属性，默认情况下该属性值为Master.

使用集群方式时，在客户端配置时把所有的主从注册中心地址端口全部填到Ice.Default.Locator,例如：

Ice.Default.Locator=IceGrid/Locator:default -p 12000:default -p 12001

在应用节点也把所有的注册中心地址端口绑定，这样应用的更新会同时通知所有的注册中心。

ZeroC IceGrid介绍及demo构建（使用IceBox）

https://blog.csdn.net/liitdar/article/details/80185904

6.4 IceSSL应用

ICE平台可以通过简单的配置来支持SSL应用，配置过程如下：

²  首先需要通过修改配置文件来启用SSL插件，C++服务端的配置方法为：Ice.Plugin.IceSSL=IceSSL:createIceSSL

只需要把IceSSL动态库放到LD_LIBRARY_PATH包含的路径下即可。

²  然后修改适配器的监听选项：

MyAdapter.Endpoints=tcp -p 8000:ssl -p 8001:udp -p 8000 //表示该适配器在三种协议端口上同时监听。

ICE还提供了多种配置属性来满足实际应用，例如下例所示：

Ice.Plugin.IceSSL=IceSSL:createIceSSL

IceSSL.DefaultDir=/opt/certs  //默认证书目录

IceSSL.CertFile=pubkey.pem  //证书文件

IceSSL.KeyFile=privkey.pem  //私钥文件

IceSSL.CertAuthFile=ca.pem  //信任的根证书文件

IceSSL.Password=password    //私钥文件查看密码

6.4 IceFreeze持久化存储

ICE提供的持久化方案可以支持普通用户数据（键/值对）的持久化存储和服务对象实例的持久化管理，普通用户数据的持久化存储使用比较简单，服务对象实例的管理相对复杂一点，暂时不关注。

ICE的持久存储介质为BerkeleyDB,对普通数据的持久化在C++实现中采用Map的方式进行操作，用户需要用Slice定义需要存储的数据，并用slice2freeze 生成相应的Map操作类，然后对数据的操作就可以使用Map容器函数来进行。示例如下：

首先生成需要存储的数据类型：

slice2freeze --dict StringIntMap,string,int StringIntMap

代码使用：

Ice::CommunicatorPtr communicator =

Ice::initialize(argc, argv);

// Create a Freeze database connection.

Freeze::ConnectionPtr connection = Freeze::createConnection(communicator, "db"); //连接到数据库文件。

// Instantiate the map.

StringIntMap map(connection, "simple");//创建表。

// Clear the map.

map.clear();

Ice::Int i;

StringIntMap::iterator p;

// Populate the map.

for (i = 0; i < 26; i++)

{

std::string key(1, 'a' + i);

map.insert(make_pair(key, i));

}

// Iterate over the map and change the values.

for (p = map.begin(); p != map.end(); ++p)

p.set(p->second + 1);

// Find and erase the last element.

p = map.find("z");

assert(p != map.end());

map.erase(p);

// Clean up.

connection->close();

communicator->destroy();

IceFreeze还允许使用结构体和类对象作为值进行存储，但是只有public的成员变量会被存储，其他成员变量不会被存储。

对于较高版本的ICE，还允许对值建立索引，如果值为结构体或者类对象，那么还允许以结构体/对象变量作为索引，通过slice2freeze编译后会生成对应的索引查询函数。例如定义了如下需要存储的数据结构：

module Demo

{

struct Struct1

{

    long l;

};

class Class1

{

    string s;

};

};

然后执行以下命令生成映射表，同时生成索引，以class1的成员变量s为索引。

Slice2freeze

--dict Demo::IndexedStruct1Class1Map,Demo::Struct1,Demo::Class1         --dict-index Demo::IndexedStruct1Class1Map,s,case-sensitive         BenchTypes Test.ice

编译后代码中会自动生成findByS(string &),在程序中可以按以下方式直接调用：

IndexedStruct1Struct2Map& m=...;

IndexedStruct1Struct2Map::iterator p = m.findByS(os.str());

6.5  Ice Box

IceBox是一个易用的ice应用服务的框架。它使用通用的IceBox server来取代一般的Ice server。IceBox server通过属性来配置，可以被远程管理。

[https://blog.csdn.net/kaede999/article/details/23257985](https://blog.csdn.net/kaede999/article/details/23257985)

七、ice使用过程遇到的问题

1 设置代理超时时间ice_timeout

   ICE的每个连接都有两个超时时间：ice_timeout、ice_connectiontimeout，分别对应消息的超时时间和连接建立

   的超时时间，可以通过在代理上调用上述方法来设置超时，也可以通过属性Ice.Override.Timeout、Ice.Override.ConnectTimeout

   来强制改变超时时间。

示例如下：

MyAdapter.Endpoints=tcp –p 9999 –t 5000

base = __ice_runtime.stringToProxy(settings.CONF_DATA_CENTER_SERVERS).ice_timeout(500) //获得代理的超时时间

 在客户端，我们使用对象代理进行远程调用，就如它们就在本地一样。但有时，网络问题还是要考虑的，于是Ice的对象代理提供了几个包装方法，以支持一些网络特性：

ice_timeout方法，声明为：Ice::ObjectPrx ice_timeout(int) const;返回一个超时代理，当在指定的时间（单位毫秒）内没有得到服务器端响应时，操作终止并抛出Ice::TimeoutException异常。

示例代码：

Filesystem::FilePrx myFile = ...;

FileSystem::FilePrx timeoutFile = FileSystem::FilePrx::uncheckedCast(myFile->ice_timeout(5000));

try {

    Lines text = timeoutFile->read(); // Read with timeout

} catch(const Ice::TimeoutException &) {

    cerr << "invocation timed out" << endl;

}

Lines text = myFile->read(); // Read without timeout

1设置定位服务缓存超时时间

Ice.Default.LocatorCacheTimeout  

对于定位服务，可以缓存 代理对应的端点地址，这样可以减少定位请求。

Description

Specifies the default locator cache timeout for indirect proxies. If num is set to a value larger than zero, locator cache entries older than num seconds are ignored. If set to 0, the locator cache is not used. If set to -1, locator cache entries do not expire.

Once a cache entry has expired, the Ice run time performs a new locate request to refresh the cache before sending the next invocation; therefore, the invocation is delayed until the run time has refreshed the entry. If you set Ice.BackgroundLocatorCacheUpdates to a non-zero value, the lookup to refresh the cache is still performed but happens in the background; this avoids the delay for the first invocation that follows expiry of a cache entry.

 Ice.BackgroundLocatorCacheUpdates 

2  报异常IceUtil::NullHandleException:

Null Smart Pointers

A null smart pointer contains a null C++ pointer to its underlying instance. This means that if you attempt to dereference a null smart pointer, you get an IceUtil::NullHandleException:

C++

TimeOfDayPtr tod;               // Construct null handle

try {

    tod->minute = 0;            // Dereference null handle

    assert(false);              // Cannot get here

} catch (const IceUtil::NullHandleException&) {

    ; // OK, expected

} catch (...) {

    assert(false);              // Must get NullHandleException

}

Default constructor

You can default-construct a proxy handle. The default constructor creates a proxy that points nowhere (that is, points at no object at all). If you invoke an operation on such a null proxy, you get an IceUtil::NullHandleException:

C++

try {

    SimplePrx s;        // Default-constructed proxy

    s->op();            // Call via nil proxy

    assert(0);          // Can't get here

} catch (const IceUtil::NullHandleException&) {

    cout << "As expected, got a NullHandleException" << endl;

}

3. 客户端自动重试

配置重试  Configuring Automatic Retries

Ice.RetryIntervals属性用来配置重试行为。它的取值是一组被空格分开的整数。这组整数代表重试多次，单位是毫秒。如果这个属性没有被定义，那么默认的行为就是立刻重试。

相当于下边的定义

Ice.RetryIntervals=0

你也可以把重试规则定义的更详细一点，如下

Ice.RetryIntervals=0 100 500 1000

这种配置下：ICE发生错误会立刻重试；如果再失败，就会等100毫秒后重试；然后是500毫秒；然后是1000毫秒后，进行最后一次重试。

某些情况下，你需要禁用重试。比如，程序想自己控制重试逻辑的时候。将值配置为-1，就可以禁止自动重试，如下。

Ice.RetryIntervals=-1

重试日志 Retry Logging

为了监视重试行为，可以配置Ice.Trace.Retry属性为1，这样ICE会把重试记入日志。

配置为2，记录更多信息。

超时和自动重试  Timeouts and Automatic Retries

开发人员在测试超时时可能会发现一个奇怪的现象，连接超时异常发生了两次。这种情况是因为重试导致的。

举个例子，假如ICE配置连接超时时间为10秒，并且设置了重试。如果发送请求时，连接超时，这个时候ICE会立刻发起重试，之后如果再失败，就会又引起一次TimeoutException。

在应用的角度来看，就是调用后经过了大约20秒，然后失败。

因此，可以得出最坏情况的时间延迟为

T = t * (N + 1) + D

t为连接超时时间，N为重试次数，D为重试间隔之和。

举个例子

Ice.RetryIntervals=0 10000 20000 30000

使这个配置，超时时间配置为10秒，它的最坏超时时间为

10*（4+1）+10+2+30=110秒。

连接和自动重试  Connections and Automatic Retries

重试和连接是否存在紧密相关。这一部分介绍了哪些错误会引起ICE关闭连接，和更多连接影响重试的细节。

当发生致命错误的时候，ICE会自动关闭连接。比如，连接超时，还有下面的错误。

当进行数据收发时，发生socket失败。

接收到一个未知格式的消息。

当将一个请求分发给JAVA Servant时，发生OutOfMemoryError和AssertionError异常。

当这些错误发生时，ICE会关闭链接。在这条链接上发出的其他请求将会失败，并且可能引起重试。

连接状态  Connection Status

连接的状态也会影响重试。当ICE因为错误关闭连接（如上个章节所述），或者无法建立连接时，ICE必须取得一个新连接才能重试。

有一点一定要明白，就算连接没有问题，ICE也未必会在原来的终端(endpoint)上发起重试。重试还受以下原则控制：

1.代理（proxy）是否缓存了他的连接。

2.代理是否有包含多个终端。

3.终端是否有多个代理。

4.代理配置的终端选择类型。

ice同步调用和异步调用实例代码[下载](https://download.csdn.net/download/byxdaz/11094490)

