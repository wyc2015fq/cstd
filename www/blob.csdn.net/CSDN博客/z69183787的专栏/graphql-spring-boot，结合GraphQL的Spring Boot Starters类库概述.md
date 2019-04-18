# graphql-spring-boot，结合GraphQL的Spring Boot Starters类库概述 - z69183787的专栏 - CSDN博客
2018年05月02日 17:00:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：869
[https://blog.csdn.net/taiyangdao/article/details/79998494](https://blog.csdn.net/taiyangdao/article/details/79998494)
GraphQL服务器能够提供的API查询请求，往往是通过HTTP实现的。而Spring Boot能够方便地构建一个Web服务，于是自然就引入了Spring Boot使能的GraphQL服务器。
graphql-spring-boot是一个Spring Boot应用的辅助类库，基于GraphQL Java实现的graphql-java和graphql-java-servlet，能够以Spring Boot应用的方式构建一个GraphQL服务器。
graphql-spring-boot类库的全名为GraphQL and GraphiQL Spring Framework Boot Starters。GraphiQL是与GraphQL功能类似的一个测试服务器，这里不详述。
graphql-spring-boot类库的最新版本是2018.1.12发布的4.0.0。其使用要求：
- 
Java 1.8
- 
Spring Boot 2.x.x
1. 组成
graphql-spring-boot类库包含两个彼此独立的子项目：
- 
com.graphql-java.graphql-spring-boot-starter
- 
com.graphql-java.graphiql-spring-boot-starter
2. 在Spring Boot的Maven项目中的配置
pom.xml文件中添加如下依赖：
[html][view
 plain](https://blog.csdn.net/taiyangdao/article/details/79998494#)[copy](https://blog.csdn.net/taiyangdao/article/details/79998494#)
- <dependency>
- <groupId>com.graphql-java</groupId>
- <artifactId>graphql-spring-boot-starter</artifactId>
- <version>4.0.0</version>
- </dependency>
3. 定义GraphQLSchema对象，并使用@Bean标注为Spring bean
这里的示例以简化的方法直接返回GraphQLSchema对象如下：
[html][view
 plain](https://blog.csdn.net/taiyangdao/article/details/79998494#)[copy](https://blog.csdn.net/taiyangdao/article/details/79998494#)
- @Bean  
- GraphQLSchema schema() {  
-     String schema = "type Query {hello: String}";  
- 
-     SchemaParser schemaParser = new SchemaParser();  
-     TypeDefinitionRegistry typeDefinitionRegistry = schemaParser.parse(schema);  
- 
-     RuntimeWiring runtimeWiring = newRuntimeWiring()  
-             .type("Query", builder -> builder.dataFetcher("hello", new StaticDataFetcher("Xiangbin")))  
-             .build();  
- 
-     SchemaGenerator schemaGenerator = new SchemaGenerator();  
-     return schemaGenerator.makeExecutableSchema(typeDefinitionRegistry, runtimeWiring);  
4.GraphQL服务器的URI设置
由于graphql-spring-boot依赖graphql-java-servlet，所以默认开放的Web访问endpoint为/graphql，这是在graphql-java-servlet中定义的。
如果要使用其他的endpoint，可以通过Spring Boot的配置文件，在application.properties文件中配置如下：
[html][view
 plain](https://blog.csdn.net/taiyangdao/article/details/79998494#)[copy](https://blog.csdn.net/taiyangdao/article/details/79998494#)
- graphql.servlet.mapping=/graphql  
- graphql.servlet.enabled=true
- graphql.servlet.corsEnabled=true
或者在application.yaml文件中配置如下：
[html][view
 plain](https://blog.csdn.net/taiyangdao/article/details/79998494#)[copy](https://blog.csdn.net/taiyangdao/article/details/79998494#)
- graphql:  
-       servlet:  
-            mapping: /graphql  
-            enabled: true  
-            corsEnabled: true  
5.启动Spring Boot应用
可以在IDE中直接run，也可以在命令行中执行java -jar my_project.jar
6.浏览器访问
使用HTTP POST请求，请求的URI为http://localhost:8080/graphql/，请求的body为{"query":"{hello}"}。
这样就可以得到响应的结果{"data":{"hello": "Xiangbin"}}。
参考链接：
https://github.com/graphql-java/graphql-spring-boot
