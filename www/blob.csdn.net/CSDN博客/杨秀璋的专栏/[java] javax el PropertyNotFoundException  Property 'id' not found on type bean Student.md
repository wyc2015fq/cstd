
# [java] javax.el.PropertyNotFoundException: Property 'id' not found on type bean.Student - 杨秀璋的专栏 - CSDN博客

2015年05月19日 03:45:28[Eastmount](https://me.csdn.net/Eastmount)阅读数：39456


**问题提出：**
在使用MyEclipse开发Java Web时，调用[DAO和Java Bean](http://blog.csdn.net/eastmount/article/details/45833663)出现了如下错误：
严重: Servlet.service() for servlet [jsp] in context with path [/JDBCbyDao] threw exception [An exception occurred processing JSP page /student.jsp at line 37
34:
35:<c:forEach items="${ studentList }" var="student">
36:<tr bgcolor="\#FFFFFF">
37:<td><input type="checkbox" name="id" value="${ student.id }" /></td>
38:<td>${ student.id }</td>
39:<td>${ student.name }</td>
40:<td>${ student.password }</td>
Stacktrace:] with root cause
javax.el.PropertyNotFoundException: Property 'id' not found on type bean.Student
at javax.el.BeanELResolver$BeanProperties.get(BeanELResolver.java:290)
at javax.el.BeanELResolver$BeanProperties.access$300(BeanELResolver.java:243)
![](https://img-blog.csdn.net/20150519022425461)
![](https://img-blog.csdn.net/20150519030701794)
其中我的类中已经定义了属性和get/set方法，如下：
```python
package bean;
public class Student {
	
	private Integer id;       //学号
	private String name;      //姓名
	private String password;  //密码
	
	public Integer getID() { return id; }
	public String getName() { return name; }
	public String getPassword() { return password; }
	public void setID(Integer id) { this.id =  id; }
	public void setName(String name) { this.name =  name; }
	public void setPassword(String pwd) { this.password = pwd; }
	
}
```
而Jsp中的调用代码是通过EL实现，也导入了相应的包。如下：
```python
<%@ page language="java" pageEncoding="UTF-8"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<jsp:directive.page import="DAO.StudentDAO"/>
<jsp:directive.page import="java.util.List"/>
<%
	List studentList = StudentDAO.listStudents();
	request.setAttribute("studentList", studentList);
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <title>My JSP 'student.jsp' starting page</title>
  </head>
  <body>
    <form action="operateStudent.jsp" method=get>
			<table bgcolor="#CCCCCC" cellspacing=1 cellpadding=5 width=100%>
				<tr bgcolor=#DDDDDD>
					<th>选择</th>
					<th>学号</th>
					<th>姓名</th>
					<th>密码</th>
					<th>操作</th>
				</tr>
				
				<c:forEach items="${studentList}" var="stu">
					<tr bgcolor="#FFFFFF">
						<td><input type="checkbox" name="id" value="${stu.id}" /></td>
						<td>${stu.id}</td>
						<td>${stu.name}</td>
						<td>${stu.password}</td>
						<td>
							<a href="addEmployee.jsp?action=edit&id=${stu.id}">修改</a>
							<a href="addEmployee.jsp?action=del&id=${stu.id}" 
								onclick="return confirm('确定删除?')">删除</a>
						</td>
					</tr>
				</c:forEach>
			</table>
		</form>
  </body>
</html>
```

**解决方案：**
1.可能你遇到的错误是“Property 'id' not found on type java.lang.String”异常
它的意思是String类中没有id这个属性，而修改的方法就是：
<c:forEach items="videos" var="video" >
修改成：
<c:forEach items="${videos}" var="video" >
但是你需要注意它的错误是：java.lang.String对比type bean.Student，其中对应src/bean.Student.java文件。而且我在JSP中已经是${studentList}这种变量了，所以该方法不是该错误的解决方案。
2.有人说是bean的属性名称错误，或者没有get,set方法，但是我的bean如下方法。又参考错误“javax.el.PropertyNotFoundException: Property 'pNum' not found on type com.manager.Paper”，此时的解决方案是：
private int pNum;
private int pSize;
建议你将这两个属性的名称换下
private int pnum;
private int psize;
据说是应为命名规范，同时stu.EmpNo估计是大小写错了，换成 ${stu.empNo} 就能成功，因为EL是读取属性的getter方法的，一般按照属性首字母小写来处理。但是我的名字是id，因此该方法也是行不通的。
3.如果上面两个方法你仍然报错，下面是我自己总结的方法：
Servlet.service() for servlet [jsp] in context with path
javax.el.PropertyNotFoundException: Property 'id' not found on type bean.Student
你需要做到的是：
(1).首先确保循环<c:forEach items="${studentList}" var="stu">，然后调用是${stu.id}、${stu.name}；
(2).然后属性命名最好是小写的，当然首字母一定要小写，如empNo；
(3).在数据库中create table student( stuid int,username varchar(20)
 )对应的Student类变量private Integer id; private String name;其中类型需要一致，同时设置get和set方法：
```python
private Integer id;       //学号
private String name;      //姓名
public Integer getID() { return id; }
public String getName() { return name; }
public void setId(Integer id) { this.id =  id; }
public void setName(String name) { this.name =  name; }
```
(4).在DAO中数据库增删改查操作中类型要一致，并且对应数据库中的学号stuid和姓名username:
//删除操作
public static int delete(Integer id) throws Exception {
String sql = "DELETE FROM student WHEREstuid= ? ";
return JDBCConnect.executeUpdate(sql, id);
}
//查询操作
student.setId(rs.getInt("stuid"));
student.setName(rs.getString("username"));
(5).如果上面的数据库、Java类变量类型都是一致的，使用方法都正确仍然存在该错误，那可能就是下面的错误：
当我定义函数public IntegergetID(){ return id; }时就会报错

# HTTP Status 500 - javax.el.PropertyNotFoundException: Property 'id' not readable on type bean.Student
而当我修改为public IntegergetId(){ return id; }后运行结果如下图所示：
![](https://img-blog.csdn.net/20150519032006800)
同样setId()方法也修改，同时DAO中调用setId()和getId()方法也修改“D=>d”。所以我还是怀疑是使用EL时的命名规范在作怪。而且我通过代码验证修改成小写d可以成功。而第一个错误Servlet.service() for
 servlet需要看它后面抛出的异常，即第二个错误。
另一种猜测：在jstl的el表达式引用错误应该使用${info.type.id } 而不是${info.id }(未验证)
参考资料：
[1.JSP not finding property in bean - stackoverflow 丢失set方法](http://stackoverflow.com/questions/6192476/jsp-not-finding-property-in-bean)
[2.javax.el.PropertyNotFoundException: Property 'answer' not - stackoverflow](http://stackoverflow.com/questions/8577545/javax-el-propertynotfoundexception-property-answer-not-found-on-type-com-pool)
[3.异常：javax.el.PropertyNotFoundException: Property 'id' not found on - CSDN](http://blog.csdn.net/xue_feitian/article/details/5988492)
[4.javax.el.PropertyNotFoundException: Property 'Owner' not found on - 百度知道](http://zhidao.baidu.com/link?url=SOIyLixCeSftHdnAed8UiafMqPLhfSJiVDbKcJdY3xUPpqt0Y7eI2yG9dD8jFJrRNprf49Ppg_d3aYHIeqIgqa)
[5.JSP没有使<c:forEach items="${specialty}" var="spe"> - CSDN论坛](http://bbs.csdn.net/topics/390222943)
该错误报告和在线笔记希望对你有所帮助~
（By:Eastmount 2015-5-19 凌晨4点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）


