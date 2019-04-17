# RestTemplate.postForObject - Error: 400 Bad Request - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年08月11日 17:53:46[boonya](https://me.csdn.net/boonya)阅读数：9234








我们需要知道，RestTmplate是基于HTTP Rest风格的请求方式，它也支持配置SSL的HTTPS访问。

我们可以对基于HTTP协议进行Header，Body等进行自定义封装，但请求Request都存在一个限制值大小范围。

### 1.代码对比



```java
/**
	 * 
	 * 发送post请求
	 * 
	 * @param url
	 *            请求URL地址
	 * @param data
	 *            json数据
	 * @param token
	 *            RSA加密token【RSA({PlatformCode+TenantCode+DateTime.Now()})//平台代码
	 *            +会员/租户代码+当前时间，然后进行RSA加密】
	 */
	public static String post(String url, String data, String token)
			throws Exception {
		HttpHeaders headers = new HttpHeaders();
		/*headers.add(HttpHeadersImpl.ACCEPT, "application/json");
		headers.add(HttpHeadersImpl.ACCEPT_ENCODING, "gzip");
		headers.add(HttpHeadersImpl.CONTENT_ENCODING, "UTF-8");
		headers.add(HttpHeadersImpl.CONTENT_TYPE,
				"application/json; charset=UTF-8");
		headers.add(HttpHeadersImpl.COOKIE, token);
		headers.add("Token", token);*/
		headers.add("Accept", "application/json");
		headers.add("Accpet-Encoding", "gzip");
		headers.add("Content-Encoding", "UTF-8");
		headers.add("Content-Type", "application/json; charset=UTF-8");
		headers.add("Token", token);

		HttpEntity<String> formEntity = new HttpEntity<String>(data, headers);
		return RestTemplateUtils.getInstance().postForObject(url, formEntity, String.class);
	}
```


将需要的属性定义到Header：





```java
/**
	 * 
	 * 发送post请求
	 * 
	 * @param url
	 *            请求URL地址
	 * @param data
	 *            json数据
	 * @param token
	 *            RSA加密token【RSA({PlatformCode+TenantCode+DateTime.Now()})//平台代码
	 *            +会员/租户代码+当前时间，然后进行RSA加密】
	 */
	public static String postJZTD(String url, String data, String token)
			throws Exception {
		String platformCode = EhcacheUtil.get("JZTD_OMS_PLATFORMCODE").toString();
		String memberCode =  EhcacheUtil.get("JZTD_OMS_MEMBERCODE").toString();
		String warehouseCode =  EhcacheUtil.get("JZTD_OMS_WAREHOUSECODE").toString();
		HttpHeaders headers = new HttpHeaders();
		/*headers.add(HttpHeadersImpl.ACCEPT, "application/json");
		headers.add(HttpHeadersImpl.ACCEPT_ENCODING, "gzip");
		headers.add(HttpHeadersImpl.CONTENT_ENCODING, "UTF-8");
		headers.add(HttpHeadersImpl.CONTENT_TYPE,
				"application/json; charset=UTF-8");
		headers.add(HttpHeadersImpl.COOKIE, token);
		headers.add("Token", token);*/
		headers.add("Accept", "application/json");
		headers.add("Accpet-Encoding", "gzip");
		headers.add("Content-Encoding", "UTF-8");
		headers.add("Content-Type", "application/json; charset=UTF-8");
		headers.add("Token", token);
		headers.add("PlatformCode", platformCode);
		headers.add("MemberCode", memberCode);
		headers.add("WarehouseCode", warehouseCode);

		HttpEntity<String> formEntity = new HttpEntity<String>(data, headers);
		return RestTemplateUtils.getInstance().postForObject(url, formEntity, String.class);
	}
```



按照经验，第一种不出意外是可以正确访问的，第二种就值得商榷了，因为加入了非HTTP定义属性字段。

### 2.解决方案

1.去掉非HTTP Header Request属性。

2.缩短HTTP Header Request属性的值。


3.了解更多关于HTTP的设置相关，以及Spring RestTemplate的HTTP Header限制。

一个简洁的Header设置：



```java
public void foo(String state) {
        MultiValueMap<String, Object> headers = new LinkedMultiValueMap<String, Object>();
        headers.add("Accept", "application/json");
        headers.add("Content-Type", "application/json");
        String requestBody = "{\"statename\":\"" + state + "\"}";
        HttpEntity request = new HttpEntity(requestBody, headers);
        String apiResponse = getRestTemplate().postForObject(apiEndpoint,request, String.class);
        System.out.println(apiResponse);
     }
```

注：验证交互的数据最好定义在body里面，尽量让你的HTTP Header 更轻量。





