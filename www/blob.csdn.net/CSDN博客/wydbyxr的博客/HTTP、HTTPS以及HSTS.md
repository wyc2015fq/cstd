# HTTP、HTTPS以及HSTS - wydbyxr的博客 - CSDN博客
2018年12月21日 10:19:03[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：24
个人分类：[计算机基础和编程基础](https://blog.csdn.net/wydbyxr/article/category/7751906)
# HTTP、HTTPS
  http是超文本传输协议，信息是明文传输，https则是具有安全性的ssl加密传输协议。
# HSTS
  HSTS（HTTP Strict Transport Security）国际互联网工程组织IETF正在推行一种新的Web安全协议。
  HSTS的作用是强制客户端（如浏览器）使用HTTPS与服务器创建连接。
  HSTS可以很大程度上解决SSL剥离攻击，因为只要浏览器曾经与服务器创建过一次安全连接，之后浏览器会强制使用HTTPS，即使链接被换成了HTTP。
