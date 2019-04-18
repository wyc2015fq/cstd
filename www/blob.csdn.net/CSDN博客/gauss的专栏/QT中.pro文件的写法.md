# QT中.pro文件的写法 - gauss的专栏 - CSDN博客
2013年01月03日 18:10:34[gauss](https://me.csdn.net/mathlmx)阅读数：206
个人分类：[客户端编程](https://blog.csdn.net/mathlmx/article/category/944323)
                
  在QT中，有一个工具qmake可以生成一个makefile文件，它是由.pro文件生成而来的，.pro文件的写法如下：
1. 注释
从“#”开始，到这一行结束。
2. 指定源文件
SOURCES = *.cpp
对于多源文件，可用空格分开，如：SOURCES = 1.cpp 2.cpp3.cpp
或者每一个文件可以被列在一个分开的行里面，通过反斜线另起一行，就像这样:
SOURCES = hello.cpp 
     main.cpp
一个更冗长的方法是单独地列出每一个文件，就像这样：
    SOURCES+= hello.cpp
    SOURCES +=main.cpp
这种方法中使用“+=”比“=”更安全，因为它只是向已有的列表中添加新的文件，而不是替换整个列表。
3. 指定头文件
HEADERS = hello.h或者HEADERS += hello.h
列出源文件的任何一个方法对头文件也都适用。
4. 配置信息
CONFIG用来告诉qmake关于应用程序的配置信息。
    CONFIG+= qt warn_on release
在这里使用“+=”，是因为我们添加我们的配置选项到任何一个已经存在中。这样做比使用“=”那样替换已经指定的所有选项是更安全的。
A> qt部分告诉qmake这个应用程序是使用Qt来连编的。这也就是说qmake在连接和为编译添加所需的包含路径的时候会考虑到Qt库的。
B> warn_on部分告诉qmake要把编译器设置为输出警告信息的。
C> release部分告诉qmake应用程序必须被连编为一个发布的应用程序。在开发过程中，程序员也可以使用debug来替换release
5. 指定目标文件名
TARGET = filename
如果不设置该项目，目标名会被自动设置为跟项目文件一样的名称
6. 添加界面文件(ui)
INTERFACES = filename.ui
7. 平台相关性处理
我们在这里需要做的是根据qmake所运行的平台来使用相应的作用域来进行处理。为Windows平台添加的依赖平台的文件的简单的作用域看起来就像这样：
win32 {
SOURCES += hello_win.cpp
}
所以如果qmake运行在Windows上的时候，它就会把hello_win.cpp添加到源文件列表中。如果qmake运行在其它平台上的时候，它会很简单地把这部分忽略。
8. 如果一个文件不存在，停止qmake
如果某一个文件不存在的时候，你也许不想生成一个Makefile。我们可以通过使用exists()函数来检查一个文件是否存在。我们可以通过使用error()函数把正在运行的qmake停下来。这和作用域的工作方式一样。只要很简单地用这个函数来替换作用域条件。对main.cpp文件的检查就像这样：
!exists( main.cpp ) {
  error( "No main.cpp file found")
}
“!”用来否定这个测试，比如，如果文件存在，exists( main.cpp)是真，如果文件不存在，!exists( main.cpp )是真。
9. 检查多于一个的条件
假设你使用Windows并且当你在命令行运行你的应用程序的时候你想能够看到qDebug()语句。除非你在连编你的程序的时候使用console设置，你不会看到输出。我们可以很容易地把console添加到CONFIG行中，这样在Windows下，Makefile就会有这个设置。但是如果告诉你我们只是想在当我们的应用程序运行在Windows下并且当debug已经在CONFIG行中的时候，添加console。这需要两个嵌套的作用域；只要生成一个作用域，然后在它里面再生成另一个。把设置放在最里面的作用域里，就像这样：
win32 {
  debug {
     CONFIG += console
  }
}
嵌套的作用域可以使用冒号连接起来，像这样：
win32:debug {
CONFIG += console
}
10. 摸板
模板变量告诉qmake为这个应用程序生成哪种makefile。下面是可供使用的选择：
A> app -建立一个应用程序的makefile。这是默认值，所以如果模板没有被指定，这个将被使用。
B> lib - 建立一个库的makefile。
C> vcapp - 建立一个应用程序的VisualStudio项目文件。
D> vclib - 建立一个库的VisualStudio项目文件。
E> subdirs -这是一个特殊的模板，它可以创建一个能够进入特定目录并且为一个项目文件生成makefile并且为它调用make的makefile。
11. 生成Makefile
当你已经创建好你的项目文件，生成Makefile就很容易了，你所要做的就是先到你所生成的项目文件那里然后输入：
Makefile可以像这样由“.pro”文件生成：
    qmake -oMakefile hello.pro
对于VisualStudio的用户，qmake也可以生成“.dsp”文件，例如：
    qmake -tvcapp -o hello.dsp hello.pro
