# 使用Struts 2将客户端JSON数据映射为服务器端Java对象 - 沧海一粟 —— 技术随手记 - CSDN博客





2013年04月17日 13:23:33[慢游](https://me.csdn.net/eengel)阅读数：6239








上文(**[使用 Struts 2将Java对象序列化成JSON](http://blog.csdn.net/eengel/article/details/8809618)**)介绍了如何将Java对象序列化成JSON格式并传到客户端。这篇文章就说说如何将客户端的JSON数据映射为服务器端的Java对象。



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

package extends要增加”json-default”；负责接收数据的SaveAction必须设置interceptor-ref为json。否则将无法得到客户端传来的数据。

```
<package name="sandbox" namespace="/" extends=" struts-default, json-default">
		<action name="main" class="sandbox.z.MainAction">
			<result>/jsp/main.jsp</result>
		</action>
<action name="test" class="sandbox.z.TestAction">
			<result type="json" />
		</action>		
		<action name="save" class="sandbox.z.SaveAction">
			<interceptor-ref name="json">
				<param name="contentType">application/json</param>
			</interceptor-ref>
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
				var data = {
						name: "john",
						age: 30,
						height: 1.78,
						scores: [85, 90],
						score_array: [85, 90],
						array: [85,"Struts2"],
						books_map: {
							"struts2": 85
						},
						book: {
							"name": "Struts2",
							"price": 85.3,
							"sellers":[
								"SA",
								"SB"
							]
						}
				};
				
				var xhrArgs = {
					url : "/save.action",
					handleAs : "json",
					contentType : 'application/json; charset=utf-8',
					postData: dojo.toJson(data), /*data必须放在postData中，并转成json格式*/
					sync : true
				};
				var deferred = xhr.post(xhrArgs);

				/*服务器收到json数据，并转成相应的JAVA对象，最后依旧返回给客户端*/
				deferred.then(function(result) {
					var output = JSON.stringify(result);
					console.log(output);
				});
			});
		});
	</script>
```


SaveAction.java

SaveAction中，和客户端data中对应的属性会得到相应的数据。

```java
package sandbox.z;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.opensymphony.xwork2.ActionSupport;

public class SaveAction extends ActionSupport {

	private static final long serialVersionUID = 1L;

	private String name;
	private int age;
	private double height;
	private int[] scores;
	
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

可以用FirebugConsole检验服务器端是否正确接收到数据了。
`{"age":30,"array":[85,"Struts2"],"book":{"name":"Struts2","price":85.3,"sellers":["SA","SB"]},"books_map":{"struts2":85},"height":1.78,"name":"john","score_array":[85,90],"scores":[85,90]}`

总结

正常情况下，从JSON到JAVA的转换也比较简单。关键是属性的名字和相应的数据类型符合转换的要求。

那当属性名字或数据类型不符合的时候，会产生什么样的问题呢？



1.      属性的数据类型不对

当JSON给相应的属性赋值时，若没有严格按照定义的数据类型赋值，则对于某些不匹配Struts2会进行自动转换。比如给String类型的赋了数值型的值，则该数值到服务器端后，会被转换成String类型。但如果对String类型的赋列表型的值，则会报错。



**这儿还有一种取巧的办法，比如将JAVA的某个属性类型设成Object，则无论JSON中给这个属性赋什么值，服务器端都不会报错。不过这样一来，哪怕服务器端拿到这个属性值了，也很难将其映射到某个具体的数据类型上，并进行进一步的操作。因此最好还是定义比较严格的数据类型，并要求客户端JSON赋值时遵守数据类型的约定。**





2.      属性少了

属性少了不会报错，而是相应的JAVA对象的属性值为null。



3.      属性多了

若在JSON中多了Action中未定义的属性，Action会直接忽视而不报错。







