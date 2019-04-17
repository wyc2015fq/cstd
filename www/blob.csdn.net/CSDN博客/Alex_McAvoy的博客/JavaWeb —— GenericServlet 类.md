# JavaWeb —— GenericServlet 类 - Alex_McAvoy的博客 - CSDN博客





2019年01月11日 22:22:18[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：22








# 【概述】

在编写一个 Servlet 对象时，必须实现 javax.servlet.Servlet 接口，在 Servlet 接口中包含 5 个方法，即创建一个 Servlet 对象时要实现这个 5 个方法，这样十分繁琐，而 javax.servlet.GenericServlet 类简化了此操作，实现了 Servlet 接口。

GenericServlet 类是一个抽象类，分别实现了 Servlet 接口与 ServletConfig 接口，实现了除 service() 之外的其他方法，在创建 Servlet 对象时，可以继承 GenericServlet 类，只需实现 service()方法，从而简化代码。

```java
public abstract class Test imprements GenericServlet{
    public void service(ServletRequest request, ServletResponse response) throws ServletException, IOException {
        ...
    }
}
```

#  【结构】

GenericServlet 抽象类继承了 Servlet 接口与 ServletConfig 接口，内部实现了除 service() 方法外的其他方法，即：实现了 Servlet 接口的 init() 方法、destroy() 方法、getServletConfig() 方法、getServletInfo() 方法以及 ServletConfig 接口的 getInitParameter() 方法、getInitParameterNames() 方法、getServletName() 方法、getServletContext() 方法。

其内部结构实现基本如下：

```java
public abstract class GenericServlet implements Servlet,ServletConfig,java.io.Serializable{

    private transient ServletConfig servletConfig;//当前类的servletConfig

    GenericServlet(){}//构造器
	
    /*
        Servlet接口的方法
     */
    @Override
    public void destroy() {}//销毁

    @Override
    public ServletConfig getServletConfig() {//返回当前对象的servletConfig
        return servletConfig;
    }

    @Override
    public String getServletInfo() {
        return null;
    }

    @Override
    public void init(ServletConfig servletConfig) throws ServletException {//初始化
        this.servletConfig=servletConfig;//令当前类的servletConfig等于Servlet的servletConfig
        this.init();//调用初始化方法
    }
	
    /*
        为防止继承GenericServlet类重写init()方法时
        忘记写super.init()而出现的空指针异常
        其并非是Servlet生命周期的方法
     */
    public void init() throws ServletException{}

    @Override
    abstract public void service(ServletRequest req, ServletResponse res) throws ServletException, IOException;//service() 抽象方法


    /* 
        ServiceConfig接口的方法
     */
    @Override
    public String getInitParameter(String name) {//返回参数name的值
        return servletConfig.getInitParameter(name);
    }

    @Override
    public Enumeration<String> getInitParameterNames() {//返回参数名的Enumeration对象
        return servletConfig.getInitParameterNames();
    }

    @Override
    public ServletContext getServletContext() {//返回当前Servlet的ServletContext
        return servletConfig.getServletContext();
    }

    @Override
    public String getServletName() {//返回当前Servlet的名字
        return servletConfig.getServletName();
    }
}
```





