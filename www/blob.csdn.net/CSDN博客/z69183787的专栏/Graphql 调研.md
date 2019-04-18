# Graphql 调研 - z69183787的专栏 - CSDN博客
2019年02月03日 10:15:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：104
个人分类：[Graphql-X](https://blog.csdn.net/z69183787/article/category/7601603)
![](https://zh69183787.github.io/images/graphql.png)
# 一、背景
首先附上 [Graphsql中文官网地址](http://graphql.cn/) 及 对应的 [JavaDoc地址](http://graphql-java.readthedocs.io/en/stable/getting_started.html)
简单介绍一下背景，公司内部有很多B端运营类项目，由于后期的不断迭代，后端的Rest服务接口（Ajax）变得越来越多，恰好适逢前端重构，故领导想试试看能否使用对前端来说适应及使用性更便捷的Graphql来替换Rest。后端的重构就落到我这边了。
# 二、介绍
GraphQL可以在原本的前端-后端的调用链中添加一个中间层BFF，用来对后端微服务的数据进行集成。
BFF：Backend for Frontends(以下简称BFF) 顾名思义，是为前端而存在的后端(服务)中间层。即传统的前后端分离应用中，前端应用直接调用后端服务，后端服务再根据相关的业务逻辑进行数据的增删查改等。那么引用了 BFF 之后，前端应用将直接和 BFF 通信，BFF 再和后端进行 API 通信，所以本质上来说，BFF 更像是一种“中间层”服务。
关于IDL（接口描述语言）以及Graphql的语法文档，大家还是看官网，这边简单的介绍一下
## 语言模式
图语言，用***“节点”***和***“关系”***来描述一组数据结构，与常规数据库join的理念不同
## 操作分类
query：查询
mutation：增删改
## 构成
### Schema/GraphQLSchema：
定义所有可供查询的字段（field），它们最终组合成一套完整的GraphQL API
Schema相当于一个数据库，它有很多GraphQLFieldDefinition组成，Field相当于数据库表/视图，每个表/视图又由名称、查询参数、数据结构、数据组成。同时它定义了一个请求可以返回的数据格式与描述
### Types
GraphQL 类型系统支持以下类型
- Scalar/基础类型
- Object/GraphQLObjectType
- Interface/GraphQLInterfaceType
- Union/GraphQLUnionType
- InputObject/GraphQLInputObjectType //专门用于定义输入参数类型
- Enum/GraphQLEnumType
### Scalar
graphql-java 支持以下基本数据类型（ Scalars）
- GraphQLBoolean
- GraphQLInt
- GraphQLFloat
- GraphQLID
- GraphQLLong
- GraphQLShort
- GraphQLByte
- GraphQLFloat
- GraphQLBigDecimal
- GraphQLBigInteger
### Object/GraphQLObjectType
对象字段类型，定义了一个数据模型，类似数据表中的每一列的字段定义
### Field/GraphQLFieldDefinition
字段类型定义，可用于对象类型中的字段设置，及请求对象上的特定字段（可根据该字段定义返回类型，匹配参数等）
### Arguments/GraphQLArgument
每一个GraphQLFieldDefinition可提供的查询条件，可设置参数值，用于查询匹配
### DataFetcher
数据返回获取器，实现Field字段上的数据返回接口，可在environment中获取查询参数
### DataFetchingEnvironment
数据获取上下文，可以拿到对应的查询参数
### GraphQL.execute
最终查询执行器
简易demo可参考范例[graphql学习（四）GraphQL和SpringMVC 的整合](https://blog.csdn.net/q15150676766/article/details/74618398)
# 三、个人理解
官网上看了一下Hello World的Demo和他人基于Graphql写的一些blog，简单总结了一些优缺点和使用上的疑问：
## 优点
- 清晰的数据模型，字段强类型
- 前端-按需获取，减少网络请求
- API迭代顺畅，无须版本化
- 协议而非存储，对服务端数据进行组装过滤
## Rest比较
- 数据获取：Rest缺乏扩展性，GraphQL获取时，payload可以扩展，按需获取
- API调用：Rest有多个endpoint，GraphQL在大多数情况下只有1个endpoint，只是body内容不同
- 复杂请求：Rest需要多次，GraphQL一次调用，减少网络开销
- 返回处理：Rest有多种httpCode及Status，GraphQL只有200响应，错误内容需要在结果集中特殊获取
- 版本号：Rest使用V1、V2，GraphQL可根据Schema自行扩展
可以看出，优点中大部分都是对前端开发有利，那后端改造的优势是什么呢？简单看了几个例子后发现，后端如果使用Graphql重构，就相当于服务层做了一层类似数据库的DDL+DML的抽象和封装。
Graphql使用了自身API构造了和数据库雷同的DDL，包含表结构（整体schema）、可查询字段（Field）、行列信息（Object），可变入参（Variable），形式极似Sql的select语句。但如此一来，在使用上就有以下的疑问了
## 疑问
- 官网标榜的精确数据返回和一个请求如何实现？表面上看似返回的内容会随着请求实体变化，但实际上，由于无法确定前端查询的内容及参数，对于后端来说，还是需要获取全量的数据结果，只是在构建Graphql自己的“数据库”时定义不同的查询schema罢了。原先的“select a，b”，在Graphql上不就变成了“select * ” ？ 这样对数据库及其他内部服务的压力必然会增大，内存&缓存解决？
- 其次，对于一个完整的系统，Graphql要求的数据模型必然需要对业务精确的理解，需要提前定义完整的数据结构，每一个返回的实体都必须单独定义一个Schema及配套的查询方法。这样才能准确的定义服务端的Schema，供前端使用。
- 对于数据校验、用户权限及数据安全性来说解决方案也比较模糊，由于开放了“select *”的功能，是否会造成全量的字段查询导致数据暴露
- 文档比较匮乏，使用案例也较少
# 四、实践
两种定义schema的方法
- Java
- IDL
Example：实际使用的时候有一点需要注意，许多官网例子中一些方法都是import static方式引入的Class，有些方法在idea中很难搜到。大GraphQL中的对象构造方式大多都是建造者模式，也是Effect java中推荐的多参数的对象构造方式，大家可以自己看一下
## Java
|```123456```|```import static graphql.Scalars.GraphQLString;import static graphql.schema.AsyncDataFetcher.async;import static graphql.schema.GraphQLArgument.newArgument;import static graphql.schema.GraphQLFieldDefinition.newFieldDefinition;import static graphql.schema.GraphQLObjectType.newObject;import static graphql.schema.idl.RuntimeWiring.newRuntimeWiring;```|
|```1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253545556575859606162636465666768```|```//假设现在有 user pojo ，dog pojo，user中有list[dog],类似一对多的关系//自定义用户类型 及 字段public static GraphQLObjectType getUserType(){    GraphQLObjectType userType = newObject()            .name("user")            .field(newFieldDefinition().name("id").type(Scalars.GraphQLInt).build())            .field(newFieldDefinition().name("age").type(Scalars.GraphQLInt).build())            .field(newFieldDefinition().name("userName").type(GraphQLString).build())            .field(newFieldDefinition().name("dogs").type(new GraphQLList(getDogType())).build())            .build();    return userType;}//dog typepublic static GraphQLObjectType getDogType(){    GraphQLObjectType dogType = newObject()            .name("dog")            .field(newFieldDefinition().name("id").type(Scalars.GraphQLInt).build())            .field(newFieldDefinition().name("dogName").type(GraphQLString).build())            .build();    return dogType;}//定义一个查询Field 字段 及可查询参数与返回值类型 & 一个数据获取器 dataFetchpublic static GraphQLFieldDefinition userQuery(){    DataFetcher userDataFetcher = async(environment -> {        // 获取查询参数        Integer id = environment.getArgument("id");        Integer start = environment.getArgument("start");        Integer limit = environment.getArgument("limit");        System.out.println("GraphQLFieldDefinition query ,id = " + id                + " ,start = "+start+ " ,limit="+limit);        // 执行查询, 这里随便用一些测试数据来说明问题        //getObj 自己组装List user 数据        List<User> result = getObj();        System.out.println(result);        return result;    });    return GraphQLFieldDefinition.newFieldDefinition()            .name("users")            .argument(newArgument().name("id").type(new GraphQLNonNull(Scalars.GraphQLInt)).build())            .argument(newArgument().name("start").type(Scalars.GraphQLInt).build())            .argument(newArgument().name("limit").type(Scalars.GraphQLInt).build())            .type(new GraphQLList(getUserType()))            .dataFetcher(userDataFetcher)            .build();}//schema 定义 ，绑定 userQuery这个字段的请求查询GraphQLSchema schema = GraphQLSchema.newSchema().query(newObject()            .name("GraphQuery")            .field(userQuery())            .build()).build();//执行处理器，采用Future的方式异步获取查询结果，可使用JAVA8的lambda函数xGraphQL graphQL = GraphQL.newGraphQL(schema).queryExecutionStrategy(new AsyncExecutionStrategy())            .mutationExecutionStrategy(new AsyncSerialExecutionStrategy()).build();    ExecutionInput executionInput = ExecutionInput.newExecutionInput().query(query1).build();    CompletableFuture<ExecutionResult> promise = graphQL.executeAsync(executionInput);    //ExecutionResult executionResult = graphQL.execute(executionInput);    //promise.join();    Future<ExecutionResult> f = promise.whenComplete((v, e) -> {        System.out.println("Future: result " + v.getData());        System.out.println("Error: result " + v.getErrors());        System.out.println("Ex: result " + v.getExtensions());        e.printStackTrace();    });    promise.thenAccept(executionResult -> {        // here you might send back the results as JSON over HTTP        System.out.println("Future: result " + executionResult.getData());    });    Thread.sleep(10000);```|
输出结果：
|```123456```|```GraphQLFieldDefinition query ,id = 2 ,start = 5 ,limit=10[graphql.User@400989ba]Future: result {users=[{id=1, userName=2854bfbf-7037-4854-a470-53360b49f1fb, dogs=[{id=100, dogName=Dog52bef01d-99d8-4712-afaa-c046d61976ab}]}]}Error: result []Ex: result nullFuture: result {users=[{id=1, userName=2854bfbf-7037-4854-a470-53360b49f1fb, dogs=[{id=100, dogName=Dog52bef01d-99d8-4712-afaa-c046d61976ab}]}]}```|
## IDL
users.graphqls（编译后在classpath下）
|```12345678910111213141516```|```schema {    query: GraphQuery2}type GraphQuery2 {    users(id: Int,start: Int,limit: Int): [User]}type User {    id: Int    age: Int    userName: String!    dogs: [Dog]}type Dog {    id: Int    dogName: String!}```|
java代码（之前对于user，dog的对象类型定义都可以去除，请求Field及schema同样也可以不要）
|```12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758```|```//加载classpath下的IDL文件private static File loadSchema(final String s) {        System.out.println(GraphqlTest2.class.getClassLoader().getResource("graphql/users.graphqls"));        return new File(GraphqlTest2.class.getClassLoader().getResource(s).getFile());    }private static DataFetcher userDataFetcher = async(environment -> {    // 获取查询参数    Integer id = environment.getArgument("id");    Integer start = environment.getArgument("start");    Integer limit = environment.getArgument("limit");    System.out.println("GraphQLFieldDefinition query ,id = " + id            + " ,start = "+start+ " ,limit="+limit);    // 执行查询, 这里随便用一些测试数据来说明问题    List<User> result = getObj();    System.out.println(result);    return result;});//运行环境构建，同时注册一个schema查询及对应的field获取器private static RuntimeWiring buildRuntimeWiring() {    //return RuntimeWiring.newRuntimeWiring().wiringFactory(new EchoingWiringFactory()).build();    return RuntimeWiring.newRuntimeWiring()                    // this uses builder function lambda syntax            .type("GraphQuery2", typeWiring -> typeWiring                            .dataFetcher("users", userDataFetcher)            ).build();}public static void main(String[] args) throws InterruptedException {    SchemaParser schemaParser = new SchemaParser();    SchemaGenerator schemaGenerator = new SchemaGenerator();    File schemaFile = loadSchema("graphql/users.graphqls");    TypeDefinitionRegistry typeRegistry = schemaParser.parse(schemaFile);    RuntimeWiring wiring = buildRuntimeWiring();    //等同于java构建schema的mainExec方法    GraphQLSchema graphQLSchema = schemaGenerator.makeExecutableSchema(typeRegistry, wiring);    //$xxx 使用了查询变量，在执行实际查询时可以传入实际的variable进行替换（Map），同时在fetch数据时，在environment中获取，供持久层使用    String query1 = "query GraphQuery2($testUserId:Int) {users(id:$testUserId,start:5,limit:10) " +            " {id,userName,dogs{id,dogName}}}";    GraphQL graphQL = GraphQL.newGraphQL(graphQLSchema).queryExecutionStrategy(new AsyncExecutionStrategy())            .mutationExecutionStrategy(new AsyncSerialExecutionStrategy()).build();    Map<String,Object> variable = Maps.newHashMap();    variable.put("testUserId",12345);    //graphQL.execute(query, null, null, variables); deprecated    ExecutionInput executionInput = ExecutionInput.newExecutionInput().variables(variable).query(query1).build();    CompletableFuture<ExecutionResult> promise = graphQL.executeAsync(executionInput);    //ExecutionResult executionResult = graphQL.execute(executionInput);    //promise.join();    Future<ExecutionResult> f = promise.whenComplete((v, e) -> {        System.out.println("Future: result " + v.getData());        System.out.println("Error: result " + v.getErrors());        System.out.println("Ex: result " + v.getExtensions());        e.printStackTrace();    });    promise.thenAccept(executionResult -> {        // here you might send back the results as JSON over HTTP        System.out.println("Future: result " + executionResult.getData());    });    Thread.sleep(10000);}```|
输出结果与JAVA方式一致
结论：可以看到，IDL中可以将schema的query名称、自定义对象类型、schema内的请求查询Field统一包括，对比Java定义的方式节省很多代码及工作量，而且定义好的文件也可供前端参考。
### Todo
- 异常处理&参数校验
- SpringBoot集成
- 数据变更mutation尝试
# 结束
总结一下整个流程，基本上就是
**定义GrapQL数据类型 -> 定义暴露给客户端的query api和mutaion api -> 创建GraphQL Schema**
# 参考
[GraphQL](http://www.ayqy.net/blog/graphql/)
[官方文档翻译](https://segmentfault.com/a/1190000012119468)
[微服务下使用GraphQL构建BFF](https://zhuanlan.zhihu.com/p/35108457)
[GraphQL和SpringMVC 的整合](https://blog.csdn.net/q15150676766/article/details/74618398)
[GraphQL这个坑可以入了](https://zhuanlan.zhihu.com/p/28962262)
[GraphQL+Java实战](http://www.zhaiqianfeng.com/2017/06/learn-graphql-action-by-java.html)
[Java Code Examples for graphql.schema.idl.SchemaGenerator](https://www.programcreek.com/java-api-examples/?api=graphql.schema.idl.SchemaGenerator)
[SpringMVC+graphql Demo](https://www.programcreek.com/java-api-examples/?code=rylitalo/graphql-with-springboot-jpa-example/graphql-with-springboot-jpa-example-master/src/main/java/com/byteperceptions/graphqldemo/graphql/GraphQLQueryController.java#)
[Demo2](https://www.programcreek.com/java-api-examples/?code=ERS-HCL/cart-cqrs-demo/cart-cqrs-demo-master/cart-query/src/main/java/com/onefoundation/cqrsdemo/cartquery/graphql/CartSchemaBuilder.java#)
[Demo3](https://blog.csdn.net/qq362228416/article/details/50854980)
[Interface使用Demo](https://github.com/zhaiqianfeng/GraphQL-Demo/blob/master/java/src/main/java/com/zqf/advance/GraphQL_interface.java)
