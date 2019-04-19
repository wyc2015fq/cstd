# morphia与spring的整合 - 零度的博客专栏 - CSDN博客
2017年01月16日 15:41:28[零度anngle](https://me.csdn.net/zmx729618)阅读数：742
简单的来说Morphia与MongoDB的关系就如Hibernate与关系数据库的关系, 是一个实现Java对象到MongoDB双向映射的类库。
首先我们需要一个生成和配置mongodb的工厂类：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52936994#)[copy](http://blog.csdn.net/z69183787/article/details/52936994#)[print](http://blog.csdn.net/z69183787/article/details/52936994#)[?](http://blog.csdn.net/z69183787/article/details/52936994#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1950503/fork)
- publicclass MongoFactoryBean extends AbstractFactoryBean<Mongo> {  
- 
- // 表示服务器列表(主从复制或者分片)的字符串数组
- private String[] serverStrings;  
- // mongoDB配置对象
- private MongoOptions mongoOptions;  
- // 是否主从分离(读取从库)，默认读写都在主库
- privateboolean readSecondary = false;  
- // 设定写策略(出错时是否抛异常)，默认采用SAFE模式(需要抛异常)
- private WriteConcern writeConcern = WriteConcern.SAFE;  
- 
- @Override
- public Class<?> getObjectType() {  
- return Mongo.class;  
-     }  
- 
- @Override
- protected Mongo createInstance() throws Exception {  
-         Mongo mongo = initMongo();  
- 
- // 设定主从分离
- if (readSecondary) {  
-             mongo.setReadPreference(ReadPreference.secondaryPreferred());  
-         }  
- 
- // 设定写策略
-         mongo.setWriteConcern(writeConcern);  
- return mongo;  
-     }  
- 
- /**
-      * 初始化mongo实例
-      * @return
-      * @throws Exception
-      */
- private Mongo initMongo() throws Exception {  
- // 根据条件创建Mongo实例
-         Mongo mongo = null;  
-         List<ServerAddress> serverList = getServerList();  
- 
- if (serverList.size() == 0) {  
-             mongo = new Mongo();  
-         }elseif(serverList.size() == 1){  
- if (mongoOptions != null) {  
-                 mongo = new Mongo(serverList.get(0), mongoOptions);  
-             }else{  
-                 mongo = new Mongo(serverList.get(0));  
-             }  
-         }else{  
- if (mongoOptions != null) {  
-                 mongo = new Mongo(serverList, mongoOptions);  
-             }else{  
-                 mongo = new Mongo(serverList);  
-             }  
-         }  
- return mongo;  
-     }  
- 
- 
- /**
-      * 根据服务器字符串列表，解析出服务器对象列表
-      * <p>
-      * 
-      * @Title: getServerList
-      *         </p>
-      * 
-      * @return
-      * @throws Exception
-      */
- private List<ServerAddress> getServerList() throws Exception {  
-         List<ServerAddress> serverList = new ArrayList<ServerAddress>();  
- try {  
- for (String serverString : serverStrings) {  
-                 String[] temp = serverString.split(":");  
-                 String host = temp[0];  
- if (temp.length > 2) {  
- thrownew IllegalArgumentException(  
- "Invalid server address string: " + serverString);  
-                 }  
- if (temp.length == 2) {  
-                     serverList.add(new ServerAddress(host, Integer  
-                             .parseInt(temp[1])));  
-                 } else {  
-                     serverList.add(new ServerAddress(host));  
-                 }  
-             }  
- return serverList;  
-         } catch (Exception e) {  
- thrownew Exception(  
- "Error while converting serverString to ServerAddressList",  
-                     e);  
-         }  
-     }  
- 
- /* ------------------- setters --------------------- */
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
public class MongoFactoryBean extends AbstractFactoryBean<Mongo> {
    // 表示服务器列表(主从复制或者分片)的字符串数组
    private String[] serverStrings;
    // mongoDB配置对象
    private MongoOptions mongoOptions;
    // 是否主从分离(读取从库)，默认读写都在主库
    private boolean readSecondary = false;
    // 设定写策略(出错时是否抛异常)，默认采用SAFE模式(需要抛异常)
    private WriteConcern writeConcern = WriteConcern.SAFE;
    @Override
    public Class<?> getObjectType() {
        return Mongo.class;
    }
    @Override
    protected Mongo createInstance() throws Exception {
        Mongo mongo = initMongo();
        
        // 设定主从分离
        if (readSecondary) {
            mongo.setReadPreference(ReadPreference.secondaryPreferred());
        }
        // 设定写策略
        mongo.setWriteConcern(writeConcern);
        return mongo;
    }
    
    /**
     * 初始化mongo实例
     * @return
     * @throws Exception
     */
    private Mongo initMongo() throws Exception {
        // 根据条件创建Mongo实例
        Mongo mongo = null;
        List<ServerAddress> serverList = getServerList();
        if (serverList.size() == 0) {
            mongo = new Mongo();
        }else if(serverList.size() == 1){
            if (mongoOptions != null) {
                mongo = new Mongo(serverList.get(0), mongoOptions);
            }else{
                mongo = new Mongo(serverList.get(0));
            }
        }else{
            if (mongoOptions != null) {
                mongo = new Mongo(serverList, mongoOptions);
            }else{
                mongo = new Mongo(serverList);
            }
        }
        return mongo;
    }
    
    
    /**
     * 根据服务器字符串列表，解析出服务器对象列表
     * <p>
     * 
     * @Title: getServerList
     *         </p>
     * 
     * @return
     * @throws Exception
     */
    private List<ServerAddress> getServerList() throws Exception {
        List<ServerAddress> serverList = new ArrayList<ServerAddress>();
        try {
            for (String serverString : serverStrings) {
                String[] temp = serverString.split(":");
                String host = temp[0];
                if (temp.length > 2) {
                    throw new IllegalArgumentException(
                            "Invalid server address string: " + serverString);
                }
                if (temp.length == 2) {
                    serverList.add(new ServerAddress(host, Integer
                            .parseInt(temp[1])));
                } else {
                    serverList.add(new ServerAddress(host));
                }
            }
            return serverList;
        } catch (Exception e) {
            throw new Exception(
                    "Error while converting serverString to ServerAddressList",
                    e);
        }
    }
    /* ------------------- setters --------------------- */
}
```

其次我们需要一个产生和配置morphia对象的工厂类：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52936994#)[copy](http://blog.csdn.net/z69183787/article/details/52936994#)[print](http://blog.csdn.net/z69183787/article/details/52936994#)[?](http://blog.csdn.net/z69183787/article/details/52936994#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1950503/fork)
- publicclass MorphiaFactoryBean extends AbstractFactoryBean<Morphia> {  
- /**
-       * 要扫描并映射的包
-       */
- private String[] mapPackages;    
- 
- /**
-       * 要映射的类
-       */
- private String[] mapClasses;    
- 
- /**
-       * 扫描包时，是否忽略不映射的类
-       * 这里按照Morphia的原始定义，默认设为false
-       */
- privateboolean ignoreInvalidClasses;  
- 
- @Override
- protected Morphia createInstance() throws Exception {  
-          Morphia m = new Morphia();  
- if (mapPackages != null) {  
- for (String packageName : mapPackages) {  
-                  m.mapPackage(packageName, ignoreInvalidClasses);  
-              }  
-          }  
- if (mapClasses != null) {    
- for (String entityClass : mapClasses) {  
-                  m.map(Class.forName(entityClass));  
-              }  
-          }  
- return m;  
-      }  
- 
- @Override
- public Class<?> getObjectType() {  
- return Morphia.class;  
-      }  
- 
- /*----------------------setters-----------------------*/
-  }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
public class MorphiaFactoryBean extends AbstractFactoryBean<Morphia> {
     /**
      * 要扫描并映射的包
      */
     private String[] mapPackages;  
     
     /**
      * 要映射的类
      */
     private String[] mapClasses;  
     
     /**
      * 扫描包时，是否忽略不映射的类
      * 这里按照Morphia的原始定义，默认设为false
      */
     private boolean ignoreInvalidClasses;
     
     @Override
     protected Morphia createInstance() throws Exception {
         Morphia m = new Morphia();
         if (mapPackages != null) {
             for (String packageName : mapPackages) {
                 m.mapPackage(packageName, ignoreInvalidClasses);
             }
         }
         if (mapClasses != null) {  
             for (String entityClass : mapClasses) {
                 m.map(Class.forName(entityClass));
             }
         }
         return m;
     }
 
     @Override
     public Class<?> getObjectType() {
         return Morphia.class;
     }
     
     /*----------------------setters-----------------------*/
 }
```
最后我们还需要一个产生和配置Datastore的工厂类：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52936994#)[copy](http://blog.csdn.net/z69183787/article/details/52936994#)[print](http://blog.csdn.net/z69183787/article/details/52936994#)[?](http://blog.csdn.net/z69183787/article/details/52936994#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1950503/fork)
- publicclass DatastoreFactoryBean extends AbstractFactoryBean<Datastore> {  
- 
- private Morphia morphia;    //morphia实例，最好是单例
- private Mongo mongo;    //mongo实例，最好是单例
- private String dbName;    //数据库名
- private String username;    //用户名，可为空
- private String password;    //密码，可为空
- privateboolean toEnsureIndexes=false;    //是否确认索引存在，默认false
- privateboolean toEnsureCaps=false;    //是否确认caps存在，默认false
- 
- 
- @Override
- protected Datastore createInstance() throws Exception {  
- //这里的username和password可以为null，morphia对象会去处理
-          Datastore ds = morphia.createDatastore(mongo, dbName, username,  
-                  password==null?null:password.toCharArray());  
- if(toEnsureIndexes){  
-              ds.ensureIndexes();  
-          }  
- if(toEnsureCaps){  
-              ds.ensureCaps();  
-          }  
- return ds;  
-      }  
- 
- @Override
- public Class<?> getObjectType() {  
- return Datastore.class;  
-      }  
- 
- @Override
- publicvoid afterPropertiesSet() throws Exception {  
- super.afterPropertiesSet();  
- if (mongo == null) {  
- thrownew IllegalStateException("mongo is not set");  
-          }  
- if (morphia == null) {  
- thrownew IllegalStateException("morphia is not set");  
-          }  
-      }  
- 
- /*----------------------setters-----------------------*/
-  }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
public class DatastoreFactoryBean extends AbstractFactoryBean<Datastore> {
     
     private Morphia morphia;    //morphia实例，最好是单例
     private Mongo mongo;    //mongo实例，最好是单例
     private String dbName;    //数据库名
     private String username;    //用户名，可为空
     private String password;    //密码，可为空
     private boolean toEnsureIndexes=false;    //是否确认索引存在，默认false
     private boolean toEnsureCaps=false;    //是否确认caps存在，默认false
     
 
     @Override
     protected Datastore createInstance() throws Exception {
         //这里的username和password可以为null，morphia对象会去处理
         Datastore ds = morphia.createDatastore(mongo, dbName, username,
                 password==null?null:password.toCharArray());
         if(toEnsureIndexes){
             ds.ensureIndexes();
         }
         if(toEnsureCaps){
             ds.ensureCaps();
         }
         return ds;
     }
 
     @Override
     public Class<?> getObjectType() {
         return Datastore.class;
     }
 
     @Override
     public void afterPropertiesSet() throws Exception {
         super.afterPropertiesSet();
         if (mongo == null) {
             throw new IllegalStateException("mongo is not set");
         }
         if (morphia == null) {
             throw new IllegalStateException("morphia is not set");
         }
     }
     
     /*----------------------setters-----------------------*/
 }
```
我们来仿照morphia文档，写两个测试的POJO：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52936994#)[copy](http://blog.csdn.net/z69183787/article/details/52936994#)[print](http://blog.csdn.net/z69183787/article/details/52936994#)[?](http://blog.csdn.net/z69183787/article/details/52936994#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1950503/fork)
- @Entity
- publicclass Hotel {  
- @Idprivate ObjectId id;  
- 
- private String name;  
- privateint stars;  
- 
- @Embedded
- private Address address;      
- 
- /*-----------gettters & setters----------*/
-  }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
@Entity
 public class Hotel {
     @Id private ObjectId id;
     
     private String name;
     private int stars;
     
     @Embedded
     private Address address;    
     
     /*-----------gettters & setters----------*/
 }
```
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52936994#)[copy](http://blog.csdn.net/z69183787/article/details/52936994#)[print](http://blog.csdn.net/z69183787/article/details/52936994#)[?](http://blog.csdn.net/z69183787/article/details/52936994#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1950503/fork)
- @Embedded
- publicclass Address {  
- private String street;  
- private String city;  
- private String postCode;  
- private String country;  
- /*-----------gettters & setters----------*/
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
@Embedded
public class Address {
    private String street;
    private String city;
    private String postCode;
    private String country;
    /*-----------gettters & setters----------*/
}
```
还需要一个为测试POJO专门服务的DAO，这里继承morphia里的BasicDAO：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52936994#)[copy](http://blog.csdn.net/z69183787/article/details/52936994#)[print](http://blog.csdn.net/z69183787/article/details/52936994#)[?](http://blog.csdn.net/z69183787/article/details/52936994#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1950503/fork)
- publicclass HotelDAO extends BasicDAO<Hotel, ObjectId> {  
- 
- protected HotelDAO(Datastore ds) {  
- super(ds);  
-     }  
- 
- /* ----------------以下是自定义的数据查询方法(finder)----------------- */
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
public class HotelDAO extends BasicDAO<Hotel, ObjectId> {
    protected HotelDAO(Datastore ds) {
        super(ds);
    }
    
    /* ----------------以下是自定义的数据查询方法(finder)----------------- */
}
```
最后是spring的XML文件：

**[html]**[view plain](http://blog.csdn.net/z69183787/article/details/52936994#)[copy](http://blog.csdn.net/z69183787/article/details/52936994#)[print](http://blog.csdn.net/z69183787/article/details/52936994#)[?](http://blog.csdn.net/z69183787/article/details/52936994#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1950503/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:p="http://www.springframework.org/schema/p"xmlns:context="http://www.springframework.org/schema/context"
- xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.1.xsd   
-     http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-3.1.xsd">
- 
- <!-- 配置文件 -->
- <context:property-placeholderlocation="classpath:config.properties"/>
- 
- <!-- mongoDB的配置对象 -->
- <beanid="mongoOptions"class="com.mongodb.MongoOptions">
- <!-- 服务器是否自动重连，默认为false -->
- <propertyname="autoConnectRetry"value="false"/>
- <!-- 对同一个服务器尝试重连的时间(毫秒)，设为0时默认使用15秒 -->
- <propertyname="maxAutoConnectRetryTime"value="0"/>
- <!-- 与每个主机的连接数，默认为10 -->
- <propertyname="connectionsPerHost"value="10"/>
- <!-- 连接超时时间(毫秒)，默认为10000 -->
- <propertyname="connectTimeout"value="10000"/>
- <!-- 是否创建一个finalize方法，以便在客户端没有关闭DBCursor的实例时，清理掉它。默认为true -->
- <propertyname="cursorFinalizerEnabled"value="true"/>
- <!-- 线程等待连接可用的最大时间(毫秒)，默认为120000 -->
- <propertyname="maxWaitTime"value="120000"/>
- <!-- 可等待线程倍数，默认为5.例如connectionsPerHost最大允许10个连接，则10*5=50个线程可以等待，更多的线程将直接抛异常 -->
- <propertyname="threadsAllowedToBlockForConnectionMultiplier"value="5"/>
- <!-- socket读写时超时时间(毫秒)，默认为0，不超时 -->
- <propertyname="socketTimeout"value="0"/>
- <!-- 是socket连接在防火墙上保持活动的特性，默认为false -->
- <propertyname="socketKeepAlive"value="false"/>
- <!-- 对应全局的WriteConcern.SAFE，默认为false -->
- <propertyname="safe"value="true"/>
- <!-- 对应全局的WriteConcern中的w，默认为0 -->
- <propertyname="w"value="0"/>
- <!-- 对应全局的WriteConcern中的wtimeout，默认为0 -->
- <propertyname="wtimeout"value="0"/>
- <!-- 对应全局的WriteConcern.FSYNC_SAFE，如果为真，每次写入要等待写入磁盘，默认为false -->
- <propertyname="fsync"value="false"/>
- <!-- 对应全局的WriteConcern.JOURNAL_SAFE，如果为真，每次写入要等待日志文件写入磁盘，默认为false -->
- <propertyname="j"value="false"/>
- </bean>
- 
- <!-- 使用工厂创建mongo实例 -->
- <beanid="mongo"class="me.watchzerg.test.morphia.spring.MongoFactoryBean">
- <!-- mongoDB的配置对象 -->
- <propertyname="mongoOptions"ref="mongoOptions"/>
- 
- <!-- 是否主从分离(读取从库)，默认为false，读写都在主库 -->
- <propertyname="readSecondary"value="false"/>
- 
- <!-- 设定写策略，默认为WriteConcern.SAFE，优先级高于mongoOptions中的safe -->
- <propertyname="writeConcern"value="SAFE"/>
- 
- <!-- 设定服务器列表，默认为localhost:27017 -->
- <propertyname="serverStrings">
- <array>
- <value>${mongoDB.server}</value>
- </array>
- </property>
- </bean>
- 
- 
- <!-- 使用工厂创建morphia实例，同时完成类映射操作 -->
- <beanid="morphia"class="me.watchzerg.test.morphia.spring.MorphiaFactoryBean">
- <!-- 指定要扫描的POJO包路径 -->
- <propertyname="mapPackages">
- <array>
- <value>me.watchzerg.test.morphia.pojo</value>
- </array>
- </property>
- 
- <!-- 指定要映射的类 -->
-         <!-- <propertyname="mapClasses">
- <array>
- <value>me.watchzerg.test.morphia.pojo.Hotel</value>
- <value>me.watchzerg.test.morphia.pojo.Address</value>
- </array>
- </property> -->
- 
- <!-- 扫描包时是否忽略不可用的类，默认为false -->
- <!-- <property name="ignoreInvalidClasses" value="false"/> -->
- </bean>
- 
- <!-- 使用工厂创建datastore，同时完成index和caps的确认操作 -->
- <beanid="datastore"class="me.watchzerg.test.morphia.spring.DatastoreFactoryBean">
- <propertyname="morphia"ref="morphia"/>
- <propertyname="mongo"ref="mongo"/>
- 
- <!-- collection的名称 -->
- <propertyname="dbName"value="${mongoDB.dbName}"/>
- 
- <!-- 用户名和密码可以为空 -->
-         <!-- <propertyname="username"value="my_username"/>
- <propertyname="password"value="my_password"/> -->
- 
- <!-- 是否进行index和caps的确认操作，默认为flase -->
- <propertyname="toEnsureIndexes"value="true"/>
- <propertyname="toEnsureCaps"value="true"/>
- </bean>
- 
- <!-- ===============以下是具体DAO的实现===================== -->
- 
- <beanid="hotelDAO"class="me.watchzerg.test.morphia.dao.impl.HotelDAO">
- <constructor-argref="datastore"/>
- </bean>
- 
- </beans>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<?xml version="1.0" encoding="UTF-8"?>  
<beans xmlns="http://www.springframework.org/schema/beans"  
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:p="http://www.springframework.org/schema/p" xmlns:context="http://www.springframework.org/schema/context" 
    xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.1.xsd 
    http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-3.1.xsd">  
    
    <!-- 配置文件 -->
    <context:property-placeholder location="classpath:config.properties" />
    
    <!-- mongoDB的配置对象 -->
    <bean id="mongoOptions" class="com.mongodb.MongoOptions">
        <!-- 服务器是否自动重连，默认为false -->
        <property name="autoConnectRetry" value="false" />
        <!-- 对同一个服务器尝试重连的时间(毫秒)，设为0时默认使用15秒 -->
        <property name="maxAutoConnectRetryTime" value="0" />
        <!-- 与每个主机的连接数，默认为10 -->
        <property name="connectionsPerHost" value="10" />
        <!-- 连接超时时间(毫秒)，默认为10000 -->
        <property name="connectTimeout" value="10000" />
        <!-- 是否创建一个finalize方法，以便在客户端没有关闭DBCursor的实例时，清理掉它。默认为true -->
        <property name="cursorFinalizerEnabled" value="true" />
        <!-- 线程等待连接可用的最大时间(毫秒)，默认为120000 -->
        <property name="maxWaitTime" value="120000" />
        <!-- 可等待线程倍数，默认为5.例如connectionsPerHost最大允许10个连接，则10*5=50个线程可以等待，更多的线程将直接抛异常 -->
        <property name="threadsAllowedToBlockForConnectionMultiplier" value="5" />
        <!-- socket读写时超时时间(毫秒)，默认为0，不超时 -->
        <property name="socketTimeout" value="0" />
        <!-- 是socket连接在防火墙上保持活动的特性，默认为false -->
        <property name="socketKeepAlive" value="false" />
        <!-- 对应全局的WriteConcern.SAFE，默认为false -->
        <property name="safe" value="true" />
        <!-- 对应全局的WriteConcern中的w，默认为0 -->
        <property name="w" value="0" />
        <!-- 对应全局的WriteConcern中的wtimeout，默认为0 -->
        <property name="wtimeout" value="0" />
        <!-- 对应全局的WriteConcern.FSYNC_SAFE，如果为真，每次写入要等待写入磁盘，默认为false -->
        <property name="fsync" value="false" />
        <!-- 对应全局的WriteConcern.JOURNAL_SAFE，如果为真，每次写入要等待日志文件写入磁盘，默认为false -->
        <property name="j" value="false" />
    </bean>
    
    <!-- 使用工厂创建mongo实例 -->
    <bean id="mongo" class="me.watchzerg.test.morphia.spring.MongoFactoryBean">
        <!-- mongoDB的配置对象 -->
        <property name="mongoOptions" ref="mongoOptions"/>
        
        <!-- 是否主从分离(读取从库)，默认为false，读写都在主库 -->
        <property name="readSecondary" value="false"/>
        
        <!-- 设定写策略，默认为WriteConcern.SAFE，优先级高于mongoOptions中的safe -->
        <property name="writeConcern" value="SAFE"/>
        
        <!-- 设定服务器列表，默认为localhost:27017 -->
        <property name="serverStrings">
            <array>
                <value>${mongoDB.server}</value>
            </array>
        </property>
    </bean>
    
    
    <!-- 使用工厂创建morphia实例，同时完成类映射操作 -->
    <bean id="morphia" class="me.watchzerg.test.morphia.spring.MorphiaFactoryBean" >
        <!-- 指定要扫描的POJO包路径 -->
        <property name="mapPackages">
            <array>
                <value>me.watchzerg.test.morphia.pojo</value>
            </array>
        </property>
        
        <!-- 指定要映射的类 -->
        <!-- <property name="mapClasses">
            <array>
                <value>me.watchzerg.test.morphia.pojo.Hotel</value>
                <value>me.watchzerg.test.morphia.pojo.Address</value>
            </array>
        </property> -->
        
        <!-- 扫描包时是否忽略不可用的类，默认为false -->
        <!-- <property name="ignoreInvalidClasses" value="false"/> -->
    </bean>
    
    <!-- 使用工厂创建datastore，同时完成index和caps的确认操作 -->
    <bean id="datastore" class="me.watchzerg.test.morphia.spring.DatastoreFactoryBean" >
        <property name="morphia" ref="morphia"/>
        <property name="mongo" ref="mongo"/>
        
        <!-- collection的名称 -->
        <property name="dbName" value="${mongoDB.dbName}"/>
        
        <!-- 用户名和密码可以为空 -->
        <!-- <property name="username" value="my_username"/>
        <property name="password" value="my_password"/> -->
        
        <!-- 是否进行index和caps的确认操作，默认为flase -->
        <property name="toEnsureIndexes" value="true"/>
        <property name="toEnsureCaps" value="true"/>
    </bean>
    
    <!-- ===============以下是具体DAO的实现===================== -->
    
    <bean id="hotelDAO" class="me.watchzerg.test.morphia.dao.impl.HotelDAO">
        <constructor-arg ref="datastore"/>
    </bean>
    
</beans>
```

最后写一个测试类看看我们的成果：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52936994#)[copy](http://blog.csdn.net/z69183787/article/details/52936994#)[print](http://blog.csdn.net/z69183787/article/details/52936994#)[?](http://blog.csdn.net/z69183787/article/details/52936994#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1950503/fork)
- publicclass MorphiaTest {  
- privatestatic HotelDAO hotelDAO;  
- 
- /**
-     * 测试Morphia的DAO层
-     * 
-     * @param args
-     * @throws Exception
-     */
- publicstaticvoid main(String[] args) throws Exception {  
- // 初始化DAO
-        initDAO();  
- 
- // 插入测试
-        saveTest();  
- 
- // 更新测试
- // updateTest();
- 
- // 删除测试
- // deleteTest();
- 
- // 查询测试
- // queryHotel();
- 
-        System.out.println("done!");  
-    }  
- 
- /**
-     * 初始化DAO
-     * <p>
-     * @Title: initDAO
-     * </p>
-     */
- privatestaticvoid initDAO() {  
-        ApplicationContext context = new ClassPathXmlApplicationContext(  
- "config.xml");  
-        hotelDAO = (HotelDAO) context.getBean("hotelDAO");  
-    }  
- 
- /**
-     * 生成指定个数的hotelList
-     * <p>
-     * @Title: getHotelList
-     * </p>
-     * 
-     * @param num
-     * @return
-     */
- privatestatic List<Hotel> getHotelList(int num) {  
-        List<Hotel> list = new ArrayList<Hotel>();  
- for (int i = 0; i < num; i++) {  
-            Hotel hotel = new Hotel();  
-            hotel.setName("编号为[" + i + "]的旅店");  
-            hotel.setStars(i % 10);  
-            Address address = new Address();  
-            address.setCountry("中国");  
-            address.setCity("北京");  
-            address.setStreet("上帝南路");  
-            address.setPostCode("10000" + (i % 10));  
-            hotel.setAddress(address);  
-            list.add(hotel);  
-        }  
- return list;  
-    }  
- 
- /**
-     * 将hotelList插入数据库
-     * <p>
-     * @Title: saveHotelList
-     * </p>
-     * 
-     * @param hotelDAO
-     * @param hotelList
-     */
- privatestaticvoid saveTest() {  
-        List<Hotel> hotelList = getHotelList(100);  
- for (Hotel hotel : hotelList) {  
- // Key<Hotel> key=hotelDAO.save(hotel,WriteConcern.SAFE);
-            Key<Hotel> key = hotelDAO.save(hotel);  
-            System.out.println("id为[" + key.getId() + "]的记录已被插入");  
-        }  
-    }  
- 
- /**
-     * 更新操作测试
-     * <p>
-     * @Title: updateTest
-     * </p>
-     * 
-     * @throws Exception
-     */
- privatestaticvoid updateTest() throws Exception {  
- //生成查询条件
-        Query<Hotel> q = hotelDAO.createQuery().field("stars")  
-                .greaterThanOrEq(9);  
- //生成更新操作
-        UpdateOperations<Hotel> ops = hotelDAO.createUpdateOperations()  
-                .set("address.city", "shanghai").inc("stars");  
- // UpdateResults<Hotel> ur=hotelDAO.update(q, ops);
-        UpdateResults<Hotel> ur = hotelDAO.updateFirst(q, ops);  
- if (ur.getHadError()) {  
-            System.out.println(ur.getError());  
- thrownew Exception("更新时发生错误");  
-        }  
- if (ur.getUpdatedExisting()) {  
-            System.out.println("更新成功，更新条数为[" + ur.getUpdatedCount()  
-                    + "]，插入条数为[" + ur.getInsertedCount() + "]");  
-        } else {  
-            System.out.println("没有记录符合更新条件");  
-        }  
-    }  
- 
- /**
-     * 删除操作测试
-     * <p>
-     * @Title: deleteTest
-     * </p>
-     */
- privatestaticvoid deleteTest() {  
-        ObjectId id = hotelDAO.findIds().get(0);  
-        hotelDAO.deleteById(id);  
- 
-        Query<Hotel> q = hotelDAO.createQuery().field("stars")  
-                .greaterThanOrEq(100);  
-        hotelDAO.deleteByQuery(q);  
-    }  
- 
- /**
-     * 查询测试
-     * <p>
-     * @Title: queryHotel
-     * </p>
-     */
- privatestaticvoid queryHotel() {  
- // 显示所有记录
-        System.out.println("\nhotelDAO.find()=");  
- for (Hotel hotel : hotelDAO.find()) {  
-            System.out.println(hotel);  
-        }  
- 
- // 统计star大于等于9的数目
-        System.out  
-                .println("\nhotelDAO.count(hotelDAO.createQuery().field(\"stars\").greaterThanOrEq(9))="
-                        + hotelDAO.count(hotelDAO.createQuery().field("stars")  
-                                .greaterThanOrEq(9)));  
- 
- // 显示符合条件的记录ID
-        List<ObjectId> ids = hotelDAO.findIds("stars", 8);  
-        System.out.println("\nhotelDAO.findIds(\"stars\", 8)=");  
- for (ObjectId id : ids) {  
-            System.out.println(id);  
-        }  
-    }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
public class MorphiaTest {
   private static HotelDAO hotelDAO;
   /**
    * 测试Morphia的DAO层
    * 
    * @param args
    * @throws Exception
    */
   public static void main(String[] args) throws Exception {
       // 初始化DAO
       initDAO();
       // 插入测试
       saveTest();
       // 更新测试
       // updateTest();
       // 删除测试
       // deleteTest();
       // 查询测试
       // queryHotel();
       System.out.println("done!");
   }
   /**
    * 初始化DAO
    * <p>
    * @Title: initDAO
    * </p>
    */
   private static void initDAO() {
       ApplicationContext context = new ClassPathXmlApplicationContext(
               "config.xml");
       hotelDAO = (HotelDAO) context.getBean("hotelDAO");
   }
   /**
    * 生成指定个数的hotelList
    * <p>
    * @Title: getHotelList
    * </p>
    * 
    * @param num
    * @return
    */
   private static List<Hotel> getHotelList(int num) {
       List<Hotel> list = new ArrayList<Hotel>();
       for (int i = 0; i < num; i++) {
           Hotel hotel = new Hotel();
           hotel.setName("编号为[" + i + "]的旅店");
           hotel.setStars(i % 10);
           Address address = new Address();
           address.setCountry("中国");
           address.setCity("北京");
           address.setStreet("上帝南路");
           address.setPostCode("10000" + (i % 10));
           hotel.setAddress(address);
           list.add(hotel);
       }
       return list;
   }
   /**
    * 将hotelList插入数据库
    * <p>
    * @Title: saveHotelList
    * </p>
    * 
    * @param hotelDAO
    * @param hotelList
    */
   private static void saveTest() {
       List<Hotel> hotelList = getHotelList(100);
       for (Hotel hotel : hotelList) {
           // Key<Hotel> key=hotelDAO.save(hotel,WriteConcern.SAFE);
           Key<Hotel> key = hotelDAO.save(hotel);
           System.out.println("id为[" + key.getId() + "]的记录已被插入");
       }
   }
   /**
    * 更新操作测试
    * <p>
    * @Title: updateTest
    * </p>
    * 
    * @throws Exception
    */
   private static void updateTest() throws Exception {
       //生成查询条件
       Query<Hotel> q = hotelDAO.createQuery().field("stars")
               .greaterThanOrEq(9);
       //生成更新操作
       UpdateOperations<Hotel> ops = hotelDAO.createUpdateOperations()
               .set("address.city", "shanghai").inc("stars");
       // UpdateResults<Hotel> ur=hotelDAO.update(q, ops);
       UpdateResults<Hotel> ur = hotelDAO.updateFirst(q, ops);
       if (ur.getHadError()) {
           System.out.println(ur.getError());
           throw new Exception("更新时发生错误");
       }
       if (ur.getUpdatedExisting()) {
           System.out.println("更新成功，更新条数为[" + ur.getUpdatedCount()
                   + "]，插入条数为[" + ur.getInsertedCount() + "]");
       } else {
           System.out.println("没有记录符合更新条件");
       }
   }
   /**
    * 删除操作测试
    * <p>
    * @Title: deleteTest
    * </p>
    */
   private static void deleteTest() {
       ObjectId id = hotelDAO.findIds().get(0);
       hotelDAO.deleteById(id);
       Query<Hotel> q = hotelDAO.createQuery().field("stars")
               .greaterThanOrEq(100);
       hotelDAO.deleteByQuery(q);
   }
   /**
    * 查询测试
    * <p>
    * @Title: queryHotel
    * </p>
    */
   private static void queryHotel() {
       // 显示所有记录
       System.out.println("\nhotelDAO.find()=");
       for (Hotel hotel : hotelDAO.find()) {
           System.out.println(hotel);
       }
       // 统计star大于等于9的数目
       System.out
               .println("\nhotelDAO.count(hotelDAO.createQuery().field(\"stars\").greaterThanOrEq(9))="
                       + hotelDAO.count(hotelDAO.createQuery().field("stars")
                               .greaterThanOrEq(9)));
       // 显示符合条件的记录ID
       List<ObjectId> ids = hotelDAO.findIds("stars", 8);
       System.out.println("\nhotelDAO.findIds(\"stars\", 8)=");
       for (ObjectId id : ids) {
           System.out.println(id);
       }
   }
}
```

