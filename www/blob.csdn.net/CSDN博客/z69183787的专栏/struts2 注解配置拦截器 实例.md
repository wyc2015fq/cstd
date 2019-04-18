# struts2 注解配置拦截器 实例 - z69183787的专栏 - CSDN博客
2014年08月08日 15:49:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11777
```java
/**
 * 
 */
package com.wonders.stpt.contractReview.action;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import org.apache.struts2.convention.annotation.Action;
import org.apache.struts2.convention.annotation.InterceptorRef;
import org.apache.struts2.convention.annotation.InterceptorRefs;
import org.apache.struts2.convention.annotation.Namespace;
import org.apache.struts2.convention.annotation.ParentPackage;
import org.apache.struts2.convention.annotation.Result;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.annotation.Scope;
import org.springframework.stereotype.Controller;
import com.opensymphony.xwork2.ModelDriven;
import com.wonders.stpt.contractReview.model.bo.WorkflowActivity;
import com.wonders.stpt.contractReview.model.vo.ContractReviewVo;
import com.wonders.stpt.contractReview.service.ContractReviewService;
import com.wonders.stpt.contractReview.util.ContractReviewUtil;
import com.wonders.stpt.core.login.entity.vo.TaskUserVo;
import com.wonders.stpt.page.model.PageInfo;
import com.wonders.stpt.page.model.PageResultSet;
import com.wonders.stpt.util.ActionWriter;
import com.wonders.stpt.util.StringUtil;
/** 
 * @ClassName: ContractReviewAction 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2013-6-9 上午9:37:12 
 *  
 */
@ParentPackage("custom-default")
@Namespace(value="/contractReview")
@Controller("contractReviewAction")
@Scope("prototype")
public class ContractReviewAction extends AbstractParamAction implements ModelDriven<ContractReviewVo>{
	/**
	 * 
	 */
	private static final long serialVersionUID = 1631323544896056619L;
	private ActionWriter aw = new ActionWriter(response);
	private ContractReviewVo vo = new ContractReviewVo();
	private ContractReviewService service;
	public ContractReviewService getService() {
		return service;
	}
	@Autowired(required=false)
	public void setService(@Qualifier("contractReviewService")ContractReviewService service) {
		this.service = service;
	}
	
	
	@SuppressWarnings({ "unchecked"})
	@Action(value="todoList",results={
			@Result(name="success",location="/contractReview/todoList.jsp")
			},
		<span style="color:#ff6666;"><strong>	interceptorRefs = {@InterceptorRef(value = "mydefault")}  )</strong></span>
	// 拦截器  
	
	public String todoItemList(){
		String loginNames = "";
		Map<String, TaskUserVo> userMap = 
				(Map<String, TaskUserVo>)this.request.getSession().getAttribute("deptUsers");
		for(Map.Entry<String, TaskUserVo> entry : userMap.entrySet()){
			loginNames += "'"+StringUtil.getNotNullValueString(entry.getKey()) +"'"+",";
		}
		if(loginNames.length() > 0){
			loginNames = loginNames.substring(0, loginNames.length()-1);
		}
		String contract_name = this.request.getParameter("contract_name");
		String contract_identifier = this.request.getParameter("contract_identifier");
		String contract_money_type_id = this.request.getParameter("contract_money_type_id");
		String contract_type1_id = this.request.getParameter("contract_type1_id");
		String contract_type2_id = this.request.getParameter("contract_type2_id");
		String contract_moneyLt = this.request.getParameter("contract_moneyLt");
		String contract_moneyGt = this.request.getParameter("contract_moneyGt");
		String project_charge_dept = this.request.getParameter("project_charge_dept");
		String contract_group_id = this.request.getParameter("contract_group_id");
		String purchase_type_id = this.request.getParameter("purchase_type_id");
		String[] kpi_controls = this.request.getParameterValues("kpi_control");
		String kpi_control = "";
		if(kpi_controls != null && kpi_controls.length > 0){
			for(String temp : kpi_controls){
				kpi_control += temp + ",";
			}
			kpi_control = kpi_control.substring(0, kpi_control.length()-1);
		}
		
		this.request.setAttribute("contract_name", contract_name);
		this.request.setAttribute("contract_identifier", contract_identifier);
		this.request.setAttribute("contract_money_type_id", contract_money_type_id);
		this.request.setAttribute("contract_type1_id", contract_type1_id);
		this.request.setAttribute("contract_type2_id", contract_type2_id);
		this.request.setAttribute("contract_moneyLt",contract_moneyLt);
		this.request.setAttribute("contract_moneyGt", contract_moneyGt);
		this.request.setAttribute("project_charge_dept", project_charge_dept);
		this.request.setAttribute("contract_group_id", contract_group_id);
		this.request.setAttribute("purchase_type_id", purchase_type_id);
		this.request.setAttribute("kpi_control", kpi_control);
		int totalRows = this.service.countTodo(contract_name,contract_identifier, 
				contract_money_type_id, contract_type1_id, 
				contract_type2_id, contract_moneyLt, 
				contract_moneyGt, project_charge_dept, 
				contract_group_id, purchase_type_id, 
				kpi_control,loginNames);
		PageInfo pageinfo = new PageInfo(totalRows, vo.pageSize, vo.page);	
		List<Object[]> list = this.service.findTodoByPage(pageinfo.getBeginIndex(), vo.pageSize,
				contract_name,
				contract_identifier, 
				contract_money_type_id, contract_type1_id, 
				contract_type2_id, contract_moneyLt, 
				contract_moneyGt, project_charge_dept, 
				contract_group_id, purchase_type_id, 
				kpi_control,loginNames);
		//System.out.println("list.size======"+list.size());
		PageResultSet<Object[]> result  = new PageResultSet<Object[]>();
		result.setList(list);
		result.setPageInfo(pageinfo);
		this.request.setAttribute("result", result);
		
		return "success";
	}
	
	/** 
	* @Title: getModel 
	* @Description: TODO(这里用一句话描述这个方法的作用) 
	* @param @return    设定文件 
	* @throws 
	*/
	@Override
	public ContractReviewVo getModel() {
		// TODO Auto-generated method stub
		return vo;
	}
	
}
```
struts.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE struts PUBLIC "-//Apache Software Foundation//DTD Struts Configuration 2.0//EN" "http://struts.apache.org/dtds/struts-2.0.dtd">
<struts> 
	<package name="custom-default" extends="struts-default">  
 <!-- 定义一个拦截器 -->  
        <interceptors>  
            <interceptor name="authority"  
                class="com.wonders.stpt.contractReview.interceptor.AuthInterceptor">  
            </interceptor>  
            <!-- 拦截器栈 -->  
            <interceptor-stack name="mydefault">  
            	<interceptor-ref name="authority" />  
                <interceptor-ref name="defaultStack" />  
                
            </interceptor-stack>  
        </interceptors>  
      </package>
	<constant name="struts.objectFactory.spring.autoWire.alwaysRespect" value="true"/>
	<constant name="struts.multipart.maxSize" value="25242880" />
	<constant name="struts.custom.i18n.resources" value="fileUpload"></constant>
</struts>
```
过滤器：
```java
/**   
* @Title: AuthInterceptor.java 
* @Package com.wonders.stpt.contractReview.interceptor 
* @Description: TODO(用一句话描述该文件做什么) 
* @author zhoushun   
* @date 2014年8月8日 下午3:25:49 
* @version V1.0   
*/
package com.wonders.stpt.contractReview.interceptor;
import java.util.Map;
import com.opensymphony.xwork2.ActionContext;
import com.opensymphony.xwork2.ActionInvocation;
import com.opensymphony.xwork2.interceptor.AbstractInterceptor;
import com.wonders.stpt.core.login.constant.LoginConstant;
/** 
 * @ClassName: AuthInterceptor 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2014年8月8日 下午3:25:49 
 *  
 */
public class AuthInterceptor extends AbstractInterceptor{
	/** 
	* @Title: intercept 
	* @Description: TODO(这里用一句话描述这个方法的作用) 
	* @param @param arg0
	* @param @return
	* @param @throws Exception    设定文件 
	* @throws 
	*/
	@Override
	public String intercept(ActionInvocation invocation) throws Exception {
		   // 取得请求相关的ActionContext实例  
        String result = "";
		ActionContext ctx = invocation.getInvocationContext();  
        Map session = ctx.getSession();  
        String user = (String) session.get(LoginConstant.STPT_SECURITY_LOGIN_NAME);  
        t("begin");
        
        if (user != null && "G00100000123".equals(user)) {  
            result = invocation.invoke();          
        }  else{
        	result = "error";
        }
        t("end");
        return result;  
	}
	private void t(String s){
		System.out.println(s);
	}
}
```
