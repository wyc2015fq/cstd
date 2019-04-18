# Graphql-Java实践(一)-查询 - z69183787的专栏 - CSDN博客
2019年02月03日 10:36:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：252
对于服务端来说，核心就是定义Schema，组合成一套对外提供的API
**GraphQL-Java**提供两种方法来定义Schema
- Java代码定义
- GraphQL SDL（IDL）定义
# Java
## 简单查询
说明一下，GraphQL-Java需要Java8支持，会频繁使用Lambda表达式，这边直接上实例先来一个java的例子：
maven依赖：
|```12345```|```<dependency>    <groupId>com.graphql-java</groupId>    <artifactId>graphql-java</artifactId>    <version>8.0</version></dependency>```|
很多方法和包都是static import，注意一下，不然都是报错
|```123456```|```import static graphql.Scalars.GraphQLString;import static graphql.schema.AsyncDataFetcher.async;import static graphql.schema.GraphQLArgument.newArgument;import static graphql.schema.GraphQLFieldDefinition.newFieldDefinition;import static graphql.schema.GraphQLObjectType.newObject;import static graphql.schema.idl.RuntimeWiring.newRuntimeWiring;```|
Pojo：
|```12345678910```|```public class User {    private int id;    private int age;    private String userName;    private List<Dog> dogs;}public class Dog {    private int id;    private String dogName;}```|
首先基于User和Dog定义2个自定义对象类型，Dog：
|```12345678```|```public static GraphQLObjectType getDogType(){    GraphQLObjectType dogType = newObject()            .name("dog")            .field(newFieldDefinition().name("id").type(Scalars.GraphQLInt).build())            .field(newFieldDefinition().name("dogName").type(GraphQLString).build())            .build();    return dogType;}```|
Person：
|```1234567891011```|```//user type    public static GraphQLObjectType getUserType(){        GraphQLObjectType userType = newObject()                .name("user")                .field(newFieldDefinition().name("id").type(Scalars.GraphQLInt)).build())                .field(newFieldDefinition().name("age").type(Scalars.GraphQLInt).build())                .field(newFieldDefinition().name("userName").type(GraphQLString).build())                .field(newFieldDefinition().name("dogs").type(new GraphQLList(getDogType())).build())                .build();        return userType;    }```|
对于每个Field需要定义对应的类型，而且基本上GraphQL构建对象都是使用的**建造者模式**。GraphQL中对象类型GraphQLObjectType也可以成为暴露给客户端的API，并不是只能用来自定义类型。
Query API：
|```123456789101112131415161718192021```|```//query    public static GraphQLObjectType userQuery(){    //getUser 返回构造的user对象    //new GraphQLList 代表返回的是List<User> 为了测试，这里只放一个        return newObject()                .name("userQuery")                .field(newFieldDefinition().type(new GraphQLList(getUserType())).name("user").staticValue(getUsers()))                .build();    }    //schema    public static void mainExec() throws InterruptedException {        //创建Schema        GraphQLSchema schema = GraphQLSchema.newSchema()                .query(userQuery())                .build();        //测试输出        GraphQL graphQL = GraphQL.newGraphQL(schema).build();        Map<String, Object> result = graphQL.execute("query userQuery{user{id,age,dogs}}").getData();        System.out.println(result);}```|
Console：
{user=[{id=1, age=4, dogs=[{id=100, dogName=Dog19fbbb43-db17-4ca8-a515-7ee8a28c92a9}]}]}
## 参数查询
接下来看看，如何做到根据查询参数来获取结果
每个字段都有自己的 DataFetcher，如果未为字段指定DataFetcher,，那么自动使用默认的PropertyDataFetcher .PropertyDataFetcher 从 Map 和 Java Beans 中获取数据. 所以，当Schema中的field名，与Map中的key值，或 Source Object 中的 java bean 字段名相同时，不需要为field指定 DataFetcher。
而对象 TypeResolver 帮助 graphql-java 判断数据的实际类型(type). 所以 Interface 和 Union 均需要指定关联的 TypeResolver(类型识别器) .
以上面那个例子为基础，如果我们想查询ID为1的User，代码做如下变动，注意所有的Argument只能定义在Field上。
|```123456789101112131415161718192021222324252627282930313233343536```|```//创建查询Field    public static GraphQLFieldDefinition createUserField(){        return newFieldDefinition().name("user").argument(newArgument().name("id").type(Scalars.GraphQLInt).build()).                //返回类型                type(new GraphQLList(getUserType())).                dataFetcher(environment -> {                    //上一级对象数据 environment.getSource()                    //Map<String,Object> environment.getArguments()                    //环境上下文，整个查询冒泡中都可以使用 environment.getContext()                    Integer id = environment.getArgument("id");                    System.out.println("argument:id="+id);                    // repository 处理                    return getUsers(id);                }).                build();    }    //query    public static GraphQLObjectType userQuery(){        return newObject()                .name("userQuery")                .field(createUserField())                .build();    }    //schema    public static void mainExec() throws InterruptedException {        //创建Schema        GraphQLSchema schema = GraphQLSchema.newSchema()                .query(userQuery())                .build();        //测试输出        GraphQL graphQL = GraphQL.newGraphQL(schema).build();        Map<String, Object> result = graphQL.execute("query userQuery{user(id:1){id,age,dogs{id,dogName}}}").getData();        System.out.println(result);    }```|
Console：
argument:id=1
{user=[{id=1, age=6, dogs=[{id=100, dogName=Dog14afa69d-42ce-49e9-ae48-9873ef68ece1}]}]}
在Schema中增加参数后，使用时带不带参数均可，如果希望使用动态参数Variable的话，查询只需要作如下改变：
|```1234```|```Map<String,Object> variable = Maps.newHashMap();    variable.put("iidd",1);    ExecutionInput executionInput = ExecutionInput.newExecutionInput().variables(variable).query("query userQuery($iidd:Int){user(id:$iidd){id,age,dogs{id,dogName}}}").build();Map<String, Object> result = graphQL.execute(executionInput).getData();```|
ExecutionInput可以构造出一个标准的GraphQL-HTTP查询的请求体及Input执行，看了它的内部成员就一目了然了
|```12345```|```private final String query;private final String operationName;private final Object context;private final Object root;private final Map<String, Object> variables;```|
# IDL
IDL与Java实现的区别就是需要定义后缀为graphqls的schema文件，其中包含了Java定义中的类型，参数等等，可以看出无论是最终暴露给Schema的API userQuery还是 自定义类型User和Dog，都保持着GraphQL的一致类型结构，[]代表数组
**变量名 ：参数类型** ：
|```12345678910111213141516```|```schema {    query: userQuery}type userQuery {    user(id: Int): [User]}type User {    id: Int    age: Int    userName: String!    dogs: [Dog]}type Dog {    id: Int    dogName: String!}```|
注意，IDL中定义的Type会自动和项目中同名的类进行关联，字段也是同理。相应的，对于非标量类型的字段，需要自行绑定，包括 DataFetcher , TypeResolvers等。若未自定义绑定，则会自动使用默认PropertyDataFetcher调用当前所在对象实例的getXXX方法，比如例子中user内部的dogs，因未自定义，故会被赋值为用user实例的getDogs方法的返回值。
|```12345678910111213141516171819202122```|```//加载schema文件    private static File loadSchema(final String s) {        System.out.println(GraphqlTest2.class.getClassLoader().getResource("graphql/userDemo.graphqls"));        return new File(GraphqlTest2.class.getClassLoader().getResource(s).getFile());    }    //构建一个运行时Java语义 绑定schema，包括（datafetcher、typeResolver、customScalar）    private static RuntimeWiring buildRuntimeWiring() {        //return RuntimeWiring.newRuntimeWiring().wiringFactory(new EchoingWiringFactory()).build();        return RuntimeWiring.newRuntimeWiring()                // this uses builder function lambda syntax                .type("userQuery", typeWiring -> typeWiring                                .dataFetcher("user", environment -> {                                    //上一级对象数据 environment.getSource()                                    //Map<String,Object> environment.getArguments()                                    //环境上下文，整个查询冒泡中都可以使用 environment.getContext()                                    Integer id = environment.getArgument("id");                                    System.out.println("argument:id=" + id);                                    // repository 处理                                    return getUsers(id);                                })                ).build();    }```|
最后，你可以通过整合静态 Schema 和 绑定（wiring），而生成一个可以执行的
Schema,最终结果与Java代码定义的schema运行一致
|```12345678910111213141516```|```SchemaParser schemaParser = new SchemaParser();SchemaGenerator schemaGenerator = new SchemaGenerator();File schemaFile = loadSchema("graphql/userDemo.graphqls");TypeDefinitionRegistry typeRegistry = schemaParser.parse(schemaFile);RuntimeWiring wiring = buildRuntimeWiring();GraphQLSchema schema = schemaGenerator.makeExecutableSchema(typeRegistry, wiring);//测试输出GraphQL graphQL = GraphQL.newGraphQL(schema).build();Map<String,Object> variable = Maps.newHashMap();variable.put("iidd",1);ExecutionInput executionInput = ExecutionInput.newExecutionInput().variables(variable).        query("query userQuery($iidd:Int){user(id:$iidd){id,age,dogs{id,dogName}}}").build();Map<String, Object> result = graphQL.execute(executionInput).getData();System.out.println(result);```|
给一个参数是数组[]的例子，只有一点点小区别：
|```123```|```type userQuery {    user(id: [Int]): [User]}```|
|```1234```|```ExecutionInput executionInput = ExecutionInput.newExecutionInput().variables(variable).query("query userQuery($iidd:[Int],$dogId:Int){user(id:$iidd){id,age,dogs(dogId:$dogId){id,dogName}}}").build();//DataFetcherList<Integer> id = environment.getArgument("id");System.out.println("argument:id=" + id);```|
当Schema文件有多个的时候，可以通过以下方式进行读取融合：
|```12345678910```|```File schemaFile1 = loadSchema("starWarsSchemaPart1.graphqls");File schemaFile2 = loadSchema("starWarsSchemaPart2.graphqls");File schemaFile3 = loadSchema("starWarsSchemaPart3.graphqls");TypeDefinitionRegistry typeRegistry = new TypeDefinitionRegistry();// each registry is merged into the main registrytypeRegistry.merge(schemaParser.parse(schemaFile1));typeRegistry.merge(schemaParser.parse(schemaFile2));typeRegistry.merge(schemaParser.parse(schemaFile3));```|
说完两种构造Schema的方式后，我们来考虑一下场景，还是User和Dog的数据结构，如果User信息需要通过RPC从A应用处获取，而Dog信息需要由每一个userId调用应用B的RPC服务来获取，应该如何操作呢？
之前几篇文章中提到过，GraphQL引擎会一直解析字段及自定义对象类型，依次冒泡查询执行DataFetcher，直到遇到一个标量类型（也就是基础类型），当解析到dogs字段时，由于是自定义类型，自然会继续，而通过DataFetcher的environment可以获取到父对象及它的主键属性userId，所以解决方案也就应运而生了，但相对的，这种情况下也就会造成N+1的问题：
schema：
|```12345678910111213141516```|```schema {    query: userQuery}type userQuery {    user(id: Int): [User]}type User {    id: Int    age: Int    userName: String!    dogs(dogId:Int): [Dog]}type Dog {    id: Int    dogName: String!}```|
|```123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354```|```//构建一个运行时Java语义 绑定schema，包括（datafetcher、typeResolver、customScalar）    private static RuntimeWiring buildRuntimeWiring() {        //return RuntimeWiring.newRuntimeWiring().wiringFactory(new EchoingWiringFactory()).build();        return RuntimeWiring.newRuntimeWiring()                // this uses builder function lambda syntax                .type("userQuery", typeWiring -> typeWiring                                .dataFetcher("user", environment -> {                                    //上一级对象数据 environment.getSource()                                    //Map<String,Object> environment.getArguments()                                    //环境上下文，整个查询冒泡中都可以使用 environment.getContext()                                    Integer id = environment.getArgument("id");                                    System.out.println("argument:id=" + id);                                    // repository 处理                                    return getUsers(id);                                })                ).type("User", typeWiring -> typeWiring                        .dataFetcher("dogs", environment -> {                            //获取父对象                            User user = environment.getSource();                            int userId = user.getId();                            System.out.println("dogs outside userId = "+ userId);                            Integer paramDogId = environment.getArgument("dogId");                            System.out.println("dogs inside dogId = "+ paramDogId);                            //模拟rpc调用                            List<Dog> dogs = Lists.newArrayList();                            return dogs;                        }))                        //去掉下面的注释甚至可以改变每个dog对象里id的值                        //.type("Dog", typeWiring -> typeWiring                        //.dataFetcher("id", environment -> {                        //        return 9999;                        //}))                .build();    }    //schema    public static void mainExec() throws InterruptedException {        //创建Schema        SchemaParser schemaParser = new SchemaParser();        SchemaGenerator schemaGenerator = new SchemaGenerator();        File schemaFile = loadSchema("graphql/userDemo.graphqls");        TypeDefinitionRegistry typeRegistry = schemaParser.parse(schemaFile);        RuntimeWiring wiring = buildRuntimeWiring();        GraphQLSchema schema = schemaGenerator.makeExecutableSchema(typeRegistry, wiring);        //测试输出        GraphQL graphQL = GraphQL.newGraphQL(schema).build();        Map<String,Object> variable = Maps.newHashMap();        variable.put("iidd",1);variable.put("dogId",2);        ExecutionInput executionInput = ExecutionInput.newExecutionInput().variables(variable).                query("query userQuery($iidd:Int,$dogId:Int){user(id:$iidd){id,age,dogs(dogId:$dogId){id,dogName}}}").build();        Map<String, Object> result = graphQL.execute(executionInput).getData();        System.out.println(result);    }```|
console：
argument:id=1
dogs outside userId = 1
dogs inside dogId = 2
{user=[{id=1, age=3, dogs=[]}]}
**注意：所有的DateFetcher和Arguments都必须定义在Field上**
关于DataFetcher方法中的入参DataFetchingEnvironment，我们可以靠它获取到很多执行中的关键数据，这边列出几个我认为比较重要的，其他的大家可以自己查看它的源码
- 父级对象数据（dogs属性所在的那个user实例）
- 当前Field上Arguments通过传递的单个参数（ T getArgument(String name)）
- 当前Field上的所有参数（Map<String, Object> getArguments();）
- 获取整个GraphQL查询中的环境上下文（ T getContext();）
	在执行查询时，可以通过以下代码设置：
	|```123```|```ExecutionInput executionInput = ExecutionInput.newExecutionInput().variables(variable).context(object)...               query("query userQuery($iidd:Int){user(id:$iidd){id,age,dogs{id,dogName}}}").build();       Map<String, Object> result = graphQL.execute(executionInput).getData();```|
当面临需要获取用户态、权限等全局环境变量的时候，Context就会变得很有用，可以帮助我们在整个GraphQL引擎执行的过程中，让每一个Field的DataFetcher获取并加以处理。
通过上面的几个例子大家可以发现，通过读取IDL解析定义Schema在代码量和可读性方面都比使用Java来定义好很多，推荐使用。
**其次，每一个在Schema中定义的type都可以对内部的字段Field定义相应的DataFetcher，并且次级Field可以获取上级Field已获得的结果，就像dogs中获取父对象User实例一样，同时可以自定义自己的返回值，而不是使用默认的PropertyDataFetcher读取pojo对应属性的get/set值。**
后面在异步篇章会介绍一下在获取数据这块能否有性能更加或者更优化的方案，解决后端数据可能造成的N+1的问题以及潜在的嵌套查询。
**4月23日更新：还是IDL定义Schema的例子，今天试验了一下只保留暴露给外部使用的顶层的Field API的获取逻辑，发现就算不对自定义类型编写自定义的DataFetcher，使用默认的PropertyDataFetcher也能正常查询，甚至针对IDL文件中需要参数的Field，也可以不用专门去定义DataFetcher，默认的也可以应对所有的查询Query（这一点和Java定义Schema很不同），只是不处理传入的Arguments，返回顶层对象中的所有数据罢了。**
**传入IDL或Java没有定义过的参数，会导致验证失败。**
**反之，定义了参数，但查询语句中不传入使用，可以正常执行。**
