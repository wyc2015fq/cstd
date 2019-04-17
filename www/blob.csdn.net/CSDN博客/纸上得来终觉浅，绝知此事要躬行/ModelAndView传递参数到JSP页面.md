# ModelAndView传递参数到JSP页面 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月07日 10:06:11[boonya](https://me.csdn.net/boonya)阅读数：11860








EL表达式使用请参考：[http://blog.csdn.net/boonya/article/details/50689295](http://blog.csdn.net/boonya/article/details/50689295)

JSTL标签使用请参考：[http://blog.csdn.net/boonya/article/details/52163664](http://blog.csdn.net/boonya/article/details/52163664)


# 1.Java代码

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

# 2.JSP代码

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


# 3.效果图

![](https://img-blog.csdn.net/20170207100434718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170207100503946?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







