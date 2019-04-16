# Java学习笔记37：Spring 使用 @ResponseBody 返回中文乱码问题解决 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年04月17日 13:46:10[initphp](https://me.csdn.net/initphp)阅读数：2858
个人分类：[Java学习笔记系列](https://blog.csdn.net/initphp/article/category/1230072)

所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)









今天在使用spring 的时候，发现中文返回的是乱码。

经过研究发现，主要是@ResponseBody 引起的。主要是由于 @ResponseBody 返回字符串结果的时候，使用了org.springframework.http.converter.StringHttpMessageConverter 中默认的编码。

请看 org.springframework.http.converter.StringHttpMessageConverter ：



```java
public class StringHttpMessageConverter extends AbstractHttpMessageConverter<String> {

	public static final Charset DEFAULT_CHARSET = Charset.forName("ISO-8859-1");

	private final List<Charset> availableCharsets;

	private boolean writeAcceptCharset = true;

	public StringHttpMessageConverter() {
		super(new MediaType("text", "plain", DEFAULT_CHARSET), MediaType.ALL);
		this.availableCharsets = new ArrayList<Charset>(Charset.availableCharsets().values());
	}
```


默认情况下，是ISO-8859-1编码，我们要将ISO-8859-1编码改成utf-8编码。



```java
public static final Charset DEFAULT_CHARSET = Charset.forName("ISO-8859-1");
```


我们的解决方法是，在spring-servlet.xml中，加入一个bean



```
<bean
		class="org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter">
		<property name="messageConverters">
			<list>
				<bean
					class="org.springframework.http.converter.StringHttpMessageConverter">
					<property name="supportedMediaTypes">
						<list>
							<bean class="org.springframework.http.MediaType">
								<constructor-arg index="0" value="text" />
								<constructor-arg index="1" value="plain" />
								<constructor-arg index="2" value="UTF-8" />
							</bean>
						</list>
					</property>
				</bean>
			</list>
		</property>
	</bean>
```







