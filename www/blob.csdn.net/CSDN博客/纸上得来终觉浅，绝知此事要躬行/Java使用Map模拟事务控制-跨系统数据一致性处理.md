# Java使用Map模拟事务控制-跨系统数据一致性处理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年08月15日 18:27:18[boonya](https://me.csdn.net/boonya)阅读数：1114








首先，在复杂的业务系统中，尤其是跨系统交互如何保证两边数据一致性是极其重要的。下面就来模拟一下使用Map与Mybatis实现的事务控制的实例：

### 1.业务场景描述

WMS-OMS库存同步，实现两边库存一致性。

### 2.WMS-OMS接口交互缺陷

业务处理逻辑代码：


```java
try{

     // ====需要事务的业务操作====

    //WMS操作1

    //WMS操作2

    //WMS/操作3

    //OMS操作1----异步线程处理

    //WMS操作4

    //OMS操作2----异步线程处理

   //WMS操作5

}catch(Exception e){

         // 事务回滚

         TransactionAspectSupport.currentTransactionStatus().setRollbackOnly();

}finally{

         // 其他处理

}
```

这里处理过程中，从WMS操作4开始就已经出现弊端了，万一后面操作异常WMS这边的数据都会回滚，而线程已经执行不能够回滚处理就存在两边数据不一致的情况。

### 3.使用Map管理两边库存差异数据

**以新增库存记录到OMS为例：**

```java
public Integer insert(String key,WmStock model) {
		String memberCode = EhcacheUtil.get("JZTD_OMS_MEMBERCODE").toString();
		// 九州通达OMS同步处理
		if(EhcacheUtil.get("OMS_API_ISENABLE").toString().equals("true")&&memberCode.equals(model.getWmstCustomerCode()+"")){
			int flag=wmStockMapper.insert(model);
			if(flag>0){
				Log.getLogger(getClass()).info("<<<<<<<<<<<<<<更新数据到九州通达OMS>>>>>>>>>>>>EVENT:insert(WmStock model):"+(flag>0?true:false));
				// 线程更新库存(increment)
				//this.updateStockToOMSBySkuIndentifyInfo(true, model);
				// ===放入待处理缓存对象===
				List<JZTDOmsProduct> stocks=StockMapHandler.get(key);
				if(stocks==null||stocks.size()==0){
					List<JZTDOmsProduct> stocks2=new ArrayList<JZTDOmsProduct>();
					JZTDOmsProduct product=getProductDifferenceNumberFromStock(true, model);
					stocks2.add(product);
					StockMapHandler.put(key, stocks2);
				}else{
					JZTDOmsProduct product=getProductDifferenceNumberFromStock(true, model);
					stocks.add(product);
					StockMapHandler.put(key, stocks);
				}
			}
			return flag;
		}
		// 一般处理 
		return wmStockMapper.insert(model);
	}
```
注：新增库存两边直接同步不需要计算差异。
**交互数据结构：**
```java
/**
	 * 封装差异库存数据
	 * 
	 * @MethodName: getProductDifferenceNumberFromStock 
	 * @Description: 
	 * @param key
	 * @param isIncrement
	 * @param wmStock
	 * @return
	 * @throws
	 */
	private JZTDOmsProduct getProductDifferenceNumberFromStock(boolean isIncrement,WmStock wmStock){
		int memberCode =  Integer.valueOf(EhcacheUtil.get("JZTD_OMS_MEMBERCODE").toString());
		CdWhItme item=CdWhItmeService.selectByPrimaryKeyOfTabel(memberCode, wmStock.getWmstSkuId()); 
		String productCode=item==null?"":item.getCdskItemCode();
		String productDate=wmStock.getWmstProductDate()==null?null:DateUtil.date2String(wmStock.getWmstProductDate(), DateUtil.PATTERN_STANDARD);
		CdCustomer customer=cdWhCustomerService.selectByCustomerName(wmStock.getWmstCustomer(), memberCode+"");
		String customerCode=customer==null?"":customer.getCdstCustomerCode();
		JZTDOmsProduct product=null;
		if(isIncrement){
			product=new JZTDOmsProduct(customerCode,productCode, wmStock.getWmstNowNumber(), wmStock.getWmstEnabledNumber(), wmStock.getWmstSkuUnit(), wmStock.getWmstSkuBatch(),productDate );
		}else{
			product=new JZTDOmsProduct(customerCode,productCode, wmStock.getWmstNowNumber()==0?0:-wmStock.getWmstNowNumber(), wmStock.getWmstEnabledNumber()==0?0:-wmStock.getWmstEnabledNumber(), wmStock.getWmstSkuUnit(), wmStock.getWmstSkuBatch(),productDate );
		}
		return product;
	}
	
	@Override
	public void executeStockToOMSBySkuIndentifyInfo(String key,String token) {
		List<JZTDOmsProduct> produts=StockMapHandler.get(key);
		if(produts==null||produts.size()==0){
			Log.getLogger(getClass()).info(">>>>>>>>开启线程处理库存同步到九州通达OMS:.................验证缓存库存同步数据失败:key="+key);
			return ;
		}
		Map<String,Object> map=new HashMap<String, Object>();
		map.put("Products", produts);
		String requestBody= Tools.toJson(map);
		// 线程处理库存同步
		Log.getLogger(getClass()).info(">>>>>>>>开启线程处理库存同步到九州通达OMS:.................线程处理中---DATA:"+requestBody);
		new UpdateProductStockThread(jztdapiService, ebInterfaceLogService,requestBody, token, StaticProperty.OMS_WMS_INTERFACE_UPDATEPRODUCTSTOCK,true).start();
	}
```

**创建Handler：**

```java
package com.wlyd.fmcgwms.util;

import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import com.wlyd.fmcgwms.persistence.beans.api.JZTDOmsProduct;
import com.wlyd.fmcgwms.service.basic.WmStockService;

/**
 * 库存缓存对象处理类
 * 
 * @packge com.wlyd.fmcgwms.util.StockMapHandler
 * @date 2016年8月15日 上午10:30:10
 * @author pengjunlin
 * @comment
 * @update
 */
public class StockMapHandler{

	/**
	 * WmStock库存差异对象存储
	 */
	public static Map<String, List<JZTDOmsProduct>> map = new ConcurrentHashMap<String, List<JZTDOmsProduct>>(
			5000);

	/**
	 * 获取List对象
	 * 
	 * @MethodName: get
	 * @Description:
	 * @param key
	 * @return
	 * @throws
	 */
	public static List<JZTDOmsProduct> get(String key) {
		if (map.containsKey(key)) {
			return map.get(key);
		}
		return null;
	}

	/**
	 * 存储List对象
	 * 
	 * @MethodName: put
	 * @Description:
	 * @param key
	 * @param produts
	 * @throws
	 */
	public static void put(String key, List<JZTDOmsProduct> produts) {
		map.put(key, produts);
	}

	/**
	 * 移除List对象
	 * 
	 * @MethodName: put
	 * @Description:
	 * @param key
	 * @throws
	 */
	public static void remove(String key) {
		if (map.containsKey(key)) {
			map.remove(key);
		}
	}
	
	/**
	 * 清除Map中存储的所有对象
	 * 
	 * @MethodName: clear 
	 * @Description: 
	 * @throws
	 */
	public static void clear(){
		map.clear();
	}
	

	public class Handler implements Runnable{
		
		private WmStockService wmStockService;
		
		private String key;
		
		public Handler( WmStockService wmStockService, String key){
			this.wmStockService=wmStockService;
			this.key=key;
		}

		@Override
		public void run() {
			wmStockService.executeStockToOMSBySkuIndentifyInfo(key, SAASTokenManager.getToken());
		}
		
	}

}
```

**测试类方法：**
```java
package fmcgwms;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import com.wlyd.fmcgwms.service.basic.WmStockService;
import com.wlyd.fmcgwms.util.StockMapHandler;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = "classpath:applicationContext.xml")
public class StockMapHandlerTest {
	
	@Autowired
	WmStockService wmStockService;
	
	@Test
	public void testHandler(){
		
	      String key="10000"+System.currentTimeMillis();
        
        StockMapHandler stockMapHandler=new StockMapHandler();
        
        new Thread(stockMapHandler.new Handler(jztdapiService,ebInterfaceLogService, key,false)).start();
        
        try {
            Thread.sleep(8000);// 线程等待执行，避免未执行完现场spring容器已销毁
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
	}

}
```

### 4.Map控制的缓存数据调用

```java
//对应的多个业务事务方法处理
public void doBusiness(String key){ 
 //key="uniqueString";
 try{

     // ====需要事务的业务操作====

    //WMS操作1

    //WMS操作2

    //WMS/操作3

    //OMS操作1----异步线程处理（不执行线程）

    //WMS操作4（For instance:insertStock----map set different stock）

    //OMS操作2----异步线程处理（不执行线程）

    //WMS操作5(For instance:updateStock----map set different stock)

 }catch(Exception e){
      // 事务回滚
      TransactionAspectSupport.currentTransactionStatus().setRollbackOnly();

 }finally{
        // 统一执行异步线程同步库存
       //handler 处理
       StockMapHandler stockMapHandler=new StockMapHandler();
	stockMapHandler.new Handler(wmStockService, key);
       // 移除该业务操作的缓存对象     
       StockMapHandler.remove(key); 
 }
}
```

# 5.关于key是否会重复的问题

在StockMapHandler.java 中新增一个生成：企业编码+UUID的唯一标识



```java
/**
	 * 生成企业的UUID编码
	 * 
	 * @MethodName: generateUuidKey 
	 * @Description: 
	 * @param esCorCode
	 * @return
	 * @throws
	 */
	public static String generateUuidKey(String esCorCode){
		String uuid=UUID.randomUUID().toString();
		uuid=uuid.replaceAll("-", "");
		uuid=esCorCode+uuid;
		return uuid;
	}
```

另外需要注意，以下操作应该放在线程内执行，避免出错：




```java
// 移除该业务操作的缓存对象     
StockMapHandler.remove(key);
```

# 6.测试UUID的唯一性

原理：入库key不重复，那么map输出的长度就是循环的长度。




```java
package ebwms;

import java.util.Map;
import java.util.UUID;
import java.util.concurrent.ConcurrentHashMap;

public class Test {
	
	public static Map<String, Object> map = new ConcurrentHashMap<String,Object>(
			5000);
	
	public String generateUuidKey(String esCorCode){
		String uuid=UUID.randomUUID().toString();
		uuid=uuid.replaceAll("-", "");
		uuid=esCorCode+uuid;
		return uuid;
	}
	
	public static void main(String[] args) {
		
		
		Test t=new Test();
		
		// 快速验证1000000个UUID
		for (int i = 1; i <= 1000000; i++) {
			String key=t.generateUuidKey("P0000007");
			map.put(key, key);
		}
		
		System.out.println(map.size());
		
		try {
			Thread.sleep(20000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}

}
```
运行测试输出：1000000.



至此，我们的事务控制配合就完美解决了线程引起的数据不一致问题。executeStockToOMSBySkuIndentifyInfo实际上就是Handler线程需要处理的方法，可以将UpdateProductStockThread线程内部的处理放到Handler稍加修改，避免线程中又开线程。









