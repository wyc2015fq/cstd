# MyBatis学习07】动态sql - z69183787的专栏 - CSDN博客
2017年07月25日 11:37:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：247
个人分类：[Mybatis-学习](https://blog.csdn.net/z69183787/article/category/7026142)
## 1. 动态sql
动态sql是mybatis中的一个核心，什么是动态sql？动态sql即对sql语句进行灵活操作，通过表达式进行判断，对sql进行灵活拼接、组装。就拿上一篇博文中对用户的综合查询一例来说：
`select * from user where user.sex = #{user.sex} and user.username like '%${user.username}%'`- 1
- 1
假如这个user是null咋整？或者user.sex或者user.username为null呢？所以更严谨的做法应该是在执行这个语句之前要先进行判断才对，确保都不为空，那么我再去查询。这就涉及到了mybatis中的动态sql了。 
在mybatis中，动态sql可以使用标签来表示，这很类似于jstl表达式，我们可以将上面的sql语句改成动态sql，如下：
```xml
<select id="findUserList" parameterType="mybatis.po.UserQueryVo" resultType="mybatis.po.User">
    select * from user
    <!-- where可以自动去掉条件中的第一个and -->
    <where>
        <if test="user!=null">
            <if test="user.sex!=null and user.sex!=''">
                and user.sex = #{user.sex}
            </if>
            <if test="user.username!=null and user.username!=''">
                and user.username like '%${user.username}%'
            </if>
        </if>
    </where>
</select>
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
上面的代码很好理解，主要就是加了一些判断，条件不为空，才进行查询条件的拼接，让mybatis动态的去执行。那么在[测试](http://lib.csdn.net/base/softwaretest)代码中，我们可以故意的将user.sex不赋初值，就可以看到查询的结果是不一样的。
## 2. sql片段
那么现在还有个问题，如果好几个statement都需要这样做，而且动态sql部分都一样，这就会导致一些代码的重复，所以如果遇到这种情况，我们就应该抽取，动态sql也可以抽取，我们可以将动态的这部分sql抽取成sql片段，然后在具体的statement中引用进来即可。如下：
```xml
<sql id="query_user_where">
    <if test="user!=null">
        <if test="user.sex!=null and user.sex!=''">
            and user.sex = #{user.sex}
        </if>
        <if test="user.username!=null and user.username!=''">
            and user.username like '%${user.username}%'
        </if>
    </if>
</sql>
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
id是给该sql片段起个名字而已，内部就是上面的where动态部分，然后我们将上面原来的动态部分改成对这个sql片段的引用，如下：
```xml
<select id="findUserList" parameterType="mybatis.po.UserQueryVo" resultType="mybatis.po.User">
    select * from user
    <where>
        <!-- 引用sql片段的id，如果refid指定的id不在本mapper文件中，需要在前面加上namespace -->
        <include refid="query_user_where"></include>
        <!-- 还可以引用其他sql片段 -->
    </where>
</select>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
## 3. foreach
还有个问题：如果我们要向sql传递数组或List该咋整呢？mybatis使用的是foreach解析。为了模拟这个场景，我们将上面的查询改成多个id查询，有两种查询方式：
```
SELECT * FROM USER WHERE id=1 OR id=12 OR id=17
SELECT * FROM USER WHERE id IN(1,12,17)
```
- 1
- 2
- 1
- 2
首先有一点很明确，既然要使用多个id进行查询，那么多个id肯定要作为参数传进来，所以存储多个id的List需要放到UserQueryVo中作为一个属性，这点很好理解，所以我们先在UserQueryVo中增加这个属性：
```java
//传入多个id
private List<Integer> ids;
```
- 1
- 2
- 1
- 2
然后我们修改UserMapper.xml中的sql片段（还是写在sql片段中），如下：
```xml
<sql id="query_user_where">
    <if test="user!=null">
        <if test="user.sex!=null and user.sex!=''">
            and user.sex = #{user.sex}
        </if>
        <if test="user.username!=null and user.username!=''">
            and user.username like '%${user.username}%'
        </if>
    </if>
    <if test="ids!=null">
        <!-- 使用右边的sql拼接：AND (id=1 OR id=12 OR id=17) -->
        <foreach collection="ids" item="user_id" open="AND (" close=")" separator="OR">
            id=#{user_id}
        </foreach>
    </if>
</sql>
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
下面简单介绍一下这个foreach中相关属性的作用：
> 
collection：指定输入对象中的集合属性，这里就是这个ids。 
item：表示每个遍历生成的对象，自己起个名儿，在foreach体中使用。 
open：开始遍历时拼接的sql串。 
close：结束遍历时拼接的sql串。 
separator：遍历的两个对象中需要拼接的sql串。
我们测试一下，然后看下控制台打印出来的sql就很容易理解了。测试程序：
```java
@Test
public void testFindUserList() throws Exception {
    SqlSession sqlSession = sqlSessionFactory.openSession();
    //创建UserMapper对象，mybatis自动生成mapper代理对象
    UserMapper userMapper = sqlSession.getMapper(UserMapper.class);
    //创建包装对象，设置查询条件
    UserQueryVo userQueryVo = new UserQueryVo();
    User user = new User();
    //由于这里使用动态sql，如果不设置某个值，条件不会拼接在sql中
    user.setSex("男");
    user.setUsername("倪升武");
    //传入多个id
    List<Integer> ids = new ArrayList<Integer>();
    ids.add(1);
    ids.add(12);
    ids.add(17);
    userQueryVo.setIds(ids);
    userQueryVo.setUser(user);  
    //调用userMapper的方法
    List<User> list = userMapper.findUserList(userQueryVo);
    System.out.println(list);
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
看下控制台打印出的sql：
`select * from user WHERE user.sex = ? and user.username like '%倪升武%' AND ( id=? OR id=? OR id=? ) `- 1
- 1
【注意一个细节：在mybatis中，如果输入的是Integer或者int类型的0，上面那个if判断标签返回的是false，也就是说，即使非空非”，也不会拼接标签体中的sql。（感谢A_Dream1在评论中给我提出了这个问题）】 
**mybatis中，当username为数字0时，<if test="user.username!=null and user.username!=''">的返回结果是false，解释动态语句的时候应该着重说明一下**
所以mybatis自动的将多个id拼接到了sql中。那么另外一个sql的实现就不再赘述了，跟上面的一样，唯一不同的就是sql片段部分，如下：
```xml
<!-- 使用右边的sql拼接：AND id IN(1,12,17) -->
<foreach collection="ids" item="user_id" open="AND id IN(" close=")" separator=",">
    #{user_id}
</foreach>
```
- 1
- 2
- 3
- 4
- 1
- 2
- 3
- 4
mybatis中的动态sql就总结这么多吧~ 
