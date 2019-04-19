# Qt 之 QuaZIP（zip 压缩/解压缩） - xqhrs232的专栏 - CSDN博客
2018年09月06日 15:05:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：145
原文地址::[https://blog.csdn.net/liang19890820/article/details/50636181](https://blog.csdn.net/liang19890820/article/details/50636181)
相关文章
1、[Qt的gzip模块实现](https://www.cnblogs.com/danju/p/3691636.html)----[https://www.cnblogs.com/danju/p/3691636.html](https://www.cnblogs.com/danju/p/3691636.html)
2、Qt利用zlib解压HTTP采用GZIP的块----[http://blog.sina.com.cn/s/blog_de7eec9c0101bo5u.html](http://blog.sina.com.cn/s/blog_de7eec9c0101bo5u.html)
# **简述**
QuaZIP是使用Qt/C++对ZLIB进行简单封装的用于压缩及解压缩ZIP的开源库。适用于多种平台，利用它可以很方便的将单个或多个文件打包为zip文件，且打包后的zip文件可以通过其它工具打开。
Qt中提供了qCompress/qUncompress来进行文件的压缩与解压，但存在两个问题：
- 无法很好地压缩/解压缩文件夹。
- 只能将一个文件压缩为某种格式，压缩后的文件无法通过其它解压工具打开（如：7zip，或WinRAR），只能通过qUncompress解压读取，也就是说qCompress生成的压缩文件不包含普通压缩文件的一些头信息，导致无法解压。
**| **版权声明：一去、二三里，未经博主允许不得转载。
# **QuaZIP类描述**
|类|描述|
|----|----|
|JlCompress|典型操作工具类|
|QuaAdler32|Adler32算法校验和|
|QuaChecksum32|校验和接口|
|QuaCrc32|CRC32校验和|
|QuaGzipFile|GZIP文件操作|
|QuaZIODevice|压缩/解压QIODevice|
|QuaZip|ZIP文件|
|QuaZipDir|ZIP文件内目录导航|
|QuaZipFile|ZIP文件内的文件|
|QuaZipFileInfo|ZIP压缩包内的文件信息|
|QuaZipFilePrivate|QuaZip的接口|
|QuaZipNewInfo|被创建的文件信息|
|QuaZipPrivate|QuaZIP内部类|
# **使用**
## **接口说明**
QuaZip中的JlCompress类用来压缩和解压缩文件
```
<span style="color:#000000"><code class="language-Qt"><span style="color:#880000">/**
    压缩文件夹
    参数一fileCompressed：压缩后的文件
    参数二dir：待压缩的目录
    参数三recursive：是否递归
**/</span>
<span style="color:#000088">static</span> bool compressDir(QString fileCompressed, QString dir = QString(), bool recursive = <span style="color:#000088">true</span>);
<span style="color:#880000">/**
    解压缩文件夹 
    参数一fileCompressed：待解压缩的文件
    参数二fileCompressed：解压缩存放的目录
**/</span>
<span style="color:#000088">static</span> QStringList extractDir(QString fileCompressed, QString dir = QString());</code></span>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
## **配置**
在工程中将quazip.lib所在库目录、头文件所在的目录添加到工程中。
pro中添加：
```
<span style="color:#000000"><code class="language-Qt"><span style="color:#009900">LIBS</span> += -<span style="color:#009900">L</span><span style="color:#009900">"D:/quazip/lib"</span> -lquazip
<span style="color:#009900">INCLUDEPATH</span> += <span style="color:#009900">D</span><span style="color:#009900">:/quazip/src</span>
<span style="color:#009900">DEPENDPATH</span> += <span style="color:#009900">D</span><span style="color:#009900">:/quazip/src</span></code></span>
```
- 1
- 2
- 3
## **压缩/解压缩**
```
<span style="color:#000000"><code class="language-Qt"><span style="color:#009900">#include <QApplication></span>
<span style="color:#009900">#include "JlCompress.h"</span>
<span style="color:#000088">int</span> main(<span style="color:#000088">int</span> argc, <span style="color:#000088">char</span> **argv)
{
    QApplication app(argc,argv);
    <span style="color:#880000">// 压缩文件夹</span>
    JlCompress::compressDir(<span style="color:#009900">"F:\\QuaZip.zip"</span>, <span style="color:#009900">"F:\\QuaZip"</span>);
    <span style="color:#880000">// 解压缩文件夹</span>
    JlCompress::extractDir(<span style="color:#009900">"F:\\QuaZip.zip"</span>, <span style="color:#009900">"F:\\ExtractQuaZip"</span>);
    <span style="color:#000088">return</span> app.exec();
}</code></span>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
# **更多参考**
可以直接下载我编译好的库，里面包含源码及库文件。
- [官方主页](http://quazip.sourceforge.net/)
- [souceforge下载](http://sourceforge.net/projects/quazip/)
- [QuaZIP已编译（含源码及库文件）](http://download.csdn.net/detail/u011012932/9427715)
