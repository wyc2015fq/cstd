# Android开发学习总结(一)——搭建最新版本的Android开发环境 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [Android开发学习总结(一)——搭建最新版本的Android开发环境](https://www.cnblogs.com/xdp-gacl/p/4322165.html)



　　最近由于工作中要负责开发一款Android的App，之前都是做JavaWeb的开发，Android开发虽然有所了解，但是一直没有搭建开发环境去学习，Android的更新速度比较快了，Android1.0是2008年发布的，截止到目前为止Android已经更新Android5.0.1，学习Android开发的第一步就是搭建Android的开发环境，博客园里面有不少人也写了关于如何搭建Android开发环境的文章，我也看了一下，但是感觉都比较旧了，对照着做不一定能够搭建成功，但是有些搭建步骤是还是可以借鉴的！这篇文章就是我借鉴了一些园友们的文章再加上自己亲自实践过之后一步步整理出来的！唉，由于天朝对Google的全面封杀，导致Google的很多产品都用不了，都需要通过FQ才能够正常使用，所以在国内要是想从事Android的开发，还是弄一个VPN吧，android也是google的产品之一，搭建Android开发使用到的相关软件也都需要通过FQ才能够正常下载下来，这无形中增加了Android的学习成本，搭建一个最基本的Android开发环境下载所需软件都千难万难的，并且搭建开发环境的步骤也挺繁琐的，或许就是这些入门的门槛把很多开发者挡在了Android开发的门外！

## 一、下载相关软件

　　android开发环境

![](https://images0.cnblogs.com/blog2015/289233/201504/061523519617593.png)

　　准备工作：下载Eclipse、JDK、Android SDK、ADT插件

　　下载地址：JDK：[http://www.oracle.com/technetwork/java/javase/downloads/index.html](http://www.oracle.com/technetwork/java/javase/downloads/index.html)

　　　　　　　Eclipse:[http://www.eclipse.org/downloads/](http://www.eclipse.org/downloads/)

              　　Android SDK:[http://developer.android.com/sdk/index.html](http://developer.android.com/sdk/index.html)

              　　ADT:[http://developer.android.com/sdk/installing/installing-adt.html](http://developer.android.com/sdk/installing/installing-adt.html)

　　上面要下载的4个软件中，除了JDK之外，Eclipse、Android SDK、ADT插件这三个都需要FQ才能够正常下载，如果不FQ，那么上述列出来的URL地址都无法正常访问，所以提前准备好FQ工具吧， 我使用的是PGFast，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/082005014301034.png)

### 第一步：下载JDK

　　我下载的是JDK1.7的版本，下载地址：[http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html](http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html)

![](https://images0.cnblogs.com/blog2015/289233/201503/082009450552116.png)

　　下载到本地电脑后双击进行安装。JDK的安装过程比较简单，做Java开发的人都会，在安装的时候注意将JDK和JRE安装到同一个目录即可，JDK默认安装成功后，会在系统目录下出现两个文件夹，一个代表jdk，一个代表jre

![](https://images0.cnblogs.com/blog2015/289233/201503/082012345551299.png)

　　JDK的全称是Java SE Development Kit，也就是Java 开发工具箱。SE表示标准版。JDK是Java的核心，包含了Java的运行环境（Java Runtime Environment），一堆Java工具和给开发者开发应用程序时调用的Java类库。

　　我们可以打开jdk的安装目录下的Bin目录，里面有许多后缀名为exe的可执行程序，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/082019144302896.png)

　　这些都是JDK包含的工具，通过配置JDK的变量环境，我们可以方便地调用这些工具及它们的命令。

　　JDK包含的基本工具主要有：
- javac：Java编译器，将源代码转成字节码。
- jar：打包工具，将相关的类文件打包成一个文件。
- javadoc：文档生成器，从源码注释中提取文档。
- java：运行编译后的java程序。

### 第二步：配置Windows上JDK的变量环境

　　为了配置JDK的系统变量环境，我们需要设置两个系统变量，分别是JAVA_HOME，Path。下面是这两个变量的设置。

**　　1、JAVA_HOME**

　　　　先设置这个系统变量名称，变量值为JDK在你电脑上的安装路径：E:\Program Files\Java\jdk1.7.0_75创建好后则可以利用%JAVA_HOME%作为JDK安装目录的统一引用路径。

![](https://images0.cnblogs.com/blog2015/289233/201503/082026103677033.png)

**　　2、Path**

　　　　PATH属性已存在，可直接编辑，在原来变量后追加：;%JAVA_HOME%\bin

![](https://images0.cnblogs.com/blog2015/289233/201503/082029212589119.png)

　　JDK环境变量的配置做Java开发的人都应该会，这里就不细讲了！

### 第三步：下载安装Eclipse

　　Eclipse的版本有多个，这里选择下载Eclipse IDE for Java Developers这个版本。

![](https://images0.cnblogs.com/blog2015/289233/201503/082041539618207.png)

　　点击【Windows 64Bit】超链接，进入如下图所示的下载页面

![](https://images0.cnblogs.com/blog2015/289233/201503/082044486029294.png)

　　下载成功后，得到一个如下图所示的压缩包：

![](https://images0.cnblogs.com/blog2015/289233/201503/082047259616369.png)

　　Eclipse为Java应用程序及Android开发的IDE（集成开发环境）。Eclipse本身是不需要安装，下载后直接把解压包解压后就可以使用了，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/082053142584842.png)

　　到此，Eclipse的下载和安装工作就算是全部完成了。

### 第四步：下载Android SDK

　　Android SDK提供了开发Android应用程序所需的API库和构建、测试和调试Android应用程序所需的开发工具。

![](https://images0.cnblogs.com/blog2015/289233/201503/082101450087279.png)

　　这里为了免去安装步骤，我们选择下载SDK的压缩包，点击【android-sdk_r24.0.2-windows.zip】这个超链接，进入下载页面，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/082104413839395.png)

　　下载成功之后，得到一个如下图所示的压缩包：

![](https://images0.cnblogs.com/blog2015/289233/201503/082107547115426.png)

　　到此，Android的SDK就下载好了！

### 第五步：下载ADT插件

　　为了使得Android应用的创建，运行和调试更加方便快捷，Android的开发团队专门针对Eclipse IDE定制了一个插件：Android Development Tools（ADT）。

![](https://images0.cnblogs.com/blog2015/289233/201503/082122331336421.png)

　　下载成功之后，得到一个如下图所示的压缩包

![](https://images0.cnblogs.com/blog2015/289233/201503/082125296174337.png)

　　到此，Android开发需要使用到的相关软件就全部下载好了。

##  二、使用下载好的软件搭建Android开发环境

　　为了便于管理Android开发使用到的相关软件，我们可以在磁盘上建立一个AndroidDevelopTool的文件夹，将相关软件统一放到这个文件夹中，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/082136093996228.png)

### 第一步：安装Android的SDK

　　在上面的操作中，我们已经下载好了Android的SDK的压缩包，现在我们就要将这个android-sdk_r24.0.2-windows.zip压缩包解压缩，解压后得到一个【android-sdk-windows】文件夹，这样就算是完成了Android的SDK的安装工作，文件夹里面的内容如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/082140229149346.png)

　　在Android SDK的安装目录下，双击“SDK Manager.exe”，打开Android SDK Manager，Android SDK Manage负责下载或更新不同版本的SDK包，我们看到默认安装的Android SDK Manager只安装了一个版本的sdk tools，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/082146530246292.png)

　　同时SDK Manager会自动帮我们选中Android最新的版本，我们只需要点击【install】按钮就可以将选中的包下载下来，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/082150298204125.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/082155331643566.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/082156593368658.png)

　　SDK Manager下载安装Android开发包的过程如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/090014538366298.png)

　　使用SDK Manager下载Android开发包同样也需要FQ才能够正常下载，有些园友们的做法是不FQ，修改Host文件的方法，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/091032165345489.png)

　　上面两行放在host文件的意思是将本地访问dl.google.com和dl-ssl.google.com定向到ip地址为203.208.46.146的服务器上，然后**将Android SDK Manage上的https请求改成http请求**

![](https://images0.cnblogs.com/blog2015/289233/201503/091034416595726.png)

　　这种方法我试了一下，不成功，或许是我运气太背了，所以我还是老老实实弄个VPN去FQ下载吧，我家里是4M的网络，下载速度还不错，下载完成之后如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/090015541959970.png)

　　这个下载和安装过程及其漫长，我用了将近2个小时才下载完成。已经安装好的Android开发包和相关工具如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/090017235248094.png)

　　我就只下载了SDK Manager默认帮我选中的那些包，这些包是搭建Android开发环境所需要的最少的安装包了，其他的包有需要再去下载！

### 第二步：为Eclipse安装ADT插件

　　这里我们直接通过下载好的ADT插件压缩包进行安装（此种方式可以在离线环境下进行）。解压eclipse-jee-luna-SR2-win32-x86_64.zip压缩包，解压成功之后，得到一个eclipse文件夹，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/090025127273372.png)

　　用鼠标双击eclipse.exe启动Eclipse，第一次启动Eclipse时需要设置Eclipse的workspace，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/090032207581306.png)

　　设置好Eclipse的WorkSpace之后，点击【OK】按钮继续启动Eclipse，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/090033452273611.png)

　　Eclipse启动完成之后，点击 【Help】菜单 -> 【Install New Software…】，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/090114574872206.png)

　　点击弹出对话框中的【Add… 】按钮，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/090117047057371.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/090118127684566.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/090119204712015.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/090120274404008.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/090121371129259.png)

　　这个安装过程稍微要一点时间，因为在安装的过程中会去下载一些东西，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/090122455345796.png)

　　安装的过程中会弹出一次警告，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/090124192524194.png)

　　无视这个警告，直接点击【OK】按钮，继续安装。安装完成之后，弹出提示消息框，提示信息告诉我们需要重启Eclipse，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/090125437842986.png)

　　点击【Yes】按钮重启Eclipse。Eclipse重启之后会根据目录的位置智能地和它相同目录下Android sdk进行关联，如果Eclipse没有自动关联Android sdk的安装目录，那么就会弹出如下图所示的提示信息，要求我们设置Android SDK的安装目录

![](https://images0.cnblogs.com/blog2015/289233/201503/090127240805760.png)

　　点击【Open Preferences】按钮，在弹出面板中就会看到Android设置项，填上安装的SDK路径，则会出现刚才在SDK中安装的各平台包，按OK完成配置。如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/090132510961832.png)

　　我们刚才使用SDK Manager只下载了Android5.0的相关包，所以这里只显示Android5.0平台的。

　　到这里，我们在windows上的Android上的开发环境搭建就完成了，这时候，你用Eclipse的File——》New——》Project...新建一个项目的时候，就会看到建立Android项目的选项了。如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/090138196434106.png)

### 第三步：测试Android开发环境是否搭建成功

　　使用Eclipse新建Android一个项目，创建步骤如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/090147241276381.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/090138196434106.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/090155520181046.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/090210367848646.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/090211376748647.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/090212321126246.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/090214259874602.png)

　　创建好的项目如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/090215502688851.png)

　　项目创建成功之后，我们直接运行这个Android项目，在Eclipse中运行Android项目的步骤如下：选中项目-----》鼠标右键------》【Run As】------》【Android Application】，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/090223385182971.png)

　　选择调试Android应用的模拟器，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/090225132684784.png)

　　我开启了手机的USB调试模式，直接使用手机作为模拟器运行这个Android应用了，Console打印出来的Log信息如下：

![](https://images0.cnblogs.com/blog2015/289233/201503/090239530026280.png)

　　应用在手机上面的运行结果如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/090227160964974.png)

　　运行成功。手机上面也会有我们这个应用的图标，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/090234599249576.png)

　　到此，我们的Android开发环境的测试工作就算是全部完成了。

## 三、配置Android SDK的环境变量

　　配置Android SDK的环境变量不是搭建Android开发环境必须要做的工作，但是配置了之后就可以很方便在命令行窗口使用Android SDK安装目录下的platform-tools和tools目录中的工具了，所以还是配置一下比较好，方便使用！

　　Android SDK的环境变量值为SDK安装目录下的platform-tools文件夹和tools文件夹子目录路径，如我的分别为：E:\AndroidDevelopTool\android-sdk-windows\platform-tools、E:\AndroidDevelopTool\android-sdk-windows\tools，在变量值中，两个路径用逗号隔开。

（1）添加环境变量：

**　　变量名：ANDROID_SDK_HOME**

　　变量值：**E:\AndroidDevelopTool\android-sdk-windows（这个Android的SDK的安装目录）**

![](https://images0.cnblogs.com/blog2015/289233/201503/091054093937509.png)

（2）在path中添加如下内容：

　　%ANDROID_SDK_HOME%\tools;%ANDROID_SDK_HOME%\platform-tools

![](https://images0.cnblogs.com/blog2015/289233/201503/091108366126082.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/091109357846526.png)

使用快捷键"Win+R"，然后输入CMD，进入命令行，分别输入android -h和adb命令，显示如下图则说明Android SDK环境变量已经配置好了

![](https://images0.cnblogs.com/blog2015/289233/201503/091113364091205.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/091115067213029.png)

　　简单说一下platform-tools目录下的adb这个工具的主要功能：
- 运行设备的shell(命令行)
- 管理模拟器或设备的端口映射
- 计算机和设备之间上传/下载文件
- 将本地apk软件安装至模拟器或android设备

## 四、总结

　　以上就是搭建最新版本的Android的开发环境的全部内容，搭建步骤和过程是我亲自实践过的，按照上述的搭建过程应该都可以正常搭建Android的开发环境，写这一篇文章花了很多时间，本着不误人子弟，认真负责的态度，把每一个过程以及可能会遇到的问题都尽可能写清楚，希望所有看了这篇文章的朋友们都能够正常搭建起Android的开发环境，踏入Android开发的大门！本文中的有些内容参照了以下几位园友的文章：

　　1、[生命壹号](http://www.cnblogs.com/smyhvae/) 的[Android配置----Android开发环境搭建](http://www.cnblogs.com/smyhvae/p/3788555.html)

　　2、[郑毅](http://home.cnblogs.com/u/allenzheng/) 的 [Android基础之用Eclipse搭建Android开发环境和创建第一个Android项目（Windows平台）](http://www.cnblogs.com/allenzheng/archive/2012/11/10/2762379.html)

　　3、[边写边唱](http://www.cnblogs.com/zoupeiyang/) 的 [五步搞定Android开发环境部署——非常详细的Android开发环境搭建教程](http://www.cnblogs.com/zoupeiyang/p/4034517.html)

　　再次感谢这几位园友，本人Android开发刚刚入门，希望以后能够有更多的机会和博客园的朋友们一起学习Android的开发！









