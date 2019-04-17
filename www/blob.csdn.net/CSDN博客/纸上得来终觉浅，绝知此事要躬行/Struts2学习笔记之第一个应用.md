# Struts2学习笔记之第一个应用 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2012年07月16日 23:29:55[boonya](https://me.csdn.net/boonya)阅读数：1243标签：[struts																[action																[login																[passwords																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)
个人分类：[Struts2](https://blog.csdn.net/boonya/article/category/879253)





Struts2学习笔记 


第二记：Struts2的第一个应用

**（1）、编写struts.xml创建Action,如：ExampleAction**

     <?xml version="1.0" encoding="UTF-8" ?>

     <!DOCTYPE struts PUBLIC "-//Apache Software Foundation//DTD Struts Configuration 2.1//EN" "http://struts.apache.org/dtds/struts-2.1.dtd">

     <struts>

         <package name="default" namespace="/" extends="struts-default">


                <action name="example" class="com.struts2.house.rent.action.ExampleAction" method="login">

                            <result name="login">/login.jsp</result>

               </action>

        </package>

     </struts>

      配置说明：

      N1、name:包名，但不同于Java程序中的包名。

      N2、namespace:命名空间，默认为“/”，表示根路径下。

      N3、extends:继承，主要是继承struts2的拦截器功能。

      N4、action_name:action的名称，也可以是是别名，唯一标识。

      N5、method：表示访问该Action的login方法。


      N6、result_name:表示访问的视图名称，即jsp页面的名称。

**（2）、编写Action对应的Java文件：**

       public class ExampleAction{  //无侵入式实现，这里没有像Struts1一样依赖于Action

                private  String   msg;

                public String getMessage(){

                       return msg;


               }

               public   String  login(){

                      msg="Hello,World!";

                      return "login";//表示要提交的视图名称,返回的必须是string


              }


      }


**（3）、视图表现层：**

       在JSP中导入Struts2的标签：

      s-tag:<%@ taglib prefix="s" uri="/struts-tags" %>

      s2-tag:<%@ taglib prefix="s2" uri="/struts-tags" %>


      sx-tag:<%@ taglib prefix="sx" uri="/struts-dojo-tags"%>


      s标签：<s:property value="This is my frirst Struts2 application!" />,s标签含有许多用法：如<s:if>,<s:bean>,<s:param>,<s:iterator>...等等。

      s2标签：<s2:form  action="login">

                           <s2:textfield  name="users.name" label="username"></s2:textfield><br/>

                          <s2:password  name="users.password" label="password"><s2:password><br/>

                          <s2:submit >login</s2:submit>


                      </s2:form>


      sx标签：<sx:a targets="pageDiv" href="a_link_url" errorText="...error...">

                                  <s:property value="点这里" />

                              </sx:a>   //生成超链接..................


       EL表达式： ${message}；//注意：直接获取后台传送的值，是get方法名而不是成员变量,必须提供get方法才行。

**（4）访问action对应的视图：**

       在浏览器中输入项目名称及其访问路径：

        如，http://localhost:8080/StrutsTest/example/login.action

        根路径：http://localhost:8080/StrutsTest/

       如果package的命名空间是"/test/user"

       则访问路径是：http://localhost:8080/StrutsTest/test/user/example/login.action

       注：”.action“可以省略不写。](https://so.csdn.net/so/search/s.do?q=passwords&t=blog)](https://so.csdn.net/so/search/s.do?q=login&t=blog)](https://so.csdn.net/so/search/s.do?q=action&t=blog)](https://so.csdn.net/so/search/s.do?q=struts&t=blog)




