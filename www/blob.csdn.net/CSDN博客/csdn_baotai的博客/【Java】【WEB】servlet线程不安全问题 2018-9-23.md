# 【Java】【WEB】servlet线程不安全问题  2018-9-23 - csdn_baotai的博客 - CSDN博客

2018年09月23日 11:03:19[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：52
个人分类：[Java](https://blog.csdn.net/csdn_baotai/article/category/7659813)



# servlet线程不安全问题

### 造成的根本原因是:Servlet是单例的,Servlet中的非static的成员变量只有一份,多个客户端好比是多个线程,都访问的是同一个空间.

解决方案:
- 
让当前Servlet实现javax.servlet.SingleThreadModel接口.

包装只有一个线程放Servlet,如果有多个线程就排队,如此的话,性能超低(已过时).

- 
在Servlet中不要使用成员变量,使用局部变量.

每一个用户,每一个请求都会调用service方法,而局部变量在service方法中,每一次都是新的空间.

Struts1,Spring MVC都是线程不安全的,都是单例的和Servlet类似.

Struts2是线程安全的,因为每一个线程(请求)都是一个新的Action对象.
![在这里插入图片描述](https://img-blog.csdn.net/20180923110250996?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NzZG5fYmFvdGFp/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

