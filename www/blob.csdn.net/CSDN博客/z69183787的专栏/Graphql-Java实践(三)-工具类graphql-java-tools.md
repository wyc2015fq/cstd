# Graphql-Java实践(三)-工具类graphql-java-tools - z69183787的专栏 - CSDN博客
2019年02月03日 10:43:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：270
开门尖山，放上github地址，大家可自行查阅使用，我这边只是尝试一下
[HomePage](https://github.com/graphql-java/graphql-java-tools)
**注：graphql-java-tools已经不更新了，内部引用的GraphQL-java版本为6.0但最新GraphQL已经更新到8.0，去除了一些6.0中的Class，所以不能另外引用最新的GraphQL-java**
首先Maven：新版本有冲突，需要注释
|```1234567891011```|```<dependency>    <groupId>com.graphql-java</groupId>    <artifactId>graphql-java-tools</artifactId>    <version>4.3.0</version></dependency><!--<dependency>-->    <!--<groupId>com.graphql-java</groupId>-->    <!--<artifactId>graphql-java</artifactId>-->    <!--<version>8.0</version>--><!--</dependency>-->```|
schema：
|```1234567891011121314151617181920212223242526272829303132```|```# 省略schema，使用默认Query,Mutationtype Query {    # 获取所有用户列表    user: [ToolUser]    # 根据id获取狗嘻嘻    dog(userId:String):[ToolDog]}type Mutation{    createUser(addUser:[InputToolUser]):[ToolUser]}input InputToolUser{    id: String!    age: Int    userName: String!    dogs:[InputToolDog]}input InputToolDog {    id: String!    dogName: String!}type ToolUser {    id: String!    age: Int    userName: String!    dogs:[ToolDog]}type ToolDog {    id: String!    dogName: String!}```|
当你申明完了Schema文件，只需在代码中定义与IDL中一样的类名与属性名，工具类会帮助你进行自动映射绑定，在执行查询时帮助我们获取数据，关于绑定映射，有以下两种情况：
标量类型：变量名字一致自动绑定
复杂/引用类型（标量类型也可以使用来设置除getXXX之外的新的返回值）：需要定义对应的Resolvers与其绑定，**GraphQLResolver**
同时，针对外部API，有特定的RootResolver **GraphQLQueryResolver** ，**GraphQLMutationResolver**，**GraphQLSubscriptionResolver**，对应着查询、变更、订阅。
无论是自定义类型或者Resolver，工具类制定了一套方法名和Schema中Field字段的对应规则，通过这些接口的实现类，可以帮助我们简化DataFetcher和Field的绑定关系。
具体可查询Github：
Field Mapping Priority（字段映射优先级）
The field mapping is done by name against public/protected methods and public/protected/private fields, with the following priority:
First on the resolver or root resolver (note that dataClassInstance doesn’t apply for root resolvers):
method (dataClassInstance, *fieldArgs [, DataFetchingEnvironment])method is(dataClassInstance, *fieldArgs [, DataFetchingEnvironment]), only if the field returns a Boolean
method get(dataClassInstance, *fieldArgs [, DataFetchingEnvironment])method getField(dataClassInstance, *fieldArgs [, DataFetchingEnvironment])
Then on the data class:
method (*fieldArgs [, DataFetchingEnvironment])method is(*fieldArgs [, DataFetchingEnvironment]), only if the field returns a Boolean
method get(*fieldArgs [, DataFetchingEnvironment])method getField(*fieldArgs [, DataFetchingEnvironment])
field
**自己测试了一下，针对DataClass，定义的method好像在执行时不会进行调用，所以暂时还是通过Resolver来获取想要的数据，无论是标量类型还是复杂类型**
这是种实现, 当提供了resolver时优先使用（在pojo是A类型，而IDL是B类型时很有用，Date类型也是）, 其次是 class this.get方法。
自定义Resolver中多了一个对象实例的参数，这个对象是当前正在处理的Field字段所在的对象实例，可以帮助我们获取对象中的属性值，在一些需要根据对象内部字段判断的场景内很有用。比如User类内部有List属性，那么getDogs方法的第一个参数就可以是user实例。
换一个角度思考，GraphQL对于Field的获取是冒泡向下获取的，直到遇到标量类型。GraphQL-java基础包中也说明了针对所有非标量类型，都可以定义一个DataFetcher，而不是使用默认的PropertyDataFetcher。这样一来，Resolver的作用也就不言而喻了。
简单来说，只要实现类中的公有方法有与Schema中Field名称满足上述规则，针对Resolver，方法的参数依次为（当前数据对象–**RootResolver没有该属性**、参数、环境上下文–可选）；针对单纯的自定义类型，方法参数依次为（参数，环境上下文–可选），Resolver多了一个当前处理的对象信息。可以看出，对于自定义类型，其实就是默认的get/set方法，和官方基础包中的PropertyDataFetcher功能一致。
至此，继续例子代码：
自定义类型：
|```123456789101112131415161718```|```@Data@AllArgsConstructorpublic class ToolUser implements Serializable{    private static final long serialVersionUID = -3272807693514763753L;    private String id;    private String userName;    private int age;    private List<ToolDog> dogs;}@Data@AllArgsConstructorpublic class ToolDog implements Serializable{    private static final long serialVersionUID = -3272807693514763753L;    private String id;    private String dogName;}```|
自定义Resolver（解决user中的非标量类型dogs的数据获取）：
|```12345678910111213141516171819```|```public class ToolUserResolver implements GraphQLResolver<ToolUser> {    //根据userId获取dogs    public List<ToolDog> getDogs(ToolUser user){        String userId = user.getId();        System.out.println("UserResolver , userId = " +userId);        return DemoRepository.dogs.get(userId);    }}//标量类型，在ToolDog中定义使用无效public class ToolDogResolver implements GraphQLResolver<ToolDog> {    //根据userId获取dogs    public String getId(ToolDog dog,DataFetchingEnvironment env){        String dogId = dog.getId();        System.out.println("ToolDogResolver , dogId = " +dogId);        return "resolver - " + dogId;    }}```|
RootResolvers-Query：
|```123456789101112```|```public class ToolQuery implements GraphQLQueryResolver {    //schema user    public List<ToolUser> user(DataFetchingEnvironment env){        return DemoRepository.users.values().stream().collect(Collectors.toList());    }    //schema dogs    public List<ToolDog> dog(String userId){        return DemoRepository.dogs.get(userId);    }}```|
RootResolvers-Mutation：
|```123456789```|```public class ToolMutation implements GraphQLMutationResolver {    //schema user    public List<ToolUser> createUser(List<InputToolUser> toolUsers){        System.out.println("createUser - " + toolUsers);        return DemoRepository.users.values().stream().collect(Collectors.toList());    }}```|
这里说一来，对于Mutation，由于是根据字段名及类名进行的自动绑定，故需要编写对应的input 自定义类型，其实蛮傻的，相当于一个类型定义了两次，官方因为只能通过map获取入参，反而没有这方面的困扰：
|```12345678910111213141516171819202122232425262728293031```|```@Data@AllArgsConstructorpublic class InputToolUser implements Serializable{    private static final long serialVersionUID = -3272807693514763753L;    private String id;    private String userName;    private int age;    private List<InputToolDog> dogs;}public class InputToolDog implements Serializable,Cloneable{    private static final long serialVersionUID = -3272807693514763753L;    private String id;    private String dogName;    public InputToolDog(){}    @Override    protected InputToolDog clone()  {        InputToolDog clone = null;        try{            clone = (InputToolDog) super.clone();        }catch(CloneNotSupportedException e){            throw new RuntimeException(e); // won't happen        }        return clone;    }}```|
这边有一点要注意的是，前面几篇文章中也提到过，到遇到复杂对象的Variable时，GraphQL引擎只会接受Map类型的数据结构。细心的你可能已经发现，这次Mutation操作的入参是一个List，所以这次的数据结构应该是List，大家在用Json作数据转换的时候需要注意了。
在这里，工具类又帮我们完成了一件事，基础包版本中，复杂类型的入参只能通过getArgument获得，而且返回值是一个Map<String,Object>，而不是这边已经帮我们转换过的对象List toolUsers。
Main函数：
|```12345678910111213141516171819202122232425```|```public static void main(String[] args) throws IllegalAccessException, InstantiationException {        new DemoRepository();        GraphQLSchema schema = SchemaParser.newParser()                .file("graphql/tool.graphqls").resolvers(new ToolQuery(),                new ToolUserResolver(),new ToolDogResolver(),new ToolMutation()).build().makeExecutableSchema();        GraphQL graphQL = GraphQL.newGraphQL(schema).build();        Map<String,Object> variable = Maps.newHashMap();       variable.put("addUser", MutationVariableHandler.getListVariablesMapFromString(JSON.toJSONString(DemoRepository.inputUsers.values().stream().               collect(Collectors.toList()), SerializerFeature.DisableCircularReferenceDetect)));//        ExecutionInput executionInput = ExecutionInput.newExecutionInput().variables(variable).//                query("query Query{user{id,userName,age,dogs{id,dogName}}}").build();//        ExecutionInput executionInput = ExecutionInput.newExecutionInput().variables(variable).//                query("query Query($userId:String){dog(userId:$userId){id,dogName}}").build();        ExecutionInput executionInput = ExecutionInput.newExecutionInput().variables(variable).                query("mutation Mutation($addUser:[InputToolUser])" +                        "{createUser(addUser:$addUser){id,userName,age,dogs{id,dogName}}}").build();        ExecutionResult result = graphQL.execute(executionInput);        Map<String, Object> data = result.getData();        List errors = result.getErrors();        System.out.println(data);        System.out.println("errors = "+errors);    }```|
Console（查询执行的结果大家可以变换注释自己执行）：
createUser - [InputToolUser(id=input-user-3, userName=uc, age=30, dogs=[InputToolDog(id=dog-2, dogName=db), InputToolDog(id=dog-3, dogName=dc)]), InputToolUser(id=input-user-2, userName=ub, age=20, dogs=[InputToolDog(id=dog-2, dogName=db)]), InputToolUser(id=input-user-1, userName=ua, age=10, dogs=[InputToolDog(id=dog-1, dogName=da), InputToolDog(id=dog-2, dogName=db)])]
UserResolver , userId = user-1
ToolDogResolver , dogId = dog-1
ToolDogResolver , dogId = dog-2
UserResolver , userId = user-3
ToolDogResolver , dogId = dog-2
ToolDogResolver , dogId = dog-3
UserResolver , userId = user-2
ToolDogResolver , dogId = dog-2
{createUser=[{id=user-1, userName=ua, age=10, dogs=[{id=resolver - dog-1, dogName=da}, {id=resolver - dog-2, dogName=db}]}, {id=user-3, userName=uc, age=30, dogs=[{id=resolver - dog-2, dogName=db}, {id=resolver - dog-3, dogName=dc}]}, {id=user-2, userName=ub, age=20, dogs=[{id=resolver - dog-2, dogName=db}]}]}
errors = []
备注：
DemoRepository：只是Mock了一些数据而已，随便写的。其实这边在运行时还碰到了一个小问题，由于构建的dog对象实例在不同的user对象中都使用了，导致触发了alibaba fastJson的循环调用监测，生成的json中会多一个$ref字段，可以设置SerializerFeature.DisableCircularReferenceDetect来禁止循环引用检测。后面其实稍微改了一下，增加了clone方法，确保实例不同
|```12345678910111213141516171819202122232425262728293031323334353637383940414243444546```|```public class DemoRepository {    //userId ,users    public static Map<String, ToolUser> users = Maps.newHashMap();    //userId ,dogs    public static Map<String, List<ToolDog>> dogs = Maps.newHashMap();    //input user    public static Map<String, InputToolUser> inputUsers = Maps.newHashMap();    public DemoRepository() throws InstantiationException, IllegalAccessException {        ToolDog da = new ToolDog("dog-1","da");        ToolDog db = new ToolDog("dog-2","db");        ToolDog dc = new ToolDog("dog-3","dc");        List<ToolDog> dogList = Lists.newArrayList(da,db,dc);        ToolUser ua = new ToolUser("user-1","ua",10,Arrays.asList(da,db));        ToolUser ub = new ToolUser("user-2","ub",20,Arrays.asList(db));        ToolUser uc = new ToolUser("user-3","uc",30,Arrays.asList(db,dc));        //dogs = dogList.stream().collect(Collectors.toMap(ToolDog::getId, Function.identity()));        users = Stream.of(ua,ub,uc).collect(Collectors.toMap(ToolUser::getId, Function.identity()));        dogs = Stream.of(ua,ub,uc).collect(Collectors.toMap(ToolUser::getId, ToolUser::getDogs));        List<InputToolDog> inputToolDogs = convert(InputToolDog.class,dogList.toArray(new ToolDog[3]));        InputToolUser iua = new InputToolUser("input-user-1","ua",10,Arrays.asList(inputToolDogs.get(0).clone(),inputToolDogs.get(1).clone()));        InputToolUser iub = new InputToolUser("input-user-2","ub",20,Arrays.asList(inputToolDogs.get(1).clone()));        InputToolUser iuc = new InputToolUser("input-user-3","uc",30,Arrays.asList(inputToolDogs.get(1).clone(),inputToolDogs.get(2).clone()));        //dogs = dogList.stream().collect(Collectors.toMap(ToolDog::getId, Function.identity()));        inputUsers = Stream.of(iua,iub,iuc).collect(Collectors.toMap(InputToolUser::getId, Function.identity()));    }    public static <T,R> List<R> convert(Class<R> clazz,T... source) throws IllegalAccessException, InstantiationException {        List<R> result = Lists.newArrayList();        if(source != null && source.length > 0){            for(T a : source){                R r = clazz.newInstance();                BeanUtils.copyProperties(a, r);                result.add(r);            }        }        return result;    }}```|
MutationVariableHandler.getListVariablesMapFromString：由于这次是List的入参，所以对应的Jackson的Reference也做了相应的变化：
|```123456789```|```private static TypeReference<List<HashMap<String, Object>>> typeRefReadJsonList = new TypeReference<List<HashMap<String, Object>>>() {    };public static List<HashMap<String, Object>> getListVariablesMapFromString(String variablesFromRequest) {        try {            return jacksonObjectMapper.readValue(variablesFromRequest, typeRefReadJsonList);        } catch (IOException exception) {            throw new GraphQLException("Cannot parse variables", exception);        }    }```|
总结：怎么说呢，工具类有不少优势，把整体的项目结构与Schema的关系梳理的更加清晰，用实现类的方式把我们从为Field定义DataFetcher解脱了出来，只用根据名称就可以进行互绑；但另一方面，因为GraphQL-java还在不断升级迭代，版本已经远远超过了工具包中自带的基础jar包版本，所以还是慎用吧。。。。，不然以后的迁移和兼容也有的搞了
