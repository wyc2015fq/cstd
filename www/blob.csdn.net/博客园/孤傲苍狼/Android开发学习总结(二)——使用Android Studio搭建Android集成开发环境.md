# Android开发学习总结(二)——使用Android Studio搭建Android集成开发环境 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [Android开发学习总结(二)——使用Android Studio搭建Android集成开发环境](https://www.cnblogs.com/xdp-gacl/p/4329469.html)



　　有很长一段时间没有更新博客了，最近实在是太忙了，没有时间去总结，现在终于可以有时间去总结一些Android上面的东西了，很久以前写过这篇关于使用Android Studio搭建Android集成开发环境，不过一直没有发表出来，写这篇文章的目的是记录一下Android开发环境的搭建过程，这篇文章写得一般般，主要是记录了整个搭建过程，没什么技术含量，觉得有帮助的朋友就看一下！

## 一、Android Studio简单介绍

　　2013年GoogleI/O大会首次发布了Android Studio IDE（Android平台集成开发环境）。它基于Intellij IDEA开发环境，旨在取代Eclipse和ADT（Android开发者工具）为开发者提供更好的开发工具。既然Google一直在努力推广，相信不久以后就有望赶上Eclipse。
- 相比Eclipse，Android Studio IDE有自己的特点：
- 对UI界面设计和编写代码有更好地支持，可以方便地调整设备上的多种分辨率。
- 同样支持ProGuard工具和应用签名。
- 不过，目前版本的Android Studio不能在同一窗口中管理多个项目。每个项目都会打开一个新窗口。我猜这是借鉴了Intellij IDEA的行为，而且Google近期不会对此做出调整。
- 支持Gradle自动化构建工具，这真是极好的，但对于刚从Eclipse平台转移过来的开发者来说还需要一段时间去学习和适应。

## 二、下载和安装相关软件

### 2.1、开发环境　

　　物理机版本：Win 8（64位）

　　Java SDK版本：jdk-7u45-windows-x64（64位）

　　Android Studio版本：android-studio-bundle-135.1740770-windows

　　注：物理机版本和Java SDK版本必须要保持一致，即：同为64位或者同为32位。

### 2.2、下载JDK

　　我下载的是JDK1.7的版本，下载地址：[http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html](http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html)

![](https://images0.cnblogs.com/blog2015/289233/201503/082009450552116.png)

　　下载到本地电脑后双击进行安装。JDK的安装过程比较简单，安装过程基本上就是一路Next即可，做Java开发的人都会，在安装的时候只需要注意将JDK和JRE安装到同一个目录即可，JDK默认安装成功后，会在系统目录下出现两个文件夹，一个代表jdk，一个代表jre

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

### 2.3、配置Windows上JDK的变量环境

　　为了配置JDK的系统变量环境，我们需要设置两个系统变量，分别是JAVA_HOME，Path。下面是这两个变量的设置。

**　　1、JAVA_HOME**

　　　　先设置这个系统变量名称，变量值为JDK在你电脑上的安装路径：E:\Program Files\Java\jdk1.7.0_75创建好后则可以利用%JAVA_HOME%作为JDK安装目录的统一引用路径。

![](https://images0.cnblogs.com/blog2015/289233/201503/082026103677033.png)

**　　2、Path**

　　　　PATH属性已存在，可直接编辑，在原来变量后追加：;%JAVA_HOME%\bin

![](https://images0.cnblogs.com/blog2015/289233/201503/082029212589119.png)

　　JDK环境变量的配置做Java开发的人都应该会，这里就不细讲了！

### 2.4、Android Studio下载

　　下载地址：http://developer.android.com/sdk/index.html，注意，下载Android Studio要FQ才行

![](https://images0.cnblogs.com/blog2015/289233/201503/111142415581534.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/111140038399414.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/111144533559731.png)

　　下载完成之后，得到一个如下图所示的安装包：

![](https://images0.cnblogs.com/blog2015/289233/201503/111148134498194.png)

### 2.5、Android Studio安装

　　下载完成后，就可以开始安装了，用鼠标双击android-studio-bundle-135.1740770-windows.exe启动安装程序，Android Studio安装过程如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/111503396999629.png)

　　AndroidStudio是集成了Android SDK的，所以在安装的时候记得勾选上Android SDK

![](https://images0.cnblogs.com/blog2015/289233/201503/111505097779382.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/111505571677059.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/111518357927924.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/111528506993782.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/111529431207563.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/111530466368968.png)

　　这个安装过程有点久，需要一点时间。

![](https://images0.cnblogs.com/blog2015/289233/201503/111546478249482.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/111549084498944.png)

### 2.6、启动运行Android Studio

　　Android Studio启动过程如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/111602043867765.png)

![](https://images0.cnblogs.com/blog2015/289233/201503/111603252616440.png)

　　第一次启动AndroidStudio时，需要设置一下SDK的安装目录，因此会弹出如下图所示的对话框，

![](https://images0.cnblogs.com/blog2015/289233/201503/111604372302593.png)

　　设置Android SDK的安装目录，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/111608103709307.png)

　　打开AndroidStudio之后，默认会帮我们创建一个app的项目，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/111609251524921.png)

　　运行这个默认创建好的项目，为了运行方便，我们直接使用真机作为模拟器运行，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/111636139335647.png)

　　注意，使用真机调试时，手机必须开启USB调试模式才行，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/111645204497509.png)

　　在手机上面的运行结果如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/111648079495348.png)

　　手机上显示应用的桌面上也显示出了我们这个Android应用的图标，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/111702105421432.png)

　　到此，使用Android Studio搭建Android集成开发环境的工作就全部完成了，测试也通过了！

## 三、Android Studio的简单使用

### 3.1查看安装好的Android SDK

![](https://images0.cnblogs.com/blog2015/289233/201503/111723444953209.png)

　　已经安装好的Android SDK版本如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/111740494332901.png)

　　我们可以看到，截止到今天为止，Anddroid的版本已经更新到Android5.1(API22)了，更新速度真的很快啊，Android5.1(API22)的相关内容现在还是处于【Not installed】状态，也就是还没有下载和安装，SDK Manager默认已经选中Android5.1(API22)的全部内容，我们如果想在Android5.1(API22)的平台下开发Android应用，那么可以下载Android5.1(API22)的相关内容，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/111755565117318.png)

　　默认下载和安装好的Android5.0.1的开发包已经满足我们开发Android应用的需求了，如下图所示：

![](https://images0.cnblogs.com/blog2015/289233/201503/111800243708716.png)

　　关于使用Android Studio搭建Android集成开发环境的内容就写这么多吧，据说AndroidStudio使用起来还是挺方便的，但由于公司开发Android的应用使用的还是Eclipse，因此暂时没有时间去研究Android Studio的使用，等有时间再去看看吧！









