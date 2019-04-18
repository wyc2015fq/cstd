# graphql学习（四）GraphQL和SpringMVC 的整合 - z69183787的专栏 - CSDN博客
2018年04月18日 16:34:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：995
[https://blog.csdn.net/q15150676766/article/details/74618398](https://blog.csdn.net/q15150676766/article/details/74618398)
graphql学习（一）graphql-java文档初步阅读：[https://blog.csdn.net/q15150676766/article/details/74231398](https://blog.csdn.net/q15150676766/article/details/74231398)
经过四天的奋战，终于把GraphQL和SpringMVC整合在了一起。这次主要采用的SpringMVC，和hibernate框架。一些和项目相关的简单的配置这里不做过多的介绍。话不多说，直接上代码吧。
定义了两个实体类 
User.java
```java
@Entity
@Table(name = "info")
public class Info {
    private int id;
    private String truename;
    private String address;
    private User user;
    public Info(int id, String truename, String address, User user) {
        super();
        this.id = id;
        this.truename = truename;
        this.address = address;
        this.user = user;
    }
    public Info() {
    }
    @Override
    public String toString() {
        return "Info [id=" + id + ", truename=" + truename + ", address=" + address + ", user=" + user + "]";
    }
    @Id
    public int getId() {
        return id;
    }
    public void setId(int id) {
        this.id = id;
    }
    public String getTruename() {
        return truename;
    }
    public void setTruename(String truename) {
        this.truename = truename;
    }
    public String getAddress() {
        return address;
    }
    public void setAddress(String address) {
        this.address = address;
    }
    @OneToOne
    @JoinColumn(name = "user")
    public User getUser() {
        return user;
    }
    public void setUser(User user) {
        this.user = user;
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
User.java
```java
@Entity
@Table(name = "user")
public class User {
    private int id;
    private String username;
    private String password;
    public User(int id, String username, String password) {
        this.id = id;
        this.username = username;
        this.password = password;
    }
    public User() {
    }
    @Override
    public String toString() {
        return "User [id=" + id + ", username=" + username + ", password=" + password  + "]";
    }
    @Id
    public int getId() {
        return id;
    }
    public void setId(int id) {
        this.id = id;
    }
    public String getUsername() {
        return username;
    }
    public void setUsername(String username) {
        this.username = username;
    }
    public String getPassword() {
        return password;
    }
    public void setPassword(String password) {
        this.password = password;
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
然后从数据库中将info数据取出，当时方法名取错了，不要在意这些细节
```java
public Info getUserInfo(int id) {
        Session session = hibernateTemplate.getSessionFactory().getCurrentSession();
        String hql = "from Info i where i.id = ?";
        Query query = session.createQuery(hql).setParameter(0, id);
        Info info = (Info) query.uniqueResult();
        System.out.println(info);
        return info;
    }
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
以上的这些部分都是实现基本上的在SpringMVC框架上的是实现最基本的获取数据库数据，不做过多的介绍，接下来是本次的重头戏： 
新建一个雷，专门用来生成每个实体对应的类型的GraphQLObjectType，这样每次就可以直接来这个类取，也增加了代码的复用性，后来去graphql-java中查看了一下源代码，它应该也是这样做的：
```java
public class GraphQLObject {
    public static GraphQLObjectType user = newObject().name("user")
            .field(newFieldDefinition().name("id").type(Scalars.GraphQLInt).build())// 类型一定要与结构类字段的类型相对应
            .field(newFieldDefinition().name("username").type(Scalars.GraphQLString).build())
            .field(newFieldDefinition().name("password").type(Scalars.GraphQLString).build()).build();
    public static GraphQLObjectType info = newObject().name("info")
            .field(newFieldDefinition().name("id").type(Scalars.GraphQLInt).build())// 类型一定要与结构类字段的类型相对应
            .field(newFieldDefinition().name("truename").type(Scalars.GraphQLString).build())
            .field(newFieldDefinition().name("address").type(Scalars.GraphQLString).build())
            .field(newFieldDefinition().name("user").type(user).build()).build();
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
然后在查询对应的方法中生成对应的搜索域和执行搜索就可以了
```java
@Override
    public Map<String, Object> getUserInfo(String ghql) {
        GraphQLFieldDefinition findforid = GraphQLFieldDefinition.newFieldDefinition().name("info")
                .argument(// 这里用GraphQL些查询语句时传入的参数
                        newArgument().name("id")// 参数名为id
                                .type(Scalars.GraphQLInt)// 参数类型
                                .build())
                .type(GraphQLObject.info)// 绑定GraphQL的一个结构，就是上面的那段代码
                .dataFetcher(environment -> {
                    // 获取查询参数
                    int id = environment.getArgument("id");
                    return userDAO.getUserInfo(id);
                }).build();
        GraphQLSchema schema = GraphQLSchema.newSchema().query(newObject().name("_123").field(findforid).build())
                .build();
        // "{info(id:1){id,truename,address,user{id,username,password}}}";
        ExecutionResult re = new GraphQL(schema).execute(ghql);
        Map<String, Object> result = (Map<String, Object>) re.getData();
        System.out.println(result);
        return result;
    }
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
最后将map转化为就送数据就可以了； 
我在前端的文本框中输入
`"{info(id:1){id,truename,address,user{id,username,password}}}";`- 1
就可以得到一下的结果：
```
{
    "info":{
        "id":1,
        "truename":"123",
        "address":"123",
        "user":{
            "id":1,
            "username":"123",
            "password":"123"
        }
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
最后简单说一下心得体会，这种东西对后端还是比较友好的，毕竟根据一定的条件，就可以返回前端所需要的数据，对后端的维护什么的还是比较方便的 
但是呢，查询条件对应的接口还是都要有的，本文是以ID为查找条件，若改为以truename为查询条件则需要在写一个借口。 
而且这种做法个人感觉有点不太安全，相当于是把后台的数据，设置是数据库的数据直接暴露在前台，感觉不是很安全的样子。 
而且，由于一开始不知道前端会需要查询结果有哪些，所以在查询的时候只能将与该信息相关的所有信息都顺带查询出来供前端选择，这样对数据库的压力还是很大的，在想有没有什么办法可以将数据查询出来之后就只接保存在内存或者服务器中，下次在需要就重新调用就好了，可是感觉这样的话就好像没有什么要数据库的必要了，感觉这个问题还是有必要好好研究或者解决的。
顺便提一下，今天下午再整这东西的时候，发现graphql-java在5个小时之前还稍稍更新了一下
