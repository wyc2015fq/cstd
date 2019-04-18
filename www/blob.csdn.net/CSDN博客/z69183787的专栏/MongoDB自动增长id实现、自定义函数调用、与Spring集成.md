# MongoDB自动增长id实现、自定义函数调用、与Spring集成 - z69183787的专栏 - CSDN博客
2016年10月26日 18:17:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：882
昨天同事问实现MongoDB主键自动增长有什么好的办法，虽然喜欢MongoDB客户端驱动程序自动生成的id，不过还是来测试了一下，仅仅是测试哦
废话少说![微笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/smile.gif)
**1、创建项目，添加依赖**
**[html]**[view
 plain](http://blog.csdn.net/gp490609578/article/details/50638448#)[copy](http://blog.csdn.net/gp490609578/article/details/50638448#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1575211/fork)
- <dependencies>
- <dependency>
- <groupId>org.springframework.data</groupId>
- <artifactId>spring-data-mongodb</artifactId>
- <version>1.7.0.RELEASE</version>
- </dependency>
- <dependency>
- <groupId>org.springframework</groupId>
- <artifactId>spring-context</artifactId>
- <version>4.1.7.RELEASE</version>
- </dependency>
- <dependency>
- <groupId>org.springframework</groupId>
- <artifactId>spring-test</artifactId>
- <version>4.1.7.RELEASE</version>
- </dependency>
- 
- <dependency>
- <groupId>junit</groupId>
- <artifactId>junit</artifactId>
- <version>4.11</version>
- </dependency>
- 
- <dependency>
- <groupId>org.slf4j</groupId>
- <artifactId>slf4j-api</artifactId>
- </dependency>
- <dependency>
- <groupId>org.slf4j</groupId>
- <artifactId>slf4j-log4j12</artifactId>
- </dependency>
- <dependency>
- <groupId>log4j</groupId>
- <artifactId>log4j</artifactId>
- </dependency>
- </dependencies>
**2、使用了[spring](http://lib.csdn.net/base/javaee)-data-mongo，所以创建封装数据的演示类**
**[java]**[view
 plain](http://blog.csdn.net/gp490609578/article/details/50638448#)[copy](http://blog.csdn.net/gp490609578/article/details/50638448#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1575211/fork)
- publicclass User {  
- 
- privatelong id;  
- 
- private String name;  
- 
- public User(long id, String name) {  
- this.id = id;  
- this.name = name;  
-     }  
- 
- publiclong getId() {  
- return id;  
-     }  
- 
- public String getName() {  
- return name;  
-     }  
- 
- @Override
- public String toString() {  
- return"User{" +  
- "id=" + id +  
- ", name='" + name + '\'' +  
- '}';  
-     }  
- }</span>  
3、创建资源配置文件app.properties
**[plain]**[view
 plain](http://blog.csdn.net/gp490609578/article/details/50638448#)[copy](http://blog.csdn.net/gp490609578/article/details/50638448#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1575211/fork)
- mongodb.host=localhost  
- mongodb.port=27017  
**4、spring配置文件**
**[html]**[view
 plain](http://blog.csdn.net/gp490609578/article/details/50638448#)[copy](http://blog.csdn.net/gp490609578/article/details/50638448#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1575211/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xmlns:context="http://www.springframework.org/schema/context"
- xmlns:mongo="http://www.springframework.org/schema/data/mongo"
- xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd  
-        http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context.xsd  
-        http://www.springframework.org/schema/data/mongo http://www.springframework.org/schema/data/mongo/spring-mongo.xsd">
- 
- <context:property-placeholderlocation="classpath:app.properties"/>
- 
- <mongo:mongo-clientid="mongoClient"host="${mongodb.host}"port="${mongodb.port}"/>
- 
- <mongo:db-factoryid="dbFactory"dbname="script"mongo-ref="mongoClient"/>
- 
- <mongo:templateid="mongoTemplate"db-factory-ref="dbFactory"converter-ref="mappingMongoConverter"/>
- 
- <beanid="dbRefResolver"class="org.springframework.data.mongodb.core.convert.DefaultDbRefResolver">
- <constructor-argref="dbFactory"/>
- </bean>
- 
- <beanid="mongoMappingContext"class="org.springframework.data.mongodb.core.mapping.MongoMappingContext"/>
- 
- <beanid="defaultMongoTypeMapper"class="org.springframework.data.mongodb.core.convert.DefaultMongoTypeMapper">
- <constructor-argname="typeKey">
- <null/>
- </constructor-arg>
- </bean>
- 
- <beanid="mappingMongoConverter"class="org.springframework.data.mongodb.core.convert.MappingMongoConverter">
- <constructor-argname="dbRefResolver"ref="dbRefResolver"/>
- <constructor-argname="mappingContext"ref="mongoMappingContext"/>
- <propertyname="typeMapper"ref="defaultMongoTypeMapper"/>
- </bean>
- 
- </beans>
**5、测试代码**
**[java]**[view
 plain](http://blog.csdn.net/gp490609578/article/details/50638448#)[copy](http://blog.csdn.net/gp490609578/article/details/50638448#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1575211/fork)
- import com.mongodb.BasicDBObject;  
- import com.mongodb.DB;  
- import org.junit.After;  
- import org.junit.Before;  
- import org.junit.Test;  
- import org.junit.runner.RunWith;  
- import org.springframework.beans.factory.annotation.Autowired;  
- import org.springframework.data.mongodb.core.MongoTemplate;  
- import org.springframework.data.mongodb.core.ScriptOperations;  
- import org.springframework.data.mongodb.core.script.ExecutableMongoScript;  
- import org.springframework.data.mongodb.core.script.NamedMongoScript;  
- import org.springframework.test.context.ContextConfiguration;  
- import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;  
- 
- importstatic org.junit.Assert.assertEquals;  
- importstatic org.junit.Assert.assertTrue;  
- importstatic org.springframework.data.mongodb.core.query.Criteria.where;  
- importstatic org.springframework.data.mongodb.core.query.Query.query;  
- importstatic org.springframework.data.mongodb.core.query.Update.update;  
- 
- /**
-  * (1) Test an auto-increment pattern for the _id field.
-  *
-  * (2) Test MongoDB Script and Spring Script Operations.
-  *
-  * Created by gaofu on 16-2-5.
-  */
- @RunWith(SpringJUnit4ClassRunner.class)  
- @ContextConfiguration(locations = "classpath:beans.xml")  
- publicclass AutoIncrementIdAndScriptTest {  
- 
- @Autowired
- private MongoTemplate template;  
- 
- @Before
- publicvoid setUp() throws Exception {  
-         ScriptOperations scriptOps = template.scriptOps();  
- 
- // 将JavaScript函数保存到MongoDB server，以便直接在客户端调用
-         scriptOps.register(new NamedMongoScript("getNextSequence", "function(name){var ret=db.counters.findAndModify({query:{_id:name},update:{$inc:{seq:NumberLong(1)}},new:true});return ret.seq.floatApprox}"));  
- //        scriptOps.register(new NamedMongoScript("getNextSequence", "function(name){var ret=db.counters.findAndModify({query:{_id:name},update:{$inc:{seq:1}},new:true});return ret.seq}")); // 这样就会把seq变成双精度浮点型
- 
-         template.upsert(query(where("_id").is("userid")), update("seq", 0L), "counters");  
-     }  
- 
- 
- @Test
- publicvoid testClientScript() {  
- final String origin = "Hello World";  
-         String script = "function(x){return x + \".\"}";  
- 
-         ExecutableMongoScript mongoScript = new ExecutableMongoScript(script);  
- 
-         ScriptOperations scriptOps = template.scriptOps();  
- 
-         Object result1 = scriptOps.execute(mongoScript, origin);  
- // Spring使用String.format()方法对字符串进行了处理
- //        System.out.println(result1);
-         assertEquals(String.format("'%s'", origin) + '.', result1);  
- 
-         Object mongoEval = template.getDb().eval(script, origin);  
- //        System.out.println(mongoEval);
-         assertEquals(origin + ".", mongoEval);  
- 
-         Object result2 = scriptOps.execute(mongoScript, 3);  
-         assertEquals("3.", result2);  
-     }  
- @Test
- publicvoid testAutoIncrementIdAndStoredScript() {  
-         ScriptOperations scriptOps = template.scriptOps();  
- 
- boolean exists = scriptOps.exists("getNextSequence");  
-         assertTrue(exists);  
- 
- // JavaScript返回的总是双精度浮点型数字,所以需要转换
-         User jack = new User(((Number) scriptOps.call("getNextSequence", "userid")).longValue(), "Jack");  
-         User rose = new User(((Number) scriptOps.call("getNextSequence", "userid")).longValue(), "Rose");  
-         template.insert(jack);  
-         template.insert(rose);  
- 
-         assertEquals(1, jack.getId());  
-         assertEquals(2, rose.getId());  
- 
-         DB db = template.getDb();  
-         Object eval = db.eval("getNextSequence('userid')");  
- // JavaScript返回的总是双精度浮点型数字
-         assertEquals(3.0d, eval);  
-     }  
- 
- 
- /**
-      * 注释掉此方法可以查看数据库中的集合数据。
-      */
- @After
- publicvoid tearDown() throws Exception {  
-         template.dropCollection("counters");  
-         template.dropCollection(User.class);  
-         template.getCollection("system.js").remove(new BasicDBObject("_id", "getNextSequence"));  
-     }  
- }  
**6、spring需要日志输出，简单配置如下**
**[html]**[view
 plain](http://blog.csdn.net/gp490609578/article/details/50638448#)[copy](http://blog.csdn.net/gp490609578/article/details/50638448#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1575211/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE log4j:configuration SYSTEM "log4j.dtd">
- 
- <log4j:configuration>
- <appendername="default"class="org.apache.log4j.ConsoleAppender">
- <layoutclass="org.apache.log4j.PatternLayout">
- <paramname="ConversionPattern"value="%d{ABSOLUTE} %-5p %40.40c:%4L - %m%n"/>
- </layout>
- </appender>
- 
- <root>
- <levelvalue="info"/>
- <appender-refref="default"/>
- </root>
- </log4j:configuration>
备注：
（1）别忘了在本地启动MongoDB服务器。
（2）如果比较懒，想亲自动手测试，又懒得拷贝代码，可以[下载源码](https://github.com/gaofu/demos/tree/master/mongo-demo)。
