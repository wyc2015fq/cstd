# SpringMVC中@RequestBody的应用 - 零度的博客专栏 - CSDN博客
2016年05月30日 13:58:23[零度anngle](https://me.csdn.net/zmx729618)阅读数：7759
        一个比较复杂的数据结构在前台组合Json数据后传递到后台，之前所有的前后台数据转换都是自己写的，比如js将json转为字符串的方法，后台接收json转成实体的方法。使用SpringMVC之后，可以使用@RequestBody注释接受参数，具体使用如下：
       首先，@RequestBody需要接的参数是一个String化的json，这里直接使用JSON.stringify(json)这个方法来转化；其次，@RequestBody从名称上来看也就是说要读取的数据在请求体里，所以要发post请求；第三，要设置请求的内容类型contentType为
 contentType:"application/json，明确的告诉服务器发送的内容是json数据，而默认的contentType是application/x-www-form-urlencoded; charset=UTF-8那么可以开始写出代码了。
前台代码：
```
$.ajax({
			type: "post",
			contentType:"application/json",
			url: "repairs/saveDispatches",
			data: JSON.stringify(dispatchesDTO),
			success: function(data){
		    	if(!data.success){
		          alertError("操作失败");
		    	}else{
			    alertSuccess("操作成功");
			}
		})
```
       后台怎么接呢？这是就需要@RequestBody出马了，
后台代码：
```
@RequestMapping("/repairs/saveDispatches")
	public void saveDispatches(@RequestBody DispatchesDTO dispatchesDTO,
			HttpServletResponse response) throws IOException {
		dispatchesService.saveDispatches(dispatchesDTO);
		success(response);
	}
```
       很简单吧，无需手动进行json与实体的转换，只要能映射上去（也就是字段名和json的key相对应，value可以匹配上数据类型），那么就可以直接转换。如何定义“能映射上去”呢？若是json中的key在实体中都能找到对应的field，那么就是“能映射上去”，也就是说：前台传入的json中的key在实体中必须要存在，不然就会报错，比如下面这个前台传入的json中多了一个key为id的键值对，会出现如下信息：
org.springframework.http.converter.HttpMessageNotReadableException: Could not read JSON: Unrecognized field "id" (Class com.westsoft.kft.repairs.dto.DispatchesDTO), not marked as ignorable。
      其实，要使用@RequestBody这个注解还需要额外配置一个Bean，也就是自动帮我们接收json并转成对象的消息转换器：messageConverter
由于我使用的是Spring3.1之后的版本，那么配置在RequestMappingHandlerAdapter下：
```
<!--Spring3.1开始的注解 HandlerAdapter -->
	<bean
		class="org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter">
		<property name="messageConverters">
			<list>
				<bean
					class="org.springframework.http.converter.json.MappingJacksonHttpMessageConverter">
				</bean>
			</list>
		</property>
	</bean>
```
       当然也可以通过<mvc:annotation-driven />来简单的配置，这里就不作多介绍了，最后再要提到几点：
        一、  若1、前台发送请求时未指定contentType为json/application； 2、Spring中未配置messageConverter，那么会报错误415，也就是类型不支持。前者是因为Content
 type 'application/x-www-form-urlencoded;charset=UTF-8' not supported，而后者是因为Content type 'application/json;charset=UTF-8' not supported。
       二、由于Spring中使用的json转换用到的是jackson，因此需要引入jackson包。
