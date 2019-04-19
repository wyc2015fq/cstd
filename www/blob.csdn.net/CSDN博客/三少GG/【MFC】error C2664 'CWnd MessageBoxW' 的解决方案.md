# 【MFC】error C2664: 'CWnd::MessageBoxW' 的解决方案 - 三少GG - CSDN博客
2012年02月20日 23:58:24[三少GG](https://me.csdn.net/scut1135)阅读数：1349标签：[mfc																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
【摘自：[http://hi.baidu.com/ccst_kmyu/blog/item/ed198ef86a1716829f514662.html](http://hi.baidu.com/ccst_kmyu/blog/item/ed198ef86a1716829f514662.html)】
MessageBox("ddd");
编译通不过显示错误
f:\draw\draw\drawview.cpp(108) : error C2664: 'CWnd::MessageBoxW' : cannot convert parameter 1 from 'const char [4]' to 'LPCTSTR'
解决办法：
(1)
在项目属性->常规中，把Uicode改成**多字符段**。
(2)
MessageBox(TEXT("ddd"));
**以上两种方法用一个即可，用于VS2008**
