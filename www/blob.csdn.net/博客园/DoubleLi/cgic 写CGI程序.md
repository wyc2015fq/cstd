# cgic 写CGI程序 - DoubleLi - 博客园






CGIC是C语言CGI库函数,用于编写CGI程序

**CGIC 主要完成以下功能**：

* 对数据进行语法分析 

* 接收以 GET 和 PSOT 两种方式发送的数据 

* 把 FORM 中的不同域连接成连续的串 

* 为检索 FORM 数据而提供字符串 , 整数 , 浮点以及单项和多项选择功能 

* 为数字字段提供边界检测 

* 把 CGI 环境变量加载到非空的 C 串中 

* 为调试而捕捉 CGI 状态 



**如何写CGIC应用程序**

任何cgic 应用程序必须连接到cgic.c，如果在Linux 下可以用Makefile 做这个工作。

因为CGI 程序需要做一些初始化操作，如获取环境变量，分析数据等，所以cgic 库提供了自己的main()函数，那自己写的cgic程序不能再包含main()函数，而必须以cgiMain()函数开始，cgic.c会调用这个函数。当然这个函数名是可以更改的.还要注意的是，自己编写的cgic 程序必须包含#include<cigi.h>

 也就是说cgic,已经做了一些相关的解析工作，在自己的main函数中只要调用cigic 中相应的函数便可。

以下是cigic 库里自带的cgic test程序的cgiMain()





**[cpp]**[view plain](http://blog.csdn.net/gl1987807/article/details/7959195#)[copy](http://blog.csdn.net/gl1987807/article/details/7959195#)

[print](http://blog.csdn.net/gl1987807/article/details/7959195#)[?](http://blog.csdn.net/gl1987807/article/details/7959195#)

- int cgiMain() {  
- #ifdef DEBUG  
-   LoadEnvironment();  
- #endif /* DEBUG */  
- /* Load a previously saved CGI scenario if that button 
-     has been pressed. */  
- if (cgiFormSubmitClicked("loadenvironment") == cgiFormSuccess) {  
-     LoadEnvironment();  
-   }  
- /* Set any new cookie requested. Must be done *before* 
-     outputting the content type. */  
-   CookieSet();  
- /* Send the content type, letting the browser know this is HTML */  
-   cgiHeaderContentType("text/html");  
- /* Top of the page */  
-   fprintf(cgiOut, "<HTML><HEAD>\n");  
-   fprintf(cgiOut, "<TITLE>cgic test</TITLE></HEAD>\n");  
-   fprintf(cgiOut, "<BODY><H1>cgic test</H1>\n");  
- /* If a submit button has already been clicked, act on the  
-     submission of the form. */  
- if ((cgiFormSubmitClicked("testcgic") == cgiFormSuccess) ||  
-     cgiFormSubmitClicked("saveenvironment") == cgiFormSuccess)  
-   {  
-     HandleSubmit();  
-     fprintf(cgiOut, "<hr>\n");  
-   }  
- /* Now show the form */  
-   ShowForm();  
- /* Finish up the page */  
-   fprintf(cgiOut, "</BODY></HTML>\n");  
- return 0;  
- }  







**[cpp]**[view plain](http://blog.csdn.net/gl1987807/article/details/7959195#)[copy](http://blog.csdn.net/gl1987807/article/details/7959195#)

[print](http://blog.csdn.net/gl1987807/article/details/7959195#)[?](http://blog.csdn.net/gl1987807/article/details/7959195#)

- #ifdef DEBUG  
-     LoadEnvironment();  
- #endif /* DEBUG */  



一般采用以上方式，调用环境变量用作调试。 可以插入语句："#define DEBUG 1 将DEBUG模式打开。

 cgiHeaderContentType("text/html") 这个函数主要是输出Content Type Header，告诉浏览器返回的是一个html页面，当然也可以是"image/gif", "image/jpeg" and "audio/wav



外一点就是，cgi程序输出HTML页面的方式都是使用printf把页面一行一行地打印出来，比如cgictest.c中的这一段代码：
fprintf(cgiOut, "<textarea NAME=\"address\" ROWS=4 COLS=40>\n");
fprintf(cgiOut, "Default contents go here. \n");
fprintf(cgiOut, "</textarea>\n");
上面这段代码的运行结果就是在页面上输出一个textarea。 第一个参数cgiOut实际上就是stdin，所以我们可以直接使用printf，而不必使用fprintf。不过在调试的时候会用到fprintf来重定向输出



CGI 调试起来比较麻烦，CGIC提供了一个函数可以将环境变量输入到文件，方便调试。





**[cpp]**[view plain](http://blog.csdn.net/gl1987807/article/details/7959195#)[copy](http://blog.csdn.net/gl1987807/article/details/7959195#)

[print](http://blog.csdn.net/gl1987807/article/details/7959195#)[?](http://blog.csdn.net/gl1987807/article/details/7959195#)

- cgiWriteEnvironment("/etc/conf.d/c0/capcgi.dat");  
- 










