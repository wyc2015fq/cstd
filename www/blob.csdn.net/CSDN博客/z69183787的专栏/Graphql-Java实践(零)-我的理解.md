# Graphql-Java实践(零)-我的理解 - z69183787的专栏 - CSDN博客
2019年02月03日 10:35:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：112
个人分类：[Graphql-X](https://blog.csdn.net/z69183787/article/category/7601603)
开门见山，谈一谈我这一个星期的对GraphQL的认识：
## Schema定义的理解
刚开始认为，暴露给外部的API类似Java类中的方法，忽视了其实API和自定义类在GraphQL中都是自定义Type。而对方法的定义其实都是对字段的定义，类似Pojo中的属性。
|123456789101112|schema {    query: userQuery}type userQuery {    selectUsers(id: Int): [User]}type User {    id: Int    age: Int    userName: String!    dogs(dogId:Int): [Dog]}|
拿这个例子来说，**id:Int** 和 **selectUsers(id: Int): [User]** 其实没有本质性的区别，都是这样的格式：
**“Field”（Arguments）-可省略 ：Type（标量类型或自定义类型）**，这样一看，结合官方说的DataFetcher、TypeResolver、Arguments只能放在Field上，我也就能释然了，其实每一个字段都可以定义DataFetcher、TypeResolver、Arguments（如果有的话）。
另外，查看别人Demo的时候经常看到如下IDL定义：
|12345678|type Query{    Field A    Field B}type Mutation{    Field C    Field C}|
省略了
|1234|schema {    query:xxxx    mutation:yyyy}|
后来了解到原来Schema中 **Query** 和 **Mutation** 是默认的注册Type名，所以可以省去schema：xxxx的定义
Default：
|1234|schema {    query:Query    mutation:Mutation}|
## 一些参考
学习的过程中，因为官方文档的不完整，网上参考了许多第三方的代码和实现方式，帮助了我很多：
[GraphQL中文网](http://blog.mygraphql.com/wordpress/?p=102)：翻译了官方的GraphQL-Java的文档
[GitHub代码](https://github.com/zhaiqianfeng/GraphQL-Demo/blob/master/java/src/main/java/com/zqf/advance/GraphQL_argument.java)：使用Java定义Schema执行Query和Mutation，研究Mutation中Variable传递的时候见解了很多
[SpringBoot集合](https://my.oschina.net/genghz/blog/1789240)
[Web集成代码1](https://www.programcreek.com/java-api-examples/?code=rylitalo/graphql-with-springboot-jpa-example/graphql-with-springboot-jpa-example-master/src/main/java/com/byteperceptions/graphqldemo/graphql/GraphQLQueryController.java)
[Web集成代码2](https://www.programcreek.com/java-api-examples/?code=ERS-HCL/cart-cqrs-demo/cart-cqrs-demo-master/cart-query/src/main/java/com/onefoundation/cqrsdemo/cartquery/graphql/CartSchemaBuilder.java)：都是很好的参考。
### 第三方工具包
项目中使用的都是GraphqL Java基础包，后来在查看别人的项目实例时，发现GraphQL-Java官方还推出了一个工具包，可以帮助我们映射Schema文件到Java的Pojo上。
|12345|<dependency>    <groupId>com.graphql-java</groupId>    <artifactId>graphql-java-tools</artifactId>    <version>4.3.0</version></dependency>|
[HomePage](https://github.com/graphql-java/graphql-java-tools)
这个工具包实现了将所有的Schema中定义的type的字段Field与相同名称的Java类中的方法名进行指定规定下的绑定（即不用自己做手动映射了），方便了许多，大家可以自己看一下Example。
未完待续、、、
