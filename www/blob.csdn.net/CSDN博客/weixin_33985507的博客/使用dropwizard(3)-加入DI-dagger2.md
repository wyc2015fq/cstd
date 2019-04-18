# 使用dropwizard(3)-加入DI-dagger2 - weixin_33985507的博客 - CSDN博客
2017年11月20日 22:39:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
![dropwizard-hat.png](http://www.dropwizard.io/1.1.4/docs/_static/dropwizard-hat.png)
![i18n](http://www.darksword-armory.com/wp-content/uploads/2014/09/gothic-medieval-dagger-1816-1600x1067.jpg)
# 前言
习惯了Spring全家桶，对spring的容器爱不释手。使用dropwizard，看起来确实很轻，然而，真正使用的时候不得不面临一个问题。**我们不可能一个resource就能把所有的业务逻辑囊括**！那么，必然就要有负责处理逻辑的代码，有要提取的公共的代码，要做面向接口开发等等。按照简单的用法，Java Bean就自己new，只要通过Jersey提供的web能力发出去就好。这样写下来，到处都需要new，难以测试等等。目前我最care的是，每个request过来都要new一堆重复的对象，垃圾回收频繁。写个单例不就解决了？是的，当然要想到单例，然后发现几乎所有的类都是设计成单例的。然后，一堆单例的代码写的死。这就是样板代码。于是，想到提取工具类，算了，不如用Dagger好了。
# 什么是Dagger
Dagger是Java里开源的DI框架中最火的之一，主要用在Android领域，很多特性也多针对Android开发的。因为Android开发对省电，性能之类的要求比较高，因此抛弃了反射，直接在编译级别生成工厂。详细学习测试：[Dagger2之helloworld原理探究](http://www.cnblogs.com/woshimrf/p/hello-dagger.html)
# Demo Source
https://github.com/Ryan-Miao/l4dropwizard
structure
```java
```
.
├── pom.xml
├── readme.md
└── src
    └── main
        ├── java
        │   └── com
        │       └── test
        │           ├── HelloWorldApplication.java
        │           ├── bundles
        │           │   └── ConnectivityBundle.java
        │           ├── configuration
        │           │   ├── HelloWorldConfiguration.java
        │           │   └── modules
        │           │       ├── ConnectAndReadConfig.java
        │           │       └── GithubApiConfig.java
        │           └── domain
        │               ├── connect
        │               │   ├── FeignClientBuilder.java
        │               │   ├── GithubClient.java
        │               │   └── GithubConnector.java
        │               ├── entiry
        │               │   ├── GithubUser.java
        │               │   └── Saying.java
        │               ├── exception
        │               │   └── UpstreamException.java
        │               ├── health
        │               │   └── TemplateHealthCheck.java
        │               ├── ioc
        │               │   ├── component
        │               │   │   └── GithubComponent.java
        │               │   └── module
        │               │       ├── ConfigurationModule.java
        │               │       ├── ConnectorModule.java
        │               │       └── ServiceModule.java
        │               ├── resource
        │               │   ├── GithubResource.java
        │               │   └── HelloWorldResource.java
        │               └── service
        │                   ├── IGithubService.java
        │                   └── impl
        │                       └── GithubService.java
        └── resources
            └── config
                └── dev.yml
```
```
本文基于之前的[dropwizard入门](http://www.cnblogs.com/woshimrf/tag/dropwizard/)演进.
## 添加dagger依赖
在`properties`结点下新增
`<dagger.verion>2.12</dagger.verion>`
在dependencies下新增
```
<dependency>
    <groupId>com.google.dagger</groupId>
    <artifactId>dagger</artifactId>
    <version>${dagger.verion}</version>
</dependency>
```
在`build.plugins`下新增`plugin`
```
<plugin>
    <groupId>org.apache.maven.plugins</groupId>
    <artifactId>maven-compiler-plugin</artifactId>
    <version>3.6.1</version>
    <configuration>
        <source>${java.version}</source>
        <target>${java.version}</target>
        <encoding>UTF-8</encoding>
        <annotationProcessorPaths>
            <path>
                <groupId>com.google.dagger</groupId>
                <artifactId>dagger-compiler</artifactId>
                <version>${dagger.verion}</version>
            </path>
        </annotationProcessorPaths>
    </configuration>
</plugin>
```
在IDEA设置中，找到`Build>Compiler>Annotation Processors`, 选择`Enable annotation processing`.
## 创建一个Component
下面创建Component，用来包容Resource类，对外提供Resource类。新建`com.test.domain.ioc.component.GithubComponent`
```java
```
package com.test.domain.ioc.component;
import com.test.domain.ioc.module.ServiceModule;
import com.test.domain.resource.GithubResource;
import dagger.Component;
import javax.inject.Singleton;
/**
 * Created by Ryan Miao on 10/26/17.
 */
@Singleton
@Component(modules = {ServiceModule.class})
public interface GithubComponent {
     GithubResource gitHubResource();
}
```
```
显然，这个`GithubResource`需要注入一个service，于是声明一个`ServiceModule`, 创建`com.test.domain.ioc.module.ServiceModule`
```java
```
package com.test.domain.ioc.module;
import com.test.domain.service.IGithubService;
import com.test.domain.service.impl.GithubService;
import dagger.Module;
import dagger.Provides;
import javax.inject.Singleton;
/**
 * Created by Ryan Miao on 10/26/17.
 */
@Module(includes = ConnectorModule.class)
public class ServiceModule {
    @Singleton
    @Provides
    public IGithubService githubService(GithubService service) {
        return service;
    }
}
```
```
`ServiceModule`用来提供service注入，service接着依赖connector层，新建`com.test.domain.ioc.module.ConnectorModule`
```java
```
package com.test.domain.ioc.module;
import com.test.domain.connect.FeignClientBuilder;
import dagger.Module;
import dagger.Provides;
import javax.inject.Singleton;
/**
 * Created by Ryan Miao on 10/26/17.
 */
@Module(includes = ConfigurationModule.class)
public class ConnectorModule {
    @Provides
    @Singleton
    public FeignClientBuilder feignClientBuilder(){
        return new FeignClientBuilder();
    }
    
}
```
```
在`connecttor`层中，需要调用GlobalConfiguration的配置项，所以，单独把配置提出来，引入。新增`com.test.domain.ioc.module.ConfigurationModule`
```java
```
package com.test.domain.ioc.module;
import com.test.configuration.HelloWorldConfiguration;
import dagger.Module;
import dagger.Provides;
import javax.inject.Singleton;
/**
 * Created by Ryan Miao on 11/20/17.
 */
@Module
public class ConfigurationModule {
    private final HelloWorldConfiguration configuration;
    public ConfigurationModule(HelloWorldConfiguration configuration) {
        this.configuration = configuration;
    }
    @Provides
    @Singleton
    public HelloWorldConfiguration helloWorldConfiguration(){
        return configuration;
    }
}
```
```
这是依赖的最底层，我们通过手动构造函数的方式注入configuration，这样可以在dropwizard启动时生成module，并且得到configuration。
## 引入我们的Component
这时候，build一下，dagger就会自动生成我们的工厂。
`mvn clean install`
然后，在IDEA里的maven plugin里，右键，reimport。防止IDEA不认识dagger自动生成的类。dagger自动生成的类位于`target/generated-sources/annotations`. 点击刷新按钮，刷新下maven依赖。
然后，在`com.test.HelloWorldApplication`中,新增
```java
```
private void registerResources(HelloWorldConfiguration configuration, Environment environment) {
    GithubComponent component = DaggerGithubComponent.builder()
            .configurationModule(new ConfigurationModule(configuration))
            .build();
    environment.jersey().register(component.gitHubResource());
}
```
```
`DaggerGithubComponent`要在maven install之后，dagger生成的。完整启动类如下：
```java
```
public class HelloWorldApplication extends Application<HelloWorldConfiguration> {
    public static void main(String[] args) throws Exception {
        new HelloWorldApplication().run(args);
    }
    @Override
    public String getName() {
        return "hello-world";
    }
    @Override
    public void initialize(Bootstrap<HelloWorldConfiguration> bootstrap) {
        bootstrap.addBundle(new ConnectivityBundle());
    }
    @Override
    public void run(HelloWorldConfiguration configuration, Environment environment) throws Exception {
        final HelloWorldResource resource = new HelloWorldResource(
                configuration.getTemplate(),
                configuration.getDefaultName()
        );
        final TemplateHealthCheck healthCheck =
                new TemplateHealthCheck(configuration.getTemplate());
        environment.healthChecks().register("template", healthCheck);
        environment.jersey().register(resource);
        environment.jersey().register(healthCheck);
        registerResources(configuration, environment);
    }
    private void registerResources(HelloWorldConfiguration configuration, Environment environment) {
        GithubComponent component = DaggerGithubComponent.builder()
                .configurationModule(new ConfigurationModule(configuration))
                .build();
        environment.jersey().register(component.gitHubResource());
    }
}
```
```
当然，我们的Resource也要改成Inject模式
```java
```
public class GithubResource {
    private IGithubService service;
    @Inject
    public GithubResource(IGithubService service) {
        this.service = service;
    }
    @GET
    @Timed
    @Path("/users/{username}")
    public GithubUser getUserProfile(@PathParam("username") final String username) {
        return service.getUserProfile(username);
    }
}
```
```
启动，运行。一切OK。以后就可以在需要注入的类的构造器上声明`@Inject`, 或者在module里`@Provide`，就可以实现构造解耦。测试不要太方便.
