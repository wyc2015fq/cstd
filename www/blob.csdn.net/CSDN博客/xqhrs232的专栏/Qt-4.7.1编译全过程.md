# Qt-4.7.1编译全过程 - xqhrs232的专栏 - CSDN博客
2016年06月10日 00:08:04[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：790
原文地址::[http://blog.csdn.net/duloveding/article/details/8617943](http://blog.csdn.net/duloveding/article/details/8617943)
相关文章
1、Linux下QT4.7.1静态编译过程（原创） 
 ----[http://blog.163.com/jw_67/blog/static/136199256201112621247903/](http://blog.163.com/jw_67/blog/static/136199256201112621247903/)
2、Qt-4.7.2开发环境搭建及Micro2440的移植(笔记)----[http://www.cnblogs.com/cassvin/archive/2011/08/08/2130190.html](http://www.cnblogs.com/cassvin/archive/2011/08/08/2130190.html)
有用，转了。
**原文地址：**[Qt-4.7.1编译全过程](http://blog.sina.com.cn/s/blog_74ed1a770100p24g.html)**作者：**[康嘉星](http://blog.sina.com.cn/u/1961695863)
**1、从qt官网**
[http://get.qt.nokia.com/qt/source/qt-everywhere-opensource-src-4.7.1.tar.gz](http://get.qt.nokia.com/qt/source/qt-everywhere-opensource-src-4.7.1.tar.gz)下载最新版的qt-4.7.1
**2、首先将源文件解压到 /usr/local/arm（没有该目录可以新建一个，当然也可以是其他目录）目录中**
sudo mkdir/usr/local/arm
cd/usr/local/arm
tar –zxvf/homeqt-everywhere-opensource-src-4.7.1.tar.gz   //假设源文件在用户主目录中
这样在/usr/local/arm 目录中就存在了Qt-4.7.1文件夹
**3、进入qt目录进行编译**
因为需要开发arm平台程序，同时为了开发方便需要qvfb 工具，因此需要编译三个平台的库文件
A、arm 平台库用于编译arm开发板上 运行的程序
B、 x11平台（PC上运行程序的库）主要用于获得qvfb工具
C、x86平台库  主要用于编译在qvfb上跑的程序
**首先编译arm平台库：**
cd/usr/local/arm/Qt-4.7.1
./configure–embedded arm –xplatform qws/linux-arm-g++
开始会让选择“开源版”还是“商业版”，选o（开源版），下同。
主要就是这两个参数，当然可以根据自己的需要添加更多的参数比如-shared–debug 等等，如果进行库裁剪的话，可以考虑加上 –no-webkit–no-qt3support等参数
具体信息可用./configure –embedded–help 进行查看
make  //漫长的等待
makeinstall      //默认安装在/usr/local/Trolltech/QtEmbedded-4.7.1-arm中，当然也可以在./configure 中用-perfix 参数进行指定安装
光有arm库还不行，还需要交叉编译工具（arm-linux-g++），这里选择了arm-linux-gcc-4.3.2.tar.gz 可以去网上去下一个
只要将arm-linux-gcc-4.3.2.tar.gz 解压到/usr/local/arm目录中即可，最终生成4.3.2文件夹
（注意：必须与后面的arm平台交叉编译工具环境变量目录一致）
**继续编译x86平台**
cd/usr/local/arm/Qt-4.7.1
makeclean 
./configure -prefix/usr/local/Trolltech/Qt-4.7.1-x86 -nomake examples -nomake demos-nomake docs -nomake tools -embedded x86-qvfb
make  
makeinstall  
**编译x11平台**
cd/usr/local/arm/Qt-4.7.1
makeclean 主要针对前一次编译arm平台
./configure -nomakeexamples -nomake demos
make         //等待…..
makeinstall    //安装在/usr/local/Trolltech/Qt-4.7.1
**4、设置相应的环境变量**
A、arm平台
exportQTDIR=/usr/local/Trolltech/QtEmbedded-4.7.1-arm
exportQTEDIR=$QTDIR
exportPATH=$PATH:$QTDIR/bin
exportPATH=$PATH:/usr/local/arm/4.3.2/bin         //提供交叉编译工具arm-linux-g++
exportLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$QTDIR/lib
可以将上面几行写在一个文件中（比如 envarm），每次要用的时候 sourceenvarm 环境即可生效下面同样如此
B、x86平台
exportQTDIR=/usr/local/Trolltech/QtEmbedded-4.7.1
exportQTEDIR=$QTDIR
exportPATH=$PATH:$QTDIR/bin
exportLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$QTDIR/lib
C、x11平台
exportQTDIR=/usr/local/Trolltech/Qt-4.7.1
exportPATH=$PATH:$QTDIR/bin
exportLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$QTDIR/lib
  编译qvfb
**1) 将QT_SOURCE_TREE的目录设置到我们当前的目录上，这个变量qvfb.pro要用到，执行命令cd qt－x11-opensource-src-4.5.2$ qmake -set QT_SOURCE_TREE $PWD （）3)改动qvfb.pro中使用变量方式**
**[QT_SOURCE_TREE],执行命令为$ sed -i "s/QT_SOURCE_TREE/[QT_SOURCE_TREE]/g"tools/qvfb/qvfb.pro4）执行qmake,make编译，执行命令为$ cd tools/qvfb/   qmake   sudo make;5)最后qvfb编译成功，在qt－x11-opensource-src-4.5.2/bin目录下.把这个拷贝到/usr/local/Trolltech/4.7.1-x86/bin中，就大功告成了**
**qvfb编译错误**
2010-06-22 22:26
|g++ -c -pipe -O2 -Wall -W -D_REENTRANT -DQT_NO_DEBUG -DQT_GUI_LIB-DQT_CORE_LIB -DQT_SHARED -I../../mkspecs/linux-g++ -I.-I../../include/QtCore -I../../include/QtGui -I../../include -I.-I. -I. -o x11keyfaker.o x11keyfaker.cppx11keyfaker.cpp:48:34: error: X11/extensions/XTest.h:没有那个文件或目录x11keyfaker.cpp: In member function ‘voidX11KeyFaker::sendKeyEvent(int, bool)’:x11keyfaker.cpp:449: error: ‘XTestFakeKeyEvent’ was not declared inthis scopex11keyfaker.cpp:452: error: ‘XTestFakeKeyEvent’ was not declared inthis scopex11keyfaker.cpp:456: error: ‘XTestFakeKeyEvent’ was not declared inthis scopex11keyfaker.cpp:459: error: ‘XTestFakeKeyEvent’ was not declared inthis scopex11keyfaker.cpp:464: error: ‘XTestFakeKeyEvent’ was not declared inthis scopex11keyfaker.cpp: In member function ‘voidX11KeyFaker::connect()’:x11keyfaker.cpp:550: error: ‘XTestQueryExtension’ was not declaredin this scopemake: *** [x11keyfaker.o] 错误 1解决方法：sudo apt-get install xorg-dev修改Makefile 中 LIBS 选项： 添加 -lXtst选项。然后make  就OK了。opensource-src-4.4.3/tools/qvfb/qvfb.cpp:306: undefined referenceto `DeviceSkin::setSecondaryView(QWidget*)'' .obj/release-shared/x11keyfaker.o: Infunction 解决方法： 1。复制deviceskin.h，deviceskin.cpp到qvfb文件下。 2。在qvfb文件夹下手动生成qvfb工程，qmake qvfb.pro。 3。修改Makefile 中 LIBS 选项： 添加 -lXtst选项。编译。。|opensource-src-4.4.3/tools/qvfb/qvfb.cpp:306: undefined referenceto `DeviceSkin::setSecondaryView(QWidget*)'' .obj/release-shared/x11keyfaker.o: Infunction 解决方法： 1。复制deviceskin.h，deviceskin.cpp到qvfb文件下。 2。在qvfb文件夹下手动生成qvfb工程，qmake qvfb.pro。 3。修改Makefile 中 LIBS 选项： 添加 -lXtst选项。编译。。|
|----|----|
|opensource-src-4.4.3/tools/qvfb/qvfb.cpp:306: undefined referenceto `DeviceSkin::setSecondaryView(QWidget*)'' .obj/release-shared/x11keyfaker.o: Infunction 解决方法： 1。复制deviceskin.h，deviceskin.cpp到qvfb文件下。 2。在qvfb文件夹下手动生成qvfb工程，qmake qvfb.pro。 3。修改Makefile 中 LIBS 选项： 添加 -lXtst选项。编译。。| |
**具体操作：**
**source env86    //这里的env86即上面的x86平台的环境变量文件**
cd$QTDIR/demos/books
qvfb –width 640–height 480 &  //注意要将x11编译生成的qvfb程序拷贝到x86 的PATH中
./books-qws

//=================================================================================================================================
备注::
1>按Linux下QT4.7.1静态编译过程（原创）这个配置方法编译成功！！！----但编译出来的都是静态库！！！
 ./configure
 -release  -static -nomake examples -nomake demos -no-qt3support -no-script -no-scripttools -no-multimedia -no-audio-backend -no-openssl -no-opengl -no-webkit -no-phonon -no-sql-sqlite
2>

