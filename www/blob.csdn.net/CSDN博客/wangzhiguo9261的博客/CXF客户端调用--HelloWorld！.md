
# CXF客户端调用--HelloWorld！ - wangzhiguo9261的博客 - CSDN博客


2018年07月23日 10:48:09[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：46


CXF通过运行时代理生成远程服务的代理对象，在客户端完成对webservice的访问
package com.flyfox.client;

import org.apache.cxf.endpoint.Client;
import org.apache.cxf.jaxws.JaxWsProxyFactoryBean;
import org.apache.cxf.jaxws.endpoint.dynamic.JaxWsDynamicClientFactory;
import com.flyfox.service.IHelloWorldService;
public class HelloWorldServiceClient {
public static void main(String[] args) throws Exception {
//调用WebService
JaxWsProxyFactoryBean factory = new JaxWsProxyFactoryBean();
factory.setServiceClass(IHelloWorldService.class);
factory.setAddress("http://localhost:9000/helloWorld");
IHelloWorldService service = (IHelloWorldService) factory.create();
System.out.println("[result]" + service.sayHello("flyfox"));
}
}
小结：这种调用service的好处在于调用过程非常简单，就几行代码就完成一个webservice的调用，但是客户端也必须依赖服务器端的接口，这种调用方式限制是很大的，要求服务器端的webservice必须是java实现--这样也就失去了使用webservice的意义
查看了官方的文档后，发现其实是可以实现不依赖服务器端接口来完成调用的，如下
使用JaxWsDynamicClientFactory类，只要指定服务器端wsdl文件的位置，然后指定要调用的方法和方法的参数即可，so simple!!
package com.flyfox.client;
import org.apache.cxf.endpoint.Client;
import org.apache.cxf.jaxws.JaxWsProxyFactoryBean;
import org.apache.cxf.jaxws.endpoint.dynamic.JaxWsDynamicClientFactory;
import com.flyfox.service.IHelloWorldService;
public class HelloWorldServiceClient {
public static void main(String[] args) throws Exception {
//不依赖服务器端接口来完成调用的，也就是不仅仅能调用Java的接口
JaxWsDynamicClientFactory clientFactory = JaxWsDynamicClientFactory.newInstance();
Client client = clientFactory.createClient("http://localhost:9000/helloWorld?wsdl");
Object[] result = client.invoke("sayHello", "KEVIN");
System.out.println(result[0]);
}
}

