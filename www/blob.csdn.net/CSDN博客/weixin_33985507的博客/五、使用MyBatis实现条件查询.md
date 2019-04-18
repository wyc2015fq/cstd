# 五、使用MyBatis实现条件查询 - weixin_33985507的博客 - CSDN博客
2018年08月25日 09:37:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
# 一、本课目标
- 掌握SQL映射文件的常用元素
- 掌握使用select完成（单/多）条件查询
![8886052-3aea420d5ecda1ac.png](https://upload-images.jianshu.io/upload_images/8886052-3aea420d5ecda1ac.png)
image.png
简介：
1、mapper，是映射文件的根节点。里面只有一个属性namespace，即命名空间，通过namespace来区分不同的mapper来做到全局唯一。并且通过namespace还可以绑定相应的DAO接口来实现面向接口编程。
# 二、mapper
![8886052-3f07ba413bdf0d79.png](https://upload-images.jianshu.io/upload_images/8886052-3f07ba413bdf0d79.png)
image.png
有了Mapper。xml文件之后，就不用写Mapper接口的具体实现类了，mybatis会通过接口的完全限定名去找到对应的Mapper里面的sql语句去执行。也就是说namaspace的命名必须跟某个接口同名，此时接口中的方法与映射文件中sql语句id一一对应。
## namespace
namespace的属性值`cn.smbms.dao.user.UserMapper`到底是java文件的名字还是xml文件的名字？
因为我们之前的目录是以下这样的：
![8886052-6c7ffcfa03bad39b.png](https://upload-images.jianshu.io/upload_images/8886052-6c7ffcfa03bad39b.png)
image.png
可以进行如下的测试：
新建一个mapper包，将UserMapper.xml放进来：
![8886052-0a80c57477815085.png](https://upload-images.jianshu.io/upload_images/8886052-0a80c57477815085.png)
image.png
同时修改改xml文件中的namespace属性值：
![8886052-d53aeac257884288.png](https://upload-images.jianshu.io/upload_images/8886052-d53aeac257884288.png)
image.png
则对于单元测试的第一种方式来说：
![8886052-4b862bcd857692f8.png](https://upload-images.jianshu.io/upload_images/8886052-4b862bcd857692f8.png)
image.png
只要将后面的字符串改为跟namespace的值相同就可以成功测试。
而对于第二种使用接口的测试方式来说则会报错，这时如果将mapper包下面的UserMapper.xml文件的namespace属性值改为：
`cn.smbms.dao.user.UserMapper`,则可以测试成功，说明当使用接口的时候，程序会去找namespace的值为接口的完全限定名的mapper标签。
**综上，在mybatis-config.xml文件中引入UserMapper.xml文件之后，当使用select的方式去执行操作的时候，只需要把namespace的值写进后面的字符串参数即可；当使用接口的方式的时候，程序会去找接口的完全限定名，只要当Mapper.xml中的namespace的值设置的跟接口的完全限定名即路径一致的时候才能执行。**也就是上面的两句话，xml文件中的namespace的值必须跟某个接口的绝对路径相同，同时xml文件中的sql语句的id必须跟接口中的方法同名。
**所以，可以把namespace的值写为java文件的名字，这样不管使用什么方法，总不会有问题。**
**习惯上都会把mapper、.xml文件跟你的mapper接口放在一个包下面，并且在MyBatis里面，我们的DAO接口一般不叫作UserDao，而叫做UserMapper**
# 三、select元素
![8886052-2a5def2677843c91.png](https://upload-images.jianshu.io/upload_images/8886052-2a5def2677843c91.png)
image.png
## 3、1单参数查询
在UserMapper.xml文件中增加查询：
```
<!-- 根据用户名称查询用户列表（模糊查询） -->
    <select id="getUserListByUserName" parameterType="string" resultType="User">
        select * from smbms_user where userName like CONCAT('%',#{userName},'%')
    </select>
```
分析：1、参数类型是string，这个地方对大小写不敏感。参数类型也可以是自己定义的实体类类型。
2、sql语句中拿到参数的方法是#{参数名}，这个参数名跟测试类中传进来的参数名必须一样。
3、数据库字段名必须跟实体类字段名保持一致，这样才能把查询到的数据跟实体类对象一一匹配。
在UserMapper.java中增加方法：
```
public List<User> getUserListByUserName(String userName);
```
3、构建测试类：
```
@Test
    public void testGetUserListByUserName() {
        List<User> userList = null;
        SqlSession sqlSession = null;
        String userName = "赵";
        try {
            sqlSession = MyBatisUtil.createSqlSession();
            userList = sqlSession.getMapper(UserMapper.class).getUserListByUserName(userName);
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            MyBatisUtil.closeSqlSession(sqlSession);
        }
        for (User user:userList) {
            logger.debug("testGetUserList userCode:" + user.getUserCode()
                    + "and userName" + user.getUserName());
        }
    }
```
## 3.2多条件查询
问题：按条件查询用户表，若多条件情况先如何处理？
![8886052-7ba8bd983323f7ff.png](https://upload-images.jianshu.io/upload_images/8886052-7ba8bd983323f7ff.png)
image.png
分析：
1、传入多个参数进行入参？
2、封装成user对象进行入参？
![8886052-e675bfaf2c31e765.png](https://upload-images.jianshu.io/upload_images/8886052-e675bfaf2c31e765.png)
image.png
#### 使用实体类入参
示例代码：
改造UserMapper.xml文件：
```
<!-- 查询用户列表 -->
    <select id="getUserList" parameterType="user" resultType="User">
        select * from smbms_user 
        where userName like CONCAT('%',#{userName},'%')
        and userRole=#{userRole}
    </select>
```
改造接口：
```
public List<User> getUserList(User user);
```
改造测试类；
```
@Test
    public void testGetUserList() {
        List<User> userList = null;
        SqlSession sqlSession = null;
        User user1 = new User();
        user1.setUserName("赵");
        user1.setUserRole(2);
        try {
            sqlSession = MyBatisUtil.createSqlSession();
        // 4、调用mapper文件来对数据进行操作,操作之前必须将mapper文件引入到mabatis-config.xml中
        //  userList = sqlSession.selectList("mmp.UserMapper.getUserList");
            userList = sqlSession.getMapper(UserMapper.class).getUserList(user1);
            
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            MyBatisUtil.closeSqlSession(sqlSession);
        }
        for (User user:userList) {
            logger.debug("testGetUserList userCode:" + user.getUserCode()
                    + "and userName" + user.getUserName());
        }
    }
```
最终的运行结果正常。
#### 使用Map入参
在UserMapper.xml文件中增加：
```
<select id="getUserListByMap" parameterType="Map" resultType="User">
        select * from smbms_user 
        where userName like CONCAT('%',#{uName},'%')
        and userRole=#{uRole}
    </select>
```
在接口中增加方法；
```
public List<User> getUserListByMap(Map<String, String> userMap);
```
实体类构建如下：
```
@Test
    public void testGetUserListByMap() {
        List<User> userList = null;
        SqlSession sqlSession = null;
        Map<String, String> userMap = new HashMap<String, String>();
        userMap.put("uName", "赵");
        userMap.put("uRole", "2");
        try {
            sqlSession = MyBatisUtil.createSqlSession();
            userList = sqlSession.getMapper(UserMapper.class).getUserListByMap(userMap);
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            MyBatisUtil.closeSqlSession(sqlSession);
        }
        for (User user:userList) {
            logger.debug("testGetUserList userCode:" + user.getUserCode()
                    + "and userName" + user.getUserName());
        }
    }
```
单元测试正常。
## 3.3小结
![8886052-c679ca1dac78d420.png](https://upload-images.jianshu.io/upload_images/8886052-c679ca1dac78d420.png)
image.png
# 四、总结
![8886052-20f399c8d920b949.png](https://upload-images.jianshu.io/upload_images/8886052-20f399c8d920b949.png)
image.png
所以mybatis的核心就是通过核心对象sqlSession的getMapper方法获得要找的sql语句的namespace+id，然后在核心配置文件引入的xml文件中去找namespace+id对应的sql语句然后执行并返回结果。如果不使用接口的话，则直接通过sqlSession的select等方法，把sql语句的namespace+id座位参数穿进去，然后mybatis会直接去找这个sql语句。
