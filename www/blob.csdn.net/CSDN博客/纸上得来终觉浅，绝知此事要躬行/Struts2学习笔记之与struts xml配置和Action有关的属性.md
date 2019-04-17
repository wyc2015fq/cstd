# Struts2学习笔记之与struts.xml配置和Action有关的属性 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2012年07月17日 22:52:30[boonya](https://me.csdn.net/boonya)阅读数：3861标签：[struts																[action																[class																[string																[login](https://so.csdn.net/so/search/s.do?q=login&t=blog)
个人分类：[Struts2](https://blog.csdn.net/boonya/article/category/879253)





                  Struts2学习笔记

第三记：与struts.xml配置和Action有关的属性

**（1）、解决struts2输入”<“无自动提示的问题**

         原因：未联网无法获取到struts的DTD文件。

         解决办法：添加struts2文件中的模板DTD。

         添加步骤：

         windows>reference>myEclipse>Files And Editors>XML>XML Catalog

         此时将弹出一个对话框：

         location：选择你要添加的DTD文件的路径。

         Key Type：URI

         Key：网络路径，如：http://struts.apache.org/dtds/struts-2.1.dtd
**（2）、Action名称的访问搜索排序：**

        2-1、探索包的命名空间是否存在，假设在example下存在login这个action,那么：test/example/hello/login.action和test/example/login.action

                  都可以访问得到login Action。

        2-2、逐级向上一级目录寻找Action是否存在，如果存在就可以访问得到url后面所指定的Action。

        2-3、如果包（命名空间）存在，但该包下面没有对应的Action，那么它会到namespace=”/“的目录下寻找，

                  如果还没有则到默认的命名空间中去找，看是否有该Action。

         注：默认命名空间是指包下面没有设定namespace属性，或namespace=""。
**（3）、Action实现页面跳转和配置的各项默认值：**

        3-1、Action设置单个页面跳转：

            <action name="example" class="com.struts2.house.rent.action.ExampleAction">

                      <result >/login.jsp</result>

           </action>

        3-2、Action设置多个页面跳转：

            <action name="example" class="com.struts2.house.rent.action.ExampleAction">

                      <result name="error">/error.jsp</result>

                      <result name="login">/login.jsp</result>

            </action>

        3-3、Action设置多个页面跳转和重定向：

            <action name="example" class="com.struts2.house.rent.action.ExampleAction">

                       <result name="success" type="redirect">house!pre_list.action</result>//--------页面重定向

                      <result name="login">/login.jsp</result>

           </action>

        3-4、Action的默认设置：

                  如果不为Action设置class,则Action默认是ActionSupport。

                  如果不为Action设置method属性，默认是执行execute方法。

                  如果没有设置result的name值，默认为”SUCCESS“

**（4）、为Action的属性注入值：**

          在struts.xml中的属性注入配置：

              <action name="example" class="com.struts2.house.rent.action.ExampleAction">

                      <param name="country">china</param>

                      <result name="success” >/login.jsp</result>

             </action>

           Java类：

           public class ExampleAction{

                   private String country;


                   public  String getCountry(){

                           return this.country;

                   }

                   public void setCountry(String country){

                           this.country=country;

                   }

                   public String execute(){

                              return "SUCCESS";

                   }


          }

         视图JSP：

         使用EL表达式：${country}//显示结果：china](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=action&t=blog)](https://so.csdn.net/so/search/s.do?q=struts&t=blog)




