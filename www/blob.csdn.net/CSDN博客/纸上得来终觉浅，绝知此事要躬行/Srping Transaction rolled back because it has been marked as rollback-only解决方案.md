# Srping Transaction rolled back because it has been marked as rollback-only解决方案 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年07月29日 10:54:54[boonya](https://me.csdn.net/boonya)阅读数：8543








### 1.异常相关描述

如题，此异常的全部信息如下：

```java
org.springframework.transaction.UnexpectedRollbackException: Transaction rolled back because it has been marked as rollback-only
	at org.springframework.transaction.support.AbstractPlatformTransactionManager.commit(AbstractPlatformTransactionManager.java:718)
	at org.springframework.transaction.interceptor.TransactionAspectSupport.commitTransactionAfterReturning(TransactionAspectSupport.java:475)
	at org.springframework.transaction.interceptor.TransactionAspectSupport.invokeWithinTransaction(TransactionAspectSupport.java:270)
	at org.springframework.transaction.interceptor.TransactionInterceptor.invoke(TransactionInterceptor.java:94)
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:172)
	at org.springframework.aop.interceptor.ExposeInvocationInterceptor.invoke(ExposeInvocationInterceptor.java:91)
	at org.springframework.aop.framework.ReflectiveMethodInvocation.proceed(ReflectiveMethodInvocation.java:172)
	at org.springframework.aop.framework.JdkDynamicAopProxy.invoke(JdkDynamicAopProxy.java:204)
	at com.sun.proxy.$Proxy81.updateProductStockProcess(Unknown Source)
	at fmcgwms.OmsAPIServiceTest.testUpdateProductToProcess(OmsAPIServiceTest.java:113)
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at sun.reflect.NativeMethodAccessorImpl.invoke(Unknown Source)
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(Unknown Source)
	at java.lang.reflect.Method.invoke(Unknown Source)
	at org.junit.runners.model.FrameworkMethod$1.runReflectiveCall(FrameworkMethod.java:47)
	at org.junit.internal.runners.model.ReflectiveCallable.run(ReflectiveCallable.java:12)
	at org.junit.runners.model.FrameworkMethod.invokeExplosively(FrameworkMethod.java:44)
	at org.junit.internal.runners.statements.InvokeMethod.evaluate(InvokeMethod.java:17)
	at org.springframework.test.context.junit4.statements.RunBeforeTestMethodCallbacks.evaluate(RunBeforeTestMethodCallbacks.java:74)
	at org.springframework.test.context.junit4.statements.RunAfterTestMethodCallbacks.evaluate(RunAfterTestMethodCallbacks.java:83)
	at org.springframework.test.context.junit4.statements.SpringRepeat.evaluate(SpringRepeat.java:72)
	at org.springframework.test.context.junit4.SpringJUnit4ClassRunner.runChild(SpringJUnit4ClassRunner.java:231)
	at org.springframework.test.context.junit4.SpringJUnit4ClassRunner.runChild(SpringJUnit4ClassRunner.java:88)
	at org.junit.runners.ParentRunner$3.run(ParentRunner.java:238)
	at org.junit.runners.ParentRunner$1.schedule(ParentRunner.java:63)
	at org.junit.runners.ParentRunner.runChildren(ParentRunner.java:236)
	at org.junit.runners.ParentRunner.access$000(ParentRunner.java:53)
	at org.junit.runners.ParentRunner$2.evaluate(ParentRunner.java:229)
	at org.springframework.test.context.junit4.statements.RunBeforeTestClassCallbacks.evaluate(RunBeforeTestClassCallbacks.java:61)
	at org.springframework.test.context.junit4.statements.RunAfterTestClassCallbacks.evaluate(RunAfterTestClassCallbacks.java:71)
	at org.junit.runners.ParentRunner.run(ParentRunner.java:309)
	at org.springframework.test.context.junit4.SpringJUnit4ClassRunner.run(SpringJUnit4ClassRunner.java:174)
	at org.eclipse.jdt.internal.junit4.runner.JUnit4TestReference.run(JUnit4TestReference.java:50)
	at org.eclipse.jdt.internal.junit.runner.TestExecution.run(TestExecution.java:38)
	at org.eclipse.jdt.internal.junit.runner.RemoteTestRunner.runTests(RemoteTestRunner.java:459)
	at org.eclipse.jdt.internal.junit.runner.RemoteTestRunner.runTests(RemoteTestRunner.java:675)
	at org.eclipse.jdt.internal.junit.runner.RemoteTestRunner.run(RemoteTestRunner.java:382)
	at org.eclipse.jdt.internal.junit.runner.RemoteTestRunner.main(RemoteTestRunner.java:192)
```
网上有很多遇到这个问题的先来说说别人的解决方案：
```
<bean id="transactionManager"
	class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
	<property name="dataSource" ref="dataSource" />
	<property name="globalRollbackOnParticipationFailure" value="false" /> <!--指定此参数为false-->

</bean>
```
这种方案不推荐大家使用，因为会影响到其他事务的管理。
### 2.解决方案

先来看看Spring事务管理的配置：

```
<tx:advice id="txAdvice" transaction-manager="transactionManager">
		<tx:attributes>
			<tx:method name="del*" propagation="REQUIRED" read-only="false"
				rollback-for="java.lang.Exception" />
			<tx:method name="insert*" propagation="REQUIRED" read-only="false"
				rollback-for="java.lang.Exception" />
			<tx:method name="update*" propagation="REQUIRED" read-only="false"
				rollback-for="java.lang.Exception" />
			<tx:method name="execute*" propagation="REQUIRED" read-only="false"
				rollback-for="java.lang.Exception" />
			<tx:method name="*" propagation="REQUIRED" read-only="true" />
		</tx:attributes>
</tx:advice>
```
很明显，事务为相关方法的Exception进行了事务管理。所以从异常入手的是最简单的，即定义自己的异常类信息：
```java
package com.wlyd.fmcgwms.util.exception;
/**
 * 不回滚接口异常（Spring exception被事务管理时只要遇到Exception就会回滚方法内所有操作）
 * 
 * @packge com.wlyd.fmcgwms.util.exception.UnrollbackException
 * @date   2016年7月29日  上午10:07:39
 * @author pengjunlin
 * @comment   
 * @update
 */
public class UnrollbackException extends Exception{

	/**
	 * 
	 */
	private static final long serialVersionUID = -1255980800389665752L;

	@Override
	public String getMessage() {
		return "接口不回滚异常信息："+super.getMessage();
	}

	public UnrollbackException(String message, Throwable cause) {
		super(message, cause);
		// TODO Auto-generated constructor stub
	}

	public UnrollbackException(String message) {
		super(message);
		// TODO Auto-generated constructor stub
	}

	public UnrollbackException(Throwable cause) {
		super(cause);
		// TODO Auto-generated constructor stub
	}
	
	

}
```
远程接口service实现时定义的事务方法，异常采用自己所定义的异常，Exception继承实现的异常也会被管理：
处理方法：将接口定义的异常内部捕获而不抛出。


```java
public String updateProductStock(String token, String requestBody,
			String platformCode, String tenantCode) throws UnrollbackException {
		Log.getLogger(getClass()).info("调用九州通达OMS接口.........../updateproductstock REQUEST:"+requestBody); 
		String url = EhcacheUtil.get("JZTD_OMS_API_BASE_URL").toString()	+ "/updateproductstock";
		String result=null;
		try {
			result = RestTemplateUtils.post(url, requestBody, token, platformCode, tenantCode);
			Log.getLogger(getClass()).info("调用九州通达OMS接口.........../updateproductstock RESPONSE:"+result); 
		} catch (Exception e) {
			Log.getLogger(getClass()).info("调用九州通达OMS接口.........../updateproductstock NO RESPONSE:"+e.getMessage()); 
			// 如需方法内业务操作回滚打开此注释代码
			//throw new UnrollbackException(">>>>>>调用九州通达OMS接口访问异常!<<<<<");
		}
		return result;
	}
```
因为是调用外部系统的接口，所以事务管理的方法要插入日志，如果抛出Exception那么即使日志插入成功也最终会回滚。下面代码的处理日志插入不会因为接口调用是否异常都会执行下去：
```java
public void updateProductStockProcess(Corporation corporation){
		Map<String, Object> keyMap=null;
		boolean flag=false;//接口调用失败
		int statusCode=102;//失败
		int interfaceType=3;//接口类型标识
		String requestBody="";
		String tenantCode=corporation.getEsCorAlias();
		// 获取商品库存记录
		List<JZTDOmsProduct> products=wmStockMapper.selectAllItemStockNumber(corporation.getEsCorId()+"");
		if(products==null||products.size()==0){
			Log.getLogger(getClass()).info(">>>>组织"+"("+corporation.getEsCorId()+")"+corporation.getEsCorName()+"没有库存记录！！！");
			return ;
		}
		Map<String,Object> map=new HashMap<String, Object>();
		map.put("Products", products);
		requestBody=JSON.toJSONString(map);
		String platformCode=EhcacheUtil.get("JZTD_OMS_PLATFORMCODE").toString();
		
		// 通过RSA生成密钥对
		try {
			keyMap = RSAUtils.genKeyPair();
		} catch (Exception e) {
			e.printStackTrace();
			return ;
		}
		// 公钥（RSA公钥加密-私钥解密）
		String publicKey=null;
		try {
			publicKey = RSAUtils.getPublicKey(keyMap);
		} catch (Exception e) {
			e.printStackTrace();
			return ;
		}
		// 模拟生成Token
		String token=SAASTokenManager.generateToken(publicKey, platformCode, tenantCode);
		// 调用九州通达OMS库存同步接口
		try {
			String result=jztdapiService.updateProductStock(token, requestBody, platformCode, tenantCode);
			JSONObject json=JSON.parseObject(result);
			if(json!=null&&json.getBooleanValue("IsSuccess")){
				flag=true;
				statusCode=101;//成功
			}
		} catch (UnrollbackException e) {
			statusCode=104;// 其他异常
			Log.getLogger(getClass()).error(">>>>调用九州通达OMS库存同步接口----访问异常:"+e.getMessage());
		}
		EbInterfaceLog log=new EbInterfaceLog(new Date(), requestBody, flag==true?0:1, 0, statusCode, interfaceType) ;
		int result=ebInterfaceLogService.insertTable(log,tenantCode );
		Log.getLogger(getClass()).error(">>>>调用九州通达OMS库存同步接口日志保存:"+(result>0?true:false));
}
```
被事务困扰了小半天了，终于解决了，故在此记录以备查阅。
### 3.改进方法

实际上Remote接口调用时可以不抛出异常，上面的远程接口代码完全没有必要做异常抛出处理，只要内部定义好返回的错误信息就可以了。

**业务处理接口定义：**

```java
/**
	 * 4.1.3商品库存同步接口
	 * 
	 * @MethodName: updateProductStock
	 * @Description:
	 * @param token
	 *            RSA加密Token
	 * @param requestBody
	 *            请求消息体
	 * @param platformCode
	 *            平台编码
	 * @param tenantCode
	 *            租户编码
	 * @return
	 * @throws UnrollbackException 因为Exception被spring所管理，如接口访问异常接口日志插入成功后仍然回滚
	 * @throws
	 */
	public String updateProductStock(String token, String requestBody,
			String platformCode, String tenantCode) ;
```
这里不再抛出异常。
**业务接口方法实现：**

```java
public void updateProductStockProcess(Corporation corporation){
        Map<String, Object> keyMap=null;
        int interfaceType=3;//接口类型标识
        String requestBody="";
        String tenantCode=corporation.getEsCorAlias();
        // 获取商品库存记录
        List<JZTDOmsProduct> products=wmStockMapper.selectAllItemStockNumber(corporation.getEsCorId()+"");
        if(products==null||products.size()==0){
            Log.getLogger(getClass()).info(">>>>组织"+"("+corporation.getEsCorId()+")"+corporation.getEsCorName()+"没有库存记录！！！");
            return ;
        }
        Map<String,Object> map=new HashMap<String, Object>();
        map.put("Products", products);
        requestBody=JSON.toJSONString(map);
        String platformCode=EhcacheUtil.get("JZTD_OMS_PLATFORMCODE").toString();
        
        // 通过RSA生成密钥对
        try {
            keyMap = RSAUtils.genKeyPair();
        } catch (Exception e) {
            e.printStackTrace();
            return ;
        }
        // 公钥（RSA公钥加密-私钥解密）
        String publicKey=null;
        try {
            publicKey = RSAUtils.getPublicKey(keyMap);
        } catch (Exception e) {
            e.printStackTrace();
            return ;
        }
        // 模拟生成Token
        String token=SAASTokenManager.generateToken(publicKey, platformCode, tenantCode);
        // 调用九州通达OMS库存同步接口
        String result=jztdapiService.updateProductStock(token, requestBody, platformCode, tenantCode);
        JSONObject json=JSON.parseObject(result);
        boolean flag=json.getBooleanValue("IsSuccess");
        int statusCode=json.getIntValue("statusCode");
        EbInterfaceLog log=new EbInterfaceLog(new Date(), requestBody, flag==true?0:1, 0, statusCode, interfaceType) ;
        int state=ebInterfaceLogService.insertTable(log,tenantCode );
        Log.getLogger(getClass()).error(">>>>调用九州通达OMS库存同步接口日志保存:"+(state>0?true:false));
}
```
此时我们不再关注远程接口是否异常，所以后面的操作就可以进行下去了。

**远程接口实现方法：**

```java
public String updateProductStock(String token, String requestBody,
            String platformCode, String tenantCode)  {
        Log.getLogger(getClass()).info("调用九州通达OMS接口.........../updateproductstock REQUEST:"+requestBody); 
        String url = EhcacheUtil.get("JZTD_OMS_API_BASE_URL").toString()    + "/updateproductstock";
        String result=null;
        try {
            result = RestTemplateUtils.post(url, requestBody, token, platformCode, tenantCode);
            Log.getLogger(getClass()).info("调用九州通达OMS接口.........../updateproductstock RESPONSE:"+result); 
            JSONObject json=JSON.parseObject(result);
            if(json.getBooleanValue("IsSuccess")){
                json.put("statusCode", 101);//成功
            }else{
                json.put("statusCode", 102);//失败
            }
            return json.toJSONString();
        } catch (Exception e) {
            Log.getLogger(getClass()).info("调用九州通达OMS接口.........../updateproductstock NO RESPONSE:"+e.getMessage()); 
            return requestExceptionInfo(e.getMessage());//异常返回
        }
}
```
**异常消息返回：**
```java
/**
     * 接口访问错误异常信息
     * 
     * @MethodName: requestExceptionInfo 
     * @Description: 
     * @param msg
     * @return
     * @throws
     */
    public String requestExceptionInfo(String msg){
        Map<String,Object> map=new HashMap<String, Object>();
        map.put("IsSuccess", false);
        map.put("OperationDesc", "接口调用异常："+msg);
        map.put("statusCode", 104);//其他异常
        return JSON.toJSONString(map);
    }
```







