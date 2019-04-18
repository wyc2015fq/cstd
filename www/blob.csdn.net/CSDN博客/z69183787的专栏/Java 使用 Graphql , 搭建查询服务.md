# Java 使用 Graphql , 搭建查询服务 - z69183787的专栏 - CSDN博客
2018年04月20日 15:23:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：373
[https://blog.csdn.net/qq362228416/article/details/50854980](https://blog.csdn.net/qq362228416/article/details/50854980)
#### 背景
随着react的开源，facebook相继开源了很多相关的项目，这些项目在他们内部已经使用了多年，其中引起我注意的就是本次讨论的是graphql，目前官方只有nodejs版，由于很多公司的后台技术栈都是java，所以便有了graphql的java版实现，在[github](https://github.com/andimarek/graphql-java)上可以找到，废话不多说，直接看代码吧，具体介绍还是去看官网吧，不然就跑题了。
#### GraphQLSchema
Schema相当于一个数据库，它有很多GraphQLFieldDefinition组成，Field相当于数据库表/视图，每个表/视图又由名称、查询参数、数据结构、数据组成.
1） 先定义一个数据结构（GraphQLOutputType）字段，然后定义一个初始化方法
```
private GraphQLOutputType userType;
private void initOutputType() {
      /**
       * 会员对象结构
       */
      userType = newObject()
              .name("User")
              .field(newFieldDefinition().name("id").type(GraphQLInt).build())
              .field(newFieldDefinition().name("age").type(GraphQLInt).build())
              .field(newFieldDefinition().name("sex").type(GraphQLInt).build())
              .field(newFieldDefinition().name("name").type(GraphQLString).build())
              .field(newFieldDefinition().name("pic").type(GraphQLString).build())
              .build();
}
```
2）再定义两个表/视图，它包括名称，查询参数，数据结构，以及数据检索器
```java
/**
     * 查询单个用户信息
     * @return
     */
    private GraphQLFieldDefinition createUserField() {
        return GraphQLFieldDefinition.newFieldDefinition()
                .name("user")
                .argument(newArgument().name("id").type(GraphQLInt).build())
                .type(userType)
                .dataFetcher(environment -> {
                    // 获取查询参数
                    int id = environment.getArgument("id");
                    // 执行查询, 这里随便用一些测试数据来说明问题
                    User user = new User();
                    user.setId(id);
                    user.setAge(id + 15);
                    user.setSex(id % 2);
                    user.setName("Name_" + id);
                    user.setPic("pic_" + id + ".jpg");
                    return user;
                })
                .build();
    }
    /**
     * 查询多个会员信息
     * @return
     */
    private GraphQLFieldDefinition createUsersField() {
        return GraphQLFieldDefinition.newFieldDefinition()
                .name("users")
                .argument(newArgument().name("page").type(GraphQLInt).build())
                .argument(newArgument().name("size").type(GraphQLInt).build())
                .argument(newArgument().name("name").type(GraphQLString).build())
                .type(new GraphQLList(userType))
                .dataFetcher(environment -> {
                    // 获取查询参数
                    int page = environment.getArgument("page");
                    int size = environment.getArgument("size");
                    String name = environment.getArgument("name");
                    // 执行查询, 这里随便用一些测试数据来说明问题
                    List<User> list = new ArrayList<>(size);
                    for (int i = 0; i < size; i++) {
                        User user = new User();
                        user.setId(i);
                        user.setAge(i + 15);
                        user.setSex(i % 2);
                        user.setName(name + "_" + page + "_" + i);
                        user.setPic("pic_" + i + ".jpg");
                        list.add(user);
                    }
                    return list;
                })
                .build();
    }
```
3）接着定义一个Schema，并将其初始化，它包含一个名称，以及一个或多个表/视图（Field）
```
private GraphQLSchema schema;
    public GraphSchema() {
        initOutputType();
        schema = GraphQLSchema.newSchema().query(newObject()
                .name("GraphQuery")
                .field(createUsersField())
                .field(createUserField())
                .build()).build();
    }
```
4）完成以上步骤之后，还需要定义一个model，类名不限，但是结构需要满足前面定义的数据结构，而且必须是public的
```
public class User {
    private int id;
    private int age;
    private int sex;
    private String name;
    private String pic;
    // getter, setter...
}
```
5）之后写一个main方法，来测试一下
```java
public static void main(String[] args) {
        GraphQLSchema schema = new GraphSchema().getSchema();
        String query1 = "{users(page:2,size:5,name:\"john\") {id,sex,name,pic}}";
        String query2 = "{user(id:6) {id,sex,name,pic}}";
        String query3 = "{user(id:6) {id,sex,name,pic},users(page:2,size:5,name:\"john\") {id,sex,name,pic}}";
        Map<String, Object> result1 = (Map<String, Object>) new GraphQL(schema).execute(query1).getData();
        Map<String, Object> result2 = (Map<String, Object>) new GraphQL(schema).execute(query2).getData();
        Map<String, Object> result3 = (Map<String, Object>) new GraphQL(schema).execute(query3).getData();
        // 查询用户列表
        System.out.println(result1);
        // 查询单个用户
        System.out.println(result2);
        // 单个用户、跟用户列表一起查
        System.out.println(result3);
}
```
输出：
```
{users=[{id=0, sex=0, name=john_2_0, pic=pic_0.jpg}, {id=1, sex=1, name=john_2_1, pic=pic_1.jpg}, {id=2, sex=0, name=john_2_2, pic=pic_2.jpg}, {id=3, sex=1, name=john_2_3, pic=pic_3.jpg}, {id=4, sex=0, name=john_2_4, pic=pic_4.jpg}]}
{user={id=6, sex=0, name=Name_6, pic=pic_6.jpg}}
{user={id=6, sex=0, name=Name_6, pic=pic_6.jpg}, users=[{id=0, sex=0, name=john_2_0, pic=pic_0.jpg}, {id=1, sex=1, name=john_2_1, pic=pic_1.jpg}, {id=2, sex=0, name=john_2_2, pic=pic_2.jpg}, {id=3, sex=1, name=john_2_3, pic=pic_3.jpg}, {id=4, sex=0, name=john_2_4, pic=pic_4.jpg}]}
```
6）最后把main方法里面的代码放到web层，只需要定义一个query参数，很容易就把查询服务搭建好了，dataFetcher 里面还是调用原来的查询接口
7）引入maven依赖
```xml
<dependency>
    <groupId>com.graphql-java</groupId>
    <artifactId>graphql-java</artifactId>
    <version>2.0.0</version>
</dependency>
```
关于graphql查询什么定义，看看这个或许对你有帮助
json
```
{
    id=6, 
    sex=0, 
    name="Name_6", 
    pic="pic_6.jpg"
}
```
query
```
{
    id,
    sex,
    name,
    pic
}
```
后面那部分，其实就是json字符串，去掉=和value的结果，还是可读的
### 结语
graphql 带了一种全新的思维方式，可以简化web api的开发，由客户端指定需要什么数据，服务端返回什么数据，减少不必要的流量传输，对移动端友好，还提供多种数据聚合查询，多个查询只是用一个请求，既满足api最小粒度，又满足前端需要，减少请求，提高性能。
