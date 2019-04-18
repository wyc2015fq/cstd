# struts2 跳转类型 result type=chain、dispatcher、redirect(redirect-action) - z69183787的专栏 - CSDN博客
2013年04月02日 14:01:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：807
个人分类：[Struts2](https://blog.csdn.net/z69183787/article/category/2175433)
**dispatcher 为默认跳转类型，用于返回一个视图资源(如:jsp) **Xml代码 :
<result name="success">/main.jsp</result> 
<result name="success">/main.jsp</result> 
以上写法使用了两个默认，其完整的写法为： 
<result name="success" type="dispatcher"> 
        <param name="location">/maini.jsp</param> 
</result>
用于页面转发，页面跳转过程一直是同一个线程，Action中的数据一直保存在。
location只能是页面，不能是另一个action（可用type="chain"解决）。
**redirect 类型用于重定向到一个页面，另一个action或一个网址。**
Xml代码:
<result name="success" type="redirect">aaa.jsp</result> 
<result name="success" type="redirect">bbb.action</result> 
<result name="success" type="redirect">www.baidu.com</result>
缺点：redirect把一个http返回码（SUCCESS）以及返回的页面位置一起重新发给web服务器，容纳后由web服务器产生一个新的HTTP请求，就会产生一个新的线程，保存在原来Action执行的线程中的数据就无法访问。
所以，result需要包含Action的数据，那么redirect不是一个可行的办法。因为新的HTTP请求时在Servlet容器的新的线程中处理的，ActionContext中的所有状态都不会存在。
处理方法：
（方法一）： 
<result name="topic" type="redirect">/topicAction!findTopics.do?topicId=${topicId}</result> 
（方法二）： 
<result name="topic" type="redirect-action"> 
<param name="actionName">findTopics</param> 
<param name="topicId">${topicId}</param> 
</result>
**redirect-action 结果类型使用ActionMapperFactory提供的ActionMapper来重定向请求到另外一个action **Xml代码:
<result name="err" type="redirect-action"> 
    <param name="actionName">重定向的Action名</param> 
     <param name="namespace">重定向Action所在的名字空间</param> 
</result> 
redirect和redirect-action两种结果类型在使用上其实并没有什么区别，只是写法不同而已。
**chain 用于把相关的几个action连接起来，共同完成一个功能。 **Xml代码:
<action name="step1" class="test.Step1Action"> 
     <result name="success" type="chain">step2.action</result> 
</action> 
<action name="step2" class="test.Step2Action"> 
<result name="success">finish.jsp</result> 
</action> 
处于chain中的action属于同一个http请求，共享一个ActionContext 
**plaintextj 结果类型用于直接在页面上显示源代码**
Xml代码:
<result name="err" type="plaintext"> 
    <param name="location">具体的位置</param> 
    <param name="charSet">字符规范(如GBK)</param> 
</result>
