# ModelAndView传递参数到JSP页面 - xqhrs232的专栏 - CSDN博客
2018年10月26日 15:47:54[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：825
原文地址::[https://blog.csdn.net/boonya/article/details/54906699](https://blog.csdn.net/boonya/article/details/54906699)
相关文章
1、[Spring MVC 向前台页面传值-ModelAndView](https://www.cnblogs.com/caoyc/p/5635782.html)----[https://www.cnblogs.com/caoyc/p/5635782.html](https://www.cnblogs.com/caoyc/p/5635782.html)
2、Spring MVC ModelAndView详解----[https://blog.csdn.net/qq_32671287/article/details/70212800?utm_source=blogxgwz3](https://blog.csdn.net/qq_32671287/article/details/70212800?utm_source=blogxgwz3)
3、SpringMVC通过ModelAndView向JSP页面传值----[https://blog.csdn.net/LanSeTianKong12/article/details/51769397?utm_source=blogxgwz4](https://blog.csdn.net/LanSeTianKong12/article/details/51769397?utm_source=blogxgwz4)
4、如何在页面中获取到ModelAndView绑定的值----[https://blog.csdn.net/yhj19920417/article/details/72824406?utm_source=blogxgwz5](https://blog.csdn.net/yhj19920417/article/details/72824406?utm_source=blogxgwz5)
5、SpringMVC在JSP页面取不到ModelAndView中的值----[https://blog.csdn.net/w893932747/article/details/79011487](https://blog.csdn.net/w893932747/article/details/79011487)
EL表达式使用请参考：http://blog.csdn.net/boonya/article/details/50689295
JSTL标签使用请参考：http://blog.csdn.net/boonya/article/details/52163664
1.Java代码
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
注：根据类型控制显示不同条件。
2.JSP代码
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
