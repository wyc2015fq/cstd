# vs2015编译EasyDarwin，修复“_U32BITARG_”无效、__iob_func错误 - xcyl的口袋 - CSDN博客
2016年06月17日 23:50:40[xcyl](https://me.csdn.net/cai6811376)阅读数：3588
使用Visual Studio 2015编译EasyDarwin会遇到一些问题，这里将对遇到的错误提供解决办法。
**文本后缀“*U32BITARG*”无效；未找到文文本运算符或文本运算符模板“operator “”“”*U32BITARG*”**
![err1](https://img-blog.csdn.net/20160617234017823)
> 
出现此错误是因为Visual Studio 2015要求“如果宏直接跟在字符串后面而没有任何空格，则视为用户定义的文本后缀。”可见[https://support.microsoft.com/zh-cn/kb/3025133](https://support.microsoft.com/zh-cn/kb/3025133)
![ans1](https://img-blog.csdn.net/20160617234054168)
解决办法：将使用字符串替换宏的地方前后加空格
**EasyRedisClient.lib(hiredis.obj) : error LNK2019: 无法解析的外部符号__iob_func，该符号在函数_redisOOM中被引用**
![error2](https://img-blog.csdn.net/20160618000253695)
VS2015中，微软的标准库对标准输入输出流的宏定义改为
```
_ACRTIMP_ALT FILE* __cdecl __acrt_iob_func(unsigned);
#define stdin (__acrt_iob_func(0))
#define stdout (__acrt_iob_func(1))
#define stderr (__acrt_iob_func(2))
```
以前的版本定义为
```
_CRTIMP FILE * __cdecl __iob_func(void);
#define stdin (&__iob_func()[0])
#define stdout (&__iob_func()[1])
#define stderr (&__iob_func()[2])
```
所以就会导致静态库链接出现错误__iob_func，所以我们找重新定义__iob_func数组就可解决此问题。
以下是我给出的解决方法，只要在文件头加如下定义即可：
```
#ifdef __cplusplus
extern "C"
#endif
FILE __iob_func[3] = { __acrt_iob_func(0), __acrt_iob_func(1), __acrt_iob_func(2) };
```
如果提示__acrt_iob_func是未定义的标识符，只需在文件头加#include
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
QQ交流群：288214068
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
