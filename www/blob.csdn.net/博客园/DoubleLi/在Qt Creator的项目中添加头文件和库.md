# 在Qt Creator的项目中添加头文件和库 - DoubleLi - 博客园






在Qt Creator中的工程中，工程通过.pro文件管理。





额外需要连接的连接库
unix:LIBS += -L your_lib_path -lyour_lib
win32:LIBS += your_lib_path/your_lib


LIBS += -L./your_lib_path -lyour_libLIBS += ./your_lib.lib

额外需要的头文件目录
INCLUDEPATH += your_include_path





另外有一些比较重要的变量：



SOURCES：应用程序中所有源文件列表

HEADERS：应用程序中所有头文件列表

FORMS：应用程序中所有.ui文件列表



CONFIG：编译器使用的选项









