# QtXlsx的使用 - baidu_33570760的博客 - CSDN博客
2017年07月07日 17:25:33[carman_风](https://me.csdn.net/baidu_33570760)阅读数：1008
原文链接：http://blog.csdn.net/c3060911030/article/details/51560239
今天有幸了解到QtXlsx：http://qtxlsx.debao.me/
一个强大的对Excel的操作，不仅跨平台，而且不需要安装Excel，读写速度杠杠的。
但是用它的话，还得稍微配置下，查询博客，都是有点缺斤少两的。感觉写的不好。
下来我来说下怎么配置。
首先，下载QtXlsx类，地址：https://github.com/dbzhang800/QtXlsxWriter
这个网页下面也有说明怎么使用。喜欢看原版的请看官方说明![大笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/laugh.gif)
第一个方式：添加到模块
1.首先，下载Perl5地址：https://www.perl.org/get.html
![](https://img-blog.csdn.net/20160602003523539)
我当时下载的版本！
根据你Qt版本下载对应版本，现在大部分都用64位的了。(要匹配)
ActivePerl-5.22.1.2201-MSWin32-x64-299574.msi
下载好了之后就一直默认安装即可。
安装好了在打开你的Qt(笔者使用的是Qt5.6.0 msvc版本)和下载的QtXlsx代码。
2.开始编译代码，如果这里你的代码编译报错，不要着急，可能是代码本身的bug。请去[Git](http://lib.csdn.net/base/git)上面查看是否有认问过了，或者直接留言。
笔者使用的时候有个bug，在Qt5.6.0上面报错：关于 "QList" 和 "QVector" 转换问题。
如果有错请自行查看解决方案或者反馈bug  (https://github.com/dbzhang800/QtXlsxWriter/issues)
3.完成第二部，在你编译的目录下回生成lib文件和需要包含的文件(include)还有一个模块pri文件
![](https://img-blog.csdn.net/20160602004325799)
如果你不了解window下模块添加的方法，请按照下面步骤来。
3.1 ： 拷贝上图 include文件下的文件到你的Qt安装目录 C:\Qt\Qt5.6.0\5.6\msvc2015_64\include (直接copy，无需修改任何内容)
![](https://img-blog.csdn.net/20160602004545192)
3.2 ： 拷贝上图lib文件夹下 Qt5Xlsxd.lib   Qt5Xlsx.lib   Qt5Xlsx.prl   Qt5Xlsxd.prl  4个文件到你的Qt的lib下 C:\Qt\Qt5.6.0\5.6\msvc2015_64\lib
![](https://img-blog.csdn.net/20160602004919556)
3.3 ： 拷贝lib文件夹下的  Qt5Xlsx.dll  Qt5Xlsxd.dll 库文件到你的Qt库路径下  C:\Qt\Qt5.6.0\5.6\msvc2015_64\bin
![](https://img-blog.csdn.net/20160602005137169)
3.4 ： 拷贝 mkspecs\modules 文件夹下的qt_lib_xlsx.pri到你的Qt模块下 C:\Qt\Qt5.6.0\5.6\msvc2015_64\mkspecs\modules
![](https://img-blog.csdn.net/20160602005349379)
此处如果不添加的话，在你的Pro文件内写  QT += xlsx 会不识别此模块
4：好了，现在可以跑例子了。
pro文件里面加上 QT += xlsx
**[cpp]**[view plain](http://blog.csdn.net/c3060911030/article/details/51560239#)[copy](http://blog.csdn.net/c3060911030/article/details/51560239#)
- #include <QtXlsx/QtXlsx>
- int main()  
-     {  
-         QXlsx::Document xlsx;  
-         xlsx.write("A1", "Hello Qt!");  
-         xlsx.saveAs("Test.xlsx");  
- return 0;  
-     }  
写好代码之后，先qmake下。
![](https://img-blog.csdn.net/20160602005843787)
然后编译代码，跑代码就OK了。
第二种方法：直接使用代码。
1.当然还是必须要有perl5环境.
2.建立工程，把代码放到工程同级目录
**[plain]**[view plain](http://blog.csdn.net/c3060911030/article/details/51560239#)[copy](http://blog.csdn.net/c3060911030/article/details/51560239#)
- |-- project.pro  
- |-- ....  
- |-- 3rdparty\  
- |     |-- qtxlsx\  
- |     |  
![](https://img-blog.csdn.net/20160602010249445)
3. 在项目pro文件里添加
**[cpp]**[view plain](http://blog.csdn.net/c3060911030/article/details/51560239#)[copy](http://blog.csdn.net/c3060911030/article/details/51560239#)
- include(3rdparty/qtxlsx/src/xlsx/qtxlsx.pri)  
添加之后会重新加载下如下图
![](https://img-blog.csdn.net/20160602010459805)
4. 在main.cpp添加代码 
**[cpp]**[view plain](http://blog.csdn.net/c3060911030/article/details/51560239#)[copy](http://blog.csdn.net/c3060911030/article/details/51560239#)
- #include "xlsxdocument.h"
- int main()  
- {  
-     QXlsx::Document xlsx;  
-     xlsx.write("A1", "Hello Qt!");  
-     xlsx.saveAs("Test.xlsx");  
- return 0;  
- }  
然后同上面一个qmake
编译代码就会生成Excel表格了。
同时注意：不用qmake 的话，使用下面宏。
![](https://img-blog.csdn.net/20160602010720493)
