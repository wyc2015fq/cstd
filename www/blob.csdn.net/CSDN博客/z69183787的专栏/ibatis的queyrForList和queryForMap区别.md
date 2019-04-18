# ibatis的queyrForList和queryForMap区别 - z69183787的专栏 - CSDN博客
2015年08月08日 18:09:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4976
**一.说明**
        iBatis的返回值参数类型也有种：resultMap与resultClass，这两种类型的选择可以用两句话说明之：
        1.当结果集列名和类的属性名完全相对应的时候，则可直接用resultClass直接指定查询结果类型。
        2.当查询结果集与属性名对应不上的时候，就可以采用resultMap指定列名与对象属性名之间的对应关系，否则对应不上的属性将为null或0。
        resultMap映射结果的目的就是要将查询的结果集绑定到映射对象的属性上。其实对于这种情况用resultClass也是可以解决的，就是直接在Sql语句中采用as重命名的方式将结果集中与对象属性名不相同的字段用as重命名。
**二.实例**
        对于resultMap和resultClass有个很有趣的例子：
        iBatis的查询方法中有这样一个查询方法：
        暂且不说这个方法是什么，假使我们有这样的需求，数据库中很多条数据，有一个字段假使是name，每一条数据映射我们一个实体对象，我们的需求是将所有的数据查询出来，根据不同的name属性值要做不同的处理。
        首先想到的肯定是先将所有的数据查询出来而不是根据不同的name重复查询多次，将所有的数据查询出来也有两种方式，第一种我们常用的List<?>集合的形式，另一种Map<?,?>的形式。
        就这个需求而言显然是直接查询出Map<?,?>比较好一点，这样我们就可以直接在程序中根据已知的name找到对应的对象，而不必循环遍历。
        这样的需求可能不多，这儿就就事论事一回吧。
        第一个方法：
Java代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- public java.util.Map queryForMap(java.lang.String id,  
-                                  java.lang.Object parameterObject,  
-                                  java.lang.String keyProp)throws java.sql.SQLException  
        该方法有三个参数，第一个是statement，第二个是查询入参，第三个是返回的Map集合的key属性。
Java代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- public Map<String, User> getUserMap2() throws SQLException  
- {  
- return sqlMapClient.queryForMap("User.queryForMap2", null, "userID");  
- }  
        Dao里面有这样两个方法，第一个作用就是查询出所有的User对象，以User对象的userID为key存放到Map集合中返回。
        xml映射文件：
Xml代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- <selectid="queryForMap2"resultClass="java.util.HashMap">//注意此处不能写java.util.Map，必须是具体的实现类，否则iBatis无法实例化  
-   select * from t_user  
- </select>
        测试代码：
Java代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- Map<String, User> user = dao.getUserMap2();  
- System.out.println(user);  
        查询Map结果：
Text代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- {null={AGE=28, USERID=6, USERNAME=zhangbo}}//输出结果十分诡异，数据库里面有六条数据，为什么只查询出来最后一条，而且key还是null  
        测试代码2：
Java代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- Map<String, User> user = dao.getUserMap2();  
- System.out.println(user);  
- User user2 = user.get(null);  
        尝试从Map中获取key为null的User对象，抛类型转换异常，即Map里面的根本不是User对象。
        关于查询出来的数据只有最后一条数可以这样理解的：
         iBatis的queryForMap()方法，其实也是先根据条件查询出一个List集合，然后遍历List集合，将对象以Key，Value的形式存入Map中返回。
        其实List集合确实查询了数据库满足条件的6条数据，只是这6条数据指定的key都为null。遍历的时候先将List中的第一条以key为null的形式存入Map中，然后又将第二条再以key为null放入Map中，显而易见，key相同，相当于替换了第一条，以此类推，Map中始终只能只有最后一条数据了。至于key为什么是null，一时我说不上来。有待研究！肯定一个根本的原因就是使用了resultClass，而HashMap又没有“userID”属性？？？
        那么我们不妨来试一下使用一下resultMap，并给HashMap指定属性：
Xml代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- <resultMapclass="java.util.HashMap"id="resultMap-UserMap2">
- <resultproperty="userID"column="userID"/>
- <resultproperty="username"column="username"/>
- <resultproperty="userage"column="age"/>
- </resultMap>
- <selectid="queryForMap2"parameterClass="java.lang.String"resultMap="resultMap-UserMap2">
-   select * from t_user  
- </select>
Java代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- public Map<String, User> getUserMap2() throws SQLException  
- {  
- return sqlMapClient.queryForMap("User.queryForMap2", null, "userID");  
- }  
        这样一来，我们就指定了数据库字段与实体对象之间的映射，先来看一下执行结果：
Text代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- {  
- 3={userID=3, username=zhangbo, userage=25},  
- 5={userID=5, username=zhangbo, userage=27},   
- 2={userID=2, username=zhangbo, userage=24},   
- 4={userID=4, username=zhangbo, userage=26},   
- 6={userID=6, username=zhangbo, userage=28},   
- 1={userID=1, username=zhangbo, userage=23}  
- }  
        输出结果似乎是正确的，有两个疑问，为什么顺序是乱的？value真的是Map<String, User>中的User对象？  
Java代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- Map<String, User> user = dao.getUserMap2();  
- System.out.println(user);  
- User user2 = user.get("3");//抛类型转换异常，即Map里面的根本不是User对象
        尝试将iBatis给我们的Map<Object, Object>集合中的Object转化成User失败，那么iBatis返回的Map中的Object  为什么不是User对象？
        既然我们不知道Map中的Object究竟是什么玩意，我们不妨这样来，将Dao方法的返回值改成Map<Object, Object>,再运行一下。
Java代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- Map<Object, Object> map = dao.getUserMap2();  
- System.out.println(map.getClass().getName());//这个肯定是一个java.util.HashMap类型
- System.out.println(map.get("1").getClass().getName());//这个居然也是java.util.HashMap类型
        以上结果表明，查询结果是Map中放了一个我们指定的"userID"为key，一个HashMap为value的HashMap集合。
        如果我们想返回Map<Object, Map<>>形式，这里不是必须写成"userID"，它只是一个形式参数，只要和映射文件中的resultMap中的某个property属性相同即可，当然这个属性对应的数据库字段值就将被当成key。
Xml代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- <resultMapclass="java.util.HashMap"id="resultMap-UserMap2">
- <resultproperty="xxx"column="userID"/>
- <resultproperty="name"column="username"/>
- <resultproperty="age"column="age"/>
- </resultMap>
        如果我们指定的key在数据库中由重复怎么办？例如：
Xml代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- <resultMapclass="java.util.HashMap"id="resultMap-UserMap2">
- <resultproperty="xxx"column="userID"/>
- <resultproperty="ooo"column="username"/>
- <resultproperty="age"column="age"/>
- </resultMap>
        return sqlMapClient.queryForMap("User.queryForMap2", null, "ooo");//将数据库username字段值作为key
  不出我们所料，结果：
        {zhangbo={age=28, xxx=6, ooo=zhangbo}}，至于为什么只有一条记录而且是最后一条，不再累述。可见我们写在resultMap映射文件中的“property”便是我们Map<Object, Map<>>中，里面个Map的key值。
        这样一来iBatis就很灵活了，对应一张数据库表，我想用iBatis进行增删该查，特别是查，我们不是非得要有需要一个对象与表结果对应，我们可以没有一个对象对应这张表，依然可以将表中的数据查询出来封装到Map中供我们很方便的使用。
        那么按照我们的初衷，将一个对象放到Map中，以这个对象的某个属性为key，这个对象为value放到Map中返回是不是也可以实现了，答案非常肯定，可以，稍作修改：
        完整的映射文件：
Xml代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- <resultMapclass="com.ibatis.entity.User"id="resultMap-UserMap2">
- <resultproperty="userID"column="userID"/>//指定User的每个属性和数据库字段的映射关系  
- <resultproperty="name"column="username"/>
- <resultproperty="age"column="age"/>
- </resultMap>
- <selectid="queryForMap2"parameterClass="java.lang.String"resultMap="resultMap-UserMap2">
-   select * from t_user  
- </select>
        完整的Dao代码：
Java代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- @SuppressWarnings("unchecked")  
- public Map<String, User> getUserMap2() throws SQLException  
- {  
- 
- return sqlMapClient.queryForMap("User.queryForMap2", null, "userID");//这里指定的key的名称应该是User的某个属性名称
- }  
        完整的测试代码：
Java代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- Map<String, User> user = dao.getUserMap2();  
- System.out.println(user);  
- System.out.println(user.get("1").getName());  
       完整的输出结果：
Text代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- {  
- 3=User: age=25 &userID=3 &name=zhangbo ,   
- 5=User: age=27 &userID=5 &name=zhangbo ,   
- 2=User: age=24 &userID=2 &name=zhangbo ,   
- 4=User: age=26 &userID=4 &name=zhangbo ,   
- 6=User: age=28 &userID=6 &name=zhangbo ,   
- 1=User: age=23 &userID=1 &name=zhangbo   
- }  
- zhangbo  
        所以想要达到这种效果，我们的resultMap中的resultClass应该是具体的对象，而不是我们想当然的java.util.HashMap。
**三.queryForMap方法的重写方法**
         还有一个queryForMap方法的重写方法
Java代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- public java.util.Map queryForMap(java.lang.String id,  
-                                  java.lang.Object parameterObject,  
-                                  java.lang.String keyProp,  
-                                  java.lang.String valueProp)  
        这个方法不但指定了我们返回的Map中的key， 而且也指定了Map中的value。通过这个方法我们就可以查询一个表中的特定的两个字段值：一个字段的值作为key，一个字段的值作为value。
        举个例子：
        完整的Dao代码：
Java代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- @SuppressWarnings("unchecked")  
- public Map<String, String> getUserMap3(String userID) throws SQLException  
- {  
- //同样，“xxx”， “ooo”只是形参，任意字符串
- return sqlMapClient.queryForMap("User.queryForMap3", userID, "xxx","ooo");//查询指定userID的一条数据，至于哪个字段是key，哪个字段是value，只有resultMap映射字段能控制。
- }  
        resultMap映射文件：
Xml代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- <resultMapclass="java.util.HashMap"id="resultMap-UserMap3">
- 
- <resultproperty="xxx"column="username"/>//指明xxx, ooo 对应的数据库字段名称，到底是数据库的哪个字段值作为key，哪个作为value。  
- <resultproperty="ooo"column="age"/>
- </resultMap>
- <selectid="queryForMap3"parameterClass="java.lang.String"resultMap="resultMap-UserMap3">
-   select * from t_user where userID=#aaa# //"aaa"也是一个形式参数，任意字符串  
- </select>
        测试代码：
Java代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- Map<String, String> map = dao.getUserMap3("3");  
- System.out.println(map);  
        输出结果：
Text代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- {zhangbo=25}  
        有一个地方值得注意：
Xml代码  ![收藏代码](http://bijian1013.iteye.com/images/icon_star.png)
- <selectid="queryForMap3"parameterClass="java.lang.String"resultClass="java.util.HashMap">//这里直接写成HashMap可以吗？  
-   select * from t_user where userID=#aaa#   
- </select>
        return sqlMapClient.queryForMap("User.queryForMap3", userID, "xxx","ooo");//假设"xxx", "ooo"就是数据库两个字段的名称，因为我们知道，当我们的对象属性名称和数据字段相同的情况下，我们是直接写resultClass="xxx.xxx.xxx"即可的，只有当我们数据库的字段名称与对象的属性不同的情况下我们才需要一个resultMap将数据库字段与对象属性名称对应起来。
         即使这样，以上的假设是不可以的，这样的情况必须是resultMap来指定数据库字段对应HashMap的key和value, 其实也挺好理解的：
         1.如果直接指定成resultClass="java.util.HashMap", 那么HashMap没有一个叫"xxx", "ooo"的属性，所以iBatis无法跟一般的对象一样将数据库字段自动映射到对象相同名称的属性上。
         2.此处的resultMap不是指定数据库哪个字段对应对象哪个属性，而是指定数据库的哪个字段作为key，哪个字段作为value。那么到底哪个作为key，哪个作为value是在我们调用queryForMap的方法时候指定的。
**四.总结**
         1.针对第一个只指定key的queryForMap方法：
        如果resultMap中依然设定resultClass="java.util.HashMap"，iBatis的处理方式就是将数据库指定的key值作为key，再将所有的字段封装成HashMap作为返回Map的value。将所有字段封转成HashMap的时候，以resultMap中我们指定的名称作为key，数据库对应字段值作为value的。
        如果设定resultClass="具体对象"，那么iBatis就会将数据库字段值设置到该对象的指定属性上作为Value，Key依然是指定的该对象的一个属性。
        2.针对第二个指定key，value的queryForMap方法：
        就是查询数据库的任意两个字段值，一个作为返回Map的key，一个作为返回Map的value，至于哪个作为key，哪个作为value，由查询方法和resultMap一起决定。
        从这两个方法看iBatis的resultclass和resultMap还是非常的灵活和有用的，尽管queryForMap方法在具体的工作中可能用的不是很多，但是有这两个方法的存在，iBatis显得更加的强大了，对数据库的操作不一定非要与某个我们的对象模型关联了，用java的HashMap就可以搞定。从这一点来看，HashMap和一个pojo对象有惊人的相似点。
文章来源：[http://blog.163.com/girl_lihuiyue@126/blog/static/18069621201421311922531/](http://blog.163.com/girl_lihuiyue@126/blog/static/18069621201421311922531/)
