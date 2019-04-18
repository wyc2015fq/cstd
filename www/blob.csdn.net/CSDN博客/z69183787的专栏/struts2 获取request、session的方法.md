# struts2 获取request、session的方法 - z69183787的专栏 - CSDN博客
2012年11月01日 09:56:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：871
个人分类：[Struts2](https://blog.csdn.net/z69183787/article/category/2175433)
**一，获取方式：**
**从Servlet容器中获取的方法与Servlet容器无关的获取方法 IoC方式非Ioc方式**
**二，获取方法**
**  2.1 与servlet无关:**
**    2.1.1 非IoC模式**
Java代码  ![收藏代码](http://yangzhibin-java.iteye.com/images/icon_star.png)
- ActionContext context ;//该方法还可以取得值栈（getValueStack()）,使用push（obj）往值栈中放数据
- Map request ;  
- Map session ;  
- Map application ;  
- 
- //以下初始化方法要放在方法中
- context = ActionContext.getContext();  
- 
- request = (Map)context.get("request");//获取request
- session = context.getSession();//获取session
- application = context.getApplication();//获取application
- 
- request.put("req", "request属性");  
- session.put("ses", "session属性");  
- application.put("app", "application属性");  
Xml代码  ![收藏代码](http://yangzhibin-java.iteye.com/images/icon_star.png)
- <!--jsp中取值-->
- request属性接受:<s:propertyvalue="#request.req"/>${requestScope.req}  
- session属性接受:<s:propertyvalue="#session.ses"/>${sessionScope.ses}  
- application属性接受：<s:propertyvalue="#application.app"/>${applicationScope.app}  
**    2.1.2  IoC模式**
Java代码  ![收藏代码](http://yangzhibin-java.iteye.com/images/icon_star.png)
- //实现接口
- publicclass LoginAction extends ActionSupport implements RequestAware,SessionAware,ApplicationAware  
- 
- Map request ;  
- Map session ;  
- Map application ;  
- 
- publicvoid setRequest(Map request) {  
- this.request = request;  
- }  
- publicvoid setSession(Map session) {  
- this.session = session;  
- }  
- publicvoid setApplication(Map application) {  
- this.application = application;  
- }      
** 2.2 与servlet有关**
**    2.2.1 非IoC模式**
Java代码  ![收藏代码](http://yangzhibin-java.iteye.com/images/icon_star.png)
- HttpServletRequest request;  
- HttpSession session ;  
- ServletContext application;  
- 
- request = ServletActionContext.getRequest();//获取request
- session = request.getSession();  
- application = ServletActionContext.getServletContext();//获取application
- 
- request.setAttribute("req", "request属性111");  
- session.setAttribute("ses", "session属性111");  
- application.setAttribute("app", "application属性111");  
**    2.2.1 IoC模式**
Java代码  ![收藏代码](http://yangzhibin-java.iteye.com/images/icon_star.png)
- //实现接口
- publicclass LoginAction extends ActionSupport implements ServletRequestAware,ServletContextAware  
- 
- 
- HttpServletRequest request;  
- ServletContext application;   
- 
- 
- publicvoid setServletRequest(HttpServletRequest request) {  
- this.request = request;  
- }  
- publicvoid setServletContext(ServletContext application) {  
- this.application = application;  
- }  
 自己用的两种方法
非IOC
```java
package com.wondersgroup.workflow.common.action;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import org.apache.struts2.ServletActionContext;
import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionSupport;
import com.wondersgroup.workflow.common.model.vo.ParamVo;
@SuppressWarnings("serial")
public abstract class AbstractParamAction extends ActionSupport{
	public ActionContext actionContext = ActionContext.getContext();
	public HttpServletRequest request = (HttpServletRequest) actionContext.get(ServletActionContext.HTTP_REQUEST);
	public HttpServletResponse response = (HttpServletResponse) actionContext.get(ServletActionContext.HTTP_RESPONSE);
	public HttpSession session = request.getSession();
	//public ParamVo params;
	abstract public ParamVo getParams();
}
```
IOC：
```java
package com.wonders.stpt.userMsg.action;
import java.util.Map;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.apache.struts2.interceptor.ServletRequestAware;
import org.apache.struts2.interceptor.ServletResponseAware;
import org.apache.struts2.interceptor.SessionAware;
import com.opensymphony.xwork2.ActionSupport;
@SuppressWarnings("serial")
public class AbstractParamAction extends ActionSupport implements 
SessionAware, ServletRequestAware, ServletResponseAware{
	private static final long serialVersionUID = 7620009925942346125L;   
	
	protected Map session;
    protected HttpServletRequest servletRequest;
    protected HttpServletResponse servletResponse;
	 public Map getSession()
    {
        return session;
    }
    public void setSession(Map session)
    {
        this.session = session;
    }
    public HttpServletRequest getServletRequest()
    {
        return servletRequest;
    }
    public HttpServletResponse getServletResponse()
    {
        return servletResponse;
    }
    public void setServletRequest(HttpServletRequest servletRequest)
    {
        this.servletRequest = servletRequest;
    }
    public void setServletResponse(HttpServletResponse servletResponse)
    {
        this.servletResponse = servletResponse;
    }
}
```
