# struts2中 ServletActionContext与ActionContext区别 - z69183787的专栏 - CSDN博客
2012年11月13日 13:42:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：734
个人分类：[Struts2](https://blog.csdn.net/z69183787/article/category/2175433)
**1. ActionContext**
在Struts2开发中,除了将请求参数自动设置到Action的字段中,我们往往也需要在Action里直接获取请求(Request)或会话(Session)的一些信息,甚至需要直接对JavaServlet Http的请求(HttpServletRequest),响应(HttpServletResponse)操作. 我们需要在Action中取得request请求参数"username"的值:
- ActionContext context = ActionContext.getContext();
- Map params = context.getParameters();
- String username = (String) params.get("username");
ActionContext(com.opensymphony.xwork.ActionContext)是Action执行时的上下文,**上下文可以看作是一个容器**(其实我们这里的容器就是一个Map而已),它存放的是Action在执行时需要用到的对象. 一般情况, 我们的ActionContext都是通过: ActionContext
 context = (ActionContext) actionContext.get();来获取的.我们再来看看这里的actionContext对象的创建:
**static ThreadLocal actionContext = new ActionContextThreadLocal();**
ActionContextThreadLocal是实现ThreadLocal的一个内部类.ThreadLocal可以命名为"线程局部变量",它为每一个使用该变量的线程都提供一个变量值的副本,使每一个线程都可以独立地改变自己的副本,而不会和其它线程的副本冲突.这样,我们ActionContext里的属性只会在对应的当前请求线程中可见,从而保证它是线程安全的.
通过ActionContext取得HttpSession: Map session = ActionContext.getContext().getSession();
**2. ServletActionContext**
ServletActionContext(com.opensymphony.webwork. ServletActionContext),这个类直接继承了我们上面介绍的ActionContext,它提供了直接与Servlet相关对象访问的功能,它可以取得的对象有:
(1)javax.servlet.http.HttpServletRequest : HTTPservlet请求对象
(2)javax.servlet.http.HttpServletResponse : HTTPservlet相应对象
(3)javax.servlet.ServletContext : Servlet上下文信息
(4)javax.servlet.ServletConfig : Servlet配置对象
(5)javax.servlet.jsp.PageContext : Http页面上下文
如何从ServletActionContext里取得Servlet的相关对象:
<1>取得HttpServletRequest对象: HttpServletRequest request = ServletActionContext. getRequest();
<2>取得HttpSession对象: HttpSession session = ServletActionContext. getRequest().getSession();
**3. ServletActionContext和ActionContext联系**
ServletActionContext和ActionContext有着一些重复的功能,在我们的Action中,该如何去抉择呢?**我们遵循的原则是:如果ActionContext能够实现我们的功能,那最好就不要使用ServletActionContext,让我们的Action尽量不要直接去访问Servlet的相关对象.**
注意：在使用ActionContext时有一点要注意: **不要在Action的构造函数里使用ActionContext.getContext(),**因为这个时候ActionContext里的一些值也许没有设置,这时通过ActionContext取得的值也许是null；**同样，HttpServletRequest req = ServletActionContext.getRequest()也不要放在构造函数中，也不要直接将req作为类变量给其赋值。**至于原因，我想是因为前面讲到的static ThreadLocal actionContext = new ActionContextThreadLocal()，从这里我们可以看出ActionContext是线程安全的，而ServletActionContext继承自ActionContext，所以ServletActionContext也线程安全，线程安全要求每个线程都独立进行，所以req的创建也要求独立进行，所以ServletActionContext.getRequest()这句话不要放在构造函数中，也不要直接放在类中，而应该放在每个具体的方法体中(eg：login()、queryAll()、insert()等)，这样才能保证每次产生对象时独立的建立了一个req。
