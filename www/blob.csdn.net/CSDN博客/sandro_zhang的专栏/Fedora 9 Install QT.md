# Fedora 9 Install QT - sandro_zhang的专栏 - CSDN博客
2013年06月19日 15:34:41[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：1059
                
1,Download qt-everywhere-opensource-src-4.7.1.tar.bz2 to ~./Downloads 
2,Unzip it to /usr/local
$ tar -jxvf qt-everywhere-opensource-src-4.7.1.tar.gz -C /usr/local
3,Add QT PATH to your .bash_profile
$ vi .bash_profile
Place below text after "export PATH"
#Add
QTDIR=/usr/local/qt-everywhere-opensource-src-4.7.1
PATH=$QTDIR/bin:$PATH
MANPATH=$QTDIR/doc/man:$MANPATH
LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH
QMAKESPEC=/usr/local/qt-x11-opensource-src-4.7.1/mkspecs/linux-g++
export QTDIR PATH MANPATH LD_LIBRARY_PATH QMAKESPEC
#End
4,Refresh environment
$ source .bash_profile
5,Start Build QT
$ cd $QTDIR
$ ./configure
Which edition of Qt do you want to use ?
Type 'c' if you want to use the Commercial Edition.
Type 'o' if you want to use the Open Source Edition.
o
Qt is now configured for building. Just run 'gmake'.
Once everything is built, you must run 'gmake install'.
Qt will be installed into /usr/local/Trolltech/Qt-4.7.1
To reconfigure, run 'gmake confclean' and 'configure'.
6,So we run gmake, gmake install. to finish the installation.
And this will take a long time.
7,You can "make clean" to remove all .o files.
Error Case:
1,Basic XLib functionality test failed!
Goto $QTDIR/config.tests/x11/xlib
Run make
This will show detail error, if report is like 
"error:x11/xlib.h: No such file or directory"
You should install libX11-dev, so,just run "yum install libX*" can fix this error.
2,The specified system/compiler is not supported
reset QMAKESPEC
$ export QMAKESPEC=
            
