
# java入门教程-1.5Java开发环境的搭建（JDK和Eclipse的安装） -  芝麻软件工作室的专栏 - CSDN博客


2016年05月24日 09:13:18[seven-soft](https://me.csdn.net/softn)阅读数：3619


要进行Java开发，首先要安装JDK（Java Development Kit，Java开发工具箱）。
JDK 是一系列工具的集合，这些工具是编译Java源码、运行Java程序所必需的，例如JVM、基础类库、编译器、打包工具等。不论是什么样的 Java 应用服务器，都是内置了某个版本的 JDK，因此掌握 JDK 是学好 Java
 的第一步。
JDK所提供的部分工具：java编译器：javac.exe
java解释器：java.exe
java文档生成器：javadoc.exe
java调试器：jdb.exe
前面所说的Java版本，实际上是指JDK的版本。
最主流的 JDK 是 Sun 公司发布的 JDK，除了 Sun 之外，还有很多公司和组织都开发了自己的 JDK，例如 IBM 公司开发的 JDK、BEA 公司的 Jrocket，还有 GNU 组织开发的 JDK等等。IBM 的
 JDK 包含的 JVM（Java Virtual Machine）运行效率要比 Sun JDK 包含的 JVM高出许多，而专门运行在 x86 平台的 Jrocket，在服务端运行效率也要比 Sun JDK 好很多，但不管怎么说，还是需要先把 Sun JDK 掌握好。
## JDK 的下载
JDK有不同的版本（J2SE、J2EE、J2ME），初学Java，一般都选择J2SE。J2SE的下载地址为：[http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html](http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html)
打开网址，会看到如下所示的页面：![](http://www.weixueyuan.net/uploads/allimg/141124/1-141124195PM30.png)
图1  J2SE(Java SE)下载页面
下载JDK需要接受相应条款，默认是不能下载的。
选择相应的操作系统，点击下载链接即可下载。
## JDK的安装
双击下载好的安装包进行安装，点击“下一步”，出现如下图所示的界面：![](http://www.weixueyuan.net/uploads/allimg/141124/1-141124200Ac94.png)
图2  改变安装目录
这里可以根据你的习惯改变JDK的安装目录，不过你要记住，后面会用到这个安装目录。
可以看到，JDK包含了Java开发工具（编译器、打包工具等）、源代码（基础类库）和公共JRE，这三项都是默认安装的，是Java开发所必须的，缺一不可。
点击“下一步”，等待安装。![](http://www.weixueyuan.net/uploads/allimg/141124/1-141124201120340.png)
图3  正在安装
JDK安装完成后，会提示你是否安装JRE，如下图所示：![](http://www.weixueyuan.net/uploads/allimg/141124/1-141124201231354.png)
图4  是否安装JRE
JDK中已经包含了JRE，无需再次安装，点击“取消”即可。![](http://www.weixueyuan.net/uploads/allimg/141124/1-141124201404153.png)
图5  确认取消
![](http://www.weixueyuan.net/uploads/allimg/141124/1-14112420142J04.png)
图6  完成安装
点击“关闭”，完成安装。
## 环境变量的设置
如果你不了解环境变量的概念，请猛击这里：[什么是环境变量](http://www.weixueyuan.net/view/6310.html)
进入环境变量配置窗口，在“用户变量”中，设置3项属性，JAVA_HOME、PATH、CLASSPATH（大小写无所谓），若已存在则点击“编辑”，不存在则点击“新建”：JAVA_HOME：设为JDK的安装路径(如D:\Program Files\jdk1.7.0_71)，此路径下包括lib，bin，jre等文件夹（此变量最好设置，因为以后运行tomcat，eclipse等都需要依靠此变量）。
Path：使得系统可以在任何路径下识别java命令，设为：%JAVA_HOME%\bin。%JAVA_HOME%就是引用前面指定的JAVA_HOME变量。
CLASSPATH：Java运行环境加载类的路径，只有类在classpath中，才能被识别和加载，设为 .;%JAVA_HOME%\lib（注意前面的点号(.)，点号表示当前路径）。
打开一个CMD窗口，输入“java -version”或者“javac”命令，看到很多说明信息，证明已经安装并配置成功了。
## Eclipse的安装
有了JDK，你可以编译Java源码，运行Java程序，但是还没有代码编辑器，没有版本管理工具，也不能方便的管理工程文件，不能与团队协作。安装Eclipse，你才能完成这些工作。
Eclipse是一款Java集成开发环境(IDE)。Java IDE有很多，有免费的，有收费的，有英文的，中文的，有多国语言的，Eclipse 是最常用的一款，IT公司大部分员工都使用Eclipse。
Eclipse 开源免费，使用人数最多，提供了丰富的插件和友好的编辑界面，资源占用比较低，速度比较快，本身就是用Java开发的。
注意：一定要设置环境变量，Eclipse 的运行依赖于这些环境变量。
Eclipse的下载地址为：[http://www.eclipse.org/downloads/](http://www.eclipse.org/downloads/)
打开链接，看到下面的页面：![](http://www.weixueyuan.net/uploads/allimg/141125/1-141125092159412.png)
图7  Eclipse 下载页面
选择红色方框框起来的版本，下载即可。
Eclipse 是免安装的，你可以将下载的压缩文件解压到任意目录，以后也可以随意更换目录。第一次启动Eclipse，会要求你设置默认的工程目录，你可以只设置一次，也可以每次启动都设置。
![](http://www.weixueyuan.net/uploads/allimg/141125/1-1411250934205V.png)
图8  设置工程目录
点击“OK”，Eclipse 就成功启动，弹出欢迎界面。
## 安装简体中文语言包
Eclipse 默认是英文的，如果你的英文能力比较弱，可以安装简体中文语言包。不过我建议你使用英文版，作为程序员，你绝对有必要提高你的英文水平，很多技术文档都是英文的，只要持之以恒，很快就能提高你的英文阅读能力。
语言包的下载地址为：[http://www.eclipse.org/babel/downloads.php](http://www.eclipse.org/babel/downloads.php)
首先查看你当前安装的Eclipse的版本。在Eclipse菜单栏中选择“Help --> About Eclipse”，弹出一个对话框：![](http://www.weixueyuan.net/uploads/allimg/141125/1-141125101313161.png)
图9  查看Eclipse版本
打开链接，向下滚动鼠标，下载对应版本的语言包：![](http://www.weixueyuan.net/uploads/allimg/141125/1-141125101G5936.png)
图10  对应版本的语言包
进入下载页面，找到简体中文语言包：![](http://www.weixueyuan.net/uploads/allimg/141125/1-141125102IWA.png)
图11  中文语言包
这里包含了Eclipse软件本身及其模块和插件的语言包，你可以一次全部下载，也可以需要的时候再来下载，红色方框框起来的是Eclipse软件本身的中文包，我们需要下载它。
将下载到的文件解压，得到一个名为eclipse的文件夹，它里面包含两个文件夹：features和plugins，复制features和plugins到你的eclipse安装目录，覆盖原程序文件即可。![](http://www.weixueyuan.net/uploads/allimg/141125/1-1411251031193V.png)
图 12  覆盖原来的文件
重启Eclipse，汉化完成，你可以享受中文版的Eclipse了。

