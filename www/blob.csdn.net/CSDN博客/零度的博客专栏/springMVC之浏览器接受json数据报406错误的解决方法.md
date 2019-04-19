# springMVC之浏览器接受json数据报406错误的解决方法 - 零度的博客专栏 - CSDN博客
2017年01月04日 10:45:36[零度anngle](https://me.csdn.net/zmx729618)阅读数：210标签：[spring mvc](https://so.csdn.net/so/search/s.do?q=spring mvc&t=blog)
个人分类：[SpringMVC](https://blog.csdn.net/zmx729618/article/category/6248665)
                
1.我后台测试代码如下:
@RequestMapping("/json")
@ResponseBody public 
	String json() {
		return "this is json";
	}
	
	@RequestMapping("/json2")
	@ResponseBody public 
	List<User> json2() {
		List<User> users = new ArrayList<User>();
		User u = new User();
		u.setAccount("aa");
		users.add(u);
		return users;
	}
在浏览器访问这两个方法时,只有json正确,而json2就报406,说明spring没有处理好对象序列化成json的问题,最后我通过log4j打印的日志,也清晰的看到他返回的格式并非json格式.
既然格式错误了,浏览器当然接受不了数据,原因就在此了
2.知道了原因,于是就上网搜了.网上有人说mvc的xml没有加这个配置
<!-- 开启注解 -->
<mvc:annotation-driven />
但是我项目早就有了,固然不是问题答案。
另外一种是说要配置json模板,觉得这个靠谱,于是乎就学着配置.....终于掉进坑里了
        <!-- 避免IE执行AJAX时,返回JSON出现下载文件 -->
	<bean id="mappingJacksonHttpMessageConverter"
		class="org.springframework.http.converter.json.MappingJacksonHttpMessageConverter">
		<property name="supportedMediaTypes">
			<list>
				<value>text/html;charset=UTF-8</value>
			</list>
		</property>
	</bean>
	<!-- 启动Spring MVC的注解功能，完成请求和注解POJO的映射 -->
	<bean
		class="org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter">
		<property name="messageConverters">
			<list>
				<ref bean="mappingJacksonHttpMessageConverter" /><!-- json转换器 -->
			</list>
		</property>
	</bean>
于是配置了一下,启动项目就报错了,缺包缘故,于是上网找jar包,这又遇到版本问题这个还真不容易,晚上资料却没说版本不同要找不到jar
这里我特意说明下
spring 4.0或一下的,一般都找jackson 1.x的版本
spring 4.1或以上的.就要找jackson2.x以上了
我的项目引用的就是spring 4.1.2的版本,一开始不清楚状况,盲目按网上的人配置,引用jackson版本比较低,到头来问题没解决,还引来一堆别的问题,头痛的要死,死去活来.....
哎......不堪回头...好吧,回归主题
以下是spring 4.1.2版本的正确配置
<-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------->
4.1.2的
<!-- 避免IE执行AJAX时,返回JSON出现下载文件 -->
	<bean id="mappingJacksonHttpMessageConverter"
		class="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter">
		<property name="supportedMediaTypes">
			<list>
				<value>text/html;charset=UTF-8</value>
			</list>
		</property>
	</bean>
	<!-- 启动Spring MVC的注解功能，完成请求和注解POJO的映射 -->
	<bean
		class="org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter">
		<property name="messageConverters">
			<list>
				<ref bean="mappingJacksonHttpMessageConverter" /><!-- json转换器 -->
			</list>
		</property>
	</bean>
引入三个jackson2.4版本
如下:
jackson-core-2.4.1.jar
jackson-annotations-2.4.1.jar
jackson-databind-2.4.1.jar
问题就解决了,不在报406了,json数据出来了
