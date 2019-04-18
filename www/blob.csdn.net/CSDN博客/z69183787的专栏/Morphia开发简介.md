# Morphia开发简介 - z69183787的专栏 - CSDN博客
2016年10月27日 07:04:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1059
快速开始：
@Entity
public class Hotel {
     @Id private ObjectId id;
    private String name;
    private int stars;
     @Embedded
    private Address address;
}
@Embedded
public class Address {
    private String street;
    private String city;
    private String postCode;
    private String country;
}
main方法：
Mongo mongo=new Mongo();
Morphia morphia = new Morphia();
morphia.map(Hotel.class).map(Address.class);
Datastore ds = morphia.createDatastore(mongo, "my_database");
//保存
ds.save(hotelToSave);
//查询四星级酒店
//List<Hotel> fourStarHotels = ds.find(Hotel.class, "stars >=", 4).asList();
List<Hotel> fourStarHotels = ds.find(Hotel.class).field("stars").greaterThanOrEq(4).asList();
-------------------------------------------------
Datastore创建后index和capped collections的初始化：
ds.ensureIndexes(); ///creates all defined with @Indexed
ds.ensureCaps(); //creates all collections for @Entity(cap=@CappedAt(...))
这两个方法应该在你把实体注册到morphia之后(morphia.map(Hotel.class).map(Address.class))来调用，它默认会同步的创建index和capped collections，这可以在每次系统启动时确保索引存在(包括第一次运行的时候)。
查询一条记录：
MyEntity e = ds.find(MyEntity.class).get(); //get the first one by type
MyEntity e = ds.find(MyEntity.class).field("name").equal("someName").get(); //get the first one where name = "someName"
-------------------------------------------------
Datastore接口介绍：
-------------------------
get方法：其实就是find方法的快捷方式，根据ID返回一个实体，或者返回null。
Hotel hotel = ds.get(Hotel.class, hotelId);
-------------------------
find方法：会返回一个Query对象，支持迭代器和QueryResults接口。
//use in a loop
for(Hotel hotel : ds.find(Hotel.class, "stars >", 3))
   print(hotel);
//get back as a list
List<Hotel> hotels = ds.find(Hotel.class, "stars >", 3).asList();
//sort the results
List<Hotel> hotels = ds.find(Hotel.class, "stars >", 3).sort("-stars").asList();
//get the first matching hotel, by querying with a limit(1)
Hotel gsHotel = ds.find(Hotel.class, "name", "Grand Sierra").get();
//same as
Hotel gsHotel = ds.find(Hotel.class, "name =", "Grand Sierra").get();
可用的操作符列表： ["=", "==","!=", "<>", ">", "<", ">=", "<=", "in", "nin", "all", "size", "exists"]
-------------------------
save方法：
Hotel hotel = new Hotel();
ds.save(hotel);
//@Id field is filled in for you (after the save), if you didn't set it.
ObjectId id = hotel.getId();
-------------------------
delete方法：根据一个ID或者查询对象来删除
ds.delete(Hotel.class, "Grand Sierra Resort");
//use a query
ds.delete(ds.createQuery(Hotel.class).filter("pendingDelete", true));
-------------------------
FindAndDelete方法：删除并返回记录的原子操作
Hotel grandSierra = ds.findAndDelete(ds.get(Hotel.class, "Grand Sierra Resort"));
-------------------------
Update方法：更新部分信息时比整体save一次要高效的多，例如更新用户的最后登录时间：
@Entity
class User
{
   @Id private ObjectId id;
   private long lastLogin;
   //... other members
   private Query<User> queryToFindMe()
   {
      return datastore.createQuery(User.class).field(Mapper.ID_KEY).equal(id);
   }
   public void loggedIn()
   {
      long now = System.currentTimeMillis();
      UpdateOperations<User> ops = datastore.createUpdateOperations(User.class).set("lastLogin", now);
      ds.update(queryToFindMe(), ops);
      lastLogin = now;
   }
}
有关update方法的更多高级操作：
http://code.google.com/p/morphia/wiki/Updating
-------------------------------------------------
Query接口介绍：
可以添加查询条件，排序，限定返回结果条数和位置。实现了QueryResults接口。
-------------------------
Filter方法：
Query q = ds.createQuery(MyEntity.class).filter("foo >", 12);
第一个参数是属性名和比较符，第二个参数是比较值。
多个fileter直接的关系是“与(and)”
Query q = ds.createQuery(MyEntity.class).filter("foo >", 12).filter("foo <", 30);
可用操作符列表：
["=", "==","!=", "<>", ">", "<", ">=", "<=", "in", "nin", "all", "size", "exists"]
-------------------------
Fluent接口：
与Fileter接口差不多，英文呢方法名，更符合使用习惯：
Query q = ds.createQuery(MyEntity.class).field("foo").equal(1);
可用方法列表：
[exists,doesNotExist,greaterThan,greaterThanOrEq,lessThan,lessThanOrEq,equal,notEqual,hasThisOne,hasAllOf,hasAnyOf,hasNoneOf,hasThisElement,sizeEq]
使用Fluent接口，可以使用or条件：
Query<Person> q = ad.createQuery(Person.class);
q.or(
        q.criteria("firstName").equal("scott"),
        q.criteria("lastName").equal("scott")
);
-------------------------
Geo-spatial：mongoDB的地理位置操作
-------------------------
Fields方法：
类似与mongo本身的query，可以用“点号”指定属性。
Query q = ds.createQuery(Person.class).field("addresses.city").equal("San Francisco");
//or with filter, or with this helper method
Query q = ds.find(Person.class, "addresses.city", "San Francisco");
-------------------------
Validation：如果查询时属性名找不到，则会抛出异常。如果用到“点号”指定属性，则表达式每一部分都必须匹配类的结构图。
如果服务器可以强制转换数据，则数据类型的错误会被作为警告记录下来。
上述校验特性可以在任何一个查询或查询的一部分处被关闭：
Query q = ds.createQuery(MyEntity.class).disableValidation();
//or it can be disabled for just one filter
Query q = ds.createQuery(MyEntity.class).disableValidation().filter("someOldField", value).enableValidation().filter("realField", otherVal);
-------------------------
Sort方法：排序
Query q = ds.createQuery(MyEntity.class).filter("foo >", 12).order("dateAdded");
... // desc order
Query q = ds.createQuery(MyEntity.class).filter("foo >", 12).order("-dateAdded");
... // asc dateAdded, desc foo
Query q = ds.createQuery(MyEntity.class).filter("foo >", 12).order("dateAdded, -foo");
-------------------------
Limit方法：
Query q = ds.createQuery(MyEntity.class).filter("foo >", 12).limit(100);
-------------------------
offset(skip)方法：要求服务器跳过一部分记录，这么做效率不如使用一些属性的range filter，比如pagination。
Query q = ds.createQuery(MyEntity.class).filter("foo >", 12).offset(1000);
-------------------------
只返回指定的属性：这么做会导致不完整的对象，小心使用。
MyEntity e = ds.createQuery(MyEntity.class).retrievedFields(true, "foo").get();
val = e.getFoo(); // 仅返回这个字段
MyEntity e = ds.createQuery(MyEntity.class).retrievedFields(false, "foo").get();
val = e.getFoo(); // 仅不返回这个字段
指定字段名的参数可以是字符串list或者数组：
MyEntity e = ds.createQuery(MyEntity.class).retrievedFields(true, "foo", "bar").get();
-------------------------
取数据：
使用QueryResults接口的方法即可取到数据，这些方法不会影响Query对象本身（返回的是副本），所以可以反复调用。
get()     使用limit(1)，返回一条记录
asList()     返回所有记录，大数据量时可能较慢
fetch()     明确要求返回一个可迭代的对象
asKeyList()     返回记录的Key<T>，只从server取id字段
fetchEmptyEntities()     与fetch类似，但只取id字段
Query q = ds.createQuery(MyEntity.class).filter("foo >", 12);
MyEntity e = q.get();
e = q.sort("foo").get();
for (MyEntity e : q)  print(e);
List<MyEntity> entities = q.asList();
-------------------------------------------------
生命周期方法注解：
@PrePersist - 在save之前调用，可以返回一个DBObject以替代一个空值
@PostPersist - 在save之后调用
@PreLoad - 在将数据映射到POJO之前调用，DBObject作为参数传入，你可以手动修改其中的值
@PostLoad - 在将数据映射到POJO之后调用
测试类例子：
http://code.google.com/p/morphia/source/browse/trunk/morphia/src/test/java/com/google/code/morphia/TestDatastore.java
//这个例子保存前将最后登录时间更新为当前时间
class BankAccount {
  @Id String id;
  Date lastUpdated = new Date();
  @PrePersist void prePersist() {lastUpdated = new Date();}
}
下面的这个例子中，通过EntityListerners注解，将生命周期事件的实现放到外部类当中：
@EntityListeners(BackAccountWatcher.class)
class BankAccount {
  @Id String id;
  Date lastUpdated = new Date();
}
class BankAccountWatcher{
  @PrePersist void prePersist(BankAccount act) {act.lastUpdated = new Date();}
}
注意：没有delete操作相关的生命周期事件。
-------------------------------------------------
类注解：
-------------------------
@Entity
标记一个要被映射的类。此注解为可选，一般写上没坏处。
@Entity("collectionName")
指定对应的collection的名称，这种情况POJO里必须有无参构造方法
@Entity(noClassnameStored = true)
注解不要存储类名。
默认是存储类名的，因为可以把不同的类(特别是相互继承的类)存入同一个collection,例如：
@Entity("animals") abstract class Animal { String name; }
@Entity("animals") Cat extends Animal { ... }
@Entity("animals") Dog extends Animal { ... }
@Entity(cap = @CappedAt(...)) 指定为Capped
-------------------------
@Indexes 索引注解可以标记在类上，这样就可以建立多列索引
@Entity
@Indexes( @Index("user, -date") )
public class ChangeLog{
Date date;
String user;
Record changedRecord;
}
注："date"前面的负号表示日期降序排列，便于查找最近的用户
还可以添加多个多列索引：
@Indexes({
   @Index("user, -cs"),
   @Index("changedRecord, -cs")})
注：也可以(但不建议)在这里定义单列索引，但最好还是注解在具体的列上
-------------------------------------------------
字段注解：
-------------------------
@Id 标记字段为数据库主键
POJO里的对应字段可以是任意支持持久化的类型，例如int,uuid,Object
使用ObjectId可以实现自动生成，否则在每次存储前需要指定此值
-------------------------
@Indexed  注解索引
每次调用datastore.ensureIndexes()时，这些索引就会被应用。
注：如果在一个已经存在数据和索引的系统上，调用datastore.ensureIndexes()方法，将不会产生任何操作，所以可以放心的调用。
@Indexed(value=IndexDirection.ASC, name="upc", unique=true, dropDups=true)
value：索引方向，默认是IndexDirection.ASC，还可以是IndexDirection.DESC和IndexDirection.BOTH
name：索引名，默认是mongoDB自动生成
unique：是否唯一索引，默认为flase
dropDups：通知唯一索引静默删除已有的重复元素，只保留第一个，默认为false
-------------------------
@Embedded 注解需要嵌入的对象(形成一个对象树来读取/写入)
被嵌入的对象将嵌套在父对象里，被存入同一个collection中，所以被嵌入的对象里不允许出现@Id注解。
也可以指定被嵌入对象在mongoDB里的属性名：
@Embedded("blog_comments")
-------------------------
@Property POJO属性注解
在POJO中，java原生类型和基本类型默认是不需要注解即可完成读取和写入的(除非注解了@Transient)。
mongoDB只支持四种数据类型：Integer，Long，Double，String
morphia会自动映射java基本数据类型和String，这些类型的数组，以及List,Set,Map到mongoDB中。
另外，以下对象也会被自动转换和读取/写入：
enum 转为String存储
java.util.Date 转为毫秒数存储
java.util.Locale 转为String存储
com.mongodb.DBRef
com.mongodb.ObjectId
注：morphia默认使用POJO属性名作为collection里的字段名，这个行为可以被覆盖：
@Property("my_integer")
private int myInt;
-------------------------
@Reference 引用注解，标记某一个字段存在另一个collection中。
该注解有三种属性：
lazy：懒汉模式，被调用时才从数据库获取此字段
ignoreMissing：读取引用失败时不产生异常
concreteClass：产生的实例的类类型
例子：
@Entity public class BlogEntry { @Id private ObjectId id; @Reference private Author author;}
@Entity public class Author { @Id private ObjectId id;}
注：被引用的对象必须存在于mongoDB中，然后才能存储引用其的对象。
注：morphia默认使用POJO里引用的属性名作为collection里的字段名，这个行为可以被覆盖：
@Reference("blog_authors")
private List<Author> authors;
-------------------------
集合类相关的属性注解：
morphia支持Collection（List,Set,Map）：
private Set<String> tags;
private Map<String,Translation> translations;
@Reference private List<Article> relatedArticles;
默认情况下，morphia读取数据创建实例时会使用以下实现类：
java.util.ArrayList
java.util.HashSet
java.util.HashMap
这个行为可以被覆盖：
@Property(concreteClass = java.util.TreeSet.class)
private Set<String> tags;
@Embedded(concreteClass = java.util.TreeMap.class)
private Map<String,Translation> translations;
@Reference(concreteClass = java.util.Vector.class)
private List<Article> relatedArticles;
-------------------------
@Transient 标记字段被忽略，包括读取/写入
@Serialized 字段被转为二进制后储存
@NotSaved 字段可以被读取，但在写入时忽略
@AlsoLoad 字段可以被任何支持的名字所读取
@Version 版本号标记，自动实现乐观锁，标记后修改操作时可能会抛出ConcurrentModificationException
-------------------------------------------------
Morphia的扩展：
-------------------------
校验扩展ValidationExtension：
符合JSR303，可以直接调用Hibernate validation。
对JSR303 API的轻量级的包装：new ValidationExtension(morphia);
import org.hibernate.validator.constraints.Email;
@Entity
public class Userlike {
        @Id ObjectId id;
        @Email String email;
}
-------------------------
日志重定向扩展：SLF4JExtension
将morphia的运行日志重定向到SLF4J中，引入morphia-logging-slf4j-0.99.jar，在系统启动时执行一次：
MorphiaLoggerFactory.registerLogger(SLF4JLoggerImplFactory.class);
-------------------------------------------------
手动映射对象到DBObjects（以便传递给底层的driver）：
-------------------------
创建Morphia实例（建议只创建一次，然后重用它）：
Morphia morphia = new Morphia();
morphia.map(BlogEntry.class).map(Author.class);
每一个这样手动映射的类都会被检查，如果失败会抛出MappingException。
也可以让morphia自动扫描某个包，并自动映射：
morphia.mapPackage("my.package.with.only.mongo.entities");
-------------------------
高级使用：从morphia里直接调用底层的java driver存储数据(手动)：
Morphia morphia = ...;
Mongo mongo = ...;
DB db = mongo.getDB("BlogSite");
//这是注解过的POJO
BlogEntry blogEntry = ...;
//让morphia将POJO转为java driver需要的DBObject
DBObject blogEntryDbObj = morphia.toDBObject(blogEntry);
//用java driver将其写入mongodb
db.getCollection("BlogEntries").save(blogEntryDbObj);
-------------------------
高级使用：从morphia里直接调用底层的java driver读取数据(手动)：
Morphia morphia = ...;
Mongo mongo = ...;
DB db = mongo.getDB("BlogSite");
//要读取的ID
String blogEntryId = ...;
//调用java driver从mongdoDB取出一个DBObject
BasicDBObject blogEntryDbObj = (BasicDBObject) db.getCollection("BlogEntries").findOne(new BasicDBObject("_id", new ObjectId(blogEntryId));
//让morphia将DBObject转为POJO
BlogEntry blogEntry = morphia.fromDBObject(BlogEntry.class, blogEntryDbObj);
-------------------------------------------------
DAO层的封装：
morphia已经提供了一个DAO接口和一个BasicDAO类。
我们只要继承BasicDAO类，覆盖其中的构造方法，将mongo和morphia对象传入即可：
public class BlogEntryDAO extends BasicDAO<BlogEntry, ObjectId> {
    public BlogEntryDAO( Morphia morphia, Mongo mongo,String dbName) {
        super(mongo, morphia, dbName);
    }
}
然后就可以实现大部分操作：
BlogEntryDAO blogEntryDAO = new BlogEntryDAO(...);
ObjectId  blogEntryId = ...;
BlogEntry myBlogEntry = blogEntryDAO.get(blogEntryId);//查询
myBlogEntry.setTitle("My Blog Entry");
blogEntryDAO.save(myBlogEntry);//保存
blogEntryDAO.deleteById(myBlogEntry.getId());//删除
然后还需要自定义一些查询方法（注意这里用了正则匹配文章标题）：
public List<BlogEntry> findByTitle( String title ) {
    Pattern regExp = Pattern.compile(title + ".*", Pattern.CASE_INSENSITIVE);
    return ds.find(entityClazz).filter("title", regExp).sort("title").asList();
}
星游注：话说，加入DAO层不就是为了与morphia解耦么？现在DAO层的父类本身就在morphia包里，“这不科学呀。。。”
建议参照其BasicDAO，自己写一个，这才实现了与持久层解耦：
http://code.google.com/p/morphia/source/browse/trunk/morphia/src/main/java/com/google/code/morphia/dao/BasicDAO.java
