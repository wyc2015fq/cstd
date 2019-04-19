# Qt处理中文乱码详解 - fanyun的博客 - CSDN博客
2017年10月28日 19:36:34[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：229
          Qt4.X版本对中文支持不是很好，Qt5对中文支持比Qt4好多了，但是依然会遇到中文支持乱码问题，甚至在一个源码里面，对一些中文支持很好，但是有些中文字符添加就会带来编译错误，常见的中文支持错误如下图1.1所示：
![](https://img-blog.csdn.net/20171028194054520?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
解决办法一：
            把源码在UE打开，设置保存编码为UTF-8带Bom格式编码，再次编译，编译通过。当然，这是在windows平台，如果要在linux平台，则需要修改为带Bom的UTF-8编码！           
解决方案二：
所有字符串常量使用 QStringLiteral 宏进行包裹：
```cpp
QString s1 = QStringLiteral("支付宝");
      widget->setCashInfo(s1);
```
       当然，我们在除了宽字符串外，你还需要使用窄字符串，Qt5要求窄字符串执行字符集为 UTF8，比如：
```cpp
QString s1 = "余额";
QString s2 = tr("余额");
QByteArray s3 = "余额";
const char * s4 = "余额";
edit.setText("余额");
```
      那么，我们需要在文件头部添加下面3行代码：
```cpp
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif
```

