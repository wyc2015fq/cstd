# Java基础, Java平台版本, JDK环境搭建详解--(一) - fanyun的博客 - CSDN博客
2016年03月24日 09:40:02[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1507
所属专栏：[Java框架精要](https://blog.csdn.net/column/details/java-01.html)
Java基础, Java平台版本, JDK环境搭建, 编译运行Java HelloWorld程序
JDK是Java Development Kit的缩写，即Java开发工具集。JDK是整个Java的核心，包括了Java运行环境（JRE）、Java开发工具和Java基础类库。
JRE是Java Runtime Environment的缩写，即Java运行时环境。Java程序运行时必需要JRE的支持。
**Java的平台版本：**
Java分为J2SE、J2EE和J2ME三个不同的平台版本，即标准版（Java 2 Platform, Standard Edition）、企业版（Java 2 Platform, Enterprise Edition）和微型版（Java 2 Platform, Micro Edition）。从Java 1.5（或者Java 5.0）开始，它们被改称为Java SE、Java EE和Java ME。
各平台版本之间的差别在于适用范围不同：
- 标准版平台（Java SE）允许开发和部署在桌面、服务器、嵌入式和实时环境中使用的Java应用程序。另外，Java SE包含了支持实现Java Web服务的类库，因此标准版是企业版的基础。
- 企业版平台（Java EE）帮助开发者开发和部署Java Web应用。企业版提供Web服务、组件模型、管理和通信API，可以用来实现企业级的面向服务的体系结构(Service-Oriented Architecture, SOA)和Web 2.0 应用程序。
- 而微型版平台（Java ME）主要用于移动设备和嵌入式设备，如手机、PDA、电视机顶盒等等。微型版包括灵活的用户界面、健壮的安全模型、许多内置的网络协议、以及对可动态下载的在线和离线应用的支持。基于Java ME规范的应用程序与普通Java程序一样，只需编译一次，即可在许多设备上运行。
**Java的版本：**
- 1995年5月23日，Java语言诞生
- 1996年1月，JDK 1.0发布
- 1997年2月18日，JDK 1.1发布
- 1998年12月4日，J2SE 1.2发布，1.2版及其之后的版本也被称为Java 2
- 1998年12月8日，Java 2企业版发布
- 2000年5月8日，J2SE 1.3发布
- 2002年2月13日，J2SE 1.4发布
- 2004年9月30日，J2SE 1.5发布，J2SE 1.5版也被称为Java 5.0
- 2006年12月，Java 6.0发布
- 2010年9月，Java 7.0发布
- 2011年7月28日，甲骨文发布Java 7.0的正式版；
- 2014年3月19日，甲骨文公司发布Java 8.0的正式版。
- 2014年11月甲骨文公司发布了Java 9.0的新特性，比较重要的的有：- 统一的[JVM](http://baike.so.com/doc/1063579.html)日志
- 支持[HTTP](http://baike.so.com/doc/5366073.html) 2.0
- [Unicode](http://baike.so.com/doc/4443890.html) 7.0支持
- 安全数据包传输（[DTLS](http://baike.so.com/doc/1736865.html)）支持
- Linux/AArch64支持
**Java开发与运行环境的搭建（Java SE）：1. 下载JDK/JRE：**
首先，访问Oracle公司的Java SE的下载主页 ([http://www.oracle.com/technetwork/java/javase/downloads/index.html](http://www.oracle.com/technetwork/java/javase/downloads/index.html))，选择一个版本（目前最新版为Java
 SE 7），如下图：
![JDK下载页面](http://hi.csdn.net/attachment/201109/9/0_1315548025E5R5.gif)
此页面包含多个版本的JDK、JRE、帮助文档、源代码等下载内容的链接。如果不是Java程序的开发者，仅仅想在自己的系统中运行Java程序，那么只需要一个JRE就可以了；如果想使用Java开发自己的应用程序，则需要下载JDK，其中已包含JRE，因此下载了JDK后无需再单独下载JRE。
这里以下载Java SE 7的JDK为例，点击相应的Download按钮，转到下载页面：
![下载JDK](http://hi.csdn.net/attachment/201109/9/0_13155480967H1W.gif)
在此页面中，包含了对应各种操作系统的JDK下载链接，选择自己系统对应的JDK，将其下载到本地硬盘上。注意，在下载之前需要先阅读“Oracle Binary Code License Agreement for Java SE”，必须接受其中的条款才能下载JDK（选中“Accept License Agreement”）。
**2. 安装JDK/JRE:**
无论是在Windows还是在Linux下安装JDK都很简单，与安装其他程序没什么不同。（因为我没有其他操作系统的环境，也没用过其他系统，因此不清楚在其他操作系统下的安装方法，但想来应该也不是难事——至少不会比安装其他程序难）。
在Windows中，双击刚才下载的“jdk-7-windows-i586.exe”文件，就会打开安装界面。点击“下一步”按钮，可以在此选择需要安装的组件和安装目录，窗口右侧是对所选组件的说明，包括组件功能和所需的磁盘空间；可以点击“更改”按钮来改变安装目录。点击“下一步”即开始正式安装。安装完毕后，将会显示安装已完成的信息，点击“完成”按钮即可完成安装。
来到安装文件夹下，即可以看到已安装的JDK的目录结构。（注意其中包含名为“jre”的文件夹，这就是前面说的JDK包含JRE的原因所在）
整个安装过程如下面几幅图所示：
![](http://hi.csdn.net/attachment/201109/9/0_1315548128tiD8.gif)![](http://hi.csdn.net/attachment/201109/9/0_13155481480D40.gif)
![](http://hi.csdn.net/attachment/201109/9/0_1315548160UulD.gif)![](http://hi.csdn.net/attachment/201109/9/0_1315548171tK51.gif)
注意：操作系统分为32位操作系统和64位操作系统，对应地，JDK也分为32位版和64位版（名称中带有“i586”或“x86”的为32位版，带有“x64”则表示该JDK为64位版）。64位版JDK只能安装在64位操作系统上，32位版JDK则既可以安装在32位操作系统上，也可以安装在64位操作系统上。原因是64位的操作系统能够兼容32位的应用程序。
换句话说，即使CPU是64位的，但如果安装的操作系统是32位的，那么也无法安装64位版的JDK。
在Linux中安装JDK与安装其他程序相同。下载时可以选择.rpm或.tar.gz格式的安装文件，这里以后者为例进行说明。
首先解压缩下载的文件，输入命令“tar -xf jdk-7-linux-i586.tar.gz -C /usr”，将文件解压到/usr目录下，这样就完成了安装（如图）：
![Linux安装JDK](http://hi.csdn.net/attachment/201109/9/0_1315548297BfFb.gif)
**3. 设置环境变量：**
环境变量是指在操作系统中用来指定操作系统运行环境的一些参数，比如临时文件夹位置和系统文件夹位置等。环境变量相当于给系统或应用程序设置的一些参数。
编译或运行Java程序时，都是基于命令行的，因此在此之前必须设置一些环境变量的值。有些Java IDE（集成开发环境）内置了JDK，因此使用这些IDE时可以不指定环境变量。还有些程序需要个性化的环境变量（如Apache Tomcat需要JAVA_HOME环境变量）。
与JDK或JRE的使用有关的是PATH、CLASSPATH等几个环境变量。这里先解释一下这些变量的含义：
PATH变量用来告诉操作系统到哪里去查找一个命令。如果清空PATH变量的值，在Windows中运行一个外部命令时，将提示未知命令错误（当然，在Linux中也是一样）：
![](http://hi.csdn.net/attachment/201109/9/0_1315548334AaYY.gif)
注意：在Windows中，如“dir”、“cd”等命令是内部命令，类似于DOS中的常驻命令。这些命令在命令行窗口启动时会自动加载到内存中，不需要到磁盘上去寻找对应的可执行文件，因此即使清空了PATH变量的值也不会影响这些命令的使用。然而，像“java”这样的外部命令，在执行时必须先由操作系统到指定的目录找到对应的可执行程序，然后才能加载并运行。到哪里去寻找这些程序就是依靠PATH变量来指定的。
Linux也是类似，甚至可以说在Linux中，PATH环境变量更为重要，因为Linux的很多基本命令都属于外部命令，如“ls”、“mkdir”等。当将PATH变量清空后，这些命令都将无法使用（当然，还是有一些内部命令我们仍然可以使用）。
CLASSPATH是编译或运行Java程序时用来告诉Java编译器或虚拟机到哪里查找Java类文件的，后面会对其做详细介绍。
在Windows XP或之前的版本中，依次点击“右键我的电脑” -> “属性” -> “高级” -> “环境变量”；在Windows Vista和Windows 7中则依次点击“右键我的电脑” -> “属性” -> “高级系统设置” -> “高级” -> “环境变量”，打开环境变量设置窗口：
![](http://hi.csdn.net/attachment/201109/9/0_1315548354kPir.gif)
新建一个用户变量，名称为PATH，值为“C:\Program Files (x86)\Java\jdk1.7.0\bin”（还记得前面JDK安装到哪个目录吗？），点击“确定”按钮。然后用同样的方法新建一个CLASSPATH变量，暂时将值设置为“.”（英文句号）。为什么说CLASSPATH的值是暂时的，后面会解释。
![](http://hi.csdn.net/attachment/201109/9/0_1315548369PBG7.gif)![](http://hi.csdn.net/attachment/201109/9/0_1315548381229u.gif)
设置完成后，环境变量设置窗口如下图所示。点击确定按钮，环境变量设置完成。
![](http://hi.csdn.net/attachment/201109/9/0_1315548393Ga4C.gif)
注意：在Windows中，环境变量分为“用户变量”和“系统变量”，它们的区别是，“用户变量”只对当前的用户起作用，而“系统变量”则对系统中的所有用户起作用。如果希望在多个用户之间共享环境变量的设置，可以将这些环境变量设置为系统变量，否则，应该使用用户变量，避免影响其他用户。在Linux中也有类似的概念，接下来会讲到。
在Linux中，可以通过编辑“~/.bashrc”文件来修改环境变量。在最后添加下面几行脚本，然后保存并退出：
JAVA_HOME=/usr/jdk1.7.0
JAVA_BIN=/usr/jdk1.7.0/bin
PATH=$PATH:$JAVA_HOME/bin
CLASSPATH=.
export JAVA_HOME JAVA_BIN PATH CLASSPATH
![](http://hi.csdn.net/attachment/201109/9/0_1315548407EL6A.gif)
注意：Linux中，每个用户的home目录下都有.bashrc文件，这个文件用来保存用户的个性化设置，如命令别名、路径等，当然也可以用来定义环境变量。此文件是与每个用户相关的，一个用户的设置不会影响到其他用户，在这里设置环境变量相当于前面讲的Windows的用户环境变量。Linux中全局设置通常保存在“/etc/profile”文件中。
另外，Linux中PATH和CLASSPATH的分割符都是“:”（冒号），而Windows中是“;”（分号）。
当环境变量设置完成后，在Windows中打开新的命令行窗口，在Linux中使用“source ~/.bashrc”命令重新加载.bashrc文件，即可使新的环境变量生效。输入“java -version”命令，应该会打印出类似下面两幅图所示的内容：
Windows命令行的输出：
![](http://hi.csdn.net/attachment/201109/9/0_1315548434wZ2w.gif)
Linux的输出：
![](http://hi.csdn.net/attachment/201109/9/0_1315548446KZC8.gif)
**对以上步骤补充说明几点：**
- 可以在Windows命令行或Linux Shell中使用命令设置环境变量。例如，在Windows中可以使用“set var_name=some value”，在Linux中使用“var_name=some value”，这种方式与上面介绍的方式的区别在于：这种方式的设置是临时性的，当重新启动一个新的命令行窗口（Windows）或重新登录（Linux）后，这些临时变量就会丢失。
- JDK版本混乱：有时候，使用“java -version”命令可以打印出JDK的版本信息，但却与我们刚刚安装的JDK版本不一致。比如我们明明安装的是JDK 7，但却打印出JDK 6的版本信息，如下图所示：
![](http://hi.csdn.net/attachment/201109/9/0_1315548458XGfI.gif)
检查PATH变量，发现其中有一个路径为“C:\Program Files (x86)\Java\jdk1.6.0_25\bin”，原来我的系统中安装了两个版本的JDK，JDK 6和JDK 7。由于此JDK 6在系统环境变量PATH中，而Windows查找命令对应的程序时，首先查找的是系统变量，当找到了一个可用的java程序时，Windows将运行这个程序，而不再进一步查找。也就是说，系统PATH环境变量屏蔽了用户PATH环境变量。
不光如此，靠近PATH变量前部的路径中的程序将屏蔽其之后的路径中的同名程序。如同样是在系统PATH变量中，“C:\Program Files (x86)\Java\jdk1.6.0_25\bin;C:\Program Files (x86)\Java\jdk1.7.0\bin”，那么JDK 6仍然将屏蔽JDK 7，如果将它们的顺序交换：“C:\Program Files (x86)\Java\jdk1.7.0\bin;C:\Program Files (x86)\Java\jdk1.6.0_25\bin”，结果则相反。
不只是用户安装了多个版本的JDK时可能导致JDK版本的混乱，而且很多软件产品自身会包含JDK，即使用户只安装了一个JDK，但仍有可能与这些软件中的JDK互相屏蔽（如果这些软件同时也设置了环境变量的话）。例如Oracle数据库、MyEclipse等都包含自己的JDK。
在Windows下我还遇到过一个问题，那就是居然在Windows\system32目录下发现了java.exe、javaw.exe、javaws.exe三个文件，因为系统PATH变量中此目录处于较靠前的位置，因此很容易将用户自己安装的JDK屏蔽掉。
有三种方法来解决这个问题：
第一种方法是使用绝对路径，例如我们运行命令时使用“"C:\Program Files (x86)\Java\jdk1.7.0\bin\java.exe"”（当然，.exe可以省略）而不是“java”。使用绝对路径时，操作系统会直接根据路径定位到命令所在的目录，不再通过PATH变量来查找。这种方法的优点是绝对不会导致命令的覆盖，但缺点也很明显：必须输入完整的路径来运行命令（通常也很长）。
需要注意的是，当绝对路径中存在空格时（如上面的例子那样），需要将命令用英文双引号引起来。在设置PATH变量时不需要这样做，操作系统会自动完成这件事。
第二种方法是将自己安装的JDK路径设置到系统PATH变量的开头，这样，操作系统查找命令时就会最先查找我们设置的路径。但这种方法的缺点就是可能会影响其他用户（设置在了系统PATH变量中），并且可能会影响其他程序（其他的JDK被我们的屏蔽了）。
第三种方法就是设置一个新的环境变量，例如“JAVA_HOME”，将其值设置为我们安装的JDK的路径，如“C:\Program Files (x86)\Java\jdk1.7.0”，我们运行时，只需输入“"%JAVA_HOME%\bin\java"”即可（注意当路径中含有空格时要用双引号将命令引起来）。Apache Tomcat就使用这种方法。
**4. 编译并运行例子程序：**
经过了以上的步骤，JDK的环境就搭建好了，此时，可能需要再编译并运行一个Java例子程序来对刚搭建的环境做最终的检验。在这一节中，也会顺便讲到如何编译和运行一个Java程序，以及CLASSPATH的作用。更详细的，可以参考另一篇文章《JDK下提供的工具详解》。
此程序包含两个.java文件：ExceptionDemo.java和HelloWorldException.java，前者属于main包，而后者位于exceptions包（虽然它位于test\exceptions文件夹，这样安排的目的是为了更好地描述CLASSPATH的作用）：
![](http://hi.csdn.net/attachment/201109/9/0_1315548481TbMd.gif)
下面是它们的源代码：
(1) ExceptionDemo.java:
**[java]**[view
 plain](http://blog.csdn.net/antineutrino/article/details/6763565#)[copy](http://blog.csdn.net/antineutrino/article/details/6763565#)
- package main;  
- import exceptions.HelloWorldException;  
- publicclass ExceptionDemo {  
- /**
-        *
-        * @param args
-        * @throws HelloWorldException
-        */
- publicstaticvoid main(String[] args)  
- throws HelloWorldException {  
- thrownew HelloWorldException();  
-       }  
- }  
(2) HelloWorldException.java:
**[java]**[view
 plain](http://blog.csdn.net/antineutrino/article/details/6763565#)[copy](http://blog.csdn.net/antineutrino/article/details/6763565#)
- package exceptions;  
- publicclass HelloWorldException extends Exception {  
- privatestaticfinallong serialVersionUID = 8679349130620681877L;  
- public HelloWorldException() {  
- super("Hello World!");  
-       }  
- }  
此程序仍然是一个经典的HelloWorld程序（虽然这次它是以很不友好的方式向世界问好——通过抛出异常）。
要编译这个程序，首先尝试第一种方法（下面的操作是在Windows命令行下进行的，Linux与此类似）：进入src文件夹，输入“javac main\ExceptionDemo.java”，但编译报错：
![](http://hi.csdn.net/attachment/201109/9/0_1315548495Fgsx.gif)
为什么会提示找不到HelloWorldException呢？那是因为该Java文件位于“test\exceptions\”目录下，但它的包名却是“exceptions”，从当前的src目录，javac无法找到exceptions目录，因为“src\exceptions”目录是不存在的。
接下来，我们尝试第二种方法：由src目录进入test目录，运行“javac ..\main\Exceptiondemo.java”：
![](http://hi.csdn.net/attachment/201109/9/0_1315548505UG90.gif)
编译居然通过了！可以看到没有报错，并且main目录下生成了ExceptionDemo.class文件（Java字节码文件），说明编译确实成功了。但是为什么？我们使用了“..\main\Exceptiondemo.java”，这明显不是ExceptionDemo的包路径，为什么编译器却不报错呢？另外我们还注意到，我错误地将“ExceptionDemo.java”写成了“Exceptiondemo.java”，即将字母“D”的大小写弄错了，编译器同样没有报错。
原来，javac只是将“..\main\Exceptiondemo.java”当做普通路径来寻找Java源程序文件，找到后即开始编译此文件，而当其在编译过程中发现程序还引用了其他类时（如ExceptionDemo.java中引用了HelloWorldException类），就会暂停对当前文件的编译，开始寻找这个引用的类文件，如果未找到，那么将会报告错误，编译失败。前一种方法就是因为没有找到HelloWorldException类而出错的。
那么javac程序是如何查找程序引用的其他类的呢？答案是按照CLASSPATH指定的路径加上程序所引用类的包名来查找的。CLASSPATH默认为“.”，即当前路径（我们之前也设置了CLASSPATH的值为“.”，但即使不设置，javac也会默认以当前路径为起点来查找所引用的类文件）。
因此在这里javac会检查“src\test\exceptions\”中是否有HelloWorldException.class文件，如果有，则继续检查其中是否有HelloWorldException.java文件，如果两者都存在，则检查HelloWorldException.class是否比HelloWorldException.java更新，如果答案是肯定的，则加载HelloWorldException.class并继续编译ExceptionDemo.java。而如果比较结果是HelloWorldException.java更新，或者不存在HelloWorldException.class，则说明需要重新编译HelloWorldException.java文件。如果只有.class文件而不存在.java文件，则加载之并继续编译ExceptionDemo.java。
如果没有找到目标文件（HelloWorldException.class或HelloWorldException.java），那么javac将报告错误（如之前那样）。
也就是说，编译是递归进行的：当程序中引用了其他类时，javac会判断是否需要编译这些类，如果需要，则javac会首先编译它们，如果这些类再次用到了其他的类，javac将再次重复此过程，直到完成全部编译。只要在此过程中有任何类没有找到，或者在其中发现了任何错误，那么javac将报告错误并中止编译（javac可能在中止之前尽可能多地编译，以尽量多地向用户报告程序中的错误）。
可以用下面的图来形象地展示这一过程：
![](http://hi.csdn.net/attachment/201109/9/0_1315548515a1Dr.gif)
至于我们将大小写弄错了但javac却没有报错的原因，其实前面的说明已经隐含了解释：是因为javac只是将命令中的.java文件当做普通文件，又由于Windows是不区分大小写的，因此不会报错。如果换成Linux系统，将会提示文件无法找到的错误。
默认的CLASSPATH是当前目录（“.”），我们也可以设置为需要的路径，让javac据此查找类文件（这就是前面所说的为什么只是暂时将CLASSPATH设置为“.”的原因）。在这个例子中，我们设置CLASSPATH为“.;D:\workspaces\workspace_v1.1\my-test\src\test”，注意Linux中分隔符为“:”（冒号）。然后在src目录下就可以使用命令“javac main\ExceptionDemo.java”进行编译：
![](http://hi.csdn.net/attachment/201109/9/0_1315548529jm9S.gif)
实际上，此时在任何目录都可以对ExceptionDemo.java进行编译，只是文件的路径要适当更改。例如我们在D盘根目录输入以下命令编译：
![](http://hi.csdn.net/attachment/201109/9/0_1315548540XT50.gif)
这是因为设置了CLASSPATH后，javac总能找到HelloWorldException类。
有时候必须使用CLASSPATH：当涉及到的类很多时，而这些类并不在同一个目录下，此时我们只能使用CLASSPATH来指定这些类的路径——我们无法同时处于多个类的“当前目录”下。
另外一个需要注意的问题是，JDK包含的Java基础类（例如java.lang包中的类）并不需要指定CLASSPATH——Java知道如何找到它们。
编译完成后，运行我们的例子，例子将抛出一个异常，并向世界问好：
![](http://hi.csdn.net/attachment/201109/9/0_1315548550wmVR.gif)
注意，必须输入完整的包名和类名（不需要.class后缀），且大小写不能弄错（因为Java是区分大小写的）。完整的包名+类名在Java中称为类的完全限定名。
       Java环境配置的异常处理：
Java环境配置的异常处理：
Windows7X64安装“jdk-8u92-windows-x64.exe”后，在cmd命令行执行javac，老是提示：javac不是内部命令或外部命令，也不是可运行的程序或批处理文件。但是输入
 java -version能正常显示jdk版本号，说明java命令可用。我输入Java -home后，发现指向的路径完全不对，环境变量设置没起作用，不知是何原因。
     解决方案：运行——>cmd ：
在控制台中运行以下命令设置java环境变量即可解决：
set java_home=C:\Program Files\Java\jdk1.8.0_92  安装JDK的根目录
setclasspath=%JAVA_HOME%\lib\tools.jar;%JAVA_HOME%\lib\dt.jar;
set path=%JAVA_HOME%\bin; 
       注：输入不区分大小写，另外附上几个Java命令
java -help  可以查看命令
java -home  查看JDK安装路径
java -version  查看JDK版本
异常如下图：
![](https://img-blog.csdn.net/20160522192616302?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
配置正确如下图：
![](https://img-blog.csdn.net/20160522192605068?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
至此为止，我们成功地搭建起了Java开发和运行环境。
转载自：http://blog.csdn.net/antineutrino/article/details/6763565
