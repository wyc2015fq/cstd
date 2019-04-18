# 扩展Struts2--自定义String和XML格式的Result - z69183787的专栏 - CSDN博客
2015年05月21日 22:12:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：702
struts2虽然继承了webwork优秀的MVC分离，可是有很多地方让人百思不得其解！最让人离谱的是，返回的结果集中居然没有String，xml这两种非常常用的类型。还是自己动手，丰衣足食：
**第一种方式：使用“PlainText Result”**
    先看官方文档对plain text结果的定义：“A result that send the content out as plain text. Usefull typically when needed to display the raw content of a JSP or Html file for example.”这是一个纯扯蛋的说法。。。貌似感觉只能返回jsp页面似的，最起码他误导了我。
    其实使用“PlainText Result” ，返回的结果是**未进行格式和编码定义的字符串**。什么意思？就类似于“FreeMarker Result”  ，返回一个*.ftl格式的模板，你完全可以在*.ftl写string，那么结果就是string；也可以在里面写xml，那么结果就是xml。
   举例如下：
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE struts PUBLIC
-         "-//Apache Software Foundation//DTD Struts Configuration 2.0//EN"
-         "http://struts.apache.org/dtds/struts-2.0.dtd">
- <struts>
- <packagename="example"namespace="/example"
- extends="struts-default">
- <actionname="outputXml"method="outxml"class="example.OutputXml">
- <resultname="xmlMessage"type="plaintext"></result>
- </action>
- </package>
- </struts>
这里定义了xmlMessage为plain text结果，至于它具体是什么，看下面的Action类：
- public**class OutputXml extends ActionSupport {**
- **publicvoid outxml() throws Exception {**
- **        HttpServletResponse response = ServletActionContext.getResponse();**
- **        response.setContentType("text/xml ");**
- **        PrintWriter pw = response.getWriter();**
- **        pw.print("<cc>cccccc</cc>");**
- **    }**
    在代码中，我们显式的给response定义了ContentType。那么返回去的内容"<cc>cccccc</cc>"就会被接收方按xml进行解析。
 而如果需要返回的是String类型，那么contentType = "text/plain”。
如果进一步需要指明编码，那么contentType = "text/plain; charset=UTF-8";
    到这里理解“plain text的结果是未进行格式和编码定义的字符串”应该就不困难了。基于http的内容传输实际都是字符串型，类型的定义是放在response的contentType 中。
**第二种方式： 直接扩展struts2的结果集StrutsResultSupport ：**
代码如下：
应该很容易懂了。。嘿嘿
- package commons.struts2;
- 
- import java.io.PrintWriter;
- 
- import javax.servlet.http.HttpServletResponse;
- 
- import org.apache.struts2.dispatcher.StrutsResultSupport;
- 
- import com.opensymphony.xwork2.ActionInvocation;
- 
- /**
-  * result type for output string in action
-  * 
-  * @author songwei,yaolei <b>Example:</b>
-  * 
-  * <pre>
-  * <!-- START SNIPPET: example -->
-  * <result name="success" type="string">
-  *   <param name="stringName">stringName</param>
-  * </result>
-  * <!-- END SNIPPET: example -->
-  * </pre>
-  * 
-  */
- publicclass StringResultType extends StrutsResultSupport {
- privatestaticfinallong serialVersionUID = 1L;
- private String contentTypeName;
- private String stringName = "";
- 
- public StringResultType() {
- super();
-     }
- 
- public StringResultType(String location) {
- super(location);
-     }
- 
- protectedvoid doExecute(String finalLocation, ActionInvocation invocation)
- throws Exception {
-         HttpServletResponse response = (HttpServletResponse) invocation
-                 .getInvocationContext().get(HTTP_RESPONSE);
- // String contentType = (String)
- // invocation.getStack().findValue(conditionalParse(contentTypeName,
- // invocation));
-         String contentType = conditionalParse(contentTypeName, invocation);
- if (contentType == null) {
-             contentType = "text/plain; charset=UTF-8";
-         }
-         response.setContentType(contentType);
-         PrintWriter out = response.getWriter();
- // String result = conditionalParse(stringName, invocation);
-         String result = (String) invocation.getStack().findValue(stringName);
-         out.println(result);
-         out.flush();
-         out.close();
- 
-     }
- 
- public String getContentTypeName() {
- return contentTypeName;
-     }
- 
- publicvoid setContentTypeName(String contentTypeName) {
- this.contentTypeName = contentTypeName;
-     }
- 
- public String getStringName() {
- return stringName;
-     }
- 
- publicvoid setStringName(String stringName) {
- this.stringName = stringName;
-     }
- 
- }
- 
使用的方法：
1.Action
- package test;
- 
- import com.opensymphony.xwork2.ActionSupport;
- 
- publicclass MyAction extends ActionSupport{
- 
-     String  result="abc";
- 
- public String ajax() {
- return"ajaxResponse";
-     }
- 
- // getter && setter
- public String getResult() {
- return result;
-     }
- publicvoid setResult(String result) {
- this.result = result;
-     }
- 
- }
2.定义struts.xml
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE struts PUBLIC
-     "-//Apache Software Foundation//DTD Struts Configuration 2.0//EN"
-     "http://struts.apache.org/dtds/struts-2.0.dtd">
- 
- <struts>
- <packagename="test"extends="struts-default">
- <result-types>
- <result-typename="string"class="test.StringResultType"></result-type>
- </result-types>
- 
- <actionname="myAction"class="test.MyAction">
- <resultname="ajaxResponse"type="string">
- <paramname="stringName">result</param>
- </result>
- </action>
- </package>
- </struts>
无非也就是将string结果集进行申明，然后给返回“ajaxResponse”的结果绑定变量名。这里定义返回Action的String  result变量，即“abc”。
**第三种方式：利用**[Velocity
 Result](file:///E:/java/jar/struts-2.0.11/docs/docs/velocity-result.html)、[FreeMarker Result](file:///E:/java/jar/struts-2.0.11/docs/docs/freemarker-result.html)
类似第一种方式，这里不再重复
这里简单说一下，怎样实现一个返回JSON串的Result
方法1：
自定义的Result
**[java]**[view
 plain](http://blog.csdn.net/yuguiyang1990/article/details/11095963#)[copy](http://blog.csdn.net/yuguiyang1990/article/details/11095963#)
- package org.ygy.demo.result;  
- 
- import java.io.PrintWriter;  
- 
- import javax.servlet.http.HttpServletResponse;  
- 
- import net.sf.json.JSONObject;  
- import net.sf.json.JSONSerializer;  
- import net.sf.json.JsonConfig;  
- 
- import org.apache.struts2.ServletActionContext;  
- import org.apache.struts2.dispatcher.StrutsResultSupport;  
- 
- import com.opensymphony.xwork2.ActionInvocation;  
- 
- /**
-  * 
-  * @author yuguiyang
-  * @description 返回JSON
-  * @time 2013-9-4
-  * @version V1.0
-  */
- publicclass JsonResult extends StrutsResultSupport {  
- privatestaticfinallong serialVersionUID = 2232581955223674065L;  
- 
- private Object result;  
- private JsonConfig jsonConfig;  
- 
- public JsonResult() {}  
- 
- public JsonResult(JsonConfig jsonConfig) {  
- super();  
- this.jsonConfig = jsonConfig;  
-     }  
- 
- public Object getResult() {  
- return result;  
-     }  
- 
- publicvoid setResult(Object result) {  
- this.result = result;  
-     }  
- 
- @Override
- protectedvoid doExecute(String finalLocation, ActionInvocation invocation) throws Exception {  
-         HttpServletResponse response = null;  
- try {  
-             response = ServletActionContext.getResponse();  
-             PrintWriter printWriter = response.getWriter();  
- 
- if (jsonConfig != null) {  
-                 printWriter.write(JSONObject.fromObject(result, jsonConfig).toString());  
-             } else {  
-                 printWriter.write(JSONSerializer.toJSON(result).toString());  
-             }  
-         } catch (Exception e) {  
- thrownew Exception("json parse error!");  
-         } finally {  
-             response.getWriter().close();  
-         }  
- 
-     }  
- 
- }  
这里使用json-lib来将对象转换成JSON串
Action中是这样的：
注意：这里的返回值，就是刚才自定义的JsonResult
**[java]**[view
 plain](http://blog.csdn.net/yuguiyang1990/article/details/11095963#)[copy](http://blog.csdn.net/yuguiyang1990/article/details/11095963#)
- public JsonResult ha() {  
-     System.out.println("--from ha().");  
- 
-     List<String> msgs = new ArrayList<String>();  
-     msgs.add("one");  
-     msgs.add("two");  
-     msgs.add("three");  
- 
-     JsonResult result = new JsonResult();  
-     result.setResult(msgs);  
- 
- return result;  
- }  
配置文件是这样的：
**[html]**[view
 plain](http://blog.csdn.net/yuguiyang1990/article/details/11095963#)[copy](http://blog.csdn.net/yuguiyang1990/article/details/11095963#)
- <actionname="ha"class="org.ygy.demo.result.HelloAction"method="ha">
- <resultname="success"type="json">/hello.jsp</result>
- </action>
PS:这里有点儿乱，明天在研究一下吧![抓狂](http://static.blog.csdn.net/xheditor/xheditor_emot/default/crazy.gif)
方法二：
**[java]**[view
 plain](http://blog.csdn.net/yuguiyang1990/article/details/11095963#)[copy](http://blog.csdn.net/yuguiyang1990/article/details/11095963#)
- package org.ygy.demo.result;  
- 
- import java.util.ArrayList;  
- import java.util.List;  
- 
- import com.opensymphony.xwork2.ActionSupport;  
- 
- publicclass HelloAction extends ActionSupport {  
- privatestaticfinallong serialVersionUID = 6422231025855671997L;  
- 
- private Person person;  
- 
- public String say() {  
-         System.out.println("--from say().");  
- 
- 
- return SUCCESS;  
-     }  
- 
- public String go() {  
-         System.out.println("--from go().");  
- 
- return SUCCESS;  
-     }  
- 
- public JsonResult ha() {  
-         System.out.println("--from ha().");  
- 
-         List<String> msgs = new ArrayList<String>();  
-         msgs.add("one");  
-         msgs.add("two");  
-         msgs.add("three");  
- 
-         JsonResult result = new JsonResult();  
-         result.setResult(msgs);  
- 
- return result;  
-     }  
- 
- public String haha() {  
-         person = new Person(100 , "拉速度");  
- 
- return SUCCESS;  
-     }  
- 
- public Person getPerson() {  
- return person;  
-     }  
- 
- publicvoid setPerson(Person person) {  
- this.person = person;  
-     }  
- 
- }  
自定义Result：
**[java]**[view
 plain](http://blog.csdn.net/yuguiyang1990/article/details/11095963#)[copy](http://blog.csdn.net/yuguiyang1990/article/details/11095963#)
- package org.ygy.demo.result;  
- 
- import java.io.PrintWriter;  
- import java.util.Map;  
- 
- import javax.servlet.http.HttpServletResponse;  
- 
- import net.sf.json.JSONSerializer;  
- 
- import org.apache.struts2.ServletActionContext;  
- import org.apache.struts2.dispatcher.StrutsResultSupport;  
- 
- import com.opensymphony.xwork2.ActionContext;  
- import com.opensymphony.xwork2.ActionInvocation;  
- import com.opensymphony.xwork2.util.ValueStack;  
- 
- publicclass JsonResult2 extends StrutsResultSupport {  
- privatestaticfinallong serialVersionUID = -6528161012160891182L;  
- 
- @Override
- protectedvoid doExecute(String finalLocation, ActionInvocation invocation) throws Exception {  
-         ActionContext context = invocation.getInvocationContext();  
- 
- //通过invocation获取Action的变量 
-         Map<String , Object> temp = context.getContextMap();  
- 
-         Object action = invocation.getAction();  
- 
-          ValueStack stack = invocation.getStack();  
-          Object rootObject = stack.findValue("person");  
- if(rootObject instanceof Person) {  
-              System.out.println("I get it.");  
-          }  
- 
- //转换为JSON字符串
- 
-         HttpServletResponse response = ServletActionContext.getResponse();  
-         PrintWriter out = response.getWriter();  
- 
-         out.println(JSONSerializer.toJSON(rootObject).toString());  
-         out.println(JSONSerializer.toJSON((Person)rootObject).toString());  
- 
-         out.flush();  
-         out.close();  
-     }  
- 
- }  
配置文件：
**[html]**[view
 plain](http://blog.csdn.net/yuguiyang1990/article/details/11095963#)[copy](http://blog.csdn.net/yuguiyang1990/article/details/11095963#)
- <actionname="haha"class="org.ygy.demo.result.HelloAction"method="haha">
- <resultname="success"type="json2">/do you know me</result>
- </action>
这一次，在自定义Result时，在在值栈中找到变量Person，Struts有自己实现的JSONResult，大家可以看一下源码。
