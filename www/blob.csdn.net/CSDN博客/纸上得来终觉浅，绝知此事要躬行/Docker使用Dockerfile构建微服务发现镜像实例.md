# Docker使用Dockerfile构建微服务发现镜像实例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年09月05日 10:41:32[boonya](https://me.csdn.net/boonya)阅读数：2348








我们以微服务发现测试demo为例，打包并构建微服务发现镜像。实现docker镜像构建需要准备docker环境，这里就不在赘述了。通过docker方式可以快速实现微服务的发布和部署，使得开发和部署不再复杂而繁琐。单体应用的整体维护容易引入bug而微服务只需要对存在bug的服务模块进行修复不影响其它模块的运行，虽然很多情况下考虑微服务会带来维护成本的增加，但是在开发运维一体化的实践过程中，“开发即运维”工作高度统一也就无足轻重了，因为我们有更多的时间去关注代码。

## Maven打出可运行的jar包

找到项目主路径使用mvn命令打包：


`mvn install`
打包后可以在target下找到相应的jar包：


![](https://img-blog.csdn.net/20170905102714362)

我们在centos中创建一个microservcice目录并上传jar包：

```
mkdir microservcice
cd microservcice
rz  microservice-discovery-eureka-0.0.1-SNAPSHOT.jar
```

## Dockerfile文件的创建和编辑

在microservcice路径下使用vi编辑Dockerfile并创建文件：


`vi Dockerfile`Dockerfile的内容如下：




```
# 基于哪个镜像
FROM java:8

# 将本地文件夹挂载到当前容器
VOLUME /tmp

# 拷贝文件到容器，也可以直接写成ADD microservice-discovery-eureka-0.0.1-SNAPSHOT.jar /app.jar
ADD microservice-discovery-eureka-0.0.1-SNAPSHOT.jar app.jar
RUN bash -c 'touch /app.jar'

# 开放8761端口
EXPOSE 8761

# 配置容器启动后执行的命令
ENTRYPOINT ["java","-Djava.security.egd=file:/dev/./urandom","-jar","/app.jar"]
```

注：为使镜像文件不至于过大所以基础镜像不采用任何Unix镜像而直接用就JDK镜像。



## Docker构建和运行微服务发现

镜像构建命令：docker build -t boonyadocker/centos-microservice-discovery-eureka .



```
[root@mysqlserver microservice]# docker build -t boonyadocker/centos-microservice-discovery-eureka .
Sending build context to Docker daemon 41.05 MB
Sending build context to Docker daemon 
Step 0 : FROM java:8
8: Pulling from java
a29f2b1e7978: Pull complete 
5b1f17435ed5: Pull complete 
b171076e9dd9: Pull complete 
ea75de9256e4: Pull complete 
3c40a2ed2543: Pull complete 
4ef6b3e5c60b: Pull complete 
b520541a1ecf: Pull complete 
5cf01c8803ea: Pull complete 
c5360aaa8537: Pull complete 
25d2b6364de8: Pull complete 
557ddbce174f: Pull complete 
7558653b0734: Pull complete 
b819dc613ab1: Pull complete 
d11c3799fa6a: Pull complete 
Digest: sha256:34959eed5df6bfe74a17fbf459ed29505d51a48d5cb4a39f215b613c65b8023b
Status: Downloaded newer image for java:8
 ---> d11c3799fa6a
Step 1 : VOLUME /tmp
 ---> Running in 0ec6432d2be9
 ---> a590e65f6941
Removing intermediate container 0ec6432d2be9
Step 2 : ADD microservice-discovery-eureka-0.0.1-SNAPSHOT.jar app.jar
 ---> b6d3c196ec77
Removing intermediate container 4250780a2192
Step 3 : RUN bash -c 'touch /app.jar'
 ---> Running in d238e199fb82
 ---> ee5b06a7ac61
Removing intermediate container d238e199fb82
Step 4 : EXPOSE 8761
 ---> Running in 362b73aecf0b
 ---> 81cedde6a988
Removing intermediate container 362b73aecf0b
Step 5 : ENTRYPOINT java -Djava.security.egd=file:/dev/./urandom -jar /app.jar
 ---> Running in c1e69d53da04
 ---> 069f26cf4360
Removing intermediate container c1e69d53da04
Successfully built 069f26cf4360
[root@mysqlserver microservice]#
```
查看镜像：




```
[root@mysqlserver microservice]# docker images
REPOSITORY                                          TAG                 IMAGE ID            CREATED              VIRTUAL SIZE
boonyadocker/centos-microservice-discovery-eureka   latest              069f26cf4360        About a minute ago   725.2 MB
boonyadocker/centos-mycat                           latest              ec847b95d08e        4 days ago           581 MB
boonyadocker/boot2docker-tomcat                     latest              31ad702ceffa        2 weeks ago          2.602 GB
boonyadocker/boot2docker-jenkins                    latest              a0090abfd7c8        2 weeks ago          2.685 GB
boonyadocker/centos-jenkins                         latest              1250a4fe0292        2 weeks ago          659 MB
boonyadocker/centos-jenkins                         allow-remote        47e38ec3f8bc        3 weeks ago          658.7 MB
boonyadocker/centos-tomcat                          allow-remote        a3cf71b7dd49        3 weeks ago          575.9 MB
tomcat                                              latest              70b2ded797df        3 weeks ago          292.4 MB
boot2docker/boot2docker                             latest              898cd2a4a97e        3 weeks ago          2.219 GB
centos                                              latest              f3b88ddaed16        4 weeks ago          192.5 MB
jenkins                                             latest              de57ef56dca0        5 weeks ago          809.9 MB
openjdk                                             8-jre               cdcc2156bae7        5 weeks ago          275.1 MB
jetbrains/teamcity-server                           latest              47728ea3474b        5 weeks ago          1.308 GB
boonyadocker/jenkins                                latest              e14944ad9023        7 weeks ago          654.2 MB
boonyadocker/tomcat-allow-remote                    latest              96d1fe00ad03        8 weeks ago          571.4 MB
java                                                8                   d11c3799fa6a        7 months ago         643.1 MB
[root@mysqlserver microservice]#
```



## Docker微服务镜像的运行和验证

镜像运行命令：docker run -p 8761:8761 boonyadocker/centos-microservice-discovery-eureka




```
[root@mysqlserver microservice]# docker run -p 8761:8761 boonyadocker/centos-microservice-discovery-eureka
2017-09-05 02:08:43.091  INFO 1 --- [           main] s.c.a.AnnotationConfigApplicationContext : Refreshing org.springframework.context.annotation.AnnotationConfigApplicationContext@3d82c5f3: startup date [Tue Sep 05 02:08:43 UTC 2017]; root of context hierarchy
2017-09-05 02:08:43.436  INFO 1 --- [           main] f.a.AutowiredAnnotationBeanPostProcessor : JSR-330 'javax.inject.Inject' annotation found and supported for autowiring
2017-09-05 02:08:43.466  INFO 1 --- [           main] trationDelegate$BeanPostProcessorChecker : Bean 'configurationPropertiesRebinderAutoConfiguration' of type [class org.springframework.cloud.autoconfigure.ConfigurationPropertiesRebinderAutoConfiguration$$EnhancerBySpringCGLIB$$ef996e77] is not eligible for getting processed by all BeanPostProcessors (for example: not eligible for auto-proxying)

  .   ____          _            __ _ _
 /\\ / ___'_ __ _ _(_)_ __  __ _ \ \ \ \
( ( )\___ | '_ | '_| | '_ \/ _` | \ \ \ \
 \\/  ___)| |_)| | | | | || (_| |  ) ) ) )
  '  |____| .__|_| |_|_| |_\__, | / / / /
 =========|_|==============|___/=/_/_/_/
 :: Spring Boot ::        (v1.4.0.RELEASE)

2017-09-05 02:08:44.048  INFO 1 --- [           main] c.itmuch.cloud.study.EurekaApplication   : No active profile set, falling back to default profiles: default
2017-09-05 02:08:44.072  INFO 1 --- [           main] ationConfigEmbeddedWebApplicationContext : Refreshing org.springframework.boot.context.embedded.AnnotationConfigEmbeddedWebApplicationContext@1d251891: startup date [Tue Sep 05 02:08:44 UTC 2017]; parent: org.springframework.context.annotation.AnnotationConfigApplicationContext@3d82c5f3
2017-09-05 02:08:45.819  WARN 1 --- [           main] o.s.c.a.ConfigurationClassPostProcessor  : Cannot enhance @Configuration bean definition 'refreshScope' since its singleton instance has been created too early. The typical cause is a non-static @Bean method with a BeanDefinitionRegistryPostProcessor return type: Consider declaring such methods as 'static'.
2017-09-05 02:08:46.078  INFO 1 --- [           main] o.s.cloud.context.scope.GenericScope     : BeanFactory id=ab5bfad3-1f36-3435-a655-0ba13f15410d
2017-09-05 02:08:46.095  INFO 1 --- [           main] f.a.AutowiredAnnotationBeanPostProcessor : JSR-330 'javax.inject.Inject' annotation found and supported for autowiring
2017-09-05 02:08:46.293  INFO 1 --- [           main] trationDelegate$BeanPostProcessorChecker : Bean 'org.springframework.cloud.autoconfigure.ConfigurationPropertiesRebinderAutoConfiguration' of type [class org.springframework.cloud.autoconfigure.ConfigurationPropertiesRebinderAutoConfiguration$$EnhancerBySpringCGLIB$$ef996e77] is not eligible for getting processed by all BeanPostProcessors (for example: not eligible for auto-proxying)
2017-09-05 02:08:47.167  INFO 1 --- [           main] s.b.c.e.t.TomcatEmbeddedServletContainer : Tomcat initialized with port(s): 8761 (http)
2017-09-05 02:08:47.184  INFO 1 --- [           main] o.apache.catalina.core.StandardService   : Starting service Tomcat
2017-09-05 02:08:47.186  INFO 1 --- [           main] org.apache.catalina.core.StandardEngine  : Starting Servlet Engine: Apache Tomcat/8.5.4
2017-09-05 02:08:47.294  INFO 1 --- [ost-startStop-1] o.a.c.c.C.[Tomcat].[localhost].[/]       : Initializing Spring embedded WebApplicationContext
2017-09-05 02:08:47.295  INFO 1 --- [ost-startStop-1] o.s.web.context.ContextLoader            : Root WebApplicationContext: initialization completed in 3223 ms
2017-09-05 02:08:48.162  INFO 1 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'metricFilter' to: [/*]
2017-09-05 02:08:48.163  INFO 1 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'characterEncodingFilter' to: [/*]
2017-09-05 02:08:48.163  INFO 1 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'hiddenHttpMethodFilter' to: [/*]
2017-09-05 02:08:48.163  INFO 1 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'httpPutFormContentFilter' to: [/*]
2017-09-05 02:08:48.163  INFO 1 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'requestContextFilter' to: [/*]
2017-09-05 02:08:48.163  INFO 1 --- [ost-startStop-1] o.s.b.c.embedded.FilterRegistrationBean  : Mapping filter: 'webRequestTraceFilter' to: [/*]
2017-09-05 02:08:48.163  INFO 1 --- [ost-startStop-1] o.s.b.c.embedded.FilterRegistrationBean  : Mapping filter: 'servletContainer' to urls: [/eureka/*]
2017-09-05 02:08:48.163  INFO 1 --- [ost-startStop-1] o.s.b.w.servlet.FilterRegistrationBean   : Mapping filter: 'applicationContextIdFilter' to: [/*]
2017-09-05 02:08:48.164  INFO 1 --- [ost-startStop-1] o.s.b.w.servlet.ServletRegistrationBean  : Mapping servlet: 'dispatcherServlet' to [/]
2017-09-05 02:08:48.291  INFO 1 --- [ost-startStop-1] c.s.j.s.i.a.WebApplicationImpl           : Initiating Jersey application, version 'Jersey: 1.19.1 03/11/2016 02:08 PM'
2017-09-05 02:08:48.401  INFO 1 --- [ost-startStop-1] c.n.d.provider.DiscoveryJerseyProvider   : Using JSON encoding codec LegacyJacksonJson
2017-09-05 02:08:48.403  INFO 1 --- [ost-startStop-1] c.n.d.provider.DiscoveryJerseyProvider   : Using JSON decoding codec LegacyJacksonJson
2017-09-05 02:08:48.539  INFO 1 --- [ost-startStop-1] c.n.d.provider.DiscoveryJerseyProvider   : Using XML encoding codec XStreamXml
2017-09-05 02:08:48.539  INFO 1 --- [ost-startStop-1] c.n.d.provider.DiscoveryJerseyProvider   : Using XML decoding codec XStreamXml
2017-09-05 02:08:49.114  INFO 1 --- [           main] o.s.c.n.eureka.InstanceInfoFactory       : Setting initial instance status as: STARTING
2017-09-05 02:08:49.177  INFO 1 --- [           main] com.netflix.discovery.DiscoveryClient    : Client configured to neither register nor query for data.
2017-09-05 02:08:49.190  INFO 1 --- [           main] com.netflix.discovery.DiscoveryClient    : Discovery Client initialized at timestamp 1504577329190 with initial instances count: 0
2017-09-05 02:08:49.282  INFO 1 --- [           main] c.n.eureka.DefaultEurekaServerContext    : Initializing ...
2017-09-05 02:08:49.285  WARN 1 --- [           main] c.n.eureka.cluster.PeerEurekaNodes       : The replica size seems to be empty. Check the route 53 DNS Registry
2017-09-05 02:08:49.298  INFO 1 --- [           main] c.n.e.registry.AbstractInstanceRegistry  : Finished initializing remote region registries. All known remote regions: []
2017-09-05 02:08:49.299  INFO 1 --- [           main] c.n.eureka.DefaultEurekaServerContext    : Initialized
2017-09-05 02:08:49.734  INFO 1 --- [           main] s.w.s.m.m.a.RequestMappingHandlerAdapter : Looking for @ControllerAdvice: org.springframework.boot.context.embedded.AnnotationConfigEmbeddedWebApplicationContext@1d251891: startup date [Tue Sep 05 02:08:44 UTC 2017]; parent: org.springframework.context.annotation.AnnotationConfigApplicationContext@3d82c5f3
2017-09-05 02:08:49.874  INFO 1 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/lastn],methods=[GET]}" onto public java.lang.String org.springframework.cloud.netflix.eureka.server.EurekaController.lastn(javax.servlet.http.HttpServletRequest,java.util.Map<java.lang.String, java.lang.Object>)
2017-09-05 02:08:49.875  INFO 1 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/],methods=[GET]}" onto public java.lang.String org.springframework.cloud.netflix.eureka.server.EurekaController.status(javax.servlet.http.HttpServletRequest,java.util.Map<java.lang.String, java.lang.Object>)
2017-09-05 02:08:49.877  INFO 1 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/error],produces=[text/html]}" onto public org.springframework.web.servlet.ModelAndView org.springframework.boot.autoconfigure.web.BasicErrorController.errorHtml(javax.servlet.http.HttpServletRequest,javax.servlet.http.HttpServletResponse)
2017-09-05 02:08:49.877  INFO 1 --- [           main] s.w.s.m.m.a.RequestMappingHandlerMapping : Mapped "{[/error]}" onto public org.springframework.http.ResponseEntity<java.util.Map<java.lang.String, java.lang.Object>> org.springframework.boot.autoconfigure.web.BasicErrorController.error(javax.servlet.http.HttpServletRequest)
2017-09-05 02:08:49.921  INFO 1 --- [           main] o.s.w.s.handler.SimpleUrlHandlerMapping  : Mapped URL path [/webjars/**] onto handler of type [class org.springframework.web.servlet.resource.ResourceHttpRequestHandler]
2017-09-05 02:08:49.921  INFO 1 --- [           main] o.s.w.s.handler.SimpleUrlHandlerMapping  : Mapped URL path [/**] onto handler of type [class org.springframework.web.servlet.resource.ResourceHttpRequestHandler]
2017-09-05 02:08:49.981  INFO 1 --- [           main] o.s.w.s.handler.SimpleUrlHandlerMapping  : Mapped URL path [/**/favicon.ico] onto handler of type [class org.springframework.web.servlet.resource.ResourceHttpRequestHandler]
2017-09-05 02:08:50.746  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/mappings || /mappings.json],methods=[GET],produces=[application/json]}" onto public java.lang.Object org.springframework.boot.actuate.endpoint.mvc.EndpointMvcAdapter.invoke()
2017-09-05 02:08:50.747  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/archaius || /archaius.json],methods=[GET],produces=[application/json]}" onto public java.lang.Object org.springframework.boot.actuate.endpoint.mvc.EndpointMvcAdapter.invoke()
2017-09-05 02:08:50.747  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/info || /info.json],methods=[GET],produces=[application/json]}" onto public java.lang.Object org.springframework.boot.actuate.endpoint.mvc.EndpointMvcAdapter.invoke()
2017-09-05 02:08:50.748  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/env],methods=[POST]}" onto public java.lang.Object org.springframework.cloud.context.environment.EnvironmentManagerMvcEndpoint.value(java.util.Map<java.lang.String, java.lang.String>)
2017-09-05 02:08:50.748  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/env/reset],methods=[POST]}" onto public java.util.Map<java.lang.String, java.lang.Object> org.springframework.cloud.context.environment.EnvironmentManagerMvcEndpoint.reset()
2017-09-05 02:08:50.749  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/beans || /beans.json],methods=[GET],produces=[application/json]}" onto public java.lang.Object org.springframework.boot.actuate.endpoint.mvc.EndpointMvcAdapter.invoke()
2017-09-05 02:08:50.750  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/dump || /dump.json],methods=[GET],produces=[application/json]}" onto public java.lang.Object org.springframework.boot.actuate.endpoint.mvc.EndpointMvcAdapter.invoke()
2017-09-05 02:08:50.752  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/configprops || /configprops.json],methods=[GET],produces=[application/json]}" onto public java.lang.Object org.springframework.boot.actuate.endpoint.mvc.EndpointMvcAdapter.invoke()
2017-09-05 02:08:50.752  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/resume || /resume.json],methods=[POST]}" onto public java.lang.Object org.springframework.cloud.endpoint.GenericPostableMvcEndpoint.invoke()
2017-09-05 02:08:50.753  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/metrics/{name:.*}],methods=[GET],produces=[application/json]}" onto public java.lang.Object org.springframework.boot.actuate.endpoint.mvc.MetricsMvcEndpoint.value(java.lang.String)
2017-09-05 02:08:50.754  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/metrics || /metrics.json],methods=[GET],produces=[application/json]}" onto public java.lang.Object org.springframework.boot.actuate.endpoint.mvc.EndpointMvcAdapter.invoke()
2017-09-05 02:08:50.756  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/autoconfig || /autoconfig.json],methods=[GET],produces=[application/json]}" onto public java.lang.Object org.springframework.boot.actuate.endpoint.mvc.EndpointMvcAdapter.invoke()
2017-09-05 02:08:50.757  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/health || /health.json],produces=[application/json]}" onto public java.lang.Object org.springframework.boot.actuate.endpoint.mvc.HealthMvcEndpoint.invoke(java.security.Principal)
2017-09-05 02:08:50.757  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/pause || /pause.json],methods=[POST]}" onto public java.lang.Object org.springframework.cloud.endpoint.GenericPostableMvcEndpoint.invoke()
2017-09-05 02:08:50.758  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/features || /features.json],methods=[GET],produces=[application/json]}" onto public java.lang.Object org.springframework.boot.actuate.endpoint.mvc.EndpointMvcAdapter.invoke()
2017-09-05 02:08:50.760  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/restart || /restart.json],methods=[POST]}" onto public java.lang.Object org.springframework.cloud.context.restart.RestartMvcEndpoint.invoke()
2017-09-05 02:08:50.761  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/env/{name:.*}],methods=[GET],produces=[application/json]}" onto public java.lang.Object org.springframework.boot.actuate.endpoint.mvc.EnvironmentMvcEndpoint.value(java.lang.String)
2017-09-05 02:08:50.761  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/env || /env.json],methods=[GET],produces=[application/json]}" onto public java.lang.Object org.springframework.boot.actuate.endpoint.mvc.EndpointMvcAdapter.invoke()
2017-09-05 02:08:50.762  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/heapdump || /heapdump.json],methods=[GET],produces=[application/octet-stream]}" onto public void org.springframework.boot.actuate.endpoint.mvc.HeapdumpMvcEndpoint.invoke(boolean,javax.servlet.http.HttpServletRequest,javax.servlet.http.HttpServletResponse) throws java.io.IOException,javax.servlet.ServletException
2017-09-05 02:08:50.762  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/trace || /trace.json],methods=[GET],produces=[application/json]}" onto public java.lang.Object org.springframework.boot.actuate.endpoint.mvc.EndpointMvcAdapter.invoke()
2017-09-05 02:08:50.763  INFO 1 --- [           main] o.s.b.a.e.mvc.EndpointHandlerMapping     : Mapped "{[/refresh || /refresh.json],methods=[POST]}" onto public java.lang.Object org.springframework.cloud.endpoint.GenericPostableMvcEndpoint.invoke()
2017-09-05 02:08:50.946  INFO 1 --- [           main] o.s.ui.freemarker.SpringTemplateLoader   : SpringTemplateLoader for FreeMarker: using resource loader [org.springframework.boot.context.embedded.AnnotationConfigEmbeddedWebApplicationContext@1d251891: startup date [Tue Sep 05 02:08:44 UTC 2017]; parent: org.springframework.context.annotation.AnnotationConfigApplicationContext@3d82c5f3] and template loader path [classpath:/templates/]
2017-09-05 02:08:50.948  INFO 1 --- [           main] o.s.w.s.v.f.FreeMarkerConfigurer         : ClassTemplateLoader for Spring macros added to FreeMarker configuration
2017-09-05 02:08:51.035  WARN 1 --- [           main] o.s.c.n.a.ArchaiusAutoConfiguration      : No spring.application.name found, defaulting to 'application'
2017-09-05 02:08:51.040  WARN 1 --- [           main] c.n.c.sources.URLConfigurationSource     : No URLs will be polled as dynamic configuration sources.
2017-09-05 02:08:51.040  INFO 1 --- [           main] c.n.c.sources.URLConfigurationSource     : To enable URLs as dynamic configuration sources, define System property archaius.configurationSource.additionalUrls or make config.properties available on classpath.
2017-09-05 02:08:51.048  WARN 1 --- [           main] c.n.c.sources.URLConfigurationSource     : No URLs will be polled as dynamic configuration sources.
2017-09-05 02:08:51.048  INFO 1 --- [           main] c.n.c.sources.URLConfigurationSource     : To enable URLs as dynamic configuration sources, define System property archaius.configurationSource.additionalUrls or make config.properties available on classpath.
2017-09-05 02:08:51.141  INFO 1 --- [           main] o.s.j.e.a.AnnotationMBeanExporter        : Registering beans for JMX exposure on startup
2017-09-05 02:08:51.150  INFO 1 --- [           main] o.s.j.e.a.AnnotationMBeanExporter        : Bean with name 'refreshScope' has been autodetected for JMX exposure
2017-09-05 02:08:51.151  INFO 1 --- [           main] o.s.j.e.a.AnnotationMBeanExporter        : Bean with name 'environmentManager' has been autodetected for JMX exposure
2017-09-05 02:08:51.152  INFO 1 --- [           main] o.s.j.e.a.AnnotationMBeanExporter        : Bean with name 'configurationPropertiesRebinder' has been autodetected for JMX exposure
2017-09-05 02:08:51.153  INFO 1 --- [           main] o.s.j.e.a.AnnotationMBeanExporter        : Bean with name 'refreshEndpoint' has been autodetected for JMX exposure
2017-09-05 02:08:51.154  INFO 1 --- [           main] o.s.j.e.a.AnnotationMBeanExporter        : Bean with name 'restartEndpoint' has been autodetected for JMX exposure
2017-09-05 02:08:51.157  INFO 1 --- [           main] o.s.j.e.a.AnnotationMBeanExporter        : Located managed bean 'environmentManager': registering with JMX server as MBean [org.springframework.cloud.context.environment:name=environmentManager,type=EnvironmentManager]
2017-09-05 02:08:51.171  INFO 1 --- [           main] o.s.j.e.a.AnnotationMBeanExporter        : Located managed bean 'restartEndpoint': registering with JMX server as MBean [org.springframework.cloud.context.restart:name=restartEndpoint,type=RestartEndpoint]
2017-09-05 02:08:51.183  INFO 1 --- [           main] o.s.j.e.a.AnnotationMBeanExporter        : Located managed bean 'refreshScope': registering with JMX server as MBean [org.springframework.cloud.context.scope.refresh:name=refreshScope,type=RefreshScope]
2017-09-05 02:08:51.195  INFO 1 --- [           main] o.s.j.e.a.AnnotationMBeanExporter        : Located managed bean 'configurationPropertiesRebinder': registering with JMX server as MBean [org.springframework.cloud.context.properties:name=configurationPropertiesRebinder,context=1d251891,type=ConfigurationPropertiesRebinder]
2017-09-05 02:08:51.202  INFO 1 --- [           main] o.s.j.e.a.AnnotationMBeanExporter        : Located managed bean 'refreshEndpoint': registering with JMX server as MBean [org.springframework.cloud.endpoint:name=refreshEndpoint,type=RefreshEndpoint]
2017-09-05 02:08:51.403  INFO 1 --- [           main] o.s.c.support.DefaultLifecycleProcessor  : Starting beans in phase 0
2017-09-05 02:08:51.405  INFO 1 --- [      Thread-10] o.s.c.n.e.server.EurekaServerBootstrap   : Setting the eureka configuration..
2017-09-05 02:08:51.405  INFO 1 --- [           main] c.n.e.EurekaDiscoveryClientConfiguration : Registering application unknown with eureka with status UP
2017-09-05 02:08:51.405  INFO 1 --- [      Thread-10] o.s.c.n.e.server.EurekaServerBootstrap   : Eureka data center value eureka.datacenter is not set, defaulting to default
2017-09-05 02:08:51.406  INFO 1 --- [      Thread-10] o.s.c.n.e.server.EurekaServerBootstrap   : Eureka environment value eureka.environment is not set, defaulting to test
2017-09-05 02:08:51.423  INFO 1 --- [      Thread-10] o.s.c.n.e.server.EurekaServerBootstrap   : isAws returned false
2017-09-05 02:08:51.423  INFO 1 --- [      Thread-10] o.s.c.n.e.server.EurekaServerBootstrap   : Initialized server context
2017-09-05 02:08:51.424  INFO 1 --- [      Thread-10] c.n.e.r.PeerAwareInstanceRegistryImpl    : Got 1 instances from neighboring DS node
2017-09-05 02:08:51.424  INFO 1 --- [      Thread-10] c.n.e.r.PeerAwareInstanceRegistryImpl    : Renew threshold is: 1
2017-09-05 02:08:51.424  INFO 1 --- [      Thread-10] c.n.e.r.PeerAwareInstanceRegistryImpl    : Changing status to UP
2017-09-05 02:08:51.433  INFO 1 --- [      Thread-10] e.s.EurekaServerInitializerConfiguration : Started Eureka Server
2017-09-05 02:08:51.517  INFO 1 --- [           main] s.b.c.e.t.TomcatEmbeddedServletContainer : Tomcat started on port(s): 8761 (http)
2017-09-05 02:08:51.518  INFO 1 --- [           main] c.n.e.EurekaDiscoveryClientConfiguration : Updating port to 8761
2017-09-05 02:08:51.522  INFO 1 --- [           main] c.itmuch.cloud.study.EurekaApplication   : Started EurekaApplication in 9.266 seconds (JVM running for 10.115)
```

查看运行的镜像容器：



```
[root@mysqlserver microservice]# docker ps
CONTAINER ID        IMAGE                                               COMMAND                CREATED             STATUS              PORTS                    NAMES
11d1b30ebe2b        boonyadocker/centos-microservice-discovery-eureka   "java -Djava.securit   36 minutes ago      Up 36 minutes       0.0.0.0:8761->8761/tcp   sharp_heisenberg    
[root@mysqlserver microservice]#
```

访问测试——打开浏览器输入**宿主的ip：8761**进行访问

![](https://img-blog.csdn.net/20170905104014209)






参考地址：[https://github.com/eacdy/spring-cloud-book/blob/master/3%20%E4%BD%BF%E7%94%A8Docker%E6%9E%84%E5%BB%BA%E5%BE%AE%E6%9C%8D%E5%8A%A1/3.6%20%E4%BD%BF%E7%94%A8Dockerfile%E6%9E%84%E5%BB%BADocker%E9%95%9C%E5%83%8F.md](https://github.com/eacdy/spring-cloud-book/blob/master/3%20%E4%BD%BF%E7%94%A8Docker%E6%9E%84%E5%BB%BA%E5%BE%AE%E6%9C%8D%E5%8A%A1/3.6%20%E4%BD%BF%E7%94%A8Dockerfile%E6%9E%84%E5%BB%BADocker%E9%95%9C%E5%83%8F.md)


，



