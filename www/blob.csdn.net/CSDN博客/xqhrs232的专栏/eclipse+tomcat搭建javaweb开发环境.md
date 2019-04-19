# eclipse+tomcat搭建javaweb开发环境 - xqhrs232的专栏 - CSDN博客
2018年09月30日 10:20:56[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：50
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/zhengyikuangge/article/details/79520488](https://blog.csdn.net/zhengyikuangge/article/details/79520488)
相关文章
1、java web开发环境搭建（jdk+Tomcat+Eclipse）（Eclipse食用方法）----[https://blog.csdn.net/yogima/article/details/79462772](https://blog.csdn.net/yogima/article/details/79462772)
2、java web开发环境搭建（Eclipse，Tomcat， mysql和Navicat）----[https://blog.csdn.net/bbyyz01/article/details/78142126](https://blog.csdn.net/bbyyz01/article/details/78142126)
我用的jdk是1.7，eclipse是luna版本，tomcat8.0。都是64位
### 工具下载
以下都是从官网下载，有某些网页可能打开速度较慢，请耐心等待
jdk1.7下载地址：[http://www.oracle.com/technetwork/java/javase/downloads/java-archive-downloads-javase7-521261.html](http://www.oracle.com/technetwork/java/javase/downloads/java-archive-downloads-javase7-521261.html)
![这里写图片描述](https://img-blog.csdn.net/20180311211822524?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhlbmd5aWt1YW5nZ2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
eclipse-luna下载地址：[http://www.eclipse.org/downloads/packages/release/luna/r](http://www.eclipse.org/downloads/packages/release/luna/r)
![这里写图片描述](https://img-blog.csdn.net/20180311211642245?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhlbmd5aWt1YW5nZ2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
tomcat 8.0下载地址：[https://tomcat.apache.org/download-80.cgi](https://tomcat.apache.org/download-80.cgi)
![这里写图片描述](https://img-blog.csdn.net/20180311211607305?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhlbmd5aWt1YW5nZ2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
吐槽下：看英文网站，英语文凭光四级水平还不够呀，下个老版本找了半天才找到
### eclipse配置tomcat
jdk安装及配置环境变量，网上很多，不再赘述
eclipse和tomcat下载后是一个压缩包，直接压缩就可以使用
如果eclipse无法打开，一般就是jdk安装出了问题或版本问题，现在这些是没有问题的
操作步骤如下：
工具栏 Window——Preferences——打开Server——选中Runtime Environment——点击 Add——选中Apache Tomcat 8.0——点击Next——点击Browse选中tomcat解压的目录——点击Finish——点击下方Servers窗口——点击蓝色字链接（或者右键——New——Server）——选择Tomcat v8.0 Server ——点击Finish
图示如下：
![这里写图片描述](https://img-blog.csdn.net/20180311212824568?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhlbmd5aWt1YW5nZ2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
--------------------- 本文来自 正义狂哥 的CSDN 博客 ，全文地址请点击：https://blog.csdn.net/zhengyikuangge/article/details/79520488?utm_source=copy
