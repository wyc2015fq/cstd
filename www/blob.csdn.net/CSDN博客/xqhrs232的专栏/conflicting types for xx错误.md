# conflicting types for xx错误 - xqhrs232的专栏 - CSDN博客
2016年08月29日 14:00:07[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1126
原文地址::[http://blog.sina.com.cn/s/blog_5420e000010185o2.html](http://blog.sina.com.cn/s/blog_5420e000010185o2.html)
相关文章
1、[关于“error: conflicting types for 'function'”编译错误的分析](http://blog.csdn.net/yigeshouyiren/article/details/39082815)----[http://blog.csdn.net/yigeshouyiren/article/details/39082815](http://blog.csdn.net/yigeshouyiren/article/details/39082815)
编译libvmi 0.8版本时，出现以下错误：
libtool: compile:  gcc -DHAVE_CONFIG_H -I. -I.. -I.. -fvisibility=hidden -I/usr/include/glib-2.0
 -I/usr/lib/glib-2.0/include -g -O2 -MT libvmi_la-pretty_print.lo -MD -MP -MF .deps/libvmi_la-pretty_print.Tpo -c pretty_print.c  -fPIC -DPIC -o .libs/libvmi_la-pretty_print.o
pretty_print.c:31: error: conflicting types for ‘vmi_print_hex’
libvmi.h:749: note: previous declaration of ‘vmi_print_hex’ was here
make[3]: *** [libvmi_la-pretty_print.lo] Fehler 1
make[3]: Leaving directory `/usr/local/src/libvmi-0.8/libvmi'
make[2]: *** [all-recursive] Fehler 1
make[2]: Leaving directory `/usr/local/src/libvmi-0.8/libvmi'
make[1]: *** [all-recursive] Fehler 1
make[1]: Leaving directory `/usr/local/src/libvmi-0.8'
make: *** [all] Fehler 2
解决方案：
将**libvmi/libvmi.h:void vmi_print_hex (unsigned char *data, unsigned long length);和**
**libvmi/pretty_print.c:void vmi_print_hex (unsigned char *data, size_t length)**
**中的数据类型改为一致的即可。**
**见其论坛：[https://groups.google.com/forum/?fromgroups#!topic/vmitools/w76m93KevFg](https://groups.google.com/forum/?fromgroups#!topic/vmitools/w76m93KevFg)**
**常见此类问题的原因如下（引）**
错误：
test.c:22: error: conflicting types for 'urlencode'
test.c:18: error: previous implicit declaration of 'urlencode' was here
原因一：
原来是因为没有先做函数声明，而函数位于main()之后。
在main函数前声明了函数原型后，一切ok.
原因二：
头文件的被循环引用，在引用时考虑清楚包含顺序
原因三：
头文件声明和定义参数稍有不同
例：
 头文件中声明 void Hanlder(const char * buf);
 在定义时写作 void Hanlder(char * buf);
这是就会发生conflicting types for 错误问题
