# 用Jersey、Grizzly在eclipse中开发第一个RESTful服务helloworld - 零度的博客专栏 - CSDN博客
2017年03月01日 11:21:36[零度anngle](https://me.csdn.net/zmx729618)阅读数：786
这几天读《Rest in Practice》，第5章的例子用了Jersey+Grizzly构建示例。随书代码中给的例子中没有指明Jersey和Grizzly使用的版本，所以下载了最新的版本，用了后才发现问题多多。不知道是不是sun公司变更的原因，许多包名、参数明都发生了变化，而网上给出的大多数例子还是老的，例如，老版本中用参数“com.sun.jersey.config.server.provider.packages”指定资源类的位置，而新版本中改用了“jersey.config.server.provider.packages”。所以许多旧的例子直接拿来就会出错。因此，本文给出了一个比较新的版本Jersey-2.11，Grizzly-2.3.14对应的例子。
        1.  在eclipse中新建一个Java Project，并建一个文件夹lib存放需要的jar包，使用的跟Jersey和Grizzly相关的包如下（可以在mvnrepository.com下载到）：
![](https://img-blog.csdn.net/20140806091655731?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbW9vbnNoZWVwX2xpdQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        2. 建立资源类HelloWorld.java（注意package名字）。
**[java]**[view plain](http://blog.csdn.net/moonsheep_liu/article/details/38395497#)[copy](http://blog.csdn.net/moonsheep_liu/article/details/38395497#)[print](http://blog.csdn.net/moonsheep_liu/article/details/38395497#)[?](http://blog.csdn.net/moonsheep_liu/article/details/38395497#)
- package com.helloworldGrizzlyJersey.resources;  
- 
- import javax.ws.rs.GET;  
- import javax.ws.rs.Path;  
- import javax.ws.rs.Produces;  
- 
- @Path("/")  
- publicclass HelloWorld {  
- 
- public HelloWorld(){}  
- 
- @GET
- @Produces("text/plain")  
- public String getMessage() {  
- return"Hello World!";  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.helloworldGrizzlyJersey.resources;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
@Path("/")
public class HelloWorld {
    
    public HelloWorld(){}
    
    @GET
    @Produces("text/plain")
    public String getMessage() {
        return "Hello World!";
    }
}
```
        3. 构建Servlet服务器。
**[java]**[view plain](http://blog.csdn.net/moonsheep_liu/article/details/38395497#)[copy](http://blog.csdn.net/moonsheep_liu/article/details/38395497#)[print](http://blog.csdn.net/moonsheep_liu/article/details/38395497#)[?](http://blog.csdn.net/moonsheep_liu/article/details/38395497#)
- package com.helloworldGrizzlyJersey;  
- 
- import java.io.IOException;  
- import java.util.HashMap;  
- 
- import org.glassfish.grizzly.http.server.HttpServer;  
- import org.glassfish.jersey.grizzly2.servlet.GrizzlyWebContainerFactory;  
- 
- publicclass Main {  
- publicstaticvoid main(String[] args) throws IOException {  
- 
- final String baseUri = "http://localhost:9999/";  
- final HashMap<String, String> initParams = new HashMap<String, String>();  
- 
-         initParams.put("jersey.config.server.provider.packages", "com.helloworldGrizzlyJersey.resources");//packages定义了Jersey寻找服务类的位置。 它必须指向定义的资源类。
- 
-         System.out.println("Starting grizzly...");  
- //      SelectorThread threadSelector = GrizzlyWebContainerFactory.create(baseUri, initParams);
-         HttpServer server  = GrizzlyWebContainerFactory.create(baseUri, initParams);   //Rest请求的Servlet分发器
- //      HttpServer server = GrizzlyHttpServerFactory.createHttpServer(baseUri, initParams);
-         System.out.println("...started");  
-         System.in.read();  
-         server.shutdown();  
-         System.exit(0);  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.helloworldGrizzlyJersey;
import java.io.IOException;
import java.util.HashMap;
import org.glassfish.grizzly.http.server.HttpServer;
import org.glassfish.jersey.grizzly2.servlet.GrizzlyWebContainerFactory;
public class Main {
	public static void main(String[] args) throws IOException {
		
		final String baseUri = "http://localhost:9999/";
		final HashMap<String, String> initParams = new HashMap<String, String>();
	    initParams.put("jersey.config.server.provider.packages", "com.helloworldGrizzlyJersey.resources");//packages定义了Jersey寻找服务类的位置。 它必须指向定义的资源类。
	    System.out.println("Starting grizzly...");
//	    SelectorThread threadSelector = GrizzlyWebContainerFactory.create(baseUri, initParams);
	    HttpServer server  = GrizzlyWebContainerFactory.create(baseUri, initParams);   //Rest请求的Servlet分发器
//	    HttpServer server = GrizzlyHttpServerFactory.createHttpServer(baseUri, initParams);
	    System.out.println("...started");
	    System.in.read();
	    server.shutdown();
	    System.exit(0);
	}
}
```
        4. 最后，运行该程序后，在浏览器键入http://localhost:9999/就能看到Hello World！
