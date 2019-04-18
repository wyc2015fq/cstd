# JSP自定义标签_属性接收复杂类型处理方式 - z69183787的专栏 - CSDN博客
2016年04月28日 19:33:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1852
个人分类：[Jsp-Tag标签](https://blog.csdn.net/z69183787/article/category/2175879)

原文：[点击打开链接](http://www.cnblogs.com/lichone2010/p/3147375.html)
jsp定义属性,如果接收参数为日期格式:
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@taglib uri="/simpleitcast" prefix="itcast"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <title>如果标签接收的是一个复杂类型，如果给其赋值</title>
  </head>
  
  <body>
  
  <% 
      Date d = new Date();
      request.setAttribute("date",d);
  %>
  
   <itcast:demo6 date="${date}"><!--第一种处理方式--!>
  </itcast:demo6>
  
  <itcast:demo6 date="<%=new Date() %>"><!--第二种处理方式--!>
  </itcast:demo6>
     
  </body>
</html>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
import java.io.IOException;
import java.util.Date;
import javax.servlet.jsp.JspException;
import javax.servlet.jsp.tagext.SimpleTagSupport;
//属性说明
public class SimpleTagDemo6 extends SimpleTagSupport {
    private Date date;
    public void setDate(Date date) {
        this.date = date;
    }
    @Override
    public void doTag() throws JspException, IOException {
        System.out.println(date);
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<tag>
        <name>demo6</name>  <!-- 为标签处理器类配一个标签名 -->
        <tag-class>cn.itcast.web.simpletag.SimpleTagDemo6</tag-class>
        <body-content>scriptless</body-content>   
        <attribute>
            <name>date</name>
            <required>true</required>
            <rtexprvalue>true</rtexprvalue>  <!-- 指示属性的值是否可以为一个表达式 -->
        </attribute>
        
    </tag>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
