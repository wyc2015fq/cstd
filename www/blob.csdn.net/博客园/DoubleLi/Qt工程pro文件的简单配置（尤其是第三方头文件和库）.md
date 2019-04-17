# Qt工程pro文件的简单配置（尤其是第三方头文件和库） - DoubleLi - 博客园






Qt开发中，pro文件是对正工程所有源码、编译、资源、目录等的全方位配置的唯一方式，pro文件的编写非常重要，以下对几个重要项进行说明（win和linux，mac平台通用配置）

## 注释

以”#”开始的行，直到结束

## 模板变量

告诉qmake生成哪种makefile
`TEMPLATE = app`- 1

其中
- app -表示该工程建立一个应用程序的makefile。这是默认值，所以如果模板没有被指定，这个将被使用
- lib - 表示该工程建立一个库的makefile

## 生成目录

指定生成的可执行程序的目录，可以绝对和相对路径
`DESTDIR += ../bin`- 1

## 生成的应用程序名

软件名
`TARGET = qtest`- 1

## 编译选项

增添或者移除某个模块 
比如增添C++11和console支持，Qt的各模块也在这里面添加

```
CONFIG += console c++11 network
CONFIG -= qt
```
- 1
- 2

可以用空格分隔或者”\”来隔行

## 目标文件目录

obj文件存放的目录，可以绝对或相对路径
`OBJECTS_DIR += ../tmp`- 1

## 工程中包含的头文件

表示本工程中的头文件

```
HEADERS += \
    libcurl/curl/curl.h \
    libcurl/curl/curlbuild.h \
    libcurl/curl/curlrules.h \
```
- 1
- 2
- 3
- 4

只要qtcreator目录树里add进去了就自动添加

## 工程中包含的源文件

表示本工程中的源文件

```
SOURCES += mytest.cpp\
           main.cpp
```
- 1
- 2

只要qtcreator目录树里add进去了就自动添加

## 工程中包含的资源文件

表示本工程包含的资源文件
`RESOURCES += qrc/painter.qrc`- 1

只要qtcreator目录树里add进去了就自动添加，更改pro文件会同步更新qtcreator里的目录树，但不会影响硬盘上的资源文件

## 附加头文件包含目录

一般要编译链接第三方库时配置include目录 
例如

```
INCLUDEPATH += ./boost/include\
               src/thirdpart/include
```
- 1
- 2

```
INCLUDEPATH += D:/boost/include\
               E:/cocos2dx/include
```
- 1
- 2
- 可以用绝对或相对路径
- 相对路径中./可以省略
- 路径中不允许出现空格

## 链接库

对于静态库（.lib, .a）以及动态库.so

```
LIBS += -LD:/codetest/libcurlcpp/libcurl/lib/-llibcurl\
        -LD:/codetest/libcurlcpp/libcurl/lib/-lws2_32\
        -LD:/codetest/libcurlcpp/libcurl/lib/-lwldap32\
        -LD:/codetest/libcurlcpp/libcurl/lib/-lopencv.so\
        -LD:/codetest/libcurlcpp/libcurl/lib/-llibzmq.a
```
- 1
- 2
- 3
- 4
- 5

或者

```
LIBS += D:/codetest/libcurlcpp/libcurl/lib/libcurl.lib\
        D:/codetest/libcurlcpp/libcurl/lib/ws2_32.lib\
        D:/codetest/libcurlcpp/libcurl/lib/wldap32.lib
        D:/codetest/libcurlcpp/libcurl/lib/opencv.so\
        D:/codetest/libcurlcpp/libcurl/lib/opencv.a
```
- 1
- 2
- 3
- 4
- 5

或者

```
LIBS += $$PWD/libcurl/lib/libcurl.lib\
        $$PWD/libcurl/lib/ws2_32.lib\
        $$PWD/libcurl/lib/wldap32.lib
```
- 1
- 2
- 3
- 4
- 可以用-L指定目录，-l指定库的名称（无需后缀，.lib，.a, .so都适用）注意-l后面没有空格
- 可以直接通过绝对路径和相对路径指定库的完整path（需后缀，.lib，.a, .so都适用），相对路径用$$pwd前缀，路径不允许空格
- LIBS += somepath 这个可以多行写，也可以用“\”隔行

对于动态库dll，必须拷贝到exe执行目录或者在环境变量里配置路径

## 预编译宏

```
DEFINES += USE_OPENSSL\
           CURL_STATICLIB
```
- 1
- 2

类似于vs里面在工程里设置预编译选项

## 平台相关性处理

根据qmake所运行的平台来使用相应的作用域来进行处理

```matlab
win32
{
    SOURCES += 1.cpp
}
else
{
    SOURCES += 2.cpp
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8

![](http://static.blog.csdn.net/images/save_snippets.png)

根据平台进行条件编译









