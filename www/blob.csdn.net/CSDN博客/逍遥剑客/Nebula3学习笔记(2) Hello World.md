# Nebula3学习笔记(2): Hello World - 逍遥剑客 - CSDN博客
2008年07月02日 18:04:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2900标签：[system																[null																[测试](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=system&t=blog)
个人分类：[引擎研究](https://blog.csdn.net/xoyojank/article/category/337622)
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
                就以这个程序做为测试程序的模板吧
 1: #include 
 2: #include 
 3: #include 
 4: #include 
 5:  6: using namespace Core
 7:  8:  9: 10: 11: 	PtrCoreServer coreServer  CoreServerCreate
12: 	coreServerSetAppName
13: 	coreServerOpen
14: 15: 	n_printf
16: 	system
17: 18: 	coreServerClose
19: 	coreServer  NULL
20: 21: 	CoreSysFuncExit
22: ![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/Nebula3_Hello_World_Include.JPG)![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/Nebula3_Hello_World_Library.JPG)![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/Nebula3_Hello_World_Comiler.JPG)![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/Nebula3_Hello_World_Comiler_Advanced.JPG)
