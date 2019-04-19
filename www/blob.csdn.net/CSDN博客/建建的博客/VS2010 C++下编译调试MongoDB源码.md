# VS2010 C++下编译调试MongoDB源码 - 建建的博客 - CSDN博客
2017年03月07日 14:10:12[纪建](https://me.csdn.net/u013898698)阅读数：160
 考虑到mongodb使用了boost库源码，参考mongodb官方文档后，下载编译boost版本是1.42(时间为2010-2-2)或更新版本:
     boost版本1.42：
     http://sourceforge.net/projects/boost/files/boost/1.42.0/boost_1_42_0.zip/download     
     下载boost源码之后，我把boost解压在D盘。
1.在D:\boost_1_42_0\下找到该批处理文件bootstrap.bat，以vs2010命令行方式运行它，并最终生成bjam.exe可执行文件（详细结果及错误信息见生成的bjam.log日志）。    
2.将生成的bjam.exe文件拷贝到“D:\boost_1_42_0”目录下,运行下面命令行：
```
bjam stage variant=debug  --stagedir="d:\boost_1_42_0" --with-filesystem --with-thread --with-date_time --with-program_options --layout=versioned threading=multi toolset=msvc-10.0
```
     如下图：
![](http://daizhj.cnblogs.com/images/cnblogs_com/daizhj/mongodebug1.png)
    注：i.如果要生成release链接库文件，需将上面的variant设置成release，形如：
bjam stage variant=release .....     
       ii.如果要全部生成使用下面编译选项
bjam ......  --build-type=complete 
     编译过程可能会用一些时间，最终会在"D:\boost_1_42_0" 生成一个lib目录，里面包括刚生成的库文件。
 3.下载已用vs2010编译的SpiderMonkey库文件及源码包，并将其解压到D盘下，下载链接：
     https://github.com/dwight/vc2010_js
     将该文件夹名称改为“js”（因为mongodb项目会默认指定该路径寻找可用的lib及c源文件），如下图：
![](http://daizhj.cnblogs.com/images/cnblogs_com/daizhj/mongodebug7.png)
    说明：SpiderMonkey是由C语言操作的JavaScript引擎，它支持JS1.4和ECMAScript-262规范。该引擎分析、编译和执行脚本，根据JS数据类型和对象的需要进行内存分配及释放操作。目前它是Mozilla项目的一部分。可通过下面链接了解更多信息：
    https://developer.mozilla.org/en/SpiderMonkey
    源码下载链接：
    https://developer.mozilla.org/En/SpiderMonkey/Getting_SpiderMonkey_source_code
    另外如何使用spidermonkey参见：
    http://www.ibm.com/developerworks/cn/linux/shell/js/js_engine/
4.接下来下载并解压mongod源码包（本人下载的是其最新版本：r1.8.0-rc1），这里我将其也解压在了D盘：
    mongodb-src-r1.8.0-rc1：
    https://github.com/mongodb/mongo/zipball/r1.8.0-rc1 
    之后利用vs2010打开 D:\mongodb-mongo-4d2aeb6\db目录下的解决方案文件“db_10.sln”, 加载之后的视图如下：
![](http://daizhj.cnblogs.com/images/cnblogs_com/daizhj/mongodebug4.png)
    在其中的mongod项目上，点击鼠标右键，打开"属性"对话框, 在“配置属性”-“c++目录”-"包含目录"中点“编辑”：
![](http://daizhj.cnblogs.com/images/cnblogs_com/daizhj/mongodebug2.png)
    添加对上面的boost源码文件路径的包含：
![](http://daizhj.cnblogs.com/images/cnblogs_com/daizhj/mongodebug3.png)
     设置库文件目录选项中添加对刚编译的boost库的路径"D:\boost_1_42_0\lib"声明，如下图：    
![](http://daizhj.cnblogs.com/images/cnblogs_com/daizhj/mongodebug5.png)
![](http://daizhj.cnblogs.com/images/cnblogs_com/daizhj/mongodebug6.png)
     在C/C++常规--“附加包含目录”，中添加"D:\boost_1_42_0"声明，如下图：
![](http://daizhj.cnblogs.com/images/cnblogs_com/daizhj/mongodebug9.png)
5.因为我的机器是32位系统，而下载的mongodb项目设置为64位，所以要修改一下编译方式，如下图：
![](http://daizhj.cnblogs.com/images/cnblogs_com/daizhj/mongodebug8.png)
     一切就绪后，开始编译，时间比较长，耐心等待...     
     注：如出现“pch.h文件无法打开”的错误信息时，只要在VS2010中的开该文件即可。
6.如果要调试的话，打开“项目属性”--“配置属性”--“调试”--“命令参数”，输入如下参数  ：
```
"--dbpath d:\mongodb\db" "--port 27017"
```
![](http://daizhj.cnblogs.com/images/cnblogs_com/daizhj/mongodebug10.png)
     然后找出db.cpp文件，将断点放在如下函数的第一行代码处： 
```
int main(int argc, char* argv[])
```
     如下图：
![](http://daizhj.cnblogs.com/images/cnblogs_com/daizhj/mongodebug11.png)
     需要说明的是，我一开始使用的是VS2008,但mongodb源码在最新版本中好像就不再提供vs2008的项目文件了，另外我使用vs2008也一直没有成功编译过mongodb，不是缺文件就是链接库编译时有问题（google一下，发现国外同行也被类似问题困扰）。所以大家也就不要再VS2008编译上浪费时间了。
     好了，到这里主要的内容介绍完了，大家可以通过调试来分析观察MONGODB代码的执行流程了。
    参考链接：
    http://www.cnblogs.com/wondering/archive/2009/05/21/boost_setup.html
    http://www.cnblogs.com/xdotnet/archive/2008/03/22/boost_install_config.html
    http://www.mongodb.org/display/DOCS/Building+with+Visual+Studio+2010      
    Tags: mongodb,c++,source code
Tags: mongodb,c++,source code
