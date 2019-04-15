# Qt深入浅出（二）Qt编译机制

2018年02月24日 00:14:02 [吓人的猿](https://me.csdn.net/qq769651718) 阅读数：720



## 3 Qt的编译机制

### 3.1 编译过程



​	使用qmake.exe 工具解析.pro文件，然后生成Makefile, 通过makefile-32bit.exe解析Makefile文件来编译,可通过查看Qt Creator显示的编译信息。

![img](https://img-blog.csdn.net/20180224001234207)

先执行qmake命令解析class1_1.pro文件，生成Makefile.Debug文件

```
"C:\Qt\Qt5.7.0\5.7\mingw53_32\bin\qmake.exe" F:\workspace2\class1_1\class1_1.pro -r -spec win32-g++ "CONFIG+=debug" "CONFIG+=qml_debug"
```

- 然后执行mingw32-make命令解析Makefile.Debug，生成最终的exe文件

```
C:/Qt/Qt5.7.0/Tools/mingw530_32/bin/mingw32-make -f Makefile.Debug
```





### 3.2 qmake文件基本语法

​	qmake中一行代表一条规则，#代表注释。例如：QT += widgets，如果想分行书写，可以使用转义字符“\”，例如：

```
SOURCES += main.cpp \
            hello.cpp
```



​	qmake文件的常用变量:

```
QT          #指定要用的哪些Qt支持库
SOURCES     #源文件
HEADERS     #头文件
FORMS       #用到的ui文件（ui文件时用QT设计器生成的）
INCLUDEPATH #这个用来指定头文件路径, 例如：INCLUDEPATH += ./mitab
LIBS        #加载动态库。例如：LIBS += ./mitab/libmitab.so
DEFINES     #可以用来定义宏
CONFIG      #添加编译选项，例如: CONFIG += c++11使用c++11特性，CONFIG += console使用控制台
TRANSLATIONS#字典文件
TEMPLATE    #生成Makefile采用的模板。以下是可供选择的模板：
            #app：建立一个应用程序Makefile，这个也是默认值，没有指定模板是默认是这个。
            #lib：建立一个库的Makefile。
            #vcapp：建立一个应用程序的visual studio项目文件。
            #vclib：建立一个库的visual studio项目文件。
            #subdirs：创建一个能够进入子目录并且为之生成Makefile。需要在后面用SUBDIRS=XX来指定那些子目录。
```



- 使用qmake编写一个简单的应用

1. 新建文件夹helloworld，在文件夹中新建文件hello.pro，内容如下:

```
SOURCES += hello.cpp
CONFIG += console   #应用程序是一个windows下的控制台应用程序
```

1. 新建文件hello.cpp文件，内容如下：

```
#include <iostream>
using namespace std;
int main(void)
{
    cout << "helloworld" << endl;
    return 0;
}
```

1. 运行qt的DOS脚本

   ![img](https://img-blog.csdn.net/20180224001310307)

2. 在DOS命令行中输入

```
cd C:\helloworld        #切换当前路径到helloworld
c:\Qt\Qt5.7.0\5.7\mingw53_32\bin\qmake.exe hello.pro #执行qmake解析hello.pro文件
c:\Qt\Qt5.7.0\Tools\mingw530_32\bin\mingw32-make.exe -f Makefile #使用Makefile编译生成最终exe
```

1. 执行exe文件

```
cd release
hello.exe
```