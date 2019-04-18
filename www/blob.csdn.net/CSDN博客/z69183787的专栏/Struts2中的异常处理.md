# Struts2中的异常处理 - z69183787的专栏 - CSDN博客
2015年03月02日 11:23:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：685

**因为在Action的execute方法声明时就抛出了Exception异常，所以我们无需再execute方法中捕捉异常，仅需在struts.xml 中配置异常处理。**
为了使用Struts2的异常处理机制，必须打开Struts2的异常映射功能，这需要**exception拦截器**。在struts-default.xml文件中已经开启了exception拦截器。
**声明式异常捕捉**
Struts2的异常处理机制是通过在struts.xml文件中配置<exception-mapping……/>元素完成的，配置该元素时，需要指定两个属性：
**exception**：指定该异常类型的完全限定名。
**result**：指定逻辑视图名。
根据<exception-mapping…../>元素出现位置的不同，异常映射又可分为两种：
**局部异常映射**：将<exception-mapping… />元素作为<action…/>元素的子元素配置；
**全局异常映射**：将<exception-mapping… />元素作为<global-exception-mappings… />元素的子元素配置；
全局异常映射对所有的Action都有效，但局部异常映射仅对该异常映射所在的Action有效。
如果局部异常映射和全局异常映射配置了同一个异常类型，在<action…./>元素内的**局部异常映射将覆盖全局异常映射**。
**Struts.xml**
  <package name="ssh2" extends="struts-default">
**<global-results>           <result name="sql">/exception.jsp</result>           <result name="root">/exception.jsp</result>     </global-results>**
**     <global-exception-mappings>           <exception-mapping exception="java.sql.SQLException" result="sql"/>           <exception-mapping exception="java.lang.Exception" result="root"/>     </global-exception-mappings>**
     <action name="login" class="loginAction">
         <result>/welcome.jsp</result>
**<result name="nullPointer">/nullPointer.jsp</result>         <exception-mapping exception="java.lang.NullPointerException" result="nullPointer"/>**      </action>
  </package>
**Action**
   public class loginAction extends ActionSupport
   {
        public String add() throws SQLException
        {
             return "toadd";
        }
   }
**有异常往外抛即可。你也可以在方法里面抛，比如throw SQLException。**
我们可以使用Struts2的标签输出异常信息：
输出异常的message属性信息:<s:property value="exception.message" />
输出异常堆栈信息:<s:property value="exceptionStack" />。
有了处理系统异常的基础，我们来看一看**自定义异常**：
package com.exception ;
public class MyException extends Exception 
{
    private String message;
    public MyException(String message)
    {
           super(message);
           this.message = message ;
    }
    public String getMessage() {
          return message;
    }
    public void setMessage(String message) {
           this.message = message;
    }
}
public String execute() throws Exception
{
    if(!"hello".equals(usename) || !"world".equals(password))
    {
** throw new MyException("用户名或密码错误，您发现了吧！");**    }
    return "success" ;
}
**在action配置中的异常处理**
<struts>
   <package name="struts2" extends="struts-default">       
      <action name="login" class="com.struts2.LoginAction">
**<exception-mapping result="myex" ****exception="com.exception.MyException">                      </exception-mapping>            <result name="myex">/error.jsp</result>**            <result name="success">/result.jsp</result>
      </action>
   </package> 
</struts>
**在全局配置中的异常处理**
<struts>
   <package name="struts2" extends="struts-default">
**<global-results>          <result name="myexception1">/error.jsp</result>      </global-results>      <global-exception-mappings>          <exception-mapping result="myexception1"               exception="com.exception.MyException">                     </exception-mapping>      </global-exception-mappings>**
      <action name="login" class="com.struts2.LoginAction">
            <result name="success">/result.jsp</result>
      </action>
   </package>
</struts>
**错误页面error.jsp**
<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ taglib prefix="s"  uri="/struts-tags"%>  
<html>
  <body>
    <!-- 这个exception 是 exception="com.exception.MyException" -->
    <s:property value="exception.message"/>
  </body>
</html>
**总结**
局部异常处理比全局异常处理高，并且可覆盖全局异常处理，如果定义了全局异常映射，那么会对所有的Action生效，反之定义了局部异常映射则会对当前Action生效，
如果在全局区域和局部区域定义了相同的异常映射，首先去局部异常区域找result结果页面，如果找到了，则直接跳转到错误结果页面，不管全局有没有相同的结果，都被局部所覆盖，如果在局部区域没找到，则去全局区域找。
原帖地址：
[http://starbhhc.iteye.com/blog/1329173](http://starbhhc.iteye.com/blog/1329173)
[http://blog.csdn.net/hzc543806053/article/details/7493268](http://blog.csdn.net/hzc543806053/article/details/7493268)
