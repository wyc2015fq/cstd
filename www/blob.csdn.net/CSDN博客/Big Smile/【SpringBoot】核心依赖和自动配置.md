# 【SpringBoot】核心依赖和自动配置 - Big Smile - CSDN博客
2018年02月02日 00:33:01[王啸tr1912](https://me.csdn.net/tr1912)阅读数：14880
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
之前介绍了springboot是有多么的好，那么，我们现在通过一个小demo来看他是有多么的强大！
# 一、核心pom引入
        我们可以知道一般的项目引入了的基本包和spring一些连接池，再加上几个配置文件还有应用服务器（或者web服务器），就可以直接运行起来了。那么springboot要如何做呢？
首先就是核心依赖了：
```
<parent>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter-parent</artifactId>
	<version>1.5.2.RELEASE</version>
</parent>
```
这个parent是springboot最最核心的引入，所有的配置都在里面，并且是springboot项目必须引入的。
当然现在都是web项目居多，我们还需要引入一个需要的依赖：
```
<dependency>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-starter-web</artifactId>
</dependency>
```
然后可以的话还可以引入一个插件：
```
<plugin>
	<groupId>org.springframework.boot</groupId>
	<artifactId>spring-boot-maven-plugin</artifactId>
</plugin>
```
引入这些东西之后，我们可以在一个包中新建一个类，然后写入以下代码：
```java
@Controller
@SpringBootApplication
@Configuration
public class HelloApplication {
    
    @RequestMapping("hello")
    @ResponseBody
    public String hello(){
        return "hello world！";
    }
    
    public static void main(String[] args) {
        SpringApplication.run(HelloApplication.class, args);
    }
}
```
之后直接用javaApplication 的run方式，我们就可以直接将springboot的这个项目run起来，可以在控制台看到：
![](https://img-blog.csdn.net/20180202001225981?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后我们在浏览器中输入：http://localhost:8080/hello
就可以看到如下页面：
![](https://img-blog.csdn.net/20180202001349872?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这样，我们就启动了一个简单的springboot项目。
# 二、核心自动配置
        我们可以在项目中看到，其实我们就是写了一个类，为什么连tomcat都不用，就可以直接启动一个web程序呢？这一切都是从springboot的配置说起。
        还记得我们引入pom了一个spring-boot-starter-web了么，这个就是spring所依赖的web项目模板，我们可以在maven管理器的依赖中找到这些已经默认加载好的东西：
![](https://img-blog.csdn.net/20180202001841708?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        我们可以直观的看到，tomcat，hibernate，还有一些web项目需要用的东西全部都引入到了项目中，在项目启动的时候就是调用的他们，然后我们就相当于吧tomcat也启动了一样的感觉，为我们的开发配置省了不少事情。
然后我们再来看我们写的这个类，为什么可以直接用web来启动呢，根据上一篇文章的介绍，java的配置方式起到了很强的作用，那就是我们可以利用注解去轻易的配置一些我们想要配置的东西，springboot就是利用了这一点。我们可以看到我们的类上面除了java的@Configuration之外，还有一个是springboot的配置项@SpringBootApplication，这个注解的引入，就默认为我们自动的配置了一些常用的配置。
![](https://img-blog.csdn.net/20180202001841708?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
那么，它都自动为我们配置了什么呢？我们可以找到library的jar包引入，找到一个springboot的autoconfigure来看一下：
![](https://img-blog.csdn.net/20180202002610033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我们展开看一下会发现很多熟悉的东西：
![](https://img-blog.csdn.net/20180202002736375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20180202002739643?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        是不是看到了很多熟悉的身影呢，我们可以在每个包下面都找到一个带有AutoConfigure的类，这就是用java配置方式写的默认配置：
![](https://img-blog.csdn.net/20180202002912327?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
当然，这些都是默认的配置，我们如果想要自己配置一个组件要怎么把默认的配置取消呢？其实很简单，因为每个springboot项目都必须有一个核心的带有@SpringBootApplication注解的类作为入口，所以我们只需要在这个注解上消除这个配置类的引入就可以了，例如：
```java
@Controller
@SpringBootApplication(exclude = {MongoAutoConfiguration.class, KafkaAutoConfiguration.class})
@Configuration
public class HelloApplication {
    
    @RequestMapping("hello")
    @ResponseBody
    public String hello(){
        return "hello world！";
    }
    
    public static void main(String[] args) {
        SpringApplication.run(HelloApplication.class, args);
    }
}
```
通过这样用exclude的方式就把mongo和kafka的自动配置去掉了，我们就可以自己进行配置了。
