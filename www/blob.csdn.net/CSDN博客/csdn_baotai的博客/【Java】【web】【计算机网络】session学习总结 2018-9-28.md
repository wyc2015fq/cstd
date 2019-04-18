# 【Java】【web】【计算机网络】session学习总结 2018-9-28 - csdn_baotai的博客 - CSDN博客

2018年09月28日 20:49:24[csdn-[baotai]](https://me.csdn.net/csdn_baotai)阅读数：51
个人分类：[Java																[计算机网络](https://blog.csdn.net/csdn_baotai/article/category/8051136)](https://blog.csdn.net/csdn_baotai/article/category/7659813)


# session学习总结

## Session概念：

> 
​	Session是服务器端技术，利用这个技术，服务器在运行时可以为每一个用户的浏览器创建一个其独享的session对象，由于session为用户浏览器独享，所以用户在访问服务器的web资源时，可以把各自的数据放在各自的session中，当用户再去访问服务器中的其它web资源时，其它web资源再从用户各自的session中取出数据为用户服务。

## Session的操作:

> - 
创建和获取Session对象.

HttpSession session = request.getSession(true);如果当前请求中存在一个Session对象,就直接返回,如果不存在Session对象,就先创建一个再返回

HttpSession session = request.getSession(false);如果当前请求中存在一个Session对象,就直接返回,如果不存在Session对象,就返回null.

HttpSession session = request.getSession();等价于HttpSession session = request.getSession(true);

- 
往Session中存储数据.

session对象.setAttribute(String name,Object value);

- 
从Session中取出数据.

Object value = session对象.getAttribute(String key);

- 
删除Session(用户注销登陆).

​     1):删除Session中指定属性名的值.

​        session对象.removeAttrbute(“currentName”);

​     2):销毁Session对象(Session中所有的属性都不存在).

​        session对象.invalidate();

- 
Session的超时管理

在超时时间之内,如果客户端和服务端没有交互(用户的两次操作之间不能超过该时间),则自动的销毁Session.

session对象.setMaxInactiveInterval(60 * 10);//超过10分钟,销毁Session.

Tomcat服务器的默认超时时间为:30分钟,Tomcat一般在20多分钟就销毁了.

- 
URL重写.

Session是一种特殊的Cookie,而浏览器可以禁用Cookie.

此时,需要在每一个资源之后,手动的携带session的ID.

/session/list;jsessionid=872870F9466CE7B3A11FD3B768FDD684

String url = response.encodeURL("/session/list");自动的在资源之后拼接;jsessionid=872870F9466CE7B3A11FD3B768FDD684
- 注意:开发中都不会取消接受Cookie的.

## Session的细节:

> - 一般的,我们存储到Session中的属性名称,要唯一,我们习惯XXX_IN_SESSION:

​     session对象.setAttribute(“USER_IN_SESSION”,“will”);
- 若需要把多个数据存放到Session中,就得调用setAttribute方法N次,可以的.

​     一般的,我们把需要存储的数据,封装成一个对象,然后在存储到Session中.

​     把用户的信息,封装到user对象.

​     session对象.setAttribute(“USER_IN_SESSION”,user对象);
- 如果多台服务器之间需要共享Session,此时Session中的对象,必须实现java.io.Serializable(才能在网络上传输).

​      序 列 化: 把对象信息存储为二进制.

​       反序列化: 把二进制信息恢复成对象.

​       public class User implements java.io.Serializable{…}

