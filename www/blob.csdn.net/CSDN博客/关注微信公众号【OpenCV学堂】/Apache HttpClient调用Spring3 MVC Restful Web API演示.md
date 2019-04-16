# Apache HttpClient调用Spring3 MVC Restful Web API演示 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2015年12月01日 16:50:05[gloomyfish](https://me.csdn.net/jia20003)阅读数：8007








**Apache HttpClient调用Spring3 MVC Restful Web API演示**


使用HttpRequest提交数据与HttpResponse响应客户端都是支持json数据格式，实现在HTTP Client端调用Spring3 MVC Restful API例子。需要用到的maven依赖如下：



```
<dependency>
	<groupId>org.apache.httpcomponents</groupId>
	<artifactId>httpclient</artifactId>
	<version>4.5.1</version>
</dependency>
<dependency>
	<groupId>com.google.code.gson</groupId>
	<artifactId>gson</artifactId>
	<version>2.5</version>
</dependency>
```



添加了maven依赖之后，就可以先完成HttpClient演示程序，这里分别实现了从服务器上拉取一个list对象、提交一个对象json数据到服务器上请求创建两个操作，分别对应Http的GET与POST请求。完整的实现代码如下：



```java
package com.httpclient.demo;

import java.io.IOException;
import java.util.List;

import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.HttpClientBuilder;
import org.apache.http.util.EntityUtils;

import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;

public class HttpClientDemo {

	public static void main(String[] args) {
		HttpClient httpClient = HttpClientBuilder.create().build(); // Use this
																	// instead
		HttpGet request = new HttpGet("http://localhost:8080/clotherdye/api/myTest/list");
		request.addHeader("content-type", "application/json");
		request.addHeader("Accept", "application/json");
		try {
			// get with json
			HttpResponse httpResponse = httpClient.execute(request);
			String response = EntityUtils.toString(httpResponse.getEntity());
			Gson gson = new Gson();
			List<PersonDto> embossList = gson.fromJson(response, new TypeToken<List<PersonDto>>(){}.getType());
			if(embossList != null && embossList.size() > 0)
			{				
				System.out.println("Name = " + embossList.get(0).getName());
				System.out.println("Country = " + embossList.get(0).getCountry());
				System.out.println("dddddddddd");
			}
			
			// post data with json
			HttpPost httpPost = new HttpPost("http://localhost:8080/clotherdye/api/myTest/create");
			httpPost.addHeader("content-type", "application/json");
			httpPost.addHeader("Accept", "application/json");
			PersonDto dto = new PersonDto();
			dto.setName("http-client");
			dto.setHobby("json parser");
			dto.setCountry("US");
			dto.setDegree("本科");
			dto.setOccupy("tester");
			String jsonString = gson.toJson(dto);
			StringEntity entity = new StringEntity(jsonString);
			httpPost.setEntity(entity);
			httpResponse = httpClient.execute(httpPost);
			response = EntityUtils.toString(httpResponse.getEntity());
			System.out.println(response);
			
		} catch (ClientProtocolException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			
		}
	}
}
```



Person对象类的代码如下：



```
public class PersonDto {
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getCountry() {
		return country;
	}
	public void setCountry(String country) {
		this.country = country;
	}
	public String getDegree() {
		return degree;
	}
	public void setDegree(String degree) {
		this.degree = degree;
	}
	public String getOccupy() {
		return occupy;
	}
	public void setOccupy(String occupy) {
		this.occupy = occupy;
	}
	public String getHobby() {
		return hobby;
	}
	public void setHobby(String hobby) {
		this.hobby = hobby;
	}
	private String name;
	private String country;
	private String degree;
	private String occupy;
	private String hobby;
}
```

服务器端是基于Spring3 MVC开发的Controller接口类，代码如下



```java
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.ResponseBody;

@Controller
@RequestMapping("/api/myTest")
public class RestfulAPIController {
	
	
	@RequestMapping("/list")
	public @ResponseBody List<PersonDto> getList() {
		List<PersonDto> persons = new ArrayList<PersonDto>();
		PersonDto p1 = new PersonDto();
		p1.setCountry("中国");
		p1.setDegree("本科");
		p1.setHobby("编程");
		p1.setName("zhigang");
		p1.setOccupy("程序员");
		
		PersonDto p2 = new PersonDto();
		p2.setCountry("中国");
		p2.setDegree("本科");
		p2.setHobby("图像处理");
		p2.setName("jia20003");
		p2.setOccupy("码农");
		
		persons.add(p1);
		persons.add(p2);
		System.out.println("start to get PersonDto list");
		return persons;
	}
	
	@RequestMapping(value = "/create", method = RequestMethod.POST)
	@ResponseBody
	public Map<String, Object> createMember(@RequestBody PersonDto dto)
	{
		System.out.println(dto.getName());
		System.out.println(dto.getOccupy());
		Map<String, Object> map = new HashMap<String, Object>();
		map.put("success", dto.getName() + "创建成功!");
		return map;
	}
}
```



**需要特别说明的是Restful Web API应该严格遵守Http的四个操作：**

GET对应查询操作

POST对应创建操作

PUT对应更新操作

DELETE对应删除操作

不应该再有其它任何操作，也就是说从HttpClient发过来的Request类型只能

是上述四种之一。否则不能称为Restful API。




JSON对象解析在HttpClient端我用了google json解析包，比较好用

推荐大家用。



