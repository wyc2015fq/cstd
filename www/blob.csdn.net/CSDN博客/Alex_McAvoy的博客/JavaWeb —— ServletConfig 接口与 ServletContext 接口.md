# JavaWeb —— ServletConfig 接口与 ServletContext 接口 - Alex_McAvoy的博客 - CSDN博客





2018年12月18日 21:27:36[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：34








# 【ServletConfig 接口】

ServletConfig 接口位于 javax.servlet 包中，封装了 Servlet 的配置信息，还可以获取 ServletContext 对象，其在 Servlet 初始化期间被传递，每一个 Servlet 对象有且仅有一个 ServletConfig 对象。

## 1.接口方法
- String getInitParameter(String name)：返回 String 类型名为 name 的初始化参数值
- Enumeration getInitParameterNames()：获取初始化参数名组成的 Enumeration 对象
- String getServletName()：返回 Servlet 对象实例名
- ServletContext getServletContext()：获取 ServletContext 对象的引用

getInitParameter() 方法与 getInitParameterNames() 方法使用前需要在 web.xml 中进行初始化参数配置。

## 2.实例

1）web.xml 中的配置

```
<servlet>
  <servlet-name>hello</servlet-name>
  <servlet-class>servlet.Test</servlet-class>
 
  <init-param>
    <param-name>user</param-name>
    <param-value>root</param-value>
  </init-param>
 
  <init-param>
    <param-name>password</param-name>
    <param-value>123456</param-value>
  </init-param>
</servlet>

<servlet-mapping>
  <servlet-name>hello</servlet-name>
  <url-pattern>/servlet/Test</url-pattern>
</servlet-mapping>
```

2）在 Server 类中 init 方法中测试 ServletConfig 对象

```java
public void init(ServletConfig config) throws ServletException{
    String user=config.getInitParameter("user");  //获取user参数的值
    System.out.println("user："+user);

    Enumeration<String> names=config.getInitParameterNames();  //获取所有参数名的集合
    while(names.hasMoreElements()){
        String name=names.nextElements();  //获取每个参数名
        String value=config.getInitParameter(name);  //获取对应参数值
        System.out.println("name:"+name+"  value:"+value);
    }

    String servletName=servletConfig.getServletName();  //获取Servlet实例名
    System.out.println(servletName);
}
```

3）运行结果

```
运行结果：
user：root
name：user  value：root
name：password  value：123456
hello
```

# 【ServletContext 接口】

ServletContext 被称为 servlet 上下文，是 Servlet 容器为每个 Web 应用程序创建的唯一对象，这个对象被包含在 ServletConfig 对象中，其在服务器启动时创建，服务器关闭后销毁。

ServletContext 全局唯一，且工程内部的所有 servlet 共享这个对象，即数据全局共享，可以通过 getServletContext() 方法来获取当前工程的 ServletContext 对象。

由于一个 Web 应用程序中所有的 Servlet 都共享一个 ServletContext 对象，所以 ServletContext 对象又被称为 application 对象。

## 1.读取全局配置参数

通过 ServletContext 对象可以读取 Web 应用的初始化参数（全局参数），其有两个方法：
- **String getInitParameter(String name)：**根据指定的 name 获取参数值
- **Enumeration getInitParameterNames()：**获取所有参数名称列表

全局参数可以为所有的 Servlet 通过 ServletContext 来获取，而 Servlet 的初始化参数只能用相应的 Servlet 的 ServletConfig 来获取。

实现步骤：

1）在 web.xml 中配置全局参数

```
<context-param>
  <param-name>param1</param-name>
  <param-value>value1</param-value>
</context-param>
<context-param>
 <param-name>param2</param-name>
 <param-value>value2</param-value>
</context-param>
```

2）在动态资源 servlet 里面使用 servletContext 读取全局参数代码

```java
public void init(ServletConfig config) throws ServletException{
    ServletContext context=config.getServletContext();

    String parame1=context.getInitParameter("parame1");  //获取user参数的值
    System.out.println("parame1："+parame1);

    Enumeration<String> names=context.getInitParameterNames();  //获取所有参数名的集合
    while(names.hasMoreElements()){
        String name=names.nextElements();  //获取每个参数名
        String value=context.getInitParameter(name);  //获取对应参数值
        System.out.println("name:"+name+"  value:"+value);
    }
}
```

3）运行结果

```
运行结果
parame1：value1
name：parame1  value：value1
name：parame2  value：value2
```

## 2.获取 Web 应用的某一个文件的绝对路径

通过 ServletContext 对象，可以根据相对路径来搜索当前工程目录下的资源文件
- **String getRealPath(String path)**：根据相对路径 path 来获取服务器上资源的绝对路径（不同于部署资源时的绝对路径）

```java
public void init(ServletConfig config) throws ServletException{
    ServletContext context=config.getServletContext();
    
    String relativePath="/test.txt";//资源相对路径
    String realPath=config.getRealPath(relativePath);//资源绝对路径
}
```

## 3.获取 Web 应用工程名

通过 ServletContext 对象，可通过** getContextPath() **方法来获取当前工程名。

```java
public void init(ServletConfig config) throws ServletException{
    ServletContext context=config.getServletContext();
    //获取工程名
    String name=config.getContextPath();
}
```

## 4.获取 RequestDispatcher 对象

通过 ServletContext 对象，可以获取用于请求和转发的 RequestDispatcher 对象
- RequestDispatcher getRequestDispatcher(String path)：根据以 " / " 开头的路径获取 RequestDispatcher 对象
- String getRequestDispatcher(String path)：根据 path 获取 RequestDispatcher 对象名

```java
protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {		
    String path="testServlet";//要转发的地址
    RequestDispatcher requestDispatcher=request.getRequestDispatcher("/"+path);//获取requestDispatcher对象并填入地址
    requestDispatcher.forward(request, response);//请求转发
}
```



