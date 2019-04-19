# error C4996: 'sprintf': This function or variable may be unsafe 解决方法 - fanyun的博客 - CSDN博客
2017年06月30日 20:10:18[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：615标签：[C++](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[C++](https://blog.csdn.net/fanyun_01/article/category/6102444)
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
在vs2013环境下编译别人写的C程序代码，在遇到输入输出时或字符串，会遇到一些错误。如下面一段代码
**[cpp]**[view
 plain](http://blog.csdn.net/u011707827/article/details/51221469#)[copy](http://blog.csdn.net/u011707827/article/details/51221469#)
- sprintf(imageNumberText, "image-%04d.jpg", imageNumber++);  
编译后，系统报错：
错误 1error C4996: 'sprintf': This function or variable may be unsafe. Consider using sprintf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online
 help for details.E:\VS2013_AR_Project\ARSimpleTest\ARSimpleTest\simpleTest.cpp392 1 ARSimpleTest
这不是语法的错误，而是IDE默认禁止这种容易产生漏洞的旧函数，解决的方法，一种是替换为新的更安全的函数，另一种在预编译出添加下面一行：
#pragma warning(disable:4996)
在网上查了以下资料，很能是互相照抄，大多写成了 prama，编译还是出错，写成上面一行的就行了。
