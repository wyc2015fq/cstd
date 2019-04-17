# Zend studio 入门 - 战斗蜗牛的专栏 - CSDN博客





2011年06月13日 10:34:00[vbskj](https://me.csdn.net/vbskj)阅读数：6228








# Zend Studio-8.0.0中文汉化教程及入门教程					收藏






之前一直[使用Zend Studio7调试PHP](http://www.leapsoul.cn/?p=791)
，
最近打算开始使用Zend Studio8，昨天装了Zend Studio 8后打算使用Zend Studio中文版，网上找了下在线的Zend 
Studio 8 汉化包对Zend Studio8进行汉化，感觉非常方便，这对于喜欢使用Zend 
Studio中文版的朋友来说真是福音，现分享下Zend Studio 8 汉化的具体过程。

**Zend Studio 8 汉化第一步：安装Zend Studio 8**

　　在进行Zend Studio 8 汉化之前，当然首先需要下载安装Zend Studio 8，Zend Studio 8安装文件的下载地址如下：[Zend Studio 8官方下载地址](http://www.zend.com/en/products/studio/downloads)
，注意由于是32位WIN7系统，请下载正确的ZendStudio-8.0.0.msi文件。下载完毕后，只要双击安装程序安装即可，并不需要Zend Studio注册码即可使用Zend Studio 8。

**Zend Studio 8 汉化第二步：在线下载安装Zend Studio 8汉化包**

　　完成Zend Studio 8安装后，打开Zend Studio 8，找到菜单栏中的Help菜单，点击后选择install new software会弹出install窗口，然后点击Add按钮，如图

![添加Zend Studio 8 汉化包的更新源](http://www.leapsoul.cn/wp-content/uploads/2010/12/zend-studio8-hanhua-add.gif)
添加Zend Studio 8 汉化包的更新源


　　在Name中你可以随便数据，我输入了我PHP网站开发博客的网址*www.leapsoul.cn*
，在Locations中需要输入Zend Studio 8 汉化包的在线更新源，地址为：*http://download.eclipse.org/technology/babel/update-site/R0.8.0/helios*
，最后点击ok。

　　之后在Name和Version中会出现Pending字样，如图

![在线实时下载Zend Studio 8 汉化包](http://www.leapsoul.cn/wp-content/uploads/2010/12/zend-studio8-hanhua-pending.gif)
在线实时下载Zend Studio 8 汉化包


　　根据网速的快慢，Pending字样会持续几分钟，需要耐心等待，Zend Studio 8 汉化包下载完毕后会出现如图

![选择Zend Studio中文简体汉化包](http://www.leapsoul.cn/wp-content/uploads/2010/12/zend-studio8-hanhua-language.gif)
选择Zend Studio中文简体汉化包


　　如图所示，选择Chinese (Simplified)选项，即Zend Studio 8 中文简体汉化包，最后只要不停的点击Next按钮即可。

　　Zend Studio 8 汉化安装界面如图

![安装Zend Studio 8 汉化包界面](http://www.leapsoul.cn/wp-content/uploads/2010/12/zend-studio8-hanhua-install.gif)
安装Zend Studio 8 汉化包界面


*Zend Studio 8 汉化安装提示*
：在Zend Studio 8 汉化的过程中会出现一个错误提示，只要忽略即可，安装完毕后，会出现restart now按钮，点击，重启Zend Studio 8






**PHP****开发工具****Zend Studio7****入门使用教程**








　对于PHP网站开发者来说，选择一款好用的PHP开发工具是很重要的，目前比较有名的PHP开发工具有zend 
studio、Eclipse、Editplus等，当初我刚入门学习PHP的时候，使用的PHP开发工具是Editplus，后来才慢慢过渡到更多使用
zend studio，相对来说Editplus是一个轻量级的PHP开发工具，适合PHP入门学习者使用，zend 
studio功能更强大，但你要说哪个是最好的PHP开发工具，那可真说不清道不明了，只有最合适，没有最好，对于有一定PHP基础知识的童鞋我更推荐使
用zend studio，对于PHP入门学习者，推荐使用Editplus啦。闲话少说，今天和大家以教程形式分享如何快速入门使用zend 
studio7。

![PHP开发工具zend studio7入门使用教程](http://www.leapsoul.cn/wp-content/uploads/2010/05/zend-studio-tutorial.gif)
PHP开发工具zend studio7入门使用教程


**Zend Studio7入门使用教程概述**

　　PHP开发工具zend studio7的使用教程手册主要涵盖以下几个方面

　　1、下载zend studio7

　　2、zend studio7工作区布局介绍及使用方法

　　3、如何使用zend studio7建立项目

　　4、使用zend studio7调试PHP的方法

　　5、配置zend studio7 PHP Manual(PHP使用手册)的方法

**下载PHP开发工具zend studio7**

　　我是通过360安全卫士下载PHP开发工具zend studio7的，你也可以通过[zend官网](http://www.zend.com/en/downloads/)
下载zend studio7，在下载完安装过程中你需要有zend studio7的安装序列号，网上很多，搜索一个使用就可以了。

**zend studio7工作区布局及使用方法**

　　打开PHP开发工具zend studio7，工作区布局映入眼帘，主要分为三列，左边为建立的PHP项目，涵盖项目的各种文件，中间为PHP程序代码编写区域，右侧为当前PHP文件函数列表区域及PHP，PHP Debug等模式切换区域，可用来调试PHP代码。

　　在使用zend studio7时左侧区域非常有用，即可以新建项目，也可以查看PHP自带的各种函数以及项目本身编写的各种类、函数、常量等，只要双击相应的类、函数、常量即可进入对应的PHP文件，如图

![列出相应项目对应的类、函数、常量等](http://www.leapsoul.cn/wp-content/uploads/2010/05/zend-studio-pro-outline-view.gif)
列出相应项目对应的类、函数、常量等


　　这里需要注意，当你使用zend studio7想要列出更多的信息时，请点击左下角的一个小图标（show view as a fast 
view），选择需要列出的面板，再将其拖动到左侧部分即可，比如PHP Functions。另外需要注意，左侧project outline 
view与右侧outline面板的区别在于，outline列出的是当前打开PHP文件中的常量、函数、变量等信息，而project outline
 view面板顾名思义列出的项目所有的常量、类、函数等信息。

**使用zend studio7建立项目**

　　使用PHP开发工具zend studio7进行PHP网站开发，就需要建立(导入)相应的项目，方法如下：右键左侧Workspace，选择New | PHP Project菜单，会出现新建项目界面

![使用zend studio7新建项目的方法](http://www.leapsoul.cn/wp-content/uploads/2010/05/zend-studio-new-php-project.gif)
使用zend studio7新建项目的方法


　　一般对于zend studio的入门使用者来说，你只要进行如下操作即可：

　　1、填写项目名称(Project Name)

　　2、选择项目所在的具体目录地址(existing source)

　　3、选择PHP运行环境对应的PHP版本(PHP Version)[如何配置PHP运行环境](http://www.leapsoul.cn/?tag=php%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE)
？

**注意**
：在填写项目名称时注意和项目所在目录名称一致，因为在使用zend 
studio调试PHP程序时URL中的目录是以项目名称为主的，如果不一致，每次都需要修改URL地址，很繁琐，我一开始就是因为没注意这个问题，程序
在test目录下，调试时由于URL为http://localhost/phptest/phpinfo.php，导致调试出错。

　　在使用PHP开发工具zend 
studio7新建项目后，有时我们会用到JS，比如Jquery等，这就需要导入它。默认Javascript库是自动导入的，如果你不需要
Javascript库，你可以通过选中项目，右键选择Configure|Add Javascript Support或Remove 
Javascript Support来操作。那么如何添加新的Javascript库呢？你只要选择Project|Properties菜单，如图

![使用zend studio7添加新的Javascript Libraries](http://www.leapsoul.cn/wp-content/uploads/2010/05/zend-studio-javascript-libraries.gif)
使用zend studio7添加新的Javascript Libraries


　　点击图右侧的Add Library Folder，指定具体的JS库地址即可。注意如果你进行了之前的Configure|Remove Javascript Support操作，你就不能添加其他的JS库。

**使用zend studio7调试PHP的方法**

　　使用zend studio7进行PHP调试，有很多种方法，zend studio7 
debug就有好几种，比如zenddebug，xdebug等，Debug的方式有三种：PHP Script、PHP Web 
Page、PHPUnit Test。由于本文是zend studio7的入门使用教程，我介绍一种最简单的使用zend 
studio7调试PHP程序代码的方法-PHP Web Page调试，你在使用zend studio7调试时，只要选择Run|Debug 
As|PHP Web Page即可转入PHP 
Debug模式，会出现Web游览器模式，显示PHP程序执行的结果或出错信息，当然你也可以设置断点进行调试。当你想回到PHP代码编写模式，你只要选
择右上角小方块，选择PHP模式即可。更多的调试方法你可以自己尝试体会。

**配置zend studio7 PHP Manual(PHP使用手册)的方法**

　　对于PHP入门学习者来说，PHP帮助使用手册必不可少，对了解函数的使用方法非常有帮助，在zend 
studio7中可以导入PHP帮助使用手册，zend 
studio可以导入三种PHP帮助使用手册，即网页、CHM、HTML三种格式，那么如何使用zend studio7指定PHP帮助使用手册呢？

　　选择Project|Zend Tool|options菜单，出现如下界面

![使用zend studio7添加PHP帮助使用手册](http://www.leapsoul.cn/wp-content/uploads/2010/05/zend-studio-php-manual.gif)
使用zend studio7添加PHP帮助使用手册


　　如图所示zend 
studio7默认指定了以网页方式访问外网的PHP帮助使用手册，这需要你能时刻访问外网，除此之外，你可以指定CHM或者HTML格式的PHP帮助使
用手册文档，注意选对File Extension。添加完新的后，只要点击Default按钮即告完成。

　　之后只要你点击对应PHP系统自带的函数，右键选择Open PHP Manual菜单即可。非常神奇哦。哈哈。

　　OK，至此PHP开发工具zend studio7快速入门使用教程就介绍完了，更多的zend studio使用心得更需要童鞋自己深入发掘，找到自己最得心应手的使用方法，还是那句话没有最好的PHP开发工具，只有最合适的PHP开发工具。




