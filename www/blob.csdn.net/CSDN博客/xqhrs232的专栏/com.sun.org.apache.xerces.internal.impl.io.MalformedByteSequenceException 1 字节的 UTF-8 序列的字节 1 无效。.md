# com.sun.org.apache.xerces.internal.impl.io.MalformedByteSequenceException: 1 字节的 UTF-8 序列的字节 1 无效。 - xqhrs232的专栏 - CSDN博客
2018年09月30日 16:14:30[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：310
原文地址::[https://blog.csdn.net/xinghuo0007/article/details/53066791](https://blog.csdn.net/xinghuo0007/article/details/53066791)
今天在配置tomcat的server.xml 文件完后，启动tomcat后报错：
`    com.sun.org.apache.xerces.internal.impl.io.MalformedByteSequenceException: 1 字节的 UTF-8 序列的字节 1 无效`- 1
分析： 因为修改server.xml的时候是使用系统自带的记事本打开的，修改完成后，直接ctrl+s保存的。而记事本默认的编码方式是ANSI的，但是我们server.xml的文件头声明为UTF-8，所以我想应该是保存的时候，编码方式乱了，于是就修改了一下
文件头声明：
`<?xml version='1.0' encoding='utf-8'?>`- 1
解决： 使用记事本打开server.xml文件—-点击文件—另存为—修改编码方式为UTF-8 —覆盖保存
![这里写图片描述](https://img-blog.csdn.net/20161107153827648)
重新启动tomcat’ —–问题解决 OK ！
希望能对没有遇到这个问题的朋友有所帮助。。。。。
--------------------- 本文来自 xinghuo0007 的CSDN 博客 ，全文地址请点击：https://blog.csdn.net/xinghuo0007/article/details/53066791?utm_source=copy
