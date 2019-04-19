# Qt下 QString转char* - xqhrs232的专栏 - CSDN博客
2016年08月14日 14:22:59[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：330
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://www.cnblogs.com/Romi/archive/2012/03/12/2392478.html](http://www.cnblogs.com/Romi/archive/2012/03/12/2392478.html)

相关文章
1、[QString和char*的汉字乱码问题](http://blog.csdn.net/ilearncpp/article/details/8104253)----[http://blog.csdn.net/ilearncpp/article/details/8104253](http://blog.csdn.net/ilearncpp/article/details/8104253)
2、Qt GBK编码QString通过toUtf8转码后发现时乱码----[http://blog.sina.com.cn/s/blog_aec26f5801015erv.html](http://blog.sina.com.cn/s/blog_aec26f5801015erv.html)
Qt下面，字符串都用QString，确实给开发者提供了方便，想想VC里面定义的各种变量类型，而且函数参数类型五花八门，经常需要今年新那个类型转换
Qt再使用第三方开源库时，由于库的类型基本上都是标准的类型，字符串遇的多的就是Char*类型
在Qt下怎样将QString转char*呢，需要用到QByteArray类，QByteArray类的说明详见Qt帮助文档。
因为char*最后都有一个‘/0’作为结束符，而采用QString::toLatin1()时会在字符串后面加上‘/0’
方法如下：
Qstring  str;
char*  ch;
QByteArray ba = str.toLatin1();    
ch=ba.data();
这样就完成了QString向char*的转化。经测试程序运行时不会出现bug
注意第三行，一定要加上，不可以str.toLatin1().data()这样一部完成，可能会出错。
补充：以上方法当QString里不含中文时，没有问题，但是QString内含有中文时，转换为char*就是乱码，采用如下方法解决：
方法1：
添加GBK编码支持：
#include <QTextCodec>
QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
然后改变上面的第三行为：QByteArray ba = str.toLoacl8Bit();      toLoacl8Bit支持中文
方法2：
先将QString转为标准库中的string类型，然后将string转为char*，如下：
std::string str = filename.toStdString();
const char* ch = str.c_str();
