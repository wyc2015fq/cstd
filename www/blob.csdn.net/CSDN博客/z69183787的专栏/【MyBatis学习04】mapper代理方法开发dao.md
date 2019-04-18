# 【MyBatis学习04】mapper代理方法开发dao - z69183787的专栏 - CSDN博客
2017年07月25日 11:27:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：255
个人分类：[Mybatis-学习](https://blog.csdn.net/z69183787/article/category/7026142)

上一篇博文总结了mybatis使用 原始dao的方法存在的一些弊端，我们肯定不会去用它，那么mybatis中该如何开发dao呢？如题所述，这篇博文主要来总结一下使用mapper代理的方法来开发dao的步骤。 
使用mapper代理的方法来开发dao时，程序员只需要干两件事即可：
> - 需要编写mapper.xml映射文件
- 需要编写mapper接口（相当于dao接口）
从做的工作来看，使用mybatis中使用mapper代理来开发dao会很方便，完全不需要我们去写具体的实现类，只需要写出接口即可，但是接口不能直接拿来用啊，那么我该如何产生它的实现类对象呢？这在下文会给出答案。 
所谓的mapper.xml映射文件，内容其实是跟前面的User.xml文件是一样的，主要是跟定义一些跟User这个pojo之间的映射相关的东西，唯一不同的地方就在于namespace的赋值。在前面的User.xml文件中，我们设定了namespace为”test”，然后在[Java](http://lib.csdn.net/base/java)方法调用的时候，我们会调用类似于
```
sqlSession.insert("test.insertUser",
 user);
```
的方法，来定位需要执行的sql语句。但是在mapper.xml映射文件中，namespace要设定为我们接下来写的mapper接口的地址，即完全限定名。假设我们新建一个mapper包，在里面新建一个UserMapper.xml，如下： 
![mapper.xml](https://img-blog.csdn.net/20160607085022082)
定义好了mapper.xml映射文件后，接下来就要编写mapper接口了，编写mapper接口要遵循以下四个开发规范：
> - 在mapper.xml中，使namespace等于mapper接口的地址（完全限定名）
- mapper.java接口中的方法名和mapper.xml中statement的id一致
- mapper.java接口中方法的输入参数类型和mapper.xml中statement的parameterType指定的类型一致
- mapper.java接口中方法返回值类型和mapper.xml中statement的resultType指定的类型一致
根据这四条开发规范，我们来完成mapper接口：UserMapper.java
```java
//mapper接口，相当于dao接口
public interface UserMapper {
    //根据id查询用户信息
    public User findUserById(int id) throws Exception;
    //根据用户名模糊查询
    public List<User> findUserByName(String name) throws Exception;
    //添加用户信息
    public void insertUser(User user) throws Exception;
    //删除用户信息
    public void deleteUser(int id) throws Exception;
    //更新用户信息
    public void updateUser(User user) throws Exception;
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
这里要注意一点就是findUserByName的方法，返回的是一个装有User的List，但是UserMapper.xml中对应的resultType类型是User，这里要注意，在前面的博文中也提到了，resultType指定的是单个返回结果的类型，也就是一条记录的类型，即User，但是这个findUserByName是返回很多User，所以返回值是`List<User>`。mybatis会自动根据返回值类型去调用不同的方法，如下：
> 
如果mapper方法返回单个pojo对象（非集合对象），代理对象内部通过selectOne来查询[数据库](http://lib.csdn.net/base/mysql)
如果mapper方法返回一个非集合对象，代理对象内部通过selectList来查询数据库
所以完全不用担心上面这个问题，mybatis已经帮我们解决好了。到这里还没完，还有一步就是别忘了在全局配置文件SqlMapConfig.xml中配置刚刚的UserMapper.xml，如下： 
![配置](https://img-blog.csdn.net/20160607090320795)
到现在为止，使用mapper代理的方式开发dao步骤基本上就搞定了。下面就开始写[测试](http://lib.csdn.net/base/softwaretest)程序了。但是还遗留一个问题，就是文章开头提到的：现在只有mapper接口啊，那么你是如何产生实现类的对象呢？我们在测试程序中来看：
```java
private SqlSessionFactory sqlSessionFactory;
    @Before //创建sqlSessionFactory
    public void setUp() throws Exception {
        String resource = "SqlMapConfig.xml"; //mybatis配置文件 
        //得到配置文件的流
        InputStream inputStream = Resources.getResourceAsStream(resource);
        //创建会话工厂SqlSessionFactory,要传入mybaits的配置文件的流
        sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);      
    }
    @Test
    public void testFindUserById() throws Exception {
        SqlSession sqlSession = sqlSessionFactory.openSession();
        //创建UserMapper对象，mybatis自动生成mapper代理对象
        UserMapper userMapper = sqlSession.getMapper(UserMapper.class);
        User user = userMapper.findUserById(1);
        System.out.println(user);
    }
    @Test
    public void testFindUserByName() throws Exception {
        SqlSession sqlSession = sqlSessionFactory.openSession();
        //创建UserMapper对象，mybatis自动生成mapper代理对象
        UserMapper userMapper = sqlSession.getMapper(UserMapper.class);
        List<User> list = userMapper.findUserByName("倪升武");
        sqlSession.close();
        System.out.println(list);
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
原来这个sqlSession可以自动创建一个mapper接口的代理对象！我们只需要把刚刚写好的mapper接口类的字节码对象传给getMapper方法，即可得到一个该接口对应的代理对象，然后我们就可以使用这个代理对象来操作接口中具体的方法了。 
　　到这里，使用mapper代理的方式开发dao就总结完了，但是有个小细节，由于mapper接口中方法的参数要根据映射文件中的parameterType来指定，而parameterType只有一个，所以mapper接口中所有方法的参数都只有一个！那如果我们要传入两个或多个参数该咋整？这没办法，想要传多个参数还是死了这条心了吧，但是可以解决这个问题，就是对传入的对象进行增强，让传进去的对象包含我们需要的参数即可。这算是个小弊端吧，但是不会影响我们开发。
　　相关阅读：[http://blog.csdn.net/column/details/smybatis.html](http://blog.csdn.net/column/details/smybatis.html)
　　学习笔记源码下载地址：[https://github.com/eson15/MyBatis_Study](https://github.com/eson15/MyBatis_Study)
