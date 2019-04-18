# Linux环境CUDA 4.0入门：安装前的准备 - 长歌行 - CSDN博客





2012年11月29日 17:20:17[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1416









笔者发现大多数CUDA爱好者是以自学为主的，因此这些入门类的文章很受欢迎。除了Windows系统，开发者最常用的就是Linux了。Linux是一种受到广泛关注和支持的操作系统，和Windows相比，Linux具有低成本、高安全性等优势。今天，笔者将向大家分享在Linux环境下CUDA 4.0如何安装和调试。

**　　什么是CUDA?**


　　首先介绍一下CUDA的概念和特点。CUDA是NVIDIA公司推出的一种通用并行计算架构。它包含了CUDA指令集架构(ISA)和GPU的并行计算引擎。使用CUDA架构编程，开发人员可以使用C语言，这是使用最广泛的高级编程语言之一，可以在支持CUDA的处理器的强大性能下运行。


　　开发CUDA架构及其相关软件有以下两个优势：


　　1、提供一个标准编程语言(如C语言)的扩展集合，使一个简单的并行算法得以执行。使用CUDA C语言，程序员可以专注于算法的并行化工作，而不是把时间花在算法的实现上。


　　2、支持异构计算的应用程序同时使用的CPU和GPU。在CPU上运行应用程序的串行部分，在GPU上运行应用程序的并行部分。同样的，CUDA可以逐步应用在现有的应用程序上。CPU和GPU被视为单独的设备，拥有自己的内存空间。该配置使CPU和GPU同时进行计算时，并不会对内存资源进行竞争。


　　支持CUDA的GPU拥有数百个内核，可以共同在数千个计算线程上运行。每个核心共享包括寄存器和存储器在内的资源。芯片上的共享内存允许并行任务在这些内核上共享数据，而不需要通过系统内存总线传递。

**　系统要求**


　　本文将介绍如何在Linux环境下安装CUDA开发工具，并检查其是否正确配置。


　　为了能够在Linux系统上使用CUDA，需要满足以下要求：


　　1、系统具备支持CUDA的GPU;


　　2、已安装设备驱动;


　　3、支持GCC(GNU Compiler Collection，GNU编译器套装)编译器和工具链的Linux系统版本;


　　4、CUDA软件(可以从http://www.nvidia.com/cuda免费下载);


　　本文的目的是让读者熟悉Linux环境和C程序命令行的编译，针对没有任何CUDA或者并行计算的基础知识的初学者，但本文只针对运行在X Windows系统的CUDA 4.0安装过程。其中，X Window系统(X Window System，也常称为X11或X)是一种以位图方式显示的软件窗口系统，是UNIX、类UNIX、以及OpenVMS等操作系统所一致适用的标准化软件工具包及显示架构的运作协议。简言之，X Windows系统就是UNIX和Linux系统的图形用户界面系统。今天我们主要针对Linux的CUDA
 4.0的安装过程进行介绍，当然其他的X Windows系统也部分适用。


　　需要注意的是本文中的一些命令可能需要用户具有superuser权限。对于大多数Linux发行版来说，用户需要以root身份登录。对于已安装 sudo程序包的系统，所有必备的命令都需要使用sudo前缀。当正确操作出现问题时，需要考虑安装过程中的用户权限问题。

**　　安装前的准备工作**


　　在Linux系统上安装CUDA开发工具包括以下四个简单的步骤：


　　1、验证系统是否具有支持CUDA的GPU;


　　2、下载NVIDIA驱动以及CUDA软件;




3、安装NVIDIA驱动;


　　4、安装CUDA软件。


　　通过编译和运行CUDA软件的示例程序测试安装情况，验证的硬件和软件是否运行正常，并且保证其连接顺畅。


　　一、验证系统是否具有支持CUDA的GPU


　　目前大多数NVIDIA的GPU产品都支持CUDA技术，主要包括以下产品：


　　1、NVIDIA GeForce 8、9、200、400和500系列的GPU;


　　2、NVIDIA Tesla GPU计算解决方案;


　　3、绝大多数NVIDIA Quadro产品。


　　NVIDIA CUDA的官方网站上可以找到最新版支持CUDA的GPU名单，详情请参考NVDIA官方网站：http://www.nvidia.com/object/cuda_gpus.html。


　　在CUDA Toolkit的发行说明里也包含一个支持CUDA的产品列表。


　　为了验证系统使用的视频适配器并查找它的型号，需要查询Linux发行版的系统属性(System Properties)，或者在命令行输入以下内容：


　　lspci | grep -i nvidia


　　如果没有看到任何设置，需要更新Linux维护的PCI硬件数据库，在命令行中输入update-pciids(通常在/sbin下找到)并重新运行之前的lspci命令。


　　二、验证Linux系统版本是否支持CUDA


　　CUDA开发工具只支持一些特定的Linux发行版，如Fedora 13、RedHat Enterprise Linux 4.8/5.5/6.0、Ubuntu Linux 10.10等等，这些版本都列在CUDA Toolkit发行说明里。


　　想要确定正在运行系统的发行版和版本号，需要在命令行中键入以下内容：


　　uname -m && cat /etc/*release


　　会出现类似于下面的输出内容，更改为特定系统：


　　i386


　　Red Hat Enterprise Linux WS release 4 (Nahant Update 6)


　　i386表明这是一个32位系统。如果是在64位系统上运行64位模式，这行信息通常会显示为：x86_64。第二行显示的是操作系统的版本号。


　　三、验证GCC是否正确安装


　　GCC(GNU Compiler Collection，GNU编译器套装)是一套由 GNU 开发的编程语言编译器。它是一套以GPL及LGPL许可证所发行的自由软件，也是GNU计划的关键部分，亦是自由的类Unix及苹果电脑Mac OS X操作系统的标准编译器。GCC原名为GNU C语言编译器，因为它原本只能处理C语言。GCC很快地扩展，变得可处理C++。之后也变得可处理Fortran、Pascal、Objective- C、Java, 以及Ada与其他语言。


　　GCC编译器和工具链的安装通常作为Linux安装的一部分，并且大多数GCC版本安装在支持的Linux版本上才能正常工作。


　　验证安装在系统中的GCC版本，需要在命令行中键入如下内容：


　　gcc --version


　　如果显示错误信息，则需要在Linux发行版中安装“开发工具”，或者从网上获得GCC版本以及相匹配的工具链的版本。


　　四、下载NVIDIA驱动和CUDA软件


　　一旦证实拥有支持的NVIDIA处理器和支持的Linux版本，则需要确保拥有最新版本的NVIDIA驱动程序。CUDA Toolkit发行说明中指定了所需最低版本的NVIDIA驱动。


　　在许多发行版中，驱动程序版本号可以在图形界面菜单下的应用程序(Applications)→系统工具(System Tools)→NVIDIA X服务器设置(NVIDIA X Server Settings)中找到，或者可以在命令行运行以下内容：


/usr/bin/nvidia-settings


　　运行CUDA程序需要以下CUDA软件的支持：


　　1、CUDA Toolkit


　　CUDA Toolkit包含编译和创建CUDA应用的工具，以及编译驱动器。它包括工具(tools)、库(libraries)、头文件(header files)和其他资源。


　　2、GPU Computing SDK


　　GPU Computing SDK包括示例项目，可以提供创建CUDA程序的源代码和其他资源。


　　NVIDIA驱动程序和CUDA软件提供免费下载，官方下载地址为： http://www.nvidia.com/object/cuda_get.html。


　　选择正在使用的Linux发行版，点击搜索按钮，下载NVIDIA驱动程序。保存在本地系统上的驱动程序文件夹中。同样的方法，下载并保存GPU Computing SDK和CUDA Toolkit。

**这篇入门级文章让大家开始对CUDA有了初步的了解，已经我们该在安装CUDA前做好哪些准备工作，接下来，我们将继续为大家介绍究竟如何安装CUDA？**

**相关文章列表：**

[Linux环境CUDA 4.0入门：安装前的准备](http://www.54master.com/html/ruanjianshebei/fuwuqi/2011/1011/6571.html)

[Linux环境CUDA 4.0入门：安装步骤详解](http://www.54master.com/html/ruanjianshebei/fuwuqi/2011/1011/6572.html)

[Linux环境CUDA 4.0入门：验证安装](http://www.54master.com/html/ruanjianshebei/fuwuqi/2011/1011/6573.html)




