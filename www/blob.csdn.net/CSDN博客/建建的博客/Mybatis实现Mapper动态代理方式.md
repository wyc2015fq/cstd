# Mybatis实现Mapper动态代理方式 - 建建的博客 - CSDN博客
2017年10月16日 11:34:37[纪建](https://me.csdn.net/u013898698)阅读数：90
## 一、实现原理
        Mapper接口开发方法只需要程序员编写Mapper接口（相当于Dao接口），由Mybatis框架根据接口定义创建接口的动态代理对象，代理对象的方法体同上边Dao接口实现类方法。
        Mapper接口开发需要遵循以下规范：
                1、Mapper.xml文件中的namespace与mapper接口的类路径相同。
                2、  Mapper接口方法名和Mapper.xml中定义的每个statement的id相同 
                3、Mapper接口方法的输入参数类型和mapper.xml中定义的每个sql 的parameterType的类型相同
                4、Mapper接口方法的输出参数类型和mapper.xml中定义的每个sql的resultType的类型相同
## 二、Mapper.xml映射文件
         定义mapper映射文件UserMapper.xml（内容同Users.xml），需要修改namespace的值为 UserMapper接口路径。将UserMapper.xml放在classpath 下mapper目录 下。
**[html]**[view
 plain](http://blog.csdn.net/lu930124/article/details/50991899#)[copy](http://blog.csdn.net/lu930124/article/details/50991899#)
[print](http://blog.csdn.net/lu930124/article/details/50991899#)[?](http://blog.csdn.net/lu930124/article/details/50991899#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE mapper  
- PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"  
- "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
- <mappernamespace="cn.itcast.mybatis.mapper.UserMapper">
- <!-- 根据id获取用户信息 -->
- <selectid="findUserById"parameterType="int"resultType="cn.itcast.mybatis.po.User">
-         select * from user where id = #{id}  
- </select>
- <!-- 自定义条件查询用户列表 -->
- <selectid="findUserByUsername"parameterType="java.lang.String"
- resultType="cn.itcast.mybatis.po.User">
-        select * from user where username like '%${value}%'   
- </select>
- <!-- 添加用户 -->
- <insertid="insertUser"parameterType="cn.itcast.mybatis.po.User">
- <selectKeykeyProperty="id"order="AFTER"resultType="java.lang.Integer">
-         select LAST_INSERT_ID()   
- </selectKey>
-       insert into user(username,birthday,sex,address)   
-       values(#{username},#{birthday},#{sex},#{address})  
- </insert>
- 
- </mapper>
## 三、Mapper.java（接口文件）
**[java]**[view
 plain](http://blog.csdn.net/lu930124/article/details/50991899#)[copy](http://blog.csdn.net/lu930124/article/details/50991899#)
[print](http://blog.csdn.net/lu930124/article/details/50991899#)[?](http://blog.csdn.net/lu930124/article/details/50991899#)
- /**
-  * 用户管理mapper
-  */
- Public interface UserMapper {  
- //根据用户id查询用户信息
- public User findUserById(int id) throws Exception;  
- //查询用户列表
- public List<User> findUserByUsername(String username) throws Exception;  
- //添加用户信息
- publicvoid insertUser(User user)throws Exception;   
- }  
         接口定义有如下特点：
                  1、Mapper接口方法名和Mapper.xml中定义的statement的id相同
                  2、Mapper接口方法的输入参数类型和mapper.xml中定义的statement的parameterType的类型相同
                  3、Mapper接口方法的输出参数类型和mapper.xml中定义的statement的resultType的类型相同
## 四、加载UserMapper.xml文件
         修改sqlMapConfig.xml文件：
**[html]**[view
 plain](http://blog.csdn.net/lu930124/article/details/50991899#)[copy](http://blog.csdn.net/lu930124/article/details/50991899#)
[print](http://blog.csdn.net/lu930124/article/details/50991899#)[?](http://blog.csdn.net/lu930124/article/details/50991899#)
- <!-- 加载映射文件 -->
- <mappers>
- <mapperresource="mapper/UserMapper.xml"/>
- </mappers>
## 五、测试
**[java]**[view
 plain](http://blog.csdn.net/lu930124/article/details/50991899#)[copy](http://blog.csdn.net/lu930124/article/details/50991899#)
[print](http://blog.csdn.net/lu930124/article/details/50991899#)[?](http://blog.csdn.net/lu930124/article/details/50991899#)
- Public class UserMapperTest extends TestCase {  
- 
- private SqlSessionFactory sqlSessionFactory;  
- 
- protectedvoid setUp() throws Exception {  
- //mybatis配置文件
-         String resource = "sqlMapConfig.xml";  
-         InputStream inputStream = Resources.getResourceAsStream(resource);  
- //使用SqlSessionFactoryBuilder创建sessionFactory
-         sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);  
-     }  
- 
- 
-     Public void testFindUserById() throws Exception {  
- //获取session
-         SqlSession session = sqlSessionFactory.openSession();  
- //获取mapper接口的代理对象
-         UserMapper userMapper = session.getMapper(UserMapper.class);  
- //调用代理对象方法
-         User user = userMapper.findUserById(1);  
-         System.out.println(user);  
- //关闭session
-         session.close();  
- 
-     }  
- @Test
- publicvoid testFindUserByUsername() throws Exception {  
-         SqlSession sqlSession = sqlSessionFactory.openSession();  
-         UserMapper userMapper = sqlSession.getMapper(UserMapper.class);  
-         List<User> list = userMapper.findUserByUsername("张");  
-         System.out.println(list.size());  
- 
-     }  
- Public void testInsertUser() throws Exception {  
- //获取session
-         SqlSession session = sqlSessionFactory.openSession();  
- //获取mapper接口的代理对象
-         UserMapper userMapper = session.getMapper(UserMapper.class);  
- //要添加的数据
-         User user = new User();  
-         user.setUsername("张三");  
-         user.setBirthday(new Date());  
-         user.setSex("1");  
-         user.setAddress("北京市");  
- //通过mapper接口添加用户
-         userMapper.insertUser(user);  
- //提交
-         session.commit();  
- //关闭session
-         session.close();  
-     }  
- 
- 
- }  
## 六、总结
         selectOne和selectList
         动态代理对象调用sqlSession.selectOne()和sqlSession.selectList()是根据mapper接口方法的返回值决定，如果返回list则调用selectList方法，如果返回单个对象则调用selectOne方法。
         namespace
         mybatis官方推荐使用mapper代理方法开发mapper接口，程序员不用编写mapper接口实现类，使用mapper代理方法时，输入参数可以使用pojo包装对象或map对象，保证dao的通用性。
