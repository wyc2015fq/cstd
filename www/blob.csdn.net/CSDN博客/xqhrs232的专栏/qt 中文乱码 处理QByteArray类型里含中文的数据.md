# qt 中文乱码 处理QByteArray类型里含中文的数据 - xqhrs232的专栏 - CSDN博客
2018年05月16日 11:03:48[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：350
原文地址::[https://blog.csdn.net/gllg1314/article/details/46989953](https://blog.csdn.net/gllg1314/article/details/46989953)
相关文章
1、Qt Creator 设置默认编码格式为 UTF-8----[https://blog.csdn.net/abeldeng/article/details/24770907](https://blog.csdn.net/abeldeng/article/details/24770907)
2、Qt中Error:Could not decode "xxx.cpp" with "System"-encoding.Editing not possible.的解决方案----[https://blog.csdn.net/mnonm_mnonm_mnonm/article/details/8480700](https://blog.csdn.net/mnonm_mnonm_mnonm/article/details/8480700)
qt解析tcp通信传来的xml时，中文有乱码
解决方法：
头文件添加 #include<QTextCodec>
QByteArraytmpQBA=m_pSocket->readAll();
QTextCodec*tc=QTextCodec::codecForName("GBK");QStringtmpQStr=tc->toUnicode(tmpQBA);
//如下转换会导致中文乱码
QStringtmpQStr=QVariant(tmpQBA).toString();
