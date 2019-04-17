# JSP 自动刷新 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年08月09日 17:10:55[boonya](https://me.csdn.net/boonya)阅读数：384标签：[jsp																[refresh](https://so.csdn.net/so/search/s.do?q=refresh&t=blog)](https://so.csdn.net/so/search/s.do?q=jsp&t=blog)
个人分类：[Servlet/JSP](https://blog.csdn.net/boonya/article/category/2864195)








文章来源：[http://www.runoob.com/jsp/jsp-auto-refresh.html](http://www.runoob.com/jsp/jsp-auto-refresh.html)



想象一下，如果要直播比赛的比分，或股票市场的实时状态，或当前的外汇配给，该怎么实现呢？显然，要实现这种实时功能，您就不得不规律性地刷新页面。

JSP提供了一种机制来使这种工作变得简单，它能够定时地自动刷新页面。

刷新一个页面最简单的方式就是使用response对象的setIntHeader()方法。这个方法的签名如下：
publicvoid setIntHeader(String header,int headerValue)
这个方法通知浏览器在给定的时间后刷新，时间以秒为单位。

## 页面自动刷新程序示例

这个例子使用了setIntHeader()方法来设置刷新头，模拟一个数字时钟：
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="java.io.*,java.util.*" %>
<html><head><title>自动刷新实例</title></head><body><h2>自动刷新实</h2><%// 设置每隔5秒刷新一次
   response.setIntHeader("Refresh",5);// 获取当前时间Calendar calendar =newGregorianCalendar();String am_pm;int hour = calendar.get(Calendar.HOUR);int minute = calendar.get(Calendar.MINUTE);int second = calendar.get(Calendar.SECOND);if(calendar.get(Calendar.AM_PM)==0)
      am_pm ="AM";else
      am_pm ="PM";String CT = hour+":"+ minute +":"+ second +" "+ am_pm;out.println("当前时间为: "+ CT +"\n");
%>

</body></html>
把以上代码保存在main.jsp文件中，访问它。它会每隔5秒钟刷新一次页面并获取系统当前时间。运行结果如下：
自动刷新实当前时间为:6:5:36 PM
您也可以自己动手写个更复杂点的程序。





