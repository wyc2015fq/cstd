# org.aspectj.lang.JoinPoint-中文简要API - z69183787的专栏 - CSDN博客
2014年04月09日 05:50:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7634
AspectJ使用org.aspectj.lang.JoinPoint接口表示目标类连接点对象，如果是环绕增强时，使用org.aspectj.lang.ProceedingJoinPoint表示连接点对象，该类是JoinPoint的子接口。任何一个增强方法都可以通过将第一个入参声明为JoinPoint访问到连接点上下文的信息。我们先来了解一下这两个接口的主要方法： 
1)JoinPoint 
 java.lang.Object[] getArgs()：获取连接点方法运行时的入参列表； 
 Signature getSignature() ：获取连接点的方法签名对象； 
 java.lang.Object getTarget() ：获取连接点所在的目标对象； 
 java.lang.Object getThis() ：获取代理对象本身； 
2)ProceedingJoinPoint 
ProceedingJoinPoint继承JoinPoint子接口，它新增了两个用于执行连接点方法的方法： 
 java.lang.Object proceed() throws java.lang.Throwable：通过反射执行目标对象的连接点处的方法； 
 java.lang.Object proceed(java.lang.Object[] args) throws java.lang.Throwable：通过反射执行目标对象连接点处的方法，不过使用新的入参替换原来的入参。 
例子：
```java
package com.wonders.contact.deptContact.aspect;
import java.lang.reflect.Method;
import javax.servlet.http.HttpServletRequest;
import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.After;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.stereotype.Component;
import com.wonders.contact.common.action.AbstractParamAction;
import com.wonders.contact.common.model.vo.ParamVo;
import com.wonders.contact.common.util.SimpleLogger;
import com.wonders.contact.deptContact.action.DeptContactAction;
import com.wonders.contact.deptContact.action.DeptContactOperateAction;
import com.wonders.contact.deptContact.model.vo.DeptContactParamVo;
import com.wonders.contact.deptContact.service.DeptContactCheckService;
import com.wonders.contact.deptContact.service.DeptContactService;
import com.wonders.contact.deptContact.util.DeptContactParamsUtil;
/**多级工作联系单AOP
 * @author XFZ
 * @version 1.0 2012-6-12
 */
@Aspect
@Component("contact-deptContactAspect")
public class DeptContactAspect {
	private DeptContactCheckService checkService;
	SimpleLogger log = new SimpleLogger(this.getClass());
	private DeptContactParamsUtil paramsUtil = new DeptContactParamsUtil();
	
	/**DeptContactAction初始化
	 * @param joinPoint
	 */
	@Before(value="execution(public * com.wonders.contact.deptContact.action.DeptContactAction.step*(..))")
	public void beforeActionStep(JoinPoint joinPoint){
		DeptContactAction target = (DeptContactAction) joinPoint.getTarget();
		String stepCode = joinPoint.getSignature().getName();
		executePrepareMethod(stepCode,target);
	}
	
	/**DeptContactOperateAction初始化
	 * @param joinPoint
	 */
	@Before(value="execution(public * com.wonders.contact.deptContact.action.DeptContactOperateAction.step*(..))")
	public void beforeOperateActionStep(JoinPoint joinPoint){
		DeptContactOperateAction target = (DeptContactOperateAction) joinPoint.getTarget();
		String stepCode = joinPoint.getSignature().getName();
		executePrepareMethod(stepCode,target);
	}
	
	/**DeptContactService流程初始化
	 * @param joinPoint
	 * @throws Throwable
	 */
	@Before(value="execution(public * com.wonders.contact.deptContact.service.DeptContactService.flowStep*(..))")
	public void beforeServiceFlowStep(JoinPoint joinPoint){
		initService(joinPoint);
		String stepCode = joinPoint.getSignature().getName();
		executeCheckMethod(stepCode);
    }
	
	/**检查待办项是否存在
	 * @param joinPoint
	 */
	@After(value="execution(public * com.wonders.contact.deptContact.service.DeptContactService.flowStepForward(..))")
	public void afterServiceForward(JoinPoint joinPoint){
		checkService.checkFlowIsInProcess();
	}
	
	
	
	/**DeptContactService变量初始化
	 * @param joinPoint
	 */
	private void initService(JoinPoint joinPoint){
		Object[] args = joinPoint.getArgs();
		DeptContactService target = (DeptContactService) joinPoint.getTarget();
		DeptContactParamVo params = (DeptContactParamVo) args[0];
		//resultInfo = params.resultInfo;
		target.init(params);
		checkService.init(params);
	}
	
	/**执行对应prepare方法
	 * @param stepCode
	 * @param target
	 */
	private void executePrepareMethod(String stepCode,AbstractParamAction target){
		if(stepCode.length()==0) return;
		
		stepCode = stepCode.substring("step".length());
//log.debug("prepare:"+stepCode);
		
		try {
			Method prepareMethod = paramsUtil.getClass()
				.getMethod("prepare"+stepCode,new Class[]{HttpServletRequest.class,ParamVo.class});
				prepareMethod.invoke(paramsUtil, new Object[]{target.request,target.getParams()});
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	/**执行对应check方法
	 * 
	 */
	private void executeCheckMethod(String stepCode){
		if(stepCode.length()==0) return;
		
		stepCode = stepCode.substring("stepFlow".length());
//log.debug("check:"+stepCode);
		
		try {
			Method checkMethod = checkService.getClass()
				.getMethod("check"+stepCode,new Class[]{});
				checkMethod.invoke(checkService, new Object[]{});
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	
	
	
	public DeptContactCheckService getCheckService() {
		return checkService;
	}
	
	@Autowired
	public void setCheckService(@Qualifier("contact-deptContactCheckService")DeptContactCheckService checkService) {
		this.checkService = checkService;
	}
}
```
