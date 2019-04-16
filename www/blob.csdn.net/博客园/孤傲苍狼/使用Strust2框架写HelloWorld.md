# 使用Strust2框架写HelloWorld - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [使用Strust2框架写HelloWorld](https://www.cnblogs.com/xdp-gacl/p/3496983.html)



## 一、创建JavaWeb项目

![](https://images0.cnblogs.com/blog/289233/201312/29225632-7c1642bd4f2a4e248bd1490846b5833c.png)

## 二、搭建Stust2 FrameWork开发环境

三步完成Struts2 FrameWork开发环境的搭建

1.加入搭建Struts2 FrameWork开发环境的必需Jar包

将必需的11个Jar包复制到【lib】文件夹中　

![](https://images0.cnblogs.com/blog/289233/201312/29225840-9a4b0b5c8b244dabbb09b49c1f371b53.png)

2.在【src】目录下创建“struts.xml”文件

![](https://images0.cnblogs.com/blog/289233/201312/29225941-86a8142a7d4d4e858725539549b7573c.png)

在strusts.xml文件中加入如下配置信息

```
1 <?xml version="1.0" encoding="UTF-8"?>
 2 <!DOCTYPE struts PUBLIC
 3     "-//Apache Software Foundation//DTD Struts Configuration 2.0//EN"
 4     "http://struts.apache.org/dtds/struts-2.0.dtd">
 5 
 6 <struts>
 7     <constant name="struts.devMode" value="true" />
 8     <package name="Hello_World_Struts2" extends="struts-default">
 9         <action name="index">
10             <result>/index.jsp</result>
11         </action>
12     </package>
13 </struts>
```

3.在【web.xml】文件中配置Struts2的核心过滤器**(****红色部分)**

```
1 <?xml version="1.0" encoding="UTF-8"?>
 2 <web-app version="3.0" 
 3     xmlns="http://java.sun.com/xml/ns/javaee" 
 4     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
 5     xsi:schemaLocation="http://java.sun.com/xml/ns/javaee 
 6     http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd">
 7   <display-name></display-name>    
 8   <welcome-file-list>
 9     <welcome-file>index.jsp</welcome-file>
10   </welcome-file-list>
11   
12     **<filter>13       <filter-name>struts2</filter-name>14       <filter-class>         org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter         </filter-class>15     </filter>16     17     <filter-mapping>18       <filter-name>struts2</filter-name>19        <url-pattern>/*</url-pattern>20     </filter-mapping>**21   
22 </web-app>
```

## 三、创建Model类MessageStore

在项目的src文件夹下添加一个“MessageStore.java”文件，如下图所示

![](https://images0.cnblogs.com/blog/289233/201312/29230437-0519f2c5db0e472b9a9d0269312f1bab.png)

![](https://images0.cnblogs.com/blog/289233/201312/29230504-8baba82a4ee14d1ca7da31da5417c134.png)

点击【Finish】完成添加，此时在src目录下就可以看到“MessageStore.java”这个文件了

![](https://images0.cnblogs.com/blog/289233/201312/29230548-26608f1d448340aa8758bad000e47360.png)

打开“MessageStore.java”，编写如下代码：

```
1 package org.apache.struts.helloworld.model;
 2 /**
 3  * Model class that stores a message.
 4  * @author xdp
 5  * @since 2013-3-24
 6  */
 7 public class MessageStore {
 8     private String message;
 9     public MessageStore(String msg){
10         this.setMessage(msg);
11     }
12     public String getMessage() {
13         return message;
14     }
15     public void setMessage(String message) {
16         this.message = message;
17     }
18 }
```

## 四、创建Action类HelloWorldAction，充当Controller

在项目的src文件夹下添加一个“HelloWorldAction.java”文件，放到“org.apache.struts.helloworld.action”这个包中，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29230843-91b7e7e216524d269008892735bdb0c9.png)

 点击【Finish】完成添加，此时在src文件夹下就可以看到“HelloWorldAction.java”文件

![](https://images0.cnblogs.com/blog/289233/201312/29230954-1bb3b4df90f74f3dabbbf10ce448c725.png)

在“HelloWorldAction.java”文件中编写如下代码：

```
1 package org.apache.struts.helloworld.action;
 2 import org.apache.struts.helloworld.model.MessageStore;
 3 import com.opensymphony.xwork2.ActionSupport;
 4 /**Acts as a Struts 2 controller that responds
 5  * to a user action by setting the value
 6  * of the Message model class, and returns a String result.
 7  * @author xdp
 8  * @since 2013-3-24
 9  * @version 1.0
10  */
11 public class HelloWorldAction extends ActionSupport {
12     private MessageStore msgStore;
13     @Override
14     public String execute() throws Exception {
15     　　 msgStore = new MessageStore("HelloWorld!");
16         return SUCCESS;
17     }
18     public MessageStore getMsgStore() {
19         return msgStore;
20     }
21     public void setMsgStore(MessageStore msgStore) {
22         this.msgStore = msgStore;
23     }
24 }
```

## 五、创建View(视图)HelloWorld.jsp

在【WebRoot】文件夹下创建“HelloWorld.jsp”文件，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29231151-59481a0d72394e849fde33a955f51969.png)

此时弹出创建JSP文件的对话框

![](https://images0.cnblogs.com/blog/289233/201312/29231244-e3ca6158603e4fb3813eab740ffd439c.png)

点击【Finish】完成添加，此时在【WebRoot】文件夹下就可以看到“HelloWorld.jsp”文件

![](https://images0.cnblogs.com/blog/289233/201312/29231322-82439b986b044b3f9998edf05185e805.png)

在HelloWorld.jsp编写如下代码

```
1 <%@ page language="java" contentType="text/html; charset=ISO-8859-1"
 2     pageEncoding="ISO-8859-1"%>
 3 **<%@ taglib prefix="s" uri="/struts-tags" %>** 4 <!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
 5 <html>
 6 <head>
 7 <meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
 8 <title>Hello World!</title>
 9 </head>
10 <body>
11     <h2>**<****s:property value="msgStore.message" ****/>**</h2>
12 </body>
13 </html>
```

## 六、配置struts.xml

在struts.xml中加入红色部分的配置信息

```
1 <?xml version="1.0" encoding="UTF-8"?>
 2 <!DOCTYPE struts PUBLIC
 3     "-//Apache Software Foundation//DTD Struts Configuration 2.0//EN"
 4     "http://struts.apache.org/dtds/struts-2.0.dtd">
 5 <struts>
 6     <constant name="struts.devMode" value="true" />
 7     <package name="Hello_World_Struts2" extends="struts-default">
 8         <action name="index">
 9             <result>/index.jsp</result>
10         </action>
11         **<action name="hello"12             class="org.apache.struts.helloworld.action.HelloWorldAction" 　　　　　　　　　　method="execute">13             <result name="success">/HelloWorld.jsp</result>14         </action>**15     </package>
16 </struts>
```

## 七、创建Action的URL链接

在index.jsp中添加Action的URL链接,index.jsp中的代码如下：

```
1 <%@ page language="java" contentType="text/html; charset=ISO-8859-1"
 2     pageEncoding="ISO-8859-1"%>
 3 <%@ taglib prefix="s" uri="/struts-tags" %>
 4 <!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
 5 <html>
 6 <head>
 7 <meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
 8 <title>Basic Struts 2 Application - Welcome</title>
 9 </head>
10 <body>
11 <h1>Welcome To Struts 2!</h1>
12 <p><a href="<s:url action='hello'/>">Hello World</a></p>
13 </body>
14 </html>
```

“**<%@ taglib prefix=*"s"* uri=*"/struts-tags"* %>**”表示引入struts2的标签，“**<s:url action>**”是一个Struts2的标签，用于创建Action的URL链接,**“****<s:url action=*'hello'*/>****”**表示链接到一个名字为“hello”的action，这个名字为“hello”的action是在struts.xml配置文件配置好的，在struts.xml文件中可以找到如下的配置信息

```
1 <action name="hello"
2    class="org.apache.struts.helloworld.action.HelloWorldAction" 
　　　　method="execute">
3    <result name="success">/HelloWorld.jsp</result>
4 </action>
```

## 八、部署运行

将项目发布到MyEclispe自带的Tomcat服务器中，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29232043-4961117cc78841ba97fb76c0bb95cc51.png)

发布成功后就可以在Tomcat服务器中看到，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29232122-4fc9094acb904d41bc39abb1baefb7f8.png)

打开浏览器，在浏览器中输入[**http://localhost:8080/Hello_World_Struts2/index.action**](http://localhost:8080/Hello_World_Struts2/index.action)，此时就可以看到运行效果，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29232205-2b52a1088ed742b6ba99ae98ca4e157c.png)

点击【HelloWorld】超链接，此时看到的效果如下：

![](https://images0.cnblogs.com/blog/289233/201312/29232244-31065347a4f54097a8d7cd5a16cb8c47.png)

运行正常，HelloWorld！输出来了，使用Struts2编写入门级HelloWord程序成功！











