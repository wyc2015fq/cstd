# Struts2中使用FreeMarker充当表现层 - z69183787的专栏 - CSDN博客
2015年07月16日 12:03:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1707
**1,在java程序中使用FreeMarker**
FreeMarker的模板就是一个.ftl文本文件,在该文件中使用了一些FreeMarker的特别标记,这些标记会动态显示,或者控制程序输出,如下面的模板文件代码:
${name}，你好！${msg}
这里类似于${}的就是动态的内容,称作"插值".
为了使用FreeMarker来将数据模型中的值合并到模板文件中,可按如下步骤进行:
1,创建Configuration实例,该实例负责管理FreeMarker的模板加载路径,负责生成模板实例
2,使用Configuration实例来生成Template实例,同进需要指定使用的模板文件
3,填充数据模型,数据模型就是一个Map对象
4,调用Template实例的process方法完成合并.
下面是一个使用FreeMarker创建输出的Java程序,程序源代码如下:
import java.util.*;
import java.io.*;
import freemarker.template.*;
public class HelloFreeMarker
{
    private Configuration cfg; 
    public void init() throws Exception
{
        //初始化FreeMarker配置
        //创建一个Configuration实例
        cfg = new Configuration();
        //设置FreeMarker的模版文件位置
        cfg.setDirectoryForTemplateLoading(new File("templates"));
    }
public void process()throws Exception
{
        Map root = new HashMap();
        root.put("name", "FreeMarker!"); 
   root.put("msg" , "您已经完成了第一个FreeMarker的示例");
        Template t = cfg.getTemplate("test.ftl");
        t.process(root, new OutputStreamWriter(System.out));
}
public static void main(String[] args)throws Exception
{
   HelloFreeMarker hf = new HelloFreeMarker();
   hf.init();
   hf.process(); 
}
}
上面的代码创建了一个Map实例,这个Map将作为模板文件的数据模型,我们要使用FreeMarker必须导入freemarker.jar文件,**FreeMarker的官网是**[**http://freemarker.sourceforge.net/**](http://freemarker.sourceforge.net/)**,虽然FreeMarker可以在Java程序中使用,但大部分时候还是用来生成HTML页面.**
**2,在Web应用中使用FreeMarker**
在Web应用中使用FreeMarker跟在Java程序中使用并没有太大的区别.下面是是一个在Web中使用的例子,用来生成HTML页面的模板文件内容如下:
<html>
<head>
<title>FreeMarker的HelloWorld</title>
</head>
<body>
${message}
</body>
</html>
我们在Web应用中使用FreeMarker时,应该让Servlet来合并模板和数据,因此,Servlet负责创建Configuration实例,并负责合并模板和数据,下面是Servlet源代码:
import java.util.*;
import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;
import freemarker.template.*;
public class HelloServlet extends HttpServlet
{
    private Configuration cfg; 
    public void init() 
{
        //初始化FreeMarker配置
        //创建一个Configuration实例
        cfg = new Configuration();
        //设置FreeMarker的模版文件位置
        cfg.setServletContextForTemplateLoading(getServletContext(), "WEB-INF/templates");
    }
    public void service(HttpServletRequest request, HttpServletResponse response)
        throws ServletException, IOException
{        
   //建立数据模型
        Map root = new HashMap();
        root.put("message", "Hello FreeMarker!");     
        //取得模版文件
        Template t = cfg.getTemplate("test.ftl");        
        // 开始准备生成输出
        // - 使用模版文件的charset作为本页面的charset
        // - 使用text/html MIME-type
        response.setContentType("text/html; charset=" + t.getEncoding());
        Writer out = response.getWriter();
        //合并数据模型和模版，并将结果输出到out中
        try
   {
            t.process(root, out);
        } 
   catch (TemplateException e)
   {
            throw new ServletException("处理Template模版中出现错误", e);
        }
    }
}
可以看到这个Servlet类的代码与普通的Java程序中使用FreeMarker大致一样,区别有两个:1,设置FreeMarker加载模板的方法不一样,在Servlet中设置加载的方法是setServletContextForTemplateLoading,第一个参数是本web应用的ServletContext,第二个参数是模板文件的路径.;2,结果必须输出到HttpServletResponse中,才能被浏览器加载.
配置Servlet的web.xml文件中的代码如下:
<web-app>
<servlet>
   <servlet-name>hello</servlet-name>
   <servlet-class>lee.HelloServlet</servlet-class>
</servlet>
<servlet-mapping>
   <servlet-name>hello</servlet-name>
   <url-pattern>/hello</url-pattern>
</servlet-mapping>
</web-app>
**3,FreeMarker的模板文件**
从上面的例子可以看到FreeMarker的模板文件并不比HTML页面复杂多少,FreeMarker模板文件主要由如下4个部分组成:
1,文本:直接输出的部分
2,注释:<#-- ... -->格式部分,不会输出
3,插值:即${...}或#{...}格式的部分,将使用数据模型中的部分替代输出
4,FTL指令:FreeMarker指定,和HTML标记类似,名字前加#予以区分,不会输出
下面是一个FreeMarker模板的例子,包含了以上所说的4个部分
<html><br>
<head><br>
<title>Welcome!</title><br>
</head><br>
<body><br>
<#-- 注释部分 --><br>
<#-- 下面使用插值 -->
<h1>Welcome ${user} !</h1><br>
<p>We have these animals:<br>
<u1><br>
<#-- 使用FTL指令 -->
<#list animals as being><br>
   <li>${being.name} for ${being.price} Euros<br>
<#list><br>
<u1><br>
</body><br>
</html>
FreeMarker的语法知识将在下一篇文章中介绍
**4,使用FreeMarker作为Struts2的视图技术**
Struts2使用FreeMarker作为其默认的模板技术,因此Struts2对FreeMarker的支持非常良好,为了在Struts2中使用FreeMarker模板技术,只需要在struts.xml文件中进行简单配置即可.
**4.1在FreeMarker模板中使用Struts2标签**
在FreeMarker模板中使用标签毕竟不同于在JSP页面中使用标签,因为FreeMarker不支持使用taglib指令来导入标签库.但Struts2为了在FreeMarker模板中使用标签库提供了额外的支持.可以按照下面的步骤进行:
1,将系统所需要的标签库定义文件(.tld文件)复制到WEB-INF目录下,对于Struts2,则是复制struts2-core-2.0.11.2.jar中的META-INF/struts-tags.tld文件
2,在web.xml文件中启动JspSupportServlet.为了启动它,则应该在web.xml文件中增加如下配置片段:
<!-- 定义一个JspSupportServlet -->
<servlet>
   <servlet-name>JspSupportServlet</servlet-name>
   <servlet-class>org.apache.struts2.views.JspSupportServlet</servlet-class>
   <!-- 配置JspSupportServlet优先启动 -->
   <load-on-startup>1</load-on-startup>
</servlet>
3,在FreeMarker模板中使用assign指令导入标签库,导入标签库的代码如下:
<#-- 定义/WEB-INF/struts-tags.tld文件对应的标签库前缀为s -->
<#assign s=JspTaglibs["/WEB-INF/struts-tags.tld"] />
4,在FreeMarker模板中增加了标签库的定义之后就可以使用了,下面是在FreeMarker模板中使用标签的代码:
<#-- 在FreeMarker中使用Struts2标签 -->
<@s.form action="Login.action">
<@s.textfield name="username" label="用户名"/>
<@s.textfield name="password" label="密码"/>
<@s.submit value="提交"/>
</@s.form>
如果是在JSP中使用Struts2标签,则格式如下:
<s:form action="Login.action">
<s:textfield name="username" label="用户名"/>
<s:textfield name="password" label="密码"/>
<s:submit value="提交"/>
</s:form>
值得注意的是,我们不能通过浏览器直接访问.ftl文件,否则将看到该模板页面的源代码,而不是我们想要的生成的HTML页面.这是因为Web容器默认不会处理FreeMarker模板页面.
在Struts2框架下,Struts2框架充当了之前的Servlet角色,只要浏览器的请求经过Struts2处理后,Struts2都会自动加载FreeMarker模板,并使用数据模型来填充该模板,再将最后的HTML页面输出给客户端.为了让所有的用户请求都经过Struts2的处理,这里将所有的FreeMarker模板放在WEB-INF/ftl目录下.(WEB-INF目录下的所有文件客户端都是不能直接访问的),因为浏览器无法访问.ftl文件了,因此我们在struts.xml配置文件中增加如下配置:
   <!-- 配置匹配所有请求的Action -->
        <action name="*">
            <result type="freemarker">/WEB-INF/ftl/{1}.ftl</result>
        </action>
国为增加了上面的配置,在浏览器中请求login.action的时候,该action会被转向login.ftl模板.此时就可以看到返回的HTML页面.
**4.2 使用FreeMarker的Result**
从前面我们可以看到,使用FreeMarker与使用JSP作为输出文件并没有太大区别,下面来看看刚才的应用中Action中的代码:
import com.opensymphony.xwork2.Action;
import com.opensymphony.xwork2.ActionContext;
public class LoginAction implements Action
{
    private String username;
    private String password;
    public String getUsername()
{
        return username;
    }
    public void setUsername(String username)
{
        this.username = username;
    }
    public String getPassword()
{
        return password;
    }
    public void setPassword(String password)
{
        this.password = password;
    }
public String execute() throws Exception
{
        if (getUsername().equals("scott")
                && getPassword().equals("tiger") )
   {
    ActionContext.getContext().getSession().put("user" , getUsername());
            return SUCCESS;
        }
   else
   {
            return ERROR;
        }
    }
}
此Action在struts.xml文件中的配置如下:
<action name="Login" class="lee.LoginAction">
<result name="error" type="freemarker">/WEB-INF/ftl/error.ftl</result>
<result name="success" type="freemarker">/WEB-INF/ftl/welcome.ftl</result>        
</action>
返回的成功页面代码如下:
<html>
    <head>
        <title>成功页面</title>
    </head>
    <body>
        欢迎，${user},您已经登录!<br>
<a href="GetBooks.action">查看作者李刚已出版的图书</a>
    </body>
</html>
可以看到这个成功页面会将user变量中的内容读出并显示,实际上,该变量值将可以从多个地方取得,Struts2解析FreeMarker模板中变量的顺序如下:
1,FreeMarker模板内建的变量;
2,ValueStack中的变量;
3,ActionContext中的变量;
4,HttpServletRequest中的属性;
5,HttpSession中的属性;
6,ServletContext范围的属性.
FreeMarker模板的内建变量有如下几个:
1,stack:代表ValueStack本身,可通过如下方式来访问其中的变量"${stack.findString('ognl expr')}"
2,action:代表刚刚执行过的Action实例
3,response:代表HpptServletResponse实例
4,res:代表HpptServletResponse实例
5,request:代表HpptServletRequest实例
6,req:代表HpptServletRequest实例
7,session:代表HpptSession实例
8,application:代表ServletContext实例
9,base:代表用户请求的上下文路径.
**4.3 FreeMarker中访问Servlet/JSP范围对象**
Struts2还为在FreeMarker模板中访问Servlet/JSP范围对象提供了支持,通过这种支持可以直接访问这些变量.
访问application范围内的属性代码如下:
<#if Application.attributeName?exists>
${Application.attributeName}
</#if>
也可以通过使用Struts2标签来输出,代码如下:
<@s.property value="${#application.attributeName}" />
访问session范围内的属性如下:
<#if Session.attributeName?exists>
${Session.attributeName}
</#if>
或
<@s.property value="${#session.attributeName}" />
访问request范围内的属性如下:
<#if Request.attributeName?exists>
${Request.attributeName}
</#if>
或
<@s.property value="${#request.attributeName}" />
对于request,还可以访问request的请求参数,如下:
<#if Parameter.parameterName?exists>
${Parameter.parameterName}
</#if>
或
<@s.property value="${#parameter.parameterName}" />
访问ValueStack上下文参数,代码如下:
${stack.findValue('#myContextParam')}
或
<@s.property value="${#myContextParam}" />
