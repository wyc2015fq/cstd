# spring bottom - 左直拳的马桶_日用桶 - CSDN博客
2019年01月28日 13:50:01[左直拳](https://me.csdn.net/leftfist)阅读数：201
个人分类：[Java](https://blog.csdn.net/leftfist/article/category/2412493)
从.net转到java后，发现了一些.net做得比java好的地方。
**1、属性**
在c#里，属性的语法简直不要太方便，如
```
//声明
public class Persion
{
	public string Name { get; set;} = "张三";
}
//使用
(new Persion()).Name = "李四";
```
相比之下，java的语法还在旧石器时代：
```java
private String name;
public String getName(){
	return this.name;
}
public String setName(String name){
	this.name = name;
}
```
当然啦，IDE可以自动生成下面的get和set，或者用lombok等插件，加注解
```java
private @Getter @Setter String name;
```
但使用起来始终没有c#直观，get呀set呀，傻得厉害。
**2、静态资源**
像JS、CSS这些，改了以后，还要重启WEB服务才能生效，这也太扯了吧。开发效率低得令人发指。怪不得你要前后分离呢。
以前用asp.net的时候，修改这些静态文件，只刷新浏览器就行了，所见即所得，便捷得很。
但java就不是这样。每次都要重新编译一遍，哪怕只改了JS文件上的一点点东西。也许这跟JAVA没关系，只跟WEB服务器有关。
而JAVA世界给出的应对办法，是系统自动编译，代替手动编译。终究还是要编译。方法，以idea作为开发工具为例：
1）修改pom.xml
```
<plugin>
				<groupId>org.springframework.boot</groupId>
				<artifactId>spring-boot-maven-plugin</artifactId>
				<configuration>
					<fork>true</fork>
					<addResources>true</addResources><!--支持静态文件热部署-->
				</configuration>
			</plugin>
```
2）修改intellij idea的setting:
builder -> compiler -> build project automatically
看到没，要勾选自动编译。这个选项本来是默认勾选的，因为大家都说为了性能起见，所以去掉了。
3）ctrl + shift + alt + /
选择Registry,勾选compiler.automake.allow.when.app.running
4）thymeleaf的缓存设为false
```
spring.thymeleaf.cache = false
```
修改一点点静态资源，就要大动干戈的重新编译一番，我不知道为啥要这么做。也许，是为了运行时效率高一点？将所有东西都编译进内存，没有磁盘IO了？不可能吧。如果静态资源很多很大呢？
java，大家都把它夸到天上去了，真正用起来，才发现，原先在.net世界里，许多理所当然的事情，在这里已经是一种奢望。java的成功，不在于它比.net有多先进，而是它开放的特性，吸引了大部分人的使用，聚集了多数人的智慧，变成了一种首选。
