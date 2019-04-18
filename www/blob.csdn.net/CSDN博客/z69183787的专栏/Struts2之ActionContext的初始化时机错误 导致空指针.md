# Struts2之ActionContext的初始化时机错误 导致空指针 - z69183787的专栏 - CSDN博客
2014年02月28日 15:05:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3478

```java
@SuppressWarnings("serial")
public abstract class AbstractParamAction extends ActionSupport{
	public ActionContext actionContext = ActionContext.getContext();
	public HttpServletRequest request = (HttpServletRequest) actionContext.get(ServletActionContext.HTTP_REQUEST);
	public HttpServletResponse response = (HttpServletResponse) actionContext.get(ServletActionContext.HTTP_RESPONSE);
	
	//public ServletContext  application = (ServletContext)actionContext.getApplication();
	public HttpSession session = request.getSession();
}
```
```java
/**
 * 
 */
package com.wonders.stpt.marquee.action;
import org.apache.struts2.convention.annotation.Action;
import org.apache.struts2.convention.annotation.Namespace;
import org.apache.struts2.convention.annotation.ParentPackage;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Controller;
import com.wonders.stpt.innerWork.action.AbstractParamAction;
import com.wonders.stpt.marquee.service.MarqueeService;
import com.wonders.stpt.util.ActionWriter;
/** 
 * @ClassName: MarqueeAction 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2014年2月28日 下午1:58:40 
 *  
 */
@ParentPackage("struts-default")
@Namespace(value="/marquee")
@Controller("marqueeAction")
@Scope("prototype")
public class MarqueeAction extends AbstractParamAction{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 4787537184825647031L;
	private ActionWriter aw = new ActionWriter(response);
	private MarqueeService service;
	public MarqueeService getService() {
		return service;
	}
	@Autowired(required=false)
	public void setService(@Qualifier("marqueeService")MarqueeService service) {
		this.service = service;
	}
	
	
	@Action(value="getMarquee")
	public String getMarquee(){
		aw.writeJson(this.service.getMsg());
		return null;
	}
}
```
原来 action 中 使用的都是 多例模式，每次初始化action都是在 一个请求发起后，故actionContext 中均有 request，response等值。
但改为单例模式后，自动在spring加载时初始化，此时actionContext中，由于未发生用户请求，request，response 均为空。
若要使用单例模式，就只能在具体方法中获得request等其他对象。
