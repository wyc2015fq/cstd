# QT项目升级(QT4到QT) - youfangyuan - CSDN博客
2014年11月04日 21:31:14[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：1554
## QT4 升级到 QT5 改动:
## PC部分：
【改 QTDIR 变量】
在工程根目录下找到 .user 文件 ,  如 InnoTabPlugin.vcxproj.user
修改指向你的 QT5 根目录：
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Template|Win32'">
    <QTDIR>C:\qt\Qt5.2.1\5.2.1\msvc2010</QTDIR>
  </PropertyGroup>   
或者找到这个文件 Qt4VSPropertySheet.props ，在里面修改：
<PropertyGroup Label="UserMacros">
    <QTDIR>c:\Qt\Qt5.2.1\5.2.1\msvc2010</QTDIR>
  </PropertyGroup>
【新增 QtWidgets 模块】
相对于 QT4, QT5 的一个主要的基础架构修改是，将 widget 从 QtGui 模块剥离开来，
新增 QtWidgets 模块，头文件包含“ QtWidgets ”。
#include <QtGui/QDialog>   
#include <QtGui/QTableView>    ......
修改为：
#include<QtWidgets/qdialog.h>
#include <QtWidgets/QTableView> ......
#include <QWidget>  修改为   #include <QtWidgets/QWidget>   ......
#include <QListWidgetItem> 修改为  #include <QtWidgets/qlistwidget.h>
.pro 文件中 Qt += widgets
【 QApplication 放入 QtWidgets 】
#include <QApplication> 改成： 
#include <QtWidgets/QApplication>
【新增 QtWetKitWidgets 模块】
头文件包含要加 QtWebKitWidgets 目录 .
#include <QtWebKit/QWebFrame>  替换为： #include<QtWebKitWidgets/ QWebFrame >
#include <QtWebkit/QWebView>  替换为：#include<QtWebKitWidgets/ QWebView >
.pro 文件中 Qt += webkitwidgets
【 QSortFilterProxyModel 】
#include <QtGui/QSortFilterProxyModel>
修改为：  #include <QtCore/QSortFilterProxyModel>
【 QDesktopServices::storageLocation 系统目录问题】
前使用 QDesktopServices::storageLocation(QDesktopServices::xxxx) 来获取一些系统目录，
现在则要改成
QStandardPaths::writableLocation(QStandardPaths::xxxx)
如 :
QString directory = QDesktopServices::storageLocation(QDesktopServices::DataLocation); 
可修改为：
QString directory = QStandardPaths::writableLocation(QStandardPaths::DataLocation); 
而
QString location = QDesktopServices::storageLocation(QDesktopServices::CacheLocation);
修改为：
QString location = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
【 QHttp 用 QNetworkAccessManager 代替】
QHttp 类已经被废弃，请用以下的类替换
QNetworkAccessManager
QNetworkReply
QNetworkRequest   ( 代替 QHttpRequestHeader ）
m_pReply = m_httpManager.get(QNetworkRequest(url));
connect(m_pReply, SIGNAL(readyRead()), this,SLOT(readyRead()));
connect(m_pReply, SIGNAL(finished()),this, SLOT(finishDownload ()));
connect(m_pReply, SIGNAL(error(QNetworkReply::NetworkError)), this,SLOT(networkError(QNetworkReply::NetworkError)));
m_pFile = new QFile(localPath); 
void MyClass::readyRead()
{
if(m_pFile)
   m_pFile->write(m_pReply->readAll());
}
void MyClass::finishDownload()
{
if(m_pFile->handle() != NULL)
{
m_pFile->flush();
m_pFile->close();
} 
}
【 qInstallMsgHandler 函数废弃】
可修改为： qInstallMessageHandler ，
MessageOutput 要多加一个参数 : QMessageLogContext& Context
void  MessageOutput( QtMsgType type, const QMessageLogContext& Context, const QString &sMsg )   
QtMsgHandler  改为  QtMessageHandler
【 #include <QX11Info> 问题】
加路径： C:\Qt\Qt5.2.1\5.2.1\Src\qtx11extras\include
修改为： #include <QtX11Extras\qx11info_x11.h>
【 RegisterDeviceNotification 问题】
RegisterDeviceNotification(this->winId(), &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE); 改成
RegisterDeviceNotification((HANDLE)this->winId(), &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
【 setYMD 不是成员函数的问题 】
把 setYMD 改为： setDate
QDate.setYMD()  修改成  QDate.setDate()
【 Q_WS_WIN 问题】
宏修改 , Q_WS_WIN  可替换为： Q_OS_WIN
【 network 模块引用问题】
#ifdef WIN32
#include "../../src/network/access/qnetworkreply.h"
#include "../../src/network/access/QNetworkAccessManager.h"
#include "../../src/network/kernel/QNetworkInterface.h"
#else 
#include "src/network/access/s.h"  
#include "src/network/access/QNetworkAccessManager.h"  
#include "src/network/kernel/QNetworkInterface.h"  
#endif
修改为：
#include <QtNetwork/qnetworkreply.h>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkinterface.h>
【 QAxWidget 放入 ActiveQt 】
#include <QAxWidget>  
修改为：
#include <ActiveQt\QAxWidget>
【 QTextCodec 编码函数已废弃】
Qt5 发布之时，默认将会是 utf8 编码 , 这些函数请去掉：
QTextCodec::setCodecForTr(...)
QTextCodec::setCodecForCStrings(...)
QTextCodec::setCodecForLocale(...)
【 QWindowsStyle 问题】
窗体风格类 QWindowsStyle 移除了，
使用 QCommonStyle 代替。
【错误 "std::numeric_limits<qint64>::min().... 解决方法】
方法： #define NOMINMAX before including windows.h header.  
代码：
#define NOMINMAX
#include <windows.h>
如果不行 ,  将 #include<windows.h> 改为 #include<qt_windows.h> 试试 .
如果还不行，把static inline qint64 nullJd() { return std::numeric_limits<qint64>::min(); }
加括号，改成：
static inline qint64 nullJd() { return (std::numeric_limits<qint64>::min)(); }
【 QCoreApplication::UnicodeUTF8 问题】
QCoreApplication::UnicodeUTF8 被废弃了 , 用到的地方，直接去掉就可以了，如
label->setText(QApplication::translate("hello", "Text:", 0, QApplication::UnicodeUTF8));
改成 :
label->setText(QApplication::translate("hello", "Text:", 0));
【 QWorkspace 废弃】
QWorkspace 遗弃了
#include <QWorkspace>  改成：
#include <QMdiArea>
【 Ascii 编码问题】
toAscii() and fromAscii()  用下面的来代替 :
toLatin1()
fromLatin1() 
【 QT_WA 的错误解决】
如果提示  QT_WA 的错误，自定义一个宏：
#if QT_VERSION >= 0x050000 
  #define QT_WA(unicode, ansi) unicode 
#endif
【 winEvent 问题】
添加     QByteArray eventType;
修改     return QMyPopupDlg::nativeEvent(eventType, message, result);
【 winId() 类型不匹配问题】
SetWindowPos(this->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE); （ 299 ）
修改为：
SetWindowPos((HWND)this->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE) ；  
或者 ...   ( HANDLE)this->winId() .....
【提示找不到 GLES2/gl2.h 】 
解决方法
#  include <GLES2/gl2.h>
换成
#include <QtANGLE/GLES2/gl2.h>
或者 #include <QtANGLE/KHR/GLES2/gl2.h>
【新增 QUrlQuery 类】
QUrl u(url); 
修改为： QUrlQuery u(url);
m_request.setUrl(u); 
修改为：  m_request.setUrl(QUrl(url) ) ；
【 khrplatform.h 找不到问题】
#include <KHR/khrplatform.h>
换成：
#include <QtANGLE/KHR/khrplatform.h>
【提示不是 QObject* 类型的问题】
QState assignProperty 函数如果不是 QObject* 类型，要强制转换：
state->assignProperty(wList[0], "geometry", positions[0]);
也是一样的道理，如：
connect() 和 QPropertyAnimation 构造函数也是同样的道理 .
【 QString:  toWCharArray 链接错误问题 】
TCHAR volstr[50] = {0};
volume.toWCharArray(volstr);
改成 :
TCHAR *volstr = (wchar_t*)volume.utf16();
【 already defined in LIBCMTD.lib(typinfo.obj) 问题】
先在 Configuration Properties->Liner->Input->Ignore Specific Default Libraries 中输入： LIBCMTD.lib;msvcrtd.lib
再在 Configuration Properties->Liner->Input->Additional Dependecies 中添加 ：msvcrtd.lib;LIBCMTD.lib
如果不行，撤销上面的改动，单单在 Additional Dependecies 中添加上 msvcrt.lib 或msvcrt d .lib 。
【 unresolved external symbol "const type_info::`vftable'"  】
用楼上的解决方法。不行，试试这个：
[http://support.microsoft.com/default.aspx?scid=kb;%5BLN%5D;830482](http://support.microsoft.com/default.aspx?scid=kb;%5BLN%5D;830482)
【 QString::fromStdWString （）引起的 link 错误】
解决方法：
![](http://img0.tuicool.com/Vze6jq.jpg)
【 Qt::WFlags 问题 】
Qt::WFlags 改成  Qt::WindowFlags
【 QSound::isAvailable() 问题】
return QSound::isAvailable(); 可替换成
return !QAudioDeviceInfo::availableDevices(QAudio::AudioOutput).isEmpty();
【 reset() 问题】
QAbstractItemModel::reset() 在 QT5 中被移除
使用下面两句代替
QAbstractItemModel::beginResetModel();
QAbstractItemModel::endResetModel();
【链接失败】
在 Configuration Properties->Liner->Input->Ignore Specific Default Librarie 里添加相应的库。
vlib.lib;qtmaind.lib;Qt5XML.lib;Qt5XmlPatterns.lib;Qt5Test.lib;Qt5WebKit.lib;Qt5Network.lib;Qt5Widgets.lib;Qt5WebKitWidgets.lib;Qt5Guid.lib;Qt5Cored.lib;gdi32.lib;comdlg32.lib;oleaut32.lib;imm32.lib;winmm.lib;netapi32.lib
============================华丽的分割线===========================
## MAC部分：
【 OS X 和 Xcode 版本选择】
请用 OS X10.7 及以上的 mac 来 build 工程。
xcode 则请用 4.0 以上版本。
【更新修改 QT pro 工程文件】
在 windows 上生成新的 .pro 文件
    c:\qt\Qt5.2.1\5.2.1\msvc2010\bin\qmake.exe -project
修改 pro 文件
    QT += core gui network webkit xml xmlpatterns widgets webkitwidgets 
在拷贝工程到 Mac 上，把 QT pro 工程文件转成 xcode 工程：
      qmake -spec macx-xcode  yourproject.pro 
【调用 webservice soap 问题】
在工程中添加 soap 库 ,
soap 库要用新的 ,  用 qt5 重新编译网上下载的 soap 源码。
【提示 soap 库文件找不到】
提示 soap 库文件找不到，请修改链接路径，或改 soap 文件名 : xxxx.1.dylib 。
【 xcode 设置 release 版本】
如果要编译 release 版本，
在 xocde 顶部选择  "Edit Scheme"--> 选择 "Release"
【 error: expected unqualified-id before 'OTHER' token 】
代码文件头多了个字符，在 windows 上用 notepad++ 等文本编辑工具打开，
另存为新文件，再重新拷贝到 MacOS 上编译即可。
【 duplicate symbols 链接不了的问题】
多个资源源文件 (qrc_xxxxx.cpp) 被加到工程，链接时会有下面类似的提示：
ld:  2  duplicate symbols for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
解决方法，把重复的资源代码文件 qrc_xxxxx.cpp 从工程中移除再 build 。
【 library  architecture 不对链接错误的问题】
在 MAC 上的编译时引用到库需要支持 x86_64 架构，  QT5 的 QT 库都是支持x86_64 架构的，用 QT5  的 qmake  生成的工程必需要 xcode4 才能打开， xcode4 的工程生来就必需先择支持 x86_64 架构，
这就要求我们用到的库都必需支持 x86_64 架构，查看库是不是支持 x86_64 架构可以用“ otool -h filepath ”
有红框里的就是支持 x86_64 架构
![](http://img1.tuicool.com/maq2If6.jpg)
只有绿框的只支持i386和ppc.
![](http://img2.tuicool.com/YzARrea.jpg)
如果我们手头有库的源码，把源代码在 xcode4 上重新生成。
编译选项“ Architectures ”设置成x86_64
![](http://img0.tuicool.com/IfYze26.jpg)
