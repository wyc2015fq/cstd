# Qt工程配置.pro详解--（如何正确的配置Qt工程） - fanyun的博客 - CSDN博客
2018年01月21日 20:34:01[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：2450
                
      编译QT的工程的时候，一般都会让qmake自动生成，但有时我们需要定制我们的工程，那么就必须改写pro文件。
要自己全部写pro文件又有点麻烦，所以，一般都是先运行
      qmake -project来生成基本的pro文件。
      在一般的.pro里已经包含了几个基本的选项，如TEMPLATE，TARGET，INCLUDEPATH，HEADS，SOURCES等选项，下面我们细讲：
（1）. 模板变量
TEMPLATE = app 
app -表示该工程建立一个应用程序的makefile。这是默认值，所以如果模板没有被指定，这个将被使用 ;
lib - 表示该工程建立一个库的makefile 
（2）.   生成目录
指定生成的可执行程序的目录，可以绝对和相对路径
DESTDIR += ../bin
（3）.  生成的应用程序名，即软件名
TARGET = qtest 
（4）.   编译选项 
增添或者移除某个模块 
比如增添C++11和console支持，Qt的各模块也在这里面添加
CONFIG += console c++11 network
CONFIG -= qt
可以用空格分隔或者”\”来隔行
（5）.目标文件目录
obj文件存放的目录，可以绝对或相对路径
OBJECTS_DIR += ../tmp
（6）.  工程中包含的头文件
表示本工程中的头文件
HEADERS += \
libcurl/curl/curl.h \
libcurl/curl/curlbuild.h \
libcurl/curl/curlrules.h \
只要qtcreator目录树里add进去了就自动添加 
 （7）. 工程中包含的源文件 
表示本工程中的源文件
SOURCES += mytest.cpp\
main.cpp
只要qtcreator目录树里add进去了就自动添加 
（8）.工程中包含的资源文件 
表示本工程包含的资源文件
RESOURCES += qrc/painter.qrc
只要qtcreator目录树里add进去了就自动添加，更改pro文件会同步更新qtcreator里的目录树，但不会影响硬盘上的资源文件 
（9）.附加头文件包含目录 
一般要编译链接第三方库时配置include目录 
例如
INCLUDEPATH += ./boost/include\
src/thirdpart/include
INCLUDEPATH += D:/boost/include\
E:/cocos2dx/include
1).可以用绝对或相对路径 2).相对路径中./可以省略 3).路径中不允许出现空格
（10）.链接库 
对于静态库（.lib, .a）以及动态库.so 
LIBS += -LD:/codetest/libcurlcpp/libcurl/lib/-llibcurl\
-LD:/codetest/libcurlcpp/libcurl/lib/-lws2_32\
-LD:/codetest/libcurlcpp/libcurl/lib/-lwldap32\
-LD:/codetest/libcurlcpp/libcurl/lib/-lopencv.so\
-LD:/codetest/libcurlcpp/libcurl/lib/-llibzmq.a
或者
LIBS += D:/codetest/libcurlcpp/libcurl/lib/libcurl.lib\
D:/codetest/libcurlcpp/libcurl/lib/ws2_32.lib\
D:/codetest/libcurlcpp/libcurl/lib/wldap32.lib
D:/codetest/libcurlcpp/libcurl/lib/opencv.so\
D:/codetest/libcurlcpp/libcurl/lib/opencv.a
或者
LIBS += $$PWD/libcurl/lib/libcurl.lib\
$$PWD/libcurl/lib/ws2_32.lib\
$$PWD/libcurl/lib/wldap32.lib
1).可以用-L指定目录，-l指定库的名称（无需后缀，.lib，.a, .so都适用）注意-l后面没有空格
2).可以直接通过绝对路径和相对路径指定库的完整path（需后缀，.lib，.a, .so都适用），相对路径用$$pwd前缀，路径不允许空格
3).LIBS += somepath 这个可以多行写，也可以用“\”隔行
对于动态库dll，必须拷贝到exe执行目录或者在环境变量里配置路径 
（11）.预编译宏 
DEFINES += USE_OPENSSL\
CURL_STATICLIB
类似于vs里面在工程里设置预编译选项 
（12）.平台相关性处理 
根据qmake所运行的平台来使用相应的作用域来进行处理
win32
{
SOURCES += 1.cpp
}else{
SOURCES += 2.cpp
}
根据平台进行条件编译 
（13）.指定来自uic的所有中间文件放置的目录
unix:UI_DIR = ../project/ui
win32:UI_DIR = D:/project/ui
（14）.指定包含用户界面翻译文本的翻译(.ts)文件列表
TRANSLATIONS += Resource/myapp_zh.ts \
                Resource/myapp_en.ts
（15）.指定的图标应该包含在一个生成的.rc文件里
如果RC_FILE
 和RES_FILE变量都没有设置这才可利用
RC_ICONS
 = logo.ico
（16）.“subdirs”模板
         subdirs”模板告诉qmake生成一个makefile，它可以进入到特定子目录并为这个目录中的项目文件生成makefile并且为它调用make。在这个模板中只有一个系统变量SUBDIRS可以被识别。这个变量中包含了所要处理的含有项目文件的子目录的列表。这个项目文件的名称是和子目录同名的，这样qmake就可以发现它。例如，如果子目里是“app1”，那么在这个目录中的项目文件应该被叫做app1.pro。
