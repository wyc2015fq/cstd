# OSChina:JSON处理器FastJson - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年09月22日 20:12:43[boonya](https://me.csdn.net/boonya)阅读数：1340标签：[java																[json																[string																[class																[api](https://so.csdn.net/so/search/s.do?q=api&t=blog)
个人分类：[Java](https://blog.csdn.net/boonya/article/category/878447)







From：[http://www.oschina.net/p/fastjson](http://www.oschina.net/p/fastjson)


fastjson 是一个性能很好的 Java 语言实现的 JSON 解析器和生成器，来自阿里巴巴的工程师开发。


主要特点：
- 

快速FAST (比其它任何基于Java的解析器和生成器更快，包括[jackson](http://www.oschina.net/p/jackson)）

- 

强大（支持普通JDK类包括任意Java Bean Class、Collection、Map、Date或enum）

- 

零依赖（没有依赖其它任何类库除了JDK）



示例代码：



[?](http://www.oschina.net/p/fastjson#)

```
```java
import
```

```java
com.alibaba.fastjson.JSON;
```





```java
Group group =
```

```java
new
```

```java
Group();
```



```java
group.setId(0L);
```



```java
group.setName(
```

```java
"admin"
```

```java
);
```





```java
User guestUser =
```

```java
new
```

```java
User();
```



```java
guestUser.setId(2L);
```



```java
guestUser.setName(
```

```java
"guest"
```

```java
);
```





```java
User rootUser =
```

```java
new
```

```java
User();
```



```java
rootUser.setId(3L);
```



```java
rootUser.setName(
```

```java
"root"
```

```java
);
```





```java
group.getUsers().add(guestUser);
```



```java
group.getUsers().add(rootUser);
```



```java
String jsonString = JSON.toJSONString(group);
```



```java
System.out.println(jsonString);
```
```









![](http://git.oschina.net/assets/logo-white.gif)

[wenshao/fastjson](http://git.oschina.net/wenshao/fastjson)
[star 39](http://git.oschina.net/wenshao/fastjson/stargazers)| [fork 24](http://git.oschina.net/wenshao/fastjson/members)

- 


[最近提交:](http://git.oschina.net/wenshao/fastjson/commits/wenshao/fastjson)- 

[b3407ba1e](http://git.oschina.net/wenshao/fastjson/commit/b3407ba1e741d8d3ba1d353269eb9a853217cf2f)[merge](http://git.oschina.net/wenshao/fastjson/commit/b3407ba1e741d8d3ba1d353269eb9a853217cf2f)
[wenshao](http://git.oschina.net/wenshao) 9个月前



- 

[e252a84d8](http://git.oschina.net/wenshao/fastjson/commit/e252a84d85c1d429713381aff682ddba7da43b13)[fastjson-1.1.38](http://git.oschina.net/wenshao/fastjson/commit/e252a84d85c1d429713381aff682ddba7da43b13)
[wenshao](http://git.oschina.net/wenshao) 9个月前



- 

[4be3edb8a](http://git.oschina.net/wenshao/fastjson/commit/4be3edb8a29d9eed5db65828ac6f2fb7ba7aa440)[bug
 fixed for unicode space](http://git.oschina.net/wenshao/fastjson/commit/4be3edb8a29d9eed5db65828ac6f2fb7ba7aa440)
[wenshao](http://git.oschina.net/wenshao) 9个月前







[下载zip](http://git.oschina.net/wenshao/fastjson/repository/archive?ref=master)[master分支](http://git.oschina.net/wenshao/fastjson) 代码最近更新：2014-01-04](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=json&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




