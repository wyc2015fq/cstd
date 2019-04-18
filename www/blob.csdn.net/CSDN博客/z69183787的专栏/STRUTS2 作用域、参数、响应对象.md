# STRUTS2 作用域、参数、响应对象 - z69183787的专栏 - CSDN博客
2013年12月02日 07:32:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1805
转载：[http://blog.csdn.net/wangyu937/archive/2010/04/25/5527570.aspx](http://blog.csdn.net/wangyu937/archive/2010/04/25/5527570.aspx)
在struts1中，获得到系统的request或者session对象非常方便，都是按照形参传递的，但是在struts2中，request和session等都被隐藏了
主要使用的类：RequestMap,SessionMap,ApplicationMap, AttributeMap(可以获得所有作用域值)，
接口：requestAware,ServletRequestAware,SessionAware,ApplicationAware分别实现不同的接口就可以得到上边相应类的对象，需要强转,还有ServletResponseAware接口获得响应对象,ParameterAware获得所有请求参数
struts提供两种方式访问session和request，其中比较常用的是利用SPRING里面所说的IOC即控制反转
IOC方式：
action类实现ServletRequestAware接口，并新建一个HttpServletRequest request
public class UserLoginAction extends ActionSupport implements ServletRequestAware{
   public void setServletRequest(HttpServletRequest request) {
     this.request=request;
  }
 然后可以生成的request得到对象，如request.getRemoteAddr()
action类实现SessionAware接口，并创建一个MAP对象session
public class UserLoginAction extends ActionSupport implements ServletRequestAware,SessionAware{
   public void setServletRequest(HttpServletRequest request) {
     this.request=request;
  }
public void setSession(Map session) {
  this.session=session;  
 }
非IOC方式
非Ioc方式
这种方式主要是利用了com.opensymphony.xwork2.ActionContext类以及org.apache.struts2.ServletActionContext类，具体的方法如下所示。
获得request对象：
A ． HttpServletRequest request = ServletActionContext.getRequest ();
B．ActionContext ct= ActionContext.getContext()
   HttpServletRequest request=
(HttpServletRequest)ct.get(ServletActionContext. HTTP_REQUEST );
C. ActionContext.getContext().put(HTTP_REQUEST, request);
or
ActionContext context = ActionContext.getContext();
HttpServletRequest request = (HttpServletRequest)context.get(StrutsStatics.HTTP_REQUEST);
获得session对象：
在Struts2中底层的session都被封装成了Map类型，我们称之为SessionMap，而平常我们所说的session则是指HttpSession对象，具体的获得方法如下所示。
A．Map session=ActionContext.getSession();
B．Map session=(Map)ActionContext.getContext().get(ActionContext.SESSION);
C．可以先获得请求，然后通过请求获得HttpSession
得到这个SessionMap之后我们就可以对session进行读写了，如果我们想得到原始的HttpSession可以首先得到HttpServletRequest对象，然后通过request.getSession()来取得原始的HttpSession对象。一般情况下SessionMap已经可以完成所有的工作，我们不必再去碰底层的session了。
注销：
if (session instanceof org.apache.struts2.dispatcher.SessionMap) {
    try {
        ((org.apache.struts2.dispatcher.SessionMap) session).invalidate();
    } catch (IllegalStateException e) {
        logger.error(msg, e);
    }
}
获得响应方式：
A．         通过Ioc方式，实现ServletResponseAware接口
B．          ServletActionContext.getResponse();
C．          (HttpServletResponse)ServletActionContext.get(ServletActionContext.HTTP_RESPONSE)
使用常量Key进行获取可以得到所有作用域对象及parameter，response对象。利用的就是Serv letActionServlet实现的接口：StrutsStatics和父类ActionContext中的常量key.
获得请求参数：
A.    通过Ioc方式：实ParameterAware接口。
Action中：private Map map
 //重写接口方法
Public void setParameters(Map<String,String[]> parameters)
{
This.map= parameters;
}
B.              Map parameters = ActionContext.getContext().getParameters();C.  ActionContext.getContext().get(ActionContext. pARAMETERS);
获得application
a. 通过Ioc方式：实ApplicationAware接口。
Action中：private ApplicationMap map
 //重写接口方法
Public void  setApplication(Map<String,Object> application)
{
This.map= application;
}
b.ActionContext.getContext().getApplication();
c.(ApplicationMap)ActionContext.getContext().get(ActionContext. APPLICATION);
d.通过前边的获得request,在通过request获得Application

ActionContext（com.opensymphony.xwork.ActionContext）是Action执行时的上下文，上下文可以看作是一个容器（其实我们这里的容器就是一个Map而已），它存放放的是Action在执行时需要用到的对象，比如：在使用WebWork时，我们的上下文放有请求的参数（Parameter）、会话(Session)、Servlet上下文（ServletContext）、本地化（Locale）信息等。
在每次执行Action之前都会创建新的ActionContext，ActionContext是线程安全的，也就是说在同一个线程里ActionContext里的属性是唯一的，这样我的Action就可以在多线程中使用。
我们可以通过ActionContext的静态方法：ActionContext.getContext()来取得当前的ActionContext对象。
也可以直接使用他的子类ServletActionContext
其他：
Parameters in action result definitions
<struts>
....
   <package name="somePackage" namespace="/myNamespace" extends="struts-default">
      <action name="myAction" class="com.project.MyAction">
         <result name="success" type="redirectAction">otherAction?id=${id}</result>
         <result name="back" type="redirect">${redirectURL}</result>
      </action>
      <action name="otherAction" class="com.project.MyOtherAction">
         ...
      </action>     
   </package>
....
</struts>
The only requirement is to declare the necessary properties in your action, in this case com.project.MyAction should define properties id and redirectURL with standard accessor methods.
public class MyAction extends ActionSupport {
   private int id;
   private String redirectURL;
   ...
   public String execute() {
       ...
      if (someCondition) {
         this.redirectURL = "/the/target/page.action";
         return "back";
      }
      this.id = 123;
      return SUCCESS;
   }
   public int getId() { return this.id; }
   public void setId(int id) { this.id = id; }
   public String getRedirectURL() { return this.redirectURL; }
   public void setRedirectURL(String redirectURL) { this.redirectURL= redirectURL; }
   ...
}
In the above code if it returns SUCCESS then the browser will be forwarded to
/<app-prefix>/myNamespace/otherAction.action?id=123
Static (or pre-defined) parameters can be set to a Map property or to individual JavaBean properties.
Define the parameters to be set by adding the name(s) and value(s) to the action mapping element (in the application's struts.xml. 
<action name="myAction" class=" MyAction">
     <param name="myStaticParam1">myStaticValue1</param>
     <param name="myStaticParam2">myStaticValue2</param>
     <param name="myStaticParam3">myStaticValue3</param>
  </action>
Ensure that staticParams Interceptor is included in the Action's stack. 
       The default stack already includes staticParams.
Edit the Action so that it implements the Parameterizable interface.
Ensure the Action defines a setParams(Map) method. 
The staticParams Interceptor will set the defined values to the Map, using the name as the entry key. 
key 
 value 
myStaticParam1 
 myStaticValue1 
myStaticParam2 
 myStaticValue2 
myStaticParam3 
 myStaticValue3 
JavaBean properties
Ensure that the Action defines JavaBean properties corresponding to the param elements in the action mapping. 
The staticParams Interceptor will set the defined values to each JavaBean property that corresponds to a param element. 
public String getMyStaticParam1()
public void setMyStaticParam1(String myStaticParam1)
public String getMyStaticParam2()
public void setMyStaticParam2(String myStaticParam2)
public String getMyStaticParam3()
public void setMyStaticParam3(String myStaticParam3)
@see com.opensymphony.xwork.interceptor.StaticParametersInterceptor
@see com.opensymphony.xwork.config.entities.Parameterizable
详情见：struts2文档中的fAQS
一、"＃"的用法 
   1、 访问OGNL上下文和Action上下文，#相当于ActionContext.getContext()；下表有几个ActionContext中有用的属性： 
        parameters  包含当前HTTP请求参数的Map #parameters.id[0]作用相当于request.getParameter("id")
       request 包含当前HttpServletRequest的属性（attribute)的Map #request.userName相当于request.getAttribute("userName")
       session 包含当前HttpSession的属性（attribute）的Map #session.userName相当于session.getAttribute("userName")
       application 包含当前应用的ServletContext的属性（attribute）的Map #application.userName相当于application.getAttribute("userName")
       attr 用于按request > session > application顺序访问其属性（attribute） #attr.userName相当于按顺序在以上三个范围（scope）内读取userName属性，直到找到为止
     2、用于过滤和投影（projecting)集合，如books.{?#this.price<100}；
     3、构造Map，如#{'foo1':'bar1', 'foo2':'bar2'}。
二、"%"的用法
    “%”符号的用途是在标志的属性为字符串类型时，计算OGNL表达式的值。例如在Ognl.jsp中加入以下代码：
    <h3>%的用途</h3> 
    <p><s:url value="#foobar['foo1']" /></p> 
    <p><s:url value="%{#foobar['foo1']}" /></p>
取得一个List的一部分：
?  –  所有满足选择逻辑的对象
^  -   第一个满足选择逻辑的对象
$  -   最后一个满足选择逻辑的对象
例如：
person.relatives.{? #this.gender == 'male'}
三、"$"的用法
    1、用于在国际化资源文件中，引用OGNL表达式
    2、在Struts 2配置文件中，引用OGNL表达式
     例如： 
         <action name="AddPhoto" class="addPhoto"> 
            <interceptor-ref name="fileUploadStack" />            
            <result type="redirect">ListPhotos.action?       albumId=${albumId}</result> 
        </action>
