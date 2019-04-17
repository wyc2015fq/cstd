# EL表达式的使用及示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年02月18日 23:30:06[boonya](https://me.csdn.net/boonya)阅读数：2165










# ``

# 1.基础介绍

# ``
`在MVC体系结构中，JSP页面只是用来显示数据，但JSP脚本中的表达式功能不够强大，它不能直接对隐式对象中某对象的属性进行显示，需要配合Scriptlet才能显示数据，很是麻烦，如果在JSP页面中使用EL表达式将大大简化JSP页面中的Java代码，在配合JSP标准动作，JSTL，可达到真正的JSP无脚本。`



`EL（Expression Language）表达式语言，语法：${表达式}，例如：${``"Beijing 2008"``}则在JSP页面输出Beijing 2008，相当于JSP脚本中的<%=`
```
"Beijing
 2008"
```
`%>，在EL表达式中的“'”和“"”功能是一样的，表示一个字符串；`



`在JSP页面中我们可以禁用EL表达式，需要配置page指令的isELIgnored属性，属性值若为``true``则支持EL表达式，若为``false``则禁用表达式，默认情况下是支持EL表达式的。`



**`在EL表达式中我们可以使用运算符以达到我们想要的结果，运算符按作用分为以下几种：`**



**`1.算术运算符`**



`+ 注意：在EL表达式中的‘+’只有数学运算的功能，没有连接符的功能，它会试着把运算符两边的操作数转换为数值类型，进而进行数学加法运算，最后把结果输出。若出现${``'a'``+``'b'`
```
}则会出现异常。
 例如：${6+6}
```


`- 例如：${4-3}`

`* 例如：${4*3}`

`/ 例如：${9/3}`

**`2.关系运算符`**



`> 例如：${8>9}`

`>= 例如：${45>=9}`

`< 例如：${4<9}`

`<= 例如：${9<=8}`

`== 例如：${4==4}`

`!= 例如：${4!=3}`

**`3.逻辑运算符`**



`&& 例如：${``false``&&``false``}`

`|| 例如：${``true``||``false``}`

`! 例如：${!``true``}相当于${``false``}`

**`4.三元运算符`**



`? : 例如：${3>2?``'是'``:``'不是'``}`

`在EL表达式中还有一些特殊运算符：`



`empty 判断EL表达式中的表达式是否为空，例如：${empty sessionScope.user}`

`. 是我们最常用的，作用相当于执行Bean中的``get``方法。 例如：${sessionScope.user.userName}意思是：在会话中得到名称为user的Bean对象，通过“.”运算符执行getUserName();方法，返回存放在Bean中的用户名属性的值。`

`[] 作用和“.”运算符的一样，只不过[]运算符可以执行一些不规则的标识符。 例如：${requestScope.user[``"score-math"`
```
]}，这个表达式中有不规则的标识符，是不能使用“.”来访问的。
```


**`在EL表达式中为我们提供了许多内置对象，使我们很方便的得到我们想要的数据：`**



**`1.从隐式对象中得到数据：`**



`pageScope 例如：${pageScope.user.userName} 相当于<%=page.getAttribute(``"user"``).getUserName%>`

`requestScope 例如：${requestScope.books} 相当于<%=request.getAttribute(``"books"``)%>`

`sessionScope 例如：${sessionScope.lastURL} 相当于<%=session.getAttribute(``"lastURL"``)%>`

`applicationScope 例如：${applicationScope.provice} 相当于<%=application.getAttribute(``"provice"``)%>`

**`2.从请求头中得到数据：`**



`header 例如：${header.referer}得到请求过来的URL`

**`3.访问JSP页面中的内置对象：`**



`pageContext 例如：${pageContext.request}在EL表达式中我们是不能直接使用JSP页面的内置对象，但可以是用pageContext间接得到这些内置对象，再来使用。`


以上内容来自：[http://www.cnblogs.com/cyjch/archive/2012/02/08/2342387.html](http://www.cnblogs.com/cyjch/archive/2012/02/08/2342387.html)

# 2.运算表达式

关系运算符 == （或 eq ）、 != （或 ne ）、 < （或 lt ）、 > （或 gt ）、 <= （或 le ）和 >= （或 ge ） 

逻辑运算符 && （或 and ）、 || （或 or ）和 ! （或 not ）




# 3.使用ModelAndView传递参数到JSP页面设置




Java代码：



```java
@RequestMapping("/sq-condition/{type}")
	public ModelAndView sqCondition(@PathVariable(value="type") int type){
		Map<String,Object> validateMap=validateSession(sessionTimeOut);
		if(validateMap.get("SESSION_VALIDATED").toString().equals("false")){
			return new ModelAndView(sessionTimeOut);
		}
		ModelAndView mv=new ModelAndView("redirect:/pda/stock/sq-condition.html");
		mv.addObject("type", type);
		return mv;
	}
```

注：根据类型控制显示不同条件。



JSP代码：



```
<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@page import="com.wlyd.fmcgwms.util.ehcache.EhcacheUtil"%>
<%@page import="com.wlyd.fmcgwms.persistence.beans.EsUser"%>
<%@page import="com.wlyd.fmcgwms.util.finals.Const"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>
<%@ page isELIgnored="false" %>
<%
    String staticUrl = (String) EhcacheUtil.get("staticUrl");
    String webRoot = request.getContextPath();
    EsUser user = (EsUser) session.getAttribute(Const.SESSION_BACK_USER);
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta charset="utf-8">
<title>智能仓库管理系统</title>
<link rel="stylesheet" href="<%=staticUrl%>/pda/css/base.css">
<link rel="stylesheet" href="<%=staticUrl %>/pda/css/style.css">


</head>

<body>
${requestScope.type}<br/>
<%=request.getAttribute("type")%>
<c:if test="${requestScope.type eq 1}">
	<div class="header">
		库存查询-2
	</div>
	<ul class="ul-table">
		<li><span class="li-th"><em>*</em>数字仓管编码：</span><div class="li-td"><input type="text"></div></li>
		<li><span class="li-th"><em>*</em>商品条码：</span><div class="li-td"><input type="text"></div></li>
	</ul>
</c:if>

<c:if test="${requestScope.type eq 2}">
	<div class="header">
		库存查询-4
	</div>
	<ul class="ul-table">
		<li><span class="li-th"><em>*</em>数字仓管编码：</span><div class="li-td"><input type="text"></div></li>
		<li><span class="li-th"><em>*</em>货位条码：</span><div class="li-td"><input type="text"></div></li>
	</ul>
</c:if>

</body>
</html>
```


效果图：



![](https://img-blog.csdn.net/20170207100434718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170207100503946?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)










