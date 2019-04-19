# STM32 ST-LINK Utility介绍、下载、安装、使用方法----读取Flash里面程序 - xqhrs232的专栏 - CSDN博客
2018年04月03日 16:21:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1414
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[https://blog.csdn.net/ybhuangfugui/article/details/52597133](https://blog.csdn.net/ybhuangfugui/article/details/52597133)
相关文章
1、**stlink usb communication error情况----[https://blog.csdn.net/endless_fighting/article/details/52603998](https://blog.csdn.net/endless_fighting/article/details/52603998)**
Ⅰ、写在前面
本文讲述的内容是STM32 ST-LINK Utility介绍、下载、安装、使用方法，如需要了解更多关于STM32相关的文章，可以到我博客，或微信公众号查看并下载。
STM32 ST-LINK Utility这个软件工具其实主要就是配套“ST-LINK”这个下载工具一起使用的上位机软件。因此使用STM32 ST-LINK Utility上位机软件需要有一个ST-LINK工具才行。它的功能和J-Link对应的工具类似，用于烧写代码。
STM32 ST-LINK Utility工具在产品开发过程中测试一些其他版本的代码，可以直接下载hex，而不用打开工程再编译去下载。 当你开发完一个STM32产品，需要量产的时候，就可以用这个工具直接下载hex代码，对代码加密（读保护）。
本文内容已经整理成PDF文件，提供给大家下载：
[http://pan.baidu.com/s/1slpMOVj](http://pan.baidu.com/s/1slpMOVj)
更多关于STM32的文章，可以进入我博客查看：
[http://blog.csdn.net/ybhuangfugui](http://blog.csdn.net/ybhuangfugui)
作者：strongerHuang
本文版权所有，未经允许，禁止用于其它商业用途！！！
关于本文的更多详情请往下看。
# Ⅱ、STM32 ST-LINK Utility介绍
## 1.关于STM32 ST-LINK Utility
STM32 ST-LINK Utility软件主要的功能就是量产（批量下载代码的工具）。它也是比较实用的一个工具，当我们需要查看芯片FLASH数据时，可以很快定位查找到想要的数据（前提是没有添加保护）。
STM32 ST-LINK Utility软件包含ST-Link驱动。若你安装了STM32 ST-LINK Utility软件，你的ST-Link就不用单独安装驱动了，可以直接使用（比如Keil、IAR在线调试、下载等）。
STM32 ST-LINK Utility软件除了可以快速读取FLASH数据外，还可快速读取STM32芯片型号、ID、版本等信息。
![](https://img-blog.csdn.net/20160920161507828)
## 2.Version 4.0.0更新说明
STM32 ST-LINK Utility V 4.0.0版本在V3.9.0基础上主要更新的内容：
A、除了使用Flash界面对选项字节寄存器编程外，还可以使用（binary/hex/srec）文件对选项字编程。
B、支持Windows10操作系统。
# Ⅲ、下载
STM32 ST-LINK Utility这个软件可以到官网下载，也可以到我百度网盘下载。下面章节讲述的安装、注册也是从官方下载的软件，为了方便大家下载，我将其上传至百度网盘（和官网的一样）。
## 1.官方下载
目前（2016年10月）STM32 ST-LINK Utility官方最新版本是V4.0.0。官方下载需要注册ST账号，ST账号登陆成功才能下载，若觉得麻烦可以在我百度网盘下载（和官网相同的文件）。
官方下载地址：
[http://www.st.com/content/st_com/en/products/embedded-software/development-tool-software/stsw-link004.html](http://www.st.com/content/st_com/en/products/embedded-software/development-tool-software/stsw-link004.html)
【点击网页中“Download”即可下载。（如果没有登录账号会提示“注册”或“登录”）】
![](https://img-blog.csdn.net/20160920161525859)
## 2.百度网盘下载
为了方便大家下载，我在百度网盘提供下载的版本也是上面官网下载，上传至百度网盘【定期更新至最新版本】。里面包含STM32 ST-LINK Utility软件、资料、老版本软件等。
百度网盘地址：[http://pan.baidu.com/s/1c16mhsk](http://pan.baidu.com/s/1c16mhsk)
注意：由于许多网盘近年来受到影响都相继停止服务或关闭了，如果网盘链接失效，请到官网下载，也可以微信公众号查看更新链接，或微信联系作者。
# Ⅳ、安装
STM32 ST-LINK Utility集成开发环境的安装比较简单（基本上就是一路Next下去）。还是按照常规安装教程（截图）讲述一下吧，以上面下载的“STM32 ST-LINK Utility v4.0.0 setup.exe”软件为例讲述。
操作系统要求：
Windows XP、Windows 7、Windows 10
256M内存及以上、30M硬盘以上
1.解压软件，双击“STM32 ST-LINK Utility v4.0.0.exe”，进入准备安装（解压）过程。
![](https://img-blog.csdn.net/20160920161539460)
2.进入安装向导，点击“Next”。
![](https://img-blog.csdn.net/20160920161548125)
3.同意许可，点击“Yes”。
![](https://img-blog.csdn.net/20160920161600460)
4.选择安装路径（这里默认），点击“Next”。
![](https://img-blog.csdn.net/20160920161616789)
5.进入安装过程，不到一分钟时间。
![](https://img-blog.csdn.net/20160920161626570)
6.安装最后提示“安装驱动”，点击“下一步”，最后点击“完成”安装完成。
![](https://img-blog.csdn.net/20160920161636617)
![](https://img-blog.csdn.net/20160920161643344)
7.点击“Finish”完成上位机软件及ST-LINK驱动的安装。
![](https://img-blog.csdn.net/20160920161651289)
8.查看版本：打开软件-> Help - > About， 可以看见版本是更新了的。
![](https://img-blog.csdn.net/20160920161658039)
# Ⅴ、基本使用方法
## 1.Keil和IAR生成hex的配置
STM32 ST-LINK Utility软件主要是下载程序（可执行hex文件），因此需要编程工具生成hex文件才行，下面将讲述常用工具Keil和IAR生成hex的配置。
A.Keil生成hex配置
Project ->Options for Target -> Output 勾选上“Create HEX File”（如下图）
![](https://img-blog.csdn.net/20160920161713938)
B.IAR生成hex配置
Project ->Options -> Output Coverter -> 勾选上“Generate additional output”、选择输出格式为“Intel extended”，再次设置输出名称就配置好了（如下图）。
![](https://img-blog.csdn.net/20160920161725063)
## 2.读取STM32内部FLASH及芯片信息
使用ST-Link连接硬件（STM32芯片），打开STM32 ST-LINK Utility软件，连接芯片：Tarage  -> connect或直接点击连接快捷按钮（如下图）。
【读取FLASH信息的前提是没有添加读保护】
点击“连接按钮”之前可以设置读取FLASH的起始地址、读取长度和数据显示的宽度。
![](https://img-blog.csdn.net/20160920161759141)
## 3.打开程序（hex）
在上一步连接好芯片，并正确识别芯片之后，打开需要下载的程序（hex）文件。打开hex文件可以从菜单栏（File -> Open File）打开，也可以直接讲hex文件拖动到FLASH区域（就像从电脑复制文件到U盘一样）。
![](https://img-blog.csdn.net/20160920161809181)
## 4.下载程序（hex）
在上一步打开hex文件完成之后，点击“下载”（可以Taraget -> Program，也可以直接点击下载快捷按钮，如下图）
![](https://img-blog.csdn.net/20160920161820947)
弹出信息确认窗口，如hex文件路径、验证方式等，确认信息无误后点击“Start”开始下载程序。
【如：我将可执行hex文件命名为“ExecutableFile.hex”，位于桌面】
![](https://img-blog.csdn.net/20160920161828501)
下载过程时间长短与程序大小有关，一般都很快，出现“Verification...OK”，说明下载成功。
![](https://img-blog.csdn.net/20160920161838135)
# Ⅵ、说明
STM32 ST-LINK Utility这个软件工具是ST官方推荐使用的一款下载工具（配套ST-Link下载器），是一款免费的软件，也是我们STM32学习者、开发者值得使用的一款工具。
以上总结仅供参考，若有不对之处，敬请谅解。
# Ⅶ、最后
我的博客：[http://blog.csdn.net/ybhuangfugui](http://blog.csdn.net/ybhuangfugui)
微信公众号：EmbeddDeveloper
本着免费分享的原则，方便大家业余利用手机学习知识，定期在微信公众号分享相关知识。如果觉得文章的内容对你有用，又想了解更多相关的文章，请用微信搜索“EmbeddDeveloper” 或者扫描下面二维码、关注，将有更多精彩内容等着你。
加入文库VIPSTM32 读保护功能和清除读保护功能设置
