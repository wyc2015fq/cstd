# QDataStream中的写入指针数组 - xqhrs232的专栏 - CSDN博客
2018年08月15日 23:32:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：166
原文地址::[http://blog.163.com/qimo601@126/blog/static/15822093201321104959456/](http://blog.163.com/qimo601@126/blog/static/15822093201321104959456/)
相关文章
1、[从QDataStream向QByteArray中写入数据时的注意点（QT）](https://www.cnblogs.com/chinese-zmm/archive/2010/05/29/1746831.html)----[https://www.cnblogs.com/chinese-zmm/archive/2010/05/29/1746831.html](https://www.cnblogs.com/chinese-zmm/archive/2010/05/29/1746831.html)
在QDataStream中有两个方法：
QDataStream & **[writeBytes](http://blog.163.com/qimo601@126/blog/static/15822093201321104959456/qdatastream.html#writeBytes)** ( const char * *s*, uint *len* )
> 
Writes the length specifier *len* and the buffer *s* to the stream and returns a reference to the stream.
The *len* is serialized as a quint32, followed by *len* bytes from *s*. Note that the data is *not* encoded.
int **[writeRawData](http://blog.163.com/qimo601@126/blog/static/15822093201321104959456/qdatastream.html#writeRawData)** ( const char * *s*, int *len* )
> 
Writes *len* bytes from *s* to the stream. Returns the number of bytes actually written, or -1 on error. The data is *not*encoded.
这个方法可以提高写入文件的速度。
仔细看英文可以发现这两个方法的区别是：writeBytes方法是将char * s的长度 和数据内容，都写入数据流。而writeRawData只写入纯数据内容。
如下代码看看writeBytes写后的文件内容：
`   1: QFile nakedFile;``   2:     nakedFile.setFileName("naked.txt");``   3:     if (!nakedFile.open(QIODevice::WriteOnly))``   4:     {``   5:         qDebug("打开裸数据文件naked.txt失败");``   6:         return;``   7:     }``   8:     ``   9:     qint32 * projectUnit = new qint32[10752]();``  10:     for(int i = 0; i <10752; i++)``  11:     {``  12:         projectUnit[i] = i;``  13:     }``  14:     QDataStream out(&nakedFile);``  15:     //QDataStream out(&nakedFile);``  16:     out.setVersion(QDataStream::Qt_4_6);``  17: ``  18:     out.writeBytes((char*)projectUnit,10752*4);`
![1.bmp](http://img2.ph.126.net/BZVdHIuC0xdv0DS-291INg==/6597147831983992142.jpg)
长度为0xA800 然后后面才跟着数据。
