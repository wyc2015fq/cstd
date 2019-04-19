# Qt安装—搭建VS2008+QT开发环境 - 三少GG - CSDN博客
2012年12月13日 23:10:48[三少GG](https://me.csdn.net/scut1135)阅读数：792
个人分类：[流媒体基础](https://blog.csdn.net/scut1135/article/category/934830)
OpenSource版本下载地址：
[http://qt-project.org/downloads](http://qt-project.org/downloads)
（一）工欲善其事，必先利其器，废话不多讲。
**总结起来网上流行的VS2008+QT安装说明有以下几个问题需要解释清楚：**
     1，首先明确需要下载什么版本的QT。网上流行的安装说明只是针对过去旧版本的QT，还有要求下载VS2008 SP1补丁的等等。很乱，对于刚接触Qt开发者会造成不必要的困扰，安装个开发环境真的需要那么繁琐和劳神吗。 
     2，对于环境变量的配置，诸如path，QTDIR 和WindowsSdkDir等等。实际上都是旧版本的Qt，新版本的Qt已经不需要设置了。
     3，真的必须要configure 和nmake QT吗？对于漫长的编译过程和七七八八的编译错误？我们完全没有不必要折腾了，这里我们下载已经编译好的QT版本就够了，估计QT版本4.6以下需要这样做。     
（二）对于以上的问题，我是按照网上说法大费周折无果，实际新版本的Qt安装起来非常简单。
      第一步：安装VS2008,我这里安装的是中文版的VS2008并且也没有安装VS2008 SP1，对于VS2008的安装过程这里省略了。
      第二步：安装Qt,访问Qt官方网站[http://qt.nokia.com/downloads-cn](http://qt.nokia.com/downloads-cn)下载2个软件：
      1，下载VS2008编译好的QT4.7版本：[http://get.qt.nokia.com/qt/source/qt-win-opensource-4.7.0-vs2008.exe](http://get.qt.nokia.com/qt/source/qt-win-opensource-4.7.0-vs2008.exe)
      提示：minGW版本不用下，他是minGW工具编译下的QT库。QTSDK-qt-sdk-win-opensource-2010.05.exe 也不用下载，它是用QT独立进行开发下的完整软件安装包，安装过程与一般软件相同，点击下一步就行了。
      2，下载VS2008的Qt插件：[http://get.qt.nokia.com/vsaddin/qt-vs-addin-1.1.7.exe](http://get.qt.nokia.com/vsaddin/qt-vs-addin-1.1.7.exe)
      提示：这是个针对VS（VS2005、VS2008、VS2010）各个版本都适用的QT插件，插件安装过程也是一样，点击下一步就行了。
      3，对于VS和QT的安装顺序，没有严格的要求，并没有谁先谁后，这里我是之前已经安装好了VS2008中文版的。
     （三）安装好VS2008和QT相关软件之后，测试安装是否成功？
      第一步：启动VS2008,会发现VS2008的菜单栏出现Qt这个菜单
![](http://pic002.cnblogs.com/images/2011/33363/2011021921154824.png)
      第二步：单击VS2008的Qt菜单
![](http://pic002.cnblogs.com/images/2011/33363/2011021921221666.png)
      第三步：打开Qt Option可以看到Qt的Version已经被自动识别好（并非像老版本需要手动去设置） 
![](http://pic002.cnblogs.com/images/2011/33363/2011021921311717.png)
![](http://pic002.cnblogs.com/images/2011/33363/2011021922225448.png)
     第四步：打开VS2008,新建一个Qt项目
![](http://pic002.cnblogs.com/images/2011/33363/2011021921360779.png)
![](http://pic002.cnblogs.com/images/2011/33363/2011021921400666.png)
![](http://pic002.cnblogs.com/images/2011/33363/2011021921425434.png)![](http://pic002.cnblogs.com/images/2011/33363/2011021921490355.png)
![](http://pic002.cnblogs.com/images/2011/33363/2011021921542297.png)
第五步：编译并运行MyApp项目,这里先不用![](http://pic002.cnblogs.com/images/2011/33363/2011021922023724.png)（Qt的UI设计器，下面章节再进行介绍）设计，默认编译运行就行
生成的项目目录结构图：
![](http://pic002.cnblogs.com/images/2011/33363/2011021921592712.png)
运行成功截图如下：
![](http://pic002.cnblogs.com/images/2011/33363/2011021922095880.png)
总结：虽然在软件开发中，建议老版本的Qt稳定些，还有最好用英文版的VS2008，主要考虑稳定性，不过这里考虑到编译和设置旧版本的Qt实在繁琐耗时，并且弄了半天还是编译不通过，只好用新版本的Qt，这里还算顺利，没有出现什么错误，希望给新手一点帮助。
