# LRESULT|| - 三少GG - CSDN博客
2011年10月17日 19:55:34[三少GG](https://me.csdn.net/scut1135)阅读数：1672
## LRESULT
(2010-06-05 21:09:07)
LRESULT是一个数据类型，
　　MSDN： 32-bit value returned from a window procedure or callback function
**指的是从窗口程序或者回调函数返回的32位值,RESULT是一个数据类型。**
    LRESULT   Type   used   for   return   value   of   window   procedures
    windows用于程序的返回值
   请看在windef.h中的宏定义：
   typedef   long   LONG; 
   typedef   LONG   LRESULT;
   现在你应该明白它实际上就是long了吧！
   用在函数前仅表示函数返回值的类型而已！
   请注意函数定义和函数原型的匹配.
**这样做的目的无非是在不同的应用场合使用不同的写法以增进程序的可读性，及兼容函数原形。**
   LRESULT一般用户消息处理的函数   比如自定义的消息处理函数。
**在编写自定义消息函数时，若设置函数的返回值为void、UINT之类的，则编译器报错**：
“static_cast”:   无法从“UINT   (__thiscall   CABCDlg::*   )(CPoint)”转换为“LRESULT  (__thiscall   CWnd::*   )(CPoint)”
**根据这个提示，我把返回值都改成了LRESULT，则编译运行成功。**
详情请看ON_MESSAGE宏的定义...
就即便是VC6中不要是返回void的，因为它会破坏栈的。
//   for   Windows   messages
#define   ON_MESSAGE(message,   memberFxn)   \
{   message,   0,   0,   0,   AfxSig_lwl,   \
(AFX_PMSG)(AFX_PMSGW)   \
(static_cast <   LRESULT   (AFX_MSG_CALL   CWnd::*)(WPARAM,   LPARAM)   >   \
(memberFxn))   },
看到没有,是static_cast,如果返回类型不是LRESULT,static_cast会通不过的。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
