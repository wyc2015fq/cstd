# Struts2学习笔记之动态方法调用和参数获取 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2012年07月20日 23:13:16[boonya](https://me.csdn.net/boonya)阅读数：2088








  Struts2学习笔记

第六记：struts.xml引入多个配置文件，动态方法调用和参数获取

**（1）、引入多个struts的配置文件**

         <include file="struts_user.xml" />

         <include file="struts_person.xml" />

**（2）、动态方法调用**

          2-1：url指定方法名访问：

         如：useraction!userinfo.action,表示调用useraction的userinfo方法。

         2-2：配置常量实现动态方法调用

         <constant name="Struts.enable.DynamicMethodInvocation"  value="true"/>


         2-3：使用通配符访问


       <action name="user_*"  class="com.controller.UserAction" method="{1}">

             <result>/index.jsp</result>

      </action>

         如果UserAction拥有两个方法：a和b

        则调用时可以这样写：user_a.action,user_b.action。


**（3）、参数获取：利用属性或对象的反射实现**

      3-1：在Action类里面添加参数字段

     如：public class UserAction{

               private  String name;

               public void setName(String name){//此方法必不可少

                    this.name=name;


             }

           //other methods ....................


    }

    注意：字段名和set后面的名称一致。

   此参数获取不区分get/post方式，其本质都是调用HttpRequest的getParameter方法获取的。


    3-2：对象数据类型（复合类型）反射接收参数

         public class UserAction{

               private  Person  person;

               public void setPerson(String person){//此方法必不可少

                    this.person=person;


              }

               public String saveUser(){

                        return "SUCCESS";


              }


       }

       表单字段可以是person.id,person.name;

     JSP页面接收EL表达式：ID=${person.id},Name=${person.name}












