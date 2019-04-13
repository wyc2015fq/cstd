
# c语言 库打印函数 - 嵌入式Linux - CSDN博客

2019年02月28日 22:48:19[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：132


![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPcBUvIaicmXXpnADp0oL5ldtWRhT7EibO2uhaich7uOguFFw0l9t8n1dU7kDiaM86XwqlGgVaneYN0PMg/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPeiauStJMDaoxoicA9rgjdWtkKj2ZZA5bCEcib9Z8bkfqBBZoO2eItzHChicibqZlWc4dP9LHIJIUGGLicg/640?wx_fmt=jpeg)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPd3pDeEtePDsNGbO6Q6AhH7vSYG8ygKrWwk9jPucDicgJVWfuO0BZVian5VPqZc3NCrAlHfwsMaTANw/640?wx_fmt=png)
## 函数








`#``include``<stdio.h>``int``printf``(``const``char``*format, ... )``;``/* [until c99]写结果到stdout */``int``printf``(``const``char``*``restrict``format, ... )``;``/* [since c99] */``int``fprintf``(FILE *stream,``const``char``*format, ... )``;``/* [until c99]写结果到文件流stream */``int``fprintf``(FILE *``restrict``stream,``const``char``*``restrict``format, ... )``;``int``sprintf``(``char``*dest,``const``char``*format, ... )``;``/* [until c99]写结果至dest */``int``sprintf``(``char``*``restrict``dest,``const``char``*``restrict``format, ... )``;``int``snprintf``(``char``*``restrict``dest,``size_t``bufsz,``const``char``*``restrict``format, ... )``;``/* [since c99]写结果至dest，至多写bufsz-1个字符。产生的字符串会以空字符终止，除非bufsz为0。若bufsz为0，则不写入任何内容，且dest可以是空指针，然而依旧计算返回值（会写入的字符数，不包含空终止符）并返回。 */``int``vprintf``(``const``char``*format, va_list ap)``;``int``vprintf``(``const``char``*``restrict``format, va_list ap)``;``int``vfprintf``(FILE *stream,``const``char``*format, va_list ap)``;``int``vfprintf``(FILE *``restrict``stream,``const``char``*``restrict``format, va_list ap)``;``int``vsprintf``(``char``*dest,``const``char``*format, va_list ap)``;``int``vsprintf``(``char``*``restrict``dest,``const``char``*``restrict``format, va_list ap)``;``int``vsnprintf``(``char``*``restrict``dest,``size_t``bufsz,``const``char``*``restrict``format, va_list ap)``;`
## 参数

`stream``-- 要写入的输出文件流``bufsz``-- 最多写入bufsz-1个字符，再加上空终止符``format -- 指向指定数据转译方式的空终止多字节字符串的指针`![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPcBUvIaicmXXpnADp0oL5ldtBqkc5Swq0vfI9FsMapvlV5WruVsDbYdlaFXBiaQZTMpdjN1ulIIZEDQ/640?wx_fmt=png)
## Example






























































`#include <stdio.h>``int``main()``{``/* String test */``char *str =``"hello"``;``printf``(``"str = .%s.\r\n"``, str);``printf``(``"str = .%-10s.\r\n"``, str);``printf``(``"str = .%10s.\r\n"``, str);``printf``(``"str = .%-*s.\r\n"``,``10``, str);``printf``(``"str = .%*s.\r\n"``,``10``, str);``printf``(``"str = .%*.*s.\r\n"``,``10``,``5``, str);``printf``(``"str = .%*.*s.\r\n"``,``10``,``2``, str);``printf``(``"str = .%10.5s.\r\n"``, str);``printf``(``"str = .%10.2s.\r\n"``, str);``/* Integer test */``printf``(``"dec = .%d.\r\n"``,``555``);``printf``(``"dec = .%10d.\r\n"``,``555``);``printf``(``"dec = .%10.5d.\r\n"``,``555``);``printf``(``"dec = .%-10.5d.\r\n"``,``555``);``printf``(``"dec = .%*.*d.\r\n"``,``10``,``5``,``555``);``printf``(``"dec = .%*.*d.\r\n"``,``10``,``2``,``555``);``printf``(``"hex = .%x.\r\n"``,``555``);``printf``(``"hex = .%X.\r\n"``,``555``);``printf``(``"hex = .%#x.\r\n"``,``555``);``printf``(``"hex = .%10x.\r\n"``,``555``);``printf``(``"hex = .%10.5x.\r\n"``,``555``);``printf``(``"hex = .%-10.5x.\r\n"``,``555``);``printf``(``"hex = .%*.*x.\r\n"``,``10``,``5``,``555``);``printf``(``"hex = .%*.*x.\r\n"``,``10``,``2``,``555``);``printf``(``"oct = .%o.\r\n"``,``555``);``printf``(``"oct = .%#o.\r\n"``,``555``);``printf``(``"oct = .%10o.\r\n"``,``555``);``printf``(``"oct = .%10.5o.\r\n"``,``555``);``printf``(``"oct = .%-10.5o.\r\n"``,``555``);``printf``(``"oct = .%*.*o.\r\n"``,``10``,``5``,``555``);``printf``(``"oct = .%*.*o.\r\n"``,``10``,``2``,``555``);``/* Float test */``printf``(``"flt = .%f.\r\n"``,``3.1415``);``printf``(``"flt = .%lf.\r\n"``,``3.1415``);``printf``(``"flt = .%10f.\r\n"``,``3.1415``);``printf``(``"flt = .%10.5f.\r\n"``,``3.1415``);``printf``(``"flt = .%-10.5f.\r\n"``,``3.1415``);``printf``(``"flt = .%*.*f.\r\n"``,``10``,``5``,``3.1415``);``printf``(``"flt = .%*.*f.\r\n"``,``10``,``2``,``3.1415``);``return``0``;``}``/* 运行结果 *``/``str = .hello.``str = .hello     .``str = .     hello.``str = .hello     .``str = .     hello.``str = .     hello.``str = .        he.``str = .     hello.``str = .        he.``dec = .555.``dec = .       555.``dec = .     00555.``dec = .00555     .``dec = .     00555.``dec = .       555.``hex = .22b.``hex = .22B.``hex = .0x22b.``hex = .       22b.``hex = .     0022b.``hex = .0022b     .``hex = .     0022b.``hex = .       22b.``oct = .1053.``oct = .01053.``oct = .      1053.``oct = .     01053.``oct = .01053     .``oct = .     01053.``oct = .      1053.``flt = .3.141500.``flt = .3.141500.``flt = .  3.141500.``flt = .   3.14150.``flt = .3.14150   .``flt = .   3.14150.``flt = .      3.14.`
`/``* snprintf实现strlen功能 *``/``#include <stdio.h>``#include <string.h>``int main()``{``char *str = "helloworld";``int slen = snprintf(NULL, 0, "%s", str);``printf("%d - %ld\r\n",slen, strlen(str));``return 0;``}``#include <stdio.h>``#include <stdarg.h>``#include <time.h>``void debug_log(const char *fmt, ...)``{``struct timespec ts;``timespec_get(&ts, TIME_UTC);``char time_buf[100];``size_t rc = strftime(time_buf, sizeof time_buf, "%D %T", gmtime(&ts.tv_sec));``snprintf(time_buf + rc, sizeof time_buf - rc, ".%06ld UTC", ts.tv_nsec /``1000``);``va_list args1;``va_start(args1, fmt);``va_list args2;``va_copy(args2, args1);``char buf[``1``+vsnprintf(NULL,``0``, fmt, args1)];``va_end(args1);``vsnprintf(buf, sizeof buf, fmt, args2);``va_end(args2);``printf``(``"%s [debug]: %s\n"``, time_buf, buf);``}``int``main(void)``{``debug_log(``"Logging, %d, %d, %d"``,``1``,``2``,``3``);``}`

文章转载自我师兄的博客，内容是我师兄总结的，如果有转载的，请注明出处，原文请点击左下角原文链接



![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/Qof5hj3zMPd3pDeEtePDsNGbO6Q6AhH7vSYG8ygKrWwk9jPucDicgJVWfuO0BZVian5VPqZc3NCrAlHfwsMaTANw/640?wx_fmt=png)

当你看到这里的时候，说明你已经阅读完上面的内容
不管怎样，感谢您有心或者无意的关注和支持
想获取学习**1024G**资料，请点击状态栏**公众号福利**按钮

![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/Qof5hj3zMPdjf4zGB2y66oSmGibcFIozabEFpsMLLROecoibJKwC177cvkgQcB7n7NLpNJLUSeGTKqUHAHQhJDxw/640?wx_fmt=jpeg)





