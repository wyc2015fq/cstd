# Alibaba dubbo-spring-boot-starter - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年02月19日 16:51:50[boonya](https://me.csdn.net/boonya)阅读数：8792










资源地址：[https://github.com/alibaba/dubbo-spring-boot-starter](https://github.com/alibaba/dubbo-spring-boot-starter)





Dubbo Spring Boot Starter。

支持jdk版本为1.6或者1.6+

（在修改源码前，请导入googlestyle-java.xml以保证一致的代码格式）






dubbo-spring-boot-starter  很简单，也很方便使用，它同时集成了dubbo 的 provider 和 consumer 的功能。
- 
当在 provider 端使用 dubbo-spring-boot-starter  时，可以选择不用 web 容器或者使用 web 容器；它作为 dubbo 的 provider 会提供简单的 RPC 服务。

- 
当在 consumer 端使用 spring-boot-starter-dubbo 时，需要使用 web 容器，它会提供 health等监控。



### [](https://github.com/alibaba/dubbo-spring-boot-starter/blob/master/README_zh.md#%E5%A6%82%E4%BD%95%E5%8F%91%E5%B8%83dubbo%E6%9C%8D%E5%8A%A1)如何发布dubbo服务
- 添加依赖:

    <dependency>
        <groupId>com.alibaba.spring.boot</groupId>
        <artifactId>dubbo-spring-boot-starter</artifactId>
        <version>1.0.0</version>
    </dependency>
- 在application.properties添加dubbo的相关配置信息,样例配置如下:

spring.dubbo.appname=dubbo-spring-boot-starter-provider-test
spring.dubbo.registry=multicast://224.0.0.0:1111
spring.dubbo.protocol=dubbo
- 接下来在Spring Boot Application的上添加`@EnableDubboConfiguration`, 表示要开启dubbo功能. (dubbo provider服务可以使用或者不使用web容器)

@SpringBootApplication@EnableDubboConfigurationpublicclassDubboProviderLauncher {
  //...
}
- 编写你的dubbo服务,只需要添加要发布的服务实现上添加`@Service`（import com.alibaba.dubbo.config.annotation.Service）注解 ,其中interfaceClass是要发布服务的接口.

@Service(interfaceClass=IHelloService.class)
publicclassHelloServiceImplimplementsIHelloService {
  //...
}
- 启动你的Spring Boot应用,观察控制台,可以看到dubbo启动相关信息.

### [](https://github.com/alibaba/dubbo-spring-boot-starter/blob/master/README_zh.md#%E5%A6%82%E4%BD%95%E6%B6%88%E8%B4%B9dubbo%E6%9C%8D%E5%8A%A1)如何消费Dubbo服务
- 添加依赖:

    <dependency>
        <groupId>com.alibaba.spring.boot</groupId>
        <artifactId>dubbo-spring-boot-starter</artifactId>
        <version>1.0.0</version>
    </dependency>
- 在application.properties添加dubbo的相关配置信息,样例配置如下:

spring.dubbo.appname=dubbo-spring-boot-starter-consumer-test
spring.dubbo.registry=multicast://224.0.0.0:1111
spring.dubbo.protocol=dubbo
- 开启`@EnableDubboConfiguration`

@SpringBootApplication@EnableDubboConfigurationpublicclassDubboConsumerLauncher {
  //...
}
- 通过`@Reference`注入需要使用的interface.

@ComponentpublicclassHelloConsumer {
  @ReferenceprivateIHelloService iHelloService;
  
}

### [](https://github.com/alibaba/dubbo-spring-boot-starter/blob/master/README_zh.md#%E5%8F%82%E8%80%83%E6%96%87%E6%A1%A3)参考文档
- dubbo 介绍: [http://dubbo.io/](http://dubbo.io/)
- spring-boot 介绍: [http://projects.spring.io/spring-boot/](http://projects.spring.io/spring-boot/)




