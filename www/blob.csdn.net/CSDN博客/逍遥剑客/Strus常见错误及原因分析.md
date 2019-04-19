# Strus常见错误及原因分析 - 逍遥剑客 - CSDN博客
2006年12月01日 00:19:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1640标签：[struts																[action																[exception																[class																[bean																[html](https://so.csdn.net/so/search/s.do?q=html&t=blog)](https://so.csdn.net/so/search/s.do?q=bean&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=exception&t=blog)](https://so.csdn.net/so/search/s.do?q=action&t=blog)](https://so.csdn.net/so/search/s.do?q=struts&t=blog)
个人分类：[基础知识](https://blog.csdn.net/xoyojank/article/category/259553)
本篇文章包含了在用Struts开发web应用时经常碰到的一些异常和错误，根据异常或错误信息本身，经常可以找到潜在的错误发生原因。
下面列出了一些Struts的常见错误和异常，并给出了一些可能发生此类错误或异常的原因。有的后面有相关连接，你可以通过它找到更多的信息。
Cannot retrieve mapping for action
异常
 javax.servlet.jsp.JspException: Cannot retrieve mapping for action /Login （/Login是你的action名字）

可能原因
 action没有再struts-config.xml 中定义，或没有找到匹配的action，例如在JSP文件中使用 <html:form action="Login.do".将表单提交给Login.do处理，如果出现上述异常，请查看struts-config.xml中的定义部分，有时可能是打错了字符或者是某些不符合规则，可以使用struts console工具来检查。
Cannot retrieve definition for form bean null
异常
 org.apache.jasper.JasperException: Cannot retrieve definition for form bean null
可能原因       
 这个异常是因为Struts根据struts-config.xml中的mapping没有找到action期望的form bean。大部分的情况可能是因为在form-bean中设置的name属性和action中设置的name属性不匹配所致。换句话说，action和form都应该各自有一个name属性，并且要精确匹配，包括大小写。这个错误当没有name属性和action关联时也会发生，如果没有在action中指定name属性，那么就没有name属性和action相关联。当然当action制作某些控制时，譬如根据参数值跳转到相应的jsp页面，而不是处理表单数据，这是就不用name属性，这也是action的使用方法之一。
No action instance for path /xxxx could be created 
异常
 No action instance for path /xxxx could be created
可能原因
 特别提示：因为有很多中情况会导致这个错误的发生，所以推荐大家调高你的web服务器的日志/调试级别，这样可以从更多的信息中看到潜在的、在试图创建action类时发生的错误，这个action类你已经在struts-config.xml中设置了关联（即添加了<action>标签）。
在struts-config.xml中通过action标签的class属性指定的action类不能被找到有很多种原因，例如：
定位编译后的.class文件失败。Failure to place compiled .class file for the action in the classpath (在web开发中，class的的位置在r WEB-INF/classes，所以你的action class必须要在这个目录下。例如你的action类位于WEB-INF/classes/action/Login.class,那么在struts-config.xml中设置action的属性type时就是action.Login). 
拼写错误，这个也时有发生，并且不易找到，特别注意第一个字母的大小写和包的名称。
在struts-config.xml中指定的action类没有继承自Stuts的Action类，或者你自定义的Action类没有继承自Struts提供的Action类。
你的action类必须继承自Struts提供的Action类。
你的classpath的问题。例如web server没有发现你的资源文件，资源文件必须在WEB-INF/classes/目录下。 
Problem in struts-config.xml file with action mapping. 
Problem with data-sources.xml file.
相关链接
[http://www.mail-archive.com/struts-user@jakarta.apache.org/msg65874.html](http://www.mail-archive.com/struts-user@jakarta.apache.org/msg65874.html)
Action Mapping mistake in struts-config.xml:
[http://www.manning.com/ao/readforum.html?forum=siaao&readthread=177](http://www.manning.com/ao/readforum.html?forum=siaao&readthread=177)
data-sources.xml file?:
[http://www.caucho.com/quercus/faq/section.xtp?section_id=30](http://www.caucho.com/quercus/faq/section.xtp?section_id=30)
No getter method for property XXXX of bean org.apache.struts.taglib.html.BEAN
异常
 javax.servlet.jsp.JspException: No getter method for property username of bean org.apache.struts.taglib.html.BEAN
可能原因
 没有位form bean中的某个变量定义getter 方法 
这个错误主要发生在表单提交的FormBean中，用struts标记<html:text property=”username”>时，在FormBean中必须有一个getUsername()方法。注意字母“U”。 
Related Links
 Case can trip up the matching between get method's name and name specified in Struts tag
[http://saloon.javaranch.com/cgi-bin/ubb/ultimatebb.cgi?ubb=get_topic&f=58&t=000163](http://saloon.javaranch.com/cgi-bin/ubb/ultimatebb.cgi?ubb=get_topic&f=58&t=000163)
java.lang.NoClassDefFoundError: org/apache/struts/action/ActionForm
错误
 java.lang.NoClassDefFoundError: org/apache/struts/action/ActionForm
可能原因
 这个错误主要发生在在classpath中找不到相应的Java .class文件。如果这个错误发生在web应用程序的运行时，主要是因为指定的class文件不在web server的classpath中（/WEB-INF/classes 和 /WEB-INF/lib）。
在上面的错误中，原因是找不到ActionForm类。
This error is sometimes seen when one or more ActionForm.class instances are actually in the classpath. This most often occurs when ActionForm.class is made available correctly by placing struts.jar in the /WEB-INF/lib directory. When this library has been correctly placed and it is verified that ActionForm.class actually is present in the struts.jar file, the problem is either that more than one copy of ActionForm.class is in the classpath or (more likely) that duplicate versions of class files other than ActionForm are in the same classpath, causing confusion. This is especially true if a class that extends ActionForm is made available twice, such as in an .ear file that encompasses a .war file as well as in the .war file's own classpath (/WEB-INF/classes). This problem can be resolved by guaranteeing that there are no redundant classes, especially those related to Struts (directly from Struts or extensions of Struts), in the web application's view.
相关连接
 EJB and Web Shared Links:
[http://forum.java.sun.com/thread.jsp?forum=26&thread=413060&tstart=0&trange=15](http://forum.java.sun.com/thread.jsp?forum=26&thread=413060&tstart=0&trange=15)
Keep Action and ActionForm (and their children) as non-overlapping unit(s) of an application
[http://www.mail-archive.com/struts-user@jakarta.apache.org/msg47466.html](http://www.mail-archive.com/struts-user@jakarta.apache.org/msg47466.html)
[http://www.mail-archive.com/struts-user@jakarta.apache.org/msg47467.html](http://www.mail-archive.com/struts-user@jakarta.apache.org/msg47467.html)
Exception creating bean of class org.apache.struts.action.ActionForm: {1}
异常
 javax.servlet.jsp.JspException: Exception creating bean of class org.apache.struts.action.ActionForm: {1}
可能原因
 Instantiating Struts-provided ActionForm class directly instead of instantiating a class derived off ActionForm. This might occur implicitly if you specify that a form-bean is this Struts ActionForm class rather than specifying a child of this class for the form-bean.
Not associating an ActionForm-descended class with an action can also lead to this error.
Related Links

Cannot find ActionMappings or ActionFormBeans collection
Exception
 javax.servlet.jsp.JspException: Cannot find ActionMappings or ActionFormBeans collection
可能原因
 不是标识Struts actionServlet的<servlet>标记就是映射.do扩展名的<sevlet-mapping>标记或者两者都没有在web.xml中声明。
在struts-config.xml中的打字或者拼写错误也可导致这个异常的发生。例如缺少一个标记的关闭符号/>。最好使用struts console工具检查一下。
另外，load-on-startup必须在web.xml中声明，这要么是一个空标记，要么指定一个数值，这个数值用来表servlet运行的优先级，数值越大优先级越低。
还有一个和使用load-on-startup有关的是使用Struts预编译JSP文件时也可能导致这个异常。
相关链接
 Explicitly Define <load-on-startup>
[http://saloon.javaranch.com/cgi-bin/ubb/ultimatebb.cgi?ubb=get_topic&f=50&t=001055](http://saloon.javaranch.com/cgi-bin/ubb/ultimatebb.cgi?ubb=get_topic&f=50&t=001055)
[http://threebit.net/tutorials/ejb/general/](http://threebit.net/tutorials/ejb/general/)
NullPointerException at ... RequestUtils.forwardURL
异常
 java.lang.NullPointerException at org.apache.struts.util.RequestUtils.forwardURL(RequestUtils.java:1223) 
可能原因
 在struts-config.xml中的forward元素缺少path属性。例如应该是如下形式：
<forward name="userhome" path="/user/userhome.jsp"/>
Cannot find bean org.apache.struts.taglib.html.BEAN in any scope
Exception
 javax.servlet.jsp.JspException: Cannot find bean org.apache.struts.taglib.html.BEAN in any scope

Probable Causes
 试图在Struts的form标记外使用form的子元素。这常常发生在你在</html:form>后面使用Struts的html标记。
另外要注意可能你不经意使用的无主体的标记，如<html:form … />，这样web 服务器解析时就当作一个无主体的标记，随后使用的所有标记都被认为是在这个标记之外的，如又使用了<html:text property=”id”>
还有就是在使用taglib引入HTML标记库时，你使用的prefix的值不是html。 
相关连接
 Using form subelements outside of a form tag
[http://forum.java.sun.com/thread.jsp?thread=337537&forum=4&message=1384153](http://forum.java.sun.com/thread.jsp?thread=337537&forum=4&message=1384153)
Missing message for key xx.xx.xx
Exception
 javax.servlet.jsp.JspException: Missing message for key xx.xx.xx
Probable Causes
 这个key的值对没有在资源文件ApplicationResources.properties中定义。如果你使用eclipse时经常碰到这样的情况，当项目重新编译时，eclipse会自动将classes目录下的资源文件删除。
资源文件ApplicationResources.properties 不在classpath中 应将资源文件放到 WEB-INF/classes 目录下，当然要在struts-config.xml中定义) 
Cannot find message resources under key org.apache.struts.action.MESSAGE
异常
 Cannot find message resources under key org.apache.struts.action.MESSAGE
可能原因
 很显然，这个错误是发生在使用资源文件时，而Struts没有找到资源文件。 
Implicitly trying to use message resources that are not available (such as using empty html:options tag instead of specifying the options in its body -- this assumes options are specified in ApplicationResources.properties file)
XML parser issues -- too many, too few, incorrect/incompatible versions
Related Links
 Provide Struts with Resource Bundle
[http://threebit.net/tutorials/ejb/general/](http://threebit.net/tutorials/ejb/general/)
XML Parser Issues
[http://www.mail-archive.com/struts-user@jakarta.apache.org/msg15779.html](http://www.mail-archive.com/struts-user@jakarta.apache.org/msg15779.html)
No input attribute for mapping path /loginAction
错误
 No input attribute for mapping path /xxxxAction
可能原因e
 No input attribute in action mapping in struts-config.xml file for the action with the name specified in the error message. An input attribute is not required if form validation is not performed (either because the validate attribute is set to false or because the validation method in the relevant form class is not implemented. The input attribute specifies the page leading to this action because that page is used to display error messages from the form validation.
Related Links

Strange Output Characters
错误
 Strange and seemingly random characters in HTML and on screen, but not in original JSP or servlet.
可能原因
 混和使用Struts的html:form标记和标准的HTML标记不正确。
使用的编码样式在本页中不支持。
"Document contained no data" or no data rendered on page
错误
 "Document contained no data" in Netscape
No data rendered (completely empty) page in Microsoft Internet Explorer
可能原因
 使用一个Action的派生类而没有实现perform()方法或execute()方法。在Struts1.0中实现的是perform()方法，在Struts1.1中实现的是execute()方法，但Struts1.1向后兼容perform()方法。
但你使用Struts1.1创建一个Action的派生类，并且实现了execute()方法，而你在Struts1.0中运行的话，就会得到"Document contained no data" error message in Netscape or a completely empty (no HTML whatsoever) page rendered in Microsoft Internet Explorer.”的错误信息。
