# Struts2 中 Session 的使用 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月17日 16:33:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：22
个人分类：[J2EE](https://blog.csdn.net/leixiaohua1020/article/category/1362940)










在Struts2里，如果需要在Action中使用session，可以通过下面两种方式得到

1.通过ActionContext class中的方法getSession得到

2.Action实现org.apache.struts2.interceptor.SessionAware接口的方式来对session进行操作

下面先看一个采用第一种方式，在action中得到session的例子


```java
package s2.ex.action; 

import java.util.Map; 
import com.opensymphony.xwork2.ActionContext; 
import com.opensymphony.xwork2.ActionSupport; 

public class SessionTestAction extends ActionSupport { 

    public String execute() { 
     ActionContext actionContext = ActionContext.getContext(); 
       Map session = actionContext.getSession(); 
       session.put("USER_NAME", "Test User"); 
       return SUCCESS; 
    } 

}
```


在这个例子中，通过ActionContext得到session，并往session里放置一个key为USER_NAME，值为Test User的内容。



下面是一个实现org.apache.struts2.interceptor.SessionAware接口来对session操作的例子

```java
package s2.ex.action; 

import java.util.Map; 
import org.apache.struts2.interceptor.SessionAware; 
import com.opensymphony.xwork2.ActionSupport; 

public class SessionTest1Action extends ActionSupport implements SessionAware { 

    private Map session; 

    public void setSession(Map session) { 
       this.session = session; 
    } 

    public String execute() { 
       this.session.put("USER_NAME", "Test User 1"); 
       return SUCCESS; 
    } 

}
```


在这个例子中实现了接口SessionAware中的setSession方法。

上面两种方式都可以得到session，能实现的功能都是一样的。

这里推荐通过第二种方式来使用session，原因是便于做单体测试，用第二种方式，只需要构造一个Map就可以对action class进行单体测试了。

 在一个项目中可能会有很多action都需要用到session，如果每个action都来实现org.apache.struts2.interceptor.SessionAware这个接口，可能会显得比较麻烦，所以建议作一个抽象的BaseAction类来实现org.apache.struts2.interceptor.SessionAware接口，以后所有的action只要继承这个BaseAction就可以了。

```
<%@ page contentType="text/html; charset=UTF-8" %> 
<%@page pageEncoding="utf-8" %> 
<%@taglib prefix="s" uri="/struts-tags" %> 
<html> 

<head> 
    <title>Session Test</title> 
</head> 

<body> 
<h1><s:property value="#session.USER_NAME"/></h1> 
<h1></h1> 
</body> 
</html>
```

一般在项目中往往会往session里放置一个Object，必如说user，user里有个boolean admin和String userName，如果user里存在isAdmin的方法，在jsp中可以通过<s:if test="#session.user.admin">来判断用户有没有管理权限，通过<s:property value="#session.user.userName">或者来取得用户名。



原文地址：[http://calatustela.iteye.com/blog/548502](http://calatustela.iteye.com/blog/548502)






