# WMS系统对接OMS接口经验 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年08月04日 13:38:55[boonya](https://me.csdn.net/boonya)阅读数：6924








![](https://img-blog.csdn.net/20160804125331836)

WMS对接OMS是实现电商平台重要的一环，这里将本人从WMS对接OMS过程中好的经验进行一个简单的总结。**本篇以库存同步为例**：

### 1.定义规范

**对外接口处理：**严格定义接口说明，调用双方接口提供方和调用方。

**内部逻辑处理：**


（1）如果以WMS为主：先调用WMS内部逻辑处理，处理完成后在调用OMS接口，中间如果OMS接口调用失败，WMS实现接口重发机制进行数据同步。

（2）如果以OMS为主：先调用OMS接口，然后再执行WMS业务逻辑，如果OMS失败WMS就没有必要往下执行了。


### 2.数据约定

请求协议约定：HTTP/HTTPS(默认使用HTTP)


对数据格式进行限定：JSON/XML（默认使用JSON）

对请求头进行相关约定：



```java
Accpet: application/json
Accpet-Encoding: gzip
Content-Encoding: UTF-8
Content-Type: application/json; charset=UTF-8
PlatformCode:{由平台分配}
MemberCode:{会员编码，对应WMS租户编码,由平台分配}
WarehouseCode:{仓库编码，双方约定}
Token: RSA(PlatformCode+ MemberCode + WarehouseCode +DateTime.Now())//签名：平台代码+会员/租户代码+仓库编码+当前时间，然后进行RSA加密，RSA密钥由平台统一分配。请求当前时间与OMS服务器时间差不大于10分钟。
```

### 3.接口的请求和响应

**接口名称：WMS同步商品库存至OMS**


请求格式：



```java
{ 
 “Products”:
[
{“ProductCode”: “A001B0001”, “ProductNum”:50, “ AvailableProductNum”:45, “ProductUnit”: “袋”,“ProductDate”: “2016-08-04 12:00:03”,},
{“ProductCode”: “A001B0002”, “ProductNum”:50, “AvailableProductNum”:45, “ProductUnit”: “袋”, “ProductDate”: “2016-08-04 12:00:03”,}
]
}
```
响应格式：



```java
{ 
“IsSuccess”:“true”,
“OperationDesc”:“成功”
}
```

### 4.HTTP请求交付数据



```java
@Override
	public String updateProductStock(String token, String requestBody) {
		Log.getLogger(getClass()).info(
				"调用九州通达OMS接口.........../updateproductstock REQUEST:" + requestBody);
		String url = EhcacheUtil.get("JZTD_OMS_API_BASE_URL").toString() + "/updateproductstock";
		String result = null;
		try {
			result = RestTemplateUtils.postJZTD(url, requestBody, token);
			Log.getLogger(getClass()).info("调用九州通达OMS接口.........../updateproductstock RESPONSE:"+ result);
			JSONObject json = JSON.parseObject(result);
			if (json.getBooleanValue("IsSuccess")) {
				json.put("statusCode", 101);// 成功
			} else {
				json.put("statusCode", 102);// 失败
			}
			return json.toJSONString();
		} catch (Exception e) {
			Log.getLogger(getClass()).info("调用九州通达OMS接口.........../updateproductstock NO RESPONSE:"+ e.getMessage());
			return requestExceptionInfo(e.getMessage());// 异常返回
		}
	}
```

### 5.WMS逻辑处理库存相关

**严格按照Controller>service>dao（Mapper-mybatis）层的规范：**

1、Dao层实现数据访问接口：只提供单一地真删改查业务处理数据访问。

2.  Service实现业务访问接口：Aservice用到其他的数据Bservice应注入被调用的Bservice对象，而不从BDao注入调用。


3. Controller访问逻辑业务视图控制：都从Service进行调用。

**好处：**便于修改处理逻辑，增强代码可扩展性和灵活性。

### 6.实现WMS到OMS业务处理逻辑快速应答

因为OMS的特性只关注商品的：商品编码、可用数量、总数量、商品单位、商品批次、商品生产日期等信息。所以WMS的很多处理逻辑只要牵涉到库存修改就需要进行库存同步。如WMS流程：收货、上架、波次、出库、整货拆散等等都需要同步商品库存到OMS。我们需要在insert*、update*、delete*等方法处实现对应的OMS库存同步处理，这样改动起来就相当方便。

由于以WMS业务处理相对比OMS远程交互数据要快，所以我们在同步过程中采用线程处理，而不需要等待同步数据完成。相当于，WMS没操作一步数据涉及到库存的都可以自己做持久化更新处理，至于线程有没有执行成功，我们使用接口日志来实现重发。

### 7.真实处理代码示例

库存管理Service新增库存：



```java
@Override
public Integer insert(WmStock model) {
		// 九州通达OMS同步处理
		if(EhcacheUtil.get("OMS_API_ISENABLE").toString().equals("true")){
			int flag=wmStockMapper.insert(model);
			if(flag>0){
				Log.getLogger(getClass()).info("<<<<<<<<<<<<<<更新数据到九州通达OMS>>>>>>>>>>>>EVENT:insert(WmStock model):"+(flag>0?true:false));
				// 线程更新库存(increment)
				this.updateStockToOMSBySkuIndentifyInfo(true, model);
			}
			return flag;
		}
		// 一般处理 
		return wmStockMapper.insert(model);
}
```
库存管理Service修改库存：



```java
@Override
public Integer updateByPrimaryKeySelective(WmStock model) {
		// 九州通达OMS同步处理
		if(EhcacheUtil.get("OMS_API_ISENABLE").toString().equals("true")){
			String memberCode = EhcacheUtil.get("JZTD_OMS_MEMBERCODE").toString();
			// 在update之前查询到修改之前的库存数据
			WmStock oldStock=wmStockMapper.selectByPrimaryKey(model.getWmstId(), memberCode);
			int flag=wmStockMapper.updateByPrimaryKeySelective(model);
			if(flag>0){
				Log.getLogger(getClass()).info("<<<<<<<<<<<<<<更新数据到九州通达OMS>>>>>>>>>>>>EVENT:updateByPrimaryKeySelective(WmStock model):"+(flag>0?true:false));
				// 计算库存差值
				oldStock.setWmstNowNumber(model.getWmstNowNumber()-oldStock.getWmstNowNumber());
				oldStock.setWmstEnabledNumber(model.getWmstEnabledNumber()-oldStock.getWmstEnabledNumber());
				// 线程更新库存(increment)
				this.updateStockToOMSBySkuIndentifyInfo(true, oldStock);
			}
			return flag;
		}
		// 一般处理 
		return wmStockMapper.updateByPrimaryKeySelective(model);
}
```
库存管理Service删除库存：

```java
@Override
public Integer deleteByPrimaryKey(Integer wmstId,String companyCode){
		// 九州通达OMS同步处理
		if(EhcacheUtil.get("OMS_API_ISENABLE").toString().equals("true")){
			// 在delete之前查询到修改之前的库存数据
			WmStock oldStock=wmStockMapper.selectByPrimaryKey(wmstId, companyCode);
			int flag=wmStockMapper.deleteByPrimaryKey(wmstId, companyCode);
			if(flag>0){
				Log.getLogger(getClass()).info("<<<<<<<<<<<<<<更新数据到九州通达OMS>>>>>>>>>>>>EVENT:deleteByPrimaryKey(Integer wmstId,String companyCode):"+(flag>0?true:false));
				// 线程更新库存(increment)
				this.updateStockToOMSBySkuIndentifyInfo(false, oldStock);
			}
			return flag;
		}
		// 一般处理 
		return wmStockMapper.deleteByPrimaryKey(wmstId, companyCode);
}
```
线程更新公共方法：



```java
@Override
public void updateStockToOMSBySkuIndentifyInfo(boolean isIncrement,WmStock wmStock) {
		String token=SAASTokenManager.getToken();
		Map<String,Object> map=new HashMap<String, Object>();
		int memberCode =  Integer.valueOf(EhcacheUtil.get("JZTD_OMS_MEMBERCODE").toString());
		CdWhItme item=CdWhItmeService.selectByPrimaryKeyOfTabel(memberCode, wmStock.getWmstSkuId()); 
		List<JZTDOmsProduct> list=new ArrayList<JZTDOmsProduct>();
		String productCode=item==null?"":item.getCdskItemCode();
		String productDate=wmStock.getWmstProductDate()==null?null:DateUtil.date2String(wmStock.getWmstProductDate(), DateUtil.PATTERN_STANDARD);
		JZTDOmsProduct product=null;
		if(isIncrement){
			product=new JZTDOmsProduct(productCode, wmStock.getWmstNowNumber(), wmStock.getWmstEnabledNumber(), wmStock.getWmstSkuUnit(), wmStock.getWmstSkuBatch(),productDate );
		}else{
			product=new JZTDOmsProduct(productCode, wmStock.getWmstNowNumber()==0?0:-wmStock.getWmstNowNumber(), wmStock.getWmstEnabledNumber()==0?0:-wmStock.getWmstEnabledNumber(), wmStock.getWmstSkuUnit(), wmStock.getWmstSkuBatch(),productDate );
		}
		list.add(product);
		map.put("Products", list);
		String requestBody= Tools.toJson(map);
		Log.getLogger(getClass()).info(">>>>>>>>开启线程处理库存同步到九州通达OMS:.................线程处理中---DATA:"+requestBody);
		new UpdateProductStockThread(jztdapiService, ebInterfaceLogService,requestBody, token, StaticProperty.OMS_WMS_INTERFACE_UPDATEPRODUCTSTOCK).start();				
}
```
采用每次更新进行**正负差值计算**，**OMS有则实现累加更新（正负不用考虑直接累加）**，无则新增库存即可，也简化了两边的交互数据量（最开始我们考虑的是采用调度任务来执行，如数据量大会造成很多不必要的数据处理，基于此后来采用分布同步的方式，将同步的压力分散到不同的业务中去）。
### 8.案例测试说明

下面是WMS同步到OMS接口日志所保存的数据：


![](https://img-blog.csdn.net/20160804133214712)


第一行（id=5）：收货流程数据库存新增（可用数量0，总数500）。

第二行（id=6）：上架流程数据库存更新（可用数量500，总数500）。

第三行（id=7）：出库波次执行库存更新（可用数量-19，总数0）。

第四行（id=8）：出库确认流程库存更新（可用数量0，总数-19）。


上面的过程是进行了商品的**收货、上架、出库**流程的库存数据更新。我们入库上架了500，出库了19，最终我们实现了WMS、OMS商品可用数量481，总数量也是481的库存量同步。




