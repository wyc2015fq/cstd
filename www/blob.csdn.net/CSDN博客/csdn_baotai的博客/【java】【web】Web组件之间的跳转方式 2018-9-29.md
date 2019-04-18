# 【java】【web】Web组件之间的跳转方式 2018-9-29 - csdn_baotai的博客 - CSDN博客

2018年09月29日 18:38:38[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：101


# Web组件之间的跳转方式:

方式1: 请求转发(forward).

方式2: URL重定向(redirect).

方式3: 请求包含(include).

## 请求转发:

从Servlet1,请求转发到Servlet2.

Servlet1完成一部分的功能,再跳转到Servlet2,继续完成剩下的功能.

### 语法:

request.getRequestDispatcher(String path).forward(request,response);

### 参数:

path,表示跳转的目标的路径(资源的名称).

注意:别忘了forward.

--------------------------------------------------------------

### 分析请求转发的特点:

> 
1:浏览器地址栏不发生改变,依然是/forward/s1,不是目标地址(/forward/s2).

2:请求转发只发送一个请求.

3:共享同一个请求中的数据.

4:最终响应给浏览器的由Servlet2来决定.

5:请求转发不能跨域访问,只能跳转到当前应用中的资源.

6:请求转发可以访问WEB-INF目录中的资源.

![在这里插入图片描述](https://img-blog.csdn.net/20180929183558754?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NzZG5fYmFvdGFp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## URL重定向:

### 语法:

response.sendRedirect(String url);

### 参数:

url,表示目标资源地址

--------------------------------------------------------------

### 分析URL重定向的特点:

> 
1:浏览器地址栏发生改变,从/redirect/s1,变成目标地址(/redirect/s2).

2:URL重定向发了两次请求.

3:因为URL重定向是两次独立的请求,所以不共享请求中的数据.

4:最终响应给浏览器的由Servlet2来决定(**把目标地址拷贝到浏览器地址栏,敲回车**).

5:URL重定向能跨域访问,可以访问其他应用中的资源.

6:URL重定向不能访问WEB-INF目录中的资源.

![在这里插入图片描述](https://img-blog.csdn.net/20180929183620559?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NzZG5fYmFvdGFp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

