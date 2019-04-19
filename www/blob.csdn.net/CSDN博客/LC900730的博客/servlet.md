# servlet - LC900730的博客 - CSDN博客
2017年08月01日 09:28:29[lc900730](https://me.csdn.net/LC900730)阅读数：93标签：[servlet](https://so.csdn.net/so/search/s.do?q=servlet&t=blog)
个人分类：[java](https://blog.csdn.net/LC900730/article/category/7027036)
|获取参数|this.getServletConfig().getInitParameters|
|----|----|
|返回值是Enumeration 《String》enums| |
|while(enums.hasMoreElements(){String paramName=enums.nextElement();| |
|String paramValue=this.getSevletConfig().getInitParameter(paramName)| |
```java
//1.tomcat服务器把这些参数会在加载web应用的时候，封装到Servl
etConfig对象中
2、tomcat服务器调用init方法传入ServletConfig对象
public class GenericServlet{
    private transient ServletConfig config;
    public void init(ServletConfig config) throws ServletException{
        this.config=config;
    }
    //在其他Servlet中可以this.getServletConfig中拿到
    public ServletConfig getServletConfig(){
        return config;
    } 
}
```
加载web应用的时候就会创建ServletContext对象； 
从ServletConfig中得到，因为所有servlet都共用一个ServletContext对象 
    path 
    e:/a.txt 
String path=config.getInitParameter(“path”); 
File file=new File(path); 
BufferedReader br=new BufferedReader(new FileReader(file));
```java
public class HelloFilter implements Filter{
        public HelloFilter(){ 
            System.out.println("1.创建过滤器实例");
        }
    public void destroy(){
        System.out.println("6.销毁过滤器实例");
    }
    //过滤器业务处理方法，在请求到达servlet之前先在此方法中处理公共的
    @Override
        public void doFilter(ServletRequest arg0, ServletResponse arg1, FilterChain arg2)
            throws IOException, ServletException {
        // TODO Auto-generated method stub
        System.out.println("3.执行过滤器业务处理方法");
        arg2.doFilter(arg0, arg1);
        System.out.println("5.执行过滤器业务处理方法");    
    }
```
```java
public class IndexServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        // TODO Auto-generated method stub
//      super.doGet(req, resp);
        System.out.println("4.处理用户请求开始。。。");
    }
}
```
Servlet与filter启动的区别： 
filter是在服务器启动时候创建实例，也就是服务器启动时候自动执行 
1.创建实例 
2.执行过滤器初始化方法
在访问的时候，执行 
3.执行过滤器业务处理方法； 
    chain.doFilter() 
(如果有下一个过滤器，则进入到下一个过滤器，否则执行访问servlet)
4.处理用户请求开始 
5.执行过滤器业务处理方法
如果配置了多个过滤器，则按照web.xml中出现的顺序，依次执行，执行完后按照过滤器倒序退回
如果在第一个过滤器中，没有执行doFilter(),那么不会进入到下一个filter
### 过滤器相关API
```
Interface
    Filter：
        void init(filterConfig):初始化方法，在服务器启动时候执行
            //获取过滤器在web.xml中配置的初始化参数
            filterConfig.getInitParameter("encoding");
            //filterConfig.getInitParameterNames();
        Enumeration<String> enums=filterConfig.getInitParameterNames();
        while(enums.hasMoreElements()){
            String name=enums.nextElement();
            String value=filterConfig.getInitParameter(name);
        }
        void doFilter(req,res,filterChain):过滤器拦截的业务处理方法
        void destroy()    销毁过滤器实例时候调用
    FilterConfig
        String getFilterName(String name):filter的内部名称;
    FilterChain
        一个个过滤器形成一个执行链           
            唯一的方法：doFilter(req,res):执行下一个过滤器或者放行
```
### 过滤器的拦截
1.
```
/*则是拦截所有的，包含jsp和servlet
```
2.
```
/index.jsp则是拦截指定的这个jsp
```
3.如果需要拦截多个
```
<url-pattern>/index.jsp</url-pattern>
<url-pattern>/list.jsp</url-pattern>
```
4.根据servlet的内部名称拦截
```
<servlet-name>IndexServlet</servlet-name>
```
5.拦截指定的servlet
```
<url-pattern>/index</url-pattern>
```
6.指定拦截指定的类型
```
默认拦截的类型 
<dispatcher>REQUEST</dispatcher>
拦截转发
<dispatcher>FORWARD</dispatcher>
拦截包含的页面(RequestDispatcher)
<dispatcher>INCLUDE</dispatcher>
拦截声明式异常
<dispatcher>ERROR</dispatcher>
    <error-page>
        <error-code>404</error-code>
        <exception-type></exception-type>
        <location></location>
    </error-page>
```
## 过滤器案例
```
几乎每一个servlet都要涉及编码处理，处理请求数据中文问题。
每个servlet都要做这些操作,把公用的代码抽取-过滤器实现。
```
代码实现： 
    1.login.jsp登录，输入“中文” 
    2.LoginServlet.java 直接处理登录请求 
    3.EncodingFilter.java:过滤器
对指定接口的某一个方法进行功能扩展，可以使用代理。 
对request对象，创建代理对象
chain.doFilter(req,res); 
即对request对象创建代理对象 
Proxy.newProxyInstance( 
    req.getClass().getClassLoader(),  //指定当前使用的类加载器 
    //对目标对象实现的接口类型 
    new Class[]{HttpServletRequest.class}, 
    new InvocationHandler(){ 
        public Object invoke(Object proxy,Method method,Object[] args)  throws Throwable{ 
        //定义方法返回值 
        Object returnValue=NULL； 
        //获取方法名 
        String methodName=method.getName(); 
    return null; 
} 
    } 
    )
