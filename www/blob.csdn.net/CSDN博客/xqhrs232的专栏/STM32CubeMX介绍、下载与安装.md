# STM32CubeMX介绍、下载与安装 - xqhrs232的专栏 - CSDN博客
2017年08月31日 11:51:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：389
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[http://blog.csdn.net/ybhuangfugui/article/details/52225736](http://blog.csdn.net/ybhuangfugui/article/details/52225736)
相关文章
1、STM32CubeMX系列教程----[http://www.waveshare.net/study/article-629-1.html](http://www.waveshare.net/study/article-629-1.html)
2、STM32CubeMX使用说明----[https://wenku.baidu.com/view/3bdd381b86c24028915f804d2b160b4e767f812b.html](https://wenku.baidu.com/view/3bdd381b86c24028915f804d2b160b4e767f812b.html)
3、STM32CubeMX----[http://www.st.com/en/development-tools/stm32cubemx.html](http://www.st.com/en/development-tools/stm32cubemx.html)
4、STM32CubeF0 ----[http://www.st.com/content/st_com/en/products/embedded-software/mcus-embedded-software/stm32-embedded-software/stm32cube-embedded-software/stm32cubef0.html](http://www.st.com/content/st_com/en/products/embedded-software/mcus-embedded-software/stm32-embedded-software/stm32cube-embedded-software/stm32cubef0.html)
# **Ⅰ、写在前面**
相信很多人都知道STM32CubeMX这个工具，也是近年来开发STM32比较流行的一个工具。这个工具从两年前第一版到现在已经有多个版本了，功能也从简单越来越强大了。
STM32CubeMX是一个配置STM32代码的工具，它把很多东西封装的比较好，硬件抽象层、中间层、示例代码等。
现在ST公司升级和维护的库主要就是**STM32CubeMX的HAL库和标准外设库**，使用这两种库开发STM32各有各的好处，下面简单描述一下。
如果你是一名刚学STM32的初学者，想要把硬件底层相关的东西搞明白，不建议使用STM32CubeMX这个工具，建议使用之前经典的**标准外设库**来开发（先学习标准外设库，但有必要抽时间了解STM32CubeMX）。标准外设库可以很简单直接跟踪到底层寄存器，而HAL库里面的代码想要跟踪并理解底层很难。
如果你是一名熟练使用标准外设库的人，有必要学习并使用STM32CubeMX来开发程序。
本文内容2016年10月31日更新，且整理成PDF文件，提供给大家下载：
[http://pan.baidu.com/s/1geB5TLp](http://pan.baidu.com/s/1geB5TLp)
更多关于STM32的文章，可以进入我博客查看：
[http://blog.csdn.net/ybhuangfugui](http://blog.csdn.net/ybhuangfugui)
**作者：strongerHuang**
**本文版权所有，未经允许，禁止用于其它商业用途！！！**
# **Ⅱ、STM32CubeMX的介绍**
![](https://img-blog.csdn.net/20161031103050516)
STM32CubeMX是一个图形化的工具，也是配置和初始化C代码生成器（STM32
 configuration and initialization C code generation），也就是自动生成开发初期关于芯片相关的一些初始化代码。
从上图可以看得出，它包含了STM32所有系列的芯片，包含示例和样本（Examples and demos）、中间组件（Middleware
 Components）、硬件抽象层（Hardwaree abstraction layer）。
**STM32CubeMX的特性如下**：
1.直观的选择 STM32 微控制器。
2.微控制器图形化配置：
l 自动处理引脚冲突
l 动态设置确定的时钟树
l 可以动态确定参数设置的外围和中间件模式和初始化
l 功耗预测
3.C代码工程生成器覆盖了STM32 微控制器初始化编译软件，如IAR、KEIL、GCC。
4.可独立使用或作为 Eclipse 插件使用。
STM32CubeMX是ST意法半导体的主动原创工具，它可以减轻开发的时间和费用。STM32CubeMX集成了一个全面的软件平台，支持STM32每一个系列的MCU开发。这个平台包括 STM32Cube
 HAL（一个 STM32 的抽象层集成软件，确保STM32 系列最大的移植性）。再加上兼容的一套中间件（RTOS、USB、TCP/IP 和图形），所有内嵌软件组件附带了全套例程。
**更多关于STM32CubeMX介绍可以进入ST官网查看。**
# **Ⅲ、下载**
**JRE**、**STM32CubeMX工具**和**库**可以到官网下载，也可以在我的百度网盘下载（定期更新）。下面章节讲述的安装也是从官方下载的软件，为了方便大家下载，我将其上传至百度网盘（和官网的一样）。
## **1.Oracle官方下载JRE**
由于STM32CubeMX软件是基于[Java](http://lib.csdn.net/base/java)环境运行的，所以需要安装JRE才能使用，目前（2016年10月31日）JRE最新版本是jre1.8.0_112。
TM32CubeMX要求JRE最低版本是1.7.0_45，如果你电脑已安装JRE版本大于1.7.0_45，则可以不用再下载安装。
[http://www.oracle.com/technetwork/java/javase/downloads/jre8-downloads-2133155.html](http://www.oracle.com/technetwork/java/javase/downloads/jre8-downloads-2133155.html)
接受许可，根据自己电脑系统选择下载（我们以Windows版本为例），如下图：
![](https://img-blog.csdn.net/20161031103112789)
## **2.ST官方下载STM32CubeMX工具、库**
官网可以下载最新版本的软件，目前（2016年10月31日）STM32CubeMX官方最新版本是V4.17.0。
STM32CubeMX工具、库都在同一个网址下：
[http://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-configurators-and-code-generators/stm32cubemx.html](http://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-configurators-and-code-generators/stm32cubemx.html)
**A、STM32CubeMX工具**
【在网页最下面（如下图），点击网页中“Download”即可下载。（如果没有登录账号会提示“注册”或“登录”）】
![](https://img-blog.csdn.net/20161031103123930)
**B、库下载**
【在网页中间偏下（如下图），点击“**EMBEDDED SOFTWARE**”切换到库软件，选择需要下载的库（看你芯片型号），同样也需要登陆账号，若觉得麻烦，可以在下面百度网盘下载】
![](https://img-blog.csdn.net/20161031103130672)
## **3.百度网盘下载**
ST官方下载资料或文档需要ST账号**登陆成功才能下载**，若觉得麻烦可以在我百度网盘下载（定期更新）。为了方便大家下载，我在百度网盘提供下载的版本也是上面官网下载，上传至百度网盘。里面包含**STM32CubeMX软件**和**库**。
JRE：[http://pan.baidu.com/s/1cML4Hg](http://pan.baidu.com/s/1cML4Hg)
STM32CubeMX工具：[http://pan.baidu.com/s/1dFMFwHF](http://pan.baidu.com/s/1dFMFwHF)
STM32CubeMX资料：[http://pan.baidu.com/s/1nvcxQC5](http://pan.baidu.com/s/1nvcxQC5)
**注意**：由于许多网盘近年来受到影响都相继停止服务或关闭了，如果网盘链接失效，请到官网下载，也可以关注[微信](http://lib.csdn.net/base/wechat)公众号查看更新链接，或微信联系作者。
# **Ⅳ、JRE的安装**
STM32CubeMX手册上要求PC安装JRE**:**
The [Java](http://lib.csdn.net/base/java)Run Time Environment 1.7 (version 1.7_45 or newer) must be installed. More information
 on installation requirements and procedure is in the STM32CubeMX User manual (UM1718).
For Eclipse plug-in installation only, install one of the following IDE (on Windows OS):
• Eclipse IDE Juno (4.2)
• Eclipse IDE Luna (4.4)
• Eclipse IDE Kepler (4.3)
• Eclipse IDE Mars (4.5)
JRE安装非常简单，以上面下载的最新版本“jre-8u112-windows-i586.exe”为例讲述一下安装过程:
**1.双击“jre-8u112-windows-i586.exe”（有管理员限制时：右键 -> 以管理员身份运行）,点击“安装”。**
【不更改目标文件夹，则默认安装在：C:\Program Files\Java\jre1.8.0_112】
![](https://img-blog.csdn.net/20161031103158165)
**2.等待安装**
![](https://img-blog.csdn.net/20161031103205587)
**3.安装完成，点击“关闭”**
![](https://img-blog.csdn.net/20161031103211454)
# **Ⅴ、STM32CubeMX的安装**
现在（2016年10月31日）最新版本的STM32CubeMX是V4.17.0，支持安装的系统如下：
Windows XP: 32-bit (x86)
Windows 7: 32-bit (x86), 64-bit (x64)
Windows 8: 32-bit (x86), 64-bit (x64)
[Linux](http://lib.csdn.net/base/linux): 64-bit (x64) (tested on
 RedHat, Ubuntu and Fedora)
Mac OS X Yosemite or newer
安装STM32CubeMX很简单，基本上一路“Next”下去就行了。
以现在最新版本V4.17.0，安装在Windows系统为例来讲述安装过程。
**1.解压下载的文件，双击“SetupSTM32CubeMX-4.17.0.exe”（有管理员限制时：右键 -> 以管理员身份运行）**
**![](https://img-blog.csdn.net/20161031103232744)**
**2.点击“Next”**
![](https://img-blog.csdn.net/20161031103240001)
**3.选择“I accept the terms...”,点击“Next”**
![](https://img-blog.csdn.net/20161031103247423)
**4.选择安装路径（这里修改D盘），点击“Next”**
![](https://img-blog.csdn.net/20161031103256142)
**5.提示创建安装目录，点击“确定”**
![](https://img-blog.csdn.net/20161031103304666)
**6.勾选快捷方式，点击“Next”**
![](https://img-blog.csdn.net/20161031103312166)
**7.等待安装进度完成，点击“Next”**
![](https://img-blog.csdn.net/20161031103318486)
**8.点击“Done”，至此已经完成STM32CubeMX的安装**
![](https://img-blog.csdn.net/20161031103325651)
以上就是安装STM32CubeMX到Windos的过程，关于[linux](http://lib.csdn.net/base/linux)和MacOS系统的安装过程比较简单（这里不再讲述），可以参看官网提供的说明。
**On Linux**：
Make sure you have access rights to the target installation directory, you can run the installation as root (or sudo)? to install STM32CubeMX on shared directories Double click (or launch from
 the console window) on the SetupSTM32CubeMX-4.17.0.linux file
**On MacOS**：
Make sure you have access rights to the target installation directory, you can run the installation as root (or sudo)? to install STM32CubeMX on shared directories Double click (or launch from
 the console window) on the SetupSTM32CubeMX-4.17.0 application
# **Ⅵ、STM32CubeMX库的安装**
STM32CubeMX库（固件支持包）的安装方式有三种：通过STM32CubeMX软件**在线安装**、**导入离线包**、**解压离线包**。
## **1.在线安装**
打开安装好的STM32CubeMX软件，进入库管理界面（Help -> Install New Libraries），会有一个列表，勾选上你要安装的固件库，点击“Install
 Now”直到安装成功。如下图：
![](https://img-blog.csdn.net/20161031103349251)
![](https://img-blog.csdn.net/20161031103406855)
## **2.导入本地离线包**
下载好需要安装的离线包（也就是上面章节下载的库），在库管理界面（Help -> Install New Libraries），点击左下角的“**From Local**” -> 弹出文件选择框（选择下载的离线包文件），点击“Open”,进入解压安装过程，直到安装完成。如下图：
![](https://img-blog.csdn.net/20161031103419731)
![](https://img-blog.csdn.net/20161031103433544)
![](https://img-blog.csdn.net/20161031103442127)
## **3.解压离线包**
解压离线包（库）是最方便的安装方式，这里重要的一点就是需要知道前面安装STM32CubeMX工具指定**库的路径**。
查看库安装路径的方法：
Help -> Updater Settings可以看到库指定路径。
默认路径是 C:/Users/Administrator/STM32Cube/Repository，如下图：
![](https://img-blog.csdn.net/20161031103452221)
解压源文件过程就不说了，**注意文件名需要匹配才行**（打开压缩文件里面的名称），如下图：
![](https://img-blog.csdn.net/20161031103459939)
# **Ⅶ、说明**
STM32CubeMX工具可以说是开发STM32必备的一个工具，如果你还没有了解STM32CubeMX工具，建议花一点时间学习一下这个工具，它或许是今后开发STM32的主流工具。
以上内容仅供参考，若有不对之处，敬请谅解。
# **Ⅷ、最后**
我的博客：[http://blog.csdn.net/ybhuangfugui](http://blog.csdn.net/ybhuangfugui)
微信公众号：EmbeddDeveloper
本着免费分享的原则，方便大家业余利用手机学习知识，定期在微信公众号分享相关知识。如果觉得文章的内容对你有用，又想了解更多相关的文章，请用微信搜索“**EmbeddDeveloper**” 或者扫描下面二维码、关注，将有更多精彩内容等着你。

