# Spring3 MVC中使用Swagger生成API文档 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2016年02月19日 23:16:37[gloomyfish](https://me.csdn.net/jia20003)阅读数：46445








Spring3 MVC中使用Swagger生成API文档

一：Swagger介绍

Swagger是当前最好用的Restful API文档生成的开源项目，通过swagger-spring项目

实现了与SpingMVC框架的无缝集成功能，方便生成spring restful风格的接口文档，

同时swagger-ui还可以测试spring restful风格的接口功能。其官方网站为：



[http://swagger.io/](http://swagger.io/)

二：Swagger集成Spring3 MVC步骤

Swagger集成springMVC步骤大致只有如下几步：

1.在pom.xml文件中添加swagger相关的依赖

```
<!-- swagger API document -->
		<dependency>
			<groupId>com.mangofactory</groupId>
			<artifactId>swagger-springmvc</artifactId>
			<version>0.6.5</version>
		</dependency>
```



2.创建classpath路径下创建一个swagger.properties, 添加如下内容：

documentation.services.version=1.0

documentation.services.basePath=http://localhost:8080/yourcontextpath

3.在springMVC的main-servlet.xml文件添加如下配置



```
<context:property-placeholder location="classpath:swagger.properties" />
	<bean id="documentationConfig" class="com.mangofactory.swagger.configuration.DocumentationConfig" />
```



4.重新打包部署你的项目到WEB服务器，访问地址

http://localhost:8080/your-contextpath /api-docs即可看到注解生成的API说明



三：常见swagger注解一览与使用

APIs.@Api

@ApiClass

@ApiError

@ApiErrors

@ApiOperation

@ApiParam

@ApiParamImplicit

@ApiParamsImplicit

@ApiProperty

@ApiResponse

@ApiResponses

@ApiModel

在代码中使用例子：



```java
import java.util.HashMap;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;

import com.wordnik.swagger.annotations.ApiOperation;

@Controller
@RequestMapping("/api/swagger")
public class SwaggerDemoController {
	private static final Logger logger = LoggerFactory.getLogger(SwaggerDemoController.class);
	
	@ApiOperation(value = "query api basic information")
	@RequestMapping(value = "/info", method = RequestMethod.GET)
	@ResponseBody
	public Map<String, String> queryAPIInfo() {
		logger.info("查询更新新版本号");
		Map<String, String> map = new HashMap<String, String>();
		map.put("language", "Java");
		map.put("format", "JSON");
		map.put("tools", "swagger");
		map.put("version", "1.0");
		return map;
	}
	
	@ApiOperation(value = "query data with parameters")
	@RequestMapping(value = "/data", method = RequestMethod.GET)
	@ResponseBody
	public Map<String, String> queryData(@RequestParam String words) {
		logger.info("查询更新新版本号");
		Map<String, String> map = new HashMap<String, String>();
		map.put("keyword", words);
		map.put("data", "this is demo data");
		return map;
	}
}
```



四：运行swagger-ui测试接口

下载swagger-ui的最新版本到本地，改名为swagger-ui，把dist下面的部署到tomcat

或者任何WEB服务器上，启动后访问如下地址: [http://localhost:8080/swagger-ui](http://localhost:8080/swagger-ui)

注意把swagger-ui中的index.html中的[http://petstore.swagger.io/v2/swagger.json](http://petstore.swagger.io/v2/swagger.json)改为

http://localhost:8080/your-contextpath /api-docs保存，然后在启动WEB服务器，

显示如下：
![](https://img-blog.csdn.net/20160219231149116)
展开输入参数以后，点击【try it out】即可测试接口，查看返回数据。
**注意：**加上之后启动报Bean not found mapping之类的错误，请在对应

xml文件中加上如下的配置：



```
<context:annotation-config />
<mvc:default-servlet-handler />
```



