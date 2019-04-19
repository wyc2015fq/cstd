# java中taglib - LC900730的博客 - CSDN博客
2017年09月09日 20:55:06[lc900730](https://me.csdn.net/LC900730)阅读数：342
- 打包整个java应用
jar cvf c:\chapter03\helloapp.war  *.*    :将helloapp目录下以及子目录下所有文件打包为helloapp.war文件，存放在c:\chapter03下
- 把helloapp.war文件复制到
CATALINA_HOME/webapps目录下
- 启动Tomcat服务器：Tomcat服务器会在启动时候，把所有WAR文件自动展开为开放式目录结构。如helloapp.war展开为helloapp文件夹
## 1.创建自定义标签
### 1.1编写用于处理hello标签的类
```java
//HelloTag.java
package mypack
import 。。。
public class HelloTag extends TagSupport{
    public int doEndTag() throws JspException{
        try{
            pageContext.getOut().print("Hello");
        }catch(Exception e){
            throw new JspTagException(e.getMessage());
        }
        return EVAL_PAGE;
    }
}
```
### 1.2创建一个TLD(Tag Library Descriptor)文件
```xml
//mytaglib.tld
<?xml version="1.0" encoding="UTF-8"?>
<!-- <!DOCTYPE taglib
    PUBLIC "-//Sun Microsystems,Inc.//DTD JSP Tag Library 1.1//EN"
    "http://java.sun.com/j2ee/dtds/web-jsptaglibrary_1_1.dtd"> -->
<taglib>
<tlibversion>1.1</tlibversion>
<jspversion>2.1</jspversion>
<short-name>mytaglib</short-name>
<uri>/mytaglib</uri>
<tag>
    <name>hello</name>
    <tagclass>mypack.HelloTag</tagclass>
    <bodycontent>empty</bodycontent>
    <info>Just say hello</info>
</tag>
</taglib>
```
### 1.3在web.xml文件中配置taglib元素
```
//web.xml
//在2.4版本以后，需要放置在jsp-config标签里面
...
<jsp-config>
<taglib>
    <taglib-uri>/mytaglib</taglib-uri>
    <taglib-location>/WEB-INF/mytaglib.tld</taglib-location>
</taglib>
</jsp-config>
```
### 1.4在jsp文件中使用hello标签
```xml
<%@ taglib uri="/mytaglib" prefix="mm" %>
    。。。
<b><mm:hello/>:<%=request.getAttribute("USER")%></b>
```
在以上taglib指令中,prefix属性用来为mytaglib标签库指定一个前缀“mm”。接下来，hello.jsp就可以使用`<mm:hello/>` 的形式来使用hello标签
当客户端请求访问hello.jsp时，Servlet容器按照如下步骤处理hello.jsp中的标签““ 的前缀为“mm”,与hello.jsp中的prefix=”mm”相互匹配，由此得知hello标签来自URL为”/mytaglib“
- 2.在web.xml文件中对URI为”/mytaglib“的标签库配置：
    tagliburi:/mytaglib
    tagliblocation:/WEB-INF/mytaglib.tld
    由此得知：URI为”/mytaglib“的标签库的TLD文件为WEB_INF/mytaglib.tld
