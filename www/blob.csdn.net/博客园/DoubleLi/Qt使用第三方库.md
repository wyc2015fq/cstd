# Qt使用第三方库 - DoubleLi - 博客园






# 简述

在 Qt 中经常会用到第三方库，例如：FFmpeg、OpenCV 等。第三方库的使用比较简单，只需要一些基本的配置就可以搞定，一起来看看吧！




- [简述](http://blog.csdn.net/liang19890820/article/details/52933198#简述)
- [第三方库](http://blog.csdn.net/liang19890820/article/details/52933198#第三方库)
- [源代码](http://blog.csdn.net/liang19890820/article/details/52933198#源代码)
- [库文件](http://blog.csdn.net/liang19890820/article/details/52933198#库文件)
- [目标目录](http://blog.csdn.net/liang19890820/article/details/52933198#目标目录)





# 第三方库

假设你需要依赖一个跨平台的库 - MyLibrary，并且有几个文件作为库的一部分。我们的项目 MyQtApp 将这些文件存储在名为 3rdparty 的文件夹中：
- **MyQtApp/**
- **MyQtApp.pro**
- **src/**
- **main.cpp**

- **3rdparty/**
- **MyLibrary**
- **include/**
- MyLibrary.h

- **lib/**
- libMyLibrary.so
- MyLibrary.lib

- **bin/**
- MyLibrary.dll





要在 MyQtApp 中使用 MyLibrary 库，qmake 需要 MyLibrary 库的位置和名称。你也可以选择：
- 提供 MyLibrary 源代码的位置，以便在将它们包含在自己的代码中时，不必输入每个文件的完整路径。
- 选择 MyQtApp 可执行文件将被创建的目标目录。

上面的信息在 .pro 文件中提供，以便 qmake 可以解析它并生成 makefiles。Makefiles 包含编译器和链接器生成输出所需的所有信息，无论它是可执行文件，另一个库文件等。

# 源代码

使用 INCLUDEPATH 变量提供 MyLibrary 的 include 目录路径：
`INCLUDEPATH += 3rdparty/MyLibrary/include`
然后，就可以这样使用：
`#include <MyLibrary.h>`
而不是：
`#include <3rdparty/MyLibrary/include/MyLibrary.h>`
# 库文件

要让 qmake 知道在哪里可以找到 MyLibrary 库文件，使用 LIBS 变量：
`LIBS += -L"3rdparty/MyLibrary/lib" -lMyLibrary`
表达式的第一部分（`-L...`）：使链接器知道应该在哪个目录中查找库文件。双引号只在路径包含空格时才需要，因此此处可以省略。

表达式的第二部分（`-l...`）：告诉链接器链接哪些库。对于 Windows 和 UNIX 平台，有两个不同的库文件：MyLibrary.lib 和 libMyLibrary.so，没有必要指定 .lib 扩展名，也不必指定 lib 前缀（在 UNIX 平台上）。

# 目标目录

默认情况下，qmake 在与 .pro 文件相同的目录中创建可执行文件，可以使用 DESTDIR 变量来选择自己的目录：
`DESTDIR = bin`
就这么简单，现在可以在项目中使用 MyLibrary 库了。最终的 .pro 文件如下所示：

```
TARGET = MyQtApp

TEMPLATE = app

INCLUDEPATH += 3rdparty/MyLibrary/include

LIBS += -L"3rdparty/MyLibrary/lib" -lMyLibrary

SOURCES += src/main.cpp
```
- 

无论使用什么库，配置基本都大同小异，Good Luck！









