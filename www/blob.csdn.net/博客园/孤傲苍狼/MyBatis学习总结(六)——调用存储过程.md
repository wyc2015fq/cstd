# MyBatis学习总结(六)——调用存储过程 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [MyBatis学习总结(六)——调用存储过程](https://www.cnblogs.com/xdp-gacl/p/4270352.html)



## 一、提出需求

查询得到男性或女性的数量, 如果传入的是0就女性否则是男性

## 二、准备数据库表和存储过程

```
1 create table p_user(  
 2     id int primary key auto_increment,  
 3     name varchar(10),
 4     sex char(2)
 5 ); 
 6 
 7 insert into p_user(name,sex) values('A',"男");  
 8 insert into p_user(name,sex) values('B',"女");  
 9 insert into p_user(name,sex) values('C',"男");  
10 
11 -- 创建存储过程(查询得到男性或女性的数量, 如果传入的是0就女性否则是男性)
12 DELIMITER $
13 CREATE PROCEDURE mybatis.ges_user_count(IN sex_id INT, OUT user_count INT)
14 BEGIN  
15 IF sex_id=0 THEN
16 SELECT COUNT(*) FROM mybatis.p_user WHERE p_user.sex='女' INTO user_count;
17 ELSE
18 SELECT COUNT(*) FROM mybatis.p_user WHERE p_user.sex='男' INTO user_count;
19 END IF;
20 END 
21 $
22 
23 -- 调用存储过程
24 DELIMITER ;
25 SET @user_count = 0;
26 CALL mybatis.ges_user_count(1, @user_count);
27 SELECT @user_count;
```

## 三、编辑userMapper.xml

　　编辑userMapper.xml文件，添加如下的配置项

```
1 <!-- 
 2         查询得到男性或女性的数量, 如果传入的是0就女性否则是男性
 3      -->
 4     <select id="getUserCount" parameterMap="getUserCountMap" statementType="CALLABLE">
 5         CALL mybatis.ges_user_count(?,?)
 6     </select>
 7     
 8     <!--
 9         parameterMap.put("sexid", 0);
10         parameterMap.put("usercount", -1);
11      -->
12     <parameterMap type="java.util.Map" id="getUserCountMap">
13         <parameter property="sexid" mode="IN" jdbcType="INTEGER"/>
14         <parameter property="usercount" mode="OUT" jdbcType="INTEGER"/>
15     </parameterMap>
```

## 四、编写单元测试代码

```
1 package me.gacl.test;
 2 
 3 import java.util.HashMap;
 4 import java.util.List;
 5 import java.util.Map;
 6 
 7 import me.gacl.custom.model.ConditionUser;
 8 import me.gacl.domain.User;
 9 import me.gacl.util.MyBatisUtil;
10 import org.apache.ibatis.session.SqlSession;
11 import org.junit.Test;
12 
13 /**
14  * @author gacl
15  * 测试调用存储过程
16  */
17 public class Test6 {
18     
19     @Test
20     public void testGetUserCount(){
21         SqlSession sqlSession = MyBatisUtil.getSqlSession();
22         /**
23          * 映射sql的标识字符串，
24          * me.gacl.mapping.userMapper是userMapper.xml文件中mapper标签的namespace属性的值，
25          * getUserCount是select标签的id属性值，通过select标签的id属性值就可以找到要执行的SQL
26          */
27         String statement = "me.gacl.mapping.userMapper.getUserCount";//映射sql的标识字符串
28         Map<String, Integer> parameterMap = new HashMap<String, Integer>();
29         parameterMap.put("sexid", 1);
30         parameterMap.put("usercount", -1);
31         sqlSession.selectOne(statement, parameterMap);
32         Integer result = parameterMap.get("usercount");
33         System.out.println(result);
34         sqlSession.close();
35     }
36 }
```











