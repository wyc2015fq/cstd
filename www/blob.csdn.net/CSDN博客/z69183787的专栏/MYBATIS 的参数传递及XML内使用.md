# MYBATIS 的参数传递及XML内使用 - z69183787的专栏 - CSDN博客
2016年06月05日 13:58:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6852
Parameter
1.   传入简单类型
    JAVA代码:
Java代码  ![收藏代码](http://zhuyuehua.iteye.com/images/icon_star.png)
- public User get(Long id) {    
- return (User) getSqlSession().selectOne("com.liulanghan.get" , id);  
- }  
 MAPPER :
Xml代码  ![收藏代码](http://zhuyuehua.iteye.com/images/icon_star.png)
- <selectid="findUserListByIdList"parameterType="java.lang.Long"resultType="User">
-         select * from user where  id = #{id};  
- </select>
2.   传入List
    JAVA代码:
Java代码  ![收藏代码](http://zhuyuehua.iteye.com/images/icon_star.png)
- public List<Area> findUserListByIdList(List<Long> idList) {  
- return getSqlSession().selectList("com.liulanghan.findUserListByIdList", idList);  
-     }  
MAPPER :
Xml代码  ![收藏代码](http://zhuyuehua.iteye.com/images/icon_star.png)
- <selectid="findUserListByIdList"parameterType="java.util.ArrayList"resultType="User">
-     select * from user user  
- <where>
-         user.ID in (  
- <foreachitem="guard"index="index"collection="list"
- separator=","> #{guard} </foreach>
-         )  
- </where>
- </select>
 单独传入list时，foreach中的collection必须是list，不不管变量的具体名称是什么。比如这里变量名为idList，
 collection却是是list。 
3.  传入数组
  JAVA代码:
Java代码  ![收藏代码](http://zhuyuehua.iteye.com/images/icon_star.png)
- public List<Area> findUserListByIdList(int[] ids) {  
- return getSqlSession().selectList("com.liulanghan.findUserListByIdList", ids);  
-     }  
 MAPPER :
Xml代码  ![收藏代码](http://zhuyuehua.iteye.com/images/icon_star.png)
- <selectid="findUserListByIdList"parameterType="java.util.HashList"resultType="User">
-     select * from user user  
- <where>
-         user.ID in (  
- <foreachitem="guard"index="index"collection="array"
- separator=","> #{guard} </foreach>
-         )  
- </where>
- </select>
 单独传入数组时，foreach中的collection必须是array，不不管变量的具体名称是什么。比如这里变量名为ids，
 collection却是是array
4.  传入map
 JAVA代码:
Java代码  ![收藏代码](http://zhuyuehua.iteye.com/images/icon_star.png)
- publicboolean exists(Map<String, Object> map){  
-         Object count = getSqlSession().selectOne("com.liulanghan.exists", map);  
- int totalCount = Integer.parseInt(count.toString());  
- return totalCount > 0 ? true : false;  
-     }  
 MAPPER :
Xml代码  ![收藏代码](http://zhuyuehua.iteye.com/images/icon_star.png)
- <selectid="exists"parameterType="java.util.HashMap"resultType="java.lang.Integer">
-         SELECT COUNT(*) FROM USER user  
- <where>
- <iftest="code != null">
-                 and user.CODE = #{code}   
- </if>
- <iftest="id != null">
-                 and user.ID = #{id}   
- </if>
- <iftest="idList !=null ">
-                 and user.ID in (  
- <foreachitem="guard"index="index"collection="idList"
- separator=","> #{guard} </foreach>
-                 )  
- </if>
- </where>
- </select>
 MAP中有list或array时，foreach中的collection必须是具体list或array的变量名。比如这里MAP含有一个
    名为idList的list，所以MAP中用idList取值，这点和单独传list或array时不太一样。
5. 传入JAVA对象
 JAVA代码:
Java代码  ![收藏代码](http://zhuyuehua.iteye.com/images/icon_star.png)
- publicboolean findUserListByDTO(UserDTO userDTO){  
-         Object count = getSqlSession().selectOne("com.liulanghan.exists", userDTO);  
- int totalCount = Integer.parseInt(count.toString());  
- return totalCount > 0 ? true : false;  
-     }  
 MAPPER :
Xml代码  ![收藏代码](http://zhuyuehua.iteye.com/images/icon_star.png)
- <selectid="findUserListByDTO"parameterType="UserDTO"resultType="java.lang.Integer">
-         SELECT COUNT(*) FROM USER user  
- <where>
- <iftest="code != null">
-                 and user.CODE = #{code}   
- </if>
- <iftest="id != null">
-                 and user.ID = #{id}   
- </if>
- <iftest="idList !=null ">
-                 and user.ID in (  
- <foreachitem="guard"index="index"collection="idList"
- separator=","> #{guard} </foreach>
-                 )  
- </if>
- </where>
- </select>
JAVA对象中有list或array时，foreach中的collection必须是具体list或array的变量名。比如这里UserDTO含有一个
    名为idList的list，所以UserDTO中用idList取值，这点和单独传list或array时不太一样。
6.取值
 由上面可以看出，取值的时候用的是#{}。它具体的意思是告诉MyBatis创建一个预处理语句参数。
 使用JDBC，这样的一个参数在SQL中会由一个“?”来标识，并被传递到一个新的预处理语句中，就像这样：
Java代码  ![收藏代码](http://zhuyuehua.iteye.com/images/icon_star.png)
- // Similar JDBC code, NOT MyBatis…
- String selectPerson = “SELECT * FROM PERSON WHERE ID=?”;  
- PreparedStatement ps = conn.prepareStatement(selectPerson);  
- ps.setInt(1,id);  
    可以看到这个写法比较简单，MyBatis为我们做了很多默认的事情，具体的写法应该如下：
Xml代码  ![收藏代码](http://zhuyuehua.iteye.com/images/icon_star.png)
- #{property,javaType=int,jdbcType=NUMERIC,typeHandler=MyTypeHandler,mode=OUT,resultMap=User}  
 property:属性名，即代码传入的变量名。
 javaType：该字段在JAVA中的类型，比如int。
 jdbcType：该字段在JDBC中的类型，比如NUMERIC。
 typeHandler:类型处理器
 mode：参数类型为IN，OUT或INOUT参数
 resultMap：结果。
 还好,MyBatis比较体谅我们，一般我们只需写一个属性名即可，如#{id},其他的如javaType和typeHandlerMybatis
 会自动帮我们填好。可是这样有时也会出问题，比如出现CLOB字段时。
 由于JAVA代码中的String类型对应的默认typeHandler为StringTypeHandler,当用String类型处理时，如果String长度超过一定长度，就会报如下错误：
 setString can only process strings of less than 32766 chararacters
 解决办法是指定该属性的typeHandler，如下：
 #{message,typeHandler=org.apache.ibatis.type.ClobTypeHandler}
 我们也可以自定义typeHandler来处理需要的数据，具体这里详述。
 JDBC类型是仅仅需要对插入，更新和删除操作可能为空的列进行处理。这是JDBC的需要，而不是MyBatis的。一般不需要配置
 mode、resultMap一般不需要，在写存储过程时会用到，这里不详述。
7.字符串替换
 一般情况下，我们采用#{}取值，产生预处理语句，但是有时我们可能不希望Mybatis来帮我们预处理，比如ORDER BY时，可以
 采用如下写法：
 ORDER BY ${columnName}
 这里MyBatis不会修改或转义字符串。而是直接拼接到SQL字符串后面。
 重要：接受从用户输出的内容并提供给语句中不变的字符串，这样做是不安全的。这会导致潜在的SQL注入攻击，因此你
 不应该允许用户输入这些字段，或者通常自行转义并检查。
