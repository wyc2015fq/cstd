# intellij 开发webservice - z69183787的专栏 - CSDN博客
2016年12月06日 11:11:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：745
   最近项目中有用到WebService，于是就研究了一下，但是关于intellij 开发 WebService 的文章极少，要不就是多年以前，于是研究一下，写这篇博文。纯属记录，分享，中间有不对的地方，望请指正，下面开始。
      首先，开发WebService的服务器端，不需要借助任何的其他，JDK就可以搞定，只要在类上标注了@WebService，以及在方法上，标注了@WebMethod方法，就可以认为他是一个WebService。
      下面，先显示一下我的目录结构：
![](http://images.cnitblog.com/blog/612581/201412/161335459698813.png)
       在server包下的是服务器端代码，在client包下的是客户端代码。
       下面看一下，服务端代码：
       HelloWorldWS.java
```java
package server;
 
 /**
  * Created by Lin_Yang on 2014/12/16.
  */
 public interface HelloWorldWS {
     public String sayHello(String name);
 }
```
 这是一个接口。(当然也可以没有这个接口，效果是一样的)
      HelloWorldImpl.java
```java
package server;
 
import javax.jws.WebMethod;
import javax.jws.WebService;
 
/**
 * Created by Lin_Yang on 2014/12/16.
 */
@WebService
public class HelloWorldImpl implements  HelloWorldWS {
    @WebMethod
    @Override
    public String sayHello(String name) {
        String str="欢迎你："+name;
        System.out.println(str);
        return str;
    }
}
```
　注意上面的两个注释@WebService 和  @WebMethod
   下面就可以发布这个WebService了
   Publish.java
```java
package server;
 
import javax.xml.ws.Endpoint;
 
/**
 * Created by Lin_Yang on 2014/12/16.
 */
public class Publish {
    public static void main(String args[]){
        Object implementor = new HelloWorldImpl();
        String address = "http://localhost:8989/HelloWorld";     //发布到的地址
        Endpoint.publish(address, implementor);
        System.out.println("发布成功");
    }
}
```
 客户端的代码很简单，这里就不连篇累牍了。
  下面着重说一下客户端代码的创建过程。
  intellij14 中内置了WebService 的客户端代码的实现方式，他是使用的 JAX-WS.废话不多说，上图。
![](http://images.cnitblog.com/blog/612581/201412/161345487973348.png)
在Intellj 的 Tool-->WebServices-->Generate Java Code From WSDL  （一看就是根据WSDL文档生成java代码了）
![](http://images.cnitblog.com/blog/612581/201412/161348218901718.png)
随后应该弹出这个一个提示框。
首先，Web service wsdl url 是指明WSDL文档的位置，这里的地址和服务端发布的地址相对应。他也可以不从网络中寻找这个WSDL文档，也可以从本地寻找。
格式是这样的：file:/c:/CRMLOYMemberCreateWorkflow.wsdl   指定文档的地址。
按照上图的配置，就会在client包中生成这些代码
![](http://images.cnitblog.com/blog/612581/201412/161354244535617.png)
下面我们就可以根据这些生成的代码，访问服务端的WebService了
test/client.java
```java
package client.test;
import client.HelloWorldImpl;
import client.HelloWorldImplService;
/**
 * Created by Lin_Yang on 2014/12/16.
 */
public class Client {
    public static void main(String args[]){
        HelloWorldImplService helloWorldImplService=new HelloWorldImplService();
        HelloWorldImpl helloWorld= helloWorldImplService.getHelloWorldImplPort();
        String returnStr=  helloWorld.sayHello("先知后觉");
        System.out.println(returnStr);
    }
}
```
服务端显示
![](http://images.cnitblog.com/blog/612581/201412/161408523125319.png)
 客户端显示：
![](http://images.cnitblog.com/blog/612581/201412/161409099842617.png)
    希望可以给大家一些启示。
[下载demo](http://download.csdn.net/detail/u013067125/8265847)
