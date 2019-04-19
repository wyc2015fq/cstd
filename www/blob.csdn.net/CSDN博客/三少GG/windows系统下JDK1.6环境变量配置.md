# windows系统下JDK1.6环境变量配置 - 三少GG - CSDN博客
2010年09月28日 21:18:00[三少GG](https://me.csdn.net/scut1135)阅读数：1267标签：[jdk																[windows																[java																[path																[myeclipse																[dos](https://so.csdn.net/so/search/s.do?q=dos&t=blog)](https://so.csdn.net/so/search/s.do?q=myeclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=jdk&t=blog)
个人分类：[重构Java](https://blog.csdn.net/scut1135/article/category/621771)
**一、JDK1.6下载**
       目前JDK最新版本是JDK1.6，到http://java.sun.com/javase/downloads/index.jsp可以下载JDK1.6。
**二、JDK1.6安装**
       JDK的安装非常简单，只需要按照向导一步一步进行即可。在安装时需记住安装的路径，在配置环境变量时将会用到这个安装路径。我的安装路径是F:jdk1.6。
**三、配置环境变量**
       对于初学者来说，环境变量的配置是比较容易出错的，在配置的过程中应当仔细。**使用JDK一共需要配置三个环境变量：java_home、classpath和path(不区分大小写)。**
**1、java_home**
       右击“我的电脑”，选择“属性”，点击“高级”选项卡，选择“环境变量”。进入如下对话框：
![windows系统下JDK1.6环境变量配置 - 阿文 - 陈文秀 的博客](http://image8.360doc.com/DownloadImg/2010/03/2910/2699765_1.jpg)
      在系统变量栏，点击“新建”按钮，弹出如下对话框：
![windows系统下JDK1.6环境变量配置 - 阿文 - 陈文秀 的博客](http://image8.360doc.com/DownloadImg/2010/03/2910/2699765_2.jpg)
     在变量名处填写java_home，变量值填写F:jdk1.6，然后点击确定，这样java_home变量就配置完成了。
**2、classpath**
     可以参考java_home的配置过程，需要新建一个系统变量，不过classpath的变量名是classpath，变量值是**.;**%JAVA_HOME%/lib**;**%JAVA_HOME%/lib/tools.jar (要加**.**表示当前路径)。
**3、path**
     path变量在系统变量中已存在，要修改path变量只需要选中path变量，点击编辑即可。在变量值中加入如下内容：%JAVA_HOME%/bin;%JAVA_HOME%/jre/bin。建议把 JDK bin 的路径放在原有 path 的最前面。因为在 %SystemRoot%/system32 中还有一个 java.exe/javaw.exe，这是 Windows 操作系统自带的那个JRE，如果我们自己装的话就不要用他默认的那个JRE 了，可以用我们自己装的。如果把 JDK bin 的 path 放在最前面的话会优先使用。
**在myeclipse中windows-->preference-->java-->Installed JREs 自动搜索search，找到安装的jre，选中即可！！！**
**配置JAVA_HOME,CLASSPATH,PATH的目的：**
     1,设置JAVA_HOME:
     JAVA_HOME环境变量。它指向jdk的安装目录，Eclipse/NetBeans/Tomcat等软件就是通过搜索JAVA_HOME变量来找到并使用安装好的jdk。
     一、为了方便引用，比如，你JDK安装在C:/Program Files/Java/jdk1.6.0目录里，则设置JAVA_HOME为该目录路径, 那么以后你要使用这个路径的时候, 只需输入%JAVA_HOME%即可, 避免每次引用都输入很长的路径串;
     二、归一原则, 当你JDK路径被迫改变的时候, 你仅需更改JAVA_HOME的变量值即可, 否则,你就要更改任何用绝对路径引用JDK目录的文档, 要是万一你没有改全, 某个程序找不到JDK, 后果是可想而知的----系统崩溃!
       2.设置CLASSPATH:
        CLASSPATH环境变量的作用是指定类搜索路径，要使用已经编写好的类，前提当然是能够找到它们了，JVM就是通过CLASSPTH来寻找类的。我们需要把jdk安装目录下的lib子目录中的dt.jar和tools.jar设置到CLASSPATH中，当然，当前目录“.”也必须加入到该变量中。这个变量设置的目的是为了程序能找到相应的".class"文件, 举个例子: 你编译一个JAVA程序---A.java, 会得到一个A.class的类文件,你在当前目录下执行java A, 将会得到相应的结果(前提是你已经设置CLASSPATH为"."). 现在, 你把A.class移到别的目录下(例如:"e:/"), 执行java A, 将会有NoClassDefFindError的异常,原因就是找不到.class文件, 现在你把CLASSPATH增加为:".;e:/"再运行java A, 看看会有什么结果~~:)~~~, 一切正常, java命令通过CLASSPATH找到了.class文件!
       3.设置PATH:
        道理很简单, 你想在任何时候都使用%JAVA_HOME%/bin/java 等来执行java命令吗, 当然不会, 于是, 你可以选择把 %JAVA_HOME%/bin添加到PATH路径下, 这样, 我们在任何路径下就可以仅用java来执行命令了.(当你在命令提示符窗口输入你个代码时,操作系统会在当前目录和PATH变量目录里查找相应的应用程序, 并且执行.)。PATH环境变量的作用是指定命令搜索路径，在dos命令行下面执行命令如javac编译java程序时，它会到PATH变量所指定的路径中查找看是否能找到相应的命令程序。我们需要把jdk安装目录下的bin目录增加到现有的PATH变量中，bin目录中包含经常要用到的可执行文件如javac/java/javadoc等待，设置好PATH变量后，就可以在任何目录下执行javac/java等工具了。
**四、安装检查**
       在dos窗口下，键入java -version命令可以查看到安装的JDK版本信息；键入java命令，可以看到此命令的帮助信息；键入javac命令可以看到此命令的帮助信息。如果都如愿看到，说明JDK安装成功。
