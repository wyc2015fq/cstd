# 【VC】VS2008+WDK 配置方法解析 - xqhrs232的专栏 - CSDN博客
2017年09月03日 16:38:44[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：413
原文地址::[http://blog.csdn.net/shen_001/article/details/9794879](http://blog.csdn.net/shen_001/article/details/9794879)
相关文章
1、[转： vs2008 hidsdi.h 问题](http://blog.csdn.net/xuxinhua/article/details/6329182)----[http://blog.csdn.net/xuxinhua/article/details/6329182](http://blog.csdn.net/xuxinhua/article/details/6329182)
2、VS2008 + hidsdi.h 的问题----[http://www.lab-z.com/vs2008-hidsdi-h-%E7%9A%84%E9%97%AE%E9%A2%98/](http://www.lab-z.com/vs2008-hidsdi-h-%E7%9A%84%E9%97%AE%E9%A2%98/)
3、USB HID VS2008调试工具----[http://download.csdn.net/download/sunjiangjun2012/8810949](http://download.csdn.net/download/sunjiangjun2012/8810949)
4、USB_HID----[http://www.pudn.com/Download/item/id/2315400.html](http://www.pudn.com/Download/item/id/2315400.html)
WDK 下载地址：[http://www.microsoft.com/en-us/download/details.aspx?id=11800](http://www.microsoft.com/en-us/download/details.aspx?id=11800)
大家根据自己需要来下载。。
下载完大家就可以安装了！根据自己的OS选择相应的安装选项！
本人安装目录：D:\WinDDK\7600.16385.1
VS2008 添加方法：
工具——选项——项目和解决方案——VC++目录
**包含文件：**
D:\WinDDK\7600.16385.1\inc\ddk   //位置提前是为了编译时,匹配对应的文件，否则编译错误
$(VCInstallDir)include
$(VCInstallDir)atlmfc\include
D:\WinDDK\7600.16385.1\inc\api  //位置提前是为了编译时,匹配对应的文件，否则编译错误
$(WindowsSdkDir)\include
$(FrameworkSDKDir)include
**库文件：**
D:\WinDDK\7600.16385.1\lib
D:\WinDDK\7600.16385.1\lib\wxp\i386
添加以上内容就Ok了。。
新建一个控制台项目：
**[cpp]**[view
 plain](http://blog.csdn.net/shen_001/article/details/9794879#)[copy](http://blog.csdn.net/shen_001/article/details/9794879#)
- #include <windows.h>
- 
- #include <setupapi.h>
- 
- extren "C"//因为WDK是用C语言编写
- 
- {  
- 
- #include <hidsdi.h>
- 
- };  
- 
- #pragma comment(lib,"setupapi.lib")
- #pragma comment(lib,"hid.lib")
- 
- int main()  
- 
- {  
- 
- return 0;  
- 
- }  
- 
