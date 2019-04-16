# QT pro文件介绍 - lyx的专栏 - CSDN博客





2015年11月03日 21:24:38[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：307标签：[qt																[配置文件](https://so.csdn.net/so/search/s.do?q=配置文件&t=blog)](https://so.csdn.net/so/search/s.do?q=qt&t=blog)
个人分类：[本科毕业设计](https://blog.csdn.net/qtlyx/article/category/5909007)








                
在QT中，有一个工具qmake可以生成一个makefile文件，它是由.pro文件生成而来的，.pro文件的写法如下：

1. 注释

从“#”开始，到这一行结束。

2. 指定源文件

SOURCES = *.cpp

对于多源文件，可用空格分开，如：SOURCES = 1.cpp 2.cpp3.cpp

或者每一个文件可以被列在一个分开的行里面，通过反斜线另起一行，就像这样:

SOURCES = hello.cpp

     main.cpp

一个更冗长的方法是单独地列出每一个文件，就像这样：

    SOURCES+= hello.cpp

    SOURCES +=main.cpp

这种方法中使用“+=”比“=”更安全，因为它只是向已有的列表中添加新的文件，而不是替换整个列表。

3. 指定头文件

HEADERS = hello.h或者HEADERS += hello.h

列出源文件的任何一个方法对头文件也都适用。

4. 配置信息

CONFIG用来告诉qmake关于应用程序的配置信息。

    CONFIG+= qt warn_on release

在这里使用“+=”，是因为我们添加我们的配置选项到任何一个已经存在中。这样做比使用“=”那样替换已经指定的所有选项是更安全的。

A> qt部分告诉qmake这个应用程序是使用Qt来连编的。这也就是说qmake在连接和为编译添加所需的包含路径的时候会考虑到Qt库的。

B> warn_on部分告诉qmake要把编译器设置为输出警告信息的。

C> release部分告诉qmake应用程序必须被连编为一个发布的应用程序。在开发过程中，程序员也可以使用debug来替换release

5. 指定目标文件名

TARGET = filename

如果不设置该项目，目标名会被自动设置为跟项目文件一样的名称

6. 添加界面文件(ui)

INTERFACES = filename.ui

7. 平台相关性处理

我们在这里需要做的是根据qmake所运行的平台来使用相应的作用域来进行处理。为Windows平台添加的依赖平台的文件的简单的作用域看起来就像这样：

win32 {

SOURCES += hello_win.cpp

}

所以如果qmake运行在Windows上的时候，它就会把hello_win.cpp添加到源文件列表中。如果qmake运行在其它平台上的时候，它会很简单地把这部分忽略。

8. 如果一个文件不存在，停止qmake

如果某一个文件不存在的 时候，你也许不想生成一个Makefile。我们可以通过使用exists()函数来检查一个文件是否存在。我们可以通过使用error()函数把正在运 行的qmake停下来。这和作用域的工作方式一样。只要很简单地用这个函数来替换作用域条件。对main.cpp文件的检查就像这样：

!exists( main.cpp ) {

  error( "No main.cpp file found")

}

“!”用来否定这个测试，比如，如果文件存在，exists( main.cpp)是真，如果文件不存在，!exists( main.cpp )是真。

9. 检查多于一个的条件

假设你使用Windows并且当你在命令 行运行你的应用程序的时候你想能够看到qDebug()语句。除非你在连编你的程序的时候使用console设置，你不会看到输出。我们可以很容易地把 console添加到CONFIG行中，这样在Windows下，Makefile就会有这个设置。但是如果告诉你我们只是想在当我们的应用程序运行在 Windows下并且当debug已经在CONFIG行中的时候，添加console。这需要两个嵌套的作用域；只要生成一个作用域，然后在它里面再生成 另一个。把设置放在最里面的作用域里，就像这样：

win32 {

  debug {

     CONFIG += console

  }

}

嵌套的作用域可以使用冒号连接起来，像这样：

win32:debug {

CONFIG += console

}

10. 摸板

模板变量告诉qmake为这个应用程序生成哪种makefile。下面是可供使用的选择：

A> app -建立一个应用程序的makefile。这是默认值，所以如果模板没有被指定，这个将被使用。

B> lib - 建立一个库的makefile。

C> vcapp - 建立一个应用程序的VisualStudio项目文件。

D> vclib - 建立一个库的VisualStudio项目文件。

E> subdirs -这是一个特殊的模板，它可以创建一个能够进入特定目录并且为一个项目文件生成makefile并且为它调用make的makefile。

11. 生成Makefile

当你已经创建好你的项目文件，生成Makefile就很容易了，你所要做的就是先到你所生成的项目文件那里然后输入：

Makefile可以像这样由“.pro”文件生成：

    qmake -oMakefile hello.pro

对于VisualStudio的用户，qmake也可以生成“.dsp”文件，例如：

    qmake -tvcapp -o hello.dsp hello.pro





 Qt pro工程文件 介绍



    分类：Qt/C++



app - 建立一个应用程序的makefile。这是默认值，所以如果模板没有被指定，这个将被使用。



lib - 建立一个库的makefile。



vcapp - 建立一个应用程序的Visual Studio项目文件。



vclib - 建立一个库的Visual Studio项目文件。



subdirs - 这是一个特殊的模板，它可以创建一个能够进入特定目录并且为一个项目文件生成makefile并且为它调用make的makefile。



“app”模板

“app”模板告诉qmake为建立一个应用程序生成一个makefile。当使用这个模板时，下面这些qmake系统变量是被承认的。你应该在你的.pro文件中使用它们来为你的应用程序指定特定信息。



HEADERS - 应用程序中的所有头文件的列表。



SOURCES - 应用程序中的所有源文件的列表。



FORMS - 应用程序中的所有.ui文件（由Qt设计器生成）的列表。



LEXSOURCES - 应用程序中的所有lex源文件的列表。



YACCSOURCES - 应用程序中的所有yacc源文件的列表。



TARGET - 可执行应用程序的名称。默认值为项目文件的名称。（如果需要扩展名，会被自动加上。）



DESTDIR - 放置可执行程序目标的目录。



DEFINES - 应用程序所需的额外的预处理程序定义的列表。



INCLUDEPATH - 应用程序所需的额外的包含路径的列表。



DEPENDPATH - 应用程序所依赖的搜索路径。



VPATH - 寻找补充文件的搜索路径。



DEF_FILE - 只有Windows需要：应用程序所要连接的.def文件。



RC_FILE - 只有Windows需要：应用程序的资源文件。



RES_FILE - 只有Windows需要：应用程序所要连接的资源文件。



CONFIG变量

配置变量指定了编译器所要使用的选项和所需要被连接的库。配置变量中可以添加任何东西，但只有下面这些选项可以被qmake识别。



下面这些选项控制着使用哪些编译器标志：



release - 应用程序将以release模式连编。如果“debug”被指定，它将被忽略。



debug - 应用程序将以debug模式连编。



warn_on - 编译器会输出尽可能多的警告信息。如果“warn_off”被指定，它将被忽略。



warn_off - 编译器会输出尽可能少的警告信息。



下面这些选项定义了所要连编的库/应用程序的类型：



qt - 应用程序是一个Qt应用程序，并且Qt库将会被连接。



thread - 应用程序是一个多线程应用程序。



x11 - 应用程序是一个X11应用程序或库。



windows - 只用于“app”模板：应用程序是一个Windows下的窗口应用程序。



console - 只用于“app”模板：应用程序是一个Windows下的控制台应用程序。



dll - 只用于“lib”模板：库是一个共享库（dll）。



staticlib - 只用于“lib”模板：库是一个静态库。



plugin - 只用于“lib”模板：库是一个插件，这将会使dll选项生效。



例如，如果你的应用程序使用Qt库，并且你想把它连编为一个可调试的多线程的应用程序，你的项目文件应该会有下面这行：



    CONFIG += qt thread debug注意，你必须使用“+=”，不要使用“=”，否则qmake就不能正确使用连编Qt的设置了，比如没法获得所编译的Qt库的类型了。






qmake高级概念

操作符



“=”操作符      分配一个值给一个变量

“+=”操作符     向一个变量的值的列表中添加一个值

“-=”操作符      从一个变量的值的列表中移去一个值

“*=”操作符      仅仅在一个值不存在于一个变量的值的列表中的时候，把它添加进去

“~=”操作符      替换任何与指定的值的正则表达式匹配的任何值 DEFINES ~= s/QT_[DT].+/QT

作用域

win32:thread {

        DEFINES += QT_THREAD_SUPPORT    } else:debug {        DEFINES += QT_NOTHREAD_DEBUG    } else {        warning("Unknown configuration")    }    }变量

到目前为止我们遇到的变量都是系统变量，比如DEFINES、SOURCES和HEADERS。你也可以为你自己创建自己的变量，这样你就可以在作用域中使用它们了。创建自己的变量很容易，只要命名它并且分配一些东西给它。比如：



    MY_VARIABLE = value你也可以通过在其它任何一个变量的变量名前加$$来把这个变量的值分配给当前的变量。例如：



   MY_DEFINES = $$DEFINESMY_DEFINES = $${DEFINES}

第 二种方法允许你把一个变量和其它变量连接起来，而不用使用空格。qmake将允许一个变量包含任何东西（包括$(VALUE)，可以直接在 makefile中直接放置，并且允许它适当地扩张，通常是一个环境变量）。无论如何，如果你需要立即设置一个环境变量，然后你就可以使用$$()方法。 比如：



    MY_DEFINES = $$(ENV_DEFINES)这将会设置MY_DEFINES为环境变量ENV_DEFINES传递给.pro文件地值。另外你可以在替换的变量里调用内置函数。这些函数（不会和下一节中列举的测试函数混淆）列出如下：



join( variablename, glue, before, after )

这 将会在variablename的各个值中间加入glue。如果这个变量的值为非空，那么就会在值的前面加一个前缀before和一个后缀after。只 有variablename是必须的字段，其它默认情况下为空串。如果你需要在glue、before或者after中使用空格的话，你必须提供它们。



member( variablename, position )

这将会放置variablename的列表中的position位置的值。如果variablename不够长，这将会返回一个空串。variablename是唯一必须的字段，如果没有指定位置，则默认为列表中的第一个值。



find( variablename, substr )

这将会放置variablename中所有匹配substr的值。substr也可以是正则表达式，而因此将被匹配。



    MY_VAR = one two three four    MY_VAR2 = $$join(MY_VAR, " -L", -L) -Lfive    MY_VAR3 = $$member(MY_VAR, 2) $$find(MY_VAR, t.*)MY_VAR2将会包含“-Lone -Ltwo -Lthree -Lfour -Lfive”，并且MYVAR3将会包含“three two three”。



system( program_and_args )

这将会返回程序执行在标准输出/标准错误输出的内容，并且正像平时所期待地分析它。比如你可以使用这个来询问有关平台的信息。



    UNAME = $$system(uname -s)    contains( UNAME, [lL]inux ):message( This looks like Linux ($$UNAME) to me )测试函数

qmake提供了可以简单执行，但强大测试的内置函数。这些测试也可以用在作用域中（就像上面一样），在一些情况下，忽略它的测试值，它自己使用测试函数是很有用的。



contains( variablename, value )

如果value存在于一个被叫做variablename的变量的值的列表中，那么这个作用域中的设置将会被处理。例如：



    contains( CONFIG, thread ) {        DEFINES += QT_THREAD_SUPPORT    }如果thread存在于CONFIG变量的值的列表中时，那么QT_THREAD_SUPPORT将会被加入到DEFINES变量的值的列表中。



count( variablename, number )

如果number与一个被叫做variablename的变量的值的数量一致，那么这个作用域中的设置将会被处理。例如：



    count( DEFINES, 5 ) {        CONFIG += debug    }error( string )

这个函数输出所给定的字符串，然后会使qmake退出。例如：



    error( "An error has occured" )文本“An error has occured”将会被显示在控制台上并且qmake将会退出。



exists( filename )

如果指定文件存在，那么这个作用域中的设置将会被处理。例如：



    exists( /local/qt/qmake/main.cpp ) {        SOURCES += main.cpp    }如果/local/qt/qmake/main.cpp存在，那么main.cpp将会被添加到源文件列表中。



注意可以不用考虑平台使用“/”作为目录的分隔符。



include( filename )

项目文件在这一点时包含这个文件名的内容，所以指定文件中的任何设置都将会被处理。例如：



     include( myotherapp.pro )myotherapp.pro项目文件中的任何设置现在都会被处理。



isEmpty( variablename )

这和使用count( variablename, 0 )是一样的。如果叫做variablename的变量没有任何元素，那么这个作用域中的设置将会被处理。例如：



    isEmpty( CONFIG ) {        CONFIG += qt warn_on debug    }message( string )

这个函数只是简单地在控制台上输出消息。



    message( "This is a message" )文本“This is a message”被输出到控制台上并且对于项目文件的处理将会继续进行。



system( command )

特定指令被执行并且如果它返回一个1的退出值，那么这个作用域中的设置将会被处理。例如：



    system( ls /bin ) {        SOURCES += bin/main.cpp        HEADERS += bin/main.h    }所以如果命令ls /bin返回1，那么bin/main.cpp将被添加到源文件列表中并且bin/main.h将被添加到头文件列表中。



infile( filename, var, val )

如果filename文件（当它被qmake自己解析时）包含一个值为val的变量var，那么这个函数将会返回成功。你也可以不传递第三个参数（val），这时函数将只测试文件中是否分配有这样一个变量var。



以下为我的一个项目举例

# 项目目标：为一个库文件



TEMPLATE = lib# 编译项目文件所需头文件的路径INCLUDEPATH += ../common .# 目标文件路径DESTDIR=../lib# 条件依赖：Unix平台上 定义本想目的 UI目录， MOC目录， 目的目录unix {  UI_DIR = ../.ui  MOC_DIR = ../.moc  OBJECTS_DIR = ../.obj}# 本项目配置：CONFIG         += qt warn_on release thread# Input  头文件，源文件HEADERS
 += COMControllerThread.h            DecodeSMS.h            monitor_common.h            monitor_interface.h            MonitorThread.h            UserEvent.h            MyCOM.h            MySMS.h            MyTagHandle.h            SMSParseThread.h        
    tag_dict.hSOURCES += COMControllerThread.cpp            DecodeSMS.cpp            monitor_common.cpp            monitor_interface.cpp            MonitorThread.cpp            MyCOM.cpp            MySMS.cpp            MyTagHandle.cpp            SMSParseThread.cpp
            tag_dict.cpp注：qmake -project 可以生成pro文件（可以根据项目需要，编辑改文件）

qmake 可以生成Makefile文件

make 编译

使用qmake -project时，会把本目录及其子目录内所有.cpp .h文件加入到项目输入文件中，使用是注意移去其他无用的文件。

qmake生成的Makefile文件，可以根据需要做相应修改
            



