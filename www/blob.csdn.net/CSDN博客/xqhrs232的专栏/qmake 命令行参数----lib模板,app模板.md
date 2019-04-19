# qmake 命令行参数----lib模板,app模板 - xqhrs232的专栏 - CSDN博客
2017年03月23日 11:44:04[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：651
原文地址::[http://blog.csdn.net/qust_mr_chen8/article/details/43743299](http://blog.csdn.net/qust_mr_chen8/article/details/43743299)
暂定主题：**qmake之命令行参数及qmake启动时加载了什么东西**，或许可以借此学习一下qmake是处理命令行参数的方法
## 命令行参数
运行 qmake -h 可以得到qmake大部分命令行参数及其说明。本文预计涉及到的参数如下：
|-project|工作于何种模式，project用来生成pro文件。默认是makefile模式|
|----|----|
|-makefile| |
|-spec spec-platform spec|使用何种spec？ 本文重点|
|-norecursive-nr|是否递归处理.pro文件，对于subdirs的层层嵌套的Qt工程来说很常用|
|-recursive-r| |
|-t templ|指定工程类型，app、lib还是其他，-tp添加前缀，比如：“-tp vc”，对于app工程来说，就是vcapp|
|-tp prefix| |
|-set <prop> <value>|设置、清除、查询属性值，属于上一篇的重点内容|
|-unset <prop>| |
|-query| |
|-cache file|控制cache文件的使用|
|-nocache| |
## 运行qmake
qmake -spec linux-g++-32 test.pro
按理说：要生成makefile
- 总要有个工程文件吧？
- 总要指定所用的平台和编译器等环境吧？
那么，-spec [Linux](http://lib.csdn.net/base/linux)-g++-32 究竟有什么用呢？实际上，是通知qmake去加载一个文件:
$QTDIR/mkspecs/linux-g++-32/qmake.conf
不妨看一眼其内容(从这儿你可以看出，CONFIG中默认启用qt，以及Qt默认有core和gui两个模块)：
#
# qmake configuration for linux-g++
#
MAKEFILE_GENERATOR      = UNIX
TARGET_PLATFORM         = unix
TEMPLATE                = app
CONFIG                  += qt warn_on release incremental link_prl gdb_dwarf_index
QT                      += core gui
QMAKE_INCREMENTAL_STYLE = sublib
QMAKE_CFLAGS            = -m32
QMAKE_LFLAGS            = -m32
include(../common/linux.conf)
include(../common/gcc-base-unix.conf)
include(../common/g++-unix.conf)
load(qt_config)
可是，事实上，运行qmake时，我们很多时候，只需要
qmake
原因何在？
### 省略 xxx.pro
一旦命令行参数中没有指定 .pro文件，那么qmake采取的策略还是很简单的：
- 如果有和目录同名的 .pro 文件，则使用
- 反之，如果目录中只有一个 .pro结尾的文件，则使用
- 反之，出错。
源码：
static QString detectProjectFile(const QString &path)
{
    QString ret;
    QDir dir(path);
    if(dir.exists(dir.dirName() + Option::pro_ext)) {
        ret = dir.filePath(dir.dirName()) + Option::pro_ext;
    } else { //last try..
        QStringList profiles = dir.entryList(QStringList("*" + Option::pro_ext));
        if(profiles.count() == 1)
            ret = dir.filePath(profiles.at(0));
    }
    return ret;
}
### 如何确定mkspec
qmake 如何确定 mkspec 呢？
啰嗦一点，**下面的内容或许可作为以后讨论其他内容的基础**。
- 环境变量QMAKEFLAGS
可以设置环境变量 QMAKEFLAGS ，该变量的值将直接作为qmake的参数使用
如果qmake的参数与QMAKEFLAGS指定的冲突，qmake的命令行参数起作用。
- 环境变量QMAKESPEC
如果 QMAKEFLAGS 和命令行参数中都没有指定 mkspec，那么将尝试查找环境变量 QMAKESPEC
- cache
你可能见过 .qmake.cache 这种文件...
如果 QMAKESPEC 也是空的，那么将判断cache中是否设置了 mkspec。
这个过程相当复杂，即使我们确定了mkspec，还有很多工作要做。
### 查找mkspec
假定我们已经知道了使用了mkspec是 linux-g++-32
qmake 何处去查找相应的mkspec文件(比如我们一开始列出的qmake.conf)？？
QStringList qmake_mkspec_paths()
{
    QStringList ret;
    const QString concat = QDir::separator() + QString("mkspecs");
    QByteArray qmakepath = qgetenv("QMAKEPATH");
    if (!qmakepath.isEmpty()) {
        const QStringList lst = splitPathList(QString::fromLocal8Bit(qmakepath));
        for(QStringList::ConstIterator it = lst.begin(); it != lst.end(); ++it)
            ret << ((*it) + concat);
    }
    ret << QLibraryInfo::location(QLibraryInfo::DataPath) + concat;
    return ret;
}
环境变量QMAKEPATH以及配置参数QLibraryInfo::[DataPath](http://127.0.0.1/wiki/DataPath)指定目录下的“mkspecs”子目录！
如果我们此时没有设置尚未设置 mkspec，它会去查找前面这些目录中有没有default的子目录，有则作为mkspec文件的目录！比如，在我的系统的QTDIR/mkspecs目录下，有一个default的链接
...
drwxr-xr-x  2 debao debao 4096 2011-06-22 23:41 cygwin-g++
drwxr-xr-x  2 debao debao 4096 2011-06-22 23:41 darwin-g++
lrwxrwxrwx  1 debao debao    9 2011-08-27 17:07 default -> linux-g++
drwxr-xr-x  6 debao debao 4096 2011-08-27 15:29 features
...
## .qmake.cache
cache 文件的定位：
看个目录结构(shadow build的经典结构)：
|----test\
|      `---- test.pro
|
|----test-build\
       |
       `--- .qmake.cache
两个文件的内容如下：
- test.pro
message($$CONFIG)- .qmake.cache
CONFIG += dbzhang800
运行qmake ../test/test.pro，结果大致如下：
Project MESSAGE: lex yacc warn_on debug uic resources qt warn_on release incremental link_prl gdb_dwarf_index def_files_disabled exceptions no_mocdepend debug stl qt_no_framework dbzhang800 release
可以看出，cache文件在起作用。
注意：如果 当前目录中没有 .qmake.cache，则会往上级目录查找，直到根目录！！例如(查找顺序)：
/home/debao/qmake-test/test-build/.qmake.cache
/home/debao/qmake-test/.qmake.cache
/home/debao/.qmake.cache
/home/.qmake.cache
/.qmake.cache
## feature文件
qmake运行时还要加载大量的 .prf 文件。
feature文件目录的确定(来自源码中的注释)
   1) environment variable QMAKEFEATURES (as separated by colons)
   2) property variable QMAKEFEATURES (as separated by colons)
   3) <project_root> (where .qmake.cache lives) + FEATURES_DIR
   4) environment variable QMAKEPATH (as separated by colons) + /mkspecs/FEATURES_DIR
   5) your QMAKESPEC/features dir
   6) your data_install/mkspecs/FEATURES_DIR
   7) your QMAKESPEC/../FEATURES_DIR dir
   FEATURES_DIR is defined as:
   1) features/(unix|win32|macx)/
   2) features/
feature文件的加载：
- 加载 default_pre.prf
- 处理工程文件.pro- CONFIG+=... 后面跟的每一个东西，qmake都会尝试去加载相应的 xxx.prf 文件
- 加载 default_post.prf 文件
