# QLibrary 动态加载外部库文件 - xqhrs232的专栏 - CSDN博客
2016年08月27日 15:49:05[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1151
原文地址::[http://blog.csdn.net/fm0517/article/details/8166678](http://blog.csdn.net/fm0517/article/details/8166678)
相关文章
1、qt调用.so库的问题----[http://bbs.csdn.net/topics/390489760](http://bbs.csdn.net/topics/390489760)
2、[QT源码解析(五）QLibrary跨平台调用动态库的实现](http://blog.csdn.net/tingsking18/article/details/5128307)----[http://blog.csdn.net/tingsking18/article/details/5128307](http://blog.csdn.net/tingsking18/article/details/5128307)
3、**QLibrary Class Reference(qt加载外部库)----[http://no001.blog.51cto.com/1142339/282536/](http://no001.blog.51cto.com/1142339/282536/)**
4、QLibrary跨平台调用动态库的实现----[http://blog.sina.com.cn/s/blog_5f853eb10100qfnh.html](http://blog.sina.com.cn/s/blog_5f853eb10100qfnh.html)
[cpp][view
 plain](http://blog.csdn.net/fm0517/article/details/8166678#)[copy](http://blog.csdn.net/fm0517/article/details/8166678#)
- /*转载请注明出处，谢谢*/
背景：想搭一个软件系统，包含一个主模块，一堆子模块（用于扩展功能）。主模块是一个可执行程序，子模块为动态库，可以加载在主模块中。
问题：每添加一个扩展功能就要修改一下主模块的源程序，在其中添加头文件，lib文件，然后重新编译。很是麻烦。
目标：主模块读一个配置文件，根据其中的内容，动态加载子模块的DLL，以实现扩展功能灵活配置，而不需要每次都修改和重新编译主模块。
下面是个例子：
子模块代码：
[cpp][view
 plain](http://blog.csdn.net/fm0517/article/details/8166678#)[copy](http://blog.csdn.net/fm0517/article/details/8166678#)
- extern"C"__declspec(dllexport) int print2(int a, int b);  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
[cpp][view
 plain](http://blog.csdn.net/fm0517/article/details/8166678#)[copy](http://blog.csdn.net/fm0517/article/details/8166678#)
- int print2(int a, int b)  
- {  
-     printf("VideoDecoderDLL printint!\n");  
- return (a+b);  
- }  
- 
主模块代码：
[cpp][view
 plain](http://blog.csdn.net/fm0517/article/details/8166678#)[copy](http://blog.csdn.net/fm0517/article/details/8166678#)
- #include <QLibrary>
- 
- void Shell::Test()  
- {  
- //m_vd->print();
- int *out=0;  
- typedefint (*myfun)(int, int);       // 定义导出函数类型  
-     QLibrary hdll( "I:/testDll/shell/VideoDecoderD.dll" );        //加载dll，当前目录  
- if(hdll.load())    
-     {    
-         myfun fun1 = (myfun)hdll.resolve("print2");        //用resolve来解析fun1函数  
- if ( fun1 )       //解析成功则进行运算并提示相关信息  
-         {    
-             printf("解析成功\n");  
-             printf("%d\n",fun1(2,3));    
-         }    
- else
-         {  
-             printf("解析失败\n");  
-         }  
-     }    
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
目前还没解决如何加载一个类，有知道办法的朋友欢迎留言。
