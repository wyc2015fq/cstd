# Struts2学习笔记之自定义拦截器 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2012年09月01日 20:21:35[boonya](https://me.csdn.net/boonya)阅读数：858标签：[struts																[interceptor																[action																[user																[exception](https://so.csdn.net/so/search/s.do?q=exception&t=blog)
个人分类：[Struts2](https://blog.csdn.net/boonya/article/category/879253)





Struts2学习笔记

第十记：自定义拦截器
注：以用户登录拦截为例,session为空不允许操作。
1、编写Action方法类
public class UserAction{
   private String message;
   public String getMessage(){
         return this.message;
   }
   public void setMessage(String message){
         this.message=message;
    }
   public String execute() throw Exception{

      //do something
       return "success";//设置想要到达的视图
    }
    //................
}
2、Session实现用户登录拦截
原理：判断Session中用于记录用户的登录状态是否为空。
设置Session的属性值,在login.jsp页面设置：
<%
User user=new User("zhangsan","123456");
request.getSession().setAttribute("user",user);
%>
使用EL表达式接收页面消息: ${message}
3、实现自定义拦截器
自定义拦截器需要实现Interceptor接口:
public class MyInterceptor implements Interceptor {
    public void destroy() {
             // TODO Auto-generated method stub
    }
   public void init() {
           // TODO Auto-generated method stub
   }
   public String intercept(ActionInvocation invocation) throws Exception {
       Object user=ActionContext.getContext().getSession().get("user");
      if(user!=null)return invocation.invoke();
      else{
            ActionContext.getContext.put("message","用户无该操作权限！");
            return "message";
      }
   }
}
4、struts.xml下配置消息页面和注册拦截器
<!-- 配置消息页面 -->
<global-results>
<result name="message">/WEB-INF/page/message.jsp</results>
<result name="success">/WEB-INF/page/index.jsp</results>
</global-results>
  </global-results>
<!-- 配置Action类 -->
  <action name="list_*" class="com.struts.action.UserAction" method="{1}"></action>
<!-- 注册拦截器 -->
<interceptors>
<interceptor name="promission" class="com.struts.interceptor.MyInterceptor"></interceptor>
</interceptors>
5、拦截器的引用
在action中的引用
<action name="user" class="com.struts.action.UserAction">
<interceptor-ref name="permission"/>
</action>
注：在action中引用拦截器时，默认的拦截器会失效。
6、拦截器栈
为了解决自定义拦截器影响而使struts2的默认拦截器失效，故引入堆栈。
<interceptor-stack name="permissionStack">
<interceptor-ref name="defaultStack"/><!-- 必须先调用默认的栈拦截器实现 -->
<interceptor-ref name="permission"/>
</interceptor-stack>
设置全局的默认拦截器:
<default-interceptor-ref name="permissionStack"/>
注:全局的默认拦截器与<interceptors></interceptors>同级。](https://so.csdn.net/so/search/s.do?q=user&t=blog)](https://so.csdn.net/so/search/s.do?q=action&t=blog)](https://so.csdn.net/so/search/s.do?q=interceptor&t=blog)](https://so.csdn.net/so/search/s.do?q=struts&t=blog)




