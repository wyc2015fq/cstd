# Struts2学习笔记之防止表单重复提交 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2012年09月06日 22:27:33[boonya](https://me.csdn.net/boonya)阅读数：3722标签：[struts																[action																[jsp																[class																[user](https://so.csdn.net/so/search/s.do?q=user&t=blog)
个人分类：[Struts2](https://blog.csdn.net/boonya/article/category/879253)





Struts2学习笔记

第十三记：Struts2标签<s:token/>防止表单重复提交

**1、使用struts2的标签编写JSP页面**
导入标签库：<%@ taglib uri="struts-tags" prefix="s" %>
add.jsp示例：
<s:from action="add" namespace="/" method="post">
<s:textfiled name="username"/>
<s:token/>
....
<s:submit value="提交"/>
</from>
**2、配置提交的拦截器（系统自带）**
<action name="user" class="com.struts.action.UserAction">
<interceptor-ref name="defaultStack">
<interceptor-ref name="token">
<result name="invalid.token">/WEB-INF/page/add.jsp</result><!-- 重复提交时返回输入页面-->
<result name="success">/WEB-INF/page/message.jsp</result>
</action>](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=jsp&t=blog)](https://so.csdn.net/so/search/s.do?q=action&t=blog)](https://so.csdn.net/so/search/s.do?q=struts&t=blog)




