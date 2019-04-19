# Java中Servlet - LC900730的博客 - CSDN博客
2017年09月10日 14:40:52[lc900730](https://me.csdn.net/LC900730)阅读数：158
### servlet-api.jar
servlet-api.jar文件为Servlet API的类库文件。Servlet API主要由两个java包组成：javax.sevlet和javax.servlet.http。在javax.servlet包中定义了Servlet接口及相关的通用接口和类；在javax.servlet.http包中主要定义了与HTTP协议相关的HttpServlet类、HttpServletRequest接口和HttpServletResponse接口。 
### 1.Servlet接口
Servlet API的核心是javax.servlet.Servlet接口，所有的servlet都必须实现这一接口。接口中定义了5个方法，3个由Servlet容器调用。 
- init(): 
负责初始化Servlet对象。容器在创建好Servlet对象后，就会调用该方法。
- service(ServletRequest req,ServletResponse res): 
负责响应客户请求，为客户提供相应服务。当容器接收到客户端要求访问特定Servlet对象的请求时，会调用该Servlet对象的service 
- destroy() 
当Servlet对象结束生命周期时，容器就会调用该方法。
Servlet接口还定义了以下两个返回Servlet的相关信息的方法。 
- getServletConfig():返回一个ServletConfig对象，在该对象中包含了Servlet的初始化参数信息
- getServletInfo():返回一个字符串，在该字符串中包含了Servlet创建者、版本和版权等信息
