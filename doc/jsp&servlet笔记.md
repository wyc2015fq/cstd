# jsp&servlet笔记





# （一）

 									

### 一、什么是容器？

对于servlet来说，它是受控于容器的。当web服务器应用得到一个指向某servlet的请求时，服务器将会先交给部署的容器，再由容器提供请求和响应给servlet。

### 二、什么又是servlet？

servlet是被称为小服务程序或者服务连接器。能够独立于平台和协议，交互式浏览和生成动态页面。

### 三、什么又是jsp？

jsp全称是java服务器页面是在HTML页面中插入java代码。本质上是生成了一个个的servlet来执行，达到动态页面的效果。

容器可以把jsp翻译成真正的java代码。

### 四、部署

部署采用tomcat+IDEA来完成。

在new一个project后把web application打钩即可。

### 五、运行









# （二）

 				

### 一、

一个servlet在项目中可以有三个名字：

1、存在于jsp页面的url

2、部署在DD中的中间名字

3、实际的class文件

 

在DD中一般为：

```xml
<servlet>
    <servlet-name>InnerServletName1</servlet-name>
    <servlet-class>practice.myServlet1</servlet-class>
</servlet>
<servlet-mapping>
    <servlet-name>InnerServletName1</servlet-name>
    <url-pattern>/handle1.do</url-pattern>
</servlet-mapping>
```

其中url-pattern就是1中名字，servlet-name是2中的，servlet-class是3中的名字。

然后可以在src文件夹中创建包practice，创建一个java class名为myservlet1

在jsp页面中建立一个`<a>`元素href指向handle1.do。点击时即可向对应的servlet发出GET请求。

### 二、如何编写Servlet类

```java
public class myServlet1 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
     resp.setContentType("text/html");
     PrintWriter pw=resp.getWriter();
     for(int i=0;i<10;i++){
         pw.println("<h2>hello ! =>"+(i+1)+"</h2>");
     }
     pw.flush();
    }
 
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        resp.setContentType("text/html");
        PrintWriter out=resp.getWriter();
        out.println("<h1>Ok I get it.</h1>");
        out.flush();
 
    }
}
```

像这样，覆盖两个方法即可（一般情况下只需要覆盖两个）。

### 三、jsp中使用java代码

比如：

```jsp
<p>Current time:<% Date d=new Date();out.println(d.getHours()+":"+d.getMinutes());%></p>
```

插入到jsp页面时将会产生当前的时间。

所以java代码实际上就是写在<%...%>中的。

### 四、把请求转发给另一个页面

调用请求分派器：

```java
req.getRequestDispatcher("result.jsp").forward(req,resp);
```

### 五、servlet的三个方法

 一个servlet类将存在三个时刻：

init（）；service（）；doget/dopost；

第一个方法是在初始时调用，比如在init方法中连接数据库；

service基本不大可能被覆盖。

get/post请求分别对应两种方法。

而且每一个请求都在一个单独的线程中运行，但一个servlet类仅有一个实例对象。

### 六、servlet对象里面的对象

1、servletConfig对象。在部署时就传递信息，这个信息在DD中，用来访问servletContext。

2、servletContext实际上是每一个web-app都有的参数。

### 七、参数传递

参数传递可以用POST方法或者GET方法。但是提交表单时一般是相当于一个hash表传递给servlet。

然后在servle中使用getParameter方法获取参数值。

如果是复选框的参数（一次性可以选择多个值）就要用getParamterValues来得到参数的数组。

### 八、用响应传输一个文件

一般来说，需要在respose中设置MIME类型：

text/html

application/pdf

video/quicktime

application/java

image/jpeg

application/jar

application/x-zip

application/octet-stream

接下来以最简单的一个图片文件来做示范：

在web目录下新建一个pictures目录，放入一张图片

在对应的方法写下：

```java
resp.setContentType("image/jpeg");//你想传送的文件类型
ServletContext context=getServletContext();
InputStream is=context.getResourceAsStream("/pictures/tb.jpg");//文件夹里的一个图片
int read=0;
byte[] bytes=new byte[1024];
OutputStream os=resp.getOutputStream();
while ((read=is.read(bytes))!=-1){//io操作
    os.write(bytes,0,read);
}
os.flush();
os.close();//刷新并关闭输出流
```

### 九、请求重定向

```java
resp.sendRedirect("result.jsp");//发送重定向到指定页面或者网址
```

 







# （三）

 

### 一、servlet的初始化参数

初始化参数是在DD文件（web.xml）中的一对映射：

```xml
<servlet>
    <servlet-name>InnerServletName1</servlet-name>
    <servlet-class>practice.myServlet1</servlet-class>
    <init-param>
    <param-name>param1</param-name>
    <param-value>abcdef</param-value>
    </init-param>
</servlet>
```

而且放在哪一个servlet标签中就只能这个servlet使用。

在servlet的方法中可以用

```java
getInitParameter("param1")
```

来获取这一个参数值。

其初始化过程是：在建立servlet类时先读取DD文件，并且配置到servletConfig中，形成了多个键值对。

上面的一行实际上是：

```java
getServletConfig().getInitParameter("param1")
```

因为是写在配置中的，servletconfig对此servlet有效。

还有一种是

```
getServletContext().getInitParameter("param2")
```

这种方法从servlet的上下文获取初始化参数：

```xml
<servlet>
    <servlet-name>InnerServletName1</servlet-name>
    <servlet-class>practice.myServlet1</servlet-class>
    <init-param>
    <param-name>param1</param-name>
    <param-value>abcdef</param-value>
    </init-param>
</servlet>
<servlet-mapping>
    <servlet-name>InnerServletName1</servlet-name>
    <url-pattern>/handle1.do</url-pattern>
</servlet-mapping>
<context-param>
    <param-name>param2</param-name>
    <param-value>xyz</param-value>
</context-param>
```

可以看到context-param是和servlet同级的，也就是说可以被全部servlet访问到。

那么在servlet中为context设置的属性：

```java
getServletContext().setAttribute("six","666");
```

在jsp中如何调用呢？

一般来说是：

```java
getServletConfig().getServletContext().getAttribute("six")
```

 

### 二、上下文监听器

监听器是一个实现ServletContextListener的类

一般需要覆盖init和destory两种方法。

而且在使用前需要在DD文件中先放入监听器标记

```xml
<listener>
    <listener-class>Listeners.myContextListener</listener-class>
</listener>
```

监听器的作用就是当设置了context参数时、撤销了context参数时，给予一些动作，比如修改，格式化等等。

 





# （四）

 

### 一、八种监听器

上一节笔记简单介绍了上下文属性监听器（ServletContextAttributeListener）,下面依次介绍另外七种监听器。

1、HttpSessionListener是会话监听器，当一个会话被创建（sessionCreated）或者被撤销（sessionDestroyed）都可以触发这个监听器。用来跟踪并发的用户。

2、ServletRequestListener是请求监听器，当一个请求被创建或者撤销时触发。

3、ServletRequestAttributeListener是请求属性监听器。这个作用大致相当于上下文属性监听器，只不过监听对象时请求（HttpRequest）。

4、HttpSessionBindingListener是会话绑定监听器，不同于第1个监听器，这个监听器监听的是“属性”，假设一个普通的java类实现了这个接口，那么当它作为属性加入到一个会话时，将会触发valueBound方法，反之将会触发valueUnbound方法。

5、HttpSessionAttributeListener是对会话的属性监听。（注意与1,4的区别）

6,、ServletContextListener上下文监听器，是否创建/撤销了一个上下文。

7、HttpSessionActivationListener对会话的动作进行监听，当会话对象迁移到另一个jvm时触发。

### 二、上下文作用域不是线程安全的。

我们知道，对上下文的属性是这样使用的：

```
getServletContext().setAttribute("six","666");
```

或者

```
getServletContext().getAttribute("six");
```

能够像一个哈希表一样使用这些键值对。

但是因为上下文的作用域是整个webapp，所以任何一个servlet都有权利对这些值进行修改，所以，并不是线程安全的。

一般需要做的是对上下文加锁，什么意思呢？就是synchronize！

```
synchronized (getServletContext()){
    //...
}
```

其实使用的是java多线程里面的对象锁原理，这样在大括号里面操作上下文就是线程安全的了。

同样地，HttpSession也不是线程安全的，同步方法是和上下文一样的对象锁。

但是一个请求的属性就是线程安全的了，毕竟一个线程只能操作唯一的一个request对象。

### 三、请求分派器的两种获得方法

可以从request和context两个地方获得请求分派器。

```
req.getRequestDispatcher("result.jsp").forward(req,resp);
getServletContext().getRequestDispatcher("/result.jsp").forward(req,resp);
```

以上两种方法的区别就是“从哪里寻找requst.jsp这个页面”。

从请求是从请求的相对位置查找，但是上下文基本上会从目录的根开始查找。

### 四、使用cookie标记客户

我们都知道在一些web时，登录都是有时效的，比如登录一次，过了半小时后就不可以继续使用登录权限了，需要再次登录。

在Servlet中是借助cookie来完成的，编写的基本步骤是：

1、实现一个servlet类处理这个用户的登录。

2、当用户填写完毕注册表单（post）时，如果检查用户名/密码正确就addCookie给response。这样用户就持有一个有通行证一样的coolie了。

3、当用户要访问需要权限的页面时，就在servlet中用一个循环来检查cookie。

ps：cookie利用setMaxAge来设置最大存活时间，单位是秒。

 



# （五）标记以及用法

 

这篇笔记主要学习jsp中的标记以及用法。

### 一、导入包

回忆一下，jsp指的是java服务器语言，最终是要成为一个servlet类对象的。所以在jsp中<% ... %>里面可以插入java 代码来渲染页面。而有些java类需要导入才可以使用，那么就需要

```jsp
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
```

插入一行：

```jsp
<%@ page contentType="text/html;charset=UTF-8" import="java.util.List,common.User"   language="java" %>
```

这样就可以在jsp中使用List和User两个类。

基本是要写在

<%@ page ....%>是一个“指令”。

### 二、简化out.println

我们知道out对象是一个隐式对象，可以在想插入一段输出时写在<% ...%>中，但是实际上有更简单的办法。

就是<%= ...%>等价于把省略号里的内容输出（或者调用toString再输出）

总结一些这几种标记，他们分别叫：

<%...%>   scriptlet

<%@ page ...%>指令

<%= %>表达式

### 三、声明一个变量

在jsp中我们可能想要一个类似于java静态变量的东西存在，比如一个页面点击了一次就加一。

那么我们需要把这个变量写进<%! ...%>中

### 四、完整的page指令

下表列出与Page指令相关的属性：

| **属性**           | **描述**                                            |
| ------------------ | --------------------------------------------------- |
| buffer             | 指定out对象使用缓冲区的大小                         |
| autoFlush          | 控制out对象的 缓存区                                |
| contentType        | 指定当前JSP页面的MIME类型和字符编码                 |
| errorPage          | 指定当JSP页面发生异常时需要转向的错误处理页面       |
| isErrorPage        | 指定当前页面是否可以作为另一个JSP页面的错误处理页面 |
| extends            | 指定servlet从哪一个类继承                           |
| import             | 导入要使用的Java类                                  |
| info               | 定义JSP页面的描述信息                               |
| isThreadSafe       | 指定对JSP页面的访问是否为线程安全                   |
| language           | 定义JSP页面所用的脚本语言，默认是Java               |
| session            | 指定JSP页面是否使用session                          |
| isELIgnored        | 指定是否执行EL表达式                                |
| isScriptingEnabled | 确定脚本元素能否被使用                              |

------

##  

### 五、taglib和include

taglib一般用来指定标记库：

```jsp
<%@ taglib uri="uri" prefix="prefixOfTag" %>
```

url是存放路径，prefix是前缀。

include可以插入另一个页面。

```jsp
<%@ include file="文件相对 url 地址" %>
```

注意是“插入”，就是在调用指令的地方把文件插进来，所以被插入文件不建议有<html><body>之类的标签。

### 六、利用jsp标准动作使用Bean对象。

首先介绍一下什么是Bean对象。我理解的Bean就是一组数据的集合。

比如：

```java
public class Boss {
    private String name;
    private double salary;
    public Boss(String name,double salary){
        this.name=name;
        this.salary=salary;
    }
    public Boss(){}
 
    public void setName(String name) {
        this.name = name;
    }
 
    public void setSalary(double salary) {
        this.salary = salary;
    }
 
    public String getName() {
        return name;
    }
 
    public double getSalary() {
        return salary;
    }
}
```

这样的一个java类。

但是有几点一定要注意

1、对弈一个属性，比如salary，一定要有get，set两种方法。

2、由于bean对象使用事先默认构造，所以一定要有默认构造器。

接下来就要用<jsp:useBean>来使用这些数据。也可以用jsp：set/getProprity来构造。

### 七、表达式语言（EL ，express language）

表达式语言形如${a.b.c}并且自动转换为string。

或者${a["b"]}这种语法，可以理解为哈希表的映射。

1、使用cookie值

```
${cookie.a.value}
```

2、使用初始化参数

```
${initParam.param3}
```

3、使用java函数

首先创建一个java类里面必须含有静态公共函数fun

然后在web.xml同级创建一个tld文件，用function来描述这个函数以及签名，并且设置这个文件的uri。

最后在jsp里面首先用taglib指明函数库，然后调用。

```
${f:stringMaker(10)}
```

### 八、使用JSTL

配置方法详见<https://blog.csdn.net/qq_34288630/article/details/80591095>

用法：<http://www.runoob.com/jsp/jsp-jstl.html>

 

