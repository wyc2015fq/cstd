# JSTL：c:forEach使用 - 零度的博客专栏 - CSDN博客
2015年11月12日 10:06:16[零度anngle](https://me.csdn.net/zmx729618)阅读数：430标签：[jstl](https://so.csdn.net/so/search/s.do?q=jstl&t=blog)
个人分类：[Java](https://blog.csdn.net/zmx729618/article/category/3108443)
<c:forEach>标签用于通用数据循环，它有以下属性
|属 性|描 述|是否必须|缺省值|
|----|----|----|----|
|items|进行循环的项目|否|无|
|begin|开始条件|否|0|
|end|结束条件|否|集合中的最后一个项目|
|step|步长|否|1|
|var|代表当前项目的变量名|否|无|
|varStatus|显示循环状态的变量|否|无|
**vaStatus 属性**
JSTL中的varStatus和 var 属性一样，
varStatus 用于创建限定了作用域的变量。不过，由 varStatus 属性命名的变量并不存储当前索引值或当前元素，而是赋予 javax.servlet.jsp.jstl.core.LoopTagStatus 类的实例。该类定义了一组特性，它们描述了迭代的当前状态，下面列出了这些特性:
特性          Getter                   描述current    getCurrent()           当前这次迭代的（集合中的）项
index       getIndex()               当前这次迭代从 0 开始的迭代索引
count       getCount()               当前这次迭代从 1 开始的迭代计数
first       isFirst()                用来表明当前这轮迭代是否为第一次迭代的标志
last        isLast()                 用来表明当前这轮迭代是否为最后一次迭代的标志
begin       getBegin()               begin 属性值
end         getEnd()                 end 属性值
step        getStep()                step 属性值
用c:forEach 中vaStatus的属性可以获得迭代的自身状态，如：
<c:forEach var=”" items=”" varStatus=”status”>
<c:out value=”${status.current}”/> 当前对象
<c:out value=”${status.index}”/> 此次迭代的索引
<c:out value=”${status.count}”/> 已经迭代的数量
<c:out value=”${status.first}”/> 是否是第一个迭代对象
<c:out value=”${status.last}”/> 是否是最后一个迭代对象
</c:forEach>
<c:forEach>标签的items属性支持Java平台所提供的所有标准集合类型。此外，您可以使用该操作来迭代数组（包括基本类型数组）中的元素。它所支持的集合类型以及迭代的元素如下所示：
java.util.Collection：调用iterator()来获得的元素。
java.util.Map：通过java.util.Map.Entry所获得的实例。
java.util.Iterator：迭代器元素。
java.util.Enumeration：枚举元素。
Object实例数组：数组元素。
基本类型值数组：经过包装的数组元素。
用逗号定界的String：分割后的子字符串。
javax.servlet.jsp.jstl.sql.Result：SQL查询所获得的行。
<%
   response.setCharacterEncoding("GB2312");
   ArrayList student = (ArrayList)request.getAttribute("list");
   int size = student.size();
   for(int i = 0;i<size;i++)
   {
    Student stu = (Student)student.get(i);
    out.println("SID="+stu.getSid());
    out.println("SName="+stu.getSname());
    out.println("Score="+stu.getScore());
   } 
%>
将以上代码片段用JSTL改写：
<%@ page language="java" import="java.util.*" pageEncoding="GB2312"%>
<%@ taglib uri="[http://java.sun.com/jsp/jstl/core](http://java.sun.com/jsp/jstl/core)" prefix="c"%>
<html>
<body>
   <table>
    <c:forEach var="stu" items="${list}">
     <tr>
      <td>
       SID=${stu.sid}
      </td>
      <td>
       SName=${stu.sname}
      </td>
      <td>
       Score=${stu.score}
      </td>
     </tr>
    </c:forEach>
   </table>
</body>
</html>
