# Ice框架简介及Vs2013安装Ice 3.7.0步骤及实例 - DoubleLi - 博客园






ICE是什么？

ICE是ZEROC[官网](https://zeroc.com/)的开源通信协议产品，它的全称是：The Internet Communications Engine，翻译为中文是互联网通信引擎，是一个面向对象的中间件，支持C++、Java、C#、VB、Python等，客户和服务器可以用不同的语言，可以运行在不同的操作系统和机器架构上，支持TCP/UDP。使我们能够以最小的代价构建分布式应用程序。ICE使我们专注于应用逻辑的开发，它来处理所有底层的网络接口编程，这样我们就不用去考虑这样的细节：打开网络连接、网络数据传输的序列化与反序列化、连接失败的尝试次数等。

为什么会有ICE？

ICE是分布式应用的一种比较好的解决方案，虽然现在也有一些比较流行的分布式应用解决方案，如微软的.NET（以及原来的DCOM）、CORBA及WEBSERVICE等，但是这些面向对象的中间件都存在一些不足：

.NET是微软产品，只面向WINDOWS系统，而实际的情况是在当前的网络环境下，不同的计算机会运行不同的系统，如LINUX上面就不可能使用.NET；

CORBA虽然在统一标准方面做了很多的工作，但是不同的供应商实现之间还是缺乏互操作性，并且目前还没有一家供应商可以针对所有的异种环境提供所有的实现支持，且CORBA的实现比较复杂，学习及实施的成本都会比较高；

WEB SERVICE最要命的缺点就是他的性能问题，对于要求比较高的行业是很少会考虑WEB SERVICE的。

ICE的产生就是源于.NET、CORBA及WEBSERVICE这些中间件的不足，它可以支持不同的系统，如WINDOWS、LINUX等，也可以支持在多种开发语言上使用，如C++、C、JAVA、RUBY、PYTHON、VB等，服务端可以是上面提到的任何一种语言实现的，客户端也可以根据自己的实际情况选择不同的语言实现，如服务端采用C语言实现，而客户端采用JAVA语言实现，底层的通讯逻辑通过ICE的封装实现，我们只需要关注业务逻辑。

ICE是如何工作的？

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
![ICE应用结构](https://img-blog.csdn.net/20170907215754333?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdmlzdGFzX2Zo/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Slice 
Slice (Specification Language for Ice, Ice规范语言)，用于对象接口定义与实现相分离的基础性抽象机制，可以把定义好的Ice对象映射成特定语言。 
Slice开发过程见下图： 
相同开发环境： 
![相同开发环境](https://img-blog.csdn.net/20170907220116725?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdmlzdGFzX2Zo/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
不同开发环境： 
![这里写图片描述](https://img-blog.csdn.net/20170907220407516?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdmlzdGFzX2Zo/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

*以上内容引用自网络*

安装 
1.下载并安装 ICE.7.0 [MSI下载点击这里](https://zeroc.com/downloads/ice),我们会发现这个版本的文件很小，不到20M，上一个版本都有差不多1G了(为什么这么小，后面说原因)

2.下载ICE-builder用于支持Visual Studio的插件扩展，文件为IceBuilder.vsix，目前只支持2012以上的版本 
vs2010安装扩展方法：官网有说明，暂时还未验证，后面补充，文件为IceBuilder_VS2010.vsix 
联网安装方法：工具->扩展和更新->联机，输入关键字ice查找 ice builder安装即可. 
离线安装可以去微软或Zeroc官网获取 [微软链接地址点击这里](https://marketplace.visualstudio.com/items?itemName=ZeroCInc.IceBuilder)
所有版本安装时直接双击运行即可，安装完成，在“工具->属性->Projects and Solutions->ICE Builder”下面可以看到“ICE Home Directory”的设置，但是3.7.0版本将所有的dll，lib及include已经全部整合进NuGet扩展包，包括所有的第三方库，这也是它为什么这么小的原因，设置后会出现如下提示： 
“invalid ice home directory” 
设置不了？嗯…，可能是微软的插件未和当前的版本同步导致，原因其实就是上面说的，找不到重要的libary和C++包含目录！ 
![这里写图片描述](https://img-blog.csdn.net/20170907222130983?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdmlzdGFzX2Zo/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这里我们不用理会，这个目录主要是重编译源码的时候使用，除非你有特殊定制需求，正常情况下，这些文件官方都已经生成好了 
安装完成后，我们可以创建一个工程，反键可以看到对应菜单项 
![这里写图片描述](https://img-blog.csdn.net/20170907223619012?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdmlzdGFzX2Zo/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
点击“add ice builder to project”即可 
点击”工程->属性->通用属性“可以查看ice映射目录相关设置，若需要可以手动修改，我这里默认generated文件夹下 
![这里写图片描述](https://img-blog.csdn.net/20170907233508264?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdmlzdGFzX2Zo/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3.安装ICE Nuget package,扩展包包含了所有重要lib，dll和include，联网安装方法如下： 
“工具->库程序包管理器->管理解决方案…->联机”，输入关键字ice查找对应的版本即可，我这里使用vs2013，选择”zeroc ice c++ for vs2013” 
![这里写图片描述](https://img-blog.csdn.net/20170907230220081?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdmlzdGFzX2Zo/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
离线使用离线包安装方法如下： 
我们把下载的nupkg格式的包(zeroc.ice.v120.3.7.0.nupkg)复制到”C:\Users\itsvse\AppData\Local\NuGet\Cache“目录 
这个目录如何查找？”工具->包管理器“,点击浏览可以快速定位 
最后，在nuget控制台执行如下命令： 
install-package zeroc.ice.v120 -version 3.7.0 
![这里写图片描述](https://img-blog.csdn.net/20170907231010026?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdmlzdGFzX2Zo/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

或者直接指向包所在的目录 
install-package zeroc.ice.v120 -version 3.7.0 
-source E:\ZeroC\Ice_install_and_documents 
安装完成，重启工程，我们会发现解决方案管理器中会多出一个Slice Files文件夹，这里就是利用slice语言生成c++需要的.h和.cpp文件的地方。 
附1:[Ice 3.7.0 Doucument](https://doc.zeroc.com/display/Ice37)
未完待续…









