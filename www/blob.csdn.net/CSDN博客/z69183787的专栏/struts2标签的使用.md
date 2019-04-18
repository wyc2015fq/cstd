# struts2标签的使用 - z69183787的专栏 - CSDN博客
2014年03月05日 12:49:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：14874

Data标签方便在页面进行格式化的日期输出。格式有多种可供选择。同时，还可以通过在properties属性文件中定义好”struts.date.format”参数的值，从而自定义格式输出。
Date标签包含三个属性，可以从下面的代码中感受一下，分别是
1、Name:
2、Nice:  指定是否输出指定日期与当前时刻之间的时差。
   可以设为true或者false，表示是否漂亮地显示日期，如果设置为true，那么将FORMAT属性将不会生效
实例：[http://blog.csdn.net/z69183787/article/details/20538473](http://blog.csdn.net/z69183787/article/details/20538473)
3、Format
   (1)当nice="false"时，format属性将起作用，如："yyyy- MM-dd hh:mm:ss"，其中，y是年(year)，M是月(Month)，d是日(day)，h是小时(hour，12小时制)，H也是小  时(hour，24小时制)，m是分钟(minute)，s是秒(second)。
   (2)当format未设置时，将会使用默认的格式DateFormat.MEDIUM format
1.dateTag.jsp
<%@ page contentType="text/html; charset=GBK"%>
<%@ taglib prefix="s" uri="/struts-tags"%>
<html>
 <head>
  <title>Date Tag 示例</title>
 </head> 
 <body>
  <h2>显示当前的时间</h2>
  <table border="1" width="35%">
   <tr>
    <td><b>日期格式</b></td>
    <td><b>日期</b></td>
   </tr>  
   <tr>
    <td>Day/Month/Year</td>
    <td><s:date name="currentDate" format="dd/MM/yyyy" /></td>
   </tr>
   <tr>  
    <td>Month/Day/Year</td>
    <td><s:date name="currentDate" format="MM/dd/yyyy" /></td>
   </tr>  
   <tr>
    <td>Month/Day/Year</td>
    <td><s:date name="currentDate" format="MM/dd/yy" /></td>
   </tr>  
   <tr>
    <td>Month/Day/Year Hour<B>:</B>Minute</td>
    <td><s:date name="currentDate" format="MM/dd/yy hh:mm" /></td>
   </tr>  
   <tr>
    <td>Month/Day/Year Hour<B>:</B>Minute<B>:</B>Second</td>
    <td><s:date name="currentDate" format="MM/dd/yy hh:mm:ss" /></td>
   </tr>  
   <tr>
    <td>Nice Date (Current Date & Time)</td>
    <td><s:date name="currentDate" nice="false" /></td>
   </tr>  
   <tr>
    <td>Nice Date</td>
    <td><s:date name="currentDate" nice="true" /></td>
   </tr>
  </table>
 </body>
</html>
2.DateTag.java
public class DateTag extends ActionSupport {
 private Date currentDate;
 public Date getCurrentDate() {
  return currentDate;
 }
 public void setCurrentDate(Date currentDate) {
  this.currentDate = currentDate;
 }
 public String execute(){
  currentDate = new Date();
  return SUCCESS;
 }
}
3.struts.xml
<action name="dateTag" class = "test1.DateTag">
 <result name="success">/test1/dateTag.jsp</result>
</action>
