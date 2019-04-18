# Qt .pro文件 详解 - gauss的专栏 - CSDN博客
2013年01月03日 18:11:15[gauss](https://me.csdn.net/mathlmx)阅读数：241
# Qt .pro文件 详解  突出 介绍libs
![](http://www.satsuns.com/wp-content/uploads/2011/01/QT.jpg)
**1. TEMPLATE**
变量TEMPLATE描述了为建立目标文件而采用何种模板,即生成何种形式的Makefile文件。Qmake
工具定义了5种模板:
1. 应用程序App,为建立一个Qt应用程序创建Makefile文件;
2. 库lib,为建立引用程序库而创建Makefile文件;
3. 子工程 subdirs,为建立子目录下的目标文件创建一个Makefile文件,
子目录通过变量SUBDIRS指定(子目录下的工程文件也需要指出使用何种模板);
4. VC应用程序vcapp,为Visual Studio 生成一个应用程序工程,仅仅用语Windos操作系统.
5. VC库vclib,为Visual Studio生成一个应用程序库工程,仅仅用语Windows操作系统.
{
app – 建立一个应用程序的makefile。这是默认值，所以如果模板没有被指定，这个将被使用。
lib – 建立一个库的makefile。
vcapp – 建立一个应用程序的Visual Studio项目文件。
vclib – 建立一个库的Visual Studio项目文件。
subdirs – 这是一个特殊的模板，它可以创建一个能够进入特定目录并且为一个项目文件生成makefile并且为它调用make的makefile。
“app”模板
“app”模板告诉qmake为建立一个应用程序生成一个makefile。当使用这个模板时，下面这些qmake系统变量是被承认的。你应该在你的.pro文件中使用它们来为你的应用程序指定特定信息。
}
**2.HEADERS - 应用程序中的所有头文件的列表。**
**3.SOURCES - 应用程序中的所有源文件的列表。**
**4.FORMS** / INTERFACES – 应用程序中的所有.ui文件（由Qt设计器生成）的列表。    eg.INTERFACES = filename.ui
**5.LEXSOURCES** – 应用程序中的所有lex源文件的列表。
**6.YACCSOURCES** – 应用程序中的所有yacc源文件的列表。
**7.TARGET** – 可执行应用程序的名称。默认值为项目文件的名称。（如果需要扩展名，会被自动加上。）
**8.DESTDIR** – 放置可执行程序目标的目录。
**9.DEFINES** – 应用程序所需的额外的预处理程序定义的列表。
**10.INCLUDEPATH **- 应用程序所需的额外的包含路径的列表（include文件路径列表）。
**11.DEPENDPATH** – 应用程序所依赖的搜索路径（描述了建立应用程序所依赖的其他文件所在的路 径）。
**12.VPATH** – 寻找补充文件的搜索路径。
**13.DEF_FILE **- 只有Windows需要：应用程序所要连接的.def文件。
**14.C_FIL**E - 只有Windows需要：应用程序的资源文件。
**15.RES_FILE** – 只有Windows需要：应用程序所要连接的资源文件。
**16.CONFIG变量**
配置变量指定了编译器所要使用的选项和所需要被连接的库。配置变量中可以添加任何东西，但只有下面这些选项可以被qmake识别。
下面这些选项控制着使用哪些编译器标志：
release – 应用程序将以release模式连编。如果“debug”被指定，它将被忽略。
debug – 应用程序将以debug模式连编。
warn_on – 编译器会输出尽可能多的警告信息。如果“warn_off”被指定，它将被忽略。
warn_off - 编译器会输出尽可能少的警告信息。
**注意事项：**
注：qmake -project 可以生成pro文件（可以根据项目需要，编辑改文件）
qmake 可以生成Makefile文件
make 编译
使用qmake -project时，会把本目录及其子目录内所有.cpp .h文件加入到项目输入文件中，使用时注意移去其他无用的文件。
qmake生成的Makefile文件，可以根据需要做相应修改。
