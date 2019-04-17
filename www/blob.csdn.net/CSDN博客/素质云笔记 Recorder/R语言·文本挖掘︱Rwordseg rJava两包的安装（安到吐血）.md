# R语言·文本挖掘︱Rwordseg/rJava两包的安装（安到吐血） - 素质云笔记/Recorder... - CSDN博客





2016年11月25日 23:24:56[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：28560
所属专栏：[R语言与自然语言处理](https://blog.csdn.net/column/details/13670.html)












**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)

———————————————————————————



R语言·文本挖掘︱Rwordseg/rJava两包的安装





笔者：文本挖掘中这两个包可谓是“老顽固”，做文本挖掘必须要过的关卡，今天倒腾了一天![抓狂](http://static.blog.csdn.net/xheditor/xheditor_emot/default/crazy.gif)，桌面下了一堆东西，终于弄出来了![哭](http://static.blog.csdn.net/xheditor/xheditor_emot/default/cry.gif)。故此，赶紧记录下来，以后可用：

关于这两个包的安装问题，有很多很多教程，很多方法，我就不一一介绍，如果我介绍的方法你安装不了，那你就得去下面的参考链接找找其他的方式。

只有RJava配置成功了，Rwordseg安装才可能成功，前者是后者的依赖包。



某神的观点，总结出以下两点经验，供后来人参考：


1、rJava在安装后除了对path配置正确外，还有个容易忽略的细节，即R版本的选择，如果JRE是64位的，那么R也应该要64位的版

本，否则肯定会报错。如果是在Rstudio中启动R，那么要检查下启动的R的版本是哪个，方法是看Tools-Global option的默认项。


2、Rwordseg放在Rforge而非CRAN上，因此如果直接install.packages，基本都会失败告终。正确方法是下载源码，本地安装，而

且打破一个传言，即Rwordseg只能在R3.0.2版本上安装成功，实际上，如果本地安装的化，R 3.1.0也可以安装成功。

（参考于：http://www.dataguru.cn/thread-503455-1-1.html）



———————————————分割线———————————————



# 一、Rwordseg包——文本分词(建议数据量<1G)



## 1、常用下载方式——语句执行法



网上盛行这样的方式：


`install.packages("Rwordseg", repos="http://R-Forge.R-project.org")`
用这一语句的前提是已经布置好rJava包，而布置好rJava包，还得布置java环境，



所以第一种语句执行法在一开始是行不通的。


会出现以下的报错：



```
trying URL 'http://R-Forge.R-project.org/src/contrib/Rwordseg_0.2-1.tar.gz'
Content type 'application/x-gzip' length 5445754 bytes (5.2 Mb)
opened URL
downloaded 5.2 Mb

* installing *source* package 'Rwordseg' ...
** R
** demo
** inst
** preparing package for lazy loading
警告: package 'rJava' was built under R version 3.1.3
Error : .onLoad failed in loadNamespace() for 'rJava', details:
  call: inDL(x, as.logical(local), as.logical(now), ...)
  error: 无法载入共享目标对象‘D:/Program Files/R/R-3.1.2/library/rJava/libs/x64/rJava.dll’：:
  LoadLibrary failure:  %1 不是有效的 Win32 应用程序。

Error : package 'rJava' could not be loaded
ERROR: lazy loading failed for package 'Rwordseg'
* removing 'D:/Program Files/R/R-3.1.2/library/Rwordseg'
Warning in install.packages :
  running command '"D:/PROGRA~1/R/R-31~1.2/bin/x64/R" CMD INSTALL -l "D:\Program Files\R\R-3.1.2\library" C:\Users\Lance\AppData\Local\Temp\Rtmpgd2sAT/downloaded_packages/Rwordseg_0.2-1.tar.gz' had status 1
Warning in install.packages :
  installation of package ‘Rwordseg’ had non-zero exit status

The downloaded source packages are in
        ‘C:\Users\Lance\AppData\Local\Temp\Rtmpgd2sAT\downloaded_packages’
```





也会出现以下的错误：



```
错误于read.dcf(file.path(pkgname, "DESCRIPTION"), c("Package", "Type")) : 
  无法打开链结
此外: 警告信息：
1: In download.file(url, destfile, method, mode = "wb", ...) :
  下载的长度23091不等于报告的长度5470730
2: In unzip(zipname, exdir = dest) : 从zip文件中抽取1时出了错
3: In read.dcf(file.path(pkgname, "DESCRIPTION"), c("Package", "Type")) :
  无法打开压缩文件'Rwordseg/DESCRIPTION'，可能是因为'No such file or directory'
```













## 2、手动下载安装包

（参考于：http://bbs.pinggu.org/thread-2989656-1-1.html）



第一步：先从官网下载安装包：https://r-forge.r-project.org/R/?group_id=1054

![](https://img-blog.csdn.net/20160312220041111)



第二步：手动下载

![](https://img-blog.csdn.net/20160312220200359)



把下载好的zip文件，用如图的方式打开即可。



———————————————分割线———————————————



# 二、rJava包



这个包是最麻烦的，要下载很多东西，R语言虽然可以通过install.packages("rJava")下载，但是会出现以下的报错：





```
载入需要的程辑包：rJava
Error : loadNamespace()里算'rJava'时.onLoad失败了，详细内容：
  调用: fun(libname, pkgname)
  错误: JAVA_HOME cannot be determined from the Registry
In addition: Warning message:
程辑包‘rJava’是用R版本3.2.3 来建造的 
Error: 无法载入程辑包‘rJava’
```


第一步：下载java



可从官网下载：http://www.java.com/zh_CN/

直接安装。



但是我在安装的时候会出现以下的错误：


`安装 JAVA 时提示，“在使用当前internet连接情况下，安装程序无法继续。”`


于是乎可以用以下的链接去下载离线的版本，然后我的电脑就可以顺利下载了，



http://www.java.com/zh_CN/download/help/connect_proxy.xml

下载按照步骤来一路自定义，没改目录地址。



第二步：下载Java环境——JDK 1.7 安装包

我是在这个网址下载到的，可以用的：http://download.csdn.net/download/jczmdeveloper/8064891

(PS:这个的重要性在于，我换了一台新电脑，虽然把rJava拷贝过来了，但是library不起来，需要JDK的环境才能library)



PS:我在官方网址（http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html）下载的，会报错，在下载最后一步出现问题，而且不能忽略。

![](https://img-blog.csdn.net/20160312221838349)



第三步：就可以直接library了。





———————————————分割线———————————————



# 三、贴一些其他报错情况下的解决方案





## 1、注意保存的工作目录



java&jdk安装-去java官网下载安装即可

（1）java-http://www.java.com/zh_CN/

（2）jdk-http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html



注意：

（1）看电脑系统配置选择Windows x86/...跟R一致

（2）java&jdk安装到同一个文件夹下面，跟R同一个目录，比如 R的工作目录是"D:/Program Files/RStudio"；java&jdk的工作目录是“D:\Program Files\Java”&“D:\Program Files\Java\jdk1.8.0_66”

（3）尤其要注意的点：安装jdk过程中，会提示jdk安装目录及jre安装目录，记住别都安装在java根目录下，比如把jre安装在java根目录下子文件夹中即可。参考：http://jingyan.baidu.com/album/6dad5075d1dc40a123e36ea3.html?picindex=1。



jdk环境配置

http://jingyan.baidu.com/article/f96699bb8b38e0894e3c1bef.html

注意：目录是jdk安装的目录，比如“D:\Program Files\Java\jdk1.8.0_66”



参考于：http://blog.sina.com.cn/s/blog_6676d74d0102w39h.html





## 2、JRE的路径设置错误等导致jvm.dll加载失败

出现以下的问题：



```
载入需要的程辑包：rJava 
Error : loadNamespace()里算'rJava'时.onLoad失败了，详细内容： 
调用: inDL(x, as.logical(local), as.logical(now), ...) 
错误: 无法载入共享目标对象‘C:/Program Files/R/R-3.1.2/library/rJava/libs/x64/rJava.dll’：: 
LoadLibrary failure: 找不到指定的模块。

错误: 无法载入程辑包‘rJava’
```









在R语言里运行下面的命令：

Sys.setenv(JAVA_HOME='C:/Program Files/Java/jdk1.6.0_33/jre')

原来运行xlsx包需要rJava，而rJava需要JAVA环境，问题主要是JAVA环境变量引起的，如果不愿意在外面设置JAVA_HOME、PATH和CLASSPATH之类的环境变量，直接在R环境里设置就行了。不过这样每次启动时都要设置一次，可以保存在R的启动环境中。



参考于：http://www.cnblogs.com/speeding/p/4159264.html





## 3、检查JAVA是否符合R的版本





1.是否安装JAVA，如果已经安装请检查JAVA是否符合R的版本。建议从新安装下JAVA：[http://www.java.com/en/download/manual.jsp]()

2.不工作，在加载包之前，手动配置下java的位置

Sys.setenv(JAVA_HOME='C:\Program Files\Java\jre7') # for 64-bit version

Sys.setenv(JAVA_HOME='C:\Program Files (x86)\Java\jre7') # for 32-bit version library(rJava)

请注意，64位得JAVA,一定要安装在Program Files之中，不然会一直报错。



（参考于：http://www.zhihu.com/question/21584334）





**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)

———————————————————————————



