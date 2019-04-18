# 【Java】【web】Servlet的三大作用域对象 2018-8-29 - csdn_baotai的博客 - CSDN博客

2018年09月29日 18:45:58[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：124


# Servlet的三大作用域对象:

## 目的:共享数据.
- request:        每一次请求都是一个新的request对象,如果在Web组件之间需要共享同一个请求中的数据,只能使用请求转发.
- session:        每一次会话都是一个新的session对象,如果需要在一次会话中的多个请求之间需要共享数据,只能使用session.
- application:    应用对象,Tomcat启动到Tomcat关闭,表示一个应用,在一个应用中有且只有一个application对象.作用于整个Web应用,可以实现多次会话之间的数据共享.
|对象名称|对象的类型|
|----|----|
|​     request|HttpServletRequest|
|session|HttpSession|
|servletcontext(application)|ServletContext|

### 作用域对象如何共享数据:
- 
设置作用域中的共享数据.

作用域对象.setAttribute(String name,Object value);

- 
获取作用域中的共享数据.

Object value   = 作用域对象.getAttribute(String name);

- 
删除作用域中的指定的共享数据.

作用域对象.removeAttribute(String name);

## 注意:

**在哪一个作用域中设置共享数据,就只能从该作用域中取出数据.**

