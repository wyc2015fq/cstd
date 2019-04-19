# 快速搭建Android开发环境ADT-Bundle及Hello World - xqhrs232的专栏 - CSDN博客
2015年11月12日 11:20:52[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：552
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/ds1121/article/details/41256619](http://blog.csdn.net/ds1121/article/details/41256619)
----下载JDK(Java Dev Kit)
官方下载：[http://www.oracle.com/technetwork/java/javase/downloads/index.html](http://www.oracle.com/technetwork/java/javase/downloads/index.html)
![](https://img-blog.csdn.net/20141118231622026)
![](https://img-blog.csdn.net/20141118231544937)
![](https://img-blog.csdn.net/20141118231952718)
![](https://img-blog.csdn.net/20141118232514268)
后续步骤：https://docs.oracle.com/javase/8/docs/
![](https://img-blog.csdn.net/20141118232644358)
安全后查看
![](https://img-blog.csdn.net/20141118232932261)
《JDK与JRE的不同以及含义》：http://www.pc6.com/infoview/Article_53547.html
1. JVM - java virtual machine！JVM是我们常说的java虚拟机，它是整个java实现跨平台的最核心部分，所有java程序会首先被编译为.class类文件在虚拟机上执行，也就是说class并不直接与机器的操作系统相对应，而是经过虚拟机间接与操作系统交互，由虚拟机将程序解释给本地系统执行.
2. JRE - java runtime environment！JRE是指Java运行环境，是面向Java程序的使用者而不是开发者. JRE如一台PC机一样，写好的Win32应用程序需要操作系统帮我们运行，编写的Java程序也同样要JRE才能运行. 光有JVM还不能完成.class类文件的执行，在解释.class时JVM需调用解释所需类库lib. 在JDK的安装目录里你可以找到jre目录，里面有两个文件夹bin和lib，可以认为bin里的就是JVM,lib中则是JVM工作所需类库，而JVM和类库合起来称为JRE.
 所以在写完java程序编译成class类文件之后，你可以把这个class文件和jre一起打包发给朋友，这样你的朋友就可以运行你写程序了（jre里有运行.class的java.exe）.
3. JDK - java development kit！JDK是面向开发人员使用的SDK（SDK是Software Development Kit 一般指软件开发工具包，可以包括函数库、编译程序等），它提供Java开发环境和运行环境.  JDK目录下有六个文件夹：src类库源码压缩包和其他几个声明文件，其中真正运行java时起作用的是以下四个文件夹：bin（最主要的是编译器javac.exe）、include（java和JVM交互用的头文件）、lib（类库）、 jre（Java运行环境）. 现在我们可以看出这样一个关系，JDK包含JRE，而JRE包含JVM（注意：JDK文件夹的bin、lib文件夹和JRE文件夹里的bin、lib是不同的，JDK是用于java程序的开发，而JRE则是只能运行class而没有编译的功能. 
JDK安装 首先直接解压JDK到c盘即可；然后就是配置环境（右击“计算机”，“属性”，“高级系统设置”，“环境变量”，请在环境变量的最上面的'XXX的用户变量'中配置环境【XXX指的是你给你电脑起的名】,点击“创建”开始配置，我JDK安装在G盘了请安装自己安装的路径参考下图自主配置 谢谢！）；
配置环境变量，桌面->计算机->右键属性->高级系统设置->高级->环境变量->adminstrator的用户变量(具体和自己用户名有关)，新建或者编辑一下的变量（如已经存在就追加，不存在就新建，追加时注意用;号与之前的隔开）
变量名： TEMP               变量值： %USERPROFILE%\AppData\Local\Temp   （请直接复制）
变量名： TMP                 变量值： %USERPROFILE%\AppData\Local\Temp   （请直接复制）
变量名： JAVA_HOME    变量值： C:\Program Files\Java\jdk1.8.0_25             （请不要在后面加";"）
变量名： CLASSPATH     变量值： .;%JAVA_HOME%\bin;
变量名： PATH               变量值： .;%JAVA_HOME%\bin;D:\Android\eclipse-jee-juno-R-win32\android-sdk-windows\tools（后面的是SDK的环境变量提前写上了）
变量名：JAVA_HOME
变量值：C:\Java\jdk1.7.0_17
变量名：ANDROID_SDK
变量值：C:\Java
变量名：Path
变量值：.;%JAVA_HOME%\bin;C:\Java\adt-bundle-windows-x86\sdk\tools;C:\Java\adt-bundle-windows-x86\sdk\platform-tools
变量名：CLASSPATH
变量值：.;%JAVA_HOME%\lib\tools.jar;%JAVA_HOME%\lib\dt.jar;%JAVA_HOME%\lib;（注意前面的.号）
![](https://img-blog.csdn.net/20141119002853890)
---测试环境变量
运行cmd，分别输入 "java -version" 和 "javac" 测试命令看成功与否；
![](https://img-blog.csdn.net/20141119005336671)
**-----------------下载ADT-Bundle for Windows**
[](http://developer.android.com/sdk/index.html)
ADT-Bundle 是由Google Android官方提供的集成式IDE，已经包含了Eclipse，Android SDK及Android所需要的平台工具，新手只需要 ADT-Bundle 即可快速搭建起Android开发环境。官方下载地址：[https://developer.android.com/sdk/index.html#download](http://developer.android.com/sdk/index.html)
**-----------------安装ADT-Bundle**
**直接把下载下来的的adt-bundle-windows-xxx-xxx**xxxxx**.zip，解压到你需要安装的位置。**
解压adt-bundle-windows-x86-20140702.zip(文件名可能因为版本，略有不一样)，内有三部分如下图：
![](https://img-blog.csdn.net/20141119004613677)
**---------------安装Android SDK**
运行SDK Manager.exe, [SDK
 Manager](http://developer.android.com/tools/help/sdk-manager.html)用于下载其他Android开发相关的组件（Eclipse如果装了ADT插件，也可以从Window > Android SDK Manager里运行），选择要下载安卓版本的API。
打开解压出来的文件夹里面的 **SDK Manager.exe**    （也可以运行eclipse/eclipse.exe 然后通过Windows -> Android SDK Manager打开）
![](https://img-blog.csdn.net/20141119012545343)
![](https://img-blog.csdn.net/20141119012527703)
![](https://img-blog.csdn.net/20141119013455000)
![](https://img-blog.csdn.net/20141119013150733)
![](https://img-blog.csdn.net/20141119013651002)
勾选上你想要开发的目标手机安卓版本，最好是全部勾上，然后点击Install Package 按提示 next，accept允许等待安装（这是一个慢长的过程）
**第五步：配置Android AVD（虚拟模拟器）**
**打开Eclipse**（eclipse/eclipse.exe），（运行启动的eclipse可能看上去有些不一样，可以通过windows把一些常用的窗口，工具条显示出来），然后从**Windows->Android Virtual Device Manger**打开AVD管理器。
点击 **New按钮** 创建你一新的AVD，如下图，我做了一个简单说明，参数其实你都是可以自己随意填的，点击OK就行了。
![](https://img-blog.csdn.net/20141119115251102)
![](https://img-blog.csdn.net/20130525233649167)
你配置的这个AVD是会做为你安卓应用的默认调试摸拟器，当然你也可以配置多个。
**------------------创建一个Hello World项目**
通过**File -> New-> Android Application Project**, 然后输入Hello World（项目名字），next，finish。
![](http://hiphotos.baidu.com/exp/pic/item/507c389759ee3d6def66e8b041166d224e4adeee.jpg)
运行Eclipse，新建一个Android Application Project，一路默认下去，就会看到自动生成了两个文件：一个是界面逻辑Java类，另一个是界面描述的XML文件。
Java类：
![](http://blog.itpub.net/attachment/201407/03/26687597_1404359926RrmM.jpg)
界面设计XML文件：你可以在这个界面修改修改。
![](http://blog.itpub.net/attachment/201407/03/26687597_1404359927txA4.jpg)
**-------------------从运行Hello World**
直接点击Run  Debug（快捷键F11）就可以在你配置的模拟器里运行了。
运行之前需要设置下Android AVD（ Android Virtual Device），即：Android虚拟设备。
![](http://blog.itpub.net/attachment/201407/03/26687597_1404359928FF7I.jpg)
这里还没有定义AVD，需要新建一个。
![](http://blog.itpub.net/attachment/201407/03/26687597_1404359928sGN4.jpg)
定义虚拟器的品牌，Android的版本，及相关的硬件配置。
![](http://blog.itpub.net/attachment/201407/03/26687597_1404359929C8nI.jpg)
![](https://img-blog.csdn.net/20141119012545343)
![](https://img-blog.csdn.net/20141119012527703)
![](https://img-blog.csdn.net/20141119011404046)
![](https://img-blog.csdn.net/20141119012357553)

