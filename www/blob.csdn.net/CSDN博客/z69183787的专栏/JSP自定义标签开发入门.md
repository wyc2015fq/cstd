# JSP自定义标签开发入门 - z69183787的专栏 - CSDN博客
2016年04月28日 19:36:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：423
个人分类：[Jsp-Tag标签](https://blog.csdn.net/z69183787/article/category/2175879)

一般情况下开发jsp自定义标签需要引用以下两个包
**import** javax.servlet.jsp.*;
**import** javax.servlet.jsp.tagext.*;
首先我们需要大致了解开发自定义标签所涉及到的接口与类的层次结构(其中SimpleTag接口与SimpleTagSupport类是JSP2.0中新引入的)。
![](http://pic002.cnblogs.com/images/2011/267998/2011122517261812.png)
# 目标1:自定义一个用表格显示用户信息的简单标签
效果图：
![](http://pic002.cnblogs.com/images/2011/267998/2011122517271753.png)
在jsp页面使用此自定义标签：
假设我们有一个UserInfo的javabean，那么在JSP页面使用此标签只需调用此标签即可
<!-- 创建需要展现UserInfo的实例（用于测试数据） -->
    <%
    UserInfo user = **new** UserInfo();
    user.setUserName("Xuwei");
    user.setAge(33);
    user.setEmail("test@test.test");
    pageContext.setAttribute("userinfo", user);  
    %>
    <!-- 给标签设置user属性绑定要展现的UserInfo对象  -->
    <cc:showUserInfo user=*"*${pageScope.userinfo }*"* />
开发步骤：
简单标签的开发我们只要实现Tag接口即可，为了简单起见可以直接继承实现了此接口的TagSupport类
1 创建自定义标签类
**public class**UserInfoTag**extends** TagSupport {
**private** UserInfo user;
    @Override
**public int** doStartTag() **throws** JspException {
**try** {
            JspWriter out = **this**.pageContext.getOut();
**if**(user == **null**) {
                out.println("No UserInfo Found...");
**return***SKIP_BODY*;
            }
            out.println("<table width='500px' border='1' align='center'>");
            out.println("<tr>");
            out.println("<td width='20%'>Username:</td>");
            out.println("<td>" + user.getUserName() + "</td>");
            out.println("</tr>");
            out.println("<tr>");
            out.println("<td>Age:</td>");
            out.println("<td>" + user.getAge() + "</td>");
            out.println("</tr>");
            out.println("<tr>");
            out.println("<td>Email:</td>");
            out.println("<td>" + user.getEmail() + "</td>");
            out.println("</tr>");
            out.println("</table>");
        } **catch**(Exception e) {
**throw new** JspException(e.getMessage());
        }
**return***SKIP_BODY*;
    }
    @Override
**public int** doEndTag() **throws** JspException {
**return***EVAL_PAGE*;
    }
    @Override
**public void** release() {
**super**.release();
**this**.user = **null**;
    }
    //getter and setters
**public** UserInfo getUser() {
**return** user;
    }
**public void** setUser(UserInfo user) {
**this**.user = user;
    }
}
2 在Web-Inf创建标签库描述文件.tdl(Tag Library Description)
<?xml version=*"1.0"* encoding=*"UTF-8"*?>
<taglib version=*"2.0"* xmlns=*"http://java.sun.com/xml/ns/j2ee"*
 xmlns:xsi=*"http://www.w3.org/2001/XMLSchema-instance"*
 xsi:schemaLocation=*"http://java.sun.com/xml/ns/j2ee web-jsptaglibrary_2_0.xsd"*>
<tlib-version>1.0</tlib-version>
<jsp-version>2.0</jsp-version>
<short-name>cc</short-name>
<uri>/mytaglib</uri>
<tag>
    <name>showUserInfo</name>
    <tag-class>com.mytags.UserInfoTag</tag-class>
    <body-content>empty</body-content>
    <attribute>
        <name>user</name>
         <required>false</required>
         <rtexprvalue>true</rtexprvalue>
    </attribute>
 </tag>
</taglib>
3 配置web.xml
<jsp-config>
    <taglib>
        <taglib-uri>/mytaglib</taglib-uri>
        <taglib-location>/WEB-INF/mytaglib.tld</taglib-location>
    </taglib>
  </jsp-config>
4 在需要使用此标签的jsp页面头部引入
<%@ taglib uri="/mytaglib" prefix="cc"%>
5 使用（参照上面的使用步骤）
此致，一个简单的JSP标签开发完成
标签类说明：
![](http://pic002.cnblogs.com/images/2011/267998/2011122517285564.png)
我们创建的UserInfoTag类继承了TagSupport类，而它又实现了Tag接口，Tag接口的生命周期由其所在的容器控制，如下图：
setPageContext() 将所在jsp页面的pageContext注入进来，目的是为了在后面的方法中可以访问到jsp页面对象的pageContext属性
setParent()       设置此标签的父标签
setAttribute()   将标签中的属性注入到此class的属性，不需要自己实现但要提供属性的get与set方法
doStartTag()      在开始标签属性设置后调用，如果返回SKIP_BODY则忽略标签之中的内容，如果返回EVAL_BODY_INCLUDE则将标签体的内容进行输出
doEndTag()         在结束标签之前调用，返回SKIP_PAGE跳过整个jsp页面后面的输出，返回EVAL_PAGE执行页面余下部分
release()          生命周期结束时调用
特别说明：在tomcat4.1之后的版本中默认开启了标签缓冲池(websphere和weblogic并不会这么做)，所以执行完标签后并不会执行release()方法（_jspDestroy()时才释放），也就是说同一个jsp页面自定义标签不管使用多少次只会存在一个实例，但也并不是每一个标签都会为其创建一个缓冲池，要根据参数来判断，例如：
<cc:UserInfoTag user=”…” />
<cc:UserInfoTag />
上面例子中由于参数不同就会创建两个标签缓冲池。
这个问题可以通过设定tomcat的配置文件加以解决：
在%tomcat%\conf\web.xml加入enablePooling参数，并设置为false（不缓存自定义标签）。
<init-param>
  <param-name>enablePooling</param-name>
  <param-value>false</param-value>
</init-param>
清空%tomcat%\conf\目录
-------------------------------------------------------------------------------------------------------------------------------
TagSupport类已经为我们实现并扩展了一些方法(比如在上述方法中我们可以直接使用pageContext对象，调用父标签getParent()等)，所以一般情况下我们只需重写doStartTag()，doEndTag() 即可
TLD文件说明：
<!--版本号-->
<tlib-version>1.0</tlib-version>
<jsp-version>2.0</jsp-version>
<short-name>cc</short-name>
<tag>
<!—指定标签名 -->
    <name>showUserInfo</name>
<!—指定标签类文件的全路径 -->
    <tag-class>com.mytags.UserInfoTag</tag-class>
<!--如果不需要标签体则设置empty，反之设定jsp -->
    <body-content>empty</body-content>
<!—设定属性(如果有的话) -->
    <attribute>
<!—指定标签名 -->
       <name>user</name>
<!—是否是必须，如果非必须没设置则为空 -->
        <required>false</required>
<rtexprvalue>true</rtexprvalue><!—是否可在属性中使用表达式 -->
    </attribute>
</tag>
Web.xml文件说明：
<jsp-config>
    <taglib>
<!--
标签库的uri路径
即jsp头文件中声明<%@ taglib uri="/mytaglib" prefix="cc"%>
的uri
 -->
        <taglib-uri>/mytaglib</taglib-uri>
<!—tld文件所在的位置-->
        <taglib-location>/WEB-INF/mytaglib.tld</taglib-location>
    </taglib>
  </jsp-config>
# 目标2:自定义一个类似于Asp.Net中的Reapter控件的标签
效果图：
![](http://pic002.cnblogs.com/images/2011/267998/2011122517302391.png)
在jsp页面使用此自定义标签：
<!-- 创建需要展现javabean（UserInfo）集合的实例（用于测试数据） -->
<%
    List<UserInfo> users = **new** ArrayList<UserInfo>();   
    users.add(**new** UserInfo("Zhangsan", 12, "Zhangsan@163.com"));
    users.add(**new** UserInfo("Lisi", 22, "Lisi@sina.com"));
    users.add(**new** UserInfo("Wangwu", 33, "Wangwu@qq.com"));
    pageContext.setAttribute("users", users);
%>
<!-- 给标签绑定数据源  -->
<table width=*'500px'* border=*'1'* align=*'center'*>
    <tr>
        <td width=*'20%'*>UserName</td>
        <td width=*'20%'*>Age</td>
        <td>Email</td>
    </tr>
    <cc:repeater var=*"item"* items=*"*${pageScope.users }*"*>
        <tr>
            <td>${item.userName }</td>
            <td>${item.age }</td>
            <td>${item.email }</td>
        </tr>
    </cc:repeater>
</table>
开发步骤：
要完成此控件我们需要实现一个迭代接口，即IterationTag，由于TagSupport同样实现了此接口，所以我们继承此类
1 创建自定义标签类
**public class**Repeater**extends** TagSupport {
**private**Collection items;
**private**Iterator it;
**private** String var;
    @Override
**public int** doStartTag() **throws** JspException {
**if**(items == **null** || items.size() == 0) **return***SKIP_BODY*;
        it = items.iterator();  
**        if**(it.hasNext()) {
            pageContext.setAttribute(var, it.next());
        }
**return***EVAL_BODY_INCLUDE*;
    }
    @Override
**public int** doAfterBody() **throws** JspException {
**if**(it.hasNext()) {
            pageContext.setAttribute(var, it.next());
**return***EVAL_BODY_AGAIN*;
        }
**return***SKIP_BODY*;
    }
    @Override
**public int** doEndTag() **throws** JspException {
**return***EVAL_PAGE*;
    }
**public void** setItems(Collection items) {
**this**.items = items;
    }
**public void** setVar(String var) {
**this**.var = var;
    }
}
2在Web-Inf创建标签库描述文件.tdl(Tag Library Description)
由于目标1种已经创建了此文件，我们只需增加此标签的配置即可
<tag>
    <name>repeater</name>
    <tag-class>com.mytags.Repeater</tag-class>
    <body-content>jsp</body-content>
    <attribute>
        <name>items</name>
        <required>false</required>
        <rtexprvalue>true</rtexprvalue>
    </attribute>
    <attribute>
        <name>var</name>
        <required>true</required>
        <rtexprvalue>true</rtexprvalue>
    </attribute>
 </tag>
3 配置web.xml （目标1中已完成，无需修改）
4 在需要使用此标签的jsp页面头部引入
<%@ taglib uri="/mytaglib" prefix="cc"%>
5 使用（参照上面的使用步骤）
标签类说明：
我们用到了迭代接口，以下是容器处理此接口的流程
![](http://pic002.cnblogs.com/images/2011/267998/2011122517310239.png)
作为目标1中的补充： 在doAfterBody()如果返回值是EVAL_BODY_AGAIN那么将重新执行此方法
# 目标3:使用BodyTagSupport
此目标并不会使用实际例子进行显示，主要是说明为什么，什么情况下需要使用到BodyTag接口或者BodyTagSupport类？
如果我们需要在<test>  ….   </test>之间的标签体的头部和尾部加上一些标记或者是其他处理，一般的处理方法是在doStartTag和doEndTag方法中进行， 但是如果是个迭代标签，标签体的每段内容在循环输出时每次都需要在头部和尾部加上一些标记，我们使用BodyTagSupport就很方便了，
此接口在doStartTag()方法返回值多了一个EVAL_BODY_BUFFERED,它将对主体进行计算，并输出到缓冲区(注：此处是缓冲区并非直接输出到客户端，需要我们手动（this.bodyContent.getEnclosingWriter().write(this.bodyContent.getString());）进行输出客户端的调用，否则主体内容不会进行显示)
标签类说明：
关于BodyTagSupport接口的说明
![](http://pic002.cnblogs.com/images/2011/267998/2011122517314844.png)
# 目标4:自定义的函数库
1 创建函数库类
**public class** MyFunctions {
**public static** String formatMyName(String name) {
**return** "your name is " + name;
    }
**public static int** add(**int** a, **int** b) {
**return** a+b;
    }
}
2 在TLD文件中配置 (引用于目标1中的tld文件)
<function>
    <name>formatMyName</name>
    <function-class>com.taglib.MyFunctions</function-class>
    <function-signature>java.lang.String formatMyName(java.lang.String)</function-signature>
 </function>
<function>
    <name>add</name>
    <function-class>com.taglib.MyFunctions</function-class>
    <function-signature>java.lang.String add(int, int)</function-signature>
 </function>
3 JSP中调用
${cc:formatMyName("wangfei") }
${cc:add(12, 34) }
