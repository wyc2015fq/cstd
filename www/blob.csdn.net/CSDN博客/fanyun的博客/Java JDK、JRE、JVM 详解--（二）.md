# Java JDK、JRE、JVM 详解--（二） - fanyun的博客 - CSDN博客
2016年03月24日 16:34:30[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1623
所属专栏：[Java框架精要](https://blog.csdn.net/column/details/java-01.html)

     JDK、JRE、JVM之间的关系：
     首先看看JDK与JRE的区别与联系，如下图所示：
![](https://img-blog.csdn.net/20160324164122908?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
   由图可知： JDK = JRE + Tools&Tool APIs
   JDK的核心是Java SE API。Java SE API是一些预定义的类库，开发人员需要用这些类来访问Java语言的功能。Java SE API包括一些重要的语言结构以及基本图形，网络和文件I/O。我们在自己的程序中，调用前辈们写好的这些Class，来作为我们自己开发的一个基础。当然，现在已经有越来越多的性能更好或者功能更强大的第三方类库供我们使用。一般来说，    Java API的非I/O部分对于运行Java的所有平台是相同的，而I/O部分则仅在通用Java环境中实现。
  JDK的种类：最主流的JDK是Sun公司发布的JDK，除了Sun之外，还有很多公司和组织都开发了自己的JDK，例如IBM公司开发的JDK，BEA公司的Jrocket，还有GNU组织开发的JDK等等。其中IBM的JDK包含的JVM（Java Virtual Machine）运行效率要比Sun JDK包含的JVM高出许多。而专门运行在x86平台的Jrocket在服务端运行效率也要比Sun JDK好很多。
  JRE（Java RuntimeEnvironment，Java运行环境），运行JAVA程序所必须的环境的集合。J2RE是Java2 Runtime Environment，即Java运行环境，有时简称JRE.如果你只需要运行Java程序或Applet，下载并安装它即可。如果你要自行开发 Java软件，请下载JDK. 在JDK中附带有JRE.注意由于Microsoft对Java的支持不完全，请不要使用IE自带的虚拟机来运行 Applet，务必安装一个JRE或JDK.包含：
  1. Java Runtime Environment（JRE）是可以在其上运行、测试和传输应用程序的Java平台。它包括Java JVM标准实现、Java平台核心类库和支持文件。它不包含开发工具——编译器、调试器和其它工具。
  JVM：Java Virtual Mechinal(JAVA虚拟机)。JVM是JRE的一部分，它是一个虚构出来的计算机，是通过在实际的计算机上仿真模拟各种计算机功能来实现的。JVM有自己完善的硬件架构，如处理器、堆栈、寄存器等，还具有相应的指令系统。JVM 的主要工作是解释自己的指令集（即字节码）并映射到本地的 CPU 的指令集或 OS 的系统调用。Java语言是跨平台运行的，其实就是不同的操作系统，使用不同的JVM映射规则，让其与操作系统无关，完成了跨平台性。JVM 对上层的 Java 源文件是不关心的，它关注的只是由源文件生成的类文件（
 class file ）。类文件的组成包括 JVM 指令集，符号表以及一些补助信息。
  2. JRE所需辅助软件——Java Plug-in——以便在浏览器中运行applet.
     一般情况下，如果您安装了JDK，那么你的电脑地下一定会有两套JRE、一套位于<jdk安装目录>\jre底下，另一套位于C:\Program File\JavaSoft底下（JDK1.4则是放在C:\Program File\Java底下）。如果你只下载了JRE而非JDK，那么就只会在C:\Program File\JavaSoft底下安装唯一的一套JRE。
由图可知JRE的地位就像一台PC一样，WIN32程序需要作业系统来帮我们执行，同样，Java应用程序需要JRE才能帮我们执行。当安装完JDK之后，如果分别在硬盘的不同地方安装了两套JRE，那么你可以想象你的电脑有两台虚拟的Java PC，都具有执行Java程序的功能。所以Java虚拟机只是JRE其中的一个成员而已，以更技术的角度来看，Java虚拟机只是JRE里头的一个动态连结函数库。
那么为什么会有两套JRE？真正原因是-JDK里面也附上了很多用Java写的开发工具（例如javac.exe、jar.exe等），而且它们都放置在<jdk安装目录\lib\tools.jar>之中。有人会问用Java编写的应用程序不是.class才对吗？其实在命令提示符中输入javac.exe和输入java -classpath
    d:\j2sdk1.4.0\lib\tools.jar com.sun.tools.javac.Main会得到相同的结果，javac.exe只是一个包装器而已。JDK还有很多开发工具采用包装器的概念，在<jdk安装目录\bin>中可以看到。
JDK里面的工具几乎都是用Java所撰写的，所以JDK本身就是Java应用程序，因此要使用JDK附的工具来开发Java程序，也必需要自行附一套JRE才行，这就是<jdk安装目录>\jre底下需要一套JRE的原因。而位于Program File\底下那套JRE就是哪来执行我们自己撰写的Java应用程序。不过，两套JRE都可以用来执行我们自己写的程序，可以JDK内附的开发工具预设置使用包装器启动的情况下，都会自己去选择用<jdk安装目录>\jre底下的那套JRE。
这样又有新的问题了，像myeclipse等开发工具也有集成JRE，那么电脑就有很多套JRE，同样执行java.exe有可能会得到不同的结果。
     那么我们执行的是那一个java.exe？
     当我们输入java XXX的时候，java.exe的工作就是找到合适的JRE来执行。Java.exe依照下面的逻辑来寻找JRE：
（1）自己的目录下有没有JRE目录
（2）父目录底下JRE子目录
（3）查询windows registry(HKEY_LOCAL_MACHINE\Software\JavaSoft\Java\Runtime Environment\)
     我们可以用path来指定执行哪个java.exe。当我们指定了path=d:\j2skd1.4.0\bin是，执行java.exe一定是d:\j2skd1.4.0\bin底下的java.exe，所以java.exe选到的是父目录d:\j2skd1.4.0底下的JRE。打开d:\j2skd1.4.0\jre\bin这个目录，您会看到client和server两个目录，里面都会分别看到jvm.dll，这就是Java虚拟机所在。
我们开发Java应用程序或是执行Java程序的时候，一定要记住两件事：
（1）那一个java.exe被执行。
（2）java.exe找到哪一套JRE。
附：
    JDK目录结构
    安装OS：win7 x86 
    安装路径： C:/Program Files/Java 
    JDK版本：jdk1.7.0_21
//---------------------------JDK开发文件和目录------------------------------------------------
              jdk1.7.0_21
     ___________|_______________________   
   |          |                     |   
    bin        lib                   jre   
   |          |            __________|_______________________   
java.exe    tools.jar       |                                 |           
javac.exe  dt.jar          bin                               lib                 
javap.exe           _________|____ __________        __________|_______ ________________           
javah.exe          |             |         |       |          |      |       |        |   
javadoc.exe    java.exe       client      server rt.jar      ext  security applet    fonts   
               java.dll          |          |  charsets.jar   |                     
               awt.dll        jvm.dll   jvm.dll          localedata.jar 
C:/Program Files/Java/jdk1.7.0_21 --JDK的根目录，包含一些软件版权，声明，和自述文件，同时包含归档了的Java平台源代码包src.zip      
C:/Program Files/Java/jdk1.7.0_21/src.zip --归档的Java源代码 
C:/Program Files/Java/jdk1.7.0_21/include --C 语言头文件 支持 用Java本地接口和Java虚拟机接口 来本机代码编程
C:/Program Files/Java/jdk1.7.0_21/lib -- Java开发工具要用的一些库文件，有包含了支持JDK工具的非核心类库tool.jar，dt.jar 归档的 BeanInfo 文件,用于告诉IDE这样显示java组件怎样让开发者在自己的应用程序中用户化它们 
================C:/ProgramFiles/Java/jdk1.7.0_21/jre================
C:/Program Files/Java/jdk1.7.0_21/jre -- JDK使用的Java运行环境（JRE）的根目录，这个运行环境实现了Java平台          
C:/Program Files/Java/jdk1.7.0_21/jre/bin --Java平台所要用的工具和库的可执行文件这些可执行文件和 /jdk1.7.0_21/bin相同的。这个路径不需要设置 PATH 环境变量 //Java 启动器工具充当了应用程序启动器(覆盖了1.1版本的JDK推出的旧版本JRE工具)
C:/Program Files/Java/jdk1.7.0_21/jre/bin/client -- 包含Java Hotspot(Java性能引擎) Client Virtual Machine 客户虚拟机要用的DLL文件 
C:/Program Files/Java/jdk1.7.0_21/jre/bin/server --包含Java Hotspot(Java性能引擎) Server Virtual Machine 服务器虚拟机要用的DLL文件 ----JDK 比 JREC:/Program Files/Java/jre7/bin多一个server端的java虚拟机。即这个folder “Server”不存在于JRE下。
C:/Program Files/Java/jdk1.7.0_21/jre/lib --JRE要用的代码库，属性设置，资源文件。 
C:/Program Files/Java/jdk1.7.0_21/jre/lib/rt.jar --Java 引导类库(java 核心APIRunTime类) 
C:/Program Files/Java/jdk1.7.0_21/jre/lib/charsets.jar --字符转换类库 
C:/Program Files/Java/jdk1.7.0_21/jre/lib/ext --默认的Java平台扩展安装环境 
C:/Program Files/Java/jdk1.7.0_21/jre/lib/ext/localedata.jar -- ava.text 和 java.util包要用到的地区数据  
C:/Program Files/Java/jdk1.7.0_21/jre/lib/security --包含安全管理文件，有安全规则(java.policy)和安全属性文件(java.security) 
C:/Program Files/Java/jdk1.7.0_21/jre/lib/applet --Java applets 要的Jar包，可以放到lib/applet/目录,可以节省 applet 类装载器从本地文件系统装载大的applets 所需的applet类时间,减少从网上下载具有相同的保护的时间。 
C:/Program Files/Java/jdk1.7.0_21/jre/lib/fonts --包含平台所需的TrueType字体文件 
================C:/ProgramFiles/Java/jdk1.7.0_21/db================
C:/Program Files/Java/jdk1.7.0_21/db  --db目录,纯Java开发的数据可 Derby，是一个开源的100%Java开发的关系数据库 
        有关 Java DB 的信息，请参见http://developers.sun.com/prodtech/javadb/。
        有关 Derby 的文档，请参见：http://db.apache.org/derby/manuals/index.html 
================C:/ProgramFiles/Java/jdk1.7.0_21/bin================
C:/Program Files/Java/jdk1.7.0_21/bin --JDK包含的一些开发工具执行文件 
C:/Program Files/Java/jdk1.7.0_21/bin/javac.exe --基本工具 - Java语言编译器，将Java源代码转换成字节码
C:/Program Files/Java/jdk1.7.0_21/bin/java.exe -- 基本工具 - Java应用程序启动器，直接从类文件执行Java应用程序字节代码
C:/Program Files/Java/jdk1.7.0_21/bin/javadoc.exe -- 基本工具 - Java API 文档生成器,从源码注释中提取文档
C:/Program Files/Java/jdk1.7.0_21/bin/apt.exe -- 基本工具 - java 注释处理器 
C:/Program Files/Java/jdk1.7.0_21/bin/appletviewer.exe -- 基本工具 - java applet 小程序浏览器，一种执行HTML文件上的Java小程序的Java浏览器。
C:/Program Files/Java/jdk1.7.0_21/bin/jar.exe -- 基本工具 - java文件压缩打包工具 
C:/Program Files/Java/jdk1.7.0_21/bin/jdb.exe -- 基本工具 - Java 调试器，debugger，查错工具
C:/Program Files/Java/jdk1.7.0_21/bin/javah.exe -- 基本工具 - C 头文件和stub生成器，用于写本地化方法，例如生产JNI样式的头文件。产生可以调用Java过程的C过程，或建立能被Java程序调用的C过程的头文件
C:/Program Files/Java/jdk1.7.0_21/bin/javap.exe -- 基本工具 - class文件反编译工具，显示编译类文件中的可访问功能和数据，同时显示字节代码含义。
C:/Program Files/Java/jdk1.7.0_21/bin/extcheck.exe -- 基本工具 - 用于检测jar包中的问题 
C:/Program Files/Java/jdk1.7.0_21/bin/keytool.exe-- 安全工具 - 管理密钥库和证书. 
C:/Program Files/Java/jdk1.7.0_21/bin/jarsigner.exe-- 安全工具 - 生产和校验JAR签名 
C:/Program Files/Java/jdk1.7.0_21/bin/policytool.exe-- 安全工具 - 有用户界面的规则管理工具   
C:/Program Files/Java/jdk1.7.0_21/bin/kinit.exe.exe-- 安全工具 - 用于获得和缓存网络认证协议Kerberos 票证的授予票证 
C:/Program Files/Java/jdk1.7.0_21/bin/klist.exe.exe-- 安全工具 - 凭据高速缓存和密钥表中的 Kerberos 显示条目  
C:/Program Files/Java/jdk1.7.0_21/bin/ktab.exe.exe-- 安全工具 - 密钥和证书管理工具  
C:/Program Files/Java/jdk1.7.0_21/bin/native2ascii.exe-- Java国际化工具 - 将文本转化为 Unicode Latin-1。详情参考http://java.sun.com/javase/6/docs/technotes/tools/windows/native2ascii.html 
C:/Program Files/Java/jdk1.7.0_21/bin/rmic.exe-- 远程方法调用工具 - 生成远程对象的stubs and skeletons(存根和框架) 
C:/Program Files/Java/jdk1.7.0_21/bin/rmid.exe-- 远程方法调用工具 - Java 远程方法调用(RMI:Remote Method Invocation) 活化系统守护进程 
C:/Program Files/Java/jdk1.7.0_21/bin/rmiregistry.exe-- 远程方法调用工具 - Java 远程对象注册表 
C:/Program Files/Java/jdk1.7.0_21/bin/serialver.exe-- 远程方法调用工具 - 返回类的 serialVersionUID.
C:/Program Files/Java/jdk1.7.0_21/bin/tnameserv.exe-- Java IDL and RMI-IIOP 工具 - Provides access to the naming service.  
C:/Program Files/Java/jdk1.7.0_21/bin/idlj.exe-- Java IDL and RMI-IIOP 工具 - 生产映射到OMG IDL接口可以使Java应用程序使用CORBA的.java文件 
C:/Program Files/Java/jdk1.7.0_21/bin/orbd.exe-- Java IDL and RMI-IIOP 工具 - 为客户可以在CORBA环境下透明的定位和调用服务器的稳定的对象提供支持 
C:/Program Files/Java/jdk1.7.0_21/bin/servertool.exe-- Java IDL and RMI-IIOP 工具 - 为应用程序提供易于使用的接口用于注册，注销，启动，关闭服务器 
C:/Program Files/Java/jdk1.7.0_21/bin/pack200.exe-- Java 部署工具 - 使用java gzip压缩工具将JAR文件转换为压缩的pack200文件，生产打包文件是高度压缩的JAR包，可以直接部署，减少下载时间 
C:/Program Files/Java/jdk1.7.0_21/bin/unpack200.exe-- Java 部署工具 - 解包pack200文件为JARs  
C:/Program Files/Java/jdk1.7.0_21/bin/htmlconverter.exe-- Java 插件工具 - Java Plug-in HTML转换器 htmlconverter -gui 可以启动图形界面 
C:/Program Files/Java/jdk1.7.0_21/bin/javaws.exe-- Java web 启动工具 - Java web 启动命令行工具 
C:/Program Files/Java/jdk1.7.0_21/bin/jvisualvm.exe-- Java 故障检修，程序概要分析，监视和管理工具 - 一个图形化的Java虚拟机，不说了大家研究一下就发现太酷了.这是想了解JVM的人的神器http://java.sun.com/javase/6/docs/technotes/guides/visualvm/index.html
C:/Program Files/Java/jdk1.7.0_21/bin/jconsole.exe-- Java 故障检修，程序概要分析，监视和管理工具 -java监视台和管理控制台，图形界面的功能太强大了，运行一下就知道，不想多说，看了就知道 
C:/Program Files/Java/jdk1.7.0_21/bin/schemagen.exe -- Java web 服务工具 - Java构架的XML Schema生成器 
C:/Program Files/Java/jdk1.7.0_21/bin/wsgen.exe -- Java web 服务工具 - 生成 JAX-WS 
C:/Program Files/Java/jdk1.7.0_21/bin/wsimport.exe-- Java web 服务工具 - 生成 JAX-WS 
C:/Program Files/Java/jdk1.7.0_21/bin/xjc.exe-- Java web 服务工具 - 绑定编译器  
C:/Program Files/Java/jdk1.7.0_21/bin/jps.exe-- 监视工具 - JVM Process Status 进程状态工具。列出目标系统的HotSpot JJVM , 监视Java虚拟机的性能，不支持Windows 98 和Windows ME 平台 
C:/Program Files/Java/jdk1.7.0_21/bin/jstat.exe-- 监视工具 - 按照命令行的具体要求记录和收集一个JVM的性能数据 
C:/Program Files/Java/jdk1.7.0_21/bin/jstatd.exe-- 监视工具 - JVM jstat 的守护进程 
C:/Program Files/Java/jdk1.7.0_21/bin/jinfo.exe-- 故障检测和修理工具 - 配置或打印某个Java进程VM flag 
C:/Program Files/Java/jdk1.7.0_21/bin/jhat.exe-- 故障检测和修理工具 - 堆储存查看器 
C:/Program Files/Java/jdk1.7.0_21/bin/jmap.exe -- 故障检测和修理工具 - Java内存图 
C:/Program Files/Java/jdk1.7.0_21/bin/jsadebugd.exe-- 故障检测和修理工具 - Java 的 Serviceability Agent Debug的守护进程 
C:/Program Files/Java/jdk1.7.0_21/bin/jstack.exe-- 故障检测和修理工具 - Java堆栈跟踪 
C:/Program Files/Java/jdk1.7.0_21/bin/jrunscript.exe-- Java脚本工具 - 运行脚本 
