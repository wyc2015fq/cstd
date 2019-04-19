# WinCE平台NKDbgPrintfW等支持的格式符及浮点数打印方法 - xqhrs232的专栏 - CSDN博客
2014年05月23日 14:26:50[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1137
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/rubyer_perler/article/details/22377917](http://blog.csdn.net/rubyer_perler/article/details/22377917)
WinCE中，NKDbgPrintfW()等串口格式化输出方法只支持了printf的一个子集，子集如下。%x - Lower case hex
%B - Similar to %0.2X
%H - Similar to %0.4X
%X - Similar to %0.8X
%d - Decimal number
%u - Floating point number (possibly a solution to this question)
%s - String
%% - Prints a %
%c - Single character
因此NKDbgPrintfW等不支持浮点数打印.现提供一个替代方案。
打印浮点数方法
float floatNum = 0.4;
LPWSTR buffer=new TCHAR[50]; //申请缓存字符串
wsprintf(buffer,L"currentTime = %f\n",floatNum);
NKDbgPrintfW(buffer);
delete buffer; //不要忘记释放
//====================================================================================================================
备注::
1》WINCE下使用Retailmsg也不能直接打印浮点数，不知道是不是同样的原因？用%u也不行！
2》参考Dbgapi.h
d:\wince600\public\common\sdk\Inc 文件里面对RETAILMSG宏的定义，其实REATILMSG最后调的还是NKDbgPrintfW
#define RETAILMSG(cond,printf_exp)   \
   ((cond)?(NKDbgPrintfW printf_exp),1:0)
3》NKDbgPrintfW函数的具体定义
Format.cd:\wince600\public\common\oak\drivers\ethdbg\Edbgfrmt

