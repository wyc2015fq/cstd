# GraphQL- Java实战入门 - z69183787的专栏 - CSDN博客
2018年04月18日 16:40:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3248

[http://www.zhaiqianfeng.com/2017/06/learn-graphql-action-by-java.html](http://www.zhaiqianfeng.com/2017/06/learn-graphql-action-by-java.html)
本篇主要使用[graphql-java](http://graphql-java.readthedocs.io/)来演示如何使用Java来构建GraphQL API，文中使了Gradle作为构建工具，但未使用其特性，因此你也可以使用Maven，只是简单添加依赖即可。主要涉及到枚举(enum)、输入类型(input)、参数(argument)、接口(interface)、联合(union)等服务端，以及客户端查询测试。本例代码存放在 [https://github.com/zhaiqianfeng/GraphQL-Demo/tree/master/java](https://github.com/zhaiqianfeng/GraphQL-Demo/tree/master/java)
![GraphQL](http://www.zhaiqianfeng.com/uploads/content/share/graphql.png)
#### [](http://www.zhaiqianfeng.com/2017/06/learn-graphql-action-by-java.html#%E6%B7%BB%E5%8A%A0%E4%BE%9D%E8%B5%96)添加依赖
gradle
compile 'com.graphql-java:graphql-java:3.0.0'
maven
<dependency>
    <groupId>com.graphql-java</groupId>
    <artifactId>graphql-java</artifactId>
    <version>3.0.0</version>
</dependency>
本文示例使用的构建工具是Gradle。
#### [](http://www.zhaiqianfeng.com/2017/06/learn-graphql-action-by-java.html#%E7%A4%BA%E4%BE%8B%E4%BB%A3%E7%A0%81%E7%BB%93%E6%9E%84)示例代码结构
示例结构目录如下
├── advance　高级示例
│   ├── GraphQL_argument.java 传入参数示例
│   ├── GraphQL_interface.java　接口示例
│   └── GraphQL_union.java　联合示例
├── get_start 入门示例
│   └── GraphQL_Simple.java 简单入门示例
└── model　Java实体
    ├── Dog4Interface.java 为接口示例准备的Dog实现
    ├── Dog4Union.java 为联合示例准备的Dog
    ├── Fish4Interface.java 为接口示例准备的Fish实现
    ├── Fish4Union.java 为联合示例准备的Fish
    ├── IAnimal.java 为接口示例准备的Animal接口
    └── User.java 为示例准备的User
每个示例基本遵循流程：**提供带查询的示例数据-> 定义GrapQL数据类型 -> 定义暴露给客户端的query api和mutaion api -> 创建GraphQL Schema -> 测试输出**。
#### [](http://www.zhaiqianfeng.com/2017/06/learn-graphql-action-by-java.html#%E7%AE%80%E5%8D%95%E7%A4%BA%E4%BE%8B)简单示例
示例代码如下
// com.zqf.get_start.GraphQL_Simple
//服务端示例数据
User user=new User();
user.setName("zhaiqianfeng");
user.setSex("男");
user.setIntro("博主，专注于Linux,Java,nodeJs,Web前端:Html5,JavaScript,CSS3");
//定义GraphQL类型
GraphQLObjectType userType = newObject()
        .name("User")
        .field(newFieldDefinition().name("name").type(GraphQLString))
        .field(newFieldDefinition().name("sex").type(GraphQLString))
        .field(newFieldDefinition().name("intro").type(GraphQLString))
        .build();
//定义暴露给客户端的查询query api
GraphQLObjectType queryType = newObject()
        .name("userQuery")
        .field(newFieldDefinition().type(userType).name("user").staticValue(user))
        .build();
//创建Schema
GraphQLSchema schema = GraphQLSchema.newSchema()
        .query(queryType)
        .build();
//测试输出
GraphQL graphQL = GraphQL.newGraphQL(schema).build();
Map<String, Object> result = graphQL.execute("{user{name,sex,intro}}").getData();
System.out.println(result);
执行输出
{user={name=zhaiqianfeng, sex=男, intro=博主，专注于Linux,Java,nodeJs,Web前端:Html5,JavaScript,CSS3}}
#### [](http://www.zhaiqianfeng.com/2017/06/learn-graphql-action-by-java.html#%E5%8F%82%E6%95%B0-argument-%E7%A4%BA%E4%BE%8B)参数(argument)示例
参数示例代码比较多，请参考[github上的源码:GraphQL_argument.java](https://github.com/zhaiqianfeng/GraphQL-Demo/blob/master/java/src/main/java/com/zqf/advance/GraphQL_argument.java)，做如下几点说明。
##### [](http://www.zhaiqianfeng.com/2017/06/learn-graphql-action-by-java.html#%E5%AE%9A%E4%B9%89%E5%BD%A2%E5%8F%82)定义形参
传输参数是通过argument方法传入并指定类型，如
.argument(newArgument()
        .name("id")
        .type(new GraphQLNonNull(GraphQLInt)))
##### [](http://www.zhaiqianfeng.com/2017/06/learn-graphql-action-by-java.html#%E6%8E%A5%E6%94%B6%E5%8F%82%E6%95%B0)接收参数
获取参数使用DataFetchingEnvironment，如
int id=environment.getArgument("id");
获取参数有如下两个重构
Map<String, Object> getArguments();
<T> T getArgument(String name);
但当我使用后者把传入的userInfo转换为User时，异常
java.util.LinkedHashMap cannot be cast to com.zqf.model.User
因此不得不如下蹩脚书写
Map<String,Object> userInfoMap=environment.getArgument("userInfo");
User userInfo=new User();
for (String key : userInfoMap.keySet()){
    switch (key){
        case "name":
            userInfo.setName(userInfoMap.get("name").toString());
            break;
        case "sex":
            userInfo.setSex(userInfoMap.get("sex").toString());
            break;
        case "intro":
            userInfo.setIntro(userInfoMap.get("intro").toString());
            break;
        case "skills":
            ArrayList<String> skillsList=(ArrayList<String>) userInfoMap.get("skills");
            String[] skills=new String[skillsList.size()];
            userInfo.setSkills(skillsList.toArray(skills));
            break;
    }
}
##### [](http://www.zhaiqianfeng.com/2017/06/learn-graphql-action-by-java.html#Resolver)Resolver
解析数据使用的是DataFetcher，如
.dataFetcher(new DataFetcher() {
    @Override
    public Object get(DataFetchingEnvironment environment) {
            int id=environment.getArgument("id");
            return users.get(id);
    }
}))
为了简洁，你可以使用lambda表达式
.dataFetcher(environment -> {
    int id=environment.getArgument("id");
    return users.get(id);
}))
#### [](http://www.zhaiqianfeng.com/2017/06/learn-graphql-action-by-java.html#%E8%81%94%E5%90%88-union-%E7%A4%BA%E4%BE%8B)联合(union)示例
这部分没说明好说的，如果了解了union的概念代码示例非常清晰，请参考[github上的源码:GraphQL_union.java](https://github.com/zhaiqianfeng/GraphQL-Demo/blob/master/java/src/main/java/com/zqf/advance/GraphQL_union.java)。使用GraphQLUnionType定义union类型，其中possibleType指定可以union的类型，重点是关注typeResolver来做类型转换，但个人觉得这里实现有待改进，对于Java这种强类型语言自动处理比较人性化。
GraphQLUnionType animalUnion = newUnionType()//定义联合类型(union)
    .name("IAnimal")
    .possibleType(dogType)
    .possibleType(fishType)
    .description("动物联合")
    .typeResolver(env -> {
        if(env.getObject() instanceof Dog4Union){
            return dogType;
        }if(env.getObject() instanceof Fish4Union){
            return fishType;
        }
        return  null;
    })
    .build();
#### [](http://www.zhaiqianfeng.com/2017/06/learn-graphql-action-by-java.html#%E6%8E%A5%E5%8F%A3-interface-%E7%A4%BA%E4%BE%8B)接口(interface)示例
graphql-java对GraphQL的接口实现确实使用的Java接口，但个人觉得使用集成或许更合适。定义接口使用的GraphQLInterfaceType关键字，实现接口使用的withInterface关键字。
##### [](http://www.zhaiqianfeng.com/2017/06/learn-graphql-action-by-java.html#%E5%9D%91%EF%BC%9A%E5%AF%B9%E8%B1%A1%E4%B8%BA%E5%AE%9A%E4%B9%89)坑：对象为定义
上面的关键字都不难理解，蛋疼的是把如下的定义放在方法体中却提示为定义
GraphQLInterfaceType animalInterface = newInterface()//定义接口(interface)类型
        .name("IAnimal")
        .description("动物接口")
        .field(newFieldDefinition()
                .name("name")
                .type(GraphQLString))
        .typeResolver(new TypeResolver() {
            @Override
            public GraphQLObjectType getType(TypeResolutionEnvironment env) {
                if (env.getObject() instanceof Dog4Interface) {
                    return dogType; //提示 dogType 未定义
                }
                if (env.getObject() instanceof Fish4Interface) {
                    return fishType; //提示 fishType 未定义
                }
                return  null;
            }
        })
        .build();
GraphQLObjectType dogType = newObject()//定义Dog类型
        .name("Dog4Interface")
        .field(newFieldDefinition().name("name").type(GraphQLString))
        .field(newFieldDefinition().name("legs").type(GraphQLInt))
        .withInterface(animalInterface)
        .build();
GraphQLObjectType fishType = newObject()//定义Fish类型
        .name("Fish4Interface")
        .field(newFieldDefinition().name("name").type(GraphQLString))
        .field(newFieldDefinition().name("tailColor").type(GraphQLString))
        .withInterface(animalInterface)
        .build();
也看到了有开发者提了issue:[Circularity between object and interface type definitions](https://github.com/graphql-java/graphql-java/issues/205)，但依然没看到有效的解决方案，只好顺着作者愿意，把上面的定义作为字段才能解决，具体代码可参考[github上的源码:GraphQL_interface.java](https://github.com/zhaiqianfeng/GraphQL-Demo/blob/master/java/src/main/java/com/zqf/advance/GraphQL_interface.java)
##### [](http://www.zhaiqianfeng.com/2017/06/learn-graphql-action-by-java.html#Schema%E5%8A%A0%E5%85%A5%E9%A2%9D%E5%A4%96%E7%B1%BB%E5%9E%8B)Schema加入额外类型
最后还要提示下，由于查询返回的接口，但实现并没有自动添加到Schema中，因此需要在创建Schema时间，将其手动加入
//额外的GraphQL类型
Set<GraphQLType> types=new HashSet<>();
types.add(dogType);
types.add(fishType);
//创建Schema
GraphQLSchema schema = GraphQLSchema.newSchema()
        .query(queryType)
        .build(types);
#### [](http://www.zhaiqianfeng.com/2017/06/learn-graphql-action-by-java.html#%E7%BB%93%E6%9D%9F%E8%AF%AD)结束语
虽然遇到了许多坑，但最后还是把需要的功能演示出来了，但无论是那种类库，基本的流程一般不会改变：基本遵循流程：**定义GrapQL数据类型 -> 定义暴露给客户端的query api和mutaion api -> 创建GraphQL Schema**。
