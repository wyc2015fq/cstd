# Servlet 初步 - LC900730的博客 - CSDN博客
2017年07月25日 09:48:15[lc900730](https://me.csdn.net/LC900730)阅读数：149标签：[servlet](https://so.csdn.net/so/search/s.do?q=servlet&t=blog)
个人分类：[java](https://blog.csdn.net/LC900730/article/category/7027036)
## Servlet学习
```
1.如何写一个Servlet
    1.编写普通Java类，继承HttpServlet类
    2.重写doGet和doPost方法
    3.把这个Servlet交给Tomcat运行
    3.1：servlet字节码拷贝到WEB-INF/classes目录
    3.2：在web.xml中进行配置
```
    tomcat启动的时候，首先加载webapps中的每个web应用的web.xml配置文件； 
```
1.在day10的web.xml中查找是否有匹配的uri-pattern内容。
2.如果找到匹配的url-pattern，则使用当前的servlet-name的名称到web.xml文件中查询是否有相同的名称的servlet配置；
3.如果找到，则取出对应的servlet配置信息中的servlet-class内容：字符串
    gz.itcast.a_servlet.firstServlet
```
通过反射： 
    a.构造FirstServlet的对象; 
    b.然后调用FirstServlet里面的方法
## Servlet映射路径
1.url-pattern要么以/开头，要么以*开头； 
2.不能同时使用两种模糊匹配 
3.当有输入的URL有多个servlet同时被匹配的情况下： 
    3.1精确匹配优先； 
    3.2以后缀名结尾的模糊匹配优先级最低
URL输入[http://localhost:8080/day10/index.html](http://localhost:8080/day10/index.html)
1.到当前day10应用下web.xml文件中查找有匹配的uri-pattern 
2.如果没有匹配的uri-pattern，那么会交给tomcat的内置DefaultServlet处理 
3.DefaultServlet程序到day10应用中的根目录下查找是否有一个名称为index.html的静态文件 
4.如果找到该文件，则读取该文件的内容，返回给浏览器； 
5.如果找不到，则返回404
结论：先找动态资源，在找静态资源。
## Servlet的生命周期
```
Servlet类对象什么时候创建？创建时候调用什么方法？什么时候销毁？
Servlet程序的声明周期是tomcat服务器控制的。
```
### Servlet重要的生命周期
构造方法：创建servlet对象的时候调用。默认情况下，第一次访问servlet的时候创建servlet对象只调用1次，证明servlet对象在tomcat是单例的。 
init方法：创建完servlet对象的时候调用，只调用1次； 
service方法：每次发出请求时候调用，调用n次。 
destroy方法：销毁的时候
改变servlet创建对象的时机，提前到加载web应用的时候！！！ 
如果让servlet对象自动加载，那么在servlet的配置信息中，加上 
1 
注意整数值越大，创建优先级越低。
### Servlet中的有参数init()和无参数init()
GenericServlet中的有参init方法：
```java
public void init(ServletConfig config) throws ServletException{
        this.config=config;
        this.init();
    }
    public void init(){
    }
```
//因此我们不覆盖有参数的init方法，只需要重写无参数的init方法，那么有参数的init方法会自动帮我们调用。
### servlet对象在tomcat是单实例多线程
```
一个用户一个线程
```
```
Thread1
    public void run(){
            first.service(request,response);        
    }
    Thread2
    public void run(){
            first.service(request,response);        
    }
//线程1和2都是同一个对象，但是2个封装的request和response对象不相同
```
### servlet线程安全问题
servlet是有线程安全问题的。如果多个线程同时访问了servlet对象的共享数据(成员变量)
解决线程安全问题
使用到共享数据的代码块进行同步，使用synchronized(response)
在servlet类中尽量不要成员变量，如果确实要使用，必须同步。而且尽量缩小同步代码块的范围(哪里使用到了成员变量，就同步哪里！！)，以避免因为同步而导致开发效率降低。
## Servlet重要对象
```
HttpServletRequest对象：请求对象，获取请求信息；
HttpServletResponse对象：响应对象，设置响应对象；
ServletConfig对象：servlet配置对象
ServletContext对象：servlet的上下文对象
```
### ServletConfig对象
```
作用：
    ServletConfig对象：主要是用于加载servlet配置参数；
创建时间：
    在创建完servlet对象之后，在调用init方法之前创建；
得到对象：
    直接从有参数的init方法中得到
```
### servlet初始化参数
```
tomcat启动会加载web.xml中，那么配置文件也会被加载，封装到ServletConfig对象中，通过有参数的init方法传递过来。
```
```java
/**
    *ServletConfig的 Api
java.lang.String getInitParameter(java.lang.String name)
java.util.Enumeration getInitParameterNames()获取所有参数
ServletContext getServletContext()得到Servlet上下文对象
java.lang.String getServletName() 得到servlet名字
    */
    public class ConfigDemo extends HttpServlet{
        private ServletConfig config=null;
        //第一种方法拿到config 
        public void init(ServletConfig config) throws Exception{
            this.config=config;
        }
        //2.第二种方法拿到config
        String path=        this.getServletConfig().getInitParameter("path");
        public void doGet(XXX){
        }
//获取所有参数
Iteration<String> enums=this.getServletConfig.getInitParam
    }
```
## ServletContext对象
```
servlet的上下文对象，表示一个web应用中的所有配置信息。
作用：
```
|String  getContextPath()|拿到当前web 应用的路径，即项目名称，如/day10|
|----|----|
|getInitParameter(String name)|web应用的初始参数(上面是servlet的初始化参数)|
|用在重定向中用的最多|response.sendRedirect(“/day10/index.html”),如果项目名称发生变化那么response.sendRedirect(this.getContext()+”/index.html”)|
|Enumeration getInitParameter(String name)|在所有servlet的外面，在web-app的里面，如|
```
<context-param>
            <param-name>AAA</param-name>     
            <param-value>AAA value</param-name>   
    </context-param>
```
|任意的Servlet都可以用|ServletContext context=this.getServletContext();    context.getInitParameterName(“AAA”);|
```
getInitParameter(String name): web应用的初始参数(上面是servlet的初始化参数)
Enumeration getInitParameter(String name)
void setAttribute(java.lang.String name,java.lang.Object object) 域对象有关的方法
getAttribute
removeAttribute
```
RequestDispatcher getRequestDispatcher(String path)  
转发，类似于重定向
getRealPath ：得到web应用的资源文件 
InputStream getResourceAsStream ：
得到对象： 
从ServletConfig对象的getServletContext方法中得到。
Sun公司设计： 
1.创建ServletContext对象 
ServletContext context=new ServletContext();
2.创建ServletConfig对象 
    ServletConfig config=new ServletConfig(); 
    config.setServletContext(context);
class ServletConfig{ 
    public ServletContext getServletContext(){ 
            return context; 
    } 
} 
public void init(ServletConfig config){ 
        得到ServletConfig对象 
       //从ServletConfig对象中得到ServletContext对象 
        ServletContext context=config.getServletContext(); 
}
一个web应用中可以有多个servlet，一个servlet就可以有一个servletConfig对象，而一个web应用只有一个ServletContext。
## 域对象有关的方法
域对象：作用是保存数据，获取数据。可以在不同动态资源之间共享数据 
Servlet1                    Servlet2 
name=eric                XXX 
方法1： 
    传参数：response。sendRedirect(“/Servlet2?name=eric”) 
    接收：request.getParameter(“name”); 
方案2：可以用 域对象，可以共享任何类型的数据
ServletContext就是一个域对象。
### 在2个Servlet中传递数据
    每个应用中都只有一个ServletContext对象，无论哪个Servlet对象调用getServletContext()得到的都是同一个“账户” 
```
public class ContextDemo3{
        public void doGet(){
            //1.得到域对象
            ServletContext context
            =this.getServletContext();
            //2.把数据保存到域对象中
            context.setAttribute("name","eric");
        }
    }
//ContextDemo4中拿到数据
    public class ContextDemo4{
        public void doGet(){
            //1.得到域对象
            ServletContext context
            =this.getServletContext();
            //2.把数据保存到域对象中
            String name=(String)context.setAttribute("name");
            System.out.println("name="+name);
        }
    }
//还可以传递对象
//ContextDemo3中
context.setAttribute("student",new Student("jacky",20));
//ContextDemo4中
Student stu=(Student)context.getAttribute("student");
System.out.println(stu);
```
## 转发与重定向
//转发 
RequestDispatcher rd= 
this.getServletContext().getRequestDispatcher(“/index.html”);
rd.forward(request,response);
//重定向 
response.sendRedirect(“day10/index.html”); 
this.getServletContext()) 
区别： 
    1.转发地址不会发生变化，重定向地址会发生变化 
    2.forward转发不能转发到当前web应用以外的资源，而重定向的可以(“/day09/adv.html”); 
    3.转发可以将数据保存到域对象，而重定向则不行，重定向的request和response不是一个对象 
转发的原理： 
//ForwardDemo1 
rd.forward(request,response)—->
//GetDataServlet中 
因此，如果要使用request域对象进行数据共享，只能使用转发技术 
重定向的话返回302+location，然后在访问GetDataServlet![这里写图片描述](https://img-blog.csdn.net/20170724234017081?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
