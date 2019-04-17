# Stripes视图框架的简单标签使用 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月05日 17:58:26[boonya](https://me.csdn.net/boonya)阅读数：2547








**1、导入基本使用标签库**



```
<%@ page contentType="text/html;charset=UTF-8" language="java"%>
<%@ taglib prefix="stripes" uri="http://stripes.sourceforge.net/stripes.tld"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt"%>
```



**2、遍历集合对象**



```
<table>
	<tr>
		<th>Order ID</th>
		<th>Date</th>
		<th>Total Price</th>
	</tr>

	<c:forEach var="order" items="${actionBean.orderList}">
		<tr>
			<td><stripes:link beanclass="org.mybatis.jpetstore.web.actions.OrderActionBean"	event="viewOrder">
				<stripes:param name="orderId" value="${order.orderId}" />
			    ${order.orderId}
			  </stripes:link></td>
			<td><fmt:formatDate value="${order.orderDate}"
				pattern="yyyy/MM/dd hh:mm:ss" /></td>
			<td><fmt:formatNumber value="${order.totalPrice}"
				pattern="$#,##0.00" /></td>
		</tr>
	</c:forEach>
</table>
```

标签及属性讲解：



2-1、<c:forEach>   JSTL 遍历标签 

                   var：循环遍历对象的临时名称声明；items： 后台传输的 数据项集合

2-2、<stripes:link>  超链接

                   beanclass： 对应的*ActionBean的全路径；event：表示处理的事件即@HandlerEvent("viewName")

2-3、<stripes:param>页面参数设置，相当于隐藏的input标签

                  name：*ActionBean对应的属性名称；value：为属性设置的值

2-4、<fmt:formatDate> JSTL格式化标签： 时间

                   value：对象的时间值；pattern:匹配表达式（格式）

2-5、<fmt:formatNumber> JSTL格式化标签：数字

                 同1-4.

**3、Form表单的使用**



```
<div id="Catalog"><stripes:form
	beanclass="org.mybatis.jpetstore.web.actions.OrderActionBean">

	<table>
		<tr>
			<th colspan=2>Shipping Address</th>
		</tr>

		<tr>
			<td>First name:</td>
			<td><stripes:text name="order.shipToFirstName" /></td>
		</tr>
		<tr>
			<td>Last name:</td>
			<td><stripes:text name="order.shipToLastName" /></td>
		</tr>
		<tr>
			<td>Address 1:</td>
			<td><stripes:text size="40" name="order.shipAddress1" /></td>
		</tr>
		<tr>
			<td>Address 2:</td>
			<td><stripes:text size="40" name="order.shipAddress2" /></td>
		</tr>
		<tr>
			<td>City:</td>
			<td><stripes:text name="order.shipCity" /></td>
		</tr>
		<tr>
			<td>State:</td>
			<td><stripes:text size="4" name="order.shipState" /></td>
		</tr>
		<tr>
			<td>Zip:</td>
			<td><stripes:text size="10" name="order.shipZip" /></td>
		</tr>
		<tr>
			<td>Country:</td>
			<td><stripes:text size="15" name="order.shipCountry" /></td>
		</tr>


	</table>

	<stripes:submit name="newOrder" value="Continue" />

</stripes:form></div>
```

标签及属性讲解：



1、<stripes:form> 表单

                                 beanclass ：对应的*ActionBean的全路径；

2、<stripes:text>  文本输入框

                                name：*ActionBean下对应的实体的属性

3、<stripes:submit>  表单提交标签

                               name：对应的*ActionBean处理的事件 ；value：按钮显示的文字

**4、获取*Action对象及其属性值**

假设有一个HelloActionBean 它里面有以下属性：

（1）List<Order> orders;//订单集合      获取集合数据对象：${actionBean.orders};

（2）Order order;//订单对象                    获取Order对象的属性：$(actionBean.order.attribute);

注：actionBean是所有设置的beanclass的基本访问对象。



