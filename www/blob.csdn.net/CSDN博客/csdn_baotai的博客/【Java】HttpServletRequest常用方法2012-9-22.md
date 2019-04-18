# 【Java】HttpServletRequest常用方法2012-9-22 - csdn_baotai的博客 - CSDN博客

2018年09月22日 18:37:33[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：35


## HttpServletRequest常用方法

ServletRequest接口:请求对象,封装了获取所有请求信息(请求行,请求头,请求实体)的方法.

HttpServletRequest接口:是ServletRequest的子接口,处理HTTP协议请求的方法.

---------------------------------------------------------------------------
- 常用方法:
- String getMethod():返回请求方式：如GET/POST
- String getRequestURI():返回请求行中的资源名字部分:如/test/index.html
- StringBuffer getRequestURL():返回浏览器地址栏中所有的信息
- String getContextPath():返回当前项目的上下文路径(元素的path属性值.)
- String getRemoteAddr():返回发出请求的客户机的IP地址
- String getHeader(String name):返回指定名称的请求头的值。
- 获取请求参数的方法:
- String getParameter(String name):返回指定名字参数的值。
- String[] getParameterValues(String name):返回指定名字参数的多个参数值。
- Enumeration getParameterNames():返回所有参数名的Enumeration对象。
- Map<String,String[]> getParameterMap():返回所有的参数和值所组成的Map对象。

