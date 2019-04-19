# Servlet运行过程 - HJ - CSDN博客
2018年03月12日 22:42:34[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：296
Sevlet程序是由Web服务器调用，其Servlet运行过程简述如下：
（1）Web服务器接收浏览器发出的“http请求”；
（2）Web服务器访问Servlet，如果是首次访问Servlet就必须先创建目标Servlet；
（3）调用Servlet实例对象的init()方法；
（4）创建一个用于封装HTTP请求消息的HttpServletRequest对象和一个代表HTTP响应消息的HttpServletResponse对象；
（5）调用Servlet的service()方法并将请求和响应对象作为参数传递进去。注意，Servlet容器调用service()方法对请求处理；
（6）返回Service方法，服务器卸载一个Servlet实例对象，并且在卸载之前调用Servlet的destroy()方法；
（7）Web服务器向浏览器发出http响应；
运行图解如下：
![](https://img-blog.csdn.net/2018031222411648)
![](https://img-blog.csdn.net/20180312224128768)
![](https://img-blog.csdn.net/20180312224138198)
![](https://img-blog.csdn.net/20180312224149220)
![](https://img-blog.csdn.net/20180312224159173)
![](https://img-blog.csdn.net/20180312224209442)
![](https://img-blog.csdn.net/2018031222422167)
