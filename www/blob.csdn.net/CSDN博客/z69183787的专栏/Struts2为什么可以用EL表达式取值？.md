# Struts2为什么可以用EL表达式取值？ - z69183787的专栏 - CSDN博客
2014年08月01日 16:44:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1863
在一个应用了Struts2系列框架的项目中，在JSP页面常常用Struts附带的标签对表单元素取值,但，为什么EL表达式也能取到值栈（value stack）中的值呢?
我们知道，JSTL默认是从page，request，session，application这四个Scope逐次查找相应的EL表达式所对应的对象的值。那么如果要使用JSTL来读取Action中的变量，就需要把Action中的变量放到request域中才行。所以，早在 Webwork2.1.X的年代，我们会编写一个拦截器来做这个事情的。大致的原理是：在Action执行完返回之前，依次读取Action中的所有的变 量，并依次调用request.setAttribute()来进行设置。
自从 Webwork2.2以后，包括Struts2，都使用另外一种整合方式：对HttpServletRequest进行装饰。源码如下：
 public class StrutsRequestWrapper extends HttpServletRequestWrapper {
    public StrutsRequestWrapper(HttpServletRequest req) {
        super(req);
    }
    public Object getAttribute(String s) {
        if (s != null && s.startsWith("javax.servlet")) {
            // don't bother with the standard javax.servlet attributes, we can short-circuit this
            // see WW-953 and the forums post linked in that issue for more info
            return super.getAttribute(s);
        }
        ActionContext ctx = ActionContext.getContext();
        Object attribute = super.getAttribute(s);
        if (ctx != null) {
            if (attribute == null) {
                boolean alreadyIn = false;
                Boolean b = (Boolean) ctx.get("__requestWrapper.getAttribute");
                if (b != null) {
                    alreadyIn = b.booleanValue();
                }
                // note: we don't let # come through or else a request for
                // #attr.foo or #request.foo could cause an endless loop
                if (!alreadyIn && s.indexOf("#") == -1) {
                    try {
