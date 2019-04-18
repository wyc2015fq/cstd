# QT编译./configure参数的详细解释 - gauss的专栏 - CSDN博客
2013年01月03日 18:01:26[gauss](https://me.csdn.net/mathlmx)阅读数：297
个人分类：[客户端编程](https://blog.csdn.net/mathlmx/article/category/944323)
   终于翻译完了，我承认翻译的很差，甚至很多都有误解，但是没办法我只是新手+对QT的兴趣，有错一定要指出来，我马上编辑下，目的只有一个，方便后来的学习者，我的目标是将QT发扬光大。很喜欢QT！在此感谢老大[url=space.php?uid=3]shiroki[/url]和群里的热心人帮助，我才能解决各种缤纷多彩的编译错误
可以随便的转载，只要按照规矩走带上咱论坛的链接就好。
==================================
====全文是按照./configure -help来翻译的========
==================================
Usage:  configure [-h] [-prefix <dir>] [-prefix-install] [-bindir <dir>] [-libdir <dir>]
[-docdir <dir>] [-headerdir <dir>] [-plugindir <dir> ] [-datadir <dir>]
[-translationdir <dir>] [-sysconfdir <dir>] [-examplesdir <dir>]
[-demosdir <dir>] [-buildkey <key>]
这些是基本参数
[-release] [-debug][-debug-and-release] [-developer-build]
[-shared] [-static] [-no-fast] [-fast] [-no-largefile]
[-largefile] [-no-exceptions] [-exceptions] [-no-accessibility]
[-accessibility] [-no-stl] [-stl] [-no-sql-<driver>] [-sql-<driver>]
[-plugin-sql-<driver>] [-system-sqlite] [-no-qt3support] [-qt3support]
[-platform] [-D <string>] [-I <string>] [-L <string>] [-help]
这些是对QT所支持模块等设置的参数
[-qt-zlib] [-system-zlib] [-no-gif] [-qt-gif] [-no-libtiff] [-qt-libtiff] [-system-libtiff]
[-no-libpng] [-qt-libpng] [-system-libpng] [-no-libmng] [-qt-libmng]
[-system-libmng] [-no-libjpeg] [-qt-libjpeg] [-system-libjpeg] [-make <part>]
[-no-make <part>] [-R <string>]  [-l <string>] [-no-rpath]  [-rpath] [-continue]
[-verbose] [-v] [-silent] [-no-nis] [-nis] [-no-cups] [-cups] [-no-iconv]
[-iconv] [-no-pch] [-pch] [-no-dbus] [-dbus] [-dbus-linked]
[-no-separate-debug-info]
这些是第三方库的支持参数
[-no-mmx] [-no-3dnow] [-no-sse] [-no-sse2]
[-qtnamespace <namespace>] [-qtlibinfix <infix>] [-separate-debug-info]
[-armfpa][-no-optimized-qmake] [-optimized-qmake] [-no-xmlpatterns]
[-xmlpatterns][-no-phonon] [-phonon] [-no-phonon-backend]
[-phonon-backend][-no-openssl] [-openssl] [-openssl-linked]
[-no-gtkstyle] [-gtkstyle] [-no-svg] [-svg] [-no-webkit] [-webkit]
[-no-scripttools] [-scripttools]
专属嵌入式QTE编译的模块选择参数
安装选项：
以下选项都是可选，通过他们指定安装目录，不指定则用默认目录。选项前标记*的则为系统默认，+则为添加才有效那种
-prefix <dir> ......... 指定安装目录默认在 /usr/local/Trolltech/QtEmbedded-4.X.X
-hostprefix [dir] ..... 指定可扩展的工具库安装路径，默认当前目录
* -prefix-install ........ 指定QT的本地安装. 至
/usr/local/Trolltech/QtEmbedded-4.X.X, 此选项优先于-PREFIX.
以下选项用来指定分开安装不同部分，比如希望将可执行的放在/usr/bin 将库文件放在/lib下等等:
-bindir <dir> ......... 指定可执行文件路径 <dir>(默认 PREFIX/bin下)
-libdir <dir> .......... 指定库文件路径 <dir>(默认 PREFIX/lib)
-docdir <dir> ........ 指定文档路径 <dir>(默认 PREFIX/doc)
-headerdir <dir> .... 指定头文件路径 <dir>(默认 PREFIX/include)
-plugindir <dir> ...... 指定插件路径 <dir> (默认 PREFIX/plugins)
-datadir <dir> ........ 指定QT要使用到的一些数据文件所在路径 <dir> (默认 PREFIX)
-translationdir <dir> . 指定QT程序的翻译文件路径 <dir>(默认 PREFIX/translations)
-sysconfdir <dir> ..... 指定QT的设置文件路径 <dir>(默认 PREFIX/etc/settings)
-examplesdir <dir> .... 指定实例文件， <dir>(默认 PREFIX/examples)
-demosdir <dir> ....... 指定demos的安装路径 <dir> (默认 PREFIX/demos)
可以用这些选项去匹配自己所想要的插件
- buildkey <key> ...... 通过指定的key生成QT库和插件。
配置选项:
默认的选项(*),可选添加选项(+)
*  -release ........... 加上此选项编译程序QT不会做出优化，往往在开发过程使用此选项，是默认选项
-debug ............. 此选项在编译过程QT会对程序进行优化，在发布软件的时候应该用此选项
-debug-and-release . 编译两个版本
-developer-build.... 编译Qt的开发商和连接选项（包括自动测试出口的Qt）
-opensource......... 以开源版本发布程序
-commercial......... 以商业版本发布程序
*  -shared ............ 创建和使用共享Qt库。
-static ............ 创建和使用静态的Qt库。
*  -no-fast ........... 使用通常的Makefile来编译，是默认选项
-fast .............. 快速编译，除了封装必要的库，部分步骤省略或者用快速的方法处理
-no-largefile ...... 禁用大文件支持。
+  -largefile ......... 支持访问大于4GB的文件
*  -no-exceptions ..... 禁用编译器本身对异常的处理
-exceptions ........ 启用编译器本身对异常的处理
-no-accessibility ..
*  -accessibility .....   禁用/启用无障碍支持
-no-stl ............
*  -stl ...............   禁用/启用STL支持
-no-sql-<driver> ... 禁用SQL
-qt-sql-<driver> ... 根据需求启用SQL插件（默认全部关闭）
-plugin-sql-<driver> 禁用/启用/启用插件 对SQL的支持
-system-sqlite ..... 启用支持sqlite也可用-qt-sqlite
-no-qt3support .....
*  -qt3support ........ 禁用/启用qt3支持模块
-no-xmlpatterns ....
+  -xmlpatterns ....... 禁用/启用xmlpatterns模块
-no-phonon .........
+  -phonon ............ 禁用/启用Phonon模块（可以理解为声音控制模块）
-no-phonon-backend..
+  -phonon-backend..... 禁用/启用phonon-backend模块
-no-svg ............
+  -svg ............... 禁用/启用svg模块
-no-webkit .........
+  -webkit ............ 禁用/启用webkit模块（访问网络模块）
-no-scripttools ....
+  -scripttools ....... 禁用/启用scripttools模块
-platform target ... x86平台指定模块 (qws/linux-x86-g++).
-no-mmx ............ 我不知道这四个是什么，貌似是几个指令系统？
-no-3dnow ..........
-no-sse ............
-no-sse2 ...........
-qtnamespace <name>  指定所有库代码路径.
-qtlibinfix <infix>  重命名所有.so库文件到指定路径
-D <string> ........ 添加一个预处理器
-I <string> ........ 添加一个明确的头文件路径
-L <string> ........ 添加一个明确的库文件路径
-help, -h .......... 显示帮助信息
少说废话，多做实事～
第三方库选项:
（以下是第三方库，往往编译过程中需要依赖的都要加上，也没什么好说的，缺什么就加什么）
-qt-zlib ...........
+  -system-zlib .......      zlib依赖包 详询 [http://www.gzip.org/zlib](http://www.gzip.org/zlib)
-no-gif ............
*  -qt-gif ............         gif图片支持包
See also src/plugins/imageformats/gif/qgifhandler.h
-no-libtiff ........
-qt-libtiff ........         tiff支持插件
+  -system-libtiff ....     默认是-system调用系统的，出错的话可以尝试用-qt，qt的在编译过程不容易出错
详询 [http://www.libtiff.org](http://www.libtiff.org)
-no-libpng .........
-qt-libpng .........        png图片支持插件
+  -system-libpng .....
详询 [http://www.libpng.org/pub/png](http://www.libpng.org/pub/png)
-no-libmng .........
-qt-libmng .........       mng支持插件
+  -system-libmng .....
详询 [http://www.libmng.com](http://www.libmng.com)
-no-libjpeg ........
-qt-libjpeg ........         jpeg图片支持插件
+  -system-libjpeg ....
详询 [http://www.ijg.org](http://www.ijg.org)
-no-openssl ........
+  -openssl ...........        openssl支持插件
-openssl-linked ....
-ptmalloc ..........        系统内存分配器
少说废话，多做实事～
其他选项：
(扩展选项，模块的选择等，使编译更加人性化)
-make <part> .......    选择是否编译某些无关紧要的子模块
-nomake <part> ..... （例如不想编译系统自导的demos则可使用-nomake demos）
-R <string> ........      添加一个动态库
-l <string> ........       添加一个静态库
-no-rpath ..........
+  -rpath .............        是否将生成的库安装在软件的安装目录，默认是在安装目录，否则自己指定路径
-continue ..........        忽略错误一路前奔的编译，因为一般情况是按照模块逐个编译的，
碰到错误就停止。加上此参数是全部编译最后处理
-verbose, -v .......      显示每个步骤的详细信息
-silent ............        安静的编译省略信息回显
*  -no-optimized-qmake ...
-optimized-qmake ......  禁用/开启qmake的优化作用
-no-nis ............
*  -nis ...............         禁用/开启NIS的支持
-no-cups ...........
*  -cups ..............       禁用/开启对CUPS的支持
需要 cups/cups.h and libcups.so.2.
-no-iconv ..........
*  -iconv .............      禁用/开启对iconv的支持.
-no-pch ............
*  -pch ...............      禁用/开启是否使用预处理器头文件
-no-dbus ...........
+  -dbus ..............     禁用/开启对QTdbus模块的编译
-dbus-linked .......
-reduce-relocations ..... 减少因为链接优化而导致的而外的库文件重置
-no-separate-debug-info . 不将调试信息放在一个文件内
*  -separate-debug-info .... 将调试信息放在一个文件内
少说废话，多做实事～
以下参数仅限于嵌入式 QTE编译使用：
-xplatform target ...      交叉编译过程指定目标平台
-no-feature-<feature>   不编译<feature>.
-feature-<feature> ..    编译<feature>. 可选feature在src/corelib/global/qfeatures.txt文件内可查寻
-embedded <arch> ....   指定嵌入式平台的类型例如：arm mips x86 generic
-armfpa .............
-no-armfpa ..........       禁用/启用armfpa浮点格式支持
-little-endian ......         指定字节顺序为小字节序、低字节序
-big-endian .........        指定字节顺序为大字节序、高字节序
-host-little-endian .       指定主机平台字节顺序为小字节序、低字节序
-host-big-endian ....     指定主机平台字节顺序为大字节序、高字节序
注：只需指定目标平台的字节顺序，否则将会选用主机的字节顺序
arm的编译过程这项得加
-no-freetype ........
-qt-freetype ........      禁用/启用对freetype 模块的支持
*  -system-freetype ....
详询 [http://www.freetype.org/](http://www.freetype.org/)
-qconfig local ......      在不指定此配置的情况下系统使用
src/corelib/global/qconfig-local.h
-depths <list> ......     支持显示设备的位深度。有1, 4, 8, 12, 15, 16, 18, 24, 32 and 'all'.
-qt-decoration-<style> ....指定编译后的界面风格默认是windows
-plugin-decoration-<style> 指定插件风格默认windows
-no-decoration-<style> ....不知定风格采用默认选项
-no-opengl ..........
-opengl <api> .......      禁用/开启对opengl的支持
不指定型号则系统会自动检测
注意：使用此选项的前提条件是硬件支持且可以驱动
-qt-gfx-<driver> ...      指定显示设备驱动库
参考选项为：[ linuxfb transformed qvfb vnc multiscreen ]
-plugin-gfx-<driver> 指定显示插件驱动库
参考选项为：[  ahi directfb hybrid linuxfb powervr qvfb transformed vnc ]
-no-gfx-<driver> ... 不加载显示设备
参考选项为: [ linuxfb transformed qvfb vnc multiscreen ]
-qt-kbd-<driver> ... 指定图形界面下的键盘驱动库
参考选项为: [ tty usb sl5000 yopy vr41xx qvfb ]
-plugin-kbd-<driver> E指定图形界面下键盘插件驱动库
参考选项为: [  linuxis sl5000 usb vr41xx yopy ]
-no-kbd-<driver> ... 不加载指定驱动
参考选项为: [ tty usb sl5000 yopy vr41xx qvfb ]
-qt-mouse-<driver> ... 指定图形界面下鼠标驱动库
参考选项为: [ pc bus linuxtp yopy vr41xx tslib(触摸屏选这个) qvfb ]
-plugin-mouse-<driver> 鼠标插件驱动库
参考选项为: [  bus linuxis linuxtp pc tslib vr41xx yopy ]
-no-mouse-<driver> ... 指定不加载的鼠标驱动库模块
参考选项为: [ pc bus linuxtp yopy vr41xx tslib qvfb ]
-iwmmxt ............ 编译使用iwmmxt指令集，方便对某些XScale CPUs的支持
-no-glib ...........
+  -glib .............. 禁用/启用对glib的支持
为了您的安全，请只打开来源可靠的网址 
打开网站    取消
来自: [http://hi.baidu.com/396954504/blog/item/cb08bb380c92052f97ddd8be.html](http://hi.baidu.com/396954504/blog/item/cb08bb380c92052f97ddd8be.html)
