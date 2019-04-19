# Servlet生命周期 - LC900730的博客 - CSDN博客
2017年07月30日 21:49:07[lc900730](https://me.csdn.net/LC900730)阅读数：149标签：[servlet](https://so.csdn.net/so/search/s.do?q=servlet&t=blog)
个人分类：[java](https://blog.csdn.net/LC900730/article/category/7027036)
1.通过映射找到servlet-class的内容：
```
字符串：gz.itcast.a_servlet.FirstServlet
```
2.通过反射构造对象 
    2.1.得到字节码对象 
    Class cls=class.forName(“gz.itcast.a_servlet.FirstServlet”); 
    2.2调用无参数的构造方法来构造对象 
    Object obj=cls.newInstance(); 
3.创建ServletConfig对象，通过反射调用init方法 
    3.1得到对象 
    Method m=cls.getDeclareMethod(“init”,ServletConfig.class); 
    m.invoke(obj,config)   //servlet中的init方法被调用。 
4.service方法 
    创建request、response对象，通过反射调用service方法 
    4.1得到方法对象 
        Method m=cls.getDeclarMethod(“service”,HttpServletRequest.class,HttpServletResponse.class); 
        m.invoke(obj,request,response);  //servlet里面的service方法被调用
5.当tomcat服务器停止或web应用重新部署，通过反射调用destroy方法
doGet还是doPost都是在service中调用的
![这里写图片描述](https://img-blog.csdn.net/20170730145200540?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170730145401811?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTEM5MDA3MzA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
默认情况下，第一次访问servlet的时候创建servlet对象，如果servlet的构造方法或init方法中执行了比较多的逻辑代码，那么导致用户第一次访问servlet的时候比较慢。
public void init(ServletConfig config){ 
    this.config=config; 
    init(); 
}
一个用户过来，tomcat开辟一个线程， 
Thread1 的public void run(){ 
    first.service(request,response); 
}
Thread2的public void run(){ 
    first.service(request,response); 
}
servlet中线程安全的问题： 
servlet对象在tomcat服务器是单实例多线程的； 
因为servlet是多线程的，所以当多个servlet线程同时访问servlet共享数据，如成员变量，可能会引发线程安全问题； 
解决办法： 
    1.把使用到共享数据的代码块进行同步(使用synchronized关键字进行同步)。 
    2.建议在servlet中尽量不要使用成员变量。如果需要使用，则必须同步。而且尽量缩小代码块的范围(哪里使用到了成员变量，就同步哪里)，避免因为同步影响性能。
