# eclipes（ADT）的配置和使用以及与AndroidStudio的对比 - Big Smile - CSDN博客
2016年10月09日 21:06:15[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1307
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
        最近经历了J2SE和软件开发工具的学习，对于eclipse的使用和java在eclipse中的使用有了很多的了解，并还要开发安卓的程序，所以学习了一些关于ADT和AndroidStudio的对于编辑Java代码的方面有些小心得，所以分享给大家。
# 一、eclipse的下载与配置
        首先我们要知道，eclipse和vs不一样的地方就是他是分64位版和32位版的，但是网上大多都流行的是32位版，所以会 造成一种假象，就是32位是通用版，其实不然，我们需要根据操作系统的位数来安装才可以，其实说是64位，就是一个通用版，在32位和64位操作系统上都可以用的版本。一般的文件名字见下图：
![](https://img-blog.csdn.net/20161009195523642?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        首先这里有个很有意思的东西就是eclipse在中文的意思就是天，日，日蚀的意思，代表了太空中的恒星，所以他的很多版本也都是以某个行星或者天上的人物，物件来命名的，比如上图的就是mars 火星版本的，还有一个是Juno（天神）版的：
![](https://img-blog.csdn.net/20161009195828677?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        这也是我们区分版本的一种方式，至于下载，在百度上搜索“eclipse x64”就有很多的下载资源。最新的（Neon）官网资源见：[https://www.eclipse.org/downloads/](https://www.eclipse.org/downloads/)
    下载完毕之后一般都是zip的压缩包，解压完成之后要先浏览一下里面的配置文件：eclipse.ini  文件，看里面的JavaVersion选项，以获得这个版本需要的最低版本的JDK，如图
![](https://img-blog.csdn.net/20161009200901304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
安装所需JDK，比写定的版本高的JDK也可以安装。如果没有发现这个文件，就需要上网上找到对应版本的这个配置文件，在网上搜星球的名字如Juno eclipse.ini 等，具体自己去找，或者下载配置好的。
安装好了JDK之后，我们需要进行常规的JAVA配置，如下：
在我的电脑上右键，属性，然后找到高级系统设置，点击环境变量打开环境变量配置
![](https://img-blog.csdn.net/20161009201824438?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
 在用户变量中需要添加Path下的JRE(java虚拟机)的bin路径，已经学过J2se的同学可能都配置过。注意不要将已经存在的路径删除，加上分号之后就可以直接添加新的路径。
![](https://img-blog.csdn.net/20161009202400499?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如图小编用的就是JDK1.7，所以带的jre也是7版本的。然后再配置classpath，在系统变量中，没有的自行添加；这个是java的运行编译的目录，写入.;%JAVA_HOME%\lib 就可以了。最后是需要配置JAVA_HOME这个变量，这个里面放置的就是JDK的安装目录（不是bin目录）如图：
![](https://img-blog.csdn.net/20161009202726669?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        然后就大功告成了，运行eclipse.exe就可以看到最开始的界面了，初次运行需要配置workSpace，就是放置程序文件的地方，找个可以方便找到的地方放置就可以了。
# 二、eclipse的内部界面以及java工程的创建
        如图就是eclipse的界面：
![](https://img-blog.csdn.net/20161009203241221?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        根据软件开发工具的书上的东西，中间的栏目就是透视图了，整个透视图分成了四块，分别是左边的资源管理透视图，就是显示工程文件的存放结构的地方，还有中间上方的编辑器，就是写代码的地方，下方是任务视图，包括自己写的任务注释的内容，要有调试控制台输出程序的地方，还有很多的东西可以在这里放置。最右边就是大纲视图，就是显示的打开的程序文件中的类，变量等东西的一般结构。
下面我们来创建第一个Java的工程，首先点击菜单栏的File项，之后new一个JavaProject，这样就是创建项目的第一步：
![](https://img-blog.csdn.net/20161009204001099?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后接着在出来的窗口中填写项目名称和使用的Java版本：
![](https://img-blog.csdn.net/20161009204125296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
填写完成之后我们有两个选项，一个是Next，一个是Finsh，如果选择Finsh的话，创项目结束，直接就可以开始编程了，如果选择的是Next，就会看到下图，可以定制自己的目录结构（默认只有一个src）：
![](https://img-blog.csdn.net/20161009204402507?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这个时候我们可以看到如下的目录：
![](https://img-blog.csdn.net/20161009204521677?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        这个时候不要慌，我们看到的东西其实和在C#中遇到的是类似的，首先src就是存放一般的类文件的地方的，整个项目都是如此，下面的Library就是这个工程用到的引用的jar包，也就相当于是C#中的引用的dll。
        我们右击src，新建一个类，叫做main，在创建类的界面可以设置引用和继承的类以及包和实现的接口。
![](https://img-blog.csdn.net/20161009204807165?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20161009205033903?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        之后写的东西就和在J2se中学习的一样了，直接就可以运行，结果在console框中，一般会有显示。
# 三、初识ADT
        什么是ADT呢？ADT的由来是用eclipse开发Android项目一个便捷的工具，起初要想在eclipse上开发Android应用需要安装安卓的一个插件，还要安装AndroidSDK，比较麻烦，所以为了方便开发，在谷歌的官方就有了一个专门的工具集，就是ADT里面包含了eclipse（打好插件），AndroidSDK，还有一些谷歌的支持。下载也是同理的，直接在百度上搜索adt-bundle-windows-x86_64 就可找到了，文后有下载地址。然后里面有一个eclipse的包，配置好环境之后直接运行eclipse就可以了，发现上面多了一些东西，如图：
![](https://img-blog.csdn.net/20161009210255451?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        这个是AndroidSDK和AVDmanager（虚拟机管理）,这个的作用就是可以管理和下载安卓的开发组件，然后使程序在安卓的虚拟机上运行了。
# 四、关于AndroidStudio
         对于安卓的开发，我不想说自己的意见，因为真的是两级分化的状态，首先，我查到的比较全面的资料就是用eclipse的ADT来开发安卓的，并且书上也是用的ADT来开发的，因为贴近于老的开发IDE，又用起来比较熟悉，所以用这个的人真的是不少。但是问题在于AndroidStudio是谷歌专门为了开发安卓而搞出来的IDE，虽然用起来比价费劲，但是对于安卓的兼容还有对于它的支持和运行都是很有帮助的，但是对于这里的初学者来说呢，全是英文的AS又是很有挑战性的，因为操作是在是需要适应很长一段时间，但是正像是VS一样，这个AS早晚也会成为开发安卓程序员的标配，所以多用用是有好处的。
        首先来看一下AS的界面：
![](https://img-blog.csdn.net/20161009234155305?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        皮肤好看了，功能看着也强大了很多，更重要的是对于莫名的问题也少了很多，比如本人用ADT的时候曾经因为SDK没有装好，就导致了各种报弹窗的问题，导致一度失去信心，所以，学好AS还是很有用处的。
