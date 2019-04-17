# Web开发环境搭建 Eclipse-Java EE 篇 - DoubleLi - 博客园






1. 下载和安装
1.1 下载JDK
在Java官方网站下载最新版本的 Java SE:  http://www.oracle.com/technetwork/java/javase/downloads/index.html

1.2 下载 Java EE 最新版本: 
     http://www.oracle.com/technetwork/java/javaee/downloads/index.html
1.3 安装JDK:
执行下载好的 JDK 安装包安装即可。

1.4 下载 Eclipse
打开 http://www.eclipse.org/downloads/eclipse-packages/

1.5 安装Java EE
将下载好的 Java EE 解压缩，然后运行 \glassfish4\bin\pkg.bat ，在提示是否安装输入 <y/n> 时，输入y后回车，然后待等完成安装。

1.6 安装 Tomcat
进入官网（http://tomcat.apache.org/），直接下载最新版本的 Tomcat 9即可。

![](https://images2015.cnblogs.com/blog/1026583/201701/1026583-20170119170830843-1697378315.png)

下载完成后，将压缩包解压到Java安装目录下 tomcat 中

2. 配置和开始使用
2.1 运行 Eclipse Java EE。

![](https://images2015.cnblogs.com/blog/1026583/201701/1026583-20170119170957234-1710885808.png)

2.2 设置 Tomcat
在 Eclipse 中，点击菜单栏 Window->Preferences ， 点击 Server / Runtime Environments ：

![](https://images2015.cnblogs.com/blog/1026583/201701/1026583-20170119171941750-1262794767.png)

点击 “Add”按钮， 新建一个服务器。我们之前安装了 Tomcat 9.0, 所以这里也选择它。

![](https://images2015.cnblogs.com/blog/1026583/201701/1026583-20170119172016437-1115277173.png)

点击"Next"进入下一步。

![](https://images2015.cnblogs.com/blog/1026583/201701/1026583-20170119172053562-2114580617.png)

在Tomcat installation directory下面的文本框中输入tomcat的安装目录，点击"finish"完成配置。

2.3 设置默认的字符集
在eclipse中，会默认使用当前操作系统的字符集，一般会是GBK。然而我们开发web应用程序时，一般使用UTF-8。所以需要设置一下默认的字符集。
在 Eclipse 中，点击菜单栏 Window->Preferences ， 点击 General / Workspace :

![](https://images2015.cnblogs.com/blog/666150/201606/666150-20160625095415781-2017796568.png)

在 Text file encoding 区域选择  Other : UTF-8 。 这样子以后我们新建项目时，会默认使用 UTF-8 字符编码。

2.4 设置 Build Path

在开发Web应用程序时，需要用到 Tomcat ，我们要将 Tomcat / lib 加入编译路径中。否则你会在建立 jsp 时，出现如下错误：

The superclass "javax.servlet.http.HttpServlet" was not found on the Java Build Path

![](https://images2015.cnblogs.com/blog/666150/201606/666150-20160625100054781-1988208364.png)

在 Eclipse 中，点击菜单栏 Window->Preferences ， 点击 Java / Build Path / Classpath Variables :

![](https://images2015.cnblogs.com/blog/666150/201606/666150-20160625100324828-391291137.png)

点击“New"新建名为“Tomcat Server”的变量，将Path设置为tomcat的安装位置中的lib目录（如上图所示），点击“OK”按钮。

接下来，添加 User Libraries。点击 Java / Build Path / User Libraries :

![](https://images2015.cnblogs.com/blog/666150/201606/666150-20160625100744125-523914405.png)

点击“New...”按钮，新建名为 Tomcat Server 的User Libraries。然后点击“Add External JARs...”按钮，将我们安装并使用的tomcal / lib 下的所有 jar 选中（Ctrl + A）并确定。完成后如上图所示。

点击“OK”完成设置。



2.5 Hello World

每一次搭建完开发环境，写一个 "hello world" 并成功的显示出来， 心情会自然变得舒畅。

在 Eclipse Java EE 中， 在左边的 Project Explorer 的空白区域点击鼠标右键， 然后点击 “New->Dynamic Web Project" ：

![](https://images2015.cnblogs.com/blog/666150/201606/666150-20160625102227985-1498081274.png)

点击“Finish”完成。

然后在WebContent上右键，在右键菜单中点击“New->JSP File”，新建一个 Index.jsp 页面。

![](https://images2015.cnblogs.com/blog/666150/201606/666150-20160625103651594-157873603.png)

双击打开 Index.jsp ，提示出错了，这个正是前面我们说到的问题，还配置了 Build Path。那么现在我们就要把设置的 Build Path / User Libraries 用上就行了。

在工程管理器的HelloWorkd的工程根目录上右键，点击“Properties”。然后在弹出的工程属性对话框中，点击“Java Build Path”， 然后切换到 Libraries 选项卡。

点击 “Add Library...”按钮：

![](https://images2015.cnblogs.com/blog/666150/201606/666150-20160625104037891-587833382.png)

双击 “User Library”， 在弹出的 Add Library 对话框中， 选中 "Tomcat Server"，点击“Finish”按钮。

![](https://images2015.cnblogs.com/blog/666150/201606/666150-20160625104207125-1826085103.png)

然后切换到 Order and Export 选项卡， 选中 Tomcat Server 。完成设置。

![](https://images2015.cnblogs.com/blog/666150/201606/666150-20160625104315047-551959124.png)

可以看到，代码中的错误已经没有了。

![](https://images2015.cnblogs.com/blog/666150/201606/666150-20160625104400156-1939596724.png)











