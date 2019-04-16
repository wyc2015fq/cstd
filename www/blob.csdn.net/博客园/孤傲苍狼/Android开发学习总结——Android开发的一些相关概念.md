# Android开发学习总结——Android开发的一些相关概念 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [Android开发学习总结——Android开发的一些相关概念](https://www.cnblogs.com/xdp-gacl/p/4324503.html)



## 一、什么是3G、4G

　　1995年问世的第一代模拟制式手机（1G）只能进行语音通话。

　　1996到1997年出现的第二代GSM、CDMA等数字制式手机（2G）便增加了接收数据的功能

　　3G指的是第三代移动通信技术（3rd - Generation），速率一般在**几百****Kbps**，较之前的2G和2.5G在数据传输速度上有很大提升。3G与2G的主要区别是在传输声音和数据的速度上的提升

　　4G指的是第四代移动通信技术（4th - Generation），速度可达到**100Mbps**以上，几乎可以满足人们的所有传输数据的需求。

　　目前主流的3G技术标准有三种：
- **WCDMA**：全球80%以上的3G网络都是采用此种制式。中国联通运营。186
- **CDMA2000**：目前日韩及北美使用较多。中国电信运营。 189
- **TD-SCDMA**：中国自主知识产权的3G通信技术。中国移动运营。 188

　　目前主流的4G技术为**LTE**，但还没有被广泛应用。

　　通信技术的发展历程：GSM→GPRS→EDGE→WCDMA→HSDPA→HSDPA+→ LTE

## 二、Andorid的相关概念

### 2.1、什么是Android

![](https://images0.cnblogs.com/blog2015/289233/201503/092031391675278.jpg)

   Android本义指“机器人”，Google于2007年11月5日宣布的基于**Linux**平台的开源手机操作系统的名称，官方中文名：安卓 。

   Android系统早期由原名为"Android"的公司开发，谷歌在2005年收购"Android.Inc"后，继续对Android系统开发运营。

   底层Linux内核只提供基本功能，其他的应用软件则由各公司自行开发，大部分程序以**Java**语言编写。

   由于Android系统的开源特性，很多制造商都在生产Android系统的设备，如：摩托罗拉、HTC、三星、索爱、LG、小米、华为、魅族等。

   Android系统除了运行在智能手机上之外，还可以用做平板电脑、电视、汽车等很多设备上。

### 2.2、Android平台架构

　　Android系统的底层建立在Linux系统之上，该平台由操作系统、中间件、用户界面和应用软件4层组成，它采用一种被称为软件叠层(Software Stack)的方式进行构建。这种软件叠层结构使得层与层之间相互分离，明确各层的分工。这种分工保证了层与层之间的低耦合，当下层的层内或层下发生改变时，上层应用程序无须任何改变。

　　Android平台的架构图如下：

![](https://images0.cnblogs.com/blog2015/289233/201503/092035185424798.jpg)

　　从图中可以看出，Android系统是由五大部分组成的。

#### 　　1、应用程序层(APPLICATIONS)

　　　　Android系统将会包含系列的核心应用程序，包括电子邮件客户端，SMS程序、日历、地图、浏览器、联系人等。这些应用程序都是用Java编写的。

#### 　　2、应用程序框架(APPLICATION FRAMEWORK)

　　　　当我们开发Android应用程序时，就是面向底层的应用框架进行的。从这个意义上来看，Android系统上的应用程序时完全平等的，不管是Android系统提供的程序， 还是普通开发者提供的程序，都可以访问Android提供的API框架。

#### 　　3、函数库(LIBRARIES)

　　　　Android包含一套被不同组件所使用的C/C++库的集合。一般来说，Android应用开发者不能直接调用这套C/C++库集，但可以通过它上面的应用程序框架来调用这些库。

#### 　　4、Android运行时(ANDROID RUNTIME)

**Android运行时由两部分组成：Android核心库集合(Core Libraries)和Dalvik虚拟机(Dalvik Virtual Machine)。**其中核心库集提供了Java语言核心库所能使用的绝大部分功能，而虚拟机则负责运行Android应用程序。 每个Android应用程序都运行在单独的Dalvik虚拟机内(即每个Android应用程序对应一条Davlik进程)，Dalvik专门针对同时高效运行多个虚拟机进行优化， 因此Android系统很方便实现对应用程序进行隔离。

　　 　　由于Android应用程序的编程语言是Java，因此有些人会把Dalvik虚拟机和JVM搞混，当实际上二者存在区别：Dalvik并未完全遵守JVM规范，两者也不兼容。 实际上，JVM虚拟机运行的是Java字节码(通常就是.class文件)，但Dalvik运行的是其专有的dex(Dalvik Executable)文件。JVM直接从.class文件或JAR包中 加载字节码然后运行；而Dalvik则无法直接从.class文件或JAR包中加载字节码，它需要通过DX工具将应用程序的所有.class文件编译成.dex文件， Dalvik则运行该.dex文件。

　　　　Dakvik虚拟机非常适合在移动终端上使用，相对于在PC或服务器上运行的虚拟机而言，Dalvik虚拟机不需要很快的CPU计算速度和大量的内存空间，它主要有如下两个特点：
- - 运行专有的.dex文件。专有的.dex文件减少了.class文件中的冗余信息，而且会把所有.class文件整合到一个文件中，从而提高运行性能； 而且DX工具还会对.dex文件进行一些性能的优化。
- 基于寄存器实现。大多数虚拟机(包括JVM)都是基于栈的，而Dalvik虚拟机则是基于寄存器的。一般来说，基于寄存器的虚拟机具有更好的性能表现，但在硬件通用性上略差。


#### 　　5、Linux内核

　　　　Android系统建立在Linux 2.6之上。Linux内核提供了安全性、内存管理、进程管理、网络协议栈和驱动模型等核心系统服务。除此之外，Linux内核也是系统硬件和软件叠层之间的抽象层。

### 2.3、Android进化史

![](https://images0.cnblogs.com/blog2015/289233/201503/092043068246161.png)

### 2.4、各种Android设备

![](https://images0.cnblogs.com/blog2015/289233/201503/092059040118688.png)![](https://images0.cnblogs.com/blog2015/289233/201503/092100051058218.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/092101016204717.png)![](https://images0.cnblogs.com/blog2015/289233/201503/092101547774357.png)

　　一张Android和Apple互相PK的图片，挺有趣的。

![](https://images0.cnblogs.com/blog2015/289233/201503/092047444957224.png)

　　掌握了移动互联网就掌握了将来，谁输在了移动互联网上，谁就输了未来，谁抓住了移动互联网的机会，谁就有了未来成功的门票。









