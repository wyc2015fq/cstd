# jni的中文字符串处理 - xqhrs232的专栏 - CSDN博客
2011年06月22日 18:04:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：860标签：[jni																[java																[dst																[string																[linux																[平台](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=dst&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/beiai1014/archive/2007/04/12/1561857.aspx](http://blog.csdn.net/beiai1014/archive/2007/04/12/1561857.aspx)
几个概念首先要明确：
java内部是使用16bit的unicode编码（UTF-16）来表示字符串的，无论中文英文都是2字节； 
jni内部是使用UTF-8编码来表示字符串的，UTF-8是变长编码的unicode，一般ascii字符是1字节，中文是3字节； 
c/c++使用的是原始数据，ascii就是一个字节了，中文一般是GB2312编码，用两个字节来表示一个汉字。 
明确了概念，操作就比较清楚了。下面根据字符流的方向来分别说明一下
1、java --> c/c++
这种情况中，java调用的时候使用的是UTF-16编码的字符串，jvm把这个字符串传给jni，c/c++得到的输入是jstring，这个时候，可以利用jni提供的两种函数，一个是GetStringUTFChars，这个函数将得到一个UTF-8编码的字符串；另一个是GetStringChars这个将得到UTF-16编码的字符串。无论那个函数，得到的字符串如果含有中文，都需要进一步转化成GB2312的编码。
       String       (UTF-16)          |[java]    |--------------------  JNI 调用[cpp]     |          v       jstring        (UTF-16)          |    +--------+---------+ |GetStringChars    |GetStringUTFChars |                  | v                  vwchar_t*          
 char*(UTF_16)           (UTF-8)                 2、c/c++ --> java
jni返回给java的字符串，c/c++首先应该负责把这个字符串变成UTF-8或者UTF-16格式，然后通过NewStringUTF或者NewString来把它封装成jstring，返回给java就可以了。
       String       (UTF-16)          ^          |[java]    |--------------------  JNI 返回[cpp]     |       jstring        (UTF-16)          ^          |    +--------+---------+ ^                  ^ |                  | |NewString         |NewStringUTFwchar_t*         
 char*(UTF_16)          (UTF-8)                 如果字符串中不含中文字符，只是标准的ascii码，那么用GetStringUTFChars/NewStringUTF就可以搞定了，因为这种情况下，UTF-8编码和ascii编码是一致的，不需要转换。
但是如果字符串中有中文字符，那么在c/c++部分进行编码转换就是一个必须了。我们需要两个转换函数，一个是把UTF8/16的编码转成GB2312；一个是把GB2312转成UTF8/16。
这里要说明一下：linux和win32都支持wchar，这个事实上就是宽度为16bit的unicode编码UTF16，所以，如果我们的c/c++程序中完全使用wchar类型，那么理论上是不需要这种转换的。但是实际上，我们不可能完全用wchar来取代char的，所以就目前大多数应用而言，转换仍然是必须的。
具体的转换函数，linux和win32都有一定的支持，比如glibc的mbstowcs就可以用来把GB2312编码转成UTF16，但是这种支持一般是平台相关的（因为c/c++的标准中并没有包括这部分），不全面的（比如glibc就没有提供转成UTF8的编码），不独立的（linux下mbstowcs的行为要受到locale设置的影响）。所以我推荐使用iconv库来完成转换。
iconv库是一个免费的独立的编码转换库，支持很多平台，多种编码（事实上，它几乎可以处理我们所使用的所有字符编码），而且它的行为不受任何外部环境的影响。iconv在*nix平台上，基本上是缺省安装的。在win32平台上需要额外安装。
下面提供一个把GB2312编码的字符串转换成UTF8编码的示例
#include <iconv.h>char* BytesToUtf8(string src, char* dst, int* nout) {    size_t n_in = src.length();    size_t n_out = *nout;        iconv_t c = iconv_open("UTF-8", "GB2312");    if (c == (iconv_t)-1) {        cerr << strerror(errno) << endl;        return
 NULL;    }    char* inbuf = new char [n_in + 1];    if (!inbuf) {        iconv_close(c);        return NULL;    }    strcpy(inbuf, src.c_str());    memset(dst, 0, n_out);    char* in = inbuf;    char* out = dst;    if (iconv(c, &in, &n_in, &out, &n_out) ==
 (size_t)-1) {        cerr << strerror(errno) << endl;        out = NULL;    }    else {        n_out = strlen(dst);        out = dst;    }    iconv_close(c);    *nout = n_out;    delete[] inbuf;    return out;}补充几点说明：1、从jni的接口看，jni提供了UTF16和UTF8两个系列的字符串处理函数，但是由于jni的文档中说，jni的内部实现中是用UTF8作为字符串编码格式的，所以使用UTF8系列比较合适（NewStringUTF/GetStringUTFChars/ReleaseStringUTFChars）
2、使用iconv库的话，运行环境的设置对于编码转换是没有影响的，但是外层java程序对于字符串的解析依赖于运行环境的locale，所以设置正确的locale对于jni意义不大，但是对整个系统还是必要的。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/beiai1014/archive/2007/04/12/1561857.aspx](http://blog.csdn.net/beiai1014/archive/2007/04/12/1561857.aspx)

