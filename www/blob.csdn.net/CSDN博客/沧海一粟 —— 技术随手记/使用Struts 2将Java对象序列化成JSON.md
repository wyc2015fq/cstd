# 使用Struts 2将Java对象序列化成JSON - 沧海一粟 —— 技术随手记 - CSDN博客





2013年04月16日 16:20:18[慢游](https://me.csdn.net/eengel)阅读数：3974
个人分类：[Dojo																[Struts](https://blog.csdn.net/eengel/article/category/1390739)](https://blog.csdn.net/eengel/article/category/773224)








Struts 2有个很强大的功能，就是可以自动完成服务器端Java对象和客户端JSON对象之间的映射。这篇文章就介绍一下如何将Java对象序列化成JSON格式并传到客户端。可以看看不同的Java类型和JSON类型之间是如何映射的。



pom.xml

需要引入struts2-json-plugin包。



```
<dependencies>
		<dependency>
			<groupId>org.apache.struts</groupId>
			<artifactId>struts2-core</artifactId>
			<version>2.3.12</version>
		</dependency>
		<dependency>
			<groupId>org.apache.struts</groupId>
			<artifactId>struts2-json-plugin</artifactId>
			<version>2.3.12</version>
		</dependency>
	</dependencies>
```




struts.xml

package extends要增加”json-default”；负责映射的TestAction设置result type为json。

```
<package name="sandbox" namespace="/" extends=" struts-default, json-default">
		<action name="main" class="sandbox.z.MainAction">
			<result>/jsp/main.jsp</result>
		</action>
<action name="test" class="sandbox.z.TestAction">
			<result type="json" />
		</action>		
	</package>
```


main.jsp

```java
<script>
		require([ "dojo/ready", 
		          "dojo/_base/xhr", 
		          "dojo/json",
		          "dijit/registry" ], function(ready, xhr, JSON, registry) {		
			ready(function() {
				var xhrArgs = {
					url : "/test.action",
					handleAs : "json", /*设置返回数据的类型，和服务器端的设置一致*/
					sync : true
				};
				var deferred = xhr.get(xhrArgs);

				deferred.then(function(result) {
					var output = JSON.stringify(result); /*将返回值变成json string*/
					console.log(output); /*打印到Firebug console*/
				});
			});
		});
	</script>
```


TestAction.java

TestAction中，凡是有setter方法的属性都会被转换成JSON并输出到客户端。

```java
package sandbox.z;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.opensymphony.xwork2.ActionSupport;

public class TestAction extends ActionSupport {

	private static final long serialVersionUID = 1L;

	private String name = "john";
	private int age = 30;
	private double height = 1.78;
	private int[] scores = {85, 90};
	
	private List<Integer> score_array = new ArrayList<Integer>();

	public void setScore_array(List<Integer> score_array) {
		this.score_array = score_array;
	}

	public List<Integer> getScore_array() {
		return score_array;
	}
	
	private List array = new ArrayList();
	
	public List getArray() {
		return array;
	}

	public void setArray(List array) {
		this.array = array;
	}
	
	private Map books_map = new HashMap();
	
	public void setBooks_map(Map books_map) {
		this.books_map = books_map;
	}

	public Map getBooks_map() {
		return books_map;
	}
	
	private Book book = new Book();
	
	public void setBook(Book book) {
		this.book = book;
	}
	
	public Book getBook() {
		return book;
	}

	public String execute() {
		score_array.add(85);
		score_array.add(90);
		
		array.add(85);
		array.add("Struts2");
		
		books_map.put("struts2", 85);
		
		getBook().setName("Struts2");
		getBook().setPrice(85.30);
		
		List<String> sellers = new ArrayList<String>();
		sellers.add("SA");
		sellers.add("SB");
		getBook().setSellers(sellers);
		
		return SUCCESS;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getName() {
		return name;
	}

	public void setAge(int age) {
		this.age = age;
	}

	public int getAge() {
		return age;
	}

	public void setHeight(double height) {
		this.height = height;
	}

	public double getHeight() {
		return height;
	}

	public void setscores(int[] scores) {
		this.scores = scores;
	}

	public int[] getscores() {
		return scores;
	}

}
```


Firebug console
`{"age":30,"array":[85,"Struts2"],"book":{"name":"Struts2","price":85.3,"sellers":["SA","SB"]},"books_map":{"struts2":85},"height":1.78,"name":"john","score_array":[85,90],"scores":[85,90]`

总结

可以看到，从JAVA对象转换到JSON格式还是非常直白的。由于JAVA对象比较复杂，表达能力强，而JSON格式比较简单，表达能力弱，因此这种转换大致不太会出错。而相反的，如果要将JSON转换成JAVA，则需要考虑是否需要额外的信息帮助将简单的格式转换成复杂对象。可以想象这中间会出现歧义。






参考文献

[1] JSON Plugin. [http://struts.apache.org/release/2.2.x/docs/json-plugin.html](http://struts.apache.org/release/2.2.x/docs/json-plugin.html)





